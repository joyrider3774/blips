#ifndef COMMON_H
#define COMMON_H

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Defines.h"
#include "CWorldParts.h"
#include "CUsbJoystickSetup.h"

#define PIXELFORMAT SDL_PIXELFORMAT_RGBA32
#define FPS_SAMPLES 5

enum GameStates {GSTitleScreen,GSCredits,GSIntro,GSQuit,GSGame,GSStageClear,GSStageSelect,GSLevelEditor,GSLevelEditorMenu,GSTest,GSSetupUsbJoystickButtons};

extern bool GlobalSoundEnabled;
extern Mix_Chunk *Sounds[NrOfSounds];

extern SDL_Texture *IMGBackground, *IMGFloor, *IMGPlayer, *IMGBox, *IMGEmpty, *IMGTitleScreen, *IMGWall,
            *IMGIntro1, *IMGIntro2, *IMGIntro3, *IMGBomb, *IMGDiamond, *IMGExplosion, *IMGGrid;

extern CUsbJoystickSetup *JoystickSetup;

extern unsigned char HashTable[1004];
extern SDL_Texture* Buffer, *Buffer2;
extern TTF_Font* font,* BigFont,*MonoFont;
extern GameStates GameState;
extern int MusicCount, SelectedMusic, InstalledLevelPacksCount, InstalledLevels, SelectedLevel, SelectedLevelPack, UnlockedLevels;
extern bool LevelEditorMode, LevelHasChanged, StageReload;
extern SDL_Color MenuBoxColor, MenuBoxBorderColor, MenuTextColor;
extern CWorldParts WorldParts;
extern Uint32 NextTime;
extern int Volume;
extern char LevelPackName[21];
extern char LevelPackFileName[21];
extern char InstalledLevelPacks[MaxLevelPacks][21];
extern Mix_Music *Music[MaxMusicFiles];
extern bool disableJoysticks;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern bool showfps;
extern bool nodelay;
extern SDL_Window *SdlWindow;
extern SDL_Renderer *Renderer;
extern Uint64 frameticks, lastfpstime;
extern double frameTime, avgfps;
extern Uint32 framecount;
extern double fpsSamples[FPS_SAMPLES];
extern int fpsAvgCount, skipCounter;
extern char basePath[FILENAME_MAX];
extern bool fullScreen;
#endif