#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_framerate.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include "Common.h"
#include "GameFuncs.h"
#include "CInput.h"
#include "SDL_rotozoom.h"

void UnLoadGraphics()
{
    if(IMGGrid)
	{
        SDL_FreeSurface(IMGGrid);
	}

    if(IMGWall)
	{
        SDL_FreeSurface(IMGWall);
	}

	if(IMGBackground)
	{
		SDL_FreeSurface(IMGBackground);
	}

	if(IMGFloor)
	{
		SDL_FreeSurface(IMGFloor);
	}

	if(IMGPlayer)
	{
		SDL_FreeSurface(IMGPlayer);
	}

	if(IMGBox)
	{
		SDL_FreeSurface(IMGBox);
	}

	if(IMGEmpty)
	{
		SDL_FreeSurface(IMGEmpty);
	}

    if(IMGDiamond)
	{
		SDL_FreeSurface(IMGDiamond);
	}

	if(IMGBomb)
	{
		SDL_FreeSurface(IMGBomb);
	}

	if(IMGTitleScreen)
	{
		SDL_FreeSurface(IMGTitleScreen);
	}

    if(IMGIntro1)
	{
        SDL_FreeSurface(IMGIntro1);
	}

    if(IMGIntro2)
	{
        SDL_FreeSurface(IMGIntro2);
	}

    if(IMGIntro3)
	{
        SDL_FreeSurface(IMGIntro3);
	}

    if(IMGExplosion)
	{
        SDL_FreeSurface(IMGExplosion);
	}
}

void UnloadMusic()
{
	int Teller;
	if (GlobalSoundEnabled)
	{
		Mix_HaltMusic();
		for (Teller=0;Teller < MusicCount;Teller++)
			if (Music[Teller])
				Mix_FreeMusic(Music[Teller]);
	}
}

void LoadSounds()
{
	if (GlobalSoundEnabled)
	{
		Sounds[SND_MENU] = Mix_LoadWAV("./sound/menu.wav");
		Sounds[SND_SELECT] = Mix_LoadWAV("./sound/select.wav");
		Sounds[SND_ERROR] = Mix_LoadWAV("./sound/error.wav");
		Sounds[SND_STAGEEND] = Mix_LoadWAV("./sound/stageend.wav");
		Sounds[SND_EXPLODE] = Mix_LoadWAV("./sound/explode.wav");
		Sounds[SND_COLLECT] = Mix_LoadWAV("./sound/collect.wav");
		Sounds[SND_BACK] = Mix_LoadWAV("./sound/menuback.wav");
	}
}

void UnloadSounds()
{
	int Teller;
	for (Teller=0;Teller<NrOfSounds;Teller++)
		if(Sounds[Teller])
			Mix_FreeChunk(Sounds[Teller]);
}

