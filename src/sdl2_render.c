/*
 * SDL2-to-SDL1.2 Compatibility Shim
 * Renderer implementation - uses OpenGL ES for hardware acceleration on webOS
 */
#include <SDL/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/SDL2/SDL_video.h"
#include "../include/SDL2/SDL_render.h"
#include "../include/SDL2/SDL_pixels.h"

#if defined(__WEBOS__)
#include <GLES/gl.h>
#define USE_GL_TEXTURES 1
#else
#define USE_GL_TEXTURES 0
#endif

/* Internal renderer structure */
struct SDL_Renderer {
    SDL_Window *window;
    SDL_Surface *target;  /* Software surface for non-GL fallback */
    int screen_w, screen_h;
    Uint8 r, g, b, a;     /* Current draw color */
    SDL_BlendMode blendMode;
    SDL_Rect viewport;
    SDL_Rect *clipRect;
#if USE_GL_TEXTURES
    int gl_initialized;
#endif
};

/* Internal texture structure */
struct SDL_Texture {
    SDL_Surface *surface;   /* Software surface (kept for UpdateTexture) */
    SDL_Surface *original;  /* Original unmodified surface (for color/alpha mod) */
    int w, h;
    int pot_w, pot_h;       /* Power-of-two dimensions for GL */
    Uint32 format;
    int access;
    Uint8 r, g, b;         /* Color modulation */
    Uint8 alpha;           /* Alpha modulation */
    SDL_BlendMode blendMode;
    int colorModDirty;     /* Need to regenerate surface with color mod */
#if USE_GL_TEXTURES
    GLuint gl_texture;      /* OpenGL texture ID */
    GLfloat max_u, max_v;   /* Texture coordinates for non-POT content */
#endif
};

/* External function from video module */
extern SDL_Surface* SDL2_GetWindowSurface(SDL_Window *window);

