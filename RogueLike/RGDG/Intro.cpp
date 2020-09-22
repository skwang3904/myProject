#include "Intro.h"

#include "Loading.h"
#include "Game.h"
#include "Proc.h"

void loadIntro()
{
    loadIntroShader();
    createPopPressAnyKey();
    createPopIntroButton();
    createPopIntroSelectStart();

    showPopPressAnyKey(true);

    audioPlay(AUDIO_INTRO);
}

void freeIntro()
{
    freeIntroShader();
    freePopPressAnyKey();
    freePopIntroButton();
    freePopIntroSelectStart();
}

void drawIntro(float dt)
{
    drawIntroShader(dt);
    drawPopPressAnyKey(dt);
    drawPopIntroButton(dt);
    drawPopIntroSelectStart(dt);

}

void keyIntro(iKeyState stat, iPoint point)
{
    if (keyPopIntroSelectStart(stat, point) ||
        keyPopIntroButton(stat, point) ||
        keyPopPressAnyKey(stat, point))
        return;
}

//------------------------------------------------------------------------
// Shader
void loadIntroShader()
{
    STInput si =
    {
        "assets/shader/toy/TextFontCommon.frag",
        // bufA
            NULL,
            NULL,
            {
                -1, NULL, CLAMP, LINEAR, false,
                -1, NULL, CLAMP, LINEAR, false,
                -1, NULL, CLAMP, LINEAR, false,
                -1, NULL, CLAMP, LINEAR, false,
            },

            // bufB
                NULL,
                NULL,
                {
                    -1, NULL, CLAMP, LINEAR, false,
                    -1, NULL, CLAMP, LINEAR, false,
                    -1, NULL, CLAMP, LINEAR, false,
                    -1, NULL, CLAMP, LINEAR, false,
                },

            // bufC
                NULL,
                NULL,
                {
                    -1, NULL, CLAMP, LINEAR, false,
                    -1, NULL, CLAMP, LINEAR, false,
                    -1, NULL, CLAMP, LINEAR, false,
                    -1, NULL, CLAMP, LINEAR, false,
                },

            // bufD
                NULL,
                NULL,
                {
                    -1, NULL, CLAMP, LINEAR, false,
                    -1, NULL, CLAMP, LINEAR, false,
                    -1, NULL, CLAMP, LINEAR, false,
                    -1, NULL, CLAMP, LINEAR, false,
                },

            // img
                "assets/shader/toy/TextFont.vert",
                "assets/shader/toy/TextFont.frag",
                {
                    -1, "assets/shader/toy/TextFont.png", REPEAT, MIPMAP, true,
                    -1, "assets/shader/toy/introBG.png", REPEAT, MIPMAP, true,
                    -1, NULL, CLAMP, LINEAR, false,
                    -1, NULL, CLAMP, LINEAR, false,
                },
    };

    shaderToy = new iShaderToy(&si);
}

void freeIntroShader()
{
    delete shaderToy;
}

void drawIntroShader(float dt)
{
    shaderToy->paint(dt);
}


//------------------------------------------------------------------------
// PressAnyKey
iPopup* popPressAnyKey;

void drawPopPressAnyKeyBefore(iPopup* me, iPoint p, float dt);

void createPopPressAnyKey()
{
    int i, j;
    Texture* tex, *t;
    iImage* img;
    iPopup* pop = new iPopup(iPopupStyleAlpha);
    iSize size;

    img = new iImage();
    size = iSizeMake(devSize.width / 2.0f, devSize.height * 0.15f);
    tex = createTexture(size.width, size.height);

    fbo->bind(tex);
    t = createImage("assets/intro/introPressAnyKey.png");
    setRGBA(0.5, 0.6, 0.4, 1);
    DRAWIMAGE(t, size);
    setRGBA(1, 1, 1, 1);
    freeImage(t);
    fbo->unbind();
    
    img->addObject(tex);
    freeImage(tex);

    pop->addObject(img);


    pop->openPosition =
    pop->closePosition = iPointMake((devSize.width - size.width) / 2.0f, devSize.height * 0.7f);
    pop->methodDrawBefore = drawPopPressAnyKeyBefore;
    pop->_showDt = 1.5f;
    popPressAnyKey = pop;
}

void freePopPressAnyKey()
{
    delete popPressAnyKey;
}

void showPopPressAnyKey(bool show)
{
    popPressAnyKey->show(show);
}

void drawPopPressAnyKeyBefore(iPopup* me, iPoint p, float dt)
{
    if (me->stat != iPopupStatProc)
        return;

    me->showDt += dt / 2.0f;
    float a = fabs(_sin(90 * (me->showDt / me->_showDt))) * 0.5f;
    a += 0.5f;
    setRGBA(1, 1, 1, a);
}

