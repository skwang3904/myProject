#include "Intro.h"

void loadIntro()
{
}

void freeIntro()
{
}

void drawIntro(float dt)
{
}

void keyIntro(iKeyState stat, iPoint point)
{
}

//------------------------------------------------------------------------
#if 0
void shadertest(float dt)
{
    if (st == NULL)
    {
        STInput si =
        {
            "assets/shader/etc/MainCommon.frag",
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
                           "assets/shader/etc/TextFont.vert",
                           "assets/shader/etc/TextFont.frag",
                           {
                               -1, "assets/textfont.png", REPEAT, MIPMAP, true,
                               -1, "assets/background.jpg", CLAMP, MIPMAP, true,
                               -1, NULL, CLAMP, LINEAR, false,
                               -1, NULL, CLAMP, LINEAR, false,
                           }

        };

        st = new iShaderToy(&si);
    }

    st->paint(dt);
}
#endif