/* Helper: apply color modulation to a surface */
static void ApplyColorMod(SDL_Texture *texture) {
    int x, y;
    Uint8 *src_pixels, *dst_pixels;
    SDL_Surface *src, *dst;

    if (!texture->colorModDirty) return;
    if (!texture->original) return;

    src = texture->original;
    dst = texture->surface;

    if (SDL_MUSTLOCK(src)) SDL_LockSurface(src);
    if (SDL_MUSTLOCK(dst)) SDL_LockSurface(dst);

    src_pixels = (Uint8 *)src->pixels;
    dst_pixels = (Uint8 *)dst->pixels;

    /* Simple per-pixel color modulation for 32-bit surfaces */
    if (src->format->BytesPerPixel == 4) {
        for (y = 0; y < src->h; y++) {
            Uint32 *src_row = (Uint32 *)(src_pixels + y * src->pitch);
            Uint32 *dst_row = (Uint32 *)(dst_pixels + y * dst->pitch);
            for (x = 0; x < src->w; x++) {
                Uint8 r, g, b, a;
                SDL_GetRGBA(src_row[x], src->format, &r, &g, &b, &a);
                r = (r * texture->r) / 255;
                g = (g * texture->g) / 255;
                b = (b * texture->b) / 255;
                a = (a * texture->alpha) / 255;
                dst_row[x] = SDL_MapRGBA(dst->format, r, g, b, a);
            }
        }
    } else {
        /* For non-32bit, just copy */
        memcpy(dst_pixels, src_pixels, src->h * src->pitch);
    }

    if (SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
    if (SDL_MUSTLOCK(dst)) SDL_UnlockSurface(dst);

    texture->colorModDirty = 0;
}

SDL_Renderer* SDL_CreateRenderer(SDL_Window *window, int index, Uint32 flags) {
    SDL_Renderer *renderer;

    (void)index;
    (void)flags;

    fprintf(stderr, "[SDL2-compat] CreateRenderer: window=%p\n", (void*)window);

    if (!window) return NULL;

    renderer = (SDL_Renderer *)malloc(sizeof(SDL_Renderer));
    if (!renderer) {
        SDL_SetError("Out of memory");
        return NULL;
    }

    memset(renderer, 0, sizeof(SDL_Renderer));

    renderer->window = window;
    renderer->target = SDL2_GetWindowSurface(window);
    renderer->r = renderer->g = renderer->b = 0;
    renderer->a = 255;
    renderer->blendMode = SDL_BLENDMODE_NONE;
    renderer->clipRect = NULL;

    if (renderer->target) {
        renderer->screen_w = renderer->target->w;
        renderer->screen_h = renderer->target->h;
        renderer->viewport.x = 0;
        renderer->viewport.y = 0;
        renderer->viewport.w = renderer->target->w;
        renderer->viewport.h = renderer->target->h;
        fprintf(stderr, "[SDL2-compat] CreateRenderer: target surface %dx%d, flags=0x%x\n",
                renderer->target->w, renderer->target->h, renderer->target->flags);
    } else {
        fprintf(stderr, "[SDL2-compat] CreateRenderer: WARNING - no target surface!\n");
    }

#if USE_GL_TEXTURES
    /* Initialize OpenGL ES state */
    fprintf(stderr, "[SDL2-compat] Initializing GL state for hardware rendering\n");

    glViewport(0, 0, renderer->screen_w, renderer->screen_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* Set up orthographic projection with (0,0) at top-left */
    glOrthof(0, renderer->screen_w, renderer->screen_h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    /* Enable blending for transparency */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Set default clear color to black */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    /* Force all GL commands to complete before continuing */
    glFinish();

    renderer->gl_initialized = 1;
    fprintf(stderr, "[SDL2-compat] GL state initialized, screen %dx%d\n",
            renderer->screen_w, renderer->screen_h);
#endif

    return renderer;
}

void SDL_DestroyRenderer(SDL_Renderer *renderer) {
    if (renderer) {
        if (renderer->clipRect) {
            free(renderer->clipRect);
        }
        free(renderer);
    }
}

int SDL_GetRendererInfo(SDL_Renderer *renderer, SDL_RendererInfo *info) {
    if (!renderer || !info) return -1;

    info->name = "SDL1.2 Software Renderer";
    info->flags = SDL_RENDERER_SOFTWARE;
    info->num_texture_formats = 1;
    info->texture_formats[0] = SDL_PIXELFORMAT_ARGB8888;
    info->max_texture_width = 4096;
    info->max_texture_height = 4096;

    return 0;
}

/* Helper: compute next power of two */
static int next_pot(int n) {
    int pot = 1;
    while (pot < n) pot <<= 1;
    return pot;
}

SDL_Texture* SDL_CreateTexture(SDL_Renderer *renderer, Uint32 format,
                               int access, int w, int h) {
    SDL_Texture *texture;

    (void)renderer;
    (void)format;

    texture = (SDL_Texture *)malloc(sizeof(SDL_Texture));
    if (!texture) {
        SDL_SetError("Out of memory");
        return NULL;
    }

    memset(texture, 0, sizeof(SDL_Texture));

    /* Use RGBA format to match GL_RGBA */
    texture->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
                                            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    if (!texture->surface) {
        free(texture);
        return NULL;
    }

    /* Fill with transparent black */
    SDL_FillRect(texture->surface, NULL, 0);

    texture->w = w;
    texture->h = h;
    texture->format = format;
    texture->access = access;
    texture->r = texture->g = texture->b = 255;
    texture->alpha = 255;
    texture->blendMode = SDL_BLENDMODE_NONE;

#if USE_GL_TEXTURES
    /* Create GL texture for this too */
    texture->pot_w = next_pot(w);
    texture->pot_h = next_pot(h);
    texture->max_u = (GLfloat)w / texture->pot_w;
    texture->max_v = (GLfloat)h / texture->pot_h;

    glGenTextures(1, &texture->gl_texture);
    glBindTexture(GL_TEXTURE_2D, texture->gl_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->pot_w, texture->pot_h,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#endif

    return texture;
}

static int tex_count = 0;
SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface) {
    SDL_Texture *texture;
    Uint32 rmask, gmask, bmask, amask;
    SDL_Surface *converted;

    (void)renderer;

    if (!surface) {
        fprintf(stderr, "[SDL2-compat] CreateTextureFromSurface #%d: NULL surface!\n", tex_count);
        return NULL;
    }
    if (tex_count < 50) {
        fprintf(stderr, "[SDL2-compat] CreateTextureFromSurface #%d: %dx%d bpp=%d colorkey=%d",
                tex_count, surface->w, surface->h, surface->format->BitsPerPixel,
                (surface->flags & SDL_SRCCOLORKEY) ? 1 : 0);
        if (surface->flags & SDL_SRCCOLORKEY) {
            Uint8 r, g, b;
            SDL_GetRGB(surface->format->colorkey, surface->format, &r, &g, &b);
            fprintf(stderr, " ck_rgb=(%d,%d,%d)", r, g, b);
        }
        fprintf(stderr, "\n");
    }
    tex_count++;

    texture = (SDL_Texture *)malloc(sizeof(SDL_Texture));
    if (!texture) {
        SDL_SetError("Out of memory");
        return NULL;
    }

    memset(texture, 0, sizeof(SDL_Texture));

    /* Use RGBA format (R in low bits) - matches GL_RGBA */
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;

    /* Create a 32-bit RGBA surface */
    converted = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, 32,
                                     rmask, gmask, bmask, amask);
    if (!converted) {
        free(texture);
        return NULL;
    }

    /* Fill with transparent black first */
    SDL_FillRect(converted, NULL, SDL_MapRGBA(converted->format, 0, 0, 0, 0));

    /* For colorkeyed surfaces, we need special handling */
    if (surface->flags & SDL_SRCCOLORKEY) {
        /* Disable alpha blending but keep colorkey for the blit */
        SDL_SetAlpha(surface, 0, SDL_ALPHA_OPAQUE);
        /* The blit should skip colorkey pixels, leaving them as transparent black */
    } else {
        SDL_SetAlpha(surface, 0, SDL_ALPHA_OPAQUE);
    }

    SDL_BlitSurface(surface, NULL, converted, NULL);

    texture->surface = converted;
    texture->original = NULL;
    texture->w = surface->w;
    texture->h = surface->h;
    texture->format = SDL_PIXELFORMAT_ARGB8888;
    texture->access = SDL_TEXTUREACCESS_STATIC;
    texture->r = texture->g = texture->b = 255;
    texture->alpha = 255;
    texture->blendMode = SDL_BLENDMODE_BLEND;
    texture->colorModDirty = 0;

#if USE_GL_TEXTURES
    /* Calculate power-of-two dimensions for GLES 1.1 */
    texture->pot_w = next_pot(surface->w);
    texture->pot_h = next_pot(surface->h);
    texture->max_u = (GLfloat)surface->w / texture->pot_w;
    texture->max_v = (GLfloat)surface->h / texture->pot_h;

    /* Create GL texture */
    glGenTextures(1, &texture->gl_texture);
    glBindTexture(GL_TEXTURE_2D, texture->gl_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Allocate POT texture with transparent pixels to avoid black borders */
    {
        size_t pot_size = texture->pot_w * texture->pot_h * 4;
        void *clear_pixels = calloc(1, pot_size);
        if (clear_pixels) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->pot_w, texture->pot_h,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, clear_pixels);
            free(clear_pixels);
        } else {
            /* Fallback to uninitialized if allocation fails */
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->pot_w, texture->pot_h,
                         0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        }
    }

    /* Upload actual texture data */
    if (SDL_MUSTLOCK(converted)) SDL_LockSurface(converted);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, surface->w, surface->h,
                    GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);
    if (SDL_MUSTLOCK(converted)) SDL_UnlockSurface(converted);

    if (tex_count <= 30) {
        fprintf(stderr, "[SDL2-compat] Created GL texture %u: %dx%d (POT %dx%d)\n",
                texture->gl_texture, surface->w, surface->h, texture->pot_w, texture->pot_h);
    }
#endif

    return texture;
}

