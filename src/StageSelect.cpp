#include <SDL3/SDL.h>
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
    CInput *Input = new CInput(InputDelay, disableJoysticks);
	if (SelectedLevel > 0)
	{
		sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, SelectedLevel);
		if(!FileExists(FileName))
			sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
		WorldParts.Load(FileName);

	}
	else
		WorldParts.RemoveAll();
	while (GameState == GSStageSelect)
	{
		frameticks = SDL_GetPerformanceCounter();
		SDL_SetRenderTarget(Renderer, Buffer);
        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                SelectedMusic = 0;
                Mix_PlayMusic(Music[SelectedMusic],0);
                SetVolume(Volume);
            }
		SDL_RenderTexture(Renderer, IMGBackground,NULL,NULL);
		WorldParts.Draw();
		SDL_FRect Rect;
		Rect.x = 0.0f;
		Rect.y = 0.0f;
		Rect.w = (float)ORIG_WINDOW_WIDTH-1.0f;
		Rect.h = 13.0f*UI_HEIGHT_SCALE;
		SDL_SetRenderDrawColor(Renderer, MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.a);
		SDL_RenderFillRect(Renderer, &Rect);
		SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
		SDL_RenderRect(Renderer, &Rect);
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
		WriteText(font,Tekst,strlen(Tekst),2,0,0,MenuTextColor,false);
		Input->Update();

        if(Input->SpecialsHeld(SPECIAL_QUIT_EV))
            GameState = GSQuit;

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_VOLUP)) || Input->KeyboardHeld(SDLK_KP_PLUS)))
        {
            IncVolume();
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_VOLMIN)) || Input->KeyboardHeld(SDLK_KP_MINUS)))
        {
            DecVolume();
            Input->Delay();
        }

        if(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_Y)) || Input->KeyboardHeld(SDLK_Y))
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

        if(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_B)) || Input->KeyboardHeld(SDLK_ESCAPE))
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
			if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_X)) || Input->KeyboardHeld(SDLK_X) || Input->KeyboardHeld(SDLK_Z)))
			{
				if(LevelEditorMode && (SelectedLevel > 0))
				{
					sprintf(Tekst,"Are you sure you want to delete this level:\n%s - Level %d\n\nPress (A) to Delete (X) to Cancel",LevelPackName,SelectedLevel);
					if (AskQuestion(Tekst))
					{
						sprintf(Tekst,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, SelectedLevel);
						if(FileExists(Tekst))
						{
							remove(Tekst);
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							//only swap levels if we did not edit a level that also exists as a default level
							if(!FileExists(FileName))
							{
								for(Teller=SelectedLevel;Teller<InstalledLevels;Teller++)
								{
									sprintf(Tekst,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, Teller+1);
									sprintf(Tekst1,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName,Teller);                      
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
								sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName,SelectedLevel);
								if(!FileExists(FileName))
									sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
								WorldParts.Load(FileName);
							}
						}
					}
					Input->Reset();
				}
				Input->Delay();
			}

			if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) || Input->KeyboardHeld(SDLK_A) || Input->KeyboardHeld(SDLK_Q) || Input->KeyboardHeld(SDLK_RETURN) || Input->KeyboardHeld(SDLK_SPACE)))
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
							sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, SelectedLevel);
							if(!FileExists(FileName))
								sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName, SelectedLevel);
							WorldParts.Load(FileName);
							GameState = GSGame;
						}
						Input->Reset();
					}
				Input->Delay();
			}

			if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_L)) || Input->KeyboardHeld(SDLK_PAGEDOWN) || Input->KeyboardHeld(SDLK_L)))
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
						sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, SelectedLevel);
						if(!FileExists(FileName))
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							
						WorldParts.Load(FileName);
					}
				}
				else
				{
					if (SelectedLevel != 1)
					{
						SelectedLevel -= 5;
						if (SelectedLevel < 1)
							SelectedLevel = 1;
						sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, SelectedLevel);
						if(!FileExists(FileName))
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							
						WorldParts.Load(FileName);
					}
				}
				Input->Delay();
			}

			if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_R)) || Input->KeyboardHeld(SDLK_PAGEUP) || Input->KeyboardHeld(SDLK_R)))
			{           
				if (SelectedLevel != InstalledLevels)
				{
					SelectedLevel +=5;
					if (SelectedLevel > InstalledLevels)
						SelectedLevel = InstalledLevels;
					sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, SelectedLevel);
					if(!FileExists(FileName))
						sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							
					WorldParts.Load(FileName);
				}
				Input->Delay();
			}

			if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_LEFT)) || Input->KeyboardHeld(SDLK_LEFT)))
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
						sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, SelectedLevel);
						if(!FileExists(FileName))
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							
						WorldParts.Load(FileName);
					}
				}
				else
				{
					if(SelectedLevel != 1)
					{
						SelectedLevel--;
						if (SelectedLevel < 1)
							SelectedLevel = 1;
						sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, SelectedLevel);
						if(!FileExists(FileName))
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);	
						WorldParts.Load(FileName);
					}
				}

				Input->Delay();
			}

			if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_RIGHT)) || Input->KeyboardHeld(SDLK_RIGHT)))
			{
				if(SelectedLevel != InstalledLevels)
				{
					SelectedLevel++;

					if (SelectedLevel > InstalledLevels)
						SelectedLevel = InstalledLevels;
					sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, SelectedLevel);
					if(!FileExists(FileName))
						sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
							
					WorldParts.Load(FileName);
				}
				Input->Delay();
			}
		}

        if (alpha < 255)
        {
            if(alpha+AlphaInc > MaxAlpha)
            {
                alpha = 255;
                SDL_SetTextureAlphaMod(Buffer,alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetTextureAlphaMod(Buffer,alpha);
            }
        }
		if(showfps)
        {
            char fpsText[100];
            sprintf(fpsText, "FPS: %.2f\n", avgfps);
            SDL_FRect Rect;
			Rect.x = 0.0f;
			Rect.y = 0.0f;
			Rect.w = 100.0f;
			Rect.h = (float)TTF_GetFontHeight(font);
            SDL_SetRenderDrawColor(Renderer, 255,255,255,255);
            SDL_RenderFillRect(Renderer, &Rect);
            SDL_Color col = {0,0,0,255};
            WriteText(font, fpsText, strlen(fpsText), 0, 0, 0, col, false);
        }
        SDL_SetRenderTarget(Renderer, Buffer2);
        SDL_RenderTexture(Renderer, Buffer, NULL, NULL);
        SDL_SetRenderTarget(Renderer, NULL);
        SDL_SetRenderDrawColor(Renderer, 0,0,0,255);
        SDL_RenderClear(Renderer);
        SDL_SetRenderLogicalPresentation(Renderer, ORIG_WINDOW_WIDTH, ORIG_WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);        
        SDL_RenderTexture(Renderer, Buffer2, NULL, NULL);
        SDL_RenderPresent(Renderer);
        Uint64 frameEndTicks = SDL_GetPerformanceCounter();
        Uint64 FramePerf = frameEndTicks - frameticks;
        frameTime = (double)FramePerf / (double)SDL_GetPerformanceFrequency() * 1000.0f;
        double delay = 1000.0f / FPS - frameTime;
        if (!nodelay && (delay > 0.0f))
            SDL_Delay((Uint32)(delay)); 
		if (showfps)
		{
			if(skipCounter > 0)
			{
				skipCounter--;
				lastfpstime = SDL_GetTicks();
			}
			else
			{
				framecount++;
				if(SDL_GetTicks() - lastfpstime >= 1000)
				{
					for (int i = FPS_SAMPLES-1; i > 0; i--)
						fpsSamples[i] = fpsSamples[i-1];
					fpsSamples[0] = framecount;
					fpsAvgCount++;
					if(fpsAvgCount > FPS_SAMPLES)
						fpsAvgCount = FPS_SAMPLES;
					int fpsSum = 0;
					for (int i = 0; i < fpsAvgCount; i++)
						fpsSum += fpsSamples[i];
					avgfps = (double)fpsSum / (double)fpsAvgCount;
					framecount = 0;
					lastfpstime = SDL_GetTicks();
				}
			}
		}
	}
	delete[] FileName;
	delete Input;
}
