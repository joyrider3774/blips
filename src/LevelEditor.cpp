#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_framerate.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Common.h"
#include "GameFuncs.h"
#include "CInput.h"
#include "CWorldPart.h"
#include "CSelector.h"
#include "CPlayer.h"

bool LevelErrorsFound()
{
	int Teller,NumPlayer=0,NumDiamond=0;
	for (Teller=0;Teller<WorldParts.ItemCount;Teller++)
	{
		if(WorldParts.Items[Teller]->GetType() == IDPlayer)
			NumPlayer++;
        if(WorldParts.Items[Teller]->GetType() == IDDiamond)
			NumDiamond++;
	}

	if (NumPlayer==0)
	{
		if (GlobalSoundEnabled)
			Mix_PlayChannel(-1,Sounds[SND_ERROR],0);
		PrintForm("Can not save this level because there's\nno player in the level! Please add a Player\nand try again or cancel saving.\n\nPress 'A' to continue");
		if (GlobalSoundEnabled)
            Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
        return true;
	}
	else
        if (NumDiamond==0)
        {
            if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_ERROR],0);
            PrintForm("Can not save this level because there's\nno star in the level! Please add a star\nand try again or cancel saving.\n\nPress 'A' to continue");
            if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
            return true;
        }

	return false;

}


void LevelEditor()
{
	int Teller,Teller2,alpha=0;
	SDL_Surface *Tmp,*Tmp1;
	bool ShowPosition=true,AnotherPartFound,SamePartFound,PlayerFound=false,ShowGrid=true;
	char Tekst[200];
	char *FileName = new char[FILENAME_MAX];
	CInput *Input = new CInput(InputDelay-5, disableJoysticks);
	CSelector Selector(&WorldParts);
	int MaxX=0,MaxY=0,MinY=NrOfRows,MinX=NrOfCols,Xi=0,Yi=0;
    Tmp1 = SDL_CreateRGBSurface(SDL_SWSURFACE,ORIG_WINDOW_WIDTH,ORIG_WINDOW_HEIGHT,SCREEN_BPP,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
	Tmp = SDL_DisplayFormat(Tmp1);
	SDL_FreeSurface(Tmp1);
	if (StageReload)
	{
		sprintf(FileName,"%s/.blips_temp.lev",getenv("HOME") == NULL ? ".": getenv("HOME"));
		WorldParts.Load(FileName);
        WorldParts.LimitVPLevel();
		remove(FileName);
		StageReload=false;
	}
    for (Teller = 0;Teller< WorldParts.ItemCount;Teller++)
        if(WorldParts.Items[Teller]->GetType() == IDPlayer)
        {
            PlayerFound = true;
            Selector.SetPosition(WorldParts.Items[Teller]->GetPlayFieldX(),WorldParts.Items[Teller]->GetPlayFieldY());
            break;
        }
    WorldParts.ViewPort->SetVPLimit(0,0,NrOfCols-1,NrOfRows-1);
    if (!PlayerFound)
    {
        WorldParts.ViewPort->SetViewPort((NrOfCols/2)-10,(NrOfRows/2)-6,(NrOfCols/2)+10,(NrOfRows/2)+6);
        Selector.SetPosition((NrOfCols/2),(NrOfRows/2));
    }

	while (GameState == GSLevelEditor)
	{
        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                SelectedMusic = 0;
                Mix_PlayMusic(Music[SelectedMusic],0);
                SetVolume(Volume);
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
		SDL_BlitSurface(IMGBackground,NULL,Tmp,NULL);
		if(ShowGrid)
            SDL_BlitSurface(IMGGrid,NULL,Tmp,NULL);
		WorldParts.Draw(Tmp);
		Selector.Draw(Tmp);
		if (ShowPosition)
		{
			sprintf(Tekst,"X: %d - Y: %d",Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
			boxRGBA(Tmp,257*UI_WIDTH_SCALE,0,ORIG_WINDOW_WIDTH-1,13*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
			rectangleRGBA(Tmp,257*UI_WIDTH_SCALE,-1,ORIG_WINDOW_WIDTH-1,13*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
			WriteText(Tmp,font,Tekst,strlen(Tekst),259*UI_WIDTH_SCALE,0,0,MenuTextColor,false);
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
        SDL_Flip(Screen);

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

        if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_B)] || Input->KeyboardHeld[KEY_B])
        {
            if (LevelHasChanged)
            {
                if (GlobalSoundEnabled)
					Mix_PlayChannel(-1,Sounds[SND_BACK],0);
                if(AskQuestion("The current level isn't saved yet!\nDo you want to save it now ?\n\nPress (A) to save, (X) to cancel saving"))
                {
                    SDL_BlitSurface(IMGBackground,NULL,Buffer,NULL);
                    WorldParts.Draw(Buffer);
                    Selector.Draw(Buffer);
                    if (ShowPosition)
                    {
                        sprintf(Tekst,"X: %d - Y: %d",Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                        boxRGBA(Buffer,257*UI_WIDTH_SCALE,0,ORIG_WINDOW_WIDTH-1,13*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
                        rectangleRGBA(Buffer,257*UI_WIDTH_SCALE,-1,ORIG_WINDOW_HEIGHT,13*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
                        WriteText(Buffer,font,Tekst,strlen(Tekst),259*UI_WIDTH_SCALE,0,0,MenuTextColor,false);
                    }
                    if (!LevelErrorsFound())
                    {

						sprintf(FileName,"%s/.blips_levelpacks",getenv("HOME") == NULL ? ".": getenv("HOME"));
#ifdef WIN32
                        mkdir(FileName);
#else
                        mkdir(FileName,S_IRWXO|S_IRWXU|S_IRWXG);
#endif
						sprintf(FileName,"%s/.blips_levelpacks/%s",getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName);
#ifdef WIN32
                        mkdir(FileName);
#else
                        mkdir(FileName,S_IRWXO|S_IRWXU|S_IRWXG);
#endif

                        if (SelectedLevel==0)
                            sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev",getenv("HOME") == NULL ? ".": getenv("HOME"),LevelPackFileName,InstalledLevels+1);
                        else
                            sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev",getenv("HOME") == NULL ? ".": getenv("HOME"),LevelPackFileName,SelectedLevel);
						WorldParts.Save(FileName);
                        FindLevels();
                        if (SelectedLevel==0)
                            SelectedLevel = InstalledLevels;
                        LevelHasChanged=false;
                        GameState = GSStageSelect;
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
            else
			{
				if (GlobalSoundEnabled)
                	Mix_PlayChannel(-1,Sounds[SND_BACK],0);
                GameState = GSStageSelect;
			}
        }

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_START)] || Input->KeyboardHeld[KEY_START]))
        {
            if(!LevelErrorsFound())
            {
                if (GlobalSoundEnabled)
					Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
				sprintf(FileName,"%s/.blips_temp.lev",getenv("HOME") == NULL ? ".": getenv("HOME"));
                WorldParts.Save(FileName);
                StageReload = true;
                GameState=GSGame;
				WorldParts.LimitVPLevel();
                WorldParts.CenterVPOnPlayer();
            }
            Input->Reset();
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_L)] || Input->KeyboardHeld[KEY_L] || Input->KeyboardHeld[SDLK_l]))
        {
            Selector.DecSelection();
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_R)] || Input->KeyboardHeld[KEY_R] || Input->KeyboardHeld[SDLK_r]))
        {
            Selector.IncSelection();
            Input->Delay();
        }

        if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] || Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_q] || Input->KeyboardHeld[KEY_A])
        {
            SamePartFound = false;
            for(Teller=0;Teller<WorldParts.ItemCount;Teller++)
                if((WorldParts.Items[Teller]->GetPlayFieldX() == Selector.GetPlayFieldX()) &&
                   (WorldParts.Items[Teller]->GetPlayFieldY() == Selector.GetPlayFieldY()))
                {
                    if(WorldParts.Items[Teller]->GetType() == Selector.GetSelection())
                    {
                        SamePartFound=true;
                    }
                    if(Selector.GetSelection() == IDEmpty)
                    {
                        LevelHasChanged=true;
                        break;
                    }
                }
            if(Selector.GetSelection() != IDEmpty)
                if(!LevelHasChanged)
                    LevelHasChanged=!SamePartFound;
            switch(Selector.GetSelection())
            {
                case IDEmpty:
                    for(Teller=0;Teller<WorldParts.ItemCount;Teller++)
                    {
                        if((WorldParts.Items[Teller]->GetPlayFieldX() == Selector.GetPlayFieldX()) &&
                           (WorldParts.Items[Teller]->GetPlayFieldY() == Selector.GetPlayFieldY()))
						   {
								if(WorldParts.Items[Teller]->GetType() == IDFloor)
								{
									AnotherPartFound = false;
									for(Teller2=Teller+1;Teller2<WorldParts.ItemCount;Teller2++)
										if((WorldParts.Items[Teller2]->GetPlayFieldX() == Selector.GetPlayFieldX()) &&
										(WorldParts.Items[Teller2]->GetPlayFieldY() == Selector.GetPlayFieldY()))
											{
													WorldParts.Remove(WorldParts.Items[Teller2]->GetPlayFieldX(),WorldParts.Items[Teller2]->GetPlayFieldY(),WorldParts.Items[Teller2]->GetType());
													AnotherPartFound = true;
													break;
											}


									if (!AnotherPartFound)
									{
										WorldParts.Remove(WorldParts.Items[Teller]->GetPlayFieldX(),WorldParts.Items[Teller]->GetPlayFieldY(),IDFloor);
										break;
									}
								}
								else
								{
									WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
									break;
								
								}
						   }
                    }
                    break;
                case IDBox:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDWall);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDBox);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDPlayer);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDBomb);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDDiamond);
                    WorldParts.Add(new CBox(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDPlayer:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDWall);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDBox);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDBomb);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDDiamond);
                    for(Teller=0;Teller<WorldParts.ItemCount;Teller++)
                    {
                        if (WorldParts.Items[Teller]->GetType() == IDPlayer)
                            WorldParts.Remove(WorldParts.Items[Teller]->GetPlayFieldX(),WorldParts.Items[Teller]->GetPlayFieldY(),IDPlayer);
                    }
                    WorldParts.Add(new CPlayer(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDWall:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                    WorldParts.Add(new CWall(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDFloor:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDFloor);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDWall);
                    WorldParts.Add(new CFloor(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDBomb:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDWall);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDBox);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDPlayer);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDBomb);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDDiamond);
                    WorldParts.Add(new CBomb(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;
                case IDDiamond:
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDWall);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDBox);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDPlayer);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDBomb);
                    WorldParts.Remove(Selector.GetPlayFieldX(),Selector.GetPlayFieldY(),IDDiamond);
                    WorldParts.Add(new CDiamond(Selector.GetPlayFieldX(),Selector.GetPlayFieldY()));
                    break;


            }
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[KEY_LEFT]))
        {
            Selector.MoveLeft();
            if(Selector.GetPlayFieldX() <  WorldParts.ViewPort->VPMinX+3)
                WorldParts.ViewPort->Move(-TileWidth,0);
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_RIGHT)] || Input->KeyboardHeld[KEY_RIGHT]))
        {
            Selector.MoveRight();
            if(Selector.GetPlayFieldX() > WorldParts.ViewPort->VPMaxX - 3)
                WorldParts.ViewPort->Move(TileWidth,0);
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_UP)] || Input->KeyboardHeld[KEY_UP]))
        {
            Selector.MoveUp();
            if(Selector.GetPlayFieldY() < WorldParts.ViewPort->VPMinY+3)
                WorldParts.ViewPort->Move(0,-TileWidth);
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_DOWN)] || Input->KeyboardHeld[KEY_DOWN]))
        {
            Selector.MoveDown();
            if(Selector.GetPlayFieldY() > WorldParts.ViewPort->VPMaxY-3)
                WorldParts.ViewPort->Move(0,TileWidth);
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_SELECT)] || Input->KeyboardHeld[KEY_SELECT]))
        {
            if(ShowPosition && ShowGrid)
                ShowGrid = !ShowGrid;
            else
            if(ShowPosition && !ShowGrid)
                ShowPosition = !ShowPosition;
            else
            if(!ShowPosition && ! ShowGrid)
               ShowGrid = !ShowGrid;
            else
            if(!ShowPosition && ShowGrid)
                ShowPosition = !ShowPosition;
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_Y)] || Input->KeyboardHeld[KEY_Y] || Input->KeyboardHeld[SDLK_BACKSPACE] || Input->KeyboardHeld[SDLK_DELETE]))
        {
            if (WorldParts.ItemCount > 0)
            {
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                if(AskQuestion("You are about to delete all parts\nin this level, are you sure\nyou want to do this?\n\nPress (A) to delete, (X) to cancel"))
                {
                    LevelHasChanged = true;
                    WorldParts.RemoveAll();
                }
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
            }
            Input->Reset();
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_X)] || Input->KeyboardHeld[KEY_X] || Input->KeyboardHeld[SDLK_z] ))
        {
            MinX = NrOfCols-1;
            MinY = NrOfRows-1;
            MaxX = 0;
            MaxY = 0;
            for (Teller = 0;Teller<WorldParts.ItemCount;Teller++)
            {
                if (WorldParts.Items[Teller]->GetPlayFieldX() < MinX)
                    MinX = WorldParts.Items[Teller]->GetPlayFieldX();
                if (WorldParts.Items[Teller]->GetPlayFieldY() < MinY)
                    MinY = WorldParts.Items[Teller]->GetPlayFieldY();
                if (WorldParts.Items[Teller]->GetPlayFieldX() > MaxX)
                    MaxX = WorldParts.Items[Teller]->GetPlayFieldX();
                if (WorldParts.Items[Teller]->GetPlayFieldY() > MaxY)
                    MaxY = WorldParts.Items[Teller]->GetPlayFieldY();
            }
            Xi = ((NrOfCols-1) / 2) - (MaxX + MinX) / 2;
            Yi = ((NrOfRows-1) / 2) - (MaxY + MinY) / 2;
            for (Teller = 0;Teller<WorldParts.ItemCount;Teller++)
            {
                WorldParts.Items[Teller]->SetPosition(WorldParts.Items[Teller]->GetPlayFieldX() + Xi,WorldParts.Items[Teller]->GetPlayFieldY() + Yi);
            }
            WorldParts.LimitVPLevel();
            //center viewport;
            WorldParts.ViewPort->SetViewPort((NrOfCols/2)-10,(NrOfRows/2)-6,(NrOfCols/2)+10,(NrOfRows/2)+6);
            Selector.SetPosition((NrOfCols/2),(NrOfRows/2));
            //if player found center viewport on player !
            for (Teller = 0;Teller<WorldParts.ItemCount;Teller++)
            {
                if(WorldParts.Items[Teller]->GetType() == IDPlayer)
                {
                    Selector.SetPosition(WorldParts.Items[Teller]->GetPlayFieldX(),WorldParts.Items[Teller]->GetPlayFieldY());
                    WorldParts.CenterVPOnPlayer();
                    break;
                }
            }
            WorldParts.ViewPort->SetVPLimit(0,0,NrOfCols-1,NrOfRows-1);
            if (Xi != 0 || Yi!=0)
                LevelHasChanged = true;
            Input->Delay();
        }
        SDL_framerateDelay(&Fpsman);
	}
	delete[] FileName;
	SDL_FreeSurface(Tmp);
	delete Input;
}