void LoadJoystickSettings()
{
#ifdef RG35XX
    JoystickSetup->AddDefinition(BUT_B,"Go back / Quit",4,4);
    JoystickSetup->AddDefinition(BUT_R,"Next Music / Next Part",8,8);
    JoystickSetup->AddDefinition(BUT_L,"Prev Part",7,7);
    JoystickSetup->AddDefinition(BUT_LEFT,"Left",JOYSTICK_LEFT,JOYSTICK_LEFT);
    JoystickSetup->AddDefinition(BUT_RIGHT,"Right",JOYSTICK_RIGHT,JOYSTICK_RIGHT);
    JoystickSetup->AddDefinition(BUT_DOWN,"Down",JOYSTICK_DOWN,JOYSTICK_DOWN);
    JoystickSetup->AddDefinition(BUT_UP,"Up",JOYSTICK_UP,JOYSTICK_UP);
    JoystickSetup->AddDefinition(BUT_A,"Select / Place part",3,3);
    JoystickSetup->AddDefinition(BUT_X,"Center level",6,6);
    JoystickSetup->AddDefinition(BUT_Y,"New Level / Erase all parts",5,5);
    JoystickSetup->AddDefinition(BUT_SELECT,"Stats / Hide position",9,9);
    JoystickSetup->AddDefinition(BUT_START,"Restart / Test level",10,10);
    JoystickSetup->AddDefinition(BUT_VOLUP,"Volume up",13,13);
    JoystickSetup->AddDefinition(BUT_VOLMIN,"Volume down",12,12);
#else
    JoystickSetup->AddDefinition(BUT_B,"Go back / Quit",1,1);
    JoystickSetup->AddDefinition(BUT_R,"Next Music / Next Part",5,5);
    JoystickSetup->AddDefinition(BUT_L,"Prev Part",4,4);
    JoystickSetup->AddDefinition(BUT_LEFT,"Left",JOYSTICK_LEFT,JOYSTICK_LEFT);
    JoystickSetup->AddDefinition(BUT_RIGHT,"Right",JOYSTICK_RIGHT,JOYSTICK_RIGHT);
    JoystickSetup->AddDefinition(BUT_DOWN,"Down",JOYSTICK_DOWN,JOYSTICK_DOWN);
    JoystickSetup->AddDefinition(BUT_UP,"Up",JOYSTICK_UP,JOYSTICK_UP);
    JoystickSetup->AddDefinition(BUT_A,"Select / Place part",0,0);
    JoystickSetup->AddDefinition(BUT_X,"Center level",2,2);
    JoystickSetup->AddDefinition(BUT_Y,"New Level / Erase all parts",3,3);
    JoystickSetup->AddDefinition(BUT_SELECT,"Stats / Hide position",6,6);
    JoystickSetup->AddDefinition(BUT_START,"Restart / Test level",7,7);
    JoystickSetup->AddDefinition(BUT_VOLUP,"Volume up",9,9);
    JoystickSetup->AddDefinition(BUT_VOLMIN,"Volume down",8,8);
#endif
JoystickSetup->LoadCurrentButtonValues("./joystick.def");
}



void FindLevels()
{
	int Teller=1;
	char *FileName = new char[FILENAME_MAX];
	char *FileName2 = new char[FILENAME_MAX];
	InstalledLevels = 0;
	bool homepath=false;
	sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, Teller);
	sprintf(FileName2,"./levelpacks/%s/level%d.lev",LevelPackFileName,Teller);		
	while (FileExists(FileName) || FileExists(FileName2))
	{
		Teller+=30;
		sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, Teller);
		sprintf(FileName2,"./levelpacks/%s/level%d.lev",LevelPackFileName,Teller);
	}
	while (!FileExists(FileName) && !FileExists(FileName2) && (Teller >=1) )
	{
		Teller--;
		sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName, Teller);
		sprintf(FileName2,"./levelpacks/%s/level%d.lev",LevelPackFileName,Teller);
	}
	InstalledLevels=Teller;
	delete[] FileName;
	delete[] FileName2;
}

void WriteText(SDL_Surface* Surface,TTF_Font* FontIn,char* Tekst,int NrOfChars,int X,int Y,int YSpacing,SDL_Color ColorIn,bool Centered)
{
	char List[100][255];
	int Lines,Teller,Chars;
	SDL_Rect DstRect;
	SDL_Surface* TmpSurface1;
	Lines = 0;
	Chars = 0;
	for (Teller=0;Teller<NrOfChars;Teller++)
	{
		if(Lines > 100)
			break;
		if((Tekst[Teller] == '\n') || (Chars==255))
		{
			List[Lines][Chars]='\0';
			Lines++;
			Chars = 0;
		}
		else
		{
		 	List[Lines][Chars]=Tekst[Teller];
		 	Chars++;
		}
	}
	List[Lines][Chars] = '\0';
	for (Teller=0;Teller <= Lines;Teller++)
	{
		if(strlen(List[Teller]) > 0)
		{
			TmpSurface1 = TTF_RenderText_Blended(FontIn,List[Teller],ColorIn);
			if(Centered)
                DstRect.x = (Surface->w /2) - (TmpSurface1->w / 2);
			else
                DstRect.x = X;
			DstRect.y = Y + (Teller) * TTF_FontLineSkip(FontIn) + (Teller*YSpacing);
			DstRect.w = TmpSurface1->w;
			DstRect.h = TmpSurface1->h;
			SDL_BlitSurface(TmpSurface1,NULL,Surface,&DstRect);
			SDL_FreeSurface(TmpSurface1);
		}
	}
}