static int destroy_count = 0;
void SDL_DestroyTexture(SDL_Texture *texture) {
    if (texture) {
        if (destroy_count < 20) {
            fprintf(stderr, "[SDL2-compat] DestroyTexture #%d: %dx%d gl=%u\n",
                    destroy_count, texture->w, texture->h, texture->gl_texture);
        }
        destroy_count++;
#if USE_GL_TEXTURES
        if (texture->gl_texture) {
            glDeleteTextures(1, &texture->gl_texture);
        }
#endif
        if (texture->surface) {
            SDL_FreeSurface(texture->surface);
        }
        if (texture->original) {
            SDL_FreeSurface(texture->original);
        }
        free(texture);
    }
}

int SDL_SetTextureColorMod(SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b) {
    if (!texture) return -1;

    if (texture->r != r || texture->g != g || texture->b != b) {
        texture->r = r;
        texture->g = g;
        texture->b = b;
        texture->colorModDirty = 1;
    }

    return 0;
}

int SDL_GetTextureColorMod(SDL_Texture *texture, Uint8 *r, Uint8 *g, Uint8 *b) {
    if (!texture) return -1;

    if (r) *r = texture->r;
    if (g) *g = texture->g;
    if (b) *b = texture->b;

    return 0;
}

int SDL_SetTextureAlphaMod(SDL_Texture *texture, Uint8 alpha) {
    if (!texture) return -1;

    if (texture->alpha != alpha) {
        texture->alpha = alpha;
        texture->colorModDirty = 1;
    }

    return 0;
}

