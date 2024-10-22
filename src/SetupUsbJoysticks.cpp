#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_mixer.h>
#include <SDL_framerate.h>
#include <SDL_rotozoom.h>
#include "Common.h"
#include "GameFuncs.h"
#include "CInput.h"

void SetupUsbJoystickButtons()
{
    int alpha = 0;
    SDL_Surface *Tmp,*Tmp1;
	int Teller, Selection = 0;
	char *Tekst = new char[300];
	Tmp1 = SDL_CreateRGBSurface(SDL_SWSURFACE,ORIG_WINDOW_WIDTH,ORIG_WINDOW_HEIGHT,16,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
	Tmp = SDL_DisplayFormat(Tmp1);
	SDL_FreeSurface(Tmp1);
	bool done = false;
	CInput *Input = new CInput(10, disableJoysticks);
	while (GameState == GSSetupUsbJoystickButtons)
	{
        if(MusicCount > 0)
		{
            if(GlobalSoundEnabled)
			{
                if(!Mix_PlayingMusic())
                {
                    SelectedMusic = 0;
                    Mix_PlayMusic(Music[SelectedMusic],0);
                    SetVolume(Volume);
                }
			}
		}

		Input->Update();

		if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;

		if(Input->KeyboardHeld[SDLK_ESCAPE] ||  Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_B)])
		{
			if (GlobalSoundEnabled)
            	Mix_PlayChannel(-1,Sounds[SND_BACK],0); 
            GameState = GSTitleScreen;
		}

        if(Input->Ready() && (Input->KeyboardHeld[SDLK_UP] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_UP)]))
        {
            Selection--;
            if( Selection < 0)
                Selection = NROFBUTTONS-1;
            if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            Input->Delay();
        }

        if (Input->KeyboardHeld[SDLK_RETURN])
            JoystickSetup->ResetToDefaults();

        if(Input->Ready() && (Input->KeyboardHeld[SDLK_DOWN] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_DOWN)]))
        {
            Selection++;
            if( Selection >=NROFBUTTONS)
                Selection = 0;
            if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            Input->Delay();
        }

        SDL_BlitSurface(IMGTitleScreen,NULL,Tmp,NULL);
        boxRGBA(Tmp,45*UI_WIDTH_SCALE,70*UI_HEIGHT_SCALE,295*UI_WIDTH_SCALE,193*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,45*UI_WIDTH_SCALE,70*UI_HEIGHT_SCALE,295*UI_WIDTH_SCALE,193*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Tmp,46*UI_WIDTH_SCALE,71*UI_HEIGHT_SCALE,294*UI_WIDTH_SCALE,192*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
        JoystickSetup->DrawCurrentSetup(Tmp,font,55*UI_WIDTH_SCALE,73*UI_HEIGHT_SCALE,155*UI_WIDTH_SCALE,8*UI_HEIGHT_SCALE,Selection,MenuTextColor,MenuTextColor);
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
			SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
			SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
			SDL_FreeSurface(ScreenBufferZoom);
		}
		else
		{
			SDL_BlitSurface(Buffer, NULL, Screen, NULL);
		}
        SDL_Flip(Screen);

        if(Input->Ready() && (Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_SPACE] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)]))
        {
            if (alpha == 255)
            {
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                Input->Reset();
                while (!done)
                {
                    Input->Update();
                    SDL_BlitSurface(IMGTitleScreen,NULL,Tmp,NULL);
                    boxRGBA(Tmp,45*UI_WIDTH_SCALE,70*UI_HEIGHT_SCALE,295*UI_WIDTH_SCALE,193*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
                    rectangleRGBA(Tmp,45*UI_WIDTH_SCALE,70*UI_HEIGHT_SCALE,295*UI_WIDTH_SCALE,193*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
                    rectangleRGBA(Tmp,46*UI_WIDTH_SCALE,71*UI_HEIGHT_SCALE,294*UI_WIDTH_SCALE,192*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);


                    JoystickSetup->DrawCurrentSetup(Tmp,font,55*UI_WIDTH_SCALE,73*UI_HEIGHT_SCALE,155*UI_WIDTH_SCALE,8*UI_HEIGHT_SCALE,Selection,MenuTextColor,MenuBoxBorderColor);
                    SDL_BlitSurface(Tmp,NULL,Buffer,NULL);
                    SDL_FillRect(Screen,NULL,SDL_MapRGB(Screen->format,0,0,0));
                    if ((WINDOW_WIDTH != ORIG_WINDOW_WIDTH) || (WINDOW_HEIGHT != ORIG_WINDOW_HEIGHT))
					{
						SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
						SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
						SDL_FreeSurface(ScreenBufferZoom);
					}
					else
					{
						SDL_BlitSurface(Buffer, NULL, Screen, NULL);
					}
                    SDL_Flip(Screen);
                    for (Teller = 0;Teller<MAXJOYSTICKBUTTONS;Teller++)
                        if(Input->JoystickHeld[0][Teller])
                        {
                            done = true;
                            JoystickSetup->SetButtonValue(Selection,Teller);
                            break;
                        }
                    if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
                    {
                        GameState = GSQuit;
                        done = true;
                    }

                    if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] || Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_SPACE]))
                        done = true;

                    if(Input->KeyboardHeld[SDLK_ESCAPE] || Input->SpecialsHeld[SPECIAL_QUIT_EV] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_B)])
                        done= true;
                    SDL_framerateDelay(&Fpsman);
                }
                Input->Reset();
                Input->Delay();
                done= false;
            }
        }
        SDL_framerateDelay(&Fpsman);

	}
	delete[] Tekst;
	delete Input;
	SDL_FreeSurface(Tmp);
}

