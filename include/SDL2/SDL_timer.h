/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Timer subsystem - mostly compatible
 */
#ifndef SDL2_COMPAT_SDL_TIMER_H
#define SDL2_COMPAT_SDL_TIMER_H

#include <SDL/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SDL_GetTicks() is the same in SDL1.2 and SDL2 */
/* SDL_Delay() is the same in SDL1.2 and SDL2 */

/* SDL2 has 64-bit performance counter - not available in SDL1.2 */
/* Just use SDL_GetTicks() * 1000 as approximation */
static inline Uint64 SDL_GetPerformanceCounter(void) {
    return (Uint64)SDL_GetTicks() * 1000;
}

static inline Uint64 SDL_GetPerformanceFrequency(void) {
    return 1000000; /* microseconds */
}

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_TIMER_H */
