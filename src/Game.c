#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_framerate.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
#include "Common.h"
#include "GameFuncs.h"
#include "CInput.h"
#include "CWorldPart.h"
#include "CWorldParts.h"
#include "stdbool.h"
bool StageDone()
{
	int Teller,NumDiamonds=0;
	for (Teller=0;Teller<WorldParts->ItemCount;Teller++)
		if (WorldParts->Items[Teller]->Type == IDDiamond)
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
	CWorldPart *Player=NULL;
	bool ResetViewPort=false;
	for (teller=0;teller<WorldParts->ItemCount;teller++)
	{
		if (WorldParts->Items[teller]->Type == IDPlayer)
		{
			Player = WorldParts->Items[teller];
			break;
		}
	}
	//should never happen
	if(!Player)
	{
		Player = CWorldPart_Create(0,0,IDPlayer);
		CWorldParts_Add(WorldParts, Player);
	}
	SDL_Rect Rect;
    Rect.w = Buffer->w;
    Rect.h = Buffer->h;
    Rect.x = StartScreenX;
    Rect.y = StartScreenY;
	CInput *Input = CInput_Create(InputDelay, disableJoysticks);
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
    CWorldParts_LimitVPLevel(WorldParts);
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
        //BUT_SELECT
        if(Input->KeyboardHeld[SDLK_ESCAPE])
        {
            if(!LevelEditorMode)
                GameState = GSStageSelect;
        }
        //BUT_START
        if(CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_RETURN]))
        {
            if(LevelEditorMode)
                GameState=GSLevelEditor;
            else
            {
                sprintf(FileName,"./levelpacks/%s/level%d.lev",LevelPackFileName,SelectedLevel);
                CWorldParts_Load(WorldParts, FileName);
				//need to find player again
				for (teller=0;teller<WorldParts->ItemCount;teller++)
				{
					if (WorldParts->Items[teller]->Type == IDPlayer)
					{
						Player = WorldParts->Items[teller];
						break;
					}
				}
            }
            CInput_Delay(Input);
        }
        //BUT_R
        if (CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_PAGEUP] || Input->KeyboardHeld[SDLK_r]))
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
            CInput_Delay(Input);
        }
        //BUT_LEFT
        if (CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_LEFT]))
        {
            CInput_Delay(Input);
        }
        //BUT_RIGHT
        if (CInput_Ready(Input) && (Input->KeyboardHeld[SDLK_RIGHT]))
        {

            CInput_Delay(Input);
        }
		//BUT_DOWN
        if (CInput_Ready(Input) && Input->KeyboardHeld[SDLK_DOWN])
        {
            CInput_Delay(Input);
        }

        //BUT_UP
        if (CInput_Ready(Input) && Input->KeyboardHeld[SDLK_UP])
        {
            CInput_Delay(Input);
        }

        if(Time+10<SDL_GetTicks())
        {
			//BUT_Y
            if (Input->KeyboardHeld[SDLK_y])
            {
				//BUT_LEFT
                if(Input->KeyboardHeld[SDLK_LEFT])
                    CViewPort_Move(WorldParts->ViewPort, -2,0);
				//BUT_RIGHT
                if(Input->KeyboardHeld[SDLK_RIGHT])
                    CViewPort_Move(WorldParts->ViewPort, -2,0);
				//BUT_UP
                if(Input->KeyboardHeld[SDLK_UP])
                    CViewPort_Move(WorldParts->ViewPort, -2,0);
				//BUT_DOWN
                if( (Input->KeyboardHeld[SDLK_DOWN]))
                    CViewPort_Move(WorldParts->ViewPort, -2,0);
                ResetViewPort = true;
            }
            else
            {
                if (ResetViewPort)
                {
					CWorldParts_LimitVPLevel(WorldParts);
                    CWorldParts_CenterVPOnPlayer(WorldParts);
                    ResetViewPort = false;
                }
            }
            //BUT_Y
            if (!Player->IsMoving && !Player->IsDeath && !(Input->KeyboardHeld[SDLK_y]))
            {
				//BUT_DOWN
                //move down
                if (Input->KeyboardHeld[SDLK_DOWN])
                {
                    CWorldPart_MoveTo(Player, Player->PlayFieldX,Player->PlayFieldY+1,false);
                }
				//BUT_UP
                //move up
                if (Input->KeyboardHeld[SDLK_UP])
                {
                        CWorldPart_MoveTo(Player, Player->PlayFieldX,Player->PlayFieldY-1,false);
                }
				//BUT_LEFT
                //move left
                if (Input->KeyboardHeld[SDLK_LEFT])
                {
                        CWorldPart_MoveTo(Player, Player->PlayFieldX-1,Player->PlayFieldY,false);
                }
				//BUT_RIGHT
                //move right
                if (Input->KeyboardHeld[SDLK_RIGHT])
                {
                        CWorldPart_MoveTo(Player, Player->PlayFieldX+1,Player->PlayFieldY,false);
                }

            }
            //printf("game():20\n");
            Time = SDL_GetTicks();

        }


		if(!Tmp)
			printf("BLEEEEEH\n");
        SDL_BlitSurface(IMGBackground,NULL,Tmp,NULL);
        CWorldParts_Draw(WorldParts,Tmp);
        CWorldParts_Move(WorldParts);


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
            for(teller=0;teller<WorldParts->ItemCount;teller++)
            {
                if(WorldParts->Items[teller]->Type == IDExplosion)
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
						CWorldParts_Load(WorldParts, FileName);
						for (teller=0;teller<WorldParts->ItemCount;teller++)
						{
							if (WorldParts->Items[teller]->Type == IDPlayer)
							{
								Player = WorldParts->Items[teller];
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
						CWorldParts_Load(WorldParts, FileName);
						for (teller=0;teller<WorldParts->ItemCount;teller++)
						{
							if (WorldParts->Items[teller]->Type == IDPlayer)
							{
								Player = WorldParts->Items[teller];
								break;
							}
						}
								//should never happen
						if(!Player)
						{
							Player = CWorldPart_Create(0,0,IDPlayer);
							CWorldParts_Add(WorldParts, Player);
						}
						CInput_Reset(Input);
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
					CWorldParts_Load(WorldParts, FileName);
					for (teller=0;teller<WorldParts->ItemCount;teller++)
					{
						if (WorldParts->Items[teller]->Type == IDPlayer)
						{
							Player = WorldParts->Items[teller];
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
    CInput_Destroy(Input);
}