void drawPopPressAnyKey(float dt)
{
    popPressAnyKey->paint(dt);
    setRGBA(1, 1, 1, 1);
}

bool keyPopPressAnyKey(iKeyState stat, iPoint point)
{
    if (popPressAnyKey->bShow == false)
        return false;

    if (popPressAnyKey->stat != iPopupStatProc)
        return false;

    if (stat == iKeyStateBegan)
    {
        showPopPressAnyKey(false);
        popPressAnyKey->_showDt = 0.5f;
        showPopIntroButton(true);
        audioPlay(AUDIO_MenuSelected);
    }

    return true;
}

//------------------------------------------------------------------------
// IntroButton
#define INTRO_BUTTON_OPEN_POSITION  iPointMake(-devSize.width * 0.5f, devSize.height * 0.6f)
#define INTRO_BUTTON_CLOSE_POSITION iPointMake(devSize.width * 0.05f, devSize.height * 0.6f)
iPopup* popIntroButton;
iImage** imgIntroButton;

void drawPopIntroButtonBefore(iPopup* me, iPoint p, float dt);

void createPopIntroButton()
{
    int i, j;
    Texture* tex, *t, *text;
    iImage* img;
    iPopup* pop = new iPopup(iPopupStyleMove);
    iSize size;

    imgIntroButton = (iImage**)malloc(sizeof(iImage*) * 3);
    const char* strPath[3] = {
        "assets/intro/introButtonStart.png",
        "assets/intro/introButtonOption.png",
        "assets/intro/introButtonExit.png",
    };

    const char* strButton[3] = {
    "Start",
    "Option",
    "Exit",
    };

    iGraphics* g = iGraphics::instance();
    size = iSizeMake(devSize.width / 5.0f, devSize.height / 10.0f);
    setStringSize(size.height * 0.7f);
    setStringRGBA(0, 0, 0, 1);
    setStringBorder(0);
    for (i = 0; i < 3; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            g->init(size);
            g->drawString(size.width / 2.0f, size.height / 2.0f, 
                VCENTER | HCENTER, strButton[i]);
            text = g->getTexture();
            
            tex = createTexture(size.width, size.height);

            fbo->bind(tex);
            if (j == 0) setRGBA(1, 1, 1, 1);
            else        setRGBA(0, 1, 0, 1);
            t = createImage("assets/intro/introButton.png");
            DRAWIMAGE(t, size);
            freeImage(t);
            setRGBA(1, 1, 1, 1);
            t = createImage(strPath[i]);
            float s = size.height * 0.33f;
            drawImage(t, 0, size.height / 2.0f,
                0, 0, t->width, t->height,
                VCENTER | LEFT, s / t->width, s / t->height,
                2, 0, REVERSE_HEIGHT);
            freeImage(t);
            DRAWIMAGE(text, size);
            freeImage(text);
            fbo->unbind();

            img->addObject(tex);
            freeImage(tex);
        }

        img->position = iPointMake(0, (size.height * 1.2f) * i);
        imgIntroButton[i] = img;
        pop->addObject(img);
    }


    pop->openPosition = INTRO_BUTTON_OPEN_POSITION;
    pop->closePosition = INTRO_BUTTON_CLOSE_POSITION;
    pop->methodDrawBefore = drawPopIntroButtonBefore;
    pop->_showDt = 1.0f;
    popIntroButton = pop;
}

void freePopIntroButton()
{
    delete popIntroButton;
    free(imgIntroButton);
}

void showPopIntroButton(bool show)
{
    popIntroButton->show(show);
}

void drawPopIntroButtonBefore(iPopup* me, iPoint p, float dt)
{
    for (int i = 0; i < 3; i++)
        imgIntroButton[i]->setTexAtIndex(i == popIntroButton->selected);
}

void drawPopIntroButton(float dt)
{
    popIntroButton->paint(dt);
}

extern bool runWnd;
bool keyPopIntroButton(iKeyState stat, iPoint point)
{
    if (popIntroButton->bShow == false)
        return false;

    if (popIntroButton->stat != iPopupStatProc)
        return false;

    int i, j = -1;
    switch (stat)
    {
    case iKeyStateBegan:
    {
        i = popIntroButton->selected;
        if (i == -1)
            break;

        if (i == 0)
        {
            // start
            showPopIntroButton(false);
            showPopIntroSelectStart(true);
        }
        else if (i == 1)
        {
            //option
            showPopOption(true);
        }
        else if (i == 2)
        {
            //exit
            runWnd = false;
            audioStop(AUDIO_INTRO);
        }
        popIntroButton->selected = -1;
        audioPlay(AUDIO_MenuSelected);
        break;
    }
    case iKeyStateMoved:
    {
        for (i = 0; i < 3; i++)
        {
            if (containPoint(point, imgIntroButton[i]->touchRect(popIntroButton->closePosition)))
            {
                j = i;
                break;
            }
        }

        if (popIntroButton->selected != j)
            ;

        popIntroButton->selected = j;

        break;
    }
    case iKeyStateEnded:
    {

        break;
    }
    }

    return true;
}

