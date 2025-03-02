#include <SDL3/SDL.h>
#include "CInput.h"
#include "Common.h"
#include "Defines.h"
#include "GameFuncs.h"

void Intro()
{
    CInput *Input = new CInput(10, disableJoysticks);
    Uint8 alpha = 0;
    Uint32 Time=0;
    int IntroScreenNr = 1;
    SDL_Surface *Tmp;
    Time = SDL_GetTicks();
    SDL_SetRenderTarget(Renderer, Buffer);
    SDL_SetRenderDrawColor(Renderer, 0,0,0,255);
    SDL_RenderClear(Renderer);
    while (GameState == GSIntro)
    {
        SDL_SetRenderTarget(Renderer, Buffer);
        frameticks = SDL_GetPerformanceCounter();
        Input->Update();

        if(Input->KeyboardHeld(SDLK_SPACE) || Input->KeyboardHeld(SDLK_RETURN) || Input->KeyboardHeld(SDLK_ESCAPE) || Input->KeyboardHeld(SDLK_A) ||
           Input->KeyboardHeld(SDLK_X) || Input->KeyboardHeld(SDLK_Z) || Input->KeyboardHeld(SDLK_Y) || Input->KeyboardHeld(SDLK_B) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) ||
           Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_B)) ||  Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_X))  || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_Y)) )
        {
            GameState=GSTitleScreen;
        }

        if(Input->SpecialsHeld(SPECIAL_QUIT_EV))
            GameState = GSQuit;

        //tekenen naar buffer

        switch(IntroScreenNr)
        {
            case 1 :
                SDL_RenderTexture(Renderer, IMGIntro1,NULL,NULL);
                break;
            case 2 :
                SDL_RenderTexture(Renderer, IMGIntro2,NULL,NULL);
                break;
            case 3 :
                SDL_RenderTexture(Renderer, IMGIntro3,NULL,NULL);
                break;
        }

        if (alpha < 255)
        {
            if(alpha+AlphaInc > MaxAlpha)
            {
                alpha = 255;
                SDL_SetTextureAlphaMod(Buffer,alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetTextureAlphaMod(Buffer,alpha);
            }
        }
        if(showfps)
        {
            char fpsText[100];
            sprintf(fpsText, "FPS: %.2f\n", avgfps);
            SDL_FRect Rect = {0, 0, 100, (float)TTF_GetFontHeight(font)};
            SDL_SetRenderDrawColor(Renderer, 255,255,255,255);
            SDL_RenderFillRect(Renderer, &Rect);
            SDL_Color col = {0,0,0,255};
            WriteText(font, fpsText, strlen(fpsText), 0, 0, 0, col, false);
        }
        SDL_SetRenderTarget(Renderer, Buffer2);
        SDL_RenderTexture(Renderer, Buffer, NULL, NULL);
        SDL_SetRenderTarget(Renderer, NULL);
        SDL_SetRenderDrawColor(Renderer, 0,0,0,255);
        SDL_RenderClear(Renderer);
        SDL_SetRenderLogicalPresentation(Renderer, ORIG_WINDOW_WIDTH, ORIG_WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);        
        SDL_RenderTexture(Renderer, Buffer2, NULL, NULL);
        SDL_RenderPresent(Renderer);
        Uint64 frameEndTicks = SDL_GetPerformanceCounter();
        Uint64 FramePerf = frameEndTicks - frameticks;
        frameTime = (double)FramePerf / (double)SDL_GetPerformanceFrequency() * 1000.0f;
        double delay = 1000.0f / FPS - frameTime;
        if (!nodelay && (delay > 0.0f))
            SDL_Delay((Uint32)(delay)); 
        if (showfps)
        {
            if(skipCounter > 0)
            {
                skipCounter--;
                lastfpstime = SDL_GetTicks();
            }
            else
            {
                framecount++;
                if(SDL_GetTicks() - lastfpstime >= 1000)
                {
                    for (int i = FPS_SAMPLES-1; i > 0; i--)
                        fpsSamples[i] = fpsSamples[i-1];
                    fpsSamples[0] = framecount;
                    fpsAvgCount++;
                    if(fpsAvgCount > FPS_SAMPLES)
                        fpsAvgCount = FPS_SAMPLES;
                    int fpsSum = 0;
                    for (int i = 0; i < fpsAvgCount; i++)
                        fpsSum += fpsSamples[i];
                    avgfps = (double)fpsSum / (double)fpsAvgCount;
                    framecount = 0;
                    lastfpstime = SDL_GetTicks();
                }
            }
        }

        if(Time + 3500 < SDL_GetTicks())
        {
            alpha = 0;
            IntroScreenNr++;
            if(IntroScreenNr > 3)
                GameState = GSTitleScreen;
            Time = SDL_GetTicks();
        }

    }
	delete Input;

}