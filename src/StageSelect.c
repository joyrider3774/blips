#include <SDL.h>
#include <SDL_framerate.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
#include "CInput.h"
#include "Common.h"
#include "GameFuncs.h"
#include "CWorldParts.h"

void StageSelect()
{
    SDL_Surface *Tmp;
	SDL_Event Event;
	SDL_PollEvent(&Event);
	int Teller;
	int alpha = 0;
	char *FileName =(char*) malloc(sizeof(char)* FILENAME_MAX);
	char Tekst[300];
	char Tekst1[300];
	SDL_Rect Rect;
    Rect.w = Buffer->w;
    Rect.h = Buffer->h;
    Rect.x = StartScreenX;
    Rect.y = StartScreenY;
    Tmp = SDL_DisplayFormat(Buffer);
    CInput *Input = CInput_Create(InputDelay, disableJoysticks);
	if (SelectedLevel > 0)
	{
		sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
		CWorldParts_Load(WorldParts, FileName);

	}
	else
		CWorldParts_RemoveAll(WorldParts);
	while (GameState == GSStageSelect)
	{
        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                SelectedMusic = 0;
                Mix_PlayMusic(Music[SelectedMusic],0);
				//Mix_HookMusicFinished(MusicFinished);
                SetVolume(Volume);
            }
		SDL_BlitSurface(IMGBackground,NULL,Tmp,NULL);
		CWorldParts_Draw(WorldParts,Tmp);
		boxRGBA(Tmp,0,0,ORIG_WINDOW_WIDTH-1,13*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,0,-1,ORIG_WINDOW_WIDTH-1,13*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		if (SelectedLevel ==0)
			sprintf(Tekst,"Level Pack: %s -> %d Levels - (A) Create New Level",LevelPackName,InstalledLevels);
		else
			if(LevelEditorMode)
				sprintf(Tekst,"Level Pack: %s Level:%d/%d - (A) Edit Level (X) Delete Level",LevelPackName,SelectedLevel,InstalledLevels);
			else
				if(SelectedLevel <= UnlockedLevels)
					sprintf(Tekst,"Level Pack: %s Level:%d/%d - (A) Play Level",LevelPackName,SelectedLevel,InstalledLevels);
				else
					sprintf(Tekst,"Level Pack: %s Level:%d/%d - Level is locked!",LevelPackName,SelectedLevel,InstalledLevels);
		WriteText(Tmp,font,Tekst,strlen(Tekst),2,0,0,MenuTextColor,false);
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
		//BUT_Y
        if(Input->KeyboardHeld[SDLK_y])
        {
            if(LevelEditorMode)
            {
                SelectedLevel = 0;
                CWorldParts_RemoveAll(WorldParts);
                printf("worldparts :%d\n",WorldParts->ItemCount);
                LevelHasChanged = false;
                GameState = GSLevelEditor;
            }
           // else
                //printf("not leveleditor mode\n");
        }
		//BUT_SELECT
        if(Input->KeyboardHeld[SDLK_ESCAPE])
        {
            if(LevelEditorMode)
                GameState= GSLevelEditorMenu;
            else
                GameState= GSTitleScreen;
            CWorldParts_RemoveAll(WorldParts);
        }
		//BUT_X
        if(CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_x]))
        {
            if(LevelEditorMode && (SelectedLevel > 0))
            {
                sprintf(Tekst,"Are you sure you want to delete this level:\n%s - Level %d\n\nPress (A) to Delete (X) to Cancel",LevelPackName,SelectedLevel);
                if (AskQuestion(Tekst))
                {
                    sprintf(Tekst,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                    //system(Command);
                    remove(Tekst);
                    //printf("%s\n",Command);
                    for(Teller=SelectedLevel;Teller<InstalledLevels;Teller++)
                    {
                        sprintf(Tekst,"./levelpacks/%s/level%d.lev",LevelPackFileName,Teller+1);
                        sprintf(Tekst1,"./levelpacks/%s/level%d.lev",LevelPackFileName,Teller);
                        rename(Tekst,Tekst1);
                      //  system(Command);
                        //printf("%s\n",Command);
                    }
                    InstalledLevels--;
                    if (SelectedLevel > InstalledLevels)
                        SelectedLevel = InstalledLevels;
                    if (SelectedLevel==0)
                        CWorldParts_RemoveAll(WorldParts);
                    else
                    {
                        sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                        CWorldParts_Load(WorldParts,FileName);
                    }
                }
                CInput_Reset(Input);
            }
            CInput_Delay(Input);
        }
		//BUT_A
        if(CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_q] || Input->KeyboardHeld[SDLK_RETURN] || Input->KeyboardHeld[SDLK_SPACE]))
        {
            if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
            if(LevelEditorMode)
            {
                LevelHasChanged = false;
                GameState = GSLevelEditor;
            }
            else
                if (SelectedLevel <= UnlockedLevels)
                    GameState = GSGame;
                else
                {
                    sprintf(Tekst,"This Level Hasn't been unlocked yet!\nDo you want to play the last unlocked\nlevel %d/%d\n\nPress (A) to Play (X) to Cancel",UnlockedLevels,InstalledLevels);
                    if	(AskQuestion(Tekst))
                    {
                        SelectedLevel = UnlockedLevels;
                        sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                        CWorldParts_Load(WorldParts,FileName);
                        GameState = GSGame;
                    }
                    CInput_Reset(Input);
                }
            CInput_Delay(Input);
        }
		//BUT_L
        if (CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_PAGEDOWN] || Input->KeyboardHeld[SDLK_l]))
        {
            SelectedLevel -= 5;
            if(LevelEditorMode)
            {
                if (SelectedLevel <= 0)
                {
                    SelectedLevel = 0;
                    CWorldParts_RemoveAll(WorldParts);
                }
                else
                {
                    sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                    CWorldParts_Load(WorldParts,FileName);
                }
            }
            else
            {
                if (SelectedLevel < 1)
                    SelectedLevel = 1;
                sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                CWorldParts_Load(WorldParts,FileName);
            }
            CInput_Delay(Input);
        }
		//BUT_R
        if (CInput_Ready(Input) && ( Input->KeyboardHeld[SDLK_PAGEUP] || Input->KeyboardHeld[SDLK_r]))
        {
            SelectedLevel +=5;
            if (SelectedLevel > InstalledLevels)
                    SelectedLevel = InstalledLevels;
            sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
            CWorldParts_Load(WorldParts,FileName);
            CInput_Delay(Input);
        }
		//BUT_LEFT
        if (CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_LEFT]))
        {
            SelectedLevel--;
            if(LevelEditorMode)
            {
                if (SelectedLevel <= 0)
                {
                    SelectedLevel = 0;
                    CWorldParts_RemoveAll(WorldParts);
                }
                else
                {
                    sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                    CWorldParts_Load(WorldParts,FileName);
                }
            }
            else
            {
                if (SelectedLevel < 1)
                    SelectedLevel = 1;
                sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                CWorldParts_Load(WorldParts,FileName);
            }

            CInput_Delay(Input);
        }
		//BUT_RIGHT
        if (CInput_Ready(Input) && ( Input->KeyboardHeld[SDLK_RIGHT]))
        {
            SelectedLevel++;
            if (SelectedLevel > InstalledLevels)
                SelectedLevel = InstalledLevels;
            sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
            CWorldParts_Load(WorldParts,FileName);
            CInput_Delay(Input);
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
        SDL_FillRect(Buffer1,NULL,SDL_MapRGB(Buffer1->format,0,0,0));
        SDL_BlitSurface(Buffer,NULL,Buffer1,&Rect);
        SDL_BlitSurface(Buffer1,NULL,Screen,NULL);
        SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer1,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
        SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
        SDL_FreeSurface(ScreenBufferZoom);
		SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);

	}
	free(FileName);
	SDL_FreeSurface(Tmp);
	CInput_Destroy(Input);
}
