#include <SDL3/SDL.h>
#include "CInput.h"
#include "Common.h"
#include "GameFuncs.h"

void TitleScreen()
{
	int alpha = 0;
    SDL_Surface *Tmp;
	int Teller, Selection = 1;
	CInput *Input = new CInput(InputDelay, disableJoysticks);
	char *Tekst = new char[300];
	while (GameState == GSTitleScreen)
	{
        frameticks = SDL_GetPerformanceCounter();
        SDL_SetRenderTarget(Renderer, Buffer);
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
		SDL_RenderTexture(Renderer, IMGTitleScreen,NULL, NULL);
		Input->Update();

		if(Input->KeyboardHeld(SDLK_ESCAPE) || Input->SpecialsHeld(SPECIAL_QUIT_EV))
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

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_LEFT)) || Input->KeyboardHeld(SDLK_LEFT)))
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
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_RIGHT)) || Input->KeyboardHeld(SDLK_RIGHT)))
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
            Input->Delay();
        }

        if((Input->Ready()) && (Input->KeyboardHeld(SDLK_DOWN) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_DOWN))) &&
           !(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_R)) || Input->KeyboardHeld(SDLK_R)))
        {
            if (Selection < 6)
            {
                Selection++;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            }

            Input->Delay();
        }

        if((Input->Ready()) && (Input->KeyboardHeld(SDLK_UP) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_UP))) &&
           !(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_R)) || Input->KeyboardHeld(SDLK_R)))
        {
            if (Selection > 1)
            {
                Selection--;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            }
            Input->Delay();
        }

        if(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) || Input->KeyboardHeld(SDLK_SPACE) || Input->KeyboardHeld(SDLK_RETURN))
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
                            SDL_RenderTexture(Renderer, IMGTitleScreen,NULL,NULL);
                            sprintf(Tekst,"There are no levels found in levelpack\n%s\n\nPlease create a level for this level pack\nfirst!",LevelPackName);
                            PrintForm(Tekst);
                            Input->Reset();
                        }
                    }
                    break;
                case 2:
                    GameState=GSLevelEditorMenu;
                    LevelEditorMode=true;
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    break;
                case 4:
                    GameState=GSCredits;
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    break;
                case 5:
                    GameState = GSSetupUsbJoystickButtons;
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
        SDL_FRect Rect;
        Rect.x = 60.0f*UI_WIDTH_SCALE;
        Rect.y = 70.0f*UI_HEIGHT_SCALE;
        Rect.w = 200.0f*UI_WIDTH_SCALE;
        Rect.h = 95.0f*UI_HEIGHT_SCALE;
        SDL_SetRenderDrawColor(Renderer, MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.a);
   		SDL_RenderFillRect(Renderer, &Rect);
        SDL_SetRenderDrawColor(Renderer,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
   		SDL_RenderRect(Renderer, &Rect);      
        SDL_FRect Rect2;
        Rect2.x = 61.0f*UI_WIDTH_SCALE;
        Rect2.y = 71.0f*UI_HEIGHT_SCALE;
        Rect2.w = 198.0f*UI_WIDTH_SCALE;
        Rect2.h = 93.0f*UI_HEIGHT_SCALE;
        SDL_RenderRect(Renderer, &Rect2);      
		
		sprintf(Tekst,"Play Selected LevelPack\nLevel Editor\n<%s>\nCredits\nSetup External Joystick\nQuit",LevelPackName);
		WriteText(BigFont,Tekst,strlen(Tekst),90*UI_WIDTH_SCALE,75*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		if (Selection > 1)
		{
			strcpy(Tekst,"\n");
			for(Teller=2;Teller<Selection;Teller++)
				strcat(Tekst, "\n");
			strcat(Tekst, ">>");
		}
		else
			strcpy(Tekst,">>");
		WriteText(BigFont,Tekst,strlen(Tekst),65*UI_WIDTH_SCALE,75*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		
		if (alpha < 255)
        {
            if(alpha+AlphaInc > MaxAlpha)
            {
                alpha = 255;
                SDL_SetTextureAlphaMod(Buffer, alpha);
            }
            else
            {
                alpha+=AlphaInc;
                SDL_SetTextureAlphaMod(Buffer, alpha);
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
	delete[] Tekst;
	delete Input;
}
