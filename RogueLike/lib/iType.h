#pragma once

// 정의 : 실체 없음. 헤더파일, 선언x(할당x) + Macro

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "iPoint.h"
#include "iSize.h"
#include "iRect.h"
#include "iColor.h"

#include "iArray.h"
#include "iString.h"

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

enum iKeyState {
    iKeyStateBegan = 0,
    iKeyStateMoved,
    iKeyStateEnded
};

#define keyboard_left       1
#define keyboard_right      2
#define keyboard_up         4
#define keyboard_down       8

#define keyboard_space      16
#define keyboard_i			32
#define keyboard_o		    64
#define keyboard_j			128

#define keyboard_r			256
#define keyboard_u          512
#define keyboard_tab		1024
#define keyboard_escape     2048

struct Texture {
    uint32 texID;
    float width, height;
    float potWidth, potHeight;
    int retainCount;
};

enum TextureWrap {
    CLAMP = 0,// GL_CLAMP_EDGE
    REPEAT,// GL_REPEAT
};

enum TextureFilter {
    LINEAR = 0,// GL_LINEAR
    NEAREST,// GL_NEAREST
    MIPMAP,// GL_LINEAR_MIPMAP_LINEAR
};

#define TOP     1
#define VCENTER 2
#define BOTTOM  4

#define LEFT    8
#define HCENTER 16
#define RIGHT   32

#define REVERSE_NONE    0
#define REVERSE_WIDTH   1
#define REVERSE_HEIGHT  2

#define PRINTF(val) printf("%.2f\n", val)
#define PRINTI(val) printf("%d\n", val)
#define PRINTS(val) printf("%s\n", val)
