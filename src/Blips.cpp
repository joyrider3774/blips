
#if defined _WIN32 || defined __CYGWIN__
    #include <windows.h>
    #undef TRANSPARENT
#endif


#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "Common.h"
#include "intro.h"
#include "CWorldParts.h"
#include "GameFuncs.h"
#include "StageSelect.h"
#include "Credits.h"
#include "TitleScreen.h"
#include "SetupUsbJoysticks.h"
#include "LevelEditorMenu.h"
#include "Game.h"
#include "LevelEditor.h"

static void printHelp(char* exe)
{
    char* binaryName = SDL_strrchr(exe, '/');
    if (binaryName == NULL)
    {
        binaryName = SDL_strrchr(exe, '\\');
        if(binaryName == NULL)
            binaryName = exe;
    }
    if(binaryName)
        ++binaryName;

    printf("Blips Sdl 3 Version\n");
    printf("Usage: %s [-w <WIDTH>] [-h <HEIGHT>] [-f] [-ns] [-a] [-fps] [-nd]\n", binaryName);
    printf("\n");
    printf("Commands:\n");
    printf("  -w <WIDTH>: use <WIDTH> as window width\n");
    printf("  -h <HEIGHT>: use <HEIGHT> as window height\n");
    printf("  -f: Run fullscreen\n");
    printf("  -ns: No Sound\n");
    printf("  -s: Use Software rendering (default is hardware accelerated)\n");
    printf("  -fps: Show fps\n");
    printf("  -nd: no fps delay (run as fast as possible)\n");
}

