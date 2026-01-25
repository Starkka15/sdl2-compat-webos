/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * SDL_keyboard.h - Keyboard functions
 */
#ifndef SDL2_COMPAT_SDL_KEYBOARD_H
#define SDL2_COMPAT_SDL_KEYBOARD_H

#include "SDL_scancode.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * SDL2's SDL_GetKeyboardState returns array indexed by SDL_Scancode.
 * SDL1.2's SDL_GetKeyState returns array indexed by SDLKey.
 * We provide a wrapper that maps between them.
 */
const Uint8* SDL_GetKeyboardState(int *numkeys);

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_KEYBOARD_H */
