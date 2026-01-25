/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Surface functions
 */
#include <SDL/SDL.h>

/* Store pointer to real SDL1.2 SDL_SetColorKey before our header overrides it */
static int (*real_SDL_SetColorKey)(SDL_Surface *surface, Uint32 flag, Uint32 key) = SDL_SetColorKey;

#include "../include/SDL2/SDL_surface.h"

int SDL_SetSurfaceAlphaMod(SDL_Surface *surface, Uint8 alpha) {
    if (!surface) return -1;

    return SDL_SetAlpha(surface, SDL_SRCALPHA, alpha);
}

int SDL_GetSurfaceAlphaMod(SDL_Surface *surface, Uint8 *alpha) {
    if (!surface || !alpha) return -1;

    /* In SDL1.2, per-surface alpha is stored in format->alpha */
    if (surface->flags & SDL_SRCALPHA) {
        *alpha = surface->format->alpha;
    } else {
        *alpha = SDL_ALPHA_OPAQUE;
    }

    return 0;
}

int SDL_SetSurfaceColorMod(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b) {
    /* SDL1.2 doesn't have per-surface color modulation */
    /* This would need to be done manually pixel by pixel */
    (void)surface;
    (void)r;
    (void)g;
    (void)b;

    return 0; /* Silently succeed - color mod applied during blit in texture */
}

int SDL_SetSurfaceBlendMode(SDL_Surface *surface, int blendMode) {
    if (!surface) return -1;

    if (blendMode == 1) { /* SDL_BLENDMODE_BLEND */
        SDL_SetAlpha(surface, SDL_SRCALPHA, surface->format->alpha);
    } else {
        SDL_SetAlpha(surface, 0, SDL_ALPHA_OPAQUE);
    }

    return 0;
}

/* SDL2-style SDL_SetColorKey wrapper
 * SDL2: int SDL_SetColorKey(surface, SDL_TRUE/SDL_FALSE, key)
 * SDL1.2: int SDL_SetColorKey(surface, SDL_SRCCOLORKEY|0, key)
 * We need to convert SDL_TRUE (1) to SDL_SRCCOLORKEY (0x1000)
 */
int SDL2_SetColorKey(SDL_Surface *surface, int flag, Uint32 key) {
    if (!surface) return -1;

    Uint32 sdl1_flag = 0;
    if (flag) {
        /* SDL_TRUE or any non-zero value means enable colorkey */
        sdl1_flag = SDL_SRCCOLORKEY;
    }

    return real_SDL_SetColorKey(surface, sdl1_flag, key);
}

/* SDL2-style SDL_GetColorKey - stores colorkey in pointer */
int SDL_GetColorKey(SDL_Surface *surface, Uint32 *key) {
    if (!surface || !key) return -1;

    /* In SDL1.2, colorkey is stored in surface->format->colorkey */
    /* It's valid if SDL_SRCCOLORKEY flag is set */
    if (surface->flags & SDL_SRCCOLORKEY) {
        *key = surface->format->colorkey;
        return 0;
    }

    return -1; /* No colorkey set */
}
