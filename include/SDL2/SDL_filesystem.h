/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Filesystem functions (SDL2 only - need shimming)
 */
#ifndef SDL2_COMPAT_SDL_FILESYSTEM_H
#define SDL2_COMPAT_SDL_FILESYSTEM_H

#include <SDL/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SDL2 filesystem functions - not in SDL1.2 */
char* SDL_GetBasePath(void);
char* SDL_GetPrefPath(const char *org, const char *app);

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_FILESYSTEM_H */
