/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Video subsystem - Window management
 */
#ifndef SDL2_COMPAT_SDL_VIDEO_H
#define SDL2_COMPAT_SDL_VIDEO_H

#include <SDL/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SDL2 Window flags */
#define SDL_WINDOW_FULLSCREEN    0x00000001
#define SDL_WINDOW_SHOWN         0x00000004
#define SDL_WINDOW_HIDDEN        0x00000008
#define SDL_WINDOW_BORDERLESS    0x00000010
#define SDL_WINDOW_RESIZABLE     0x00000020
#define SDL_WINDOW_MINIMIZED     0x00000040
#define SDL_WINDOW_MAXIMIZED     0x00000080
#define SDL_WINDOW_INPUT_GRABBED 0x00000100
#define SDL_WINDOW_INPUT_FOCUS   0x00000200
#define SDL_WINDOW_MOUSE_FOCUS   0x00000400
#define SDL_WINDOW_FULLSCREEN_DESKTOP (SDL_WINDOW_FULLSCREEN | 0x00001000)
#define SDL_WINDOW_OPENGL        0x00000002

/* Window position macros */
#define SDL_WINDOWPOS_UNDEFINED_MASK  0x1FFF0000
#define SDL_WINDOWPOS_CENTERED_MASK   0x2FFF0000
#define SDL_WINDOWPOS_UNDEFINED       SDL_WINDOWPOS_UNDEFINED_MASK
#define SDL_WINDOWPOS_CENTERED        SDL_WINDOWPOS_CENTERED_MASK

/* SDL_Window - opaque handle, actually points to our internal struct */
typedef struct SDL_Window SDL_Window;

/* Display mode */
typedef struct SDL_DisplayMode {
    Uint32 format;
    int w;
    int h;
    int refresh_rate;
    void *driverdata;
} SDL_DisplayMode;

/* Window functions */
SDL_Window* SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
void SDL_DestroyWindow(SDL_Window *window);
void SDL_SetWindowTitle(SDL_Window *window, const char *title);
void SDL_SetWindowIcon(SDL_Window *window, SDL_Surface *icon);
void SDL_SetWindowSize(SDL_Window *window, int w, int h);
void SDL_GetWindowSize(SDL_Window *window, int *w, int *h);
int SDL_SetWindowFullscreen(SDL_Window *window, Uint32 flags);
Uint32 SDL_GetWindowFlags(SDL_Window *window);
int SDL_GetWindowDisplayIndex(SDL_Window *window);

/* Display functions */
int SDL_GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode *mode);
int SDL_GetNumVideoDisplays(void);

/* Internal: get the screen surface for a window */
SDL_Surface* SDL2_GetWindowSurface(SDL_Window *window);

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_VIDEO_H */
