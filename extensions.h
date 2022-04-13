#ifndef _EXTENSIONS_H_
#define _EXTENSIONS_H_
#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

bool init_extensions();

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv;

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;
PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;
PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB;
PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB;

PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT;

#ifdef __cplusplus
}
#endif
#endif /* _EXTENSIONS_H_ */

