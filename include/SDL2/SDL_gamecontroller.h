/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * SDL_gamecontroller.h - Game controller functions
 *
 * SDL1.2 doesn't have the GameController API, only Joystick.
 * We provide stubs that return "not supported" for most functions.
 */
#ifndef SDL2_COMPAT_SDL_GAMECONTROLLER_H
#define SDL2_COMPAT_SDL_GAMECONTROLLER_H

#include <SDL/SDL_joystick.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque game controller type */
typedef struct _SDL_GameController SDL_GameController;

/* Game controller button enum */
typedef enum {
    SDL_CONTROLLER_BUTTON_INVALID = -1,
    SDL_CONTROLLER_BUTTON_A,
    SDL_CONTROLLER_BUTTON_B,
    SDL_CONTROLLER_BUTTON_X,
    SDL_CONTROLLER_BUTTON_Y,
    SDL_CONTROLLER_BUTTON_BACK,
    SDL_CONTROLLER_BUTTON_GUIDE,
    SDL_CONTROLLER_BUTTON_START,
    SDL_CONTROLLER_BUTTON_LEFTSTICK,
    SDL_CONTROLLER_BUTTON_RIGHTSTICK,
    SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
    SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
    SDL_CONTROLLER_BUTTON_DPAD_UP,
    SDL_CONTROLLER_BUTTON_DPAD_DOWN,
    SDL_CONTROLLER_BUTTON_DPAD_LEFT,
    SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
    SDL_CONTROLLER_BUTTON_MAX
} SDL_GameControllerButton;

/* Game controller axis enum */
typedef enum {
    SDL_CONTROLLER_AXIS_INVALID = -1,
    SDL_CONTROLLER_AXIS_LEFTX,
    SDL_CONTROLLER_AXIS_LEFTY,
    SDL_CONTROLLER_AXIS_RIGHTX,
    SDL_CONTROLLER_AXIS_RIGHTY,
    SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
    SDL_CONTROLLER_AXIS_MAX
} SDL_GameControllerAxis;

/* Check if joystick is a game controller - always returns false in our shim */
static inline SDL_bool SDL_IsGameController(int joystick_index) {
    (void)joystick_index;
    return (SDL_bool)SDL_FALSE;
}

/* Open game controller - returns NULL since not supported */
static inline SDL_GameController* SDL_GameControllerOpen(int joystick_index) {
    (void)joystick_index;
    return NULL;
}

/* Close game controller */
static inline void SDL_GameControllerClose(SDL_GameController *gamecontroller) {
    (void)gamecontroller;
}

/* Get joystick from game controller */
static inline SDL_Joystick* SDL_GameControllerGetJoystick(SDL_GameController *gamecontroller) {
    (void)gamecontroller;
    return NULL;
}

/* Game controller rumble - stub that does nothing */
static inline int SDL_GameControllerRumble(SDL_GameController *gamecontroller,
                                            Uint16 low_frequency_rumble,
                                            Uint16 high_frequency_rumble,
                                            Uint32 duration_ms) {
    (void)gamecontroller;
    (void)low_frequency_rumble;
    (void)high_frequency_rumble;
    (void)duration_ms;
    return -1; /* Not supported */
}

/* Get button state */
static inline Uint8 SDL_GameControllerGetButton(SDL_GameController *gamecontroller,
                                                 SDL_GameControllerButton button) {
    (void)gamecontroller;
    (void)button;
    return 0;
}

/* Get axis state */
static inline Sint16 SDL_GameControllerGetAxis(SDL_GameController *gamecontroller,
                                                SDL_GameControllerAxis axis) {
    (void)gamecontroller;
    (void)axis;
    return 0;
}

/* Load controller mappings from file - stub that does nothing */
static inline int SDL_GameControllerAddMappingsFromFile(const char *file) {
    (void)file;
    return 0; /* Return 0 mappings added */
}

/* Add a single mapping string */
static inline int SDL_GameControllerAddMapping(const char *mappingString) {
    (void)mappingString;
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_GAMECONTROLLER_H */
