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
	SDL_Rect Rect;
    Rect.w = Buffer->w;
    Rect.h = Buffer->h;
    Rect.x = StartScreenX;
    Rect.y = StartScreenY;
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
				//Mix_HookMusicFinished(MusicFinished);
                SetVolume(Volume);
            }
		SDL_BlitSurface(IMGTitleScreen,NULL,Tmp,NULL);
		Input->Update();

		if(Input->KeyboardHeld[SDLK_ESCAPE] || Input->SpecialsHeld[SPECIAL_QUIT_EV] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_SELECT)])
		{
			if (GlobalSoundEnabled)
            	Mix_PlayChannel(-1,Sounds[SND_BACK],0);
            GameState = GSTitleScreen;
		}

		if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_R)] || Input->KeyboardHeld[SDLK_r])
        {
            if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[SDLK_LEFT]))
            {
                if(StartScreenX - 2 >= 0)
                    StartScreenX -=2;
                Input->Delay();
            }

            if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_RIGHT)] || Input->KeyboardHeld[SDLK_RIGHT]))
            {
                if(StartScreenX + 2 + Buffer->w <= Screen->w)
                    StartScreenX +=2;
                Input->Delay();
            }

            if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_UP)] || Input->KeyboardHeld[SDLK_UP]))
            {
                if(StartScreenY - 2 >=0)
                    StartScreenY -=2;
                Input->Delay();
            }

            if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_DOWN)] || Input->KeyboardHeld[SDLK_DOWN]))
            {
                if(StartScreenY + 2 + Buffer->h <= Screen->h)
                    StartScreenY +=2;
                Input->Delay();
            }
        }

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_L)] || Input->KeyboardHeld[SDLK_PAGEDOWN] || Input->KeyboardHeld[SDLK_l]))
        {
            Input->Delay();
        }

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

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[SDLK_LEFT]))
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

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_RIGHT)] || Input->KeyboardHeld[SDLK_RIGHT]))
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

        if((Input->Ready()) && (Input->KeyboardHeld[SDLK_DOWN] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_DOWN)]) &&
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

        if((Input->Ready()) && (Input->KeyboardHeld[SDLK_UP] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_UP)]) &&
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

        if(Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] || Input->KeyboardHeld[SDLK_SPACE] || Input->KeyboardHeld[SDLK_RETURN])
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
                            sprintf(Tekst,"./levelpacks/%s",LevelPackFileName);
                            //system(Command);
#ifdef WIN32
                            mkdir(Tekst);
#else
                            mkdir(Tekst,S_IRWXO|S_IRWXU|S_IRWXG);
#endif
                            //sprintf(Command,"chdmod 777 ./levelpacks/%s",LevelPackFileName);
                            //system(Command);
                            sprintf(FileName,"./levelpacks/%s/credits.dat",LevelPackFileName);
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
                            for(Teller=1;Teller<50;Teller++)
                            {
                                sprintf(Tekst,"./levelpacks/%s/level%d.lev",LevelPackFileName,Teller);
                                if(FileExists(Tekst))
                                {
                                    remove(Tekst);
                                }
                                else
                                    break;
                            }
                            sprintf(Tekst,"./levelpacks/%s/credits.dat",LevelPackFileName);
                            if(FileExists(Tekst))
                            {
                                remove(Tekst);
                            }
                            sprintf(Tekst,"./levelpacks/%s",LevelPackFileName);
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
		boxRGBA(Tmp,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Tmp,61*UI_WIDTH_SCALE,81*UI_HEIGHT_SCALE,259*UI_WIDTH_SCALE,159*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		sprintf(Tekst,"Create New Levelpack\nLoad Selected LevelPack\nDelete Selected Levelpack\n<%s>\nMain Menu",LevelPackName);
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),90*UI_WIDTH_SCALE,85*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		if (Selection > 1)
		{
			sprintf(Tekst,"\n");
			for(Teller=2;Teller<Selection;Teller++)
				sprintf(Tekst,"%s%s",Tekst,"\n");
			sprintf(Tekst,"%s%s",Tekst,">>");
		}
		else
			sprintf(Tekst,">>");
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
        SDL_FillRect(Buffer1,NULL,SDL_MapRGB(Buffer1->format,0,0,0));
        SDL_BlitSurface(Buffer,NULL,Buffer1,&Rect);
        SDL_BlitSurface(Buffer1,NULL,Screen,NULL);
		SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer1,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
        SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
        SDL_FreeSurface(ScreenBufferZoom);
        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);
	}
	delete[] Tekst;
	SDL_FreeSurface(Tmp);
	delete Input;
}
