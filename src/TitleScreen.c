#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
#include "CInput.h"
#include "Common.h"
#include "GameFuncs.h"

void TitleScreen()
{
	int alpha = 0;
    SDL_Surface *Tmp;
	int Teller, Selection = 1;
	CInput *Input = CInput_Create(InputDelay, disableJoysticks);
	SDL_Rect Rect;
	char *Tekst = (char*) malloc(sizeof(char)* 300);
	Tmp = SDL_DisplayFormat(Buffer);
	while (GameState == GSTitleScreen)
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
		SDL_BlitSurface(IMGTitleScreen,NULL,Tmp,NULL);
		CInput_Update(Input);
		//BUT_SELECT
		if(Input->KeyboardHeld[SDLK_ESCAPE] || Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;
		//BUT_VOLUP
		if (CInput_Ready(Input) && ( Input->KeyboardHeld[SDLK_KP_PLUS]))
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
		//BUT_LEFT
        if (CInput_Ready(Input) && ( Input->KeyboardHeld[SDLK_LEFT]))
        {
            if(Selection==3)
                if (InstalledLevelPacksCount > 0)
                    if(SelectedLevelPack > 0)
                    {
                        SelectedLevelPack--;
                        sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        AddUnderScores(LevelPackFileName);
                    }
            CInput_Delay(Input);
        }
		//BUT_RIGHT
        if (CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_RIGHT]))
        {
            if (Selection==3)
                if (InstalledLevelPacksCount > 0)
                    if(SelectedLevelPack < InstalledLevelPacksCount-1)
                    {
                        SelectedLevelPack++;
                        sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        AddUnderScores(LevelPackFileName);
                    }
            CInput_Delay(Input);
        }
		//BUT_DOWN 
        if(CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_DOWN]))
        {
            if (Selection < 6)
            {
                Selection++;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            }

            CInput_Delay(Input);
        }
		//BUT_UP
        if(CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_UP]))
        {
            if (Selection > 1)
            {
                Selection--;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            }
            CInput_Delay(Input);
        }
		//BUT_A
        if(Input->KeyboardHeld[SDLK_SPACE] || Input->KeyboardHeld[SDLK_RETURN])
        {
            switch(Selection)
            {
                case 1:
                    if (InstalledLevelPacksCount >0)
                    {
                        FindLevels();
                        if(InstalledLevels > 0)
                        {
                            LoadUnlockData();
                            SelectedLevel=UnlockedLevels;
                            LevelEditorMode=false;
                            GameState=GSStageSelect;
                            if (GlobalSoundEnabled)
                                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                        }
                        else
                        {
                            if (GlobalSoundEnabled)
                                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                            SDL_BlitSurface(IMGTitleScreen,NULL,Buffer,NULL);
                            sprintf(Tekst,"There are no levels found in levelpack\n%s\n\nPlease create a level for this level pack\nfirst!",LevelPackName);
                            PrintForm(Tekst);
                            CInput_Reset(Input);
                        }

                    	char *FileName =(char*) malloc(sizeof(char)* FILENAME_MAX);
						for (SelectedLevel = 0; SelectedLevel <= InstalledLevels; SelectedLevel++)
						{
							if (SelectedLevel > 0)
							{
								sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
								CWorldParts_Load(WorldParts, FileName);
								sprintf(FileName,"./levelpacks/%s/u32_level%d.lev",LevelPackFileName,SelectedLevel);
								CWorldParts_Save_vircon(WorldParts, FileName);
							}
						}
						free(FileName);                        
                    }
                    break;
                case 4:
                    GameState=GSCredits;
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    break;
                case 6:
                    GameState = GSQuit;
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    break;

            }

        }
   		boxRGBA(Tmp,60*UI_WIDTH_SCALE,70*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,165*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,60*UI_WIDTH_SCALE,70*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,165*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Tmp,61*UI_WIDTH_SCALE,71*UI_HEIGHT_SCALE,259*UI_WIDTH_SCALE,164*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		sprintf(Tekst,"Play Selected LevelPack\nLevel Editor\n<%s>\nCredits\nSetup External Joystick\nQuit",LevelPackName);
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),90*UI_WIDTH_SCALE,75*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		if (Selection > 1)
		{
			sprintf(Tekst,"\n");
			for(Teller=2;Teller<Selection;Teller++)
				sprintf(Tekst,"%s%s",Tekst,"\n");
			sprintf(Tekst,"%s%s",Tekst,">>");
		}
		else
			sprintf(Tekst,">>");
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),65*UI_WIDTH_SCALE,75*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		Rect.w = Buffer->w;
        Rect.h = Buffer->h;
        Rect.x = StartScreenX;
        Rect.y = StartScreenY;
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
        SDL_BlitSurface(Buffer1,NULL,Screen,NULL);
		SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer1,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
        SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
        SDL_FreeSurface(ScreenBufferZoom);
        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);
	}
	free(Tekst);
	SDL_FreeSurface(Tmp);
	CInput_Destroy(Input);
}
