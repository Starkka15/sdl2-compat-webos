/*
 * SDL2 to SDL1.2 compatibility shim - OpenGL ES 2 header wrapper
 * Redirects to the actual GLES2 headers from the PDK
 * Also defines missing GLES2 extension types/constants
 */

#ifndef SDL_opengles2_h_
#define SDL_opengles2_h_

#include <GLES2/gl2.h>

/* GLchar type - may not be defined in older GLES2 headers */
#ifndef GLchar
typedef char GLchar;
#endif

/* Extension constants that may be missing */
#ifndef GL_HALF_FLOAT_OES
#define GL_HALF_FLOAT_OES                 0x8D61
#endif

#ifndef GL_TEXTURE_3D_OES
#define GL_TEXTURE_3D_OES                 0x806F
#endif

#ifndef GL_R8_EXT
#define GL_R8_EXT                         0x8229
#endif

#ifndef GL_RED_EXT
#define GL_RED_EXT                        0x1903
#endif

#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#endif

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

#ifndef GL_TEXTURE_MAX_LEVEL_APPLE
#define GL_TEXTURE_MAX_LEVEL_APPLE        0x813D
#endif

#ifndef GL_COLOR_EXT
#define GL_COLOR_EXT                      0x1800
#endif

#ifndef GL_DEPTH_EXT
#define GL_DEPTH_EXT                      0x1801
#endif

#ifndef GL_PROGRAM_BINARY_LENGTH_OES
#define GL_PROGRAM_BINARY_LENGTH_OES      0x8741
#endif

/* OES/EXT function pointer types */
typedef void (GL_APIENTRY *PFNGLGENVERTEXARRAYSOESPROC)(GLsizei n, GLuint *arrays);
typedef void (GL_APIENTRY *PFNGLDELETEVERTEXARRAYSOESPROC)(GLsizei n, const GLuint *arrays);
typedef void (GL_APIENTRY *PFNGLBINDVERTEXARRAYOESPROC)(GLuint array);
typedef void (GL_APIENTRY *PFNGLGETPROGRAMBINARYOESPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
typedef void (GL_APIENTRY *PFNGLPROGRAMBINARYOESPROC)(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
typedef void (GL_APIENTRY *PFNGLDISCARDFRAMEBUFFEREXTPROC)(GLenum target, GLsizei numAttachments, const GLenum *attachments);

/* Stub function for discard framebuffer - not available on all GLES2 implementations */
static inline void glDiscardFramebufferEXT(GLenum target, GLsizei numAttachments, const GLenum *attachments) {
    (void)target;
    (void)numAttachments;
    (void)attachments;
    /* No-op - discard is just an optimization hint */
}

#endif /* SDL_opengles2_h_ */
