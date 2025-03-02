#include <SDL3/SDL.h>
#include <stdio.h>
#include "CInput.h"
#include "Common.h"
#include "GameFuncs.h"

void Credits()
{
    int alpha = 0;
	CInput *Input = new CInput(InputDelay, disableJoysticks);
	char *LevelPackCreator = new char[21];
	char FileName[FILENAME_MAX];
	FILE *Fp;
    char *Tekst = new char[500];
	sprintf(FileName,"%s/.blips_levelpacks/%s/credits.dat", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName);
	if(!FileExists(FileName))
    {
        char *TmpPath = assetPath("levelpacks");
		sprintf(FileName,"%s/%s/credits.dat",TmpPath,LevelPackFileName);
        SDL_free(TmpPath);
    }
	Fp = fopen(FileName,"rt");
	if (Fp)
	{
		fscanf(Fp,"[Credits]\nCreator='%[^']'\n",LevelPackCreator);
		fclose(Fp);
		sprintf(Tekst,"Blips was created by\nWillems Davy - Willems Soft 2008-2024.\nHttps://joyrider3774.itch.io\n\nLevelpack %s was created\nby %s.",LevelPackName,LevelPackCreator);
	}
	else
		sprintf(Tekst,"Blips was created by\nWillems Davy - Willems Soft 2008-2024.\nHttps://joyrider3774.itch.io\n\nLevelpack %s was created\nby unknown person.",LevelPackName);
	while (GameState == GSCredits)
	{
        frameticks = SDL_GetPerformanceCounter();
        SDL_SetRenderTarget(Renderer, Buffer);
        if(GlobalSoundEnabled)
            if(!Mix_PlayingMusic())
            {
                SelectedMusic = 0;
                Mix_PlayMusic(Music[SelectedMusic],0);
                SetVolume(Volume);
            }
		SDL_RenderTexture(Renderer, IMGTitleScreen,NULL,NULL);

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

		if(Input->KeyboardHeld(SDLK_SPACE) || Input->KeyboardHeld(SDLK_RETURN) || Input->KeyboardHeld(SDLK_ESCAPE) || Input->KeyboardHeld(SDLK_A) || Input->KeyboardHeld(SDLK_Q) ||
           Input->KeyboardHeld(SDLK_X) || Input->KeyboardHeld(SDLK_Z) || Input->KeyboardHeld(SDLK_Y) || Input->KeyboardHeld(SDLK_B) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) ||
           Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_X)) ||  Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_Y))  || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_B)) )
        {
			if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_BACK],0);
            GameState=GSTitleScreen;
        }

        SDL_SetRenderDrawColor(Renderer, MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.a);
        SDL_FRect Rect1;
        Rect1.x = 60.0f*UI_WIDTH_SCALE;
        Rect1.y = 80.0f*UI_HEIGHT_SCALE;
        Rect1.w = 200.0f*UI_WIDTH_SCALE;
        Rect1.h = 80.0f*UI_HEIGHT_SCALE;
		SDL_RenderFillRect(Renderer, &Rect1);
        SDL_SetRenderDrawColor(Renderer, MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.a);
		SDL_RenderRect(Renderer, &Rect1);
        SDL_FRect Rect2;
        Rect2.x = 61.0f*UI_WIDTH_SCALE;
        Rect2.y = 81.0f*UI_HEIGHT_SCALE;
        Rect2.w = 198.0f*UI_WIDTH_SCALE;
        Rect2.h = 78.0f*UI_HEIGHT_SCALE;
        SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
        SDL_RenderRect(Renderer, &Rect2);
		WriteText(font,Tekst,strlen(Tekst),65*UI_WIDTH_SCALE,82*UI_HEIGHT_SCALE,1,MenuTextColor,false);
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
	delete[] Tekst;
	delete[] LevelPackCreator;
	delete Input;
}