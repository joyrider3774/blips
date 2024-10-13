
#include <time.h>
#include <SDL_keysym.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Common.h"
#include "intro.h"
#include "CWorldParts.h"
#include "GameFuncs.h"
#include "StageSelect.h"
#include "Credits.h"
#include "TitleScreen.h"
#include "Game.h"
#include "stdbool.h"

int main(int argc, char **argv)
{
    SDL_Surface *Icon;
	bool fullscreen = false;
	disableJoysticks = false;
	for (int i=0; i < argc; i++)
	{
		if(strcasecmp(argv[i], "-f") == 0)
			fullscreen = true;
		if(strcasecmp(argv[i], "-d") == 0)
			disableJoysticks = true;
		if(strcasecmp(argv[i], "-s2") == 0)
		{
			WINDOW_WIDTH = 640 * 2;
			WINDOW_HEIGHT = 360 * 2;
		}

		if(strcasecmp(argv[i], "-s3") == 0)
		{
			WINDOW_WIDTH = 640 * 3;
			WINDOW_HEIGHT = 360 * 3;
		}

		if(strcasecmp(argv[i], "-s4") == 0)
		{
			WINDOW_WIDTH = 640 * 4;
			WINDOW_HEIGHT = 360 * 4;
		}

		if(strcasecmp(argv[i], "-s5") == 0)
		{
			WINDOW_WIDTH = 640 * 5;
			WINDOW_HEIGHT = 360 * 5;
		}
	}

	srand((int) time(NULL));
	if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == 0)
	{
		printf("SDL Succesfully initialized\n");
		Uint32 flags = SDL_SWSURFACE;
		if (fullscreen)
			flags |= SDL_FULLSCREEN;
        Screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT,SCREEN_BPP, flags);
        Icon = IMG_Load("./graphics/blips.png");
        SDL_WM_SetIcon(Icon,NULL);
        SDL_WM_SetCaption( "Blips - Created By Willems Davy - Original game by Bryant Brownell", NULL );

		if(Screen)
		{
		    printf("Succesfully Set %dx%dx%d\n",WINDOW_WIDTH,WINDOW_HEIGHT,SCREEN_BPP);
			if(fullscreen)
			{
				printf("Running FullScreen\n");
			}
		    Buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,ORIG_WINDOW_WIDTH,ORIG_WINDOW_HEIGHT,SCREEN_BPP,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
            Buffer1 = SDL_CreateRGBSurface(SDL_SWSURFACE,ORIG_WINDOW_WIDTH,ORIG_WINDOW_HEIGHT,SCREEN_BPP,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
			if (Buffer && Buffer1)
			{
			    SDL_FillRect(Buffer,NULL,SDL_MapRGB(Buffer->format,0,0,0));
                SDL_ShowCursor(SDL_DISABLE);
                if (Mix_OpenAudio(22050,AUDIO_S16,MIX_DEFAULT_CHANNELS,1024) < 0)
				{
					GlobalSoundEnabled = false;
					printf("Failed to initialise sound!\n");
					printf("%s\n",Mix_GetError());
				}
				else
				{
					printf("Audio Succesfully initialised!\n");
				}
				if (TTF_Init() == 0)
				{
					printf("Succesfully initialized TTF\n");
					font = TTF_OpenFont("./fonts/font.ttf",9*UI_HEIGHT_SCALE);
					BigFont = TTF_OpenFont("./fonts/font2.ttf",20*UI_HEIGHT_SCALE);
					MonoFont = TTF_OpenFont("./fonts/font1.ttf",17*UI_HEIGHT_SCALE);
					if (font && BigFont && MonoFont)
					{
						WorldParts = CWorldParts_Create();
						printf("Succesfully Loaded fonts\n");
						SDL_initFramerate(&Fpsman);
						SDL_setFramerate(&Fpsman,FPS);
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
						CWorldParts_Destroy(WorldParts);
					}
					else
					{
						printf("Failed to Load fonts\n");
					}
					TTF_Quit();
				}
				else
				{
					printf("Failed to initialize TTF\n");
				}



                SDL_FreeSurface(Buffer);
                Buffer=NULL;
                SDL_FreeSurface(Buffer1);
                Buffer1=NULL;
			}
			else
                printf("Failed To Create Buffer\n");
            SDL_FreeSurface(Screen);
            Screen=NULL;
            SDL_FreeSurface(Icon);
		}
		else
		{
			printf("Failed to Set Videomode %dx%dx%d\n",WINDOW_WIDTH, WINDOW_HEIGHT,SCREEN_BPP);
		}
		SDL_Quit();
	}
	else
	{
		printf("Couldn't initialise SDL!: %s\n", SDL_GetError());
	}	
	return 0;

}
