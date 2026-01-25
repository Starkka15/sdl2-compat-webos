/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Renderer subsystem - SDL_Renderer and SDL_Texture
 */
#ifndef SDL2_COMPAT_SDL_RENDER_H
#define SDL2_COMPAT_SDL_RENDER_H

#include <SDL/SDL.h>
#include "SDL_video.h"
#include "SDL_blendmode.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Renderer flags */
#define SDL_RENDERER_SOFTWARE      0x00000001
#define SDL_RENDERER_ACCELERATED   0x00000002
#define SDL_RENDERER_PRESENTVSYNC  0x00000004
#define SDL_RENDERER_TARGETTEXTURE 0x00000008

/* Texture access patterns */
typedef enum {
    SDL_TEXTUREACCESS_STATIC,
    SDL_TEXTUREACCESS_STREAMING,
    SDL_TEXTUREACCESS_TARGET
} SDL_TextureAccess;

/* Flip flags for SDL_RenderCopyEx */
typedef enum {
    SDL_FLIP_NONE       = 0x00000000,
    SDL_FLIP_HORIZONTAL = 0x00000001,
    SDL_FLIP_VERTICAL   = 0x00000002
} SDL_RendererFlip;

/* SDL_Point for SDL_RenderCopyEx */
typedef struct SDL_Point {
    int x;
    int y;
} SDL_Point;

/* Opaque types - internally backed by SDL1.2 surfaces */
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;

/* Renderer info */
typedef struct SDL_RendererInfo {
    const char *name;
    Uint32 flags;
    Uint32 num_texture_formats;
    Uint32 texture_formats[16];
    int max_texture_width;
    int max_texture_height;
} SDL_RendererInfo;

/* Renderer creation/destruction */
SDL_Renderer* SDL_CreateRenderer(SDL_Window *window, int index, Uint32 flags);
void SDL_DestroyRenderer(SDL_Renderer *renderer);
int SDL_GetRendererInfo(SDL_Renderer *renderer, SDL_RendererInfo *info);

/* Texture creation/destruction */
SDL_Texture* SDL_CreateTexture(SDL_Renderer *renderer, Uint32 format,
                               int access, int w, int h);
SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface);
void SDL_DestroyTexture(SDL_Texture *texture);

/* Texture modification */
int SDL_SetTextureColorMod(SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b);
int SDL_GetTextureColorMod(SDL_Texture *texture, Uint8 *r, Uint8 *g, Uint8 *b);
int SDL_SetTextureAlphaMod(SDL_Texture *texture, Uint8 alpha);
int SDL_GetTextureAlphaMod(SDL_Texture *texture, Uint8 *alpha);
int SDL_SetTextureBlendMode(SDL_Texture *texture, SDL_BlendMode blendMode);
int SDL_QueryTexture(SDL_Texture *texture, Uint32 *format, int *access, int *w, int *h);
int SDL_UpdateTexture(SDL_Texture *texture, const SDL_Rect *rect,
                      const void *pixels, int pitch);
int SDL_LockTexture(SDL_Texture *texture, const SDL_Rect *rect, void **pixels, int *pitch);
void SDL_UnlockTexture(SDL_Texture *texture);

/* Render state */
int SDL_SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int SDL_GetRenderDrawColor(SDL_Renderer *renderer, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);
int SDL_SetRenderDrawBlendMode(SDL_Renderer *renderer, SDL_BlendMode blendMode);
int SDL_RenderSetClipRect(SDL_Renderer *renderer, const SDL_Rect *rect);
int SDL_RenderGetViewport(SDL_Renderer *renderer, SDL_Rect *rect);
int SDL_RenderSetViewport(SDL_Renderer *renderer, const SDL_Rect *rect);

/* Render operations */
int SDL_RenderClear(SDL_Renderer *renderer);
int SDL_RenderCopy(SDL_Renderer *renderer, SDL_Texture *texture,
                   const SDL_Rect *srcrect, const SDL_Rect *dstrect);
int SDL_RenderCopyEx(SDL_Renderer *renderer, SDL_Texture *texture,
                     const SDL_Rect *srcrect, const SDL_Rect *dstrect,
                     double angle, const SDL_Point *center, SDL_RendererFlip flip);
int SDL_RenderDrawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);
int SDL_RenderDrawPoint(SDL_Renderer *renderer, int x, int y);
int SDL_RenderDrawRect(SDL_Renderer *renderer, const SDL_Rect *rect);
int SDL_RenderFillRect(SDL_Renderer *renderer, const SDL_Rect *rect);
int SDL_RenderFillRects(SDL_Renderer *renderer, const SDL_Rect *rects, int count);
int SDL_RenderReadPixels(SDL_Renderer *renderer, const SDL_Rect *rect,
                         Uint32 format, void *pixels, int pitch);
void SDL_RenderPresent(SDL_Renderer *renderer);

#ifdef __cplusplus
}
#endif

#endif /* SDL2_COMPAT_SDL_RENDER_H */
