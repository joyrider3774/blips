#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include "Common.h"
#include "GameFuncs.h"
#include "CInput.h"

char* assetPath(const char* assetSubPath)
{
	char* Result = (char*) SDL_malloc(FILENAME_MAX);

	snprintf(Result, FILENAME_MAX, "%s/%s", basePath, assetSubPath);
	
	return Result;
}

void logMessage(SDL_PRINTF_FORMAT_STRING const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
#if defined _WIN32 || defined __CYGWIN__
    vprintf(fmt, ap);
#else
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, ap);
#endif    
    va_end(ap);   
}

void UnLoadGraphics()
{
    if(IMGGrid)
	{
        SDL_DestroyTexture(IMGGrid);
	}

    if(IMGWall)
	{
        SDL_DestroyTexture(IMGWall);
	}

	if(IMGBackground)
	{
		SDL_DestroyTexture(IMGBackground);
	}

	if(IMGFloor)
	{
		SDL_DestroyTexture(IMGFloor);
	}

	if(IMGPlayer)
	{
		SDL_DestroyTexture(IMGPlayer);
	}

	if(IMGPlayer2)
	{
		SDL_DestroyTexture(IMGPlayer2);
	}

	if(IMGBox)
	{
		SDL_DestroyTexture(IMGBox);
	}

	if(IMGEmpty)
	{
		SDL_DestroyTexture(IMGEmpty);
	}

    if(IMGDiamond)
	{
		SDL_DestroyTexture(IMGDiamond);
	}

	if(IMGBomb)
	{
		SDL_DestroyTexture(IMGBomb);
	}

	if(IMGTitleScreen)
	{
		SDL_DestroyTexture(IMGTitleScreen);
	}

    if(IMGIntro1)
	{
        SDL_DestroyTexture(IMGIntro1);
	}

    if(IMGIntro2)
	{
        SDL_DestroyTexture(IMGIntro2);
	}

    if(IMGIntro3)
	{
        SDL_DestroyTexture(IMGIntro3);
	}

    if(IMGExplosion)
	{
        SDL_DestroyTexture(IMGExplosion);
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
		char *Tmp = assetPath("sound/menu.wav");
		Sounds[SND_MENU] = Mix_LoadWAV(Tmp);
		SDL_free(Tmp);

		Tmp = assetPath("sound/select.wav");
		Sounds[SND_SELECT] = Mix_LoadWAV(Tmp);
		SDL_free(Tmp);

		Tmp = assetPath("sound/error.wav");
		Sounds[SND_ERROR] = Mix_LoadWAV(Tmp);
		SDL_free(Tmp);

		Tmp = assetPath("sound/stageend.wav");
		Sounds[SND_STAGEEND] = Mix_LoadWAV(Tmp);
		SDL_free(Tmp);

		Tmp = assetPath("sound/explode.wav");
		Sounds[SND_EXPLODE] = Mix_LoadWAV(Tmp);
		SDL_free(Tmp);

		Tmp = assetPath("sound/collect.wav");
		Sounds[SND_COLLECT] = Mix_LoadWAV(Tmp);
		SDL_free(Tmp);

		Tmp = assetPath("sound/menuback.wav");
		Sounds[SND_BACK] = Mix_LoadWAV(Tmp);
		SDL_free(Tmp);
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
	char FileName[FILENAME_MAX];
	sprintf(FileName,"%s/.blips_joystick.def", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
	JoystickSetup->LoadCurrentButtonValues(FileName);
}



void FindLevels()
{
	int Teller=1;
	char *FileName = new char[FILENAME_MAX];
	char *FileName2 = new char[FILENAME_MAX];
	InstalledLevels = 0;
	bool homepath=false;
	sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, Teller);
	char* TmpPath = assetPath("levelpacks");
	sprintf(FileName2,"%s/%s/level%d.lev",TmpPath, LevelPackFileName,Teller);		
	while (FileExists(FileName) || FileExists(FileName2))
	{
		Teller+=30;
		sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, Teller);
		sprintf(FileName2,"%s/%s/level%d.lev",TmpPath, LevelPackFileName,Teller);		
	}
	while (!FileExists(FileName) && !FileExists(FileName2) && (Teller >=1) )
	{
		Teller--;
		sprintf(FileName,"%s/.blips_levelpacks/%s/level%d.lev", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName, Teller);
		sprintf(FileName2,"%s/%s/level%d.lev",TmpPath, LevelPackFileName,Teller);		
	}
	InstalledLevels=Teller;
	SDL_free(TmpPath);
	delete[] FileName;
	delete[] FileName2;
}

