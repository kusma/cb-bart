#ifndef _EXTENSIONS_H_
#define _EXTENSIONS_H_
#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#ifdef _WIN32
#include <GL/wglext.h>
#endif

bool init_extensions();

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv;

#ifdef _WIN32
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;
PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;
PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB;
PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB;
#endif

PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT;

#ifdef __cplusplus
}
#endif
#endif /* _EXTENSIONS_H_ */

