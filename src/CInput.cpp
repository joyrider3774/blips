#include "CInput.h"
#include "GameFuncs.h"
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_joystick.h>

// All joysticks are always opened and they all assign all the same values of joystick nr 0 in the array
// this game was initially made for a system that had fixed amount of joysticks

CInput::CInput(int UpdateCounterDelay, bool DisableJoysticks) {
    Reset();
    OpenJoystickCount = 0;

	if(!DisableJoysticks)
	{
        SDL_JoystickID* JoystickIDs = SDL_GetJoysticks(&PNumJoysticks);
        if (JoystickIDs == NULL)
            logMessage("SDL_GetJoysticks Failed %s\n", SDL_GetError());
        else
            logMessage("Found %d Joysticks\n", PNumJoysticks);

    	for (int teller=0;teller< PNumJoysticks;teller++)
        {
            Joysticks[OpenJoystickCount] = SDL_OpenJoystick(JoystickIDs[teller]);
            if(Joysticks[OpenJoystickCount])
            {
                logMessage("Opened Joystick %d %s\n", OpenJoystickCount+1, SDL_GetJoystickName(Joysticks[OpenJoystickCount]));
                SDL_SetJoystickEventsEnabled(true);
                OpenJoystickCount++;
            }
            else
                logMessage("Failed opening joystick %d: %s\n", OpenJoystickCount+1, SDL_GetError());
        }
	}
    PUpdateCounterDelay = UpdateCounterDelay;
    UpdateCounter = 0;
}

int CInput::GetJoystickNr(SDL_JoystickID ID)
{
    for (int i = 0; i < OpenJoystickCount; i++)
        if(SDL_GetJoystickID(Joysticks[i]) == ID)
        {
            return i;
            break;
        }
    return -1;
}

CInput::~CInput() {
    for(int i = 0; i < OpenJoystickCount; i++)
    {
        logMessage("Closing joystick %d: %s\n", 1, SDL_GetJoystickName(Joysticks[i]));
        SDL_CloseJoystick(Joysticks[i]);
    }
}

void CInput::Update() {
    SDL_Event Event;
    if (UpdateCounter > 0)
        UpdateCounter--;
    while(SDL_PollEvent(&Event))
    {
        if(Event.type == SDL_EVENT_QUIT)
            _SpecialsHeld[SPECIAL_QUIT_EV] = true;
        if(Event.type == SDL_EVENT_KEY_DOWN)
            _KeyboardHeld[Event.key.scancode] = true;
        if(Event.type == SDL_EVENT_KEY_UP)
            _KeyboardHeld[Event.key.scancode] = false;
		if (Event.type == SDL_EVENT_JOYSTICK_HAT_MOTION)
		{
			switch (Event.jhat.value)
			{
				case 1: 
					_JoystickHeld[0][JOYSTICK_UP] = true;
					break;
				case 2: 
					_JoystickHeld[0][JOYSTICK_RIGHT] = true;
					break;
				case 4: 
					_JoystickHeld[0][JOYSTICK_DOWN] = true;
					break;
				case 8: 
					_JoystickHeld[0][JOYSTICK_LEFT] = true;
					break;
				default:
					_JoystickHeld[0][JOYSTICK_UP] = false;
					_JoystickHeld[0][JOYSTICK_RIGHT] = false;
					_JoystickHeld[0][JOYSTICK_DOWN] = false;
					_JoystickHeld[0][JOYSTICK_LEFT] = false;
					break;
			
			}
		}

        if(Event.type == SDL_EVENT_JOYSTICK_AXIS_MOTION)
		{
            if(Event.jaxis.axis == 0)
            {
                if(Event.jaxis.value > JOYSTICKDEADZONE)
                {
                    _JoystickHeld[0][JOYSTICK_RIGHT] = true;
                }
                else
                    if(Event.jaxis.value < -JOYSTICKDEADZONE)
                    {
                        _JoystickHeld[0][JOYSTICK_LEFT] = true;
                    }
                    else
                    {
                        _JoystickHeld[0][JOYSTICK_LEFT] = false;
                        _JoystickHeld[0][JOYSTICK_RIGHT] = false;
                    }
            }
            else
			{
                if(Event.jaxis.axis == 1)
                {
                    if(Event.jaxis.value > JOYSTICKDEADZONE)
                    {
                        _JoystickHeld[0][JOYSTICK_DOWN] = true;
                    }
                    else
                        if(Event.jaxis.value < -JOYSTICKDEADZONE)
                        {
                            _JoystickHeld[0][JOYSTICK_UP] = true;
                        }
                        else
                        {
                            _JoystickHeld[0][JOYSTICK_DOWN] = false;
                            _JoystickHeld[0][JOYSTICK_UP] = false;
                        }
                }
			}
		}

        if(Event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN)
		{
            if(Event.jbutton.button < MAXJOYSTICKBUTTONS)
            {
                _JoystickHeld[0][Event.jbutton.button] = true;
            }
		}

        if(Event.type == SDL_EVENT_JOYSTICK_BUTTON_UP)
        {
            if(Event.jbutton.button < MAXJOYSTICKBUTTONS)
            {
                _JoystickHeld[0][Event.jbutton.button] = false;
            }
        }

        if(Event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            if(Event.button.button < MAXMOUSEBUTTONS)
                _MouseHeld[0][Event.button.button] = true;

        if(Event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            if(Event.button.button < MAXMOUSEBUTTONS)
                _MouseHeld[0][Event.button.button] = false;

    }
}

void CInput::Reset() {
    int x,y;
    for (x=0;x<MAXJOYSTICKS;x++)
        for (y=0;y<MAXJOYSTICKBUTTONS;y++)
            _JoystickHeld[x][y] = false;
    for (x=0;x<MAXMOUSES;x++)
        for (y=0;y<MAXMOUSEBUTTONS;y++)
            _MouseHeld[x][y] = false;
    for (x=0;x<SDL_SCANCODE_COUNT;x++)
        _KeyboardHeld[x] = false;
    for (x=0;x<MAXSPECIALKEYS;x++)
        _SpecialsHeld[x] = false;
}

