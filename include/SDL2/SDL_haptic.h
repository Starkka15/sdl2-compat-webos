/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * SDL_haptic.h - Haptic (force feedback) functions
 *
 * SDL1.2 doesn't have the Haptic API.
 * We provide stubs that return "not supported".
 */
#ifndef SDL2_COMPAT_SDL_HAPTIC_H
#define SDL2_COMPAT_SDL_HAPTIC_H

#include <SDL/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque haptic device type */
typedef struct _SDL_Haptic SDL_Haptic;

/* Open haptic device from joystick - returns NULL since not supported */
static inline SDL_Haptic* SDL_HapticOpenFromJoystick(SDL_Joystick *joystick) {
    (void)joystick;
    return NULL;
}

/* Close haptic device */
static inline void SDL_HapticClose(SDL_Haptic *haptic) {
    (void)haptic;
}

/* Initialize rumble support */
static inline int SDL_HapticRumbleInit(SDL_Haptic *haptic) {
    (void)haptic;
    return -1; /* Not supported */
}

/* Check if rumble is supported */
static inline int SDL_HapticRumbleSupported(SDL_Haptic *haptic) {
    (void)haptic;
    return SDL_FALSE;
}

/* Play rumble effect */
static inline int SDL_HapticRumblePlay(SDL_Haptic *haptic, float strength, Uint32 length) {
    (void)haptic;
    (void)strength;
    (void)length;
    return -1; /* Not supported */
}

/* Stop rumble effect */
static inline int SDL_HapticRumbleStop(SDL_Haptic *haptic) {
    (void)haptic;
    return -1; /* Not supported */
}

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_HAPTIC_H */
