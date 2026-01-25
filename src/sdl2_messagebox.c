/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Message box functions
 */
#include <SDL/SDL.h>
#include <stdio.h>
#include "../include/SDL2/SDL_messagebox.h"
#include "../include/SDL2/SDL_video.h"

int SDL_ShowSimpleMessageBox(Uint32 flags, const char *title,
                             const char *message, struct SDL_Window *window) {
    const char *type;

    (void)window;

    /* Determine message type for console output */
    if (flags & SDL_MESSAGEBOX_ERROR) {
        type = "ERROR";
    } else if (flags & SDL_MESSAGEBOX_WARNING) {
        type = "WARNING";
    } else {
        type = "INFO";
    }

    /* SDL1.2 doesn't have native message boxes */
    /* Just print to console */
    fprintf(stderr, "[%s] %s: %s\n", type, title ? title : "Message", message ? message : "");

    return 0;
}
