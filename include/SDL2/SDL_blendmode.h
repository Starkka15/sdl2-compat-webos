/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Blend modes
 */
#ifndef SDL2_COMPAT_SDL_BLENDMODE_H
#define SDL2_COMPAT_SDL_BLENDMODE_H

#ifdef __cplusplus
extern "C" {
#endif

/* SDL2 blend modes */
typedef enum {
    SDL_BLENDMODE_NONE  = 0x00000000,
    SDL_BLENDMODE_BLEND = 0x00000001,
    SDL_BLENDMODE_ADD   = 0x00000002,
    SDL_BLENDMODE_MOD   = 0x00000004
} SDL_BlendMode;

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_BLENDMODE_H */
