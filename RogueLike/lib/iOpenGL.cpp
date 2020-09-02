#include "iOpenGL.h"
#include "iStd.h"

HGLRC hRC;

iMatrix* mProjection;
iMatrix* mModelview;

void loadShader();
void freeShader();

void setupOpenGL(bool setup, HDC hDC)
{
	if (setup)
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0x00, sizeof(PIXELFORMATDESCRIPTOR));

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER;

		int pixelFormat = ChoosePixelFormat(hDC, &pfd);
		SetPixelFormat(hDC, pixelFormat, &pfd);

#if 0
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 1024, 768, 0, -1000, 1000);

		float m[16];
		glGetFloatv(GL_PROJECTION_MATRIX, m);
		printf("=============================glOrtho\n");
		for (int i = 0; i < 16; i++)
		{
			printf("%f, ", m[i]);
			if (i % 4 == 3)
				printf("\n");
		}
		printf("============================= mProjection\n");

		float* d = mProjection->d();
		for (int i = 0; i < 16; i++)
		{
			printf("%f, ", d[i]);
			if (i % 4 == 3)
				printf("\n");
		}


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

#endif		

		if (wglewIsSupported("WGL_ARB_create_context"))
		{
			int attr[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3, // 3 : vbo, vao, pipeline
				WGL_CONTEXT_MINOR_VERSION_ARB, 2,
				WGL_CONTEXT_FLAGS_ARB, 0,
				0,
			};

			hRC = wglCreateContextAttribsARB(hDC, NULL, attr);
		}
		else
		{
			hRC = wglCreateContext(hDC);
		}
		wglMakeCurrent(hDC, hRC);

		mProjection = new iMatrix;
		mModelview = new iMatrix;
	}
	else
	{
		freeShader();

		delete mProjection;
		delete mModelview;

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
	}
}


bool startGLEW()
{
	glewExperimental = true;
	GLenum error = glewInit();
	if (error != GLEW_OK)
		return false;
	return true;
}

void initOpenGL()
{
	loadShader();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// GL_MODULATE / GL_BLEND_SRC
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);

#if 0
	glDisable(GL_DEPTH_TEST);
#else
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL); 
	glDepthFunc(GL_ALWAYS);
	glClearDepth(1.0f);
#endif

	glEnable(GL_BLEND);

	// Stright Alpha : 2D
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // alpha == 1 일때만 정확
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); 
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	// =>칼라와 알파를 세부적으로 나누어서 블렌딩
	// color : bend.rgb * blend.rgb + base.rgb * (1.0 - blend.a)
	// alpha : blend.a * blend.a + base.a * (1.0 - blend.a)

	// Pre-multiplay Alpha : 3D
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); 
	// => 쉐이더(alpha)에서 수정해서 rgba값을 넘김
	// color : bend.rgb + base.rgb * (1.0 - blend.a)

	glEnable(GL_SMOOTH);
}

GLuint vertexBuffer, vertexObject;
GLuint* programIDs;
GLuint* programGDIs;
GLuint programID;

void loadShader()
{
	int i;

	//vao
	glGenVertexArrays(1, &vertexObject);
	glBindVertexArray(vertexObject);

	//vbo
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(iVertex) * 4, NULL, GL_STATIC_DRAW);

	programIDs = (GLuint*)malloc(sizeof(GLuint) * iBlendMax);

	int length;
	char* str = loadFile("assets/shader/std.vert", length);
	GLuint vertexID = createShader(str, GL_VERTEX_SHADER);
	free(str);

	const char* strFragList[iBlendMax] = {
		"alpha", "grey", "add"
	};

	for (i = 0; i < iBlendMax; i++)
	{
		char s[256];
		sprintf(s, "assets/shader/%s.frag", strFragList[i]);
		str = loadFile(s, length);
		GLuint fragID = createShader(str, GL_FRAGMENT_SHADER);
		free(str);

		programIDs[i] = createProgramID(vertexID, fragID);
		destroyShader(fragID);
	}
	destroyShader(vertexID);

	setGLBlend(iBlendAlpha);

	{
		programGDIs = (GLuint*)malloc(sizeof(GLuint) * iGDIID_Max);

		str = loadFile("assets/shader/gdi/gdi.vert", length);
		vertexID = createShader(str, GL_VERTEX_SHADER);
		free(str);

		const char* strGdiList[iGDIID_Max] = {
			"drawLine",
			"drawRect","fillRect",
			"drawCircle","fillCircle",
			"drawEllipse","fillEllipse",
		};

		for (i = 0; i < iGDIID_Max; i++)
		{
			char s[256];
			sprintf(s, "assets/shader/gdi/%s.frag", strGdiList[i]);
			str = loadFile(s, length);
			GLuint fragID = createShader(str, GL_FRAGMENT_SHADER);
			free(str);

			programGDIs[i] = createProgramID(vertexID, fragID);
			destroyShader(fragID);
		}
		destroyShader(vertexID);
	}
}

void freeShader()
{
	//vao
	glDeleteVertexArrays(1, &vertexObject);

	//vbo
	glDeleteBuffers(1, &vertexBuffer);

	for (int i = 0; i < iBlendMax; i++)
		destroyProgram(programIDs[i]);
	free(programIDs);

	for (int i = 0; i < iGDIID_Max; i++)
		destroyProgram(programGDIs[i]);
	free(programGDIs);
}