int SDL_GetTextureAlphaMod(SDL_Texture *texture, Uint8 *alpha) {
    if (!texture) return -1;

    if (alpha) *alpha = texture->alpha;

    return 0;
}

int SDL_SetTextureBlendMode(SDL_Texture *texture, SDL_BlendMode blendMode) {
    if (!texture) return -1;

    texture->blendMode = blendMode;

    return 0;
}

int SDL_QueryTexture(SDL_Texture *texture, Uint32 *format, int *access, int *w, int *h) {
    if (!texture) return -1;

    if (format) *format = texture->format;
    if (access) *access = texture->access;
    if (w) *w = texture->w;
    if (h) *h = texture->h;

    return 0;
}

int SDL_UpdateTexture(SDL_Texture *texture, const SDL_Rect *rect,
                      const void *pixels, int pitch) {
    SDL_Rect dstrect;
    int y;
    Uint8 *src, *dst;

    if (!texture || !texture->surface) return -1;

    if (rect) {
        dstrect = *rect;
    } else {
        dstrect.x = 0;
        dstrect.y = 0;
        dstrect.w = texture->w;
        dstrect.h = texture->h;
    }

    if (SDL_MUSTLOCK(texture->surface)) {
        SDL_LockSurface(texture->surface);
    }

    src = (Uint8 *)pixels;
    dst = (Uint8 *)texture->surface->pixels +
          dstrect.y * texture->surface->pitch +
          dstrect.x * texture->surface->format->BytesPerPixel;

    for (y = 0; y < dstrect.h; y++) {
        memcpy(dst, src, dstrect.w * texture->surface->format->BytesPerPixel);
        src += pitch;
        dst += texture->surface->pitch;
    }

    if (SDL_MUSTLOCK(texture->surface)) {
        SDL_UnlockSurface(texture->surface);
    }

    return 0;
}

int SDL_LockTexture(SDL_Texture *texture, const SDL_Rect *rect, void **pixels, int *pitch) {
    (void)rect;

    if (!texture || !texture->surface) return -1;

    if (SDL_MUSTLOCK(texture->surface)) {
        SDL_LockSurface(texture->surface);
    }

    *pixels = texture->surface->pixels;
    *pitch = texture->surface->pitch;

    return 0;
}

void SDL_UnlockTexture(SDL_Texture *texture) {
    if (!texture || !texture->surface) return;

    if (SDL_MUSTLOCK(texture->surface)) {
        SDL_UnlockSurface(texture->surface);
    }
}

int SDL_SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (!renderer) return -1;

    renderer->r = r;
    renderer->g = g;
    renderer->b = b;
    renderer->a = a;

    return 0;
}

