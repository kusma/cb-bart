#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;
PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv = NULL;

/* render to texture-saker */
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB = NULL;
PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB = NULL;
PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB = NULL;
PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB = NULL;
PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB = NULL;
PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB = NULL;
PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB = NULL;


PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT = NULL;

BOOL init_extensions(){

	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	if(!glActiveTextureARB) return FALSE;

	glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC)wglGetProcAddress("glMultiTexCoord2fv");
	if(!glMultiTexCoord2fv) return FALSE;

	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if(!wglChoosePixelFormatARB) return FALSE;
	wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
	if(!wglCreatePbufferARB) return FALSE;
	wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
	if(!wglGetPbufferDCARB) return FALSE;
	wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");
	if(!wglQueryPbufferARB) return FALSE;
	wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
	if(!wglReleasePbufferDCARB) return FALSE;
	wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
	if(!wglDestroyPbufferARB) return FALSE;
	wglBindTexImageARB = (PFNWGLBINDTEXIMAGEARBPROC)wglGetProcAddress("wglBindTexImageARB");
	if(!wglBindTexImageARB) return FALSE;

	wglReleaseTexImageARB = (PFNWGLRELEASETEXIMAGEARBPROC)wglGetProcAddress("wglReleaseTexImageARB");
	if(!wglReleaseTexImageARB) return FALSE;

	glBlendEquationEXT = (PFNGLBLENDEQUATIONEXTPROC)wglGetProcAddress("glBlendEquationEXT");
	if(!glBlendEquationEXT) return FALSE;

	return TRUE;
}