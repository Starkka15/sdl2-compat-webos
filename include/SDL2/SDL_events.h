/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Events - mostly pass through to SDL1.2
 */
#ifndef SDL2_COMPAT_SDL_EVENTS_H
#define SDL2_COMPAT_SDL_EVENTS_H

#include <SDL/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SDL2 uses SDL_KEYDOWN/SDL_KEYUP same as SDL1.2 */
/* Event types should be compatible */

/* SDL_Keycode - in SDL2 this is Sint32, in SDL1.2 it's SDLKey enum */
typedef SDLKey SDL_Keycode;
/* Note: SDL_Scancode is defined in SDL_scancode.h */

/* SDL2 event type aliases that may differ */
#ifndef SDL_WINDOWEVENT
#define SDL_WINDOWEVENT SDL_VIDEOEXPOSE
#endif

/* SDL2 key repeat handling (SDL1.2 uses SDL_EnableKeyRepeat) */
/* In SDL2, key repeat is always enabled */

/* Window event IDs (SDL2 specific - simulate where needed) */
typedef enum {
    SDL_WINDOWEVENT_NONE,
    SDL_WINDOWEVENT_SHOWN,
    SDL_WINDOWEVENT_HIDDEN,
    SDL_WINDOWEVENT_EXPOSED,
    SDL_WINDOWEVENT_MOVED,
    SDL_WINDOWEVENT_RESIZED,
    SDL_WINDOWEVENT_SIZE_CHANGED,
    SDL_WINDOWEVENT_MINIMIZED,
    SDL_WINDOWEVENT_MAXIMIZED,
    SDL_WINDOWEVENT_RESTORED,
    SDL_WINDOWEVENT_ENTER,
    SDL_WINDOWEVENT_LEAVE,
    SDL_WINDOWEVENT_FOCUS_GAINED,
    SDL_WINDOWEVENT_FOCUS_LOST,
    SDL_WINDOWEVENT_CLOSE
} SDL_WindowEventID;

/* SDL2 event types for joystick/controller that don't exist in SDL1.2 */
/* We define high values that won't conflict with SDL1.2 event types */
#ifndef SDL_JOYDEVICEADDED
#define SDL_JOYDEVICEADDED      0x605
#define SDL_JOYDEVICEREMOVED    0x606
#endif

#ifndef SDL_CONTROLLERAXISMOTION
#define SDL_CONTROLLERAXISMOTION    0x650
#define SDL_CONTROLLERBUTTONDOWN    0x651
#define SDL_CONTROLLERBUTTONUP      0x652
#define SDL_CONTROLLERDEVICEADDED   0x653
#define SDL_CONTROLLERDEVICEREMOVED 0x654
#endif

/* SDL2 joystick ID type */
typedef Sint32 SDL_JoystickID;

/* Controller button event structure */
typedef struct SDL_ControllerButtonEvent {
    Uint32 type;
    Uint32 timestamp;
    Sint32 which;
    Uint8 button;
    Uint8 state;
    Uint8 padding1;
    Uint8 padding2;
} SDL_ControllerButtonEvent;

/* Controller axis event structure */
typedef struct SDL_ControllerAxisEvent {
    Uint32 type;
    Uint32 timestamp;
    Sint32 which;
    Uint8 axis;
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 value;
    Uint16 padding4;
} SDL_ControllerAxisEvent;

/* Controller device event structure */
typedef struct SDL_ControllerDeviceEvent {
    Uint32 type;
    Uint32 timestamp;
    Sint32 which;
} SDL_ControllerDeviceEvent;

/* Joystick device event structure */
typedef struct SDL_JoyDeviceEvent {
    Uint32 type;
    Uint32 timestamp;
    Sint32 which;
} SDL_JoyDeviceEvent;

/* SDL2 text input events - not fully supported in SDL1.2 */
/* Games that need text input will need special handling */

/* SDL_PollEvent and SDL_WaitEvent are the same in SDL1.2 */

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_EVENTS_H */