void WriteText(TTF_Font* FontIn,const char* Tekst,int NrOfChars,int X,int Y,int YSpacing,SDL_Color ColorIn,bool Centered)
{
	char List[100][255];
	int Lines,Teller,Chars;
	SDL_FRect DstRect;
	SDL_Surface* TmpSurface1;
	SDL_Texture *TmpTexture;
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
			TmpSurface1 = TTF_RenderText_Blended(FontIn,List[Teller],strlen(List[Teller]) * sizeof(char), ColorIn);
			if(Centered)
			{
				int w;
				SDL_GetCurrentRenderOutputSize(Renderer, &w, NULL);
                DstRect.x = (w /2) - (TmpSurface1->w / 2);
			}
			else
                DstRect.x = X;
			DstRect.y = Y + (Teller) * TTF_GetFontLineSkip(FontIn) + (Teller*YSpacing);
			DstRect.w = TmpSurface1->w;
			DstRect.h = TmpSurface1->h;
			TmpTexture = SDL_CreateTextureFromSurface(Renderer, TmpSurface1);
			SDL_RenderTexture(Renderer, TmpTexture,NULL,&DstRect);
			SDL_DestroyTexture(TmpTexture);
			SDL_DestroySurface(TmpSurface1);
		}
	}
}


bool FileExists(const char * FileName)
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


