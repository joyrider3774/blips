#include <SDL.h>
#include "CInput.h"
#include "Common.h"
#include "Defines.h"
#include "GameFuncs.h"
#include <SDL_framerate.h>
#include <SDL_rotozoom.h>
void Intro()
{
    SDL_Rect Rect;
    CInput *Input = new CInput(10, disableJoysticks);
    Uint8 alpha = 0;
    Uint32 Time=0;
    int IntroScreenNr = 1;
    SDL_Surface *Tmp;
    Tmp = SDL_DisplayFormat(Buffer);
    Time = SDL_GetTicks();
    while (GameState == GSIntro)
    {
        Input->Update();

        if(Input->KeyboardHeld[SDLK_SPACE] || Input->KeyboardHeld[SDLK_RETURN] || Input->KeyboardHeld[SDLK_ESCAPE] || Input->KeyboardHeld[SDLK_a] ||
           Input->KeyboardHeld[SDLK_x] || Input->KeyboardHeld[SDLK_z] || Input->KeyboardHeld[SDLK_y] || Input->KeyboardHeld[SDLK_b] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] ||
           Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_B)] ||  Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_X)]  || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_Y)] )
        {
            GameState=GSTitleScreen;
        }

        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;

        if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_R)] || Input->KeyboardHeld[SDLK_r])
        {
            if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[SDLK_LEFT]))
            {
                if(StartScreenX - 2 >= 0)
                    StartScreenX -=2;
                Input->Delay();
            }

            if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_RIGHT)] || Input->KeyboardHeld[SDLK_RIGHT]))
            {
                if(StartScreenX + 2 + Buffer->w <= Screen->w)
                    StartScreenX +=2;
                Input->Delay();
            }

            if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_UP)] || Input->KeyboardHeld[SDLK_UP]))
            {
                if(StartScreenY - 2 >=0)
                    StartScreenY -=2;
                Input->Delay();
            }

            if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_DOWN)] || Input->KeyboardHeld[SDLK_DOWN]))
            {
                if(StartScreenY + 2 + Buffer->h <= Screen->h)
                    StartScreenY +=2;
                Input->Delay();
            }
        }


        //tekenen naar buffer

        switch(IntroScreenNr)
        {
            case 1 :
                SDL_BlitSurface(IMGIntro1,NULL,Tmp,NULL);
                break;
            case 2 :
                SDL_BlitSurface(IMGIntro2,NULL,Tmp,NULL);
                break;
            case 3 :
                SDL_BlitSurface(IMGIntro3,NULL,Tmp,NULL);
                break;
        }

        if (alpha < 255)
        {
            if(alpha+AlphaInc > MaxAlpha)
            {
                alpha = 255;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetAlpha(Tmp,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
            }
        }




        Rect.w = Buffer->w;
        Rect.h = Buffer->h;
        Rect.x = StartScreenX;
        Rect.y = StartScreenY;


        SDL_FillRect(Buffer1,NULL,SDL_MapRGB(Buffer1->format,0,0,0));
        SDL_BlitSurface(Tmp,NULL,Buffer,NULL);
        SDL_BlitSurface(Buffer,NULL,Buffer1,&Rect);
      	SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer1,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
        SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
        SDL_FreeSurface(ScreenBufferZoom);

        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);

        if(Time + 3500 < SDL_GetTicks())
        {
            alpha = 0;
            IntroScreenNr++;
            if(IntroScreenNr > 3)
                GameState = GSTitleScreen;
            Time = SDL_GetTicks();
        }

    }
    SDL_FreeSurface(Tmp);

}