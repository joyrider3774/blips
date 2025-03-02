#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "Common.h"
#include "GameFuncs.h"
#include "CInput.h"

void SetupUsbJoystickButtons()
{
    int alpha = 0;
    SDL_Surface *Tmp,*Tmp1;
	int Teller, Selection = 0;
	char *Tekst = new char[300];
	bool done = false;
	CInput *Input = new CInput(10, disableJoysticks);
	while (GameState == GSSetupUsbJoystickButtons)
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

		Input->Update();

		if(Input->SpecialsHeld(SPECIAL_QUIT_EV))
            GameState = GSQuit;

		if(Input->KeyboardHeld(SDLK_ESCAPE) ||  Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_B)))
		{
			if (GlobalSoundEnabled)
            	Mix_PlayChannel(-1,Sounds[SND_BACK],0); 
            GameState = GSTitleScreen;
		}

        if(Input->Ready() && (Input->KeyboardHeld(SDLK_UP) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_UP))))
        {
            Selection--;
            if( Selection < 0)
                Selection = NROFBUTTONS-1;
            if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            Input->Delay();
        }

        if (Input->KeyboardHeld(SDLK_RETURN))
            JoystickSetup->ResetToDefaults();

        if(Input->Ready() && (Input->KeyboardHeld(SDLK_DOWN) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_DOWN))))
        {
            Selection++;
            if( Selection >=NROFBUTTONS)
                Selection = 0;
            if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            Input->Delay();
        }

        SDL_RenderTexture(Renderer, IMGTitleScreen,NULL, NULL);
        SDL_FRect Rect1;
        Rect1.x = 45.0f*UI_WIDTH_SCALE;
        Rect1.y = 70.0f*UI_HEIGHT_SCALE;
        Rect1.w = 250.0f*UI_WIDTH_SCALE;
        Rect1.h = 123.0f*UI_HEIGHT_SCALE;
        SDL_SetRenderDrawColor(Renderer, MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.a);
        SDL_RenderFillRect(Renderer, &Rect1);
        SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
        SDL_RenderRect(Renderer, &Rect1);
        SDL_FRect Rect2;
        Rect2.x = 46.0f*UI_WIDTH_SCALE;
        Rect2.y = 71.0f*UI_HEIGHT_SCALE;
        Rect2.w = 248.0f*UI_WIDTH_SCALE;
        Rect2.h = 121.0f*UI_HEIGHT_SCALE;
        SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
		SDL_RenderRect(Renderer, &Rect2);
        JoystickSetup->DrawCurrentSetup(font,55*UI_WIDTH_SCALE,73*UI_HEIGHT_SCALE,155*UI_WIDTH_SCALE,8*UI_HEIGHT_SCALE,Selection,MenuTextColor,MenuTextColor);
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

        if(Input->Ready() && (Input->KeyboardHeld(SDLK_A) || Input->KeyboardHeld(SDLK_SPACE) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A))))
        {
            if (alpha == 255)
            {
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                Input->Reset();
                while (!done)
                {
                    frameticks = SDL_GetPerformanceCounter();
                    Input->Update();
                    SDL_SetRenderTarget(Renderer, Buffer);
                    SDL_RenderTexture(Renderer, IMGTitleScreen,NULL, NULL);
                    SDL_FRect Rect1;
                    Rect1.x = 45.0f*UI_WIDTH_SCALE;
                    Rect1.y = 70.0f*UI_HEIGHT_SCALE;
                    Rect1.w = 295.0f*UI_WIDTH_SCALE;
                    Rect1.h = 193.0f*UI_HEIGHT_SCALE;
                    SDL_SetRenderDrawColor(Renderer, MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.a);
                    SDL_RenderFillRect(Renderer, &Rect1);
                    SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
                    SDL_RenderRect(Renderer, &Rect1);
                    SDL_FRect Rect2;
                    Rect2.x = 46.0f*UI_WIDTH_SCALE;
                    Rect2.y = 71.0f*UI_HEIGHT_SCALE;
                    Rect2.w = 294.0f*UI_WIDTH_SCALE;
                    Rect2.h = 192.0f*UI_HEIGHT_SCALE;
                    SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
                    SDL_RenderRect(Renderer, &Rect2);
                    JoystickSetup->DrawCurrentSetup(font,55*UI_WIDTH_SCALE,73*UI_HEIGHT_SCALE,155*UI_WIDTH_SCALE,8*UI_HEIGHT_SCALE,Selection,MenuTextColor,MenuBoxBorderColor);
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
                    for (Teller = 0;Teller<MAXJOYSTICKBUTTONS;Teller++)
                        if(Input->JoystickHeld(0,Teller))
                        {
                            done = true;
                            JoystickSetup->SetButtonValue(Selection,Teller);
                            break;
                        }
                    if(Input->SpecialsHeld(SPECIAL_QUIT_EV))
                    {
                        GameState = GSQuit;
                        done = true;
                    }

                    if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) || Input->KeyboardHeld(SDLK_A) || Input->KeyboardHeld(SDLK_SPACE)))
                        done = true;

                    if(Input->KeyboardHeld(SDLK_ESCAPE) || Input->SpecialsHeld(SPECIAL_QUIT_EV) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_B)))
                        done= true;

                }
                Input->Reset();
                Input->Delay();
                done= false;
            }
        }
	}
	delete[] Tekst;
	delete Input;
}