char *GetString(const char *NameIn,const char *Msg)
{
	char *PackName = new char[21];
	bool End=false,SubmitChanges=false;
	int Teller,MaxSelection=0, Selection = 0,asci=97;
	sprintf(PackName,"%s",NameIn);
	MaxSelection = strlen(NameIn);
	PackName[Selection+1]='\0';
	if (MaxSelection == 0)
		PackName[MaxSelection]=chr(asci);
	char Tekst[100];
	//reset 
	Input->SetInputDelay(InputDelay);
	Input->Reset();
	while (!End)
	{
		frameticks = SDL_GetPerformanceCounter();
	    if(GlobalSoundEnabled)
			if(!Mix_PlayingMusic())
			{
				Mix_PlayMusic(Music[SelectedMusic],0);
				SetVolume(Volume);
			}
		Input->Update();

        if(Input->SpecialsHeld(SPECIAL_QUIT_EV))
		{
            GameState = GSQuit;
			SubmitChanges=false;
			End = true;
		}
		
		if (Input->Ready() && ((Input->KeyboardHeld(SDLK_LALT) || Input->KeyboardHeld(SDLK_RALT)) && Input->KeyboardHeld(SDLK_RETURN)))
        {
            fullScreen = !fullScreen;
            SDL_SetWindowFullscreen(SdlWindow, fullScreen);
            Input->Delay();
            continue;
        }

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_LEFT)) || Input->KeyboardHeld(SDLK_LEFT)))
        {
            if (Selection > 0)
            {	Selection--;
                asci = ord(PackName[Selection]);
            }
            Input->Delay();
        }

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_RIGHT)) || Input->KeyboardHeld(SDLK_RIGHT)))
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

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_UP)) || Input->KeyboardHeld(SDLK_UP)))
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

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_DOWN)) || Input->KeyboardHeld(SDLK_DOWN)))
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

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) || Input->KeyboardHeld(SDLK_Q) || Input->KeyboardHeld(SDLK_A) || Input->KeyboardHeld(SDLK_RETURN)))
        {
            if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
            End = true;
            SubmitChanges=true;
        }

        if(Input->Ready() && (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_X)) || Input->KeyboardHeld(SDLK_X) || Input->KeyboardHeld(SDLK_Z) || Input->KeyboardHeld(SDLK_ESCAPE)))
        {
            End=true;
            SubmitChanges=false;
        }
		SDL_SetRenderTarget(Renderer, Buffer);
		SDL_RenderTexture(Renderer, IMGTitleScreen,NULL,NULL);	
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
		Rect2.x = 62.0f*UI_WIDTH_SCALE;
        Rect2.y = 82.0f*UI_HEIGHT_SCALE;
        Rect2.w = 196.0f*UI_WIDTH_SCALE;
        Rect2.h = 76.0f*UI_HEIGHT_SCALE;
		SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
		SDL_RenderRect(Renderer, &Rect2);
		WriteText(font,Msg,strlen(Msg),65*UI_WIDTH_SCALE,85*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		WriteText(MonoFont,PackName,strlen(PackName),85*UI_WIDTH_SCALE,110*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		
		if (Selection > 0)
		{
			strcpy(Tekst," ");
			for (Teller=1;Teller<Selection;Teller++)
				strcat(Tekst," ");
			strcat(Tekst,"_");
		}
		else
			strcpy(Tekst,"_");
		WriteText(MonoFont,Tekst,strlen(Tekst),85*UI_WIDTH_SCALE,112*UI_HEIGHT_SCALE,2,MenuTextColor,false);
		sprintf(Tekst,"Use Up,Down,Left,right. A = Ok X = Cancel" );
		WriteText(font,Tekst,strlen(Tekst),65*UI_WIDTH_SCALE,135*UI_HEIGHT_SCALE,2,MenuTextColor,false);
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
	sprintf(Filename,"%s/.blips_%s.dat", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName);
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
			HashBuffer[Teller] = SDL_rand(256);
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
	sprintf(Filename,"%s/.blips_%s.dat", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"), LevelPackFileName);
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

bool AskQuestion(const char *Msg)
{
	bool Result = false;
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
	Rect2.x = 62.0f*UI_WIDTH_SCALE;
	Rect2.y = 82.0f*UI_HEIGHT_SCALE;
	Rect2.w = 196.0f*UI_WIDTH_SCALE;
	Rect2.h = 76.0f*UI_HEIGHT_SCALE;
	SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
	SDL_RenderRect(Renderer, &Rect2);
	WriteText(font,Msg,strlen(Msg),65*UI_WIDTH_SCALE,83*UI_HEIGHT_SCALE,2,MenuTextColor,false);
	SDL_SetRenderTarget(Renderer, Buffer2);
	SDL_RenderTexture(Renderer, Buffer, NULL, NULL);
	SDL_SetRenderTarget(Renderer, NULL);
	SDL_SetRenderDrawColor(Renderer, 0,0,0,255);
	SDL_RenderClear(Renderer);
	SDL_SetRenderLogicalPresentation(Renderer, ORIG_WINDOW_WIDTH, ORIG_WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);        
	SDL_RenderTexture(Renderer, Buffer2, NULL, NULL);
	SDL_RenderPresent(Renderer);
	//reset 
	Input->SetInputDelay(InputDelay);
	Input->Reset();
	while (!(Input->KeyboardHeld(SDLK_Z) || Input->SpecialsHeld(SPECIAL_QUIT_EV) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) || Input->KeyboardHeld(SDLK_RETURN) || Input->KeyboardHeld(SDLK_SPACE) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_X)) || Input->KeyboardHeld(SDLK_A) || Input->KeyboardHeld(SDLK_Q) || Input->KeyboardHeld(SDLK_Y) || Input->KeyboardHeld(SDLK_ESCAPE) || Input->KeyboardHeld(SDLK_N) || Input->KeyboardHeld(SDLK_X)))
	{
		frameticks = SDL_GetPerformanceCounter();
		Input->Update();

		if (Input->Ready() && ((Input->KeyboardHeld(SDLK_LALT) || Input->KeyboardHeld(SDLK_RALT)) && Input->KeyboardHeld(SDLK_RETURN)))
        {
            fullScreen = !fullScreen;
            SDL_SetWindowFullscreen(SdlWindow, fullScreen);
            Input->Delay();
            continue;
        }

		if(GlobalSoundEnabled)
			if(!Mix_PlayingMusic())
			{
				Mix_PlayMusic(Music[SelectedMusic],0);
				SetVolume(Volume);
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
	if (Input->SpecialsHeld(SPECIAL_QUIT_EV))
		GameState = GSQuit;
	if (Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) || Input->KeyboardHeld(SDLK_RETURN) || Input->KeyboardHeld(SDLK_SPACE) || Input->KeyboardHeld(SDLK_A) || Input->KeyboardHeld(SDLK_Q) || Input->KeyboardHeld(SDLK_Y))
		Result = true;
	return Result;
}

void PrintForm(const char *msg)
{
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
	Rect2.x = 62.0f*UI_WIDTH_SCALE;
	Rect2.y = 82.0f*UI_HEIGHT_SCALE;
	Rect2.w = 196.0f*UI_WIDTH_SCALE;
	Rect2.h = 76.0f*UI_HEIGHT_SCALE;
	SDL_SetRenderDrawColor(Renderer, MenuBoxBorderColor.r,MenuBoxBorderColor.g,MenuBoxBorderColor.b,MenuBoxBorderColor.a);
	SDL_RenderRect(Renderer, &Rect2);
	WriteText(font,msg,strlen(msg),65*UI_WIDTH_SCALE,83*UI_HEIGHT_SCALE,2,MenuTextColor,false);
    SDL_SetRenderTarget(Renderer, Buffer2);
	SDL_RenderTexture(Renderer, Buffer, NULL, NULL);
	SDL_SetRenderTarget(Renderer, NULL);
	SDL_SetRenderDrawColor(Renderer, 0,0,0,255);
	SDL_RenderClear(Renderer);
	SDL_SetRenderLogicalPresentation(Renderer, ORIG_WINDOW_WIDTH, ORIG_WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);        
	SDL_RenderTexture(Renderer, Buffer2, NULL, NULL);
	SDL_RenderPresent(Renderer);
	//reset 
	Input->SetInputDelay(InputDelay);
	Input->Reset();
    while (!( Input->SpecialsHeld(SPECIAL_QUIT_EV) || Input->JoystickHeld(0, JoystickSetup->GetButtonValue(BUT_A)) || Input->KeyboardHeld(SDLK_ESCAPE) || Input->KeyboardHeld(SDLK_A) || Input->KeyboardHeld(SDLK_Q) || Input->KeyboardHeld(SDLK_RETURN) || Input->KeyboardHeld(SDLK_SPACE)))
    {
		frameticks = SDL_GetPerformanceCounter();
        Input->Update();

		if (Input->Ready() && ((Input->KeyboardHeld(SDLK_LALT) || Input->KeyboardHeld(SDLK_RALT)) && Input->KeyboardHeld(SDLK_RETURN)))
        {
            fullScreen = !fullScreen;
            SDL_SetWindowFullscreen(SdlWindow, fullScreen);
            Input->Delay();
            continue;
        }

        if(GlobalSoundEnabled)
			if(!Mix_PlayingMusic())
			{
				Mix_PlayMusic(Music[SelectedMusic],0);
				SetVolume(Volume);
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
	if(Input->SpecialsHeld(SPECIAL_QUIT_EV))
		GameState = GSQuit;
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
	sprintf(FileName,"%s/.blips_settings", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
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
	sprintf(FileName,"%s/.blips_settings", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
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
	char *Tmp;
	char FileName[FILENAME_MAX];
	if (GlobalSoundEnabled)
	{
		Tmp = assetPath("music/title.mod");
		Music[0] = Mix_LoadMUS(Tmp);
		SDL_free(Tmp);
	}
	Teller=1;
	char* musPath = assetPath("music");
	Directory = opendir(musPath);
	if (Directory)
	{
		Entry=readdir(Directory);
		while(Entry)
		{
			sprintf(FileName,"%s/%s",musPath, Entry->d_name);
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
	SDL_free(musPath);
}

void DoSearchForLevelPacks(const char* Path)
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
	char* TmpPath = assetPath("levelpacks");
	DoSearchForLevelPacks(TmpPath);
	SDL_free(TmpPath);
	char Path[FILENAME_MAX];
	sprintf(Path, "%s/.blips_levelpacks", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
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
        SDL_DestroyTexture(IMGGrid);
	}

	if(IMGWall)
	{
        SDL_DestroyTexture(IMGWall);
	}

	if(IMGBackground)
	{
		SDL_DestroyTexture(IMGBackground);
	}

	if(IMGFloor)
	{
		SDL_DestroyTexture(IMGFloor);
	}

	if(IMGPlayer)
	{
		SDL_DestroyTexture(IMGPlayer);
	}

	if(IMGPlayer2)
	{
		SDL_DestroyTexture(IMGPlayer2);
	}

	if(IMGBox)
	{
		SDL_DestroyTexture(IMGBox);
	}

	if(IMGEmpty)
	{
		SDL_DestroyTexture(IMGEmpty);
	}

    if(IMGDiamond)
	{
		SDL_DestroyTexture(IMGDiamond);
	}

	if(IMGBomb)
	{
		SDL_DestroyTexture(IMGBomb);
	}

	if(IMGTitleScreen)
	{
		SDL_DestroyTexture(IMGTitleScreen);
	}

    if(IMGIntro1)
	{
        SDL_DestroyTexture(IMGIntro1);
	}

    if(IMGIntro2)
	{
        SDL_DestroyTexture(IMGIntro2);
	}

    if(IMGIntro3)
	{
        SDL_DestroyTexture(IMGIntro3);
	}

    if(IMGExplosion)
	{
        SDL_DestroyTexture(IMGExplosion);
	}

	char *TmpPath = assetPath("graphics/grid.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGGrid = SDL_CreateTextureFromSurface(Renderer, Tmp);
	SDL_SetTextureBlendMode(IMGGrid, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(Tmp);

    TmpPath = assetPath("graphics/intro1.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
	IMGIntro1 = SDL_CreateTextureFromSurface(Renderer, Tmp);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/intro2.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGIntro2 = SDL_CreateTextureFromSurface(Renderer, Tmp);
    SDL_DestroySurface(Tmp);
 
	TmpPath = assetPath("graphics/intro3.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGIntro3 = SDL_CreateTextureFromSurface(Renderer, Tmp);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/floor.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGFloor = SDL_CreateTextureFromSurface(Renderer, Tmp);
	SDL_SetTextureBlendMode(IMGFloor, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/explosion.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGExplosion = SDL_CreateTextureFromSurface(Renderer, Tmp);
	SDL_SetTextureBlendMode(IMGExplosion, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/wall.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGWall = SDL_CreateTextureFromSurface(Renderer, Tmp);
	SDL_SetTextureBlendMode(IMGWall, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/bomb.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGBomb = SDL_CreateTextureFromSurface(Renderer, Tmp);
	SDL_SetTextureBlendMode(IMGBomb, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/diamond.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGDiamond = SDL_CreateTextureFromSurface(Renderer, Tmp);
	SDL_SetTextureBlendMode(IMGDiamond, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/box.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGBox = SDL_CreateTextureFromSurface(Renderer, Tmp);
	SDL_SetTextureBlendMode(IMGBox, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/player.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGPlayer = SDL_CreateTextureFromSurface(Renderer, Tmp);
	SDL_SetTextureBlendMode(IMGPlayer, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/player2.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGPlayer2 = SDL_CreateTextureFromSurface(Renderer, Tmp);
	SDL_SetTextureBlendMode(IMGPlayer, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/empty.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGEmpty = SDL_CreateTextureFromSurface(Renderer, Tmp);
	SDL_SetTextureBlendMode(IMGEmpty, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/titlescreen.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGTitleScreen = SDL_CreateTextureFromSurface(Renderer, Tmp);
    SDL_DestroySurface(Tmp);

	TmpPath = assetPath("graphics/background.png");
    Tmp = IMG_Load(TmpPath);
	SDL_free(TmpPath);
    IMGBackground = SDL_CreateTextureFromSurface(Renderer, Tmp);
    SDL_DestroySurface(Tmp);
}
