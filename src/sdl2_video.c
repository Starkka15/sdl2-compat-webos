/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Video/Window implementation
 */
#include <SDL/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/SDL2/SDL_video.h"

#if defined(__WEBOS__)
#include <GLES/gl.h>
#endif

/* Internal window structure */
struct SDL_Window {
    SDL_Surface *surface;  /* Software surface for rendering */
    SDL_Surface *screen;   /* The actual screen surface from SetVideoMode */
    char *title;
    int w, h;
    Uint32 flags;
    int fullscreen;
};

/* Global window pointer - SDL1.2 only supports one window */
static SDL_Window *g_window = NULL;

SDL_Window* SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags) {
    SDL_Window *window;
    SDL_Surface *screen;

    (void)x; (void)y; /* Position ignored in SDL1.2 */

    if (g_window != NULL) {
        SDL_SetError("SDL1.2 only supports one window");
        return NULL;
    }

    window = (SDL_Window *)malloc(sizeof(SDL_Window));
    if (!window) {
        SDL_SetError("Out of memory");
        return NULL;
    }

    memset(window, 0, sizeof(SDL_Window));

    /* Convert SDL2 flags to SDL1.2 */
    if (flags & SDL_WINDOW_FULLSCREEN) {
        window->fullscreen = 1;
    }

#if defined(__WEBOS__)
    /* webOS: Use pure OpenGL ES context like Palm's own samples do */
    fprintf(stderr, "[SDL2-compat] Setting SDL_GL_CONTEXT_MAJOR_VERSION = 1 (GLES 1.1)\n");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);

    fprintf(stderr, "[SDL2-compat] CreateWindow: requesting %dx%d with SDL_OPENGL|SDL_FULLSCREEN\n", w, h);
    screen = SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_FULLSCREEN);
    if (!screen) {
        fprintf(stderr, "[SDL2-compat] CreateWindow: SDL_SetVideoMode FAILED: %s\n", SDL_GetError());
        free(window);
        return NULL;
    }
    window->screen = screen;
    fprintf(stderr, "[SDL2-compat] CreateWindow: got GL context, screen=%p\n", (void*)screen);

    /* Force GL context to be fully initialized before proceeding */
    glFinish();
    fprintf(stderr, "[SDL2-compat] CreateWindow: GL context synchronized\n");

    /* Create a software surface for the renderer to draw to */
    /* Use RGBA format that matches GL_RGBA/GL_UNSIGNED_BYTE */
    window->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
                                           0x000000FF,  /* R mask */
                                           0x0000FF00,  /* G mask */
                                           0x00FF0000,  /* B mask */
                                           0xFF000000); /* A mask */
    if (!window->surface) {
        fprintf(stderr, "[SDL2-compat] CreateWindow: Failed to create software surface: %s\n", SDL_GetError());
        free(window);
        return NULL;
    }
    fprintf(stderr, "[SDL2-compat] CreateWindow: created software surface %dx%d bpp=%d\n",
            window->surface->w, window->surface->h, window->surface->format->BitsPerPixel);
#else
    Uint32 sdl_flags = SDL_SWSURFACE;
    if (window->fullscreen) {
        sdl_flags |= SDL_FULLSCREEN;
    }
    window->surface = SDL_SetVideoMode(w, h, 0, sdl_flags);
    window->screen = window->surface;
    if (!window->surface) {
        fprintf(stderr, "[SDL2-compat] CreateWindow: SDL_SetVideoMode FAILED: %s\n", SDL_GetError());
        free(window);
        return NULL;
    }
#endif

    window->w = w;
    window->h = h;
    window->flags = flags;

    if (title) {
        window->title = strdup(title);
        SDL_WM_SetCaption(title, title);
    }

    g_window = window;
    return window;
}

