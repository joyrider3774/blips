#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
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
	Tmp = SDL_DisplayFormat(Buffer);
	while (GameState == GSTitleScreen)
	{
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
		SDL_BlitSurface(IMGTitleScreen,NULL,Tmp,NULL);
		Input->Update();

		if(Input->KeyboardHeld[KEY_B] || Input->SpecialsHeld[SPECIAL_QUIT_EV])
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

        if (Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[KEY_LEFT]))
        {
            if(Selection==3)
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
            if (Selection==3)
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
            if (Selection < 6)
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

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] || Input->KeyboardHeld[KEY_A] || Input->KeyboardHeld[KEY_START]))
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
                            SDL_BlitSurface(IMGTitleScreen,NULL,Buffer,NULL);
                            sprintf(Tekst,"There are no levels found in levelpack\n%s\n\nPlease create a level for this level pack\nfirst!",LevelPackName);
                            PrintForm(Tekst);
                            if (GlobalSoundEnabled)
                                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
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
                case 3:
                    if (InstalledLevelPacksCount > 0)
                    {
                        if (GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_SELECT], 0);
                        SelectedLevelPack++;
                        if(SelectedLevelPack > InstalledLevelPacksCount-1)
                            SelectedLevelPack = 0;
                        sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
                        AddUnderScores(LevelPackFileName);
                    }
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
            Input->Delay();
        }
   		boxRGBA(Tmp,60*UI_WIDTH_SCALE,70*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,165*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Tmp,60*UI_WIDTH_SCALE,70*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,165*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Tmp,62*UI_WIDTH_SCALE,72*UI_HEIGHT_SCALE,258*UI_WIDTH_SCALE,163*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		sprintf(Tekst,"Play Selected LevelPack\nLevel Editor\n<%s>\nCredits\nSetup External Joystick\nQuit",LevelPackName);
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),90*UI_WIDTH_SCALE,75*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		if (Selection > 1)
		{
			strcpy(Tekst,"\n");
			for(Teller=2;Teller<Selection;Teller++)
				strcat(Tekst, "\n");
			strcat(Tekst, ">>");
		}
		else
			strcpy(Tekst,">>");
		WriteText(Tmp,BigFont,Tekst,strlen(Tekst),65*UI_WIDTH_SCALE,75*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		
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