void checkShaderID(GLuint id)
{
	GLint result = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE)
	{
		int length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* s = (char*)calloc(sizeof(char), 1 + length);
		glGetShaderInfoLog(id, length, NULL, s);

		wchar_t* ws = utf8_to_utf16(s);
		MessageBox(NULL, ws, TEXT("glCreateShader error"), MB_OK);
		free(ws);
		free(s);
	}
}

GLuint createShader(const char* str, GLuint flag)
{
	GLuint id = glCreateShader(flag);
	glShaderSource(id, 1, &str, NULL);
	glCompileShader(id);

	checkShaderID(id);

	return id;
}

void destroyShader(GLuint id)
{
	glDeleteShader(id);
}

void checkProgramID(GLuint id)
{
	GLint result = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &result);

	if (result != GL_TRUE)
	{
		int length = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
		char* s = (char*)calloc(sizeof(char), 1 + length);
		glGetProgramInfoLog(id, length, NULL, s);

		wchar_t* ws = utf8_to_utf16(s);
		MessageBox(NULL, ws, TEXT("glCreateProgramID error"), MB_OK);
		free(ws);
		free(s);
	}
}

GLuint createProgramID(GLuint vertID, GLuint fragID)
{
	GLuint id = glCreateProgram();
	glAttachShader(id, vertID);
	glAttachShader(id, fragID);

	glLinkProgram(id);

	glDetachShader(id, vertID);
	glDetachShader(id, fragID);

	checkProgramID(id);

	return id;
}

void destroyProgram(GLuint programID)
{
	glDeleteProgram(programID);
}

void setGLBlend(iBlend blend)
{
	programID = programIDs[blend];
}

GLuint getProgramGdiID(iGDIID ids)
{
	return programGDIs[ids];
}

GLuint getProgramID()
{
	return programID;
}

extern int monitorSizeW, monitorSizeH;

void checkOpenGL()
{
	const GLubyte* strGL = glGetString(GL_VERSION);
	const GLubyte* strGLEW = glewGetString(GLEW_VERSION);
	const GLubyte* strGLSL = glGetString(GL_SHADING_LANGUAGE_VERSION);

	printf("strGL[%s] strGLEW[%s] strGLSL[%s]\n", strGL, strGLEW, strGLSL);

	GLfloat matrix[2][16];
	glGetFloatv(GL_PROJECTION_MATRIX, matrix[0]);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix[1]);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			GLfloat* m = &matrix[i][4*j];
			printf("%f\t%f\t%f\t%f\n", m[0], m[1], m[2], m[3]);
		}
	}

	GLint maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	printf("maxTextureSize = %d\n", maxTextureSize);

	GLenum error = glGetError();
	const GLubyte* strError = glewGetErrorString(error);
	printf("strError[%s]\n", strError);

	static Texture* tex = createImage("assets/ex.png");
	glBindTexture(GL_TEXTURE_2D, tex->texID);
	GLint w, h, format;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);// GL_RGBA
	GLint texID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &texID);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLint fbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
}

void reshapeOpenGL(int width, int height)
{
	float r0 = devSize.width / devSize.height;

	monitorSizeW = width;
	monitorSizeH = height;
	float r1 = 1.0f * width / height;

	if (r0 < r1)// 모니터 가로가 큰 경우
	{
		viewport.origin.y = 0; viewport.size.height = height;
		viewport.size.width = devSize.width * height / devSize.height;
		viewport.origin.x = (width - viewport.size.width) / 2;
	}
	else if (r0 > r1)// 모니터 가로가 작은 경우(세로 긴 경우)
	{
		viewport.origin.x = 0; viewport.size.width = width;
		viewport.size.height = devSize.height * width / devSize.width;
		viewport.origin.y = (height - viewport.size.height) / 2;
	}
	else// 정비율
	{
		viewport = iRectMake(0, 0, width, height);
	}

	//glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
	//mProjection->loadIdentity();
	//mProjection->ortho(0, devSize.width, devSize.height, 0, -1000, 1000);
	//mModelview->loadIdentity();
}

GLuint nextPOT(GLuint x)
{
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x + 1;
}

struct xTexParam {
	GLuint minFilter;
	GLuint magFilter;
	GLuint wrapS;
	GLuint wrapT;
};

static xTexParam texParam = { GL_NEAREST, GL_NEAREST ,
		GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

void setAntiAliasParameters(bool anti)
{
	if (anti == false)
	{
		texParam.minFilter = GL_NEAREST;
		texParam.magFilter = GL_NEAREST;
	}
	else
	{
		texParam.minFilter = GL_LINEAR;
		texParam.magFilter = GL_LINEAR;
	}
}

void applyTexParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParam.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParam.magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParam.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParam.wrapT);
}

Texture* createImageWithRGBA(GLubyte* rgba, GLuint width, GLuint height)
{
	GLuint texID;
	glGenTextures(1, &texID);       // create texture
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);// use

	applyTexParameters();

	int potWidth = nextPOT(width), potHeight = nextPOT(height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		potWidth, potHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	Texture* tex = (Texture*)malloc(sizeof(Texture));
	tex->texID = texID;
	tex->width = width;
	tex->height = height;
	tex->potWidth = potWidth;
	tex->potHeight = potHeight;
	tex->retainCount = 1;


	return tex;
}

// glDeleteTextures(1, &texID);    // destroy texture
