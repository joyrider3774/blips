#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "CUsbJoystickSetup.h"
#include "CInput.h"
#include "SPoint.h"
#include "Defines.h"
#include "Common.h"

bool GlobalSoundEnabled = true;
Mix_Chunk *Sounds[NrOfSounds];

SDL_Texture *IMGBackground=NULL,*IMGFloor=NULL,*IMGPlayer=NULL,*IMGBox=NULL,*IMGEmpty=NULL,*IMGTitleScreen=NULL,*IMGWall=NULL,
            *IMGIntro1=NULL,*IMGIntro2=NULL,*IMGIntro3=NULL,*IMGBomb=NULL,*IMGDiamond=NULL,*IMGExplosion=NULL,*IMGGrid=NULL;

CUsbJoystickSetup *JoystickSetup;
bool showfps = false;
unsigned char HashTable[1004] = {0xE4,0xF6,0xD7,0xA4,0xA5,0xC3,0xA3,0xE5,0xA4,0xF8,0xA3,0xF6,0xB5,0x98,0xA3,0xA7,0xB9,0xA0,0xD9,0xD6,0xE6,0x29,0x49,0xF8,0xB9,0xF0,0x79,0xA6,0x88,0xA8,0xC6,0xC2,0xF2,0xB7,0x79,0xC0,0xF0,0xD0,0xA8,0xE1,0xE1,0xF9,0xA5,0xE5,0xF1,0xE4,0xC7,0xE8,0xD9,0xB7,0xC0,0xE1,0xC0,0xB4,0xA9,0xF8,0xF8,0xF8,0xA1,0xA4,0xD0,0xC6,0x06,0xB9,0xA1,0xB0,0xF0,0xE8,0xB0,0xE5,0xC3,0xC8,0xA1,0xB2,0xF1,0xE2,0xE0,0xF0,0xF8,0xA5,0xE6,0xF2,0xB4,0xF7,0x79,0xB5,0x75,0xB0,0xF1,0xA6,0xC9,0xE2,0xB6,0x68,0xD9,0xA0,0xE7,0xD1,0xB4,0x51,0xD9,0xE9,0x96,0x61,0xD2,0xC2,0xC2,0x06,0x47,0x86,0x88,0xF4,0xC7,0x88,0xA0,0xA9,0xB1,0xD2,0xE9,0xF1,0x93,0xF4,0x35,0xC6,0xB2,0xC8,0xB8,0xB0,0xA8,0xE9,0xC1,0xA4,0xF5,0xD6,0xD1,0xC8,0xD3,0xA9,0xE9,0xA9,0xF4,0xB4,0xA3,0x57,0xE1,0xC5,0xF1,0xB7,0xB6,0xB2,0xE3,0x45,0x64,0xE3,0xD9,0xE5,0xB5,0xE2,0xE8,0xC8,0xB5,0xE7,0xD3,0xB0,0xD2,0xF7,0x81,0xB2,0xE4,0xA8,0x00,0xC9,0xD5,0xD4,0x86,0xC9,0xB1,0xA2,0xA0,0xD3,0xE3,0xD5,0xF7,0xE0,0xF9,0xE5,0xE2,0xD1,0xA5,0xD4,0xF4,0xC6,0xB4,0xD3,0xA2,0xD6,0xD3,0x35,0xE4,0xE3,0xC4,0xB6,0x18,0xD7,0xE7,0xC5,0xE2,0x59,0xA0,0xB5,0xA7,0xC5,0xF6,0xA7,0xC2,0xD7,0xA2,0xB5,0xB9,0xF6,0x81,0xA3,0xA5,0x00,0xA2,0xA8,0xE7,0xD5,0x87,0xC8,0xE9,0xD7,0xE6,0xA0,0x54,0xF9,0xF9,0xF4,0xB4,0xF4,0xE9,0xA7,0xD0,0x74,0xD0,0xE7,0x24,0xD9,0xB0,0xE8,0xE3,0xF1,0xF7,0xF7,0x88,0x97,0xC0,0xE0,0xD2,0xF3,0xC2,0xA1,0xC4,0xE1,0xF8,0xE8,0xB1,0x76,0x97,0xE7,0xB6,0x96,0xA7,0xD8,0xA4,0xF1,0xA0,0xF3,0xE8,0xE7,0x76,0xE3,0xB1,0xC9,0xE4,0xB4,0xF0,0xA4,0xD0,0x28,0xB8,0xF7,0xD9,0xD3,0xE5,0xC9,0x81,0xF7,0xC0,0xD9,0xC9,0xE3,0xA8,0xD6,0xB9,0xA7,0xF9,0xD0,0xD8,0x54,0xF6,0xF9,0xB8,0xC8,0xC9,0xB9,0xF0,0xF4,0xD1,0xA4,0xD0,0xE6,0xA8,0xB6,0xD2,0xB0,0xE9,0xE9,0xE9,0xB7,0xD9,0xD4,0xE6,0xF0,0xA4,0xC7,0xF1,0xC0,0xC2,0xA8,0xA7,0x14,0x25,0xA1,0xB6,0xB9,0xD5,0x78,0xC1,0xD1,0xC6,0x93,0xE8,0xE7,0xE7,0xA9,0xA2,0xA8,0x89,0xA1,0xA6,0xF9,0xA8,0xF7,0xE4,0xD8,0xA2,0xD2,0xB0,0x39,0xD1,0xC5,0x72,0xB0,0xD6,0xC6,0xA0,0xE9,0xE5,0xC1,0xC2,0xF9,0xF3,0xB2,0xC8,0xD0,0xD2,0xD1,0xC1,0xD7,0x53,0x93,0xB4,0xF9,0xB0,0xD0,0xD4,0xF5,0xE4,0xE4,0x13,0xB2,0xE9,0x70,0xB3,0xB2,0xD8,0xF0,0xC0,0xC1,0xB0,0xE8,0xA3,0xE7,0x33,0xB1,0xC0,0xA7,0x17,0xD4,0xA9,0xE5,0xF6,0xF3,0xF3,0xA3,0xE0,0xD5,0xE2,0x71,0xB7,0xB4,0xA9,0xD6,0xC1,0xC5,0xA3,0xB2,0xD7,0xB6,0xE8,0xC5,0xF0,0x10,0xA6,0xB0,0xC3,0xA3,0xA0,0xB2,0xF6,0xB9,0xF4,0xD8,0xD5,0xF8,0xB9,0xE3,0x70,0xD1,0xF1,0xA6,0xE6,0xE5,0xA8,0xD5,0xC9,0xC1,0xA5,0xE5,0x61,0xF4,0xB2,0xA9,0xA4,0xF2,0xE8,0xB7,0xD7,0xA1,0xD0,0xF4,0xF9,0xE4,0xE8,0xC6,0xB3,0xC4,0xB2,0xC9,0xE3,0x49,0xC3,0x62,0xA6,0xF3,0xB0,0x57,0x65,0xC4,0xF6,0xB9,0x23,0xB8,0xB3,0xA9,0xE6,0xB7,0xA5,0xE8,0xF6,0xC9,0xE7,0x63,0xF7,0xD9,0xA5,0xA5,0xD2,0xD4,0xA6,0xF2,0x94,0x04,0xB6,0x85,0xA3,0xD3,0x59,0xA3,0xA8,0x69,0xF1,0xB9,0xD1,0xD1,0xE5,0xE5,0x57,0xB0,0xD5,0x59,0xD4,0xD9,0x52,0xB4,0xA8,0x75,0xF0,0xE0,0x20,0xA1,0xE7,0x69,0xA2,0xC0,0xB1,0xA3,0xE5,0xB9,0xF5,0xB9,0xB5,0xE1,0xF3,0xB7,0x56,0xA1,0xA4,0xA9,0xA4,0xE7,0xC5,0xC7,0xC7,0x32,0xB7,0xA6,0xB4,0xB2,0xF6,0xB1,0xC4,0xB5,0xC4,0xC1,0xE1,0x97,0xB1,0xE3,0x37,0xA6,0xD8,0xC3,0xF1,0xD5,0xE9,0xF2,0xE2,0xF6,0xF3,0xF6,0xF4,0x51,0xE7,0xE9,0xC1,0xE8,0xA8,0x69,0xD3,0xB4,0xD8,0xD6,0xE8,0xD8,0x47,0xE1,0xA8,0xA0,0x94,0xB5,0xC2,0xD3,0xF6,0x28,0xF5,0x21,0xD7,0x48,0xC4,0xD8,0xF2,0xC7,0xC0,0xA7,0xF6,0xC7,0xE7,0x64,0xB5,0xD5,0x97,0xA0,0xD6,0xF7,0xC4,0xF7,0xD0,0xD7,0xA8,0xD7,0x28,0xC2,0xE5,0xD0,0xC2,0xF2,0xC8,0xF9,0xE5,0x08,0xD3,0xA0,0xA0,0xB2,0xD3,0x29,0xF0,0xB1,0xD0,0xB2,0xD7,0xF1,0x47,0xF2,0xD4,0xD3,0x53,0x92,0xC4,0xF9,0xA2,0xE8,0xC3,0xA9,0xF8,0xB1,0xE4,0x90,0xD0,0xE4,0x26,0x18,0xB4,0xF8,0xE7,0xC7,0xE0,0xF9,0xC1,0x36,0xE3,0xC2,0xE6,0xC9,0xC2,0xF8,0xA1,0x26,0xE7,0xC1,0x57,0xE9,0xD1,0xC8,0xC3,0xB2,0xE1,0x86,0xA5,0xE8,0xB4,0xA3,0xB5,0xD7,0x89,0xD5,0xF7,0xD0,0xD6,0xD7,0xB4,0x41,0xE1,0xA2,0xD5,0xF1,0xA8,0xF5,0xF7,0xC8,0xA3,0xB1,0xC0,0xC7,0x83,0x65,0xE6,0x46,0xA8,0xA3,0xE8,0xC5,0xF9,0xC2,0xD6,0xA5,0xD5,0xA5,0xA8,0xB6,0xE0,0xD0,0xC4,0xE1,0xF0,0x08,0x06,0x45,0xA7,0xF1,0xA1,0xE8,0xF3,0xF1,0xE2,0xC4,0xA6,0xF0,0xF3,0xD3,0xB2,0xE5,0xC5,0x85,0xA6,0xC7,0xD5,0xD4,0xD7,0xC2,0xC8,0xC8,0xD2,0xB8,0xA4,0x79,0x06,0xF7,0xE0,0xB6,0xF4,0xF4,0xC2,0xD1,0xB4,0xA4,0xB5,0xA8,0xC0,0xB2,0xB9,0xF7,0xC0,0xD6,0xD6,0xC3,0xF5,0xA3,0xF5,0xA7,0xE5,0xE1,0xB0,0xA5,0xE0,0xC3,0x41,0xD3,0xB2,0xF9,0xF2,0x19,0xB3,0xA7,0x58,0x40,0xB1,0x86,0xD5,0x71,0xF8,0xF6,0xD2,0xF2,0xA5,0xF4,0xB7,0x80,0xE5,0x03,0xF8,0x14,0xF5,0xE4,0xE7,0xE2,0xB9,0xA6,0xB5,0xE2,0xB5,0xD8,0xC7,0xE5,0xC5,0xF9,0xD4,0xD3,0xE5,0xD9,0xE8,0x79,0xD2,0xE3,0xF0,0xA1,0xF8,0xD8,0x31,0x78,0xB9,0xA9,0x41,0xC1,0xB1,0xC6,0x52,0xC5,0xA7,0xD0,0xF9,0xF5,0xA8,0xF6,0xA3,0xA3,0xD0,0xA5,0xF3,0xB1,0xB1,0x31,0xE0,0xC5,0xA7,0x29,0xC0,0xE1,0xD4,0xF7,0x15,0xC2,0xF3,0xA1,0xC9,0xF3,0xE0,0xA5,0xB6,0xE6,0xE5,0xB1,0xA3,0xB3,0xE7,0xF5,0xD8,0x93,0xF8,0xB2,0x94,0xC8,0xD2,0xE2,0xB0,0xF9,0xF1,0xB4,0xE0,0xD5,0x65,0x81,0xC4,0xC7,0xD0,0xE8,0xA7,0xA2,0xF7,0x45,0xA0,0xE9,0xC5,0xB6,0xE2,0xF2,0xE6,0xA7,0xE8,0x28,0xA6,0xE5,0x90,0xF0,0xD6,0xE1,0xC4,0xD2,0xD5,0xF0,0xA5,0xB1,0xC6,0xA0,0xC1,0xF5,0xB1,0xA4,0xD3,0xF3,0xA3,0xF3,0x10,0xE8,0xA9,0xC1,0xB2,0xB8,0xA5,0xD4,0x41,0xB4,0xC6,0xF7,0xC6,0xF9,0xD9,0xD0,0xC2,0xE9,0xD5,0xC7};
SDL_Texture* Buffer, *Buffer2;
TTF_Font* font,* BigFont,*MonoFont;
GameStates GameState = GSIntro;
int MusicCount=0,SelectedMusic=0,InstalledLevelPacksCount=0,InstalledLevels=0,SelectedLevel=0,SelectedLevelPack=0,UnlockedLevels=1;
bool LevelEditorMode=false,LevelHasChanged=false,StageReload=false;
SDL_Color MenuBoxColor = {75,138,158,255}, MenuBoxBorderColor = {2,66,87,255}, MenuTextColor = {2,66,87,0};
CWorldParts WorldParts;
Uint32 NextTime=0;
int Volume = 128;
char LevelPackName[21]= "";
char LevelPackFileName[21] = "";
char InstalledLevelPacks[MaxLevelPacks][21];
Mix_Music *Music[MaxMusicFiles];
bool disableJoysticks = false;
bool nodelay = false;
int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 360;
SDL_Window *SdlWindow = NULL;
SDL_Renderer *Renderer = NULL;
Uint64 frameticks = 0, lastfpstime = 0;
double frameTime = 0, avgfps = 0;
Uint32 framecount = 0;
double fpsSamples[FPS_SAMPLES];
int fpsAvgCount = 0, skipCounter = 10;
char basePath[FILENAME_MAX];
bool fullScreen = false;
CInput *Input;
