#ifndef COMMON_H
#define COMMON_H

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Defines.h"
#include "CInput.h"
#include "CWorldParts.h"
#include "CUsbJoystickSetup.h"

#define PIXELFORMAT SDL_PIXELFORMAT_RGBA32
#define FPS_SAMPLES 5

enum GameStates {GSTitleScreen,GSCredits,GSInstructions,GSIntro,GSQuit,GSGame,GSStageClear,GSStageSelect,GSLevelEditor,GSLevelEditorMenu,GSTest,GSSetupUsbJoystickButtons};

extern bool GlobalSoundEnabled;
extern MIX_Audio *Sounds[NrOfSounds];
extern MIX_Track* SoundsTracks[NrOfTracks];
extern SDL_Texture *IMGBackground, *IMGFloor, *IMGPlayer, *IMGPlayer2, *IMGBox, *IMGEmpty, *IMGTitleScreen, *IMGInstructions, *IMGWall,
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
extern MIX_Audio *Music[MaxMusicFiles];
extern MIX_Track *MusicTrack;
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
extern CInput *Input;
extern MIX_Mixer *Mixer;
#endif