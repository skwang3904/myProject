#include "iShaderToy.h"
#include "iStd.h"

iShaderToy::iShaderToy(STInput* si)
{
	int i, j;
	texBuf = (Texture***)malloc(sizeof(Texture**) * 4);
	programID = (GLuint*)calloc(sizeof(GLuint), 5);
	texBufChannel = (Texture***)malloc(sizeof(Texture**) * 5);
	setbufChannel = (GLenum***)malloc(sizeof (GLenum**) * 5);
	bufIndex = (int8**)malloc(sizeof(int8*) * 5);

	int lenCommon;
	char* strCommon = loadFile(si->strCommonPath, lenCommon);

#if 1
	const char* strEnd[2] = {
			"void main()							\
			{										\
				mainImage(fragColor, gl_FragCoord.xy);	\
			}",

			"void main()							\
			{										\
				mainImage(fragColor, gl_FragCoord.xy);	\
				fragColor.a = 1.0;					\
			}",
	};
#else
	const char* strEnd[2] = {
		"",""
	};
#endif
	int lenEnd = strlen(strEnd[1]);

	for (i = 0; i < 5; i++)
	{
		STBuf* input = &si->sb[i];
		if (input->strVertPath == NULL ||
			input->strFragPath == NULL)
			continue;

		int length;
		char* str = loadFile(input->strVertPath, length);
		GLuint vertID = createShader(str, GL_VERTEX_SHADER);
		free(str);

		str = loadFile(input->strFragPath, length);
		char* s = (char*)calloc(sizeof(char), lenCommon + length + lenEnd + 3);
		sprintf(s, "%s\n%s\n%s", strCommon, str, strEnd[i < 4 ? 0 : 1]);

		GLuint fragID = createShader(s, GL_FRAGMENT_SHADER);
		free(str);
		free(s);
		
		programID[i] = createProgramID(vertID, fragID);

		destroyShader(vertID);
		destroyShader(fragID);

		if (i < 4)
		{
			texBuf[i] = (Texture**)malloc(sizeof(Texture*) * 2);
			for (j = 0; j < 2; j++)
				texBuf[i][j] = createTexture(devSize.width, devSize.height, true);
		}
	}

	for (i = 0; i < 5; i++)
	{
		STBuf* input = &si->sb[i];
		if (input->strVertPath == NULL ||
			input->strFragPath == NULL)
			continue;

		texBufChannel[i] = (Texture**)calloc(sizeof(Texture*), 4);
		setbufChannel[i] = (GLenum**)malloc(sizeof(GLenum*) * 4);
		bufIndex[i] = (int8*)malloc(sizeof(int8) * 4);
		for (j = 0; j < 4; j++)
		{
			bufIndex[i][j] = input->channel[j].buf;
			if (bufIndex[i][j] != -1)
			{
				//texBufChannel[i][j] = texBuf[buf];
				//texBuf[buf]->retainCount++;
			}
			else if (input->channel[j].strPath)
			{
#if 1
				if (input->channel[j].vFlip)
					texBufChannel[i][j] = createReverseImage(input->channel[j].strPath);
				else
					texBufChannel[i][j] = createImage(input->channel[j].strPath);
#else
				texBufChannel[i][j] = createImage(input->channel[j].strPath);
#endif

			}

			setbufChannel[i][j] = (GLenum*)calloc(sizeof(GLenum), 2);

			setbufChannel[i][j][0] = input->channel[j].wrap;
			setbufChannel[i][j][1] = input->channel[j].filter;
		}
	}

	toggle = true;
	takeTime = 0.0f;
	frame = 0;
	
	memset(mouse, 0x00, sizeof(float) * 4);
	memset(keyboard, 0x00, sizeof(bool) * 5);
}

iShaderToy::~iShaderToy()
{
	int i, j;
	for (i = 0; i < 5; i++)
	{
		if (programID[i])
			destroyProgram(programID[i]);
		
		for (int j = 0; j < 4; j++)
		{
			if (texBufChannel[i][j])
				freeImage(texBufChannel[i][j]);
		}
		free(texBufChannel[i]);
		free(bufIndex[i]);
	}
	free(texBufChannel);
	free(bufIndex);
	free(programID);

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 2; j++)
		{
			if (texBuf[i][j])
				freeImage(texBuf[i][j]);
		}
		free(texBuf[i]);
	}
	free(texBuf);
}

