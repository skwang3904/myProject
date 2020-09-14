#include "Intro.h"

void loadIntro()
{
    loadIntroShader();
}

void freeIntro()
{
    freeIntroShader();
}

void drawIntro(float dt)
{
    drawIntroShader(dt);
}

void keyIntro(iKeyState stat, iPoint point)
{
}

//------------------------------------------------------------------------
// Shader
void loadIntroShader()
{
    if (shaderToy == NULL)
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
