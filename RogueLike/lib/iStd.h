#pragma once

#include "iGraphics.h"
#include "iOpenGL.h"
#include "iType.h"

#include "iImage.h"
#include "iPopup.h"
#include "iStrTex.h"
/////////////////////////////////////////////////////////////////////////////////

#include "rgArray.h"
#include "../roguelike/Player.h"
#include "../roguelike/RgTile.h"
#include "../roguelike/RgLoading.h"

/////////////////////////////////////////////////////////////////////////////////

#include "iSort.h"
#include "iShortestPath.h"

#include "iFPS.h"

#include "iOpenAL.h"

#if _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#define xprintf printf
#else
#define xprintf(...) (void)0
#endif

extern int monitorSizeW, monitorSizeH;
extern iSize devSize;
#define DEVSIZE_WIDTH 1920
#define DEVSIZE_HEIGHT 1080
extern iRect viewport;

void loadLib(HDC hDC);
void freeLib();
typedef void (*Method_Paint)(float dt);
void drawLib(Method_Paint method);
void keyLib(iKeyState stat, int c);
uint32 getKeyDown();
bool getKeyDown(uint32 key);
uint32 getKeyStat();
bool getKeyStat(uint32 key);
void resizeLib(int width, int height);
void zoomLib(iPoint point, float rate);

class iFBO
{
public:
    iFBO(int width, int height);
    virtual ~iFBO();

    void clear(float r, float g, float b, float a);
    void bind();
    void bind(Texture* tex);
    void unbind();

    Texture* getTexture();
    uint32 bindingTexID();

public:
    GLuint fbo;

    GLuint depthBuffer;

    Texture* tex;// colorBuffer

    GLint prevFbo;// multiple fbo 고려
    Texture** listTex;
    int listNum;
};
extern iFBO* fbo;

uint8 float2uint8(float f);
void setRGBA(float r, float g, float b, float a);
void getRGBA(float& r, float& g, float& b, float& a);

void setLineWidth(float lineWidth);
void drawLine(iPoint sp, iPoint ep);
void drawLine(float x0, float y0, float x1, float y1);

void drawRect(float x, float y, float width, float height, float radius=0.0f);
void drawRect(iRect rt, float radius = 0.0f);
void fillRect(float x, float y, float width, float height, float radius=0.0f);
void fillRect(iRect rt, float radius=0.0f);

void saveImageFromRGBA(const char* path, uint8* rgba, int width, int height);
uint8* bmp2rgba(Bitmap* bmp, int& width, int& height);

Texture* createTexture(int width, int height, bool rgba32f=false);
void setTexture(TextureWrap wrap, TextureFilter filter);

Texture* createImage(const char* szFormat, ...);
Texture* createGreyImage(const char* szFormat, ...);
Texture* createReflectImage(float rateY, const char* szFormat, ...);
Texture** createDivideImage(int wNum, int hNum, const char* szFormat, ...);
void freeImage(Texture* tex);

void drawImageLikeCircle(Texture* tex, int x, int y, float rate);
void drawImage(Texture* tex, int x, int y, int anc);
void drawImage(Texture* tex, int x, int y,
    int ix, int iy, int iw, int ih, int anc,
    float ratX, float ratY, int xyz, float degree,
    int reverse=REVERSE_NONE);

iRect getHitBoxRect(Texture* tex, int x, int y,
    int ix, int iy, int iw, int ih, int anc,
    float ratX, float ratY, int xyz, int degree);

void setClip(int x, int y, int width, int height);
void setClip(iRect rect);

void setStringName(const char* name);
const char* getStringName();
void setStringSize(float size);
float getStringSize();
void setStringRGBA(float r, float g, float b, float a);
void getStringRGBA(float& r, float& g, float& b, float& a);
void setStringBorder(float size);
float getStringBorder();
void setStringBorderRGBA(float r, float g, float b, float a);
void getStringBorderRGBA(float& r, float& g, float& b, float& a);
void drawString(int x, int y, int anc, const char* szFormat, ...);
iSize sizeOfString(const char* szFormat, ...);

uint32 random();
float _sin(float degree);
float _cos(float degree);
float linear(float r, float a, float b);
iPoint linear(float r, iPoint a, iPoint b);
float easeIn(float r, float a, float b);
iPoint easeIn(float r, iPoint a, iPoint b);
float easeOut(float r, float a, float b);
iPoint easeOut(float r, iPoint a, iPoint b);
float easeInOut(float r, float a, float b);
iPoint easeInOut(float r, iPoint a, iPoint b);

bool containPoint(iPoint p, iRect rt);
bool containRect(iRect src, iRect dst);

char* loadFile(const char* filePath, int& length);
void saveFile(const char* filePath, char* buf, int bufLength);
