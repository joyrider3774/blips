#ifndef COMMON_H
#define COMMON_H

#include <SDL.h>
#include <SDL_framerate.h>
#include "Defines.h"
#include "CWorldParts.h"
#include "CUsbJoystickSetup.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>

enum GameStates {GSTitleScreen,GSCredits,GSIntro,GSQuit,GSGame,GSStageClear,GSStageSelect,GSLevelEditor,GSLevelEditorMenu,GSTest,GSSetupUsbJoystickButtons};

extern bool GlobalSoundEnabled;
extern Mix_Chunk *Sounds[NrOfSounds];

extern SDL_Surface *IMGBackground, *IMGFloor, *IMGPlayer, *IMGBox, *IMGEmpty, *IMGTitleScreen, *IMGWall,
            *IMGIntro1, *IMGIntro2, *IMGIntro3, *IMGBomb, *IMGDiamond, *IMGExplosion, *IMGGrid;

extern CUsbJoystickSetup *JoystickSetup;

extern unsigned char HashTable[1004];
extern SDL_Surface* Screen,*Buffer,*Buffer1;
extern TTF_Font* font,* BigFont,*MonoFont;
extern SDL_Joystick *Joystick;
extern GameStates GameState;
extern int MusicCount, SelectedMusic, InstalledLevelPacksCount, InstalledLevels, SelectedLevel, SelectedLevelPack, UnlockedLevels;
extern bool LevelEditorMode, LevelHasChanged, StageReload;
extern SDL_Color MenuBoxColor, MenuBoxBorderColor, MenuTextColor;
extern CWorldParts WorldParts;
extern Uint32 NextTime;
extern int Volume, StartScreenX, StartScreenY;
extern char LevelPackName[21];
extern char LevelPackFileName[21];
extern char InstalledLevelPacks[MaxLevelPacks][21];
extern Mix_Music *Music[MaxMusicFiles];
extern FPSmanager Fpsman;
extern bool disableJoysticks;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
#endif