int SDL_GetRenderDrawColor(SDL_Renderer *renderer, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a) {
    if (!renderer) return -1;

    if (r) *r = renderer->r;
    if (g) *g = renderer->g;
    if (b) *b = renderer->b;
    if (a) *a = renderer->a;

    return 0;
}

int SDL_SetRenderDrawBlendMode(SDL_Renderer *renderer, SDL_BlendMode blendMode) {
    if (!renderer) return -1;

    renderer->blendMode = blendMode;

    return 0;
}

int SDL_RenderSetClipRect(SDL_Renderer *renderer, const SDL_Rect *rect) {
    if (!renderer) return -1;

    if (rect) {
        if (!renderer->clipRect) {
            renderer->clipRect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
        }
        if (renderer->clipRect) {
            *renderer->clipRect = *rect;
        }
    } else {
        if (renderer->clipRect) {
            free(renderer->clipRect);
            renderer->clipRect = NULL;
        }
    }

    return 0;
}

int SDL_RenderGetViewport(SDL_Renderer *renderer, SDL_Rect *rect) {
    if (!renderer || !rect) return -1;

    *rect = renderer->viewport;

    return 0;
}

int SDL_RenderSetViewport(SDL_Renderer *renderer, const SDL_Rect *rect) {
    if (!renderer) return -1;

    if (rect) {
        renderer->viewport = *rect;
    } else {
        renderer->viewport.x = 0;
        renderer->viewport.y = 0;
        renderer->viewport.w = renderer->screen_w;
        renderer->viewport.h = renderer->screen_h;
    }

    return 0;
}

