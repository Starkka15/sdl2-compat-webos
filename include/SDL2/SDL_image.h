/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * SDL_image wrapper - maps to SDL_image 1.2
 */
#ifndef SDL2_COMPAT_SDL_IMAGE_H
#define SDL2_COMPAT_SDL_IMAGE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/* SDL2_image and SDL_image 1.2 have nearly identical APIs */
/* IMG_Load, IMG_LoadPNG_RW, etc. are the same */

/* IMG_Init flags - SDL_image 1.2 doesn't have IMG_Init/IMG_Quit
 * We define these as no-ops since SDL_image 1.2 initializes automatically
 */
#ifndef IMG_INIT_JPG
#define IMG_INIT_JPG 0x01
#endif
#ifndef IMG_INIT_PNG
#define IMG_INIT_PNG 0x02
#endif
#ifndef IMG_INIT_TIF
#define IMG_INIT_TIF 0x04
#endif
#ifndef IMG_INIT_WEBP
#define IMG_INIT_WEBP 0x08
#endif

/* IMG_Init stub - SDL_image 1.2 auto-initializes */
static inline int IMG_Init(int flags) {
    (void)flags;
    return flags;  /* Return flags as if all requested formats initialized */
}

/* IMG_Quit stub */
static inline void IMG_Quit(void) {
    /* Nothing to do in SDL_image 1.2 */
}

#endif /* SDL2_COMPAT_SDL_IMAGE_H */
