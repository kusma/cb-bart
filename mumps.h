#ifndef _MUMPS_H_
#define _MUMPS_H_

#ifdef __cplusplus
extern "C"{
#endif

extern HINSTANCE mumps_instance;
extern BOOL mumps_fullscreen;
extern HWND mumps_win;
extern HDC mumps_hdc;
extern HGLRC mumps_hrc;
extern int mumps_width;
extern int mumps_height;

/*
opens an opengl-window

usage:

title			- a windowtitle, you fool!
width, height	- dimentions of the window
bpp				- bits per pixel
zbuffer			- bits per pixel for the zbuffer
visual_id		- if you know the id of the DEVMODE you want put it here (width,height and bpp is then
				ignored), otherwise use -1.
fullscreen		- guess! ;)
*/
int mumps_open(char *title, int width, int height, int bpp, int zbuffer, int visual_id, BOOL fullscreen);

/* updates the window */
void mumps_update();

/* closes the window */
void mumps_close();

#ifdef __cplusplus
}
#endif

#endif /* _MUMPS_H_ */