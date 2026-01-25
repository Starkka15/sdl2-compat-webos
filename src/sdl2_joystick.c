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
