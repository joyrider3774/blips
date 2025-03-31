#ifndef GAMEFUNCS_H
#define GAMEFUNCS_H

#include <SDL.h>
#include <SDL_ttf.h>

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
bool FileExists(const char * FileName);
void WriteText(SDL_Surface* Surface,TTF_Font* FontIn,const char* Tekst,int NrOfChars,int X,int Y,int YSpacing,SDL_Color ColorIn,bool Centered);
char *GetString(const char *NameIn,const char *Msg);
void LoadUnlockData();
void SaveUnlockData();
void PrintForm(const char *msg);
bool AskQuestion(const char *Msg);
void SearchForLevelPacks();
void SearchForMusic();
void SaveSettings();
void LoadSettings();
void DecVolume();
void IncVolume();
void SetVolume(const int VolumeIn);
void LoadGraphics();

#endif