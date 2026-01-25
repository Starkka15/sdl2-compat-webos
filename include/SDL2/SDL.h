/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Main header - includes all SDL2 subsystems
 */
#ifndef SDL2_COMPAT_SDL_H
#define SDL2_COMPAT_SDL_H

#include <SDL/SDL.h>

/* SDL2 version info - don't redefine, SDL1.2 has its own */
#undef SDL_MAJOR_VERSION
#undef SDL_MINOR_VERSION
#undef SDL_PATCHLEVEL
#define SDL_MAJOR_VERSION 2
#define SDL_MINOR_VERSION 0
#define SDL_PATCHLEVEL 0

/* Include our compatibility headers */
#include "SDL_video.h"
#include "SDL_render.h"
#include "SDL_events.h"
#include "SDL_joystick.h"
#include "SDL_timer.h"
#include "SDL_error.h"
#include "SDL_surface.h"
#include "SDL_pixels.h"
#include "SDL_blendmode.h"
#include "SDL_filesystem.h"
#include "SDL_messagebox.h"

/* SDL2 init flags (map to SDL1.2) */
#ifndef SDL_INIT_EVENTS
#define SDL_INIT_EVENTS 0  /* No separate events init in SDL1.2 */
#endif

/* SDL2 boolean type - SDL1.2 already defines this in SDL_stdinc.h */
/* So we just check it's available and don't redefine */

#ifdef __cplusplus
extern "C" {
#endif

/* SDL2 memory functions (map to stdlib) */
#include <stdlib.h>
#include <string.h>

#define SDL_malloc malloc
#define SDL_free free
#define SDL_realloc realloc
#define SDL_memcpy memcpy
#define SDL_memset memset
#define SDL_strlen strlen
#define SDL_strdup strdup

/* SDL2 GetTicks is same as SDL1.2 */
/* SDL_GetTicks() already defined in SDL.h */

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_H */