//------------------------------------------------------------------------
// SelectStart
iPopup* popSelectStart;
iImage** imgSelectStart;

void drawPopIntroSelectStartBefore(iPopup* me, iPoint p, float dt);

void createPopIntroSelectStart()
{
    int i, j;
    Texture* tex, * t, * text;
    iImage* img;
    iPopup* pop = new iPopup(iPopupStyleMove);
    iSize size;

    imgSelectStart = (iImage**)malloc(sizeof(iImage*) * 3);
    const char* strPath[3] = {
        "assets/intro/introSelectStartNewGame.png",
        "assets/intro/introSelectStartContinue.png",
        "assets/intro/introSelectStartBack.png",
    };

    const char* strButton[3] = {
    "New Game",
    "Continue",
    "Back",
    };

    iGraphics* g = iGraphics::instance();
    size = iSizeMake(devSize.width / 5.0f, devSize.height / 10.0f);
    setStringSize(size.height * 0.5f);
    setStringRGBA(0, 0, 0, 1);
    setStringBorder(0);
    for (i = 0; i < 3; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            g->init(size);
            g->drawString(size.width / 2.0f, size.height / 2.0f,
                VCENTER | HCENTER, strButton[i]);
            text = g->getTexture();

            tex = createTexture(size.width, size.height);

            fbo->bind(tex);
            if (j == 0) setRGBA(1, 1, 1, 1);
            else        setRGBA(0, 1, 0, 1);
            t = createImage("assets/intro/introButton.png");
            DRAWIMAGE(t, size);
            freeImage(t);
            setRGBA(1, 1, 1, 1);
            t = createImage(strPath[i]);
            float s = size.height * 0.33f;
            drawImage(t, 0, size.height / 2.0f,
                0,0,t->width,t->height,
                VCENTER | LEFT, s/t->width, s/t->height,
                2, 0, REVERSE_HEIGHT);
            freeImage(t);
            DRAWIMAGE(text, size);
            freeImage(text);
            fbo->unbind();

            img->addObject(tex);
            freeImage(tex);
        }

        img->position = iPointMake(0, (size.height * 1.2f) * i);
        if (i == 1 && existFile(FILE_PATH) == false)
            img->position = iPointMake(-3000, -3000);
        imgSelectStart[i] = img;
        pop->addObject(img);
    }


    pop->openPosition = INTRO_BUTTON_OPEN_POSITION;
    pop->closePosition = INTRO_BUTTON_CLOSE_POSITION;
    pop->methodDrawBefore = drawPopIntroSelectStartBefore;
    pop->_showDt = 1.0f;
    popSelectStart = pop;

}

void freePopIntroSelectStart()
{
    delete popSelectStart;
    free(imgSelectStart);
}

void showPopIntroSelectStart(bool show)
{
    popSelectStart->show(show);
}

void drawPopIntroSelectStartBefore(iPopup* me, iPoint p, float dt)
{
    for (int i = 0; i < 3; i++)
        imgSelectStart[i]->setTexAtIndex(i == popSelectStart->selected);
}
void drawPopIntroSelectStart(float dt)
{
    popSelectStart->paint(dt);
}

bool keyPopIntroSelectStart(iKeyState stat, iPoint point)
{
    if (popSelectStart->bShow == false)
        return false;

    if (popSelectStart->stat != iPopupStatProc)
        return false;

    int i, j = -1;
    switch (stat)
    {
    case iKeyStateBegan:
    {
        i = popSelectStart->selected;
        if (i == -1)
            break;

        if (i == 0)
        {
            // new start
            isNewGame = true;
            setLoading(gamestat_proc, freeIntro, loadProc);
            audioStop(AUDIO_INTRO);
        }
        else if (i == 1)
        {
            //continue
            isNewGame = false;
            setLoading(gamestat_proc, freeIntro, loadProc);
            audioStop(AUDIO_INTRO);
        }
        else if (i == 2)
        {
            //back
            showPopIntroSelectStart(false);
            showPopIntroButton(true);
        }
        popSelectStart->selected = -1;
        audioPlay(AUDIO_MenuSelected);

        break;
    }
    case iKeyStateMoved:
    {
        for (i = 0; i < 3; i++)
        {
            if (containPoint(point, imgSelectStart[i]->touchRect(popSelectStart->closePosition)))
            {
                j = i;
                break;
            }
        }

        if (popSelectStart->selected != j)
            ;

        popSelectStart->selected = j;

        break;
    }
    case iKeyStateEnded:
    {

        break;
    }
    }

    return true;
}

//------------------------------------------------------------------------
