/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Joystick functions
 */
#include <SDL/SDL.h>
#include "../include/SDL2/SDL_joystick.h"

const char* SDL_JoystickNameForIndex(int device_index) {
    return SDL_JoystickName(device_index);
}

SDL_bool SDL_JoystickGetAttached(SDL_Joystick *joystick) {
    if (!joystick) return SDL_FALSE;

    /* In SDL1.2, if we have a valid joystick pointer it's attached */
    /* Check by trying to get the number of axes */
    return (SDL_JoystickNumAxes(joystick) >= 0) ? SDL_TRUE : SDL_FALSE;
}

int SDL_JoystickRumble(SDL_Joystick *joystick, Uint16 low_freq, Uint16 high_freq, Uint32 duration) {
    /* Rumble not supported in SDL1.2 */
    (void)joystick;
    (void)low_freq;
    (void)high_freq;
    (void)duration;

    return -1; /* Unsupported */
}

/* Track opened joysticks - simple array for SDL1.2 single-threaded use */
#define MAX_JOYSTICKS 8
static SDL_Joystick *opened_joysticks[MAX_JOYSTICKS] = {0};

SDL_JoystickID SDL_JoystickInstanceID(SDL_Joystick *joystick) {
    int i;
    if (!joystick) return -1;

    /* Find this joystick in our tracking array */
    for (i = 0; i < MAX_JOYSTICKS; i++) {
        if (opened_joysticks[i] == joystick) {
            return (SDL_JoystickID)i;
        }
    }

    /* Not found - try to add it */
    for (i = 0; i < MAX_JOYSTICKS; i++) {
        if (opened_joysticks[i] == NULL) {
            opened_joysticks[i] = joystick;
            return (SDL_JoystickID)i;
        }
    }

    return -1; /* No room */
}

SDL_Joystick* SDL_JoystickFromInstanceID(SDL_JoystickID instance_id) {
    if (instance_id < 0 || instance_id >= MAX_JOYSTICKS) {
        return NULL;
    }
    return opened_joysticks[instance_id];
}
