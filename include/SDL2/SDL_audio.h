/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * SDL_audio.h - Audio device functions
 */
#ifndef SDL2_COMPAT_SDL_AUDIO_H
#define SDL2_COMPAT_SDL_AUDIO_H

#include <SDL/SDL_audio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SDL2 audio device ID type */
typedef Uint32 SDL_AudioDeviceID;

/* Audio format additions from SDL2 */
#ifndef AUDIO_F32
#define AUDIO_F32LSB    0x8120  /* 32-bit floating point samples */
#define AUDIO_F32MSB    0x9120
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define AUDIO_F32       AUDIO_F32LSB
#else
#define AUDIO_F32       AUDIO_F32MSB
#endif
#endif

/*
 * SDL2 audio device functions
 * In SDL2 you can open multiple audio devices. SDL1.2 only supports one.
 * We simulate device IDs where device 1 is the opened device.
 */

/* Open a specific audio device (SDL2 style) */
SDL_AudioDeviceID SDL_OpenAudioDevice(const char *device,
                                       int iscapture,
                                       const SDL_AudioSpec *desired,
                                       SDL_AudioSpec *obtained,
                                       int allowed_changes);

/* Pause/unpause a specific audio device */
void SDL_PauseAudioDevice(SDL_AudioDeviceID dev, int pause_on);

/* Close a specific audio device */
void SDL_CloseAudioDevice(SDL_AudioDeviceID dev);

/* Get number of audio devices */
int SDL_GetNumAudioDevices(int iscapture);

/* Get audio device name */
const char* SDL_GetAudioDeviceName(int index, int iscapture);

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_AUDIO_H */
