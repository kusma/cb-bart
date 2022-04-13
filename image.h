/*

  Routine for loading JPEG, GIF and BMP images through win32
  Copyright 2003 Erik Faye-Lund aka kusma/excess

*/

#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>

bool image_load( char *filename, unsigned int **pixels, unsigned int *w, unsigned int *h );

#ifdef __cplusplus
}
#endif

#endif /*__IMAGE_H__*/