static int clear_count = 0;
int SDL_RenderClear(SDL_Renderer *renderer) {
    if (!renderer) {
        fprintf(stderr, "[SDL2-compat] RenderClear #%d: NULL!\n", clear_count);
        return -1;
    }

    if (clear_count < 5) {
        fprintf(stderr, "[SDL2-compat] RenderClear #%d\n", clear_count);
    }
    clear_count++;

#if USE_GL_TEXTURES
    glClearColor(renderer->r / 255.0f, renderer->g / 255.0f,
                 renderer->b / 255.0f, renderer->a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
#else
    if (renderer->target) {
        Uint32 color = SDL_MapRGBA(renderer->target->format,
                                   renderer->r, renderer->g, renderer->b, renderer->a);
        SDL_FillRect(renderer->target, NULL, color);
    }
#endif

    return 0;
}

static int copy_count = 0;
int SDL_RenderCopy(SDL_Renderer *renderer, SDL_Texture *texture,
                   const SDL_Rect *srcrect, const SDL_Rect *dstrect) {
    SDL_Rect src, dst;

    if (!renderer) {
        fprintf(stderr, "[SDL2-compat] RenderCopy #%d: NULL renderer!\n", copy_count);
        return -1;
    }
    if (!texture) {
        fprintf(stderr, "[SDL2-compat] RenderCopy #%d: NULL texture!\n", copy_count);
        return -1;
    }

    if (copy_count < 50) {
        fprintf(stderr, "[SDL2-compat] RenderCopy #%d: tex %dx%d gl=%u\n",
                copy_count, texture->w, texture->h, texture->gl_texture);
    }
    copy_count++;

    /* Set up source rect */
    if (srcrect) {
        src = *srcrect;
    } else {
        src.x = 0;
        src.y = 0;
        src.w = texture->w;
        src.h = texture->h;
    }

    /* Set up dest rect */
    if (dstrect) {
        dst = *dstrect;
    } else {
        dst.x = 0;
        dst.y = 0;
        dst.w = texture->w;
        dst.h = texture->h;
    }

#if USE_GL_TEXTURES
    /* Check if GL texture is valid */
    if (texture->gl_texture == 0 || texture->pot_w == 0 || texture->pot_h == 0) {
        fprintf(stderr, "[SDL2-compat] RenderCopy: invalid GL texture! id=%u pot=%dx%d\n",
                texture->gl_texture, texture->pot_w, texture->pot_h);
        return -1;
    }
    {
        /* Calculate texture coordinates for the source rectangle */
        GLfloat u0 = (GLfloat)src.x / texture->pot_w;
        GLfloat v0 = (GLfloat)src.y / texture->pot_h;
        GLfloat u1 = (GLfloat)(src.x + src.w) / texture->pot_w;
        GLfloat v1 = (GLfloat)(src.y + src.h) / texture->pot_h;

        /* Destination coordinates */
        GLfloat x0 = (GLfloat)dst.x;
        GLfloat y0 = (GLfloat)dst.y;
        GLfloat x1 = (GLfloat)(dst.x + dst.w);
        GLfloat y1 = (GLfloat)(dst.y + dst.h);

        GLfloat vertices[] = {
            x0, y0,
            x1, y0,
            x1, y1,
            x0, y1
        };
        GLfloat texcoords[] = {
            u0, v0,
            u1, v0,
            u1, v1,
            u0, v1
        };

        /* Bind texture and set color modulation */
        glBindTexture(GL_TEXTURE_2D, texture->gl_texture);
        glColor4f(texture->r / 255.0f, texture->g / 255.0f,
                  texture->b / 255.0f, texture->alpha / 255.0f);

        /* Set up blending */
        if (texture->blendMode == SDL_BLENDMODE_BLEND) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        } else {
            glDisable(GL_BLEND);
        }

        /* Draw textured quad */
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
#else
    /* Software fallback */
    if (renderer->target && texture->surface) {
        if (texture->blendMode == SDL_BLENDMODE_BLEND) {
            SDL_SetAlpha(texture->surface, SDL_SRCALPHA, texture->alpha);
        } else {
            SDL_SetAlpha(texture->surface, 0, SDL_ALPHA_OPAQUE);
        }
        SDL_BlitSurface(texture->surface, &src, renderer->target, &dst);
    }
#endif

    return 0;
}

int SDL_RenderCopyEx(SDL_Renderer *renderer, SDL_Texture *texture,
                     const SDL_Rect *srcrect, const SDL_Rect *dstrect,
                     double angle, const SDL_Point *center, SDL_RendererFlip flip) {
    SDL_Rect src, dst;

    (void)angle;   /* Rotation not supported */
    (void)center;

    if (!renderer || !texture) return -1;

    /* Set up source rect */
    if (srcrect) {
        src = *srcrect;
    } else {
        src.x = 0;
        src.y = 0;
        src.w = texture->w;
        src.h = texture->h;
    }

    /* Set up dest rect */
    if (dstrect) {
        dst = *dstrect;
    } else {
        dst.x = 0;
        dst.y = 0;
        dst.w = texture->w;
        dst.h = texture->h;
    }

#if USE_GL_TEXTURES
    if (texture->gl_texture == 0 || texture->pot_w == 0 || texture->pot_h == 0) {
        return -1;
    }
    {
        /* Calculate texture coordinates */
        GLfloat u0 = (GLfloat)src.x / texture->pot_w;
        GLfloat v0 = (GLfloat)src.y / texture->pot_h;
        GLfloat u1 = (GLfloat)(src.x + src.w) / texture->pot_w;
        GLfloat v1 = (GLfloat)(src.y + src.h) / texture->pot_h;

        /* Handle horizontal flip by swapping u coordinates */
        if (flip & SDL_FLIP_HORIZONTAL) {
            GLfloat tmp = u0;
            u0 = u1;
            u1 = tmp;
        }

        /* Handle vertical flip by swapping v coordinates */
        if (flip & SDL_FLIP_VERTICAL) {
            GLfloat tmp = v0;
            v0 = v1;
            v1 = tmp;
        }

        /* Destination coordinates */
        GLfloat x0 = (GLfloat)dst.x;
        GLfloat y0 = (GLfloat)dst.y;
        GLfloat x1 = (GLfloat)(dst.x + dst.w);
        GLfloat y1 = (GLfloat)(dst.y + dst.h);

        GLfloat vertices[] = {
            x0, y0,
            x1, y0,
            x1, y1,
            x0, y1
        };
        GLfloat texcoords[] = {
            u0, v0,
            u1, v0,
            u1, v1,
            u0, v1
        };

        glBindTexture(GL_TEXTURE_2D, texture->gl_texture);
        glColor4f(texture->r / 255.0f, texture->g / 255.0f,
                  texture->b / 255.0f, texture->alpha / 255.0f);

        if (texture->blendMode == SDL_BLENDMODE_BLEND) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        } else {
            glDisable(GL_BLEND);
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
#else
    /* Software fallback - not implemented for flips */
    if (renderer->target && texture->surface) {
        SDL_BlitSurface(texture->surface, &src, renderer->target, &dst);
    }
#endif

    return 0;
}

int SDL_RenderDrawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
    if (!renderer) return -1;

#if USE_GL_TEXTURES
    {
        GLfloat vertices[] = {
            (GLfloat)x1, (GLfloat)y1,
            (GLfloat)x2, (GLfloat)y2
        };

        glDisable(GL_TEXTURE_2D);
        glColor4f(renderer->r / 255.0f, renderer->g / 255.0f,
                  renderer->b / 255.0f, renderer->a / 255.0f);

        if (renderer->a < 255) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_LINES, 0, 2);
        glDisableClientState(GL_VERTEX_ARRAY);

        glEnable(GL_TEXTURE_2D);
    }
    return 0;
#else
    /* Simple line drawing - Bresenham's algorithm */
    int dx, dy, sx, sy, err, e2;
    Uint32 color;

    if (!renderer->target) return -1;

    color = SDL_MapRGBA(renderer->target->format,
                        renderer->r, renderer->g, renderer->b, renderer->a);

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    sx = (x1 < x2) ? 1 : -1;
    sy = (y1 < y2) ? 1 : -1;
    err = dx - dy;

    while (1) {
        /* Draw pixel */
        SDL_Rect pixel = {x1, y1, 1, 1};
        SDL_FillRect(renderer->target, &pixel, color);

        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    return 0;
#endif
}

int SDL_RenderDrawPoint(SDL_Renderer *renderer, int x, int y) {
    if (!renderer) return -1;

#if USE_GL_TEXTURES
    /* TODO: implement GL point drawing */
    (void)x; (void)y;
    return 0;
#else
    {
        SDL_Rect pixel;
        Uint32 color;

        if (!renderer->target) return -1;

        pixel.x = x;
        pixel.y = y;
        pixel.w = 1;
        pixel.h = 1;

        color = SDL_MapRGBA(renderer->target->format,
                            renderer->r, renderer->g, renderer->b, renderer->a);
        SDL_FillRect(renderer->target, &pixel, color);
    }
    return 0;
#endif
}

int SDL_RenderDrawRect(SDL_Renderer *renderer, const SDL_Rect *rect) {
    if (!renderer) return -1;

#if USE_GL_TEXTURES
    /* TODO: implement GL rect outline drawing */
    (void)rect;
    return 0;
#else
    {
        SDL_Rect line;
        Uint32 color;

        if (!renderer->target) return -1;

        color = SDL_MapRGBA(renderer->target->format,
                            renderer->r, renderer->g, renderer->b, renderer->a);

        if (rect) {
            /* Top */
            line.x = rect->x; line.y = rect->y;
            line.w = rect->w; line.h = 1;
            SDL_FillRect(renderer->target, &line, color);
            /* Bottom */
            line.y = rect->y + rect->h - 1;
            SDL_FillRect(renderer->target, &line, color);
            /* Left */
            line.x = rect->x; line.y = rect->y;
            line.w = 1; line.h = rect->h;
            SDL_FillRect(renderer->target, &line, color);
            /* Right */
            line.x = rect->x + rect->w - 1;
            SDL_FillRect(renderer->target, &line, color);
        }
    }
    return 0;
#endif
}

int SDL_RenderFillRect(SDL_Renderer *renderer, const SDL_Rect *rect) {
    SDL_Rect fill;

    if (!renderer) return -1;

    if (rect) {
        fill = *rect;
    } else {
        fill.x = 0;
        fill.y = 0;
        fill.w = renderer->screen_w;
        fill.h = renderer->screen_h;
    }

#if USE_GL_TEXTURES
    {
        GLfloat x0 = (GLfloat)fill.x;
        GLfloat y0 = (GLfloat)fill.y;
        GLfloat x1 = (GLfloat)(fill.x + fill.w);
        GLfloat y1 = (GLfloat)(fill.y + fill.h);

        GLfloat vertices[] = {
            x0, y0,
            x1, y0,
            x1, y1,
            x0, y1
        };

        glDisable(GL_TEXTURE_2D);
        glColor4f(renderer->r / 255.0f, renderer->g / 255.0f,
                  renderer->b / 255.0f, renderer->a / 255.0f);

        if (renderer->a < 255) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        } else {
            glDisable(GL_BLEND);
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);

        glEnable(GL_TEXTURE_2D);
    }
#else
    if (renderer->target) {
        Uint32 color = SDL_MapRGBA(renderer->target->format,
                                   renderer->r, renderer->g, renderer->b, renderer->a);
        SDL_FillRect(renderer->target, &fill, color);
    }
#endif

    return 0;
}

