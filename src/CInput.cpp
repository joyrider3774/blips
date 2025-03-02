#include "CInput.h"
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_joystick.h>

CInput::CInput(int UpdateCounterDelay, bool DisableJoysticks) {
    Reset();
    SDL_GetJoysticks(&PNumJoysticks);
	if(!DisableJoysticks)
	{
    	for (int teller=0;teller< PNumJoysticks;teller++)
        	if(SDL_OpenJoystick(teller) != NULL)
                SDL_SetJoystickEventsEnabled(true);
	}
    PUpdateCounterDelay = UpdateCounterDelay;
    UpdateCounter = 0;
}

CInput::~CInput() {
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
					_JoystickHeld[Event.jhat.which][JOYSTICK_UP] = true;
					break;
				case 2: 
					_JoystickHeld[Event.jhat.which][JOYSTICK_RIGHT] = true;
					break;
				case 4: 
					_JoystickHeld[Event.jhat.which][JOYSTICK_DOWN] = true;
					break;
				case 8: 
					_JoystickHeld[Event.jhat.which][JOYSTICK_LEFT] = true;
					break;
				default:
					_JoystickHeld[Event.jhat.which][JOYSTICK_UP] = false;
					_JoystickHeld[Event.jhat.which][JOYSTICK_RIGHT] = false;
					_JoystickHeld[Event.jhat.which][JOYSTICK_DOWN] = false;
					_JoystickHeld[Event.jhat.which][JOYSTICK_LEFT] = false;
					break;
			
			}
		}

        if(Event.type == SDL_EVENT_JOYSTICK_AXIS_MOTION)
		{
            if(Event.jaxis.axis == 0)
            {
                if(Event.jaxis.value > JOYSTICKDEADZONE)
                {
                    if(Event.jaxis.which < MAXJOYSTICKS)
                        _JoystickHeld[Event.jaxis.which][JOYSTICK_RIGHT] = true;
                }
                else
                    if(Event.jaxis.value < -JOYSTICKDEADZONE)
                    {
                        if(Event.jaxis.which < MAXJOYSTICKS)
                            _JoystickHeld[Event.jaxis.which][JOYSTICK_LEFT] = true;
                    }
                    else
                    {
                        if(Event.jaxis.which < MAXJOYSTICKS)
                        {

                            _JoystickHeld[Event.jaxis.which][JOYSTICK_LEFT] = false;
                            _JoystickHeld[Event.jaxis.which][JOYSTICK_RIGHT] = false;
                        }
                    }
            }
            else
			{
                if(Event.jaxis.axis == 1)
                {
                    if(Event.jaxis.value > JOYSTICKDEADZONE)
                    {
                       if(Event.jaxis.which < MAXJOYSTICKS)
                            _JoystickHeld[Event.jaxis.which][JOYSTICK_DOWN] = true;
                    }
                    else
                        if(Event.jaxis.value < -JOYSTICKDEADZONE)
                        {
                            if(Event.jaxis.which < MAXJOYSTICKS)
                                _JoystickHeld[Event.jaxis.which][JOYSTICK_UP] = true;
                        }
                        else
                        {
                            if(Event.jaxis.which < MAXJOYSTICKS)
                            {
                                _JoystickHeld[Event.jaxis.which][JOYSTICK_DOWN] = false;
                                _JoystickHeld[Event.jaxis.which][JOYSTICK_UP] = false;
                            }
                        }
                }
			}
		}
        if(Event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN)
		{
            if(Event.jbutton.button < MAXJOYSTICKBUTTONS)
            {
                if(Event.jbutton.which < MAXJOYSTICKS)
                    _JoystickHeld[Event.jbutton.which][Event.jbutton.button] = true;
            }
		}
        if(Event.type == SDL_EVENT_JOYSTICK_BUTTON_UP)
            if(Event.jbutton.button < MAXJOYSTICKBUTTONS)
            {
                if(Event.jbutton.which < MAXJOYSTICKS)
                    _JoystickHeld[Event.jbutton.which][Event.jbutton.button] = false;
            }
        if(Event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            if(Event.button.which < MAXMOUSES)
                if(Event.button.button < MAXMOUSEBUTTONS)
                    _MouseHeld[Event.button.which][Event.button.button] = true;

        if(Event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            if(Event.button.which < MAXMOUSES)
                if(Event.button.button < MAXMOUSEBUTTONS)
                    _MouseHeld[Event.button.which][Event.button.button] = false;

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

