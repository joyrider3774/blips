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
	SDL_Rect Rect;
    Rect.w = Buffer->w;
    Rect.h = Buffer->h;
    Rect.x = StartScreenX;
    Rect.y = StartScreenY;
    Tmp = SDL_DisplayFormat(Buffer);
	char *Tekst = new char[500];
	sprintf(FileName,"./levelpacks/%s/credits.dat",LevelPackFileName);
	Fp = fopen(FileName,"rt");
	if (Fp)
	{
		fscanf(Fp,"[Credits]\nCreator='%[^']'\n",LevelPackCreator);
		fclose(Fp);
		sprintf(Tekst,"Blips was created by\nWillems Davy - Willems Soft 2008.\nHttp://www.willemssoft.be\n\nLevelpack %s was created\nby %s.",LevelPackName,LevelPackCreator);
	}
	else
		sprintf(Tekst,"Blips was created by\nWillems Davy - Willems Soft 2008.\nHttp://www.willemssoft.be\n\nLevelpack %s was created\nby unknown person.",LevelPackName);
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

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_VOLUP)] || Input->KeyboardHeld[SDLK_KP_PLUS]))
        {
            IncVolume();
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_VOLMIN)] || Input->KeyboardHeld[SDLK_KP_MINUS]))
        {
            DecVolume();
            Input->Delay();
        }

		if(Input->KeyboardHeld[SDLK_SPACE] || Input->KeyboardHeld[SDLK_RETURN] || Input->KeyboardHeld[SDLK_ESCAPE] || Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_q] ||
           Input->KeyboardHeld[SDLK_x] || Input->KeyboardHeld[SDLK_y] || Input->KeyboardHeld[SDLK_b] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] ||
           Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_X)] ||  Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_Y)]  || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_B)] )
        {
			if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_BACK],0);
            GameState=GSTitleScreen;
        }

		boxRGBA(Tmp,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Tmp,61*UI_WIDTH_SCALE,81*UI_HEIGHT_SCALE,259*UI_WIDTH_SCALE,159*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
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
        SDL_FillRect(Buffer1,NULL,SDL_MapRGB(Buffer1->format,0,0,0));
        SDL_BlitSurface(Buffer,NULL,Buffer1,&Rect);
        SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer1,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
		SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
		SDL_FreeSurface(ScreenBufferZoom);
        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);
	}
	delete[] Tekst;
	delete[] LevelPackCreator;
	SDL_FreeSurface(Tmp);
	delete Input;
}