bool FileExists(char * FileName)
{
	FILE *Fp;
	Fp = fopen(FileName,"rb");
	if (Fp)
	{
		fclose(Fp);
		return true;
	}
	else
		return false;
}


char chr(int ascii)
{
	return((char)ascii);
}

int ord(char chr)
{
	return((int)chr);
}

void AddUnderScores (char *string)
{
	size_t Teller;
	for(Teller=0;Teller<strlen(string);Teller++)
		if(string[Teller] == ' ')
			string[Teller] = '_';
}

void RemoveUnderScores (char *string)
{
	size_t Teller;
	for(Teller=0;Teller<strlen(string);Teller++)
		if(string[Teller] == '_')
			string[Teller] = ' ';
}


char *GetString(char *NameIn,char *Msg)
{
	char *PackName = new char[21];
	bool End=false,SubmitChanges=false;
	int Teller,MaxSelection=0, Selection = 0,asci=97;
	CInput *Input = new CInput(InputDelay, disableJoysticks);
	sprintf(PackName,"%s",NameIn);
	MaxSelection = strlen(NameIn);
	PackName[Selection+1]='\0';
	if (MaxSelection == 0)
		PackName[MaxSelection]=chr(asci);
	char Tekst[100];
	while (!End)
	{
	    if(GlobalSoundEnabled)
		if(!Mix_PlayingMusic())
        {
            Mix_PlayMusic(Music[SelectedMusic],0);
            SetVolume(Volume);
        }
		Input->Update();

        if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
		{
            GameState = GSQuit;
			SubmitChanges=false;
			End = true;
		}

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_LEFT)] || Input->KeyboardHeld[SDLK_LEFT]))
        {
            if (Selection > 0)
            {	Selection--;
                asci = ord(PackName[Selection]);
            }
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_RIGHT)] || Input->KeyboardHeld[SDLK_RIGHT]))
        {
            if (Selection < 19)
            {
                Selection++;
                if (Selection > MaxSelection)
                {
                    PackName[Selection] = chr(97);
                    PackName[Selection+1] = '\0';
                    MaxSelection=Selection;
                }
                asci = ord(PackName[Selection]);
            }
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_UP)] || Input->KeyboardHeld[SDLK_UP]))
        {
            asci++;
            if (asci==123)
			{
                asci=32;
			}

            if (asci==33)
			{
                asci=48;
			}
            if (asci==58)
			{
                asci=97;                
			}
			PackName[Selection] = chr(asci);
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_DOWN)] || Input->KeyboardHeld[SDLK_DOWN]))
        {
            asci--;
            if(asci==96)
			{
                asci=57;
			}
            if(asci==47)
			{
                asci=32;
			}
            if(asci==31)
			{
                asci=122;
			}
            PackName[Selection] = chr(asci);
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] || Input->KeyboardHeld[SDLK_q] || Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_RETURN]))
        {
            if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
            End = true;
            SubmitChanges=true;
        }

        if(Input->Ready() && (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_X)] || Input->KeyboardHeld[SDLK_x] || Input->KeyboardHeld[SDLK_z] || Input->KeyboardHeld[SDLK_ESCAPE] ))
        {
            End=true;
            SubmitChanges=false;
        }
		SDL_BlitSurface(IMGTitleScreen,NULL,Buffer,NULL);
		boxRGBA(Buffer,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
		rectangleRGBA(Buffer,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		rectangleRGBA(Buffer,61*UI_WIDTH_SCALE,81*UI_HEIGHT_SCALE,259*UI_WIDTH_SCALE,159*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
		WriteText(Buffer,font,Msg,strlen(Msg),65*UI_WIDTH_SCALE,85*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		WriteText(Buffer,MonoFont,PackName,strlen(PackName),85*UI_WIDTH_SCALE,110*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		if (Selection > 0)
		{
			strcpy(Tekst," ");
			for (Teller=1;Teller<Selection;Teller++)
				strcat(Tekst," ");
			strcat(Tekst,"_");
		}
		else
			strcpy(Tekst,"_");
		WriteText(Buffer,MonoFont,Tekst,strlen(Tekst),85*UI_WIDTH_SCALE,112*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		sprintf(Tekst,"Use Up,Down,Left,right. A = Ok X = Cancel" );
		WriteText(Buffer,font,Tekst,strlen(Tekst),65*UI_WIDTH_SCALE,135*UI_HEIGHT_SCALE,2,MenuTextColor,false);
        if ((WINDOW_WIDTH != ORIG_WINDOW_WIDTH) || (WINDOW_HEIGHT != ORIG_WINDOW_HEIGHT))
		{
			SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
			SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
			SDL_FreeSurface(ScreenBufferZoom);
		}
		else
		{
			SDL_BlitSurface(Buffer, NULL, Screen, NULL);
		}
        SDL_Flip(Screen);
        SDL_framerateDelay(&Fpsman);
	}
	PackName[MaxSelection+1] = '\0';
	while ((PackName[0] == ' ') && (MaxSelection>-1))
	{
		for (Teller=0;Teller<MaxSelection;Teller++)
			PackName[Teller] = PackName[Teller+1];
		MaxSelection--;
	}
	if (MaxSelection>-1)
		while ((PackName[MaxSelection] == ' ') && (MaxSelection>0))
		{
			PackName[MaxSelection] = '\0';
			MaxSelection--;
		}
	if (!SubmitChanges)
		sprintf(PackName,"%s",NameIn);
	delete Input;
	return PackName;
}


void SaveUnlockData()
{
	FILE *Fp;
	int Teller;
	char Filename[FILENAME_MAX];
	unsigned char LevelHash[4];
	unsigned char HashBuffer[64];
	int CheckSum = 0;
	LevelHash[0] = HashTable[UnlockedLevels-1] ;
	LevelHash[1] = HashTable[UnlockedLevels];
	LevelHash[2] = HashTable[UnlockedLevels+1];
	LevelHash[3] = HashTable[UnlockedLevels+2];
	sprintf(Filename,"%s/.blips_%s.dat", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName);
	for (Teller=0;Teller<4;Teller++)
		LevelHash[Teller] = LevelHash[Teller] ^ LevelPackFileName[strlen(LevelPackFileName)-1];
	for (Teller=0;Teller<strlen(LevelPackFileName);Teller++)
		LevelHash[Teller%4] = LevelHash[Teller%4] ^ LevelPackFileName[Teller];
	LevelHash[0] = LevelHash[0] ^ LevelHash[2];
	LevelHash[1] = LevelHash[1] ^ LevelHash[0];
	LevelHash[2] = LevelHash[2] ^ LevelHash[3];
	LevelHash[3] = LevelHash[3] ^ LevelHash[2];
	for (Teller=0;Teller<64;Teller++)
		if ((Teller+1) % 16 == 0)
		{
			HashBuffer[Teller] = LevelHash[(Teller)/16];
			CheckSum += HashBuffer[Teller];
		}
		else
		{
			HashBuffer[Teller] = rand() % 256;
			CheckSum += HashBuffer[Teller];
		}
	CheckSum = CheckSum ^ 50;
	Fp = fopen(Filename,"wb");
	if (Fp)
	{
		fwrite(HashBuffer,1,64,Fp);
		fwrite(&CheckSum,sizeof(int),1,Fp);
		fclose(Fp);
	}
}

/*void SaveAllHashes()
{
	FILE *Fp;
	char Filename[FILENAME_MAX];
	char LevelHash[4];
	int Teller;
	sprintf(Filename,"./hashes.txt");
	Fp = fopen(Filename,"wt");
	if (Fp)
	{
		for (Teller=0;Teller<1000;Teller++)
		{
			LevelHash[0] = HashTable[Teller];
			LevelHash[1] = HashTable[Teller+1];
			LevelHash[2] = HashTable[Teller+2];
			LevelHash[3] = HashTable[Teller+3];

			fprintf(Fp,"%d|%d|%d|%d\n",(int)LevelHash[0],(int)LevelHash[1],(int)LevelHash[2],(int)LevelHash[3]);

		}
	fclose(Fp);
	sync();
	}
}
*/

void LoadUnlockData()
{
	FILE *Fp;
	unsigned char LevelHash[4];
	int Teller=0;
	unsigned char HashBuffer[64];
	char Filename[FILENAME_MAX];
	sprintf(Filename,"%s/.blips_%s.dat", getenv("HOME") == NULL ? ".": getenv("HOME"), LevelPackFileName);
	Fp = fopen(Filename,"rb");
	int CheckSum,ValidCheckSum=0;
	if (Fp)
	{
		fflush(Fp);
		fread(HashBuffer,1,64,Fp);
		fread(&CheckSum,sizeof(int),1,Fp);
		fclose(Fp);
		for (Teller = 0 ;Teller<64;Teller++)
		{
			ValidCheckSum += HashBuffer[Teller];
			if ((Teller+1)%16 == 0)
				LevelHash[Teller/16] = HashBuffer[Teller];
		}
		CheckSum = CheckSum ^ 50;
		if (CheckSum == ValidCheckSum)
		{
			LevelHash[3] = LevelHash[3] ^ LevelHash[2];
			LevelHash[2] = LevelHash[2] ^ LevelHash[3];
			LevelHash[1] = LevelHash[1] ^ LevelHash[0];
			LevelHash[0] = LevelHash[0] ^ LevelHash[2];
			for (Teller=0;Teller<(int)strlen(LevelPackFileName);Teller++)
				LevelHash[Teller%4] = LevelHash[Teller%4] ^ LevelPackFileName[Teller];
			for (Teller=0;Teller<4;Teller++)
				LevelHash[Teller] = LevelHash[Teller] ^ LevelPackFileName[strlen(LevelPackFileName)-1];

			Teller=0;
			while (LevelHash[0] != HashTable[Teller] || LevelHash[1] != HashTable[Teller+1] || 	LevelHash[2] != HashTable[Teller+2] || LevelHash[3] != HashTable[Teller+3] && Teller+3 < 1004)
				Teller++;
			if (Teller < InstalledLevels)
				UnlockedLevels = Teller+1;
			else
				UnlockedLevels = 1;
		}
		else
			UnlockedLevels = 1;
	}
	else
	 	UnlockedLevels = 1;

}

bool AskQuestion(char *Msg)
{
	bool Result = false;
	CInput *Input = new CInput(InputDelay, disableJoysticks);
	boxRGBA(Buffer,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
	rectangleRGBA(Buffer,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
	rectangleRGBA(Buffer,61*UI_WIDTH_SCALE,81*UI_HEIGHT_SCALE,259*UI_WIDTH_SCALE,159*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
	WriteText(Buffer,font,Msg,strlen(Msg),65*UI_WIDTH_SCALE,83*UI_HEIGHT_SCALE,2,MenuTextColor,false);
	if ((WINDOW_WIDTH != ORIG_WINDOW_WIDTH) || (WINDOW_HEIGHT != ORIG_WINDOW_HEIGHT))
	{
		SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
		SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
		SDL_FreeSurface(ScreenBufferZoom);
	}
	else
	{
		SDL_BlitSurface(Buffer, NULL, Screen, NULL);
	}
    SDL_Flip(Screen);
	{
		while (!(Input->KeyboardHeld[SDLK_z] || Input->SpecialsHeld[SPECIAL_QUIT_EV] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] || Input->KeyboardHeld[SDLK_RETURN] || Input->KeyboardHeld[SDLK_SPACE] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_X)] || Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_q] || Input->KeyboardHeld[SDLK_y] || Input->KeyboardHeld[SDLK_ESCAPE] || Input->KeyboardHeld[SDLK_n] || Input->KeyboardHeld[SDLK_x]))
		{
		    Input->Update();
			if(GlobalSoundEnabled)
			if(!Mix_PlayingMusic())
			{
				Mix_PlayMusic(Music[SelectedMusic],0);
				SetVolume(Volume);
			}
			SDL_framerateDelay(&Fpsman);
		}
		if (Input->SpecialsHeld[SPECIAL_QUIT_EV])
            GameState = GSQuit;
		if (Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] || Input->KeyboardHeld[SDLK_RETURN] || Input->KeyboardHeld[SDLK_SPACE] || Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_q] || Input->KeyboardHeld[SDLK_y])
			Result = true;
	}
	delete Input;
	return Result;
}

void PrintForm(char *msg)
{
    CInput *Input = new CInput(InputDelay, disableJoysticks);
	boxRGBA(Buffer,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxColor.r,MenuBoxColor.g,MenuBoxColor.b,MenuBoxColor.unused);
	rectangleRGBA(Buffer,60*UI_WIDTH_SCALE,80*UI_HEIGHT_SCALE,260*UI_WIDTH_SCALE,160*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
	rectangleRGBA(Buffer,61*UI_WIDTH_SCALE,81*UI_HEIGHT_SCALE,259*UI_WIDTH_SCALE,159*UI_HEIGHT_SCALE,MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.unused);
	WriteText(Buffer,font,msg,strlen(msg),65*UI_WIDTH_SCALE,83*UI_HEIGHT_SCALE,2,MenuTextColor,false);
	if ((WINDOW_WIDTH != ORIG_WINDOW_WIDTH) || (WINDOW_HEIGHT != ORIG_WINDOW_HEIGHT))
	{
		SDL_Surface *ScreenBufferZoom = zoomSurface(Buffer,(double)WINDOW_WIDTH / ORIG_WINDOW_WIDTH,(double)WINDOW_HEIGHT / ORIG_WINDOW_HEIGHT,0);
		SDL_BlitSurface(ScreenBufferZoom,NULL,Screen,NULL);
		SDL_FreeSurface(ScreenBufferZoom);
	}
	else
	{
		SDL_BlitSurface(Buffer, NULL, Screen, NULL);
	}
    SDL_Flip(Screen);
    while (!( Input->SpecialsHeld[SPECIAL_QUIT_EV] || Input->JoystickHeld[0][JoystickSetup->GetButtonValue(BUT_A)] || Input->KeyboardHeld[SDLK_ESCAPE] || Input->KeyboardHeld[SDLK_a] || Input->KeyboardHeld[SDLK_q] || Input->KeyboardHeld[SDLK_RETURN] || Input->KeyboardHeld[SDLK_SPACE]))
    {
        Input->Update();
        if(GlobalSoundEnabled)
        if(!Mix_PlayingMusic())
        {
            Mix_PlayMusic(Music[SelectedMusic],0);
            SetVolume(Volume);
        }
        SDL_framerateDelay(&Fpsman);
    }
	if(Input->SpecialsHeld[SPECIAL_QUIT_EV])
		GameState = GSQuit;

	delete Input;
}


void SetVolume(const int VolumeIn)
{
	if (GlobalSoundEnabled)
	{
		Mix_Volume(0,VolumeIn);
		Mix_Volume(1,VolumeIn);
		Mix_VolumeMusic(VolumeIn);
	}
}

void IncVolume()
{
	if (Volume < 128)
	{
		Volume += 8;
		SetVolume(Volume);
	}
}

void DecVolume()
{
	if (Volume > 0)
	{
		Volume-= 8;
		SetVolume(Volume);
	}
}


void LoadSettings()
{
	FILE *Fp;
	char FileName[FILENAME_MAX];
	sprintf(FileName,"%s/.blips_settings", getenv("HOME") == NULL ? ".": getenv("HOME"));
	Fp = fopen(FileName,"rt");
	if (Fp)
	{
		fscanf(Fp,"Volume=%d\n",&Volume);
		fclose(Fp);
	}
	else
	{
		Volume = 128;
	}
	SetVolume(Volume);
}

void SaveSettings()
{
	FILE *Fp;
	char FileName[FILENAME_MAX];
	sprintf(FileName,"%s/.blips_settings", getenv("HOME") == NULL ? ".": getenv("HOME"));
	Fp = fopen(FileName,"wt");
	if (Fp)
	{
		fprintf(Fp,"Volume=%d\n",Volume);
		fclose(Fp);
	}
}

void SearchForMusic()
{
	struct dirent *Entry;
	DIR *Directory;
	struct stat Stats;
	int Teller;
	char FileName[FILENAME_MAX];
	if (GlobalSoundEnabled)
		Music[0] = Mix_LoadMUS("./music/title.mod");
	Teller=1;
	Directory = opendir("./music");
	if (Directory)
	{
		Entry=readdir(Directory);
		while(Entry)
		{
			sprintf(FileName,"./music/%s",Entry->d_name);
			stat(FileName,&Stats);
			if(!S_ISDIR(Stats.st_mode))
			{
				if(strncmp(".", Entry->d_name, 1) && (strcmp("title.mod",Entry->d_name) != 0) && (Teller< MaxMusicFiles))
				{
					if (GlobalSoundEnabled)
					{
						Music[Teller] = Mix_LoadMUS(FileName);
						Teller++;
					}
				}
			}
			Entry=readdir(Directory);
		}
		closedir(Directory);
	}
	MusicCount = Teller;
}

void DoSearchForLevelPacks(char* Path)
{
	struct dirent *Entry;
	DIR *Directory;
	struct stat Stats;
	char FileName[FILENAME_MAX];
	char Name[21];
	Directory = opendir(Path);
	if (Directory)
	{
		Entry=readdir(Directory);
		while(Entry)
		{
			sprintf(FileName,"%s/%s",Path,Entry->d_name);
			stat(FileName,&Stats);
			if(S_ISDIR(Stats.st_mode))
			{
				if(strncmp(".", Entry->d_name, 1)  && (InstalledLevelPacksCount< MaxLevelPacks) && (strlen(Entry->d_name) < 21))
				{
					sprintf(Name,"%s",Entry->d_name);
					RemoveUnderScores(Name);
					bool found = false;
					for (int i = 0; i < InstalledLevelPacksCount; i++)
					{
						if(strcmp(Name, InstalledLevelPacks[i]) == 0)
						{
							found = true;
							break;
						}
					}
					
					if(!found)
					{
						sprintf(InstalledLevelPacks[InstalledLevelPacksCount],"%s",Entry->d_name);
						RemoveUnderScores(InstalledLevelPacks[InstalledLevelPacksCount]);
						InstalledLevelPacksCount++;
					}
				}
			}
			Entry=readdir(Directory);
		}
		closedir(Directory);
	}
}

void SearchForLevelPacks()
{
	InstalledLevelPacksCount = 0;
	DoSearchForLevelPacks("./levelpacks");
	char Path[FILENAME_MAX];
	sprintf(Path, "%s/.blips_levelpacks", getenv("HOME") == NULL ? ".": getenv("HOME"));
	DoSearchForLevelPacks(Path);
	SelectedLevelPack=0;
	if (InstalledLevelPacksCount > 0)
	{
		sprintf(LevelPackName,"%s",InstalledLevelPacks[SelectedLevelPack]);
		sprintf(LevelPackFileName,"%s",InstalledLevelPacks[SelectedLevelPack]);
		AddUnderScores(LevelPackFileName);

	}
	else
	{
		sprintf(LevelPackName,"%s","");
		sprintf(LevelPackFileName,"%s","");
	}
}

void LoadGraphics()
{
	SDL_Surface *Tmp;
    if(IMGGrid)
	{
        SDL_FreeSurface(IMGGrid);
	}

	if(IMGWall)
	{
        SDL_FreeSurface(IMGWall);
	}

	if(IMGBackground)
	{
		SDL_FreeSurface(IMGBackground);
	}

	if(IMGFloor)
	{
		SDL_FreeSurface(IMGFloor);
	}

	if(IMGPlayer)
	{
		SDL_FreeSurface(IMGPlayer);
	}

	if(IMGBox)
	{
		SDL_FreeSurface(IMGBox);
	}

	if(IMGEmpty)
	{
		SDL_FreeSurface(IMGEmpty);
	}

    if(IMGDiamond)
	{
		SDL_FreeSurface(IMGDiamond);
	}

	if(IMGBomb)
	{
		SDL_FreeSurface(IMGBomb);
	}

	if(IMGTitleScreen)
	{
		SDL_FreeSurface(IMGTitleScreen);
	}

    if(IMGIntro1)
	{
        SDL_FreeSurface(IMGIntro1);
	}

    if(IMGIntro2)
	{
        SDL_FreeSurface(IMGIntro2);
	}

    if(IMGIntro3)
	{
        SDL_FreeSurface(IMGIntro3);
	}

    if(IMGExplosion)
	{
        SDL_FreeSurface(IMGExplosion);
	}

    Tmp = IMG_Load("./graphics/grid.png");
    IMGGrid = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGGrid, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGGrid->format,255,0,255));

    Tmp = IMG_Load("./graphics/intro1.png");
    IMGIntro1 = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./graphics/intro2.png");
    IMGIntro2 = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
 
    Tmp = IMG_Load("./graphics/intro3.png");
    IMGIntro3 = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./graphics/floor.png");
    IMGFloor = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGFloor, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGFloor->format,255,0,255));

    Tmp = IMG_Load("./graphics/explosion.png");
    IMGExplosion = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGExplosion, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGExplosion->format,255,0,255));

    Tmp = IMG_Load("./graphics/wall.png");
    IMGWall = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGWall, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGWall->format,255,0,255));

    Tmp = IMG_Load("./graphics/bomb.png");
    IMGBomb = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGBomb, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGBomb->format,255,0,255));

    Tmp = IMG_Load("./graphics/diamond.png");
    IMGDiamond = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGDiamond, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGDiamond->format,255,0,255));

    Tmp = IMG_Load("./graphics/box.png");
    IMGBox = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGBox, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGBox->format,255,0,255));

    Tmp = IMG_Load("./graphics/player.png");
    IMGPlayer = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGPlayer, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGPlayer->format,255,0,255));

    Tmp = IMG_Load("./graphics/empty.png");
    IMGEmpty = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGEmpty, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGEmpty->format,255,0,255));

    Tmp = IMG_Load("./graphics/titlescreen.png");
    IMGTitleScreen = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./graphics/background.png");
    IMGBackground = SDL_DisplayFormat(Tmp);
    SDL_FreeSurface(Tmp);
    SDL_SetColorKey(IMGBackground, (SDL_SRCCOLORKEY|SDL_RLEACCEL),SDL_MapRGB(IMGBackground->format,255,0,255));
}
