/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Message box functions
 */
#ifndef SDL2_COMPAT_SDL_MESSAGEBOX_H
#define SDL2_COMPAT_SDL_MESSAGEBOX_H

#include <SDL/SDL.h>

/* Forward declaration */
struct SDL_Window;

#ifdef __cplusplus
extern "C" {
#endif

/* Message box flags */
#define SDL_MESSAGEBOX_ERROR        0x00000010
#define SDL_MESSAGEBOX_WARNING      0x00000020
#define SDL_MESSAGEBOX_INFORMATION  0x00000040

/* SDL2 simple message box - not in SDL1.2 */
int SDL_ShowSimpleMessageBox(Uint32 flags, const char *title,
                             const char *message, struct SDL_Window *window);

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_MESSAGEBOX_H */
