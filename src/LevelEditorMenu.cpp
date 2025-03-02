#include <SDL3/SDL.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Common.h"
#include "CInput.h"
#include "GameFuncs.h"


void LevelEditorMenu()
{
	int alpha = 0;
    SDL_Surface *Tmp;
	FILE *Fp;
	int Teller, Selection = 1;
	char *PackName,*CreatorName;
	char FileName[FILENAME_MAX];
	CInput *Input = new CInput(InputDelay, disableJoysticks);
	char *Tekst = new char[300];
	while (GameState == GSLevelEditorMenu)
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
		SDL_RenderTexture(Renderer, IMGTitleScreen,NULL,NULL);
		Input->Update();

		if(Input->KeyboardHeld(SDLK_ESCAPE) || Input->SpecialsHeld(SPECIAL_QUIT_EV) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_B)))
		{
			if (GlobalSoundEnabled)
            	Mix_PlayChannel(-1,Sounds[SND_BACK],0);
            GameState = GSTitleScreen;
		}

		if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_L)) || Input->KeyboardHeld(SDLK_PAGEDOWN) || Input->KeyboardHeld(SDLK_L)))
        {
            Input->Delay();
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

        if (Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_LEFT)) || Input->KeyboardHeld(SDLK_LEFT)))
        {
            if(Selection==4)
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
            if (Selection==4)
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
            if (Selection < 5)
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

        if(Input->SpecialsHeld(SPECIAL_QUIT_EV))
            GameState = GSQuit;

        if(Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) || Input->KeyboardHeld(SDLK_SPACE) || Input->KeyboardHeld(SDLK_RETURN))
        {
            switch(Selection)
            {
                case 1:
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    PackName = GetString("","Enter the Levelpack name:");
                    Input->Reset();
                    if (strlen(PackName) > 0)
                    {
                        SDL_SetRenderTarget(Renderer, NULL);
                        SDL_RenderTexture(Renderer, IMGTitleScreen,NULL,NULL);
                        CreatorName = GetString("","Enter the Levelpack Creator name:");
                        Input->Reset();
                        if(strlen(CreatorName)>0)
                        {
                            sprintf(LevelPackName,"%s",PackName);
                            sprintf(LevelPackFileName,"%s",PackName);
                            AddUnderScores(LevelPackFileName);
							sprintf(Tekst,"%s", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
#ifdef WIN32
                            mkdir(Tekst);
#else
                            mkdir(Tekst,S_IRWXO|S_IRWXU|S_IRWXG);
#endif

							sprintf(Tekst,"%s/.blips_levelpacks", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
#ifdef WIN32
                            mkdir(Tekst);
#else
                            mkdir(Tekst,S_IRWXO|S_IRWXU|S_IRWXG);
#endif

							sprintf(Tekst,"%s/.blips_levelpacks/%s", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName);
#ifdef WIN32
                            mkdir(Tekst);
#else
                            mkdir(Tekst,S_IRWXO|S_IRWXU|S_IRWXG);
#endif
							sprintf(FileName,"%s/.blips_levelpacks/%s/credits.dat", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName);
                            Fp = fopen(FileName,"wt");
                            if (Fp)
                            {
                                fprintf(Fp,"[Credits]\nCreator='%s'\n",CreatorName);
                                fclose(Fp);
                            }
                            SearchForLevelPacks();
                            for (Teller=0;Teller<InstalledLevelPacksCount;Teller++)
                                if(strcmp(PackName,InstalledLevelPacks[Teller]) == 0)
                                {
                                    SelectedLevelPack = Teller;
                                    sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                                    sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                                    AddUnderScores(LevelPackFileName);
                                }
                        }
                        delete[] CreatorName;
                    }
                    delete[] PackName;
                    break;
                case 2:
                    if (InstalledLevelPacksCount >0)
                    {
                        FindLevels();
                        SelectedLevel=0;
                        GameState=GSStageSelect;
                        if (GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    }
                    break;
                case 3:
                    if (InstalledLevelPacksCount >0)
                    {
                        if (GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                        sprintf(Tekst,"do you want to delete the selected level pack:\n\"%s\"\nAll Levels in Levelpack will be deleted !!!\n\nPress A to Delete, X to Cancel",InstalledLevelPacks[SelectedLevelPack]);
                        if(AskQuestion(Tekst))
                        {
                            FindLevels();
                            for(Teller=1;Teller<=InstalledLevels;Teller++)
                            {
                                sprintf(Tekst,"%s/.blips_levelpacks/%s/level%d.lev",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName,Teller);
                                if(FileExists(Tekst))
                                {
                                    remove(Tekst);
                                }
                            }
                            sprintf(Tekst,"%s/.blips_levelpacks/%s/credits.dat", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName);
                            if(FileExists(Tekst))
                            {
                                remove(Tekst);
                            }
                            sprintf(Tekst,"%s/.blips_levelpacks/%s",SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName);
                            rmdir(Tekst);

                            SearchForLevelPacks();
                        }
                        Input->Reset();

                    }
                    break;
                case 5:
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    GameState = GSTitleScreen;
                    break;
            }
        }
        SDL_SetRenderTarget(Renderer, Buffer);
        SDL_FRect Rect1;
        Rect1.x = 60.0f*UI_WIDTH_SCALE;
        Rect1.y = 80.0f*UI_HEIGHT_SCALE;
        Rect1.w = 200.0f*UI_WIDTH_SCALE;
        Rect1.h = 80.0f*UI_HEIGHT_SCALE;
        SDL_SetRenderDrawColor(Renderer, MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.a);
        SDL_RenderFillRect(Renderer, &Rect1);
        SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
        SDL_RenderRect(Renderer, &Rect1);
        SDL_FRect Rect2;
        Rect2.x = 61.0f*UI_WIDTH_SCALE;
        Rect2.y = 81.0f*UI_HEIGHT_SCALE;
        Rect2.w = 198.0f*UI_WIDTH_SCALE;
        Rect2.h = 78.0f*UI_HEIGHT_SCALE;
        SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
        SDL_RenderRect(Renderer, &Rect2);
		sprintf(Tekst,"Create New Levelpack\nLoad Selected LevelPack\nDelete Selected Levelpack\n<%s>\nMain Menu",LevelPackName);
		WriteText(BigFont,Tekst,strlen(Tekst),90*UI_WIDTH_SCALE,85*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		if (Selection > 1)
		{
			strcpy(Tekst,"\n");
			for(Teller=2;Teller<Selection;Teller++)
				strcat(Tekst, "\n");
			strcat(Tekst, ">>");
		}
		else
			strcpy(Tekst,">>");
		WriteText(BigFont,Tekst,strlen(Tekst),65*UI_WIDTH_SCALE,85*UI_HEIGHT_SCALE,2,MenuTextColor,false);
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
	delete Input;
}
