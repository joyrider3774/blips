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
	char *FileName = new char[FILENAME_MAX];
	char Tekst[300];
	char Tekst1[300];
    Tmp = SDL_DisplayFormat(Buffer);
    CInput *Input = new CInput(InputDelay, disableJoysticks);
	if (SelectedLevel > 0)
	{
		sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, SelectedLevel);
		if(!FileExists(FileName))
			sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
		WorldParts.Load(FileName);
		WorldParts.LimitVPLevel();
	}
	else
		WorldParts.RemoveAll();
	while (GameState == GSStageSelect)
	{
        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                SelectedMusic = 0;
                Mix_PlayMusic(Music[SelectedMusic],0);
                SetVolume(Volume);
            }
		SDL_BlitSurface(IMGBackground,NULL,Tmp,NULL);
		WorldParts.Draw(Tmp);
		boxRGBA(Tmp,0,0,ORIG_WINDOW_WIDTH-1,13*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,-1,-1,ORIG_WINDOW_WIDTH,13*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
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

        if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_Y)] || Input->KeyboardHeld[KEY_Y])
        {
            if(LevelEditorMode)
            {
                SelectedLevel = 0;
                WorldParts.RemoveAll();
                printf("worldparts :%d\n",WorldParts.ItemCount);
                LevelHasChanged = false;
                GameState = GSLevelEditor;
            }
        }

        if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_B)] || Input->KeyboardHeld[KEY_B])
        {
			if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_BACK],0);
            if(LevelEditorMode)
                GameState= GSLevelEditorMenu;
            else
                GameState= GSTitleScreen;
            WorldParts.RemoveAll();
        }
		//check on gamestate to fix bug where B (above here) and A are pressed at same time when comming from gamestate
		//it would result in clearing the worldparts but still going back to Game Gamestate with no level loaded !
		if(GameState == GSStageSelect)
		{
			if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_X)] || Input->KeyboardHeld[KEY_X] || Input->KeyboardHeld[SDLK_z]))
			{
				if(LevelEditorMode && (SelectedLevel > 0))
				{
					if (GlobalSoundEnabled)
                	    Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
					sprintf(Tekst,"Are you sure you want to delete this level:\n%s - Level %d\n\nPress (A) to Delete (X) to Cancel",LevelPackName,SelectedLevel);
					if (AskQuestion(Tekst))
					{
						sprintf(Tekst,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, SelectedLevel);
						if(FileExists(Tekst))
						{
							remove(Tekst);
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							//only swap levels if we did not edit a level that also exists as a default level
							if(!FileExists(FileName))
							{
								for(Teller=SelectedLevel;Teller<InstalledLevels;Teller++)
								{
									sprintf(Tekst,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, Teller+1);
									sprintf(Tekst1,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName,Teller);                      
									rename(Tekst,Tekst1);
								}
							}
							FindLevels();
							if (SelectedLevel > InstalledLevels)
								SelectedLevel = InstalledLevels;
							if (SelectedLevel==0)
								WorldParts.RemoveAll();
							else
							{
								sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev",getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName,SelectedLevel);
								if(!FileExists(FileName))
									sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
								WorldParts.Load(FileName);
								WorldParts.LimitVPLevel();
							}
						}
					}
					if (GlobalSoundEnabled)
						Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
					Input->Reset();
				}
				Input->Delay();
			}

			if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] || Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_q] || Input->KeyboardHeld[KEY_START] || Input->KeyboardHeld[KEY_A]))
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
							sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, SelectedLevel);
							if(!FileExists(FileName))
								sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName, SelectedLevel);
							WorldParts.Load(FileName);
							WorldParts.LimitVPLevel();
							GameState = GSGame;							
						}
						if (GlobalSoundEnabled)
							Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
						Input->Reset();
					}
				Input->Delay();
			}

			if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_L)] || Input->KeyboardHeld[KEY_L] || Input->KeyboardHeld[SDLK_l]))
			{
				if(LevelEditorMode)
				{
					SelectedLevel -= 5;
					if (SelectedLevel <= 0)
					{
						SelectedLevel = 0;
						WorldParts.RemoveAll();
					}
					else
					{
						sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, SelectedLevel);
						if(!FileExists(FileName))
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							
						WorldParts.Load(FileName);
						WorldParts.LimitVPLevel();
					}
				}
				else
				{
					if (SelectedLevel != 1)
					{
						SelectedLevel -= 5;
						if (SelectedLevel < 1)
							SelectedLevel = 1;
						sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, SelectedLevel);
						if(!FileExists(FileName))
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							
						WorldParts.Load(FileName);
						WorldParts.LimitVPLevel();
					}
				}
				Input->Delay();
			}

			if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_R)] || Input->KeyboardHeld[KEY_R] || Input->KeyboardHeld[SDLK_r]))
			{           
				if (SelectedLevel != InstalledLevels)
				{
					SelectedLevel +=5;
					if (SelectedLevel > InstalledLevels)
						SelectedLevel = InstalledLevels;
					sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, SelectedLevel);
					if(!FileExists(FileName))
						sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							
					WorldParts.Load(FileName);
					WorldParts.LimitVPLevel();
				}
				Input->Delay();
			}

			if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[KEY_LEFT]))
			{
				
				if(LevelEditorMode)
				{	
					SelectedLevel--;
					if (SelectedLevel <= 0)
					{
						SelectedLevel = 0;
						WorldParts.RemoveAll();
					}
					else
					{
						sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, SelectedLevel);
						if(!FileExists(FileName))
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							
						WorldParts.Load(FileName);
						WorldParts.LimitVPLevel();
					}
				}
				else
				{
					if(SelectedLevel != 1)
					{
						SelectedLevel--;
						if (SelectedLevel < 1)
							SelectedLevel = 1;
						sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, SelectedLevel);
						if(!FileExists(FileName))
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);	
						WorldParts.Load(FileName);
						WorldParts.LimitVPLevel();
					}
				}

				Input->Delay();
			}

			if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_RIGHT)] || Input->KeyboardHeld[KEY_RIGHT]))
			{
				if(SelectedLevel != InstalledLevels)
				{
					SelectedLevel++;

					if (SelectedLevel > InstalledLevels)
						SelectedLevel = InstalledLevels;
					sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, SelectedLevel);
					if(!FileExists(FileName))
						sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							
					WorldParts.Load(FileName);
					WorldParts.LimitVPLevel();
				}
				Input->Delay();
			}
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
		HandleFPS();
		SDL_Flip(Screen);
        if(!noDelay)
            SDL_framerateDelay(&Fpsman);

	}
	delete[] FileName;
	SDL_FreeSurface(Tmp);
	delete Input;
}
