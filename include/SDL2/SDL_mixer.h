/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * SDL_mixer wrapper - maps to SDL_mixer 1.2
 */
#ifndef SDL2_COMPAT_SDL_MIXER_H
#define SDL2_COMPAT_SDL_MIXER_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

/* SDL2_mixer and SDL_mixer 1.2 have nearly identical APIs */
/* Most functions are compatible:
 *   Mix_OpenAudio()
 *   Mix_CloseAudio()
 *   Mix_LoadWAV()
 *   Mix_LoadMUS()
 *   Mix_FreeChunk()
 *   Mix_FreeMusic()
 *   Mix_PlayChannel()
 *   Mix_PlayMusic()
 *   Mix_HaltChannel()
 *   Mix_HaltMusic()
 *   Mix_VolumeMusic()
 *   Mix_VolumeChunk()
 *   Mix_SetMusicPosition()
 *   etc.
 */

/* SDL2_mixer added Mix_OpenAudioDevice - not in 1.2 */
/* Just use Mix_OpenAudio instead */

#endif /* SDL2_COMPAT_SDL_MIXER_H */
