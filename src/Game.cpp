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
	int Teller,NumDiamonds=0;
	for (Teller=0;Teller<WorldParts.ItemCount;Teller++)
		if (WorldParts.Items[Teller]->GetType() == IDDiamond)
		{
			NumDiamonds++;
		}
    return (NumDiamonds ==0);
}

void Game()
{
	SDL_Surface *Tmp;
	bool ExplosionsFound =false;
	char FileName[FILENAME_MAX];
	char Text[500];
	int alpha=0,teller;
	Uint32 Time=0,Time2=0;
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
	int framecounter=0,fps=0;
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
                //printf("game():2\n");
                SelectedMusic =	1+rand()%(MusicCount-1);
                Mix_PlayMusic(Music[SelectedMusic],0);
                SetVolume(Volume);
                //printf("game():3\n");
            }
            else
                if(MusicCount ==1)
                {
                    //printf("game():4\n");
                    SelectedMusic =	0;
                    Mix_PlayMusic(Music[SelectedMusic],0);
                    SetVolume(Volume);
                    //printf("game():5\n");
                }

        }

        Input->Update();
        //printf("game():6\n");
        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;
        //printf("game():7\n");
        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_VOLUP)] || Input->KeyboardHeld[SDLK_KP_PLUS]))
        {
            IncVolume();
            Input->Delay();
        }
        //printf("game():8\n");
        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_VOLMIN)] || Input->KeyboardHeld[SDLK_KP_MINUS]))
        {
            DecVolume();
            Input->Delay();
        }
        //printf("game():9\n");
        if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_SELECT)] || Input->KeyboardHeld[SDLK_ESCAPE])
        {
            if(!LevelEditorMode)
                GameState = GSStageSelect;
        }
        //printf("game():10\n");
        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_START)] || Input->KeyboardHeld[SDLK_RETURN]))
        {
            if(LevelEditorMode)
                GameState=GSLevelEditor;
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
        //printf("game():12\n");
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
                    //Mix_HookMusicFinished(MusicFinished);
                    SetVolume(Volume);
                }
            }
            Input->Delay();
        }
        //printf("game():13\n");
        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[SDLK_LEFT]))
        {
            Input->Delay();
        }
        //printf("game():14\n");
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

        //printf("game():15\n");
        if(Time+10<SDL_GetTicks())
        {
            //printf("game():16\n");
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
            //printf("game():17\n");
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
            //printf("game():20\n");
            Time = SDL_GetTicks();

        }


		if(!Tmp)
			printf("BLEEEEEH\n");
        SDL_BlitSurface(IMGBackground,NULL,Tmp,NULL);
        WorldParts.Draw(Tmp);
        WorldParts.Move();


        sprintf(Text,"Fps:%d",fps);
       // WriteText(Tmp,BigFont,Text,strlen(Text),0,0,0,Color,false);
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
        SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer1,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT, 0);
        SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
        SDL_FreeSurface(ScreenBufferZoom);

        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);
        framecounter++;
        if(Time2 + 1000 < SDL_GetTicks())
        {
            fps = framecounter;
            framecounter = 0;
            Time2 = SDL_GetTicks();
        }
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
						sprintf(FileName,"./temp.lev");
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
				}
				else
				{
					if (AskQuestion("Too bad you died !\nDo you want to try again?\n\n(A) Try Again (X) Level Selector"))
					{
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
						Input->Reset();
					}
					else
						GameState = GSStageSelect;

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
					sprintf(FileName,"./temp.lev");
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
		}
    }
    Mix_HaltMusic();
    SDL_FreeSurface(Tmp);
    delete Input;
}