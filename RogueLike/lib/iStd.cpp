#include "iStd.h"

uint8 float2uint8(float f)
{
    return f * 0xFF;
}

int monitorSizeW, monitorSizeH;
iSize devSize;
iRect viewport;
static float _r, _g, _b, _a;

char* _stringName;
float _stringSize;
float _stringR, _stringG, _stringB, _stringA;
float _stringBorder;
float _stringBorderR, _stringBorderG, _stringBorderB, _stringBorderA;

DWORD prevTickCount;

static bool* keys;
static uint32 keyDown, keyStat;
uint32 getKeyDown() { return keyDown; }
bool getKeyDown(uint32 key) { return keyDown & key; }
uint32 getKeyStat() { return keyStat; }
bool getKeyStat(uint32 key) { return keyStat & key; }

iVBO* gVbo = NULL;
Texture** texGdi;
void loadLib(HDC hDC)
{
    setupOpenGL(true, hDC);
    startGLEW();
    initOpenGL();

    devSize = iSizeMake(DEVSIZE_WIDTH, DEVSIZE_HEIGHT);
    fbo = new iFBO(devSize.width, devSize.height);
    texFboForiPopup = createTexture(devSize.width, devSize.height);
    reshapeOpenGL(monitorSizeW, monitorSizeH);// App.cpp

    _r = 1.0f;
    _g = 1.0f;
    _b = 1.0f;
    _a = 1.0f;

    _stringName = NULL;
    setStringName("궁서체");
    setStringSize(10);
    setStringRGBA(1, 1, 1, 1);
    setStringBorder(1);
    setStringBorderRGBA(0, 0, 0, 1);

    glEnable(GL_LINE_SMOOTH);

    prevTickCount = GetTickCount();

    keys = (bool*)calloc(sizeof(bool), 256);
    keyDown = 0;
    keyStat = 0;

    srand(time(NULL));
    void sRandom();
    sRandom();

    gVbo = new iVBO();
    gVbo->programID = getProgramID();

    texGdi = (Texture**)malloc(sizeof(Texture*) * 2);

    iPoint p = iPointMake(32, 32);
    Texture* tex = createTexture(p.x, p.y);
    setTexture(CLAMP, MIPMAP);
    fbo->bind(tex); //
    fillCircle(16, 16, 16);
    fbo->unbind(); //
    texGdi[0] = tex;

    tex = createTexture(p.x, p.y);
    fbo->bind(tex); //
    fbo->clear(1, 1, 1, 1);
    fbo->unbind(); //
    texGdi[1] = tex;
}

void freeLib()
{
    delete fbo;
    freeImage(texFboForiPopup);
    setupOpenGL(false, NULL);

    free(keys);

    for (int i = 0; i < 2; i++)
        freeImage(texGdi[i]);
    free(texGdi);

    iGraphics* g = iGraphics::instance();
    delete g;
}

iPoint zoomPosition;
float zoomRate;
#define _zoomDt 0.5f
float zoomDt = _zoomDt;

void drawLib(Method_Paint method)
{
    DWORD d = GetTickCount();
    float delta = (d - prevTickCount)/1000.f;
    prevTickCount = d;

    // --------------------
    fbo->bind();
    fbo->clear(0, 0, 0, 1);

    method(delta);
    keyDown = 0;

    extern void drawCursor(float dt);
    drawCursor(0.0f);
    fbo->unbind();
    // ------------------

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    setRGBA(1, 1, 1, 1);
    Texture* tex = fbo->getTexture();

    if (zoomDt < _zoomDt)
    {
        float r = 1.0f + (zoomRate - 1.0f) * _sin(180 * zoomDt / _zoomDt);
        zoomDt += delta;
        iPoint c = iPointMake(devSize.width / 2, devSize.height / 2);
        iPoint dp = zoomPosition - c;
        iPoint rp = dp * r;
        c -= rp - dp;

        drawImage(tex, c.x, c.y,
            0, 0, tex->width, tex->height, VCENTER | HCENTER,
            r, r, 2, 0, REVERSE_HEIGHT);
    }
    else
    {
        //drawImage(tex, devSize.width / 2, devSize.height / 2,
        //    0, 0, tex->width, tex->height, VCENTER | HCENTER,
        //    2.0f, 2.0f, 2, 0, REVERSE_HEIGHT);
#if 1
		drawImage(tex, viewport.origin.x * 0.5f + viewport.size.width / 2.0f, viewport.origin.y * 0.5f + viewport.size.height / 2.0f,
			0, 0, tex->width, tex->height, 
            VCENTER | HCENTER,	(viewport.size.width - viewport.origin.x) / tex->width, (viewport.size.height - viewport.origin.y) / tex->height,
            2, 0, REVERSE_HEIGHT);
#else
        drawImage(tex, viewport.size.width / 2, viewport.size.height / 2,
            0, 0, tex->width, tex->height,
            VCENTER | HCENTER, 1.0f, 1.0f,
            2, 0, REVERSE_HEIGHT);
#endif
    }

}

static void keyLib(uint32& key, iKeyState stat, int c)
{
    if (stat == iKeyStateBegan)
    {
        switch (c) {
            // 이동 조작------------------------------------
        case 'a':case 'A': 
        case VK_LEFT:   key |= keyboard_left; break;
        case 'd':case 'D':
        case VK_RIGHT:  key |= keyboard_right; break;
        case 'w':case 'W':
        case VK_UP:     key |= keyboard_up; break;
        case 's':case 'S':
        case VK_DOWN:   key |= keyboard_down; break;

            //회피------------------------------------
        case VK_SPACE:  key |= keyboard_space; break;

            //상호작용 조작------------------------------------
        case 'i': case 'I': key |= keyboard_i; break;
        case 'o': case 'O': key |= keyboard_o; break;
		case 'j': case 'J': key |= keyboard_j; break;
        case 'r': case 'R': key |= keyboard_r; break;
        case 'u': case 'U': key |= keyboard_u; break;
		case VK_TAB:  key |= keyboard_tab; break;
        case VK_ESCAPE: key |= keyboard_escape; break;
        }
    }
    else if (stat == iKeyStateEnded)
    {
        switch (c) {
            // 이동 조작------------------------------------
        case 'a':case 'A':
        case VK_LEFT:   key &= ~keyboard_left; break;
        case 'd':case 'D':
        case VK_RIGHT:  key &= ~keyboard_right; break;
        case 'w':case 'W':
        case VK_UP:     key &= ~keyboard_up; break;
        case 's':case 'S':
        case VK_DOWN:   key &= ~keyboard_down; break;

            //회피------------------------------------
        case VK_SPACE:  key &= ~keyboard_space; break;

            //상호작용 조작------------------------------------
        case 'i': case 'I': key &= ~keyboard_i; break;
        case 'o': case 'O': key &= ~keyboard_o; break;
		case 'j': case 'J': key &= ~keyboard_j; break;
		case 'r': case 'R': key &= ~keyboard_r; break;
        case 'u': case 'U': key &= ~keyboard_u; break;
		case VK_TAB: key &= ~keyboard_r; break;
        case VK_ESCAPE: key &= ~keyboard_escape; break;
        }
    }
}