extern iVBO* gVbo;
void iShaderToy::paint(float dt)
{
	int i, j;
	char s[64];

	for (i = 0; i < 5; i++)
	{
		GLuint pID = programID[i];
		if (pID == 0) continue;
		if (i < 4)
			fbo->bind(texBuf[i][toggle]);

		glViewport(0, 0, devSize.width, devSize.height);
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(pID);

		float p[4][4] = {
			{-1, 1, 0, 1},		{1, 1, 0, 1},
			{-1,-1, 0, 1},		{1,-1, 0, 1}
		};

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

		GLuint posAttr = glGetAttribLocation(pID, "position");
		glEnableVertexAttribArray(posAttr);
		glVertexAttribPointer(posAttr, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glUniform3f(glGetUniformLocation(pID, "iResolution"), devSize.width, devSize.height, 0);
		glUniform1f(glGetUniformLocation(pID, "iTime"), takeTime);
		glUniform1f(glGetUniformLocation(pID, "iTimeDelta"), dt);
		glUniform1i(glGetUniformLocation(pID, "iFrame"), frame);
		glUniform4fv(glGetUniformLocation(pID, "iMouse"), 1, mouse);


		for (j = 0; j < 4; j++)
		{
			Texture* tex = NULL;// = texBufChannel[i][j];
			if (bufIndex[i][j] != -1)
				tex = texBuf[bufIndex[i][j]][!toggle];
			else
				tex = texBufChannel[i][j];

			float channelResolution[3] = { 0, 0, 0 };
			if (tex)
			{
				sprintf(s, "iChannel%d", j);
				GLuint tID = glGetUniformLocation(pID, s);
				glUniform1i(tID, j);
				glActiveTexture(GL_TEXTURE0 + j);
				glBindTexture(GL_TEXTURE_2D, tex->texID);
				setTexture((TextureWrap)setbufChannel[i][j][0],
					(TextureFilter)setbufChannel[i][j][1]);

				channelResolution[0] = tex->width;
				channelResolution[1] = tex->height;
			}
			char s[32];
			sprintf(s, "iChannelResolution[%d]", j);
			glUniform3fv(glGetUniformLocation(pID, s), 1, channelResolution);
		}
		
		for (j = 0; j < 9; j++)
		{
			char s[32];
			sprintf(s, "iKeyboard[%d]", j);
			glUniform1i(glGetUniformLocation(pID, s), keyboard[j]);
		}

		uint8 indices[6] = {0,1,2, 1,2,3};
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

		glDisableVertexAttribArray(posAttr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		for (j = 0; j < 4; j++)
		{
			Texture* tex = texBufChannel[i][j];
			if (tex)
			{
				glActiveTexture(GL_TEXTURE0 + j);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		if (i < 4)
			fbo->unbind();
	}

	takeTime += dt;
	frame++;

	toggle = !toggle;

	uint32 key = getKeyStat();
	keyboard[0] = (key & keyboard_left);
	keyboard[1] = (key & keyboard_right);
	keyboard[2] = (key & keyboard_up);
	keyboard[3] = (key & keyboard_down);
	keyboard[4] = (key & keyboard_space);
	keyboard[5] = 0;
	keyboard[6] = 0;
	keyboard[7] = 0;
	keyboard[8] = 0;
}


void iShaderToy::key(iKeyState stat, iPoint point)
{
	if (stat == iKeyStateBegan)
	{
		mouse[0] = mouse[2] = point.x;
		mouse[1] = mouse[3] = devSize.height - point.y;
	}

	else if (stat == iKeyStateMoved)
	{
		//mouse[0] = point.x;
		//mouse[1] = devSize.height - point.y;
	}
	else if (stat == iKeyStateEnded)
	{
		mouse[2] *= -1.0f;
		mouse[3] *= -1.0f;
	}
}

iShaderToy* shaderToy = NULL;
void keyShader(iKeyState stat, iPoint point)
{
	shaderToy->key(stat, point);
}