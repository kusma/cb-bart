#include <windows.h>
#include <gl/gl.h>

/* variabler */
HINSTANCE mumps_instance;
BOOL mumps_fullscreen = FALSE;
HWND mumps_win = 0;
HDC mumps_hdc = NULL;
HGLRC mumps_hrc = NULL;
int mumps_width;
int mumps_height;

static LRESULT CALLBACK window_proc( HWND win, UINT message, WPARAM wparam, LPARAM lparam );

int mumps_open(char *title, int width_, int height_, int bpp, int zbuffer, int visual_id, BOOL fullscreen){
	unsigned int style;
	DEVMODE devmode;
	RECT rect;
	GLuint pixelformat;

	WNDCLASS wc = {
		CS_HREDRAW|CS_VREDRAW|CS_OWNDC,
		window_proc,
		0,
		0,
		0,
		NULL,
		NULL,
		NULL,
		NULL,
		"mumps"
	};

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		0, /* bpp */
		0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		0, /* zbuffer */
		8, /* stencil */
		0,
		PFD_MAIN_PLANE,
		0,0,0,0
	};

	mumps_fullscreen = fullscreen;

	/* behandle input */
	if(visual_id != -1){
		EnumDisplaySettings( NULL, visual_id, &devmode);
		bpp = devmode.dmBitsPerPel;
	}else{
		memset(&devmode, 0, sizeof(DEVMODE));
		devmode.dmSize = sizeof(DEVMODE);
		devmode.dmPelsWidth = width_;
		devmode.dmPelsHeight = height_;
		devmode.dmBitsPerPel = bpp;
		devmode.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
	}
	mumps_width = devmode.dmPelsWidth;
	mumps_height = devmode.dmPelsHeight;

	/* vinduklasse-stash */
	wc.hInstance = mumps_instance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	if(!RegisterClass(&wc)) return false;

	/* rektangel-fjas */
	rect.left = 0;
	rect.top = 0;
	rect.right = mumps_width;
	rect.bottom = mumps_height;

	if(!fullscreen){
		style = WS_OVERLAPPEDWINDOW|WS_VISIBLE;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);		
	}else{
		style = WS_POPUP|WS_VISIBLE;
	}
	mumps_win = CreateWindowEx(0, "mumps", title, style, 0, 0, rect.right-rect.left, rect.bottom-rect.top, 0, 0, mumps_instance, 0);
	if(!mumps_win) return false;

	/* sett fullscreen med en gang, liksågodt */
	if(fullscreen && ChangeDisplaySettings(&devmode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) return false;

	/* og fjern muspekern i fullscreen */
	if(fullscreen)ShowCursor(FALSE);

	mumps_hdc = GetDC(mumps_win);

	/* pixelformat */
	pfd.cColorBits = bpp;
	pfd.cDepthBits = zbuffer;
	pixelformat = ChoosePixelFormat(mumps_hdc, &pfd);
	if(!pixelformat) return false;
	SetPixelFormat(mumps_hdc, pixelformat, &pfd);

	/* lag opengl her ;) */
	mumps_hrc = wglCreateContext(mumps_hdc);
	if(!mumps_hrc) return false;
	wglMakeCurrent(mumps_hdc,mumps_hrc);

	/* alt gikk fint */
	return true;
}

void mumps_update(){
	if(mumps_hdc) SwapBuffers(mumps_hdc);
}

void mumps_close(){
	if(mumps_fullscreen){
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}
	if(mumps_hrc){
		wglMakeCurrent(NULL,0);
		wglDeleteContext(mumps_hrc);
		mumps_hrc = NULL;
	}
	if(mumps_hdc&&!ReleaseDC(mumps_win,mumps_hdc)) mumps_hdc = NULL;
	if(mumps_win&&!DestroyWindow(mumps_win)) mumps_win=NULL;
	UnregisterClass("mumps", mumps_instance);
}

static LRESULT CALLBACK window_proc( HWND win, UINT message, WPARAM wparam, LPARAM lparam ){
	switch(message){
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_SYSCOMMAND:
		switch(wparam){
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	case WM_SIZE:
		mumps_width = LOWORD(lparam);
		mumps_height = HIWORD(lparam);
		return 0;
	}
	return DefWindowProc(win,message,wparam,lparam);
}