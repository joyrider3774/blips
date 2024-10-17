#include <SDL_gfxPrimitives.h>
#include "CUsbJoystickSetup.h"
#include "CInput.h"
#include "Common.h"
#include "GameFuncs.h"
#include <unistd.h>

CUsbJoystickSetup::CUsbJoystickSetup() {
    for (int Teller=0; Teller < MAXDEFINITIONS; Teller++) {
          PJoystickButtons[Teller].CurrentButtonValue = JOYSTICK_NONE;
          PJoystickButtons[Teller].DefaultButtonValue = JOYSTICK_NONE;
          memset(PJoystickButtons[Teller].ButtonDescription,0,DESCRIPTIONSIZE);
    }
    PNrOfDefinitions = 0;
}

CUsbJoystickSetup::~CUsbJoystickSetup() {
}

bool CUsbJoystickSetup::AddDefinition(int Button, char* Definition, int Value, int DefaultValue) {
    if (Button >=0 && Button < NROFBUTTONS)
    {
        memset( PJoystickButtons[Button].ButtonDescription,0,DESCRIPTIONSIZE);
        strncpy( PJoystickButtons[Button].ButtonDescription,Definition,DESCRIPTIONSIZE);
        PJoystickButtons[Button].CurrentButtonValue = Value;
        PJoystickButtons[Button].DefaultButtonValue = DefaultValue;
        PNrOfDefinitions++;
        return true;
    }
    return false;

}

int CUsbJoystickSetup::GetButtonValue(int Button) {
    if (Button >= 0 && Button <NROFBUTTONS)
        return PJoystickButtons[Button].CurrentButtonValue;
    return -1;
}

void CUsbJoystickSetup::DrawCurrentSetup(SDL_Surface *Surface,TTF_Font* FontIn,int X, int Y, int XSpacing,int YSpacing,int Selection,  SDL_Color TextColor,SDL_Color SelectedColor) {
    char ButtonText[100];
    char SelectedDescText[DESCRIPTIONSIZE+4];
    for (int Teller=0; Teller < NROFBUTTONS; Teller++)
    {
        if (strlen(PJoystickButtons[Teller].ButtonDescription) > 0)
        {
            WriteText(Surface,FontIn,PJoystickButtons[Teller].ButtonDescription,strlen(PJoystickButtons[Teller].ButtonDescription),X ,Y + (YSpacing*Teller),0,TextColor,false);
            if(PJoystickButtons[Teller].CurrentButtonValue >= -1 && PJoystickButtons[Teller].CurrentButtonValue < MAXJOYSTICKBUTTONS)

                switch (PJoystickButtons[Teller].CurrentButtonValue) {
                    case JOYSTICK_NONE : sprintf(ButtonText,"Button: None"); break;
                    case JOYSTICK_LEFT : sprintf(ButtonText,"Button: Left"); break;
                    case JOYSTICK_UP : sprintf(ButtonText,"Button: Up"); break;
                    case JOYSTICK_RIGHT : sprintf(ButtonText,"Button: Right"); break;
                    case JOYSTICK_DOWN : sprintf(ButtonText,"Button: Down"); break;
                    default: sprintf(ButtonText,"Button: %d",PJoystickButtons[Teller].CurrentButtonValue+1); break;
                }

            if(Selection == Teller)
            {
                sprintf(SelectedDescText,"%s <<",ButtonText);
                WriteText(Surface,FontIn,SelectedDescText,strlen(SelectedDescText),X + XSpacing ,Y + (YSpacing*Teller),YSpacing,SelectedColor,false);
            }
            else
                WriteText(Surface,FontIn,ButtonText,strlen(ButtonText),X + XSpacing ,Y + (YSpacing*Teller),0,TextColor,false);
        }
    }
}

void CUsbJoystickSetup::SetButtonValue(int Button, int Value) {
    int Teller;
    if(Button >= 0 && Button < NROFBUTTONS)
    {
        for(Teller=0;Teller<NROFBUTTONS;Teller++)
            if(PJoystickButtons[Teller].CurrentButtonValue == Value)
                PJoystickButtons[Teller].CurrentButtonValue = JOYSTICK_NONE;
        PJoystickButtons[Button].CurrentButtonValue = Value;
    }
}

bool CUsbJoystickSetup::SaveCurrentButtonValues(const char *Filename) {
    FILE *f;
    int Teller;
    f = fopen(Filename,"wb");
    if(f)
    {
        for(Teller = 0;Teller < NROFBUTTONS;Teller++)
        {
            fprintf(f,"%d\n",PJoystickButtons[Teller].CurrentButtonValue);
        }
        fclose(f);
        return true;
    }
    return false;
}

bool CUsbJoystickSetup::LoadCurrentButtonValues(const char *Filename) {
    FILE *f;
    int Teller;
    f = fopen(Filename,"rt");
    if(f)
    {
        for(Teller = 0;Teller < NROFBUTTONS;Teller++)
        {
            fscanf(f,"%d\n",&PJoystickButtons[Teller].CurrentButtonValue);
        }
        fclose(f);
        return true;
    }
    return false;
}

void CUsbJoystickSetup::ResetToDefaults() {
    for (int Teller=0; Teller < NROFBUTTONS; Teller++)
        PJoystickButtons[Teller].CurrentButtonValue = PJoystickButtons[Teller].DefaultButtonValue;
}
