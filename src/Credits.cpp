#include <SDL.h>
#include <stdio.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
#include "CInput.h"
#include "Common.h"
#include "GameFuncs.h"

void Credits()
{
    int alpha = 0;
    SDL_Surface *Tmp;
	CInput *Input = new CInput(InputDelay, disableJoysticks);
	char *LevelPackCreator = new char[21];
	char FileName[FILENAME_MAX];
	FILE *Fp;
    Tmp = SDL_DisplayFormat(Buffer);    
	char *Tekst = new char[500];
	sprintf(FileName,"%s/.blips_levelpacks/%s/credits.dat", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName);
	if(!FileExists(FileName))
		sprintf(FileName,"./levelpacks/%s/credits.dat",LevelPackFileName);		
	Fp = fopen(FileName,"rt");
	if (Fp)
	{
		fscanf(Fp,"[Credits]\nCreator='%[^']'\n",LevelPackCreator);
		fclose(Fp);
		sprintf(Tekst,"Blips was created by\nWillems Davy - Willems Soft 2008-2025.\nHttps://joyrider3774.itch.io\n\nLevelpack %s was created\nby %s.",LevelPackName,LevelPackCreator);
	}
	else
		sprintf(Tekst,"Blips was created by\nWillems Davy - Willems Soft 2008-2025.\nHttps://joyrider3774.itch.io\n\nLevelpack %s was created\nby unknown person.",LevelPackName);
	while (GameState == GSCredits)
	{
	    if(GlobalSoundEnabled)
		if(!Mix_PlayingMusic())
        {
            SelectedMusic = 0;
            Mix_PlayMusic(Music[SelectedMusic],0);
            SetVolume(Volume);
        }
		SDL_BlitSurface(IMGTitleScreen,NULL,Tmp,NULL);

		Input->Update();
        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_VOLUP)] || Input->KeyboardHeld[KEY_VOLUP]))
        {
            IncVolume();
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_VOLMIN)] || Input->KeyboardHeld[KEY_VOLMIN]))
        {
            DecVolume();
            Input->Delay();
        }

		if(Input->KeyboardHeld[KEY_A] || Input->KeyboardHeld[KEY_START] || Input->KeyboardHeld[KEY_B] || Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_q] ||
           Input->KeyboardHeld[KEY_X] || Input->KeyboardHeld[SDLK_z] || Input->KeyboardHeld[KEY_Y] || Input->KeyboardHeld[KEY_SELECT] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] ||
           Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_X)] ||  Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_Y)]  || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_B)] )
        {
			if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_BACK],0);
            GameState=GSTitleScreen;
        }

		boxRGBA(Tmp,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Tmp,62*UI_WIDTH_SCALE,82*UI_HEIGHT_SCALE,258*UI_WIDTH_SCALE,158*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		WriteText(Tmp,font,Tekst,strlen(Tekst),65*UI_WIDTH_SCALE,82*UI_HEIGHT_SCALE,1,MenuTextColor,false);
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
			dst.y =(WINDOW_HEIGHT - (ORIG_WINDOW_HEIGHT * wscale)) / 2,
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
        HandleFPS();
        SDL_Flip(Screen);
        if(!noDelay)
            SDL_framerateDelay(&Fpsman);
	}
	delete[] Tekst;
	delete[] LevelPackCreator;
	SDL_FreeSurface(Tmp);
	delete Input;
}