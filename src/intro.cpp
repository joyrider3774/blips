#include <SDL.h>
#include "CInput.h"
#include "Common.h"
#include "Defines.h"
#include "GameFuncs.h"
#include <SDL_framerate.h>
#include <SDL_rotozoom.h>
void Intro()
{
    CInput *Input = new CInput(10, disableJoysticks);
    Uint8 alpha = 0;
    Uint32 Time=0;
    int IntroScreenNr = 1;
    SDL_Surface *Tmp;
    Tmp = SDL_DisplayFormat(Buffer);
    Time = SDL_GetTicks();
    SDL_FillRect(Buffer,NULL,SDL_MapRGB(Buffer->format,0,0,0));
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

        SDL_BlitSurface(Tmp,NULL,Buffer,NULL);
        if ((WINDOW_WIDTH != ORIG_WINDOW_WIDTH) || (WINDOW_HEIGHT != ORIG_WINDOW_HEIGHT))
		{
			double wscale = (double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH;
			if(ORIG_WINDOW_HEIGHT * wscale > WINDOW_HEIGHT)
				wscale = (double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT;
			SDL_Rect dst;
			dst.x = (WINDOW_WIDTH - (ORIG_WINDOW_WIDTH * wscale)) / 2;
			dst.y = (WINDOW_HEIGHT - (ORIG_WINDOW_HEIGHT * wscale)) / 2,
			dst.w = ORIG_WINDOW_WIDTH * wscale;
			dst.h = ORIG_WINDOW_HEIGHT * wscale;
			SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer,wscale,wscale,0);
			SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,&dst);
			SDL_FreeSurface(ScreenBufferZoom);
		}
		else
		{
			SDL_BlitSurface(Buffer, NULL, Screen, NULL);
		}

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
	delete Input;

}