#include <SDL.h>
#include <SDL_framerate.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
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
    Tmp = SDL_DisplayFormat(Buffer);
	CInput *Input = new CInput(InputDelay, disableJoysticks);
	char *Tekst = new char[300];
	while (GameState == GSLevelEditorMenu)
	{
        if(GlobalSoundEnabled)
            if (! Mix_PlayingMusic())
            {
                SelectedMusic = 0;
                Mix_PlayMusic(Music[SelectedMusic],0);
                SetVolume(Volume);
            }
		SDL_BlitSurface(IMGTitleScreen,NULL,Tmp,NULL);
		Input->Update();

		if(Input->KeyboardHeld[KEY_B] || Input->SpecialsHeld[SPECIAL_QUIT_EV] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_B)])
		{
			if (GlobalSoundEnabled)
            	Mix_PlayChannel(-1,Sounds[SND_BACK],0);
            GameState = GSTitleScreen;
		}

		if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_L)] || Input->KeyboardHeld[KEY_L] || Input->KeyboardHeld[SDLK_l]))
        {
            Input->Delay();
        }

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

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[KEY_LEFT]))
        {
            if(Selection==4)
                if (InstalledLevelPacksCount > 0)
                {
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    SelectedLevelPack--;
                    if(SelectedLevelPack < 0)
                        SelectedLevelPack = InstalledLevelPacksCount -1;
                    sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                    sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                    AddUnderScores(LevelPackFileName);
                }
            Input->Delay();
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_RIGHT)] || Input->KeyboardHeld[KEY_RIGHT]))
        {
            if (Selection==4)
                if (InstalledLevelPacksCount > 0)
                {
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    SelectedLevelPack++;
                    if(SelectedLevelPack > InstalledLevelPacksCount-1)
                        SelectedLevelPack = 0;
                    sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                    sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                    AddUnderScores(LevelPackFileName);
                }
            Input->Delay();
        }

        if((Input->Ready()) && (Input->KeyboardHeld[KEY_DOWN] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_DOWN)]) &&
           !(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_R)] || Input->KeyboardHeld[SDLK_r]))
        {
            if (Selection < 5)
            {
                Selection++;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            }

            Input->Delay();
        }

        if((Input->Ready()) && (Input->KeyboardHeld[KEY_UP] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_UP)]) &&
           !(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_R)] || Input->KeyboardHeld[SDLK_r]))
        {
            if (Selection > 1)
            {
                Selection--;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_MENU],0);
            }
            Input->Delay();
        }

        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] || Input->KeyboardHeld[KEY_A] || Input->KeyboardHeld[KEY_START]))
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
                        SDL_BlitSurface(IMGTitleScreen,NULL,Buffer,NULL);
                        CreatorName = GetString("","Enter the Levelpack Creator name:");
                        Input->Reset();
                        if(strlen(CreatorName)>0)
                        {
                            sprintf(LevelPackName,"%s",PackName);
                            sprintf(LevelPackFileName,"%s",PackName);
                            AddUnderScores(LevelPackFileName);
							sprintf(Tekst,"%s", getenv("HOME") == NULL ? ".": getenv("HOME"));
#ifdef WIN32
                            mkdir(Tekst);
#else
                            mkdir(Tekst,S_IRWXO|S_IRWXU|S_IRWXG);
#endif

							sprintf(Tekst,"%s/.blips_levelpacks", getenv("HOME") == NULL ? ".": getenv("HOME"));
#ifdef WIN32
                            mkdir(Tekst);
#else
                            mkdir(Tekst,S_IRWXO|S_IRWXU|S_IRWXG);
#endif

							sprintf(Tekst,"%s/.blips_levelpacks/%s", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName);
#ifdef WIN32
                            mkdir(Tekst);
#else
                            mkdir(Tekst,S_IRWXO|S_IRWXU|S_IRWXG);
#endif
							sprintf(FileName,"%s/.blips_levelpacks/%s/credits.dat", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName);
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
                                sprintf(Tekst,"%s/.blips_levelpacks/%s/level%d.lev",getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName,Teller);
                                if(FileExists(Tekst))
                                {
                                    remove(Tekst);
                                }
                            }
                            sprintf(Tekst,"%s/.blips_levelpacks/%s/credits.dat", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName);
                            if(FileExists(Tekst))
                            {
                                remove(Tekst);
                            }
                            sprintf(Tekst,"%s/.blips_levelpacks/%s",getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName);
                            rmdir(Tekst);

                            SearchForLevelPacks();
                        }
                        if (GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                        Input->Reset();

                    }
                    break;
                case 4:
                    if (InstalledLevelPacksCount > 0)
                    {
                        if (GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                        SelectedLevelPack++;
                        if(SelectedLevelPack > InstalledLevelPacksCount-1)
                            SelectedLevelPack = 0;
                        sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        AddUnderScores(LevelPackFileName);
                    }
                    break;
                case 5:
                    if (GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                    GameState = GSTitleScreen;
                    break;
            }
            Input->Delay();
        }
		boxRGBA(Tmp,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Tmp,62*UI_WIDTH_SCALE,82*UI_HEIGHT_SCALE,258*UI_WIDTH_SCALE,158*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		sprintf(Tekst,"Create New Levelpack\nLoad Selected LevelPack\nDelete Selected Levelpack\n<%s>\nMain Menu",LevelPackName);
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),90*UI_WIDTH_SCALE,85*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		if (Selection > 1)
		{
			strcpy(Tekst,"\n");
			for(Teller=2;Teller<Selection;Teller++)
				strcat(Tekst, "\n");
			strcat(Tekst, ">>");
		}
		else
			strcpy(Tekst,">>");
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),65*UI_WIDTH_SCALE,85*UI_HEIGHT_SCALE,2,MenuTextColor,false);
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
        SDL_framerateDelay(&Fpsman);
	}
	delete[] Tekst;
	SDL_FreeSurface(Tmp);
	delete Input;
}