void keyLib(iKeyState stat, int c)
{
    if (stat == iKeyStateBegan)
    {
        //xprintf("%d\n", c);
        if (keys[c]==false)
            keyLib(keyDown, stat, c);
        keys[c] = true;
    }
    else if (stat == iKeyStateEnded)
    {
        keys[c] = false;
    }
    keyLib(keyStat, stat, c);
}

void resizeLib(int width, int height)
{
    reshapeOpenGL(width, height);
}

void zoomLib(iPoint point, float rate)
{
    zoomPosition = point;
    zoomRate = rate;
    zoomDt = 0.0f;
}

GLuint vbe = NULL;
iVBO::iVBO(int qNum_)
{
    _qNum = qNum_;
    qNum = 0;
    q = (iQuad*)malloc(sizeof(iQuad) * _qNum);
    indices = (short*)malloc(sizeof(short) * 6 * _qNum);

    for (int i = 0; i < _qNum; i++)
    {
        iQuad* quad = &q[i];
        quad->tl.p[2] = 0; quad->tl.p[3] = 1;
        quad->tr.p[2] = 0; quad->tr.p[3] = 1;
        quad->bl.p[2] = 0; quad->bl.p[3] = 1;
        quad->br.p[2] = 0; quad->br.p[3] = 1;

        quad->tl.uv = iPointMake(0, 0);
        quad->tr.uv = iPointMake(1, 0);
        quad->bl.uv = iPointMake(0, 1);
        quad->br.uv = iPointMake(1, 1);

        indices[6 * i + 0] = 4 * i + 0;
        indices[6 * i + 1] = 4 * i + 1;
        indices[6 * i + 2] = 4 * i + 2;
        indices[6 * i + 3] = 4 * i + 1;
        indices[6 * i + 4] = 4 * i + 2;
        indices[6 * i + 5] = 4 * i + 3;
    }

    glGenBuffers(1, &vbe);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * 6 * _qNum, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    tex = NULL;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(iQuad) * _qNum, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    programID = 0;
}

iVBO::~iVBO()
{
    free(q);
    free(indices);

    freeImage(tex);
    glDeleteBuffers(1, &vbo);
}

