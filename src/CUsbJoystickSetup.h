#ifndef CUSBJOYSTICK_H
#define CUSBJOYSTICK_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>

#define MAXDEFINITIONS 50
#define DESCRIPTIONSIZE 1024

struct SJoystickButtonDefinition {
    char ButtonDescription[DESCRIPTIONSIZE];
    int DefaultButtonValue;
    int CurrentButtonValue;
};

class CUsbJoystickSetup {
    private:
       int PNrOfDefinitions;
       SJoystickButtonDefinition PJoystickButtons[MAXDEFINITIONS];
    public:
        bool AddDefinition(int Button,const char* Definition, int value, int defaultValue);
        int GetButtonValue(int Button);
        bool SaveCurrentButtonValues(const char *Filename);
        bool LoadCurrentButtonValues(const char *Filename);
        void SetButtonValue(int Button, int Value);
        void DrawCurrentSetup(TTF_Font* FontIn,int X, int Y, int XSpacing, int YSpacing ,int Selection, SDL_Color TextColor,SDL_Color SelectedTextColor);
        CUsbJoystickSetup();
        ~CUsbJoystickSetup();
        void ResetToDefaults();

};
#endif
