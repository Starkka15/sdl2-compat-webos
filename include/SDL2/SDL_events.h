/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Events - mostly pass through to SDL1.2
 */
#ifndef SDL2_COMPAT_SDL_EVENTS_H
#define SDL2_COMPAT_SDL_EVENTS_H

#include <SDL/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SDL2 uses SDL_KEYDOWN/SDL_KEYUP same as SDL1.2 */
/* Event types should be compatible */

/* SDL_Keycode - in SDL2 this is Sint32, in SDL1.2 it's SDLKey enum */
typedef SDLKey SDL_Keycode;
typedef SDLKey SDL_Scancode;

/* SDL2 event type aliases that may differ */
#ifndef SDL_WINDOWEVENT
#define SDL_WINDOWEVENT SDL_VIDEOEXPOSE
#endif

/* SDL2 key repeat handling (SDL1.2 uses SDL_EnableKeyRepeat) */
/* In SDL2, key repeat is always enabled */

/* Window event IDs (SDL2 specific - simulate where needed) */
typedef enum {
    SDL_WINDOWEVENT_NONE,
    SDL_WINDOWEVENT_SHOWN,
    SDL_WINDOWEVENT_HIDDEN,
    SDL_WINDOWEVENT_EXPOSED,
    SDL_WINDOWEVENT_MOVED,
    SDL_WINDOWEVENT_RESIZED,
    SDL_WINDOWEVENT_SIZE_CHANGED,
    SDL_WINDOWEVENT_MINIMIZED,
    SDL_WINDOWEVENT_MAXIMIZED,
    SDL_WINDOWEVENT_RESTORED,
    SDL_WINDOWEVENT_ENTER,
    SDL_WINDOWEVENT_LEAVE,
    SDL_WINDOWEVENT_FOCUS_GAINED,
    SDL_WINDOWEVENT_FOCUS_LOST,
    SDL_WINDOWEVENT_CLOSE
} SDL_WindowEventID;

/* SDL2 text input events - not fully supported in SDL1.2 */
/* Games that need text input will need special handling */

/* SDL_PollEvent and SDL_WaitEvent are the same in SDL1.2 */

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_EVENTS_H */
