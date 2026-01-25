/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Surface handling
 */
#ifndef SDL2_COMPAT_SDL_SURFACE_H
#define SDL2_COMPAT_SDL_SURFACE_H

#include <SDL/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SDL_Surface is the same structure in SDL1.2 and SDL2 */

/* SDL2 uses SDL_TRUE/SDL_FALSE for colorkey enable */
#ifndef SDL_TRUE
#define SDL_TRUE 1
#define SDL_FALSE 0
#endif

/* SDL2 SetColorKey signature differs */
/* SDL2: int SDL_SetColorKey(SDL_Surface *surface, int flag, Uint32 key) */
/*   flag is SDL_TRUE (1) to enable, SDL_FALSE (0) to disable */
/* SDL1.2: int SDL_SetColorKey(SDL_Surface *surface, Uint32 flag, Uint32 key) */
/*   flag is SDL_SRCCOLORKEY (0x1000) to enable, 0 to disable */
/* We need a wrapper to convert SDL_TRUE to SDL_SRCCOLORKEY */
int SDL2_SetColorKey(SDL_Surface *surface, int flag, Uint32 key);
#undef SDL_SetColorKey
#define SDL_SetColorKey SDL2_SetColorKey

/* SDL_CreateRGBSurface is compatible */
/* SDL_FreeSurface is compatible */
/* SDL_BlitSurface is compatible */

/* SDL2 has SDL_CreateRGBSurfaceWithFormat - not in SDL1.2 */
/* Use SDL_CreateRGBSurface instead */

/* SDL_ConvertSurface is compatible */
/* SDL_SetSurfaceAlphaMod is new in SDL2 */
int SDL_SetSurfaceAlphaMod(SDL_Surface *surface, Uint8 alpha);
int SDL_GetSurfaceAlphaMod(SDL_Surface *surface, Uint8 *alpha);
int SDL_SetSurfaceColorMod(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b);
int SDL_SetSurfaceBlendMode(SDL_Surface *surface, int blendMode);

/* SDL_GetColorKey differs between SDL1.2 and SDL2 */
/* SDL1.2: Uint32 SDL_GetColorKey(SDL_Surface *surface) - returns colorkey directly */
/* SDL2: int SDL_GetColorKey(SDL_Surface *surface, Uint32 *key) - stores in pointer */
/* We override the SDL1.2 function with SDL2-style signature */
#undef SDL_GetColorKey
int SDL_GetColorKey(SDL_Surface *surface, Uint32 *key);

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_SURFACE_H */
