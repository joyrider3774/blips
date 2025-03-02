#ifndef GAMEFUNCS_H
#define GAMEFUNCS_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

void LoadJoystickSettings();
void UnloadSounds();
void LoadSounds();
void UnloadMusic();
void UnLoadGraphics();
void FindLevels();
void RemoveUnderScores (char *string);
void AddUnderScores (char *string);
int ord(char chr);
char chr(int ascii);
bool FileExists(char * FileName);
void WriteText(TTF_Font* FontIn,char* Tekst,int NrOfChars,int X,int Y,int YSpacing,SDL_Color ColorIn,bool Centered);
char *GetString(char *NameIn,char *Msg);
void LoadUnlockData();
void SaveUnlockData();
void PrintForm(char *msg);
bool AskQuestion(char *Msg);
void SearchForLevelPacks();
void SearchForMusic();
void SaveSettings();
void LoadSettings();
void DecVolume();
void IncVolume();
void SetVolume(const int VolumeIn);
void LoadGraphics();
void logMessage(SDL_PRINTF_FORMAT_STRING const char *fmt, ...);
#endif