int main(int argc, char **argv)
{
	SDL_srand((int) time(NULL));
	//attach to potential console when using -mwindows so we can get output in a cmd / msys prompt 
	//but see no console window when running from explorer start menu or so
	#if defined _WIN32 || defined __CYGWIN__
	if(AttachConsole((DWORD)-1))
	{
		freopen("CON", "w", stderr);
		freopen("CON", "w", stdout);
	}
	#endif
	fullScreen = false;
	bool useHWSurface = true;
	bool noAudioInit = false;
	for (int i = 0; i < argc; i++)
	{

		if((SDL_strcmp(argv[i], "-?") == 0) || (SDL_strcmp(argv[i], "--?") == 0) || 
			(SDL_strcmp(argv[i], "/?") == 0) || (SDL_strcmp(argv[i], "-help") == 0) || (SDL_strcmp(argv[i], "--help") == 0))
		{
			printHelp(argv[0]);
			return 0;
		}

		if(SDL_strcmp(argv[i], "-f") == 0)
			fullScreen = true;
		
		if(SDL_strcmp(argv[i], "-s") == 0)
			useHWSurface = false;
		
		if(SDL_strcmp(argv[i], "-fps") == 0)
			showfps = true;
		
		if(SDL_strcmp(argv[i], "-ns") == 0)
			noAudioInit = true;

		if(SDL_strcmp(argv[i], "-nd") == 0)
			nodelay = true;

		if(SDL_strcmp(argv[i], "-w") == 0)
			if(i+1 < argc)
				WINDOW_WIDTH = SDL_atoi(argv[i+1]);
		
		if(SDL_strcmp(argv[i], "-h") == 0)
			if(i+1 < argc)
				WINDOW_HEIGHT = SDL_atoi(argv[i+1]);
	}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO))
	{
		logMessage("SDL Succesfully initialized\n");

		memset(basePath, 0, FILENAME_MAX);
		const char* SDL_BasePath = SDL_GetBasePath();
		if(SDL_BasePath)
			snprintf(basePath, strlen(SDL_BasePath), "%s", SDL_BasePath);
		else
			snprintf(basePath, FILENAME_MAX, "./");

		logMessage("Using Base Path: %s\n", basePath);

		Uint32 WindowFlags = SDL_WINDOW_RESIZABLE;
		
		SdlWindow = SDL_CreateWindow("Blips (SDL3)", WINDOW_WIDTH, WINDOW_HEIGHT, WindowFlags);

		if (SdlWindow)
		{
			//        Screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT,0, flags);
  //      Icon = IMG_Load("./graphics/blips.png");
    //    SDL_WM_SetIcon(Icon,NULL);
      //  SDL_WM_SetCaption( "Blips - Created By Willems Davy - Original game by Bryant Brownell", NULL );

			if (fullScreen)
			{
				SDL_SetWindowFullscreen(SdlWindow, fullScreen);
			}
			//SDL_AddEventWatch(resizingEventWatcher, SdlWindow);
			logMessage("Succesfully Set %dx%d\n",WINDOW_WIDTH, WINDOW_HEIGHT);
			if (useHWSurface == 0)
				Renderer = SDL_CreateRenderer(SdlWindow, SDL_SOFTWARE_RENDERER);
			else
				Renderer = SDL_CreateRenderer(SdlWindow, NULL);
			
			if (Renderer)
			{
			
				logMessage("Using Renderer:%s\n", SDL_GetRendererName(Renderer));
				char RenderDriverNames[1000];
				memset(RenderDriverNames, 0, 1000);
				for (int i = 0; i < SDL_GetNumRenderDrivers(); i++)
				{
					if(i > 0)
						strcat(RenderDriverNames, ",");
					strcat(RenderDriverNames, SDL_GetRenderDriver(i));
				}
				logMessage("Available Renders: %s\n",RenderDriverNames);
				logMessage("Succesfully Created Buffer\n");
				GlobalSoundEnabled = false;
				if(!noAudioInit)
				{
					const char* audioName = SDL_GetCurrentAudioDriver();
					if(audioName)
						logMessage("Using Audio Driver:%s\n", audioName);
					char AudioDriverNames[1000];
					memset(AudioDriverNames, 0, 1000);
					for (int i = 0; i < SDL_GetNumAudioDrivers(); i++)
					{
						audioName = SDL_GetAudioDriver(i);
						if (audioName)
						{
							if(i > 0)
								strcat(AudioDriverNames, ",");
							strcat(AudioDriverNames, audioName);
						}
					}
					logMessage("Available Audio Drivers: %s\n", AudioDriverNames);
					SDL_AudioSpec spec;
					spec.channels = MIX_DEFAULT_CHANNELS;
					spec.format = SDL_AUDIO_S16;
					spec.freq = 22050;

					if (!Mix_OpenAudio(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec))
					{
						logMessage("Failed to initialise sound!\n");
						logMessage("%s\n", SDL_GetError());
					}
					else
					{
						GlobalSoundEnabled = true;
						logMessage("Audio Succesfully initialised!\n");
					}
				}
			

				Buffer = SDL_CreateTexture(Renderer,PIXELFORMAT, SDL_TEXTUREACCESS_TARGET, ORIG_WINDOW_WIDTH,ORIG_WINDOW_HEIGHT);
				Buffer2 = SDL_CreateTexture(Renderer,PIXELFORMAT, SDL_TEXTUREACCESS_TARGET, ORIG_WINDOW_WIDTH,ORIG_WINDOW_HEIGHT);
				if (Buffer && Buffer2)
				{
				
					SDL_SetTextureBlendMode(Buffer, SDL_BLENDMODE_BLEND);

					if (TTF_Init())
					{
						logMessage("Succesfully initialized TTF\n");
						char *TmpPath = assetPath("fonts/font.ttf");
						font = TTF_OpenFont(TmpPath,9*UI_HEIGHT_SCALE);
						SDL_free(TmpPath);
						TmpPath = assetPath("fonts/font2.ttf");
						BigFont = TTF_OpenFont(TmpPath,20*UI_HEIGHT_SCALE);
						SDL_free(TmpPath);
						TmpPath = assetPath("fonts/font1.ttf");
						MonoFont = TTF_OpenFont(TmpPath,17*UI_HEIGHT_SCALE);
						SDL_free(TmpPath);
						if (font && BigFont && MonoFont)
						{
							logMessage("Succesfully Loaded fonts\n");
							JoystickSetup = new CUsbJoystickSetup();
							LoadJoystickSettings();
							TTF_SetFontStyle(font,TTF_STYLE_NORMAL);
							SearchForLevelPacks();
							SearchForMusic();
							LoadSounds();
							FindLevels();
							LoadSettings();
							LoadGraphics();
							while (GameState != GSQuit)
							{
								switch(GameState)
								{
									case GSIntro :
										Intro();
										break;
									case GSTitleScreen :
										TitleScreen();
										break;
									case GSCredits :
										Credits();
										break;
									case GSGame :
										Game();
										break;
									case GSStageClear:
										break;
									case GSStageSelect:
										StageSelect();
										break;
									case GSLevelEditor:
										LevelEditor();
										break;
									case GSLevelEditorMenu:
										LevelEditorMode = true;
										LevelEditorMenu();
										break;
									case GSSetupUsbJoystickButtons:
										SetupUsbJoystickButtons();
										break;
									default:
										break;
								}
							}
							SaveSettings();
							UnLoadGraphics();
							UnloadSounds();
							UnloadMusic();
							TTF_CloseFont(font);
							TTF_CloseFont(BigFont);
							TTF_CloseFont(MonoFont);
							MonoFont=NULL;
							font=NULL;
							BigFont=NULL;
							char FileName[FILENAME_MAX];
							sprintf(FileName,"%s/.blips_joystick.def", SDL_getenv("HOME") == NULL ? ".": SDL_getenv("HOME"));
							JoystickSetup->SaveCurrentButtonValues(FileName);
							delete JoystickSetup;
						}
						else
						{
							logMessage("Failed to Load fonts\n");
						}
						TTF_Quit();
						if(GlobalSoundEnabled)
							Mix_CloseAudio();
					}
					else
					{
						logMessage("Failed to initialize TTF\n");
					}

	                SDL_DestroyTexture(Buffer);
					SDL_DestroyTexture(Buffer2);
				}
				else
				{
					logMessage("Failed to create Buffer\n");
				}
				SDL_DestroyRenderer(Renderer);			
			}
			else
               	logMessage("Failed To Create Renderer\n");
            //SDL_DestroyTexture(Icon);
			SDL_DestroyWindow(SdlWindow);
		}
		else
		{
			logMessage("Failed to create window %dx%d\n",WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		
		SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_AUDIO);
		SDL_Quit();
	}
	else
	{
		logMessage("Couldn't initialise SDL!: %s\n", SDL_GetError());
	}
	WorldParts.RemoveAll();
	return 0;
}
