/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Joystick subsystem
 */
#ifndef SDL2_COMPAT_SDL_JOYSTICK_H
#define SDL2_COMPAT_SDL_JOYSTICK_H

#include <SDL/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Most joystick functions are compatible between SDL1.2 and SDL2 */
/* SDL_Joystick type is the same */

/* SDL2 joystick instance ID - use joystick index in SDL1.2 */
typedef Sint32 SDL_JoystickID;

/* SDL2-specific functions that need shimming */
const char* SDL_JoystickNameForIndex(int device_index);
SDL_bool SDL_JoystickGetAttached(SDL_Joystick *joystick);
int SDL_JoystickRumble(SDL_Joystick *joystick, Uint16 low_freq, Uint16 high_freq, Uint32 duration);
SDL_JoystickID SDL_JoystickInstanceID(SDL_Joystick *joystick);

/* For tracking joysticks - simple wrapper around index for SDL1.2 */
SDL_Joystick* SDL_JoystickFromInstanceID(SDL_JoystickID instance_id);

/* These are already in SDL1.2:
 * SDL_NumJoysticks()
 * SDL_JoystickOpen()
 * SDL_JoystickClose()
 * SDL_JoystickNumAxes()
 * SDL_JoystickNumButtons()
 * SDL_JoystickNumBalls()
 * SDL_JoystickNumHats()
 * SDL_JoystickGetAxis()
 * SDL_JoystickGetButton()
 * SDL_JoystickGetHat()
 */

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_JOYSTICK_H */
