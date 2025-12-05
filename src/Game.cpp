#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "Common.h"
#include "GameFuncs.h"
#include "CInput.h"
#include "CPlayer.h"

bool StageDone()
{
	int Teller;
	for (Teller=0;Teller<WorldParts.ItemCount;Teller++)
		if (WorldParts.Items[Teller]->GetType() == IDDiamond)
		{
			return false;
		}
    return true;
}

void Game()
{
	bool ExplosionsFound =false;
	char FileName[FILENAME_MAX];
	char Text[500];
	int alpha=0,teller;
	Uint32 Time=0;
	CPlayer *Player=NULL;
    CPlayer2 *Player2=NULL;;
	bool ResetViewPort=false;
	for (teller=0;teller<WorldParts.ItemCount;teller++)
	{
		if (WorldParts.Items[teller]->GetType() == IDPlayer)
		{
			Player = (CPlayer*) WorldParts.Items[teller];
			break;
		}
        if (WorldParts.Items[teller]->GetType() == IDPlayer2)
		{
			Player2 = (CPlayer2*) WorldParts.Items[teller];
		}
	}
	//should never happen
	if(!Player && !Player2)
	{
		Player = new CPlayer(0,0);
		WorldParts.Add(Player);
	}
	Time = SDL_GetTicks();
	if (MusicCount > 1)
 	{

		if(GlobalSoundEnabled)
		{
		    SelectedMusic =	1+SDL_rand(MusicCount-1);
			Mix_PlayMusic(Music[SelectedMusic],0);
			SetVolume(Volume);
		}
	}
    WorldParts.LimitVPLevel();
    while (GameState == GSGame)
    {
        frameticks = SDL_GetPerformanceCounter();
        if(!Mix_PlayingMusic())
            if(GlobalSoundEnabled)
            {
                if (MusicCount > 1)
                {
                    SelectedMusic =	1+SDL_rand(MusicCount-1);
                    Mix_PlayMusic(Music[SelectedMusic],0);
                    SetVolume(Volume);
                }
                else
                    if(MusicCount ==1)
                    {
                        SelectedMusic =	0;
                        Mix_PlayMusic(Music[SelectedMusic],0);
                        SetVolume(Volume);
                    }

            }

        Input->Update();

        if(Input->SpecialsHeld(SPECIAL_QUIT_EV))
            GameState = GSQuit;

        if (Input->Ready() && ((Input->KeyboardHeld(SDLK_LALT) || Input->KeyboardHeld(SDLK_RALT)) && Input->KeyboardHeld(SDLK_RETURN)))
        {
            fullScreen = !fullScreen;
            SDL_SetWindowFullscreen(SdlWindow, fullScreen);
            Input->Delay();
            continue;
        }
            
        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_VOLUP)) || Input->KeyboardHeld(SDLK_KP_PLUS)))
        {
            IncVolume();
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0,JoystickSetup->GetButtonValue(BUT_VOLMIN)) || Input->KeyboardHeld(SDLK_KP_MINUS)))
        {
            DecVolume();
            Input->Delay();
        }

        if(Input->Ready() && Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_B)) || Input->KeyboardHeld(SDLK_ESCAPE))
        {
            if(!LevelEditorMode)
			{
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_BACK],0);
                    
                if (AskQuestion("Are you sure you want to return to the stage selector?\n\nPress (A) to confirm (X) to Cancel"))
                {
                    GameState = GSStageSelect;
                }
                if (GlobalSoundEnabled)
                	Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                Input->Reset();
                
			}
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_START)) || Input->KeyboardHeld(SDLK_RETURN)))
        {
            if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_BACK],0);
                    
            if(LevelEditorMode)
			{				
                if (AskQuestion("Are you sure you want to return to level editor?\n\nPress (A) to confirm (X) to Cancel"))
                {
                    GameState=GSLevelEditor;
                }
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                Input->Reset();
			}
            else
            {
                if (AskQuestion("Are you sure you want to reload the level?\n\nPress (A) to confirm (X) to Cancel"))
			    {
                    char *TmpPath = assetPath("levelpacks");
                    sprintf(FileName,"%s/%s/level%d.lev",TmpPath,LevelPackFileName,SelectedLevel);
                    SDL_free(TmpPath);
                    WorldParts.Load(FileName);
                    //need to find player again
                    for (teller=0;teller<WorldParts.ItemCount;teller++)
                    {
                        if (WorldParts.Items[teller]->GetType() == IDPlayer)
                        {
                            Player = (CPlayer*) WorldParts.Items[teller];
                            break;
                        }
                    }
                }
                if (GlobalSoundEnabled)
					Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                Input->Reset();
            }
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_R)) || Input->KeyboardHeld(SDLK_PAGEUP) || Input->KeyboardHeld(SDLK_R)))
        {
            if (MusicCount > 1)
            {
                SelectedMusic++;
                if (SelectedMusic >= MusicCount)
                    SelectedMusic = 1;
                if(GlobalSoundEnabled)
                {
                    Mix_HaltMusic();
                    Mix_PlayMusic(Music[SelectedMusic],0);
                    SetVolume(Volume);
                }
            }
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_LEFT)) || Input->KeyboardHeld(SDLK_LEFT)))
        {
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_RIGHT)) || Input->KeyboardHeld(SDLK_RIGHT)))
        {

            Input->Delay();
        }

        if (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_DOWN))|| Input->KeyboardHeld(SDLK_DOWN))
        {
            Input->Delay();
        }

        //move up
        if (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_UP))|| Input->KeyboardHeld(SDLK_UP))
        {
            Input->Delay();
        }

        if(Time+10<SDL_GetTicks())
        {
            if (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_Y)) || Input->KeyboardHeld(SDLK_Y))
            {
                if(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_LEFT)) || Input->KeyboardHeld(SDLK_LEFT))
                    WorldParts.ViewPort->Move(-2,0);
                if(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_RIGHT)) || Input->KeyboardHeld(SDLK_RIGHT))
                    WorldParts.ViewPort->Move(2,0);
                if(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_UP)) || Input->KeyboardHeld(SDLK_UP))
                    WorldParts.ViewPort->Move(0,-2);
                if( (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_DOWN)) || Input->KeyboardHeld(SDLK_DOWN)))
                    WorldParts.ViewPort->Move(0,2);
                ResetViewPort = true;
            }
            else
            {
                if (ResetViewPort)
                {
					WorldParts.LimitVPLevel();
                    WorldParts.CenterVPOnPlayer();
                    ResetViewPort = false;
                }
            }
            if (!WorldParts.Player->IsMoving && (((WorldParts.Player1) && !WorldParts.Player1->IsDeath) || ((WorldParts.Player2) && !WorldParts.Player2->IsDeath)) && 
                !(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_Y)) || Input->KeyboardHeld(SDLK_Y)))
            {

                if (Input->Ready() && ((JoystickSetup->GetButtonValue(BUT_A)) || Input->KeyboardHeld(SDLK_A) || Input->KeyboardHeld(SDLK_Q) || Input->KeyboardHeld(SDLK_SPACE)))
				{
					WorldParts.SwitchPlayers();
					Input->Delay();
				}

                //move down
                if (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_DOWN))|| Input->KeyboardHeld(SDLK_DOWN))
                {
                        WorldParts.Player->MoveTo(WorldParts.Player->GetPlayFieldX(),WorldParts.Player->GetPlayFieldY()+1,false);
                }

                //move up
                if (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_UP))|| Input->KeyboardHeld(SDLK_UP))
                {
                        WorldParts.Player->MoveTo(WorldParts.Player->GetPlayFieldX(),WorldParts.Player->GetPlayFieldY()-1,false);
                }
                //move left
                if (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_LEFT)) || Input->KeyboardHeld(SDLK_LEFT))
                {
                        WorldParts.Player->MoveTo(WorldParts.Player->GetPlayFieldX()-1,WorldParts.Player->GetPlayFieldY(),false);
                }
                //move right
                if (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_RIGHT)) || Input->KeyboardHeld(SDLK_RIGHT))
                {
                        WorldParts.Player->MoveTo(WorldParts.Player->GetPlayFieldX()+1,WorldParts.Player->GetPlayFieldY(),false);
                }

            }
            Time = SDL_GetTicks();

        }

        SDL_SetRenderTarget(Renderer, Buffer);
        SDL_RenderTexture(Renderer, IMGBackground,NULL, NULL);
        WorldParts.Draw();
        WorldParts.Move();


        //tekenen naar buffer
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
        SDL_SetRenderTarget(Renderer, Buffer);
        if (((WorldParts.Player1) && WorldParts.Player1->IsDeath) || ((WorldParts.Player2) && WorldParts.Player2->IsDeath))
        {
            ExplosionsFound =false;
            for(teller=0;teller<WorldParts.ItemCount;teller++)
            {
                if(WorldParts.Items[teller]->GetType() == IDExplosion)
                {
                    ExplosionsFound = true;
                }
            }

            if(!ExplosionsFound)
			{
				if (LevelEditorMode)
				{
					if (AskQuestion("Too bad you died !\nDo you want to return to the\nlevel editor ?\n\n(A) Leveleditor (X) Play Again"))
                    {
						GameState = GSLevelEditor;
                    }
					else
					{
						sprintf(FileName,"%s/.blips_temp.lev",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
						WorldParts.Load(FileName);
						for (teller=0;teller<WorldParts.ItemCount;teller++)
						{
							if (WorldParts.Items[teller]->GetType() == IDPlayer)
							{
								Player = (CPlayer*)WorldParts.Items[teller];
								break;
							}

						}
					}
                    if (GlobalSoundEnabled)
				        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
					Input->Reset();
				}
				else
				{
					if (AskQuestion("Too bad you died !\nDo you want to try again?\n\n(A) Try Again (X) Level Selector"))
					{
						sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName,SelectedLevel);
						if(!FileExists(FileName))
                        {
                            char *TmpPath = assetPath("levelpacks");
                            sprintf(FileName,"%s/%s/level%d.lev",TmpPath,LevelPackFileName,SelectedLevel);
                            SDL_free(TmpPath);
                        }
						WorldParts.Load(FileName);
						for (teller=0;teller<WorldParts.ItemCount;teller++)
						{
							if (WorldParts.Items[teller]->GetType() == IDPlayer)
							{
								Player = (CPlayer*) WorldParts.Items[teller];
								break;
							}
						}
						//should never happen
						if(!Player)
						{
							Player = new CPlayer(0,0);
							WorldParts.Add(Player);
						}
					}
					else
                    {
						GameState = GSStageSelect;
                    }
                    if (GlobalSoundEnabled)
				        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
					Input->Reset();

				}
			}
        }
        if ( StageDone())
		{
			SDL_Delay(250);
			if (GlobalSoundEnabled)
				Mix_PlayChannel(-1,Sounds[SND_STAGEEND],0);
			if (LevelEditorMode)
			{
				if (AskQuestion("Congratulations !\nYou Succesfully Solved this level\nDo you want to return to the\nlevel editor ?\n(A) Leveleditor (X) Play Again"))
                {
					GameState = GSLevelEditor;
                }
				else
				{
					sprintf(FileName,"%s/.blips_temp.lev",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
					WorldParts.Load(FileName);
					for (teller=0;teller<WorldParts.ItemCount;teller++)
					{
						if (WorldParts.Items[teller]->GetType() == IDPlayer)
						{
							Player = (CPlayer*)WorldParts.Items[teller];
							break;
						}
					}

				}
                if (GlobalSoundEnabled)
				    Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
				Input->Reset();
			}
			else
			{
				if (SelectedLevel == UnlockedLevels)
				{
					if ( UnlockedLevels < InstalledLevels)
					{
						sprintf(Text,"Congratulations !\nYou Succesfully Solved Level %d/%d\nThe next level has now been unlocked!\n\nPress (A) to continue",SelectedLevel,InstalledLevels);
						PrintForm(Text);
                        if (GlobalSoundEnabled)
                			Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
						UnlockedLevels++;
						SelectedLevel++;
						SaveUnlockData();
						GameState = GSStageSelect;
					}
					else
					{
						sprintf(Text,"Congratulations !\nYou Succesfully Solved Level %d/%d\nlevelpack %s\nis now finished, try out another one!\n\nPress (A) to continue",SelectedLevel,InstalledLevels,LevelPackName);
						PrintForm(Text);
                        if (GlobalSoundEnabled)
                			Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
						GameState = GSTitleScreen;
					}
				}
				else
				{
					sprintf(Text,"Congratulations !\nYou Succesfully Solved Level %d/%d\n\nPress (A) to continue",SelectedLevel,InstalledLevels);
					PrintForm(Text);
                    if (GlobalSoundEnabled)
                			Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
					GameState = GSStageSelect;

				}
			}
			Input->Reset();
		}
    }
}
