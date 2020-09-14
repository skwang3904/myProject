#pragma once

#include "iType.h"
#include "iOpenGL.h"

struct STChannel
{
	int buf = -1; // buf A, B, C, D
	const char* strPath = NULL;
	TextureWrap wrap = CLAMP;
	TextureFilter filter = LINEAR;
	bool vFlip = false;
};

struct STBuf // A, B, C, D, Image
{
	const char* strVertPath = NULL;
	const char* strFragPath = NULL;
	STChannel channel[4];
};

struct STInput
{
	const char* strCommonPath;
	STBuf sb[5];
};

class iShaderToy 
{
public:
	iShaderToy(STInput* si);
	virtual ~iShaderToy();

	void paint(float dt);
	void key(iKeyState stat, iPoint point);

public:
	Texture*** texBuf; // 4 * 2 using fbo
	GLuint* programID; // 5
	Texture*** texBufChannel; // 5 * 4
	int8** bufIndex; // 5 * 4
	GLenum*** setbufChannel; // 5 * 4 * 2

	bool toggle;

	float takeTime;
	int frame;

	float mouse[4];
	bool keyboard[9];
};
extern iShaderToy* st;

void keyShader(iKeyState stat, iPoint point);