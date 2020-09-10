#pragma once

#include <windows.h>
// GLEW_STATIC 전처리기에 정의.
#include <GL/glew.h>
#include <GL/wglew.h>
//#include <GL/GL.h>
//#include <GL/GLU.h>

// glew.c 파일을 프로젝트 추가.

#pragma comment(lib, "opengl32.lib")

#include "iMatrix.h"
extern iMatrix* mProjection;
extern iMatrix* mModelview;

void setupOpenGL(bool setup, HDC hDC);
bool startGLEW();
void initOpenGL();
void reshapeOpenGL(int width, int height);

void setAntiAliasParameters(bool anti);

extern GLuint vertexBuffer, vertexObject;
GLuint createShader(const char* str, GLuint flag);
void destroyShader(GLuint id);
GLuint createProgramID(GLuint vertID, GLuint fragID);
void destroyProgram(GLuint programID);

enum iGDIID{
	iGDIID_drawLine = 0,
	iGDIID_drawRect,
	iGDIID_fillRect,
	iGDIID_drawCircle,
	iGDIID_fillCircle,
	iGDIID_fillReverseCircle,
	iGDIID_drawEllipse,
	iGDIID_fillEllipse,

	iGDIID_Max
};

enum iBlend {
	iBlendAlpha = 0,
	iBlendGrey,
	iBlendAdd,

	iBlendMax
};
void setGLBlend(iBlend blend);
GLuint getProgramGdiID(iGDIID ids);
GLuint getProgramID();

GLuint nextPOT(GLuint x);
struct Texture;
Texture* createImageWithRGBA(GLubyte* rgba, GLuint width, GLuint height);

/*
	라이브러리 접근방법
	1. compile : header file include (*.obj, *.o)
	2. link : .lib include (archive *.obj, make lib or exe)
*/