void SDL_DestroyWindow(SDL_Window *window) {
    if (window) {
#if defined(__WEBOS__)
        if (window->surface && window->surface != window->screen) {
            SDL_FreeSurface(window->surface);
        }
#endif
        if (window->title) {
            free(window->title);
        }
        free(window);
        if (g_window == window) {
            g_window = NULL;
        }
    }
}

void SDL_SetWindowTitle(SDL_Window *window, const char *title) {
    if (window) {
        if (window->title) {
            free(window->title);
        }
        window->title = title ? strdup(title) : NULL;
        SDL_WM_SetCaption(title, title);
    }
}

void SDL_SetWindowIcon(SDL_Window *window, SDL_Surface *icon) {
    (void)window;
    SDL_WM_SetIcon(icon, NULL);
}

void SDL_SetWindowSize(SDL_Window *window, int w, int h) {
    if (!window) return;

#if defined(__WEBOS__)
    /* On webOS, recreate the software surface with new size */
    if (window->surface) {
        SDL_FreeSurface(window->surface);
    }
    window->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
                                           0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    if (window->surface) {
        window->w = w;
        window->h = h;
    }
#else
    Uint32 sdl_flags = SDL_SWSURFACE;
    if (window->fullscreen) {
        sdl_flags |= SDL_FULLSCREEN;
    }
    window->surface = SDL_SetVideoMode(w, h, 0, sdl_flags);
    window->screen = window->surface;
    if (window->surface) {
        window->w = w;
        window->h = h;
    }
#endif
}

void SDL_GetWindowSize(SDL_Window *window, int *w, int *h) {
    if (window) {
        if (w) *w = window->w;
        if (h) *h = window->h;
    }
}

int SDL_SetWindowFullscreen(SDL_Window *window, Uint32 flags) {
    if (!window) return -1;

    window->fullscreen = (flags & SDL_WINDOW_FULLSCREEN) ? 1 : 0;

#if defined(__WEBOS__)
    /* On webOS, always fullscreen with GL context - nothing to change */
    return 0;
#else
    Uint32 sdl_flags = SDL_SWSURFACE;
    if (window->fullscreen) {
        sdl_flags |= SDL_FULLSCREEN;
    }
    window->surface = SDL_SetVideoMode(window->w, window->h, 0, sdl_flags);
    window->screen = window->surface;
    return window->surface ? 0 : -1;
#endif
}

Uint32 SDL_GetWindowFlags(SDL_Window *window) {
    Uint32 flags = 0;
    Uint8 state;

    if (!window) return 0;

    state = SDL_GetAppState();

    if (state & SDL_APPACTIVE) {
        flags |= SDL_WINDOW_SHOWN;
    } else {
        flags |= SDL_WINDOW_MINIMIZED;
    }

    if (state & SDL_APPINPUTFOCUS) {
        flags |= SDL_WINDOW_INPUT_FOCUS;
    }

    if (state & SDL_APPMOUSEFOCUS) {
        flags |= SDL_WINDOW_MOUSE_FOCUS;
    }

    if (window->fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    return flags;
}

int SDL_GetWindowDisplayIndex(SDL_Window *window) {
    (void)window;
    return 0; /* SDL1.2 doesn't have multiple displays */
}

int SDL_GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode *mode) {
    const SDL_VideoInfo *info;

    (void)displayIndex;

    if (!mode) return -1;

    info = SDL_GetVideoInfo();
    if (!info) return -1;

    mode->w = info->current_w;
    mode->h = info->current_h;
    mode->refresh_rate = 0; /* Not available in SDL1.2 */
    mode->format = 0;
    mode->driverdata = NULL;

    return 0;
}

int SDL_GetNumVideoDisplays(void) {
    return 1; /* SDL1.2 only supports one display */
}

/* Internal helper to get screen surface */
SDL_Surface* SDL2_GetWindowSurface(SDL_Window *window) {
    fprintf(stderr, "[SDL2-compat] GetWindowSurface called, window=%p\n", (void*)window);
    return window ? window->surface : NULL;
}