int SDL_RenderFillRects(SDL_Renderer *renderer, const SDL_Rect *rects, int count) {
    int i;

    if (!renderer || !rects) return -1;

    for (i = 0; i < count; i++) {
        SDL_RenderFillRect(renderer, &rects[i]);
    }

    return 0;
}

int SDL_RenderReadPixels(SDL_Renderer *renderer, const SDL_Rect *rect,
                         Uint32 format, void *pixels, int pitch) {
    (void)format;

    fprintf(stderr, "[SDL2-compat] RenderReadPixels called! renderer=%p\n", (void*)renderer);

    if (!renderer || !pixels) return -1;

#if USE_GL_TEXTURES
    /* Can't read pixels from GL in this shim - return black */
    fprintf(stderr, "[SDL2-compat] RenderReadPixels: GL mode, returning zeros\n");
    {
        int h = rect ? rect->h : renderer->screen_h;
        memset(pixels, 0, pitch * h);
    }
    return -1;
#else
    {
        SDL_Rect src;
        int y;
        Uint8 *dst;

        if (!renderer->target) return -1;

        /* Check if surface can be locked - OPENGLESBLIT surfaces may not support this */
        if (renderer->target->flags & (SDL_OPENGL | SDL_OPENGLBLIT)) {
            fprintf(stderr, "[SDL2-compat] RenderReadPixels: GL surface, returning zeros\n");
            /* Can't read pixels from GL surface in SDL1.2 */
            memset(pixels, 0, pitch * (rect ? rect->h : renderer->target->h));
            return -1;
        }

        if (rect) {
            src = *rect;
        } else {
            src.x = 0;
            src.y = 0;
            src.w = renderer->target->w;
            src.h = renderer->target->h;
        }

        if (SDL_MUSTLOCK(renderer->target)) {
            if (SDL_LockSurface(renderer->target) < 0) {
                return -1;
            }
        }

        dst = (Uint8 *)pixels;
        for (y = 0; y < src.h; y++) {
            Uint8 *src_row = (Uint8 *)renderer->target->pixels +
                             (src.y + y) * renderer->target->pitch +
                             src.x * renderer->target->format->BytesPerPixel;
            memcpy(dst, src_row, src.w * renderer->target->format->BytesPerPixel);
            dst += pitch;
        }

        if (SDL_MUSTLOCK(renderer->target)) {
            SDL_UnlockSurface(renderer->target);
        }
    }
    return 0;
#endif
}

void SDL_RenderPresent(SDL_Renderer *renderer) {
    static int frame_count = 0;

    if (!renderer) {
        return;
    }

    if (frame_count < 20) {
        fprintf(stderr, "[SDL2-compat] RenderPresent frame %d\n", frame_count);
    }

#if USE_GL_TEXTURES
    /* All rendering was done via GL in RenderCopy - just swap buffers */
    SDL_GL_SwapBuffers();
#else
    if (renderer->target) {
        SDL_Flip(renderer->target);
    }
#endif

    frame_count++;
}
