/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Audio device functions implementation
 *
 * SDL1.2 only supports a single audio device, so we simulate
 * SDL2's multi-device API with device ID 1 being the single device.
 */

#include <SDL/SDL.h>
#include "../include/SDL2/SDL_audio.h"

/* Track if audio device is open */
static int audio_device_open = 0;

SDL_AudioDeviceID SDL_OpenAudioDevice(const char *device,
                                       int iscapture,
                                       const SDL_AudioSpec *desired,
                                       SDL_AudioSpec *obtained,
                                       int allowed_changes) {
    (void)device;  /* SDL1.2 doesn't support device selection */
    (void)allowed_changes;  /* SDL1.2 doesn't support this */

    /* SDL1.2 doesn't support capture */
    if (iscapture) {
        SDL_SetError("Audio capture not supported");
        return 0;
    }

    /* Only one device can be open */
    if (audio_device_open) {
        SDL_SetError("Audio device already open");
        return 0;
    }

    /* Open the audio device using SDL1.2 API */
    if (SDL_OpenAudio((SDL_AudioSpec*)desired, obtained) < 0) {
        return 0;
    }

    audio_device_open = 1;
    return 1;  /* Device ID 1 */
}

void SDL_PauseAudioDevice(SDL_AudioDeviceID dev, int pause_on) {
    if (dev == 1 && audio_device_open) {
        SDL_PauseAudio(pause_on);
    }
}

void SDL_CloseAudioDevice(SDL_AudioDeviceID dev) {
    if (dev == 1 && audio_device_open) {
        SDL_CloseAudio();
        audio_device_open = 0;
    }
}

int SDL_GetNumAudioDevices(int iscapture) {
    (void)iscapture;
    /* SDL1.2 doesn't enumerate devices, just report 1 */
    return 1;
}

const char* SDL_GetAudioDeviceName(int index, int iscapture) {
    (void)iscapture;
    if (index == 0) {
        return "Default Audio Device";
    }
    return NULL;
}
