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
	CInput *Input = CInput_Create(InputDelay, disableJoysticks);
	char *LevelPackCreator = (char*) malloc(sizeof(char)* 21);
	char FileName[FILENAME_MAX];
	FILE *Fp;
	SDL_Rect Rect;
    Rect.w = Buffer->w;
    Rect.h = Buffer->h;
    Rect.x = StartScreenX;
    Rect.y = StartScreenY;
    Tmp = SDL_DisplayFormat(Buffer);
	char *Tekst = (char*) malloc(sizeof(char)* 500);
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

		CInput_Update(Input);
        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;
		//BUT_VOLUP
        if (CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_KP_PLUS]))
        {
            IncVolume();
            CInput_Delay(Input);
        }
		//BUT_VOLMIN
        if (CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_KP_MINUS]))
        {
            DecVolume();
            CInput_Delay(Input);
        }
		//BUT_X BUT_Y BUT_A BUT_B
		if(Input->KeyboardHeld[SDLK_SPACE] || Input->KeyboardHeld[SDLK_RETURN] || Input->KeyboardHeld[SDLK_ESCAPE] || Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_q] ||
           Input->KeyboardHeld[SDLK_x] || Input->KeyboardHeld[SDLK_y] || Input->KeyboardHeld[SDLK_b])
        {
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
	free(Tekst);
	free(LevelPackCreator);
	SDL_FreeSurface(Tmp);
	CInput_Destroy(Input);
}