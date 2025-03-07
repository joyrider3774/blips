#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
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
		return true;
	}
	else
        if (NumDiamond==0)
        {
            if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_ERROR],0);
            PrintForm("Can not save this level because there's\nno star in the level! Please add a star\nand try again or cancel saving.\n\nPress 'A' to continue");
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
    if (StageReload)
	{
		sprintf(FileName,"%s/.blips_temp.lev",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
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
        frameticks = SDL_GetPerformanceCounter();
        SDL_SetRenderTarget(Renderer, Buffer);
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
                SDL_SetTextureAlphaMod(Buffer,alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetTextureAlphaMod(Buffer,alpha);
            }
        }
		SDL_RenderTexture(Renderer, IMGBackground,NULL,NULL);
		if(ShowGrid)
            SDL_RenderTexture(Renderer, IMGGrid,NULL,NULL);
		WorldParts.Draw();
		Selector.Draw();
		if (ShowPosition)
		{
			sprintf(Tekst,"X: %d - Y: %d",Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
			SDL_SetRenderDrawColor(Renderer, MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.a);
            SDL_FRect Rect;
            Rect.x = 257.0f*UI_WIDTH_SCALE;
            Rect.y = 0.0f;
            Rect.w = (float)ORIG_WINDOW_WIDTH-257.0f-1.0f;
            Rect.h = 13.0f*UI_HEIGHT_SCALE;
            SDL_RenderFillRect(Renderer, &Rect);
            SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
            SDL_RenderRect(Renderer, &Rect);
			WriteText(font,Tekst,strlen(Tekst),259*UI_WIDTH_SCALE,0,0,MenuTextColor,false);
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

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_VOLMIN)) || Input->KeyboardHeld(SDLK_KP_MINUS)))
        {
            DecVolume();
            Input->Delay();
        }

        if(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_B)) || Input->KeyboardHeld(SDLK_ESCAPE))
        {
            if (LevelHasChanged)
            {
                if(AskQuestion("The current level isn't saved yet!\nDo you want to save it now ?\n\nPress (A) to save, (X) to cancel saving"))
                {                    
                    SDL_SetRenderTarget(Renderer, Buffer);
                    SDL_RenderTexture(Renderer, IMGBackground,NULL, NULL);
                    WorldParts.Draw();
                    Selector.Draw();
                    if (ShowPosition)
                    {
                        sprintf(Tekst,"X: %d - Y: %d",Selector.GetPlayFieldX(),Selector.GetPlayFieldY());
                        SDL_SetRenderDrawColor(Renderer, MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.a);
                        SDL_FRect Rect;
                        Rect.x = 257.0f*UI_WIDTH_SCALE;
                        Rect.y = 0.0f;
                        Rect.w = (float)ORIG_WINDOW_WIDTH-257.0f-1.0f;
                        Rect.h = 13.0f*UI_HEIGHT_SCALE;
                        SDL_RenderFillRect(Renderer, &Rect);
                        SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
                        SDL_RenderRect(Renderer, &Rect);
                        WriteText(font,Tekst,strlen(Tekst),259*UI_WIDTH_SCALE,0,0,MenuTextColor,false);

                    }
                    if (!LevelErrorsFound())
                    {

						sprintf(FileName,"%s/.blips_levelpacks",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
#ifdef WIN32
                        mkdir(FileName);
#else
                        mkdir(FileName,S_IRWXO|S_IRWXU|S_IRWXG);
#endif
						sprintf(FileName,"%s/.blips_levelpacks/%s",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName);
#ifdef WIN32
                        mkdir(FileName);
#else
                        mkdir(FileName,S_IRWXO|S_IRWXU|S_IRWXG);
#endif

                        if (SelectedLevel==0)
                            sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"),LevelPackFileName,InstalledLevels+1);
                        else
                            sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"),LevelPackFileName,SelectedLevel);
						WorldParts.Save(FileName);
                        FindLevels();
                        if (SelectedLevel==0)
                            SelectedLevel = InstalledLevels;
                        LevelHasChanged=false;
                        GameState = GSStageSelect;
						if (GlobalSoundEnabled)
                			Mix_PlayChannel(-1,Sounds[SND_BACK],0);
                    }
                }
                else
				{
					if (GlobalSoundEnabled)
                		Mix_PlayChannel(-1,Sounds[SND_BACK],0);
                    GameState = GSStageSelect;
				}
                Input->Reset();
            }
            else
			{
				if (GlobalSoundEnabled)
                		Mix_PlayChannel(-1,Sounds[SND_BACK],0);
                GameState = GSStageSelect;
			}
        }

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_START)) || Input->KeyboardHeld(SDLK_RETURN)))
        {
            if(!LevelErrorsFound())
            {
				sprintf(FileName,"%s/.blips_temp.lev",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
                WorldParts.Save(FileName);
                StageReload = true;
                GameState=GSGame;
				WorldParts.LimitVPLevel();
                WorldParts.CenterVPOnPlayer();
            }
            Input->Reset();
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_L)) || Input->KeyboardHeld(SDLK_PAGEDOWN) || Input->KeyboardHeld(SDLK_L)))
        {
            Selector.DecSelection();
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_R)) || Input->KeyboardHeld(SDLK_PAGEUP) || Input->KeyboardHeld(SDLK_R)))
        {
            Selector.IncSelection();
            Input->Delay();
        }

        if(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) || Input->KeyboardHeld(SDLK_A) || Input->KeyboardHeld(SDLK_Q) || Input->KeyboardHeld(SDLK_SPACE))
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

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_LEFT)) || Input->KeyboardHeld(SDLK_LEFT)))
        {
            Selector.MoveLeft();
            if(Selector.GetPlayFieldX() <  WorldParts.ViewPort->VPMinX+3)
                WorldParts.ViewPort->Move(-TileWidth,0);
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_RIGHT)) || Input->KeyboardHeld(SDLK_RIGHT)))
        {
            Selector.MoveRight();
            if(Selector.GetPlayFieldX() > WorldParts.ViewPort->VPMaxX - 3)
                WorldParts.ViewPort->Move(TileWidth,0);
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_UP)) || Input->KeyboardHeld(SDLK_UP)))
        {
            Selector.MoveUp();
            if(Selector.GetPlayFieldY() < WorldParts.ViewPort->VPMinY+3)
                WorldParts.ViewPort->Move(0,-TileWidth);
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_DOWN)) || Input->KeyboardHeld(SDLK_DOWN)))
        {
            Selector.MoveDown();
            if(Selector.GetPlayFieldY() > WorldParts.ViewPort->VPMaxY-3)
                WorldParts.ViewPort->Move(0,TileWidth);
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_SELECT)) || Input->KeyboardHeld(SDLK_B)))
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

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_Y)) || Input->KeyboardHeld(SDLK_Y) || Input->KeyboardHeld(SDLK_BACKSPACE) || Input->KeyboardHeld(SDLK_DELETE)))
        {
            if (WorldParts.ItemCount > 0)
            if(AskQuestion("You are about to delete all parts\nin this level, are you sure\nyou want to do this?\n\nPress (A) to delete, (X) to cancel"))
            {
                LevelHasChanged = true;
                WorldParts.RemoveAll();
            }
            Input->Reset();
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_X)) || Input->KeyboardHeld(SDLK_X) || Input->KeyboardHeld(SDLK_Z) ))
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
	}
	delete[] FileName;
	delete Input;
}