void iVBO::paint(float dt)
{
    glUseProgram(programID);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(iQuad) * qNum, q);

    GLuint positionAttr = glGetAttribLocation(programID, "position");
    glEnableVertexAttribArray(positionAttr);
    glVertexAttribPointer(positionAttr, 4, GL_FLOAT, GL_FALSE, sizeof(iVertex), (GLvoid*)0);

    GLuint texCoordAttr = glGetAttribLocation(programID, "texCoord");
    glEnableVertexAttribArray(texCoordAttr);
    glVertexAttribPointer(texCoordAttr, 2, GL_FLOAT, GL_FALSE, sizeof(iVertex), (GLvoid*)offsetof(iVertex, uv));

    GLuint colorAttr = glGetAttribLocation(programID, "color");
    glEnableVertexAttribArray(colorAttr);
    glVertexAttribPointer(colorAttr, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(iVertex), (GLvoid*)offsetof(iVertex, c));

    GLuint tID = glGetUniformLocation(programID, "texBlend");
    glUniform1i(tID, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->texID);

    GLuint mProj = glGetUniformLocation(programID, "mProjection");
    glUniformMatrix4fv(mProj, 1, GL_FALSE, mProjection->d());
    GLuint mModelv = glGetUniformLocation(programID, "mModelview");
    glUniformMatrix4fv(mModelv, 1, GL_FALSE, mModelview->d());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
    glDrawElements(GL_TRIANGLES, 6 * qNum, GL_UNSIGNED_SHORT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(positionAttr);
    glDisableVertexAttribArray(texCoordAttr);
    glDisableVertexAttribArray(colorAttr);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


iFBO* fbo = NULL;
iFBO::iFBO(int width, int height)
{
    // renderBuffer(depthBuffer)
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // colorBuffer
    tex = createTexture(width, height, false);
    GLuint texID = tex->texID;

    // frameBuffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GLenum fboBuffs[1] = { GL_COLOR_ATTACHMENT0, };
    glDrawBuffers(1, fboBuffs);//glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (stat != GL_FRAMEBUFFER_COMPLETE)
        printf("checkFrameBufferStatus( %d, %d )\n", stat, GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    prevFbo = 0;
    listTex = (Texture**)malloc(sizeof(Texture*) * 10);
    listNum = 0;

    size = iSizeMake(width, height);
}

iFBO::~iFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDeleteRenderbuffers(1, &depthBuffer);
    freeImage(tex);

    glDeleteFramebuffers(1, &fbo);

    free(listTex);
}

void iFBO::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void iFBO::bind()
{
    bind(tex);
}

void iFBO::bind(Texture* tex)
{
    if (listNum == 0)
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
    listTex[listNum] = tex;
    listNum++;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GLenum fboBuffs[1] = { GL_COLOR_ATTACHMENT0, };
    glDrawBuffers(1, fboBuffs); //glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texID, 0);

    glViewport(0, 0, tex->width, tex->height);

    mProjection->loadIdentity();
    mProjection->ortho(0, tex->width, tex->height, 0, -1000, 1000);
    mModelview->loadIdentity();
    devSize = iSizeMake(tex->width, tex->height);
}

void iFBO::unbind()
{
    mProjection->loadIdentity();
    mModelview->loadIdentity();

    listNum--;
    if (listNum)
    {
        bind(listTex[listNum - 1]);
        listNum--;
        devSize = size;
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
        glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
        mProjection->ortho(viewport.origin.x, viewport.size.width, viewport.size.height, viewport.origin.y, -1000, 1000);
        devSize = size;
    }

#if 0
    mProjection->loadIdentity();
    mProjection->ortho(viewport.origin.x, viewport.size.width, viewport.size.height, viewport.origin.y, -1000, 1000);
    mModelview->loadIdentity();
#endif
}

Texture* iFBO::getTexture()
{
    return tex;
}

uint32 iFBO::bindingTexID()
{
    return 0;
}

void setRGBA(float r, float g, float b, float a)
{
    _r = r;
    _g = g;
    _b = b;
    _a = a;
}

void getRGBA(float& r, float& g, float& b, float& a)
{
    r = _r;
    g = _g;
    b = _b;
    a = _a;
}

static float _lineWidth = 1.0f;
void setLineWidth(float lineWidth)
{
    _lineWidth = lineWidth;
    //glLineWidth(lineWidth);
}

static float _angle = 0.0f;
void setDrawAngle(float angle)
{
    _angle = angle;
}

void drawLine2(float x1, float y1, float x2, float y2)
{
    GLuint proID = getProgramGdiID(iGDIID_drawLine);
    glUseProgram(proID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    float lineWidth = _lineWidth;
    float lw = lineWidth / 2.0f;

    iPoint sp = iPointMake(x1, y1);
    iPoint ep = iPointMake(x2, y2);
    float theta = iPointAngle(iPointMake(1, 0), iPointZero, ep - sp);
    float d = iPointDistance(sp, ep) / 2.0f;

    float p[4][4] = {
        {-d - lw, lw, 0, 1},       {d + lw, lw, 0, 1},
        {-d - lw, -lw, 0, 1},      {d + lw, -lw, 0, 1}
    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

    GLuint positionAttr = glGetAttribLocation(proID, "position");
    glEnableVertexAttribArray(positionAttr);
    glVertexAttribPointer(positionAttr, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint mProj = glGetUniformLocation(proID, "mProjection");
    glUniformMatrix4fv(mProj, 1, GL_FALSE, mProjection->d());

    iMatrix mv;
    mv.loadIdentity();
    iPoint c = (sp + ep) / 2;
    mv.translate(c.x, c.y, 0);
    mv.rotate(0, 0, 1, theta);
    GLuint mModelv = glGetUniformLocation(proID, "mModelview");
    glUniformMatrix4fv(mModelv, 1, GL_FALSE, mv.d());

    GLuint uColor = glGetUniformLocation(proID, "color");
    glUniform4f(uColor, _r, _g, _b, _a);

    GLuint uSp = glGetUniformLocation(proID, "sp");
    sp.y = devSize.height - sp.y;
    glUniform2fv(uSp, 1, (float*)&sp);

    GLuint uEp = glGetUniformLocation(proID, "ep");
    ep.y = devSize.height - ep.y;
    glUniform2fv(uEp, 1, (float*)&ep);

    GLuint uLineWidth = glGetUniformLocation(proID, "lineWidth");
    glUniform1f(uLineWidth, lineWidth);

    uint8 indices[6] = { 0,1,2 , 1,2,3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);


    glDisableVertexAttribArray(positionAttr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawLine(iPoint sp, iPoint ep)
{
#if 1 // cw
    float theta = iPointAngle(iPointMake(1, 0), iPointZero, ep - sp);

    float cosT = _cos(theta);
    float sinT = _sin(theta);
    // cos  -sin
    // sin   cos
    float lw = _lineWidth / 2.0f;
    iPoint tl = iPointMake(-lw * cosT + lw * sinT, -lw * sinT - lw * cosT);
    iPoint tc = iPointMake(lw * sinT, -lw * cosT);
    iPoint tr = iPointMake(lw * cosT + lw * sinT, lw * sinT - lw * cosT);

    iPoint bl = iPointMake(-lw * cosT - lw * sinT, -lw * sinT + lw * cosT);
    iPoint bc = iPointMake(-lw * sinT, lw * cosT);
    iPoint br = iPointMake(lw * cosT - lw * sinT, lw * sinT + lw * cosT);
#else // ccw
    float theta = 360 - iPointAngle(iPointMake(1, 0), iPointZero, ep - sp);

    float cosT = _cos(theta);
    float sinT = _sin(theta);
    // cos    sin
    // -sin   cos
    float lw = _lineWidth / 2.0f;
    iPoint tl = iPointMake(-lw * cosT - lw * sinT, lw * sinT - lw * cosT);
    iPoint tc = iPointMake(-lw * sinT, -lw * cosT);
    iPoint tr = iPointMake(lw * cosT - lw * sinT, -lw * sinT - lw * cosT);

    iPoint bl = iPointMake(-lw * cosT + lw * sinT, lw * sinT + lw * cosT);
    iPoint bc = iPointMake(lw * sinT, lw * cosT);
    iPoint br = iPointMake(lw * cosT + lw * sinT, -lw * sinT + lw * cosT);
#endif

    gVbo->tex = texGdi[0];
    gVbo->qNum = 3;
    iColor4b c = iColor4bMake(_r * 255, _g * 255, _b * 255, _a * 255);

    iQuad* q = &gVbo->q[0];
    memcpy(&q->tl.p, &(sp + tl), sizeof(iPoint));
    memcpy(&q->tr.p, &(sp + tc), sizeof(iPoint));
    memcpy(&q->bl.p, &(sp + bl), sizeof(iPoint));
    memcpy(&q->br.p, &(sp + bc), sizeof(iPoint));

    q->tl.uv = iPointMake(0, 0);
    q->tr.uv = iPointMake(0.5f, 0);
    q->bl.uv = iPointMake(0, 1);
    q->br.uv = iPointMake(0.5f, 1);

    q->tl.c = c;
    q->tr.c = c;
    q->bl.c = c;
    q->br.c = c;

    q = &gVbo->q[1];
    memcpy(&q->tl.p, &(sp + tc), sizeof(iPoint));
    memcpy(&q->tr.p, &(ep + tc), sizeof(iPoint));
    memcpy(&q->bl.p, &(sp + bc), sizeof(iPoint));
    memcpy(&q->br.p, &(ep + bc), sizeof(iPoint));

    q->tl.uv = iPointMake(0.5f, 0);
    q->tr.uv = iPointMake(0.5f, 0);
    q->bl.uv = iPointMake(0.5f, 1);
    q->br.uv = iPointMake(0.5f, 1);

    q->tl.c = c;
    q->tr.c = c;
    q->bl.c = c;
    q->br.c = c;

    q = &gVbo->q[2];
    memcpy(&q->tl.p, &(ep + tc), sizeof(iPoint));
    memcpy(&q->tr.p, &(ep + tr), sizeof(iPoint));
    memcpy(&q->bl.p, &(ep + bc), sizeof(iPoint));
    memcpy(&q->br.p, &(ep + br), sizeof(iPoint));

    q->tl.uv = iPointMake(0.5f, 0);
    q->tr.uv = iPointMake(1, 0);
    q->bl.uv = iPointMake(0.5f, 1);
    q->br.uv = iPointMake(1, 1);

    q->tl.c = c;
    q->tr.c = c;
    q->bl.c = c;
    q->br.c = c;

    gVbo->programID = getProgramID();
    gVbo->paint(0.0f);
}

void drawLine(float x0, float y0, float x1, float y1)
{
    drawLine(iPointMake(x0, y0), iPointMake(x1, y1));
}

static void drawPoly(iPoint* poly, int num, bool fill)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    //float color[num][4];
    float color[4][4] = { {_r, _g, _b, _a}, {_r, _g, _b, _a},
        {_r, _g, _b, _a}, {_r, _g, _b, _a} };
    glVertexPointer(2, GL_FLOAT, 0, poly);
    glColorPointer(4, GL_FLOAT, 0, color);

    glDrawArrays(fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, num);
    //glDrawElements;

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void drawRect(iRect rt, float radius)
{
    drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}

void drawRect(float x, float y, float width, float height, float radius)
{
    GLuint proID = getProgramGdiID(iGDIID_drawRect);
    glUseProgram(proID);

    float w = width / 2.0f;
    float h = height / 2.0f;
    float lw = _lineWidth / 2.0f;

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    float p[4][4] = {
        {-w - lw,  h + lw, 0, 1}, {w + lw,  h + lw, 0, 1},
        {-w - lw, -h - lw, 0, 1}, {w + lw, -h - lw, 0, 1}
    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

    GLuint positionAttr = glGetAttribLocation(proID, "position");
    glEnableVertexAttribArray(positionAttr);
    glVertexAttribPointer(positionAttr, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint mProj = glGetUniformLocation(proID, "mProjection");
    glUniformMatrix4fv(mProj, 1, GL_FALSE, mProjection->d());
    iMatrix mv;
    mv.loadIdentity();
    mv.translate(x + w, y + h, 0);
    GLuint mModelv = glGetUniformLocation(proID, "mModelview");
    glUniformMatrix4fv(mModelv, 1, GL_FALSE, mv.d());

    GLuint uColor = glGetUniformLocation(proID, "color");
    glUniform4f(uColor, _r, _g, _b, _a);

    GLuint uRect = glGetUniformLocation(proID, "rect");
    glUniform4f(uRect, x, devSize.height - y, width, height);

    GLuint uRadius = glGetUniformLocation(proID, "radius");
    glUniform1f(uRadius, radius);

    GLuint uLineWidth = glGetUniformLocation(proID, "lineWidth");
    glUniform1f(uLineWidth, _lineWidth);

    uint8 indices[6] = { 0,1,2 , 1,2,3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

    glDisableVertexAttribArray(positionAttr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void fillRect(iRect rt, float radius)
{
    fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}

void fillRect(float x, float y, float width, float height, float radius)
{
    if (radius == 0.0f)
    {
        Texture* t = texGdi[1];
        drawImage(t, x, y,
            0, 0, t->width, t->height,
            TOP | LEFT, width / t->width, height / t->height,
            2, _angle);
        return;
    }

    GLuint proID = getProgramGdiID(iGDIID_fillRect);
    glUseProgram(proID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    float w = width / 2.0f;
    float h = height / 2.0f;

    float p[4][4] = {
        {-w, h, 0, 1},  {w, h, 0, 1},
        {-w, -h, 0, 1}, {w, -h, 0, 1}
    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

    GLuint positionAttr = glGetAttribLocation(proID, "position");
    glEnableVertexAttribArray(positionAttr);
    glVertexAttribPointer(positionAttr, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint mProj = glGetUniformLocation(proID, "mProjection");
    glUniformMatrix4fv(mProj, 1, GL_FALSE, mProjection->d());
    iMatrix mv;
    mv.loadIdentity();
    mv.translate(x + w, y + h, 0);
    mv.rotate(0, 0, 1, _angle);
    GLuint mModelv = glGetUniformLocation(proID, "mModelview");
    glUniformMatrix4fv(mModelv, 1, GL_FALSE, mv.d());

    GLuint uColor = glGetUniformLocation(proID, "color");
    glUniform4f(uColor, _r, _g, _b, _a);

    GLuint uRect = glGetUniformLocation(proID, "rect");
    glUniform4f(uRect, x, devSize.height - y, width, height);

    GLuint uRadius = glGetUniformLocation(proID, "radius");
    glUniform1f(uRadius, radius);

    float radian = _angle * M_PI / 180.0;
    GLuint uRadian = glGetUniformLocation(proID, "radian");
    glUniform1f(uRadian, radian);

    uint8 indices[6] = { 0,1,2 , 1,2,3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

    glDisableVertexAttribArray(positionAttr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawCircle(iPoint p, float radius)
{
    drawCircle(p.x, p.y, radius);
}

void drawCircle(float x, float y, float radius)
{
    GLuint proID = getProgramGdiID(iGDIID_drawCircle);
    glUseProgram(proID);

    float lw = _lineWidth/ 2.0f;

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    float p[4][4] = {
        {-radius - lw, radius + lw, 0, 1}, {radius + lw, radius + lw, 0, 1},
        {-radius - lw, -radius - lw, 0, 1}, {radius + lw, -radius - lw, 0, 1}
    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

    GLuint positionAttr = glGetAttribLocation(proID, "position");
    glEnableVertexAttribArray(positionAttr);
    glVertexAttribPointer(positionAttr, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint mProj = glGetUniformLocation(proID, "mProjection");
    glUniformMatrix4fv(mProj, 1, GL_FALSE, mProjection->d());

    iMatrix mv;
    mv.loadIdentity();
    mv.translate(x, y, 0);
    GLuint mModelv = glGetUniformLocation(proID, "mModelview");
    glUniformMatrix4fv(mModelv, 1, GL_FALSE, mv.d());


    GLuint uCenter = glGetUniformLocation(proID, "center");
    glUniform2f(uCenter, x, devSize.height - y);

    GLuint uRadius = glGetUniformLocation(proID, "radius");
    glUniform1f(uRadius, radius);

    GLuint uColor = glGetUniformLocation(proID, "color");
    glUniform4f(uColor, _r, _g, _b, _a);


    GLuint uLineWidth = glGetUniformLocation(proID, "lineWidth");
    glUniform1f(uLineWidth, _lineWidth);

    uint8 indices[6] = { 0,1,2 , 1,2,3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

    glDisableVertexAttribArray(positionAttr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void fillCircle(iPoint p, float radius, bool reverse)
{
    fillCircle(p.x, p.y, radius, reverse);
}

void fillCircle(float x, float y, float radius, bool reverse)
{
    GLuint proID = getProgramGdiID(reverse ? iGDIID_fillReverseCircle : iGDIID_fillCircle);
    glUseProgram(proID);

    iSize size = devSize * reverse;
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    float p[4][4] = {
        {-radius - size.width, radius + size.height, 0, 1},    {radius + size.width, radius + size.height, 0, 1},
        {-radius - size.width, -radius - size.height, 0, 1},   {radius + size.width, -radius - size.height, 0, 1}

    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

    GLuint positionAttr = glGetAttribLocation(proID, "position");
    glEnableVertexAttribArray(positionAttr);
    glVertexAttribPointer(positionAttr, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint mProj = glGetUniformLocation(proID, "mProjection");
    glUniformMatrix4fv(mProj, 1, GL_FALSE, mProjection->d());
    
    iMatrix mv;
    mv.loadIdentity();
    mv.translate(x, y, 0);
    GLuint mModelv = glGetUniformLocation(proID, "mModelview");
    glUniformMatrix4fv(mModelv, 1, GL_FALSE, mv.d());


    GLuint uCenter = glGetUniformLocation(proID, "center");
    glUniform2f(uCenter, x, devSize.height - y);

    GLuint uRadius = glGetUniformLocation(proID, "radius");
    glUniform1f(uRadius, radius);

    GLuint uColor = glGetUniformLocation(proID, "color");
    glUniform4f(uColor, _r, _g, _b, _a);

    uint8 indices[6] = { 0,1,2 , 1,2,3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

    glDisableVertexAttribArray(positionAttr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawEllipse(float x, float y, float width, float height)
{
    GLuint proID = getProgramGdiID(iGDIID_fillEllipse);
    glUseProgram(proID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    float w = width / 2.0f;
    float h = height / 2.0f;
    float lw = _lineWidth / 2.0f;

    float p[4][4] = {
        {-w - lw,  h + lw, 0, 1},     {w + lw,  h + lw, 0, 1},
        {-w - lw, -h - lw, 0, 1},     {w + lw, -h - lw, 0, 1}

    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

    GLuint positionAttr = glGetAttribLocation(proID, "position");
    glEnableVertexAttribArray(positionAttr);
    glVertexAttribPointer(positionAttr, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint mProj = glGetUniformLocation(proID, "mProjection");
    glUniformMatrix4fv(mProj, 1, GL_FALSE, mProjection->d());
    iMatrix mv;
    mv.loadIdentity();
    mv.translate(x, y, 0);
    mv.rotate(0, 0, 1, _angle);
    GLuint mModelv = glGetUniformLocation(proID, "mModelview");
    glUniformMatrix4fv(mModelv, 1, GL_FALSE, mv.d());


    GLuint uCenter = glGetUniformLocation(proID, "center");
    glUniform2f(uCenter, x, devSize.height - y);

    GLuint uSize = glGetUniformLocation(proID, "size");
    glUniform2f(uSize, width, height);

    float radian = _angle * M_PI / 180.0;
    GLuint uRadian = glGetUniformLocation(proID, "radian");
    glUniform1f(uRadian, radian);

    GLuint uLineWidth = glGetUniformLocation(proID, "lineWidth");
    glUniform1f(uLineWidth, _lineWidth);

    GLuint uColor = glGetUniformLocation(proID, "color");
    glUniform4f(uColor, _r, _g, _b, _a);

    uint8 indices[6] = { 0,1,2 , 1,2,3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

    glDisableVertexAttribArray(positionAttr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void fillEllipse(float x, float y, float width, float height)
{
    GLuint proID = getProgramGdiID(iGDIID_fillEllipse);
    glUseProgram(proID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    float w = width / 2.0f;
    float h = height / 2.0f;

    float p[4][4] = {
        {-w, h, 0, 1},      {w, h, 0, 1},
        {-w, -h, 0, 1},     {w, -h, 0, 1}

    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, p);

    GLuint positionAttr = glGetAttribLocation(proID, "position");
    glEnableVertexAttribArray(positionAttr);
    glVertexAttribPointer(positionAttr, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    GLuint mProj = glGetUniformLocation(proID, "mProjection");
    glUniformMatrix4fv(mProj, 1, GL_FALSE, mProjection->d());
    iMatrix mv;
    mv.loadIdentity();
    mv.translate(x, y, 0);
    mv.rotate(0, 0, 1, _angle);
    GLuint mModelv = glGetUniformLocation(proID, "mModelview");
    glUniformMatrix4fv(mModelv, 1, GL_FALSE, mv.d());


    GLuint uCenter = glGetUniformLocation(proID, "center");
    glUniform2f(uCenter, x, devSize.height - y);

    GLuint uSize = glGetUniformLocation(proID, "size");
    glUniform2f(uSize, width, height);

    float radian = _angle * M_PI / 180.0;
    GLuint uRadian = glGetUniformLocation(proID, "radian");
    glUniform1f(uRadian, radian);

    GLuint uColor = glGetUniformLocation(proID, "color");
    glUniform4f(uColor, _r, _g, _b, _a);

    uint8 indices[6] = { 0,1,2 , 1,2,3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

    glDisableVertexAttribArray(positionAttr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

Texture* createTexture(int width, int height, bool rgba32f)
{
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    setTexture(CLAMP, LINEAR);
    
    if (rgba32f == false)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    else
    {
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    Texture* tex = (Texture*)malloc(sizeof(Texture));
    tex->texID = texID;
    tex->width = width;
    tex->height = height;
    tex->potWidth = width;
    tex->potHeight = height;
    tex->retainCount = 1;

    return tex;
}

void setTexture(TextureWrap wrap, TextureFilter filter)
{
    GLenum e = (wrap == CLAMP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, e);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, e);

    if (filter != MIPMAP)
    {
        e = (filter == NEAREST ? GL_NEAREST : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, e);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, e);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

void saveImageFromRGBA(const char* path, uint8* rgba, int width, int height)
{
    int potWidth = nextPOT(width);
    //int potHeight = nextPOT(height);

    Bitmap* bmp = new Bitmap(width, height, PixelFormat32bppARGB);
    BitmapData bmpData;
    Rect rt(0, 0, width, height);
    bmp->LockBits(&rt, ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);

    int stride = bmpData.Stride / 4;
    uint8* bgra = (uint8*)bmpData.Scan0;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            uint8* dst = &bgra[stride * 4 * j + 4 * i];
            uint8* src = &rgba[potWidth * 4 * (height-1-j) + 4 * i];
            dst[2] = src[0];
            dst[1] = src[1];
            dst[0] = src[2];
            dst[3] = src[3];
        }
    }

    bmp->UnlockBits(&bmpData);

    wchar_t* wPath = utf8_to_utf16(path);
    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);
    bmp->Save(wPath, &pngClsid);
    free(wPath);
    delete bmp;
}

uint8* bmp2rgba(Bitmap* bmp, int& width, int& height)
{
    width = bmp->GetWidth();
    height = bmp->GetHeight();
    int potWidth = nextPOT(width);
    int potHeight = nextPOT(height);

    BitmapData bmpData;
    Rect rt(0, 0, width, height);
    bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

    int stride = bmpData.Stride / 4;
    uint8* argb = (uint8*)bmpData.Scan0;
    uint8* rgba = (uint8*)calloc(sizeof(uint8), potWidth * potHeight * 4);
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            uint8* dst = &rgba[potWidth * 4 * j + 4 * i];
            uint8* src = &argb[stride * 4 * j + 4 * i];
#if 1 // Straight Alpha : 2D
            dst[0] = src[2];
            dst[1] = src[1];
            dst[2] = src[0];
            dst[3] = src[3];
#else // Pre-multiply Alpha : 3D
            dst[0] = src[2] * 255.0f / src[3];
            dst[1] = src[1] * 255.0f / src[3];
            dst[2] = src[0] * 255.0f / src[3];
            dst[3] = src[3];
#endif
        }
    }

    bmp->UnlockBits(&bmpData);

    return rgba;
}

Texture* createImage(const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    wchar_t* ws = utf8_to_utf16(szText);
    Bitmap* bmp = new Bitmap(ws);
    free(ws);

    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);
    delete bmp;

    Texture* tex = createImageWithRGBA(rgba, width, height);
    free(rgba);

    return tex;
}

uint8* convertHorizonReverse(uint8* rgba, int width, int height)
{
    int potWidth = nextPOT(width);
    int potHeight = nextPOT(height);
    uint8* buf = (uint8*)calloc(sizeof(uint8), potWidth * potHeight * 4);
    for (int j = 0; j < height; j++)
    {
        uint8* dst = &buf[potWidth * j * 4];
        uint8* src = &rgba[potWidth * (height - j - 1) * 4];
        memcpy(dst, src, sizeof(uint8) * 4 * potWidth);
    }

    return buf;
}

Texture* createReverseImage(const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    wchar_t* ws = utf8_to_utf16(szText);
    Bitmap* bmp = new Bitmap(ws);
    free(ws);

    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);
    delete bmp;

    uint8* reverseRGBA = convertHorizonReverse(rgba, width, height);
    free(rgba);

    Texture* tex = createImageWithRGBA(reverseRGBA, width, height);
    free(reverseRGBA);

    return tex;
}

void convertGrey(uint8* rgba, int width, int height, int potWidth)
{
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            uint8* c = &rgba[potWidth*4 * j + 4 * i];
            uint8 grey = (299 * c[0] + 587 * c[1] + 114 * c[2]) / 1000.f;
            c[0] = grey;
            c[1] = grey;
            c[2] = grey;
        }
    }
}

Texture* createGreyImage(const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    wchar_t* ws = utf8_to_utf16(szText);
    Bitmap* bmp = new Bitmap(ws);
    free(ws);

    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);
    delete bmp;

    convertGrey(rgba, width, height, nextPOT(width));

    Texture* tex = createImageWithRGBA(rgba, width, height);
    free(rgba);

    return tex;
}

uint8* convertReflect(uint8* rgba, int width, int& height, int potWidth, float rateY)
{
    int newHeight = height * rateY;
    int newPotHeight = nextPOT(newHeight);
    uint8* buf = (uint8*)calloc(sizeof(uint8), potWidth * newPotHeight * 4);
    memcpy(buf, rgba, sizeof(uint8) * potWidth * height * 4);

    int remindHeight = newPotHeight - height;
    for (int j = 0; j < remindHeight; j++)
    {
        int J = height * (1.0f * j / remindHeight);
        uint8 a = 0xFF * (1.0f - 1.0f * j / remindHeight);
        for (int i = 0; i < width; i++)
        {
            uint8* dst = &buf[potWidth * 4 * (height + j) + 4 * i];
            uint8* src = &rgba[potWidth * 4 * (height - 1 - J) + 4 * i];
            memcpy(dst, src, sizeof(uint8) * 3);
            dst[3] = a;
        }
    }

    height = newHeight;

    return buf;
}

Texture* createReflectImage(float rateY, const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    wchar_t* ws = utf8_to_utf16(szText);
    Bitmap* bmp = new Bitmap(ws);
    free(ws);

    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);
    delete bmp;

    uint8* tmp = convertReflect(rgba, width, height, nextPOT(width), rateY);
    free(rgba);

    Texture* tex = createImageWithRGBA(tmp, width, height);
    free(tmp);

    return tex;
}

Texture** createDivideImage(int wNum, int hNum, const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    wchar_t* ws = utf8_to_utf16(szText);
    Bitmap* bmp = new Bitmap(ws);
    free(ws);

    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);
    delete bmp;

    int potWidth = nextPOT(width);
    int potHeight = nextPOT(height);

    int w = width / wNum;
    int h = height / hNum;
    int potW = nextPOT(w);
    int potH = nextPOT(h);

    Texture** texs = (Texture**)malloc(sizeof(Texture*) * wNum * hNum);
#if 0
    uint8* buf = (uint8*)malloc(sizeof(uint8) * potW * potH * 4);
    for (int j = 0; j < hNum; j++)
    {
        for (int i = 0; i < wNum; i++)
        {
            memset(buf, 0x00, sizeof(uint8) * potW * potH * 4);
            for (int k = 0; k < h; k++)
                memcpy(&buf[potW*4 * k], &rgba[potWidth*4 * (h*j + k) + w*4 * i],
                    sizeof(uint8) * w * 4);

            texs[wNum * j + i] = createImageWithRGBA(buf, w, h);
        }
    }
#else
    int* RGBA = (int*)rgba;
    int* buf = (int*)calloc(sizeof(int), potW * potH);
    for (int j = 0; j < hNum; j++)
    {
        for (int i = 0; i < wNum; i++)
        {
            // BUF <=   w * i, h * j    RGBA
            for (int n = 0; n < h; n++)
            {
                //for (int m = 0; m < w; m++)
                //    buf[potW * n + m] = RGBA[potWidth * (h * j + n) + w * i + m];
                memcpy(&buf[potW * n], &RGBA[potWidth * (h * j + n) + w * i], sizeof(int)*w);
            }
            texs[wNum * j + i] = createImageWithRGBA((GLubyte*)buf, w, h);
        }
    }
#endif
    free(buf);
    free(rgba);

    return texs;
}

void freeImage(Texture* tex)
{
    if (tex->retainCount > 1)
    {
        tex->retainCount--;
        return;
    }
    glDeleteTextures(1, &tex->texID);

    free(tex);
}


void drawImageLikeCircle(Texture* tex, int x, int y, float rate)
{
#define DEGREE 45//360
    float position[DEGREE][6];
    float coordinate[DEGREE][6];
    short indices[DEGREE][3];
    for (int i = 0; i < DEGREE; i++)
    {
        //////////////////////////////////////
        position[i][0] = x;
        position[i][1] = y;

        iPoint p0 = { x + tex->width / 2 * rate, y };
        iPoint p1 = iPointRotate(p0, iPointMake(x,y), i);
        if (p1.x < x - tex->width / 2) p1.x = x - tex->width / 2;
        else if (p1.x > x + tex->width / 2) p1.x = x + tex->width / 2;
        if (p1.y < y - tex->height / 2) p1.y = y - tex->height / 2;
        else if (p1.y > y + tex->height / 2) p1.y = y + tex->height / 2;
        position[i][2] = p1.x;
        position[i][3] = p1.y;

        p1 = iPointRotate(p0, iPointMake(x, y), 1+i);
        if (p1.x < x - tex->width / 2) p1.x = x - tex->width / 2;
        else if (p1.x > x + tex->width / 2) p1.x = x + tex->width / 2;
        if (p1.y < y - tex->height / 2) p1.y = y - tex->height / 2;
        else if (p1.y > y + tex->height / 2) p1.y = y + tex->height / 2;
        position[i][4] = p1.x;
        position[i][5] = p1.y;
        ///////////////////////////////////////
        coordinate[i][0] = tex->width / tex->potWidth / 2;
        coordinate[i][1] = tex->height / tex->potHeight / 2;

        iPoint c0 = { coordinate[i][0] + 0.5f * rate, coordinate[i][1] };
        iPoint c1 = iPointRotate(c0, iPointMake(coordinate[i][0], coordinate[i][1]), i);
        if (c1.x < 0.0f) c1.x = 0.0f; else if (c1.x > 1.0f) c1.x = 1.0f;
        if (c1.y < 0.0f) c1.y = 0.0f; else if (c1.y > 1.0f) c1.y = 1.0f;
        coordinate[i][2] = c1.x;
        coordinate[i][3] = c1.y;

        c1 = iPointRotate(c0, iPointMake(coordinate[i][0], coordinate[i][1]), 1+i);
        if (c1.x < 0.0f) c1.x = 0.0f; else if (c1.x > 1.0f) c1.x = 1.0f;
        if (c1.y < 0.0f) c1.y = 0.0f; else if (c1.y > 1.0f) c1.y = 1.0f;
        coordinate[i][4] = c1.x;
        coordinate[i][5] = c1.y;

        //////////////////////////////////////
        indices[i][0] = 3 * i + 0;
        indices[i][1] = 3 * i + 1;
        indices[i][2] = 3 * i + 2;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex->texID);

    glVertexPointer(2, GL_FLOAT, 0, position);
    glTexCoordPointer(2, GL_FLOAT, 0, coordinate);
    //glColorPointer(4, GL_FLOAT, 0, color);

    glDrawElements(GL_TRIANGLES, DEGREE * 3, GL_UNSIGNED_SHORT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawImage(Texture* tex, int x, int y, int anc)
{
    drawImage(tex, x, y,
        0, 0, tex->width, tex->height,
        anc, 1.0f, 1.0f, 2, 0, REVERSE_NONE);
}

void drawImage(Texture* tex, int x, int y,
    int ix, int iy, int iw, int ih, 
    int anc, float ratX, float ratY,
    int xyz, float degree, int reverse)
{
    int width = tex->width * ratX;
    int height = tex->height * ratY;
    switch (anc) {
    case TOP | LEFT:                                         break;
    case TOP | HCENTER:     x -= width / 2;                  break;
    case TOP | RIGHT:       x -= width;                      break;
    case VCENTER | LEFT:                    y -= height / 2; break;
    case VCENTER | HCENTER: x -= width / 2; y -= height / 2; break;
    case VCENTER | RIGHT:   x -= width;     y -= height / 2; break;
    case BOTTOM | LEFT:                     y -= height;     break;
    case BOTTOM | HCENTER:  x -= width / 2; y -= height;     break;
    case BOTTOM | RIGHT:    x -= width;     y -= height;     break;
    }

    gVbo->qNum = 1;
    iQuad* q = &gVbo->q[0];

    q->tl.p[0] = x;         q->tl.p[1] = y;
    q->tr.p[0] = x + width; q->tr.p[1] = y;
    q->bl.p[0] = x;         q->bl.p[1] = y + height;
    q->br.p[0] = x + width; q->br.p[1] = y + height;

    q->tl.uv.x = ix / tex->potWidth;        q->tl.uv.y = iy / tex->potHeight;
    q->tr.uv.x = (ix + iw) / tex->potWidth; q->tr.uv.y = iy / tex->potHeight;
    q->bl.uv.x = ix / tex->potWidth;        q->bl.uv.y = (iy + ih) / tex->potHeight;
    q->br.uv.x = (ix + iw) / tex->potWidth; q->br.uv.y = (iy + ih) / tex->potHeight;

    iColor4b c = iColor4bMake(_r * 255.f, _g * 255.f, _b * 255.f, _a * 255.f);
    q->tl.c = c;
    q->tr.c = c;
    q->bl.c = c;
    q->br.c = c;

    if (reverse & REVERSE_WIDTH)
    {
        float t = q->tl.p[0];
        q->tl.p[0] = q->tr.p[0];
        q->tr.p[0] = t;

        t = q->bl.p[0];
        q->bl.p[0] = q->br.p[0];
        q->br.p[0] = t;
    }
    if (reverse & REVERSE_HEIGHT)
    {
        float t = q->tl.p[1];
        q->tl.p[1] = q->bl.p[1];
        q->bl.p[1] = t;

        t = q->tr.p[1];
        q->tr.p[1] = q->br.p[1];
        q->br.p[1] = t;
    }


    float m[16];
    if (degree)
    {
        memcpy(m, mModelview->d(), sizeof(float) * 16);
        iPoint t = iPointMake(x + width / 2, y + height / 2);
        q->tl.p[0] -= t.x;      q->tl.p[1] -= t.y;
        q->tr.p[0] -= t.x;      q->tr.p[1] -= t.y;
        q->bl.p[0] -= t.x;      q->bl.p[1] -= t.y;
        q->br.p[0] -= t.x;      q->br.p[1] -= t.y;
        mModelview->translate(t.x, t.y, 0);

        float _xyz[3] = { 0, 0, 0 };
        _xyz[xyz] = 1;
        while (degree > 360) degree -= 360;
        degree = 360 - degree;
        mModelview->rotate(_xyz[0], _xyz[1], _xyz[2], degree);
    }

    gVbo->tex = tex;
    gVbo->programID = getProgramID();
    gVbo->paint(0.0f);
    if(degree)
        memcpy(mModelview->d(), m, sizeof(float) * 16);

}

void setClip(int x, int y, int width, int height)
{
    if (x == 0 && y == 0 && width == 0 && height == 0)
        glDisable(GL_SCISSOR_TEST);
    else
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, devSize.height-y-height, width, height);
    }
}

void setClip(iRect rt)
{
    setClip(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height);
}

void setStringName(const char* name)
{
    if (_stringName)
    {
        if (strcmp(_stringName, name) == 0)
            return;
        free(_stringName);
    }

    char* s = (char*)calloc(sizeof(char), 1 + strlen(name));
    strcpy(s, name);
    _stringName = s;
}

const char* getStringName() { return _stringName; }
float getStringSize() { return _stringSize; }

void setStringSize(float size)
{
    _stringSize = size;
}

void setStringRGBA(float r, float g, float b, float a)
{
    _stringR = r;
    _stringG = g;
    _stringB = b;
    _stringA = a;
}

void getStringRGBA(float& r, float& g, float& b, float& a)
{
    r = _stringR;
    g = _stringG;
    b = _stringB;
    a = _stringA;
}

void setStringBorder(float size)
{
    _stringBorder = size;
}

float getStringBorder()
{
    return _stringBorder;
}

void setStringBorderRGBA(float r, float g, float b, float a)
{
    _stringBorderR = r;
    _stringBorderG = g;
    _stringBorderB = b;
    _stringBorderA = a;
}
void getStringBorderRGBA(float& r, float& g, float& b, float& a)
{
    r = _stringBorderR;
    g = _stringBorderG;
    b = _stringBorderB;
    a = _stringBorderA;
}

iRect rectOfString(uint8* rgba, int width, int height)
{
    iRect rt;

    // 왼쪽에서 오른쪽 찾아감
    for (int i = 0; i < width; i++)
    {
        bool found = false;
        for (int j = 0; j < height; j++)
        {
            if (rgba[width * 4 * j + 4 * i + 3])
            {
                rt.origin.x = i;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    // 오른쪽에서 왼쪽 찾아감
    for (int i = width - 1; i > -1; i--)
    {
        bool found = false;
        for (int j = 0; j < height; j++)
        {
            if (rgba[width * 4 * j + 4 * i + 3])
            {
                rt.size.width = i - rt.origin.x - 1;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    // 위에서 아래로 찾아감
    for (int j = 0; j < height; j++)
    {
        bool found = false;
        for (int i = 0; i < width; i++)
        {
            if (rgba[width * 4 * j + 4 * i + 3])
            {
                rt.origin.y = j;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    // 아래에서 위로 찾아감
    for (int j = height - 1; j > -1; j--)
    {
        bool found = false;
        for (int i = 0; i < width; i++)
        {
            if (rgba[width * 4 * j + 4 * i + 3])
            {
                rt.size.height = j - rt.origin.y - 1;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    return rt;
}

iSize sizeOfString(const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    wchar_t* wStringName = utf8_to_utf16(_stringName);
    wchar_t* wStr = utf8_to_utf16(szText);

    GraphicsPath path;
    FontFamily  fontFamily(wStringName);
    PointF      pointF(0, 0);
    StringFormat sf;// top|left
    //sf.SetFormatFlags();

    GraphicsPath p;
    p.AddString(wStr, lstrlenW(wStr), &fontFamily, FontStyleRegular,
        _stringSize, pointF, &sf);
    Rect rt;
    p.GetBounds(&rt);

    free(wStringName);
    free(wStr);

    return iSizeMake(rt.Width, rt.Height);
}

void drawString(int x, int y, int anc, const char* szFormat, ...)
{
    // #bug
}

#define W 32
#define R 16
#define P 0
#define M1 13
#define M2 9
#define M3 5

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define MAT3NEG(t,v) (v<<(-(t)))
#define MAT4NEG(t,b,v) (v ^ ((v<<(-(t))) & b))

#define V0            STATE[state_i                   ]
#define VM1           STATE[(state_i+M1) & 0x0000000fU]
#define VM2           STATE[(state_i+M2) & 0x0000000fU]
#define VM3           STATE[(state_i+M3) & 0x0000000fU]
#define VRm1          STATE[(state_i+15) & 0x0000000fU]
#define VRm2          STATE[(state_i+14) & 0x0000000fU]
#define newV0         STATE[(state_i+15) & 0x0000000fU]
#define newV1         STATE[state_i                 ]
#define newVRm1       STATE[(state_i+14) & 0x0000000fU]

#define FACT 2.32830643653869628906e-10

static unsigned int state_i = 0;
static unsigned int STATE[R];
static unsigned int z0, z1, z2;

void sRandom() {
    int j;
    state_i = 0;
    for (j = 0; j < R; j++)
        STATE[j] = rand();
}

uint32 random()
{
#if 1
    return rand();
#else
    z0 = VRm1;
    z1 = MAT0NEG(-16, V0) ^ MAT0NEG(-15, VM1);
    z2 = MAT0POS(11, VM2);
    newV1 = z1 ^ z2;
    newV0 = MAT0NEG(-2, z0) ^ MAT0NEG(-18, z1) ^ MAT3NEG(-28, z2) ^ MAT4NEG(-5, 0xda442d24U, newV1);
    state_i = (state_i + 15) & 0x0000000fU;
    return STATE[state_i] * FACT;
#endif
}

float _sin(float degree)
{
    return sin(M_PI / 180 * degree);
}

float _cos(float degree)
{
    return cos(M_PI / 180 * degree);
}

float linear(float r, float a, float b)
{
    return a + (b - a) * r;
}

iPoint linear(float r, iPoint a, iPoint b)
{
    return a + (b - a) * r;
}

float easeIn(float r, float a, float b)
{
    return a + (b - a) * _sin(90 * r);
}

iPoint easeIn(float r, iPoint a, iPoint b)
{
    return a + (b - a) * _sin(90 * r);
}

float easeOut(float r, float a, float b)
{
    return a + (b - a) * (1 + _sin(270 + 90 * r)) * (1 + _sin(270 + 90 * r));
    //return a + (b - a) * r * r;
}

iPoint easeOut(float r, iPoint a, iPoint b)
{
    return a + (b - a) * (1 + _sin(270 + 90 * r));
    //return a + (b - a) * r * r;
}

float easeInOut(float r, float a, float b)
{
    if (r > 0.5f)
        return  easeIn(r * 2.0f, a, a + (b - a) / 2.0f);
    else
        return easeOut((r - 0.5f) * 2.0f, a + (b - a) / 2.0f, b);
}

iPoint easeInOut(float r, iPoint a, iPoint b)
{
    if (r < 0.5f)
        easeIn(r * 2.0f, a, a + (b - a) / 2);
    return easeOut((r - 0.5f) * 2.0f, a + (b - a) / 2, b);
}

bool containPoint(iPoint p, iRect rt)
{
    if (p.x < rt.origin.x ||
        p.x > rt.origin.x + rt.size.width ||
        p.y < rt.origin.y ||
        p.y > rt.origin.y + rt.size.height)
        return false;
    return true;
}

bool containRect(iRect src, iRect dst)
{
    if (src.origin.x + src.size.width < dst.origin.x ||
        src.origin.x > dst.origin.x + dst.size.width ||
        src.origin.y + src.size.height < dst.origin.y ||
        src.origin.y > dst.origin.y + dst.size.height)
        return false;

    return true;
}

char* loadFile(const char* filePath, int& length)
{
    FILE* pf = fopen(filePath, "rb");
    if (pf == NULL)
        return NULL;
    fseek(pf, 0, SEEK_END);
    length = ftell(pf);
    char* buf = (char*)calloc(sizeof(char), 1 + length);
    fseek(pf, 0, SEEK_SET);//rewind(pf);
    fread(buf, length, 1, pf);

    fclose(pf);

    return buf;
}

void saveFile(const char* filePath, char* buf, int bufLength)
{
    FILE* pf = fopen(filePath, "wb");

    fwrite(buf, bufLength, 1, pf);

    fclose(pf);
}

bool existFile(const char* filePath)
{
    FILE* pf = fopen(filePath, "rb");
    if (pf)
    {
        fclose(pf);
        return true;
    }
    return false;
}

static iCriticalSection* instanceCS = NULL;
iCriticalSection::iCriticalSection()
{
    InitializeCriticalSection(&cs);
}

iCriticalSection::~iCriticalSection()
{
    DeleteCriticalSection(&cs);
    instanceCS = NULL;
}

iCriticalSection* iCriticalSection::instance()
{
    if (instanceCS == NULL)
        instanceCS = new iCriticalSection();
    return instanceCS;
}

void iCriticalSection::start()
{
    EnterCriticalSection(&cs);
}

void iCriticalSection::end()
{
    LeaveCriticalSection(&cs);
}
