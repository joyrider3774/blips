#include <SDL.h>
#include <SDL_framerate.h>
#include <SDL_rotozoom.h>
#include "CInput.h"
#include "Common.h"
#include "defines.h"
#include "GameFuncs.h"

void Intro()
{
    SDL_Rect Rect;
    CInput *Input = CInput_Create(10, disableJoysticks);
    Uint8 alpha = 0;
    Uint32 Time=0;
    int IntroScreenNr = 1;
    SDL_Surface *Tmp;
    Tmp = SDL_DisplayFormat(Buffer);
    Time = SDL_GetTicks();
    while (GameState == GSIntro)
    {
        CInput_Update(Input);
		//BUT_A BUT_B  BUT_X BUT_Y
        if(Input->KeyboardHeld[SDLK_SPACE] || Input->KeyboardHeld[SDLK_RETURN] || Input->KeyboardHeld[SDLK_ESCAPE] || Input->KeyboardHeld[SDLK_a] ||
           Input->KeyboardHeld[SDLK_x] || Input->KeyboardHeld[SDLK_y] || Input->KeyboardHeld[SDLK_b] )
        {
            GameState=GSTitleScreen;
        }

        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;
		

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