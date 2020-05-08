#include "iOpenGL.h"

HGLRC hRC;
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
		pfd.dwFlags =	PFD_DRAW_TO_WINDOW |
						PFD_SUPPORT_OPENGL |
						PFD_DOUBLEBUFFER;

		int pixelFormat = ChoosePixelFormat(hDC, &pfd);
		SetPixelFormat(hDC, pixelFormat, &pfd);

		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
	}
	else
	{
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_SMOOTH);
}

#include "iStd.h"
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

	glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, devSize.width, devSize.height, 0, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
#ifdef _DEBUG
	texNum++;
#endif
	return tex;
}

// glDeleteTextures(1, &texID);    // destroy texture
