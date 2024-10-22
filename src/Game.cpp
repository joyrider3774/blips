#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_framerate.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
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
	SDL_Surface *Tmp;
	bool ExplosionsFound =false;
	char FileName[FILENAME_MAX];
	char Text[500];
	int alpha=0,teller;
	Uint32 Time=0;
	CPlayer *Player=0;
	bool ResetViewPort=false;
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
	SDL_Rect Rect;
    Rect.w = Buffer->w;
    Rect.h = Buffer->h;
    Rect.x = StartScreenX;
    Rect.y = StartScreenY;
	CInput *Input = new CInput(InputDelay, disableJoysticks);
	Tmp = SDL_DisplayFormat(Buffer);
	Time = SDL_GetTicks();
	if (MusicCount > 1)
 	{

		if(GlobalSoundEnabled)
		{
		    SelectedMusic =	1+rand()%(MusicCount-1);
			Mix_PlayMusic(Music[SelectedMusic],0);
			SetVolume(Volume);
		}
	}
    WorldParts.LimitVPLevel();
    while (GameState == GSGame)
    {
        if(!Mix_PlayingMusic())
        if(GlobalSoundEnabled)
        {
            if (MusicCount > 1)
            {
                SelectedMusic =	1+rand()%(MusicCount-1);
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

        if(Input->Ready() && Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_B)] || Input->KeyboardHeld[SDLK_ESCAPE])
        {
            if(!LevelEditorMode)
			{
				if (GlobalSoundEnabled)
                	Mix_PlayChannel(-1,Sounds[SND_BACK],0);
                GameState = GSStageSelect;
			}
        }

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_START)] || Input->KeyboardHeld[SDLK_RETURN]))
        {
            if(LevelEditorMode)
			{
				if (GlobalSoundEnabled)
                	Mix_PlayChannel(-1,Sounds[SND_BACK],0);
                GameState=GSLevelEditor;
			}
            else
            {
                sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
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
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_R)] || Input->KeyboardHeld[SDLK_PAGEUP] || Input->KeyboardHeld[SDLK_r]))
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

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[SDLK_LEFT]))
        {
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_RIGHT)] || Input->KeyboardHeld[SDLK_RIGHT]))
        {

            Input->Delay();
        }

        if (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_DOWN)]|| Input->KeyboardHeld[SDLK_DOWN])
        {
            Input->Delay();
        }

        //move up
        if (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_UP)]|| Input->KeyboardHeld[SDLK_UP])
        {
            Input->Delay();
        }

        if(Time+10<SDL_GetTicks())
        {
            if (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_Y)] || Input->KeyboardHeld[SDLK_y])
            {
                if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[SDLK_LEFT])
                    WorldParts.ViewPort->Move(-2,0);
                if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_RIGHT)] || Input->KeyboardHeld[SDLK_RIGHT])
                    WorldParts.ViewPort->Move(2,0);
                if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_UP)] || Input->KeyboardHeld[SDLK_UP])
                    WorldParts.ViewPort->Move(0,-2);
                if( (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_DOWN)] || Input->KeyboardHeld[SDLK_DOWN]))
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
            if (!Player->IsMoving && !Player->IsDeath && !(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_Y)] || Input->KeyboardHeld[SDLK_y]))
            {

                //move down
                if (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_DOWN)]|| Input->KeyboardHeld[SDLK_DOWN])
                {
                        Player->MoveTo(Player->GetPlayFieldX(),Player->GetPlayFieldY()+1,false);
                }

                //move up
                if (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_UP)]|| Input->KeyboardHeld[SDLK_UP])
                {
                        Player->MoveTo(Player->GetPlayFieldX(),Player->GetPlayFieldY()-1,false);
                }
                //move left
                if (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[SDLK_LEFT])
                {
                        Player->MoveTo(Player->GetPlayFieldX()-1,Player->GetPlayFieldY(),false);
                }
                //move right
                if (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_RIGHT)] || Input->KeyboardHeld[SDLK_RIGHT])
                {
                        Player->MoveTo(Player->GetPlayFieldX()+1,Player->GetPlayFieldY(),false);
                }

            }
            Time = SDL_GetTicks();

        }


        SDL_BlitSurface(IMGBackground,NULL,Tmp,NULL);
        WorldParts.Draw(Tmp);
        WorldParts.Move();


        //tekenen naar buffer
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
        if ((WINDOW_WIDTH != ORIG_WINDOW_WIDTH) || (WINDOW_HEIGHT != ORIG_WINDOW_HEIGHT))
		{
			SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer1,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
			SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
			SDL_FreeSurface(ScreenBufferZoom);
		}
		else
		{
			SDL_BlitSurface(Buffer1, NULL, Screen, NULL);
		}

        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);
        if (Player->IsDeath)
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
						GameState = GSLevelEditor;
					else
					{
						
						sprintf(FileName,"%s/.blips_temp.lev",getenv("HOME") == NULL ? ".": getenv("HOME"));
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
					Input->Reset();
				}
				else
				{
					if (AskQuestion("Too bad you died !\nDo you want to try again?\n\n(A) Try Again (X) Level Selector"))
					{
						sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev",getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName,SelectedLevel);
						if(!FileExists(FileName))
							sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
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
						GameState = GSStageSelect;
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
					GameState = GSLevelEditor;
				else
				{
					sprintf(FileName,"%s/.blips_temp.lev",getenv("HOME") == NULL ? ".": getenv("HOME"));
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
						UnlockedLevels++;
						SelectedLevel++;
						SaveUnlockData();
						GameState = GSStageSelect;
					}
					else
					{
						sprintf(Text,"Congratulations !\nYou Succesfully Solved Level %d/%d\nlevelpack %s\nis now finished, try out another one!\n\nPress (A) to continue",SelectedLevel,InstalledLevels,LevelPackName);
						PrintForm(Text);
						GameState = GSTitleScreen;
					}
				}
				else
				{
					sprintf(Text,"Congratulations !\nYou Succesfully Solved Level %d/%d\n\nPress (A) to continue",SelectedLevel,InstalledLevels);
					PrintForm(Text);
					GameState = GSStageSelect;

				}
			}
			Input->Reset();
		}
    }
    SDL_FreeSurface(Tmp);
    delete Input;
}
