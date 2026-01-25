/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Keyboard functions implementation
 */

#include <SDL/SDL.h>
#include "../include/SDL2/SDL_scancode.h"
#include "../include/SDL2/SDL_keyboard.h"
#include <string.h>

/* Internal state array for scancodes */
static Uint8 scancode_state[SDL_NUM_SCANCODES];

/* Map SDL1.2 SDLKey to SDL2 SDL_Scancode */
static SDL_Scancode sdlkey_to_scancode(SDLKey key) {
    switch (key) {
        case SDLK_a: return SDL_SCANCODE_A;
        case SDLK_b: return SDL_SCANCODE_B;
        case SDLK_c: return SDL_SCANCODE_C;
        case SDLK_d: return SDL_SCANCODE_D;
        case SDLK_e: return SDL_SCANCODE_E;
        case SDLK_f: return SDL_SCANCODE_F;
        case SDLK_g: return SDL_SCANCODE_G;
        case SDLK_h: return SDL_SCANCODE_H;
        case SDLK_i: return SDL_SCANCODE_I;
        case SDLK_j: return SDL_SCANCODE_J;
        case SDLK_k: return SDL_SCANCODE_K;
        case SDLK_l: return SDL_SCANCODE_L;
        case SDLK_m: return SDL_SCANCODE_M;
        case SDLK_n: return SDL_SCANCODE_N;
        case SDLK_o: return SDL_SCANCODE_O;
        case SDLK_p: return SDL_SCANCODE_P;
        case SDLK_q: return SDL_SCANCODE_Q;
        case SDLK_r: return SDL_SCANCODE_R;
        case SDLK_s: return SDL_SCANCODE_S;
        case SDLK_t: return SDL_SCANCODE_T;
        case SDLK_u: return SDL_SCANCODE_U;
        case SDLK_v: return SDL_SCANCODE_V;
        case SDLK_w: return SDL_SCANCODE_W;
        case SDLK_x: return SDL_SCANCODE_X;
        case SDLK_y: return SDL_SCANCODE_Y;
        case SDLK_z: return SDL_SCANCODE_Z;
        case SDLK_0: return SDL_SCANCODE_0;
        case SDLK_1: return SDL_SCANCODE_1;
        case SDLK_2: return SDL_SCANCODE_2;
        case SDLK_3: return SDL_SCANCODE_3;
        case SDLK_4: return SDL_SCANCODE_4;
        case SDLK_5: return SDL_SCANCODE_5;
        case SDLK_6: return SDL_SCANCODE_6;
        case SDLK_7: return SDL_SCANCODE_7;
        case SDLK_8: return SDL_SCANCODE_8;
        case SDLK_9: return SDL_SCANCODE_9;
        case SDLK_RETURN: return SDL_SCANCODE_RETURN;
        case SDLK_ESCAPE: return SDL_SCANCODE_ESCAPE;
        case SDLK_BACKSPACE: return SDL_SCANCODE_BACKSPACE;
        case SDLK_TAB: return SDL_SCANCODE_TAB;
        case SDLK_SPACE: return SDL_SCANCODE_SPACE;
        case SDLK_UP: return SDL_SCANCODE_UP;
        case SDLK_DOWN: return SDL_SCANCODE_DOWN;
        case SDLK_LEFT: return SDL_SCANCODE_LEFT;
        case SDLK_RIGHT: return SDL_SCANCODE_RIGHT;
        case SDLK_LSHIFT: return SDL_SCANCODE_LSHIFT;
        case SDLK_RSHIFT: return SDL_SCANCODE_RSHIFT;
        case SDLK_LCTRL: return SDL_SCANCODE_LCTRL;
        case SDLK_RCTRL: return SDL_SCANCODE_RCTRL;
        case SDLK_LALT: return SDL_SCANCODE_LALT;
        case SDLK_RALT: return SDL_SCANCODE_RALT;
        case SDLK_F1: return SDL_SCANCODE_F1;
        case SDLK_F2: return SDL_SCANCODE_F2;
        case SDLK_F3: return SDL_SCANCODE_F3;
        case SDLK_F4: return SDL_SCANCODE_F4;
        case SDLK_F5: return SDL_SCANCODE_F5;
        case SDLK_F6: return SDL_SCANCODE_F6;
        case SDLK_F7: return SDL_SCANCODE_F7;
        case SDLK_F8: return SDL_SCANCODE_F8;
        case SDLK_F9: return SDL_SCANCODE_F9;
        case SDLK_F10: return SDL_SCANCODE_F10;
        case SDLK_F11: return SDL_SCANCODE_F11;
        case SDLK_F12: return SDL_SCANCODE_F12;
        case SDLK_INSERT: return SDL_SCANCODE_INSERT;
        case SDLK_DELETE: return SDL_SCANCODE_DELETE;
        case SDLK_HOME: return SDL_SCANCODE_HOME;
        case SDLK_END: return SDL_SCANCODE_END;
        case SDLK_PAGEUP: return SDL_SCANCODE_PAGEUP;
        case SDLK_PAGEDOWN: return SDL_SCANCODE_PAGEDOWN;
        default: return SDL_SCANCODE_UNKNOWN;
    }
}

const Uint8* SDL_GetKeyboardState(int *numkeys) {
    int i;
    Uint8 *sdl1_keys;
    int num_sdl1_keys;

    /* Clear our state array */
    memset(scancode_state, 0, sizeof(scancode_state));

    /* Get SDL1.2 key state */
    sdl1_keys = SDL_GetKeyState(&num_sdl1_keys);

    /* Map each pressed key to our scancode array */
    for (i = 0; i < num_sdl1_keys && i < SDLK_LAST; i++) {
        if (sdl1_keys[i]) {
            SDL_Scancode sc = sdlkey_to_scancode((SDLKey)i);
            if (sc != SDL_SCANCODE_UNKNOWN) {
                scancode_state[sc] = 1;
            }
        }
    }

    if (numkeys) {
        *numkeys = SDL_NUM_SCANCODES;
    }

    return scancode_state;
}
