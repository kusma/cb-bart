/*

  Routine for loading JPEG, GIF and BMP images through win32
  Copyright 2022 Erik Faye-Lund aka kusma/excess

*/

#include "file.h"
#include <FreeImage.h>
#include <stdbool.h>
#include <string.h>

bool image_load( char *filename, unsigned int **pixels, unsigned int *w, unsigned int *h )
{
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(filename);
		if (fif == FIF_UNKNOWN)
			return false;
	}

	if (!FreeImage_FIFSupportsReading(fif))
		return false;

	file *file = file_open(filename);
	if (!file)
		return false;

	FIMEMORY *hmem = FreeImage_OpenMemory(file->data, file->size);
	if (!hmem)
		return false;
	FIBITMAP *dib = FreeImage_LoadFromMemory(fif, hmem, 0);
	FreeImage_CloseMemory(hmem);
	if (!dib)
		return false;

	if (FreeImage_GetImageType(dib) != FIT_BITMAP) {
		FreeImage_Unload(dib);
		return false;
	}

	FIBITMAP *temp = dib;
	dib = FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(temp);
	if (!dib)
		return false;

	// FreeImage uses bottom-left origin, we use top-left
	FreeImage_FlipVertical(dib);

	unsigned width = FreeImage_GetWidth(dib);
	unsigned height = FreeImage_GetHeight(dib);
	size_t size = width * height * sizeof(uint32_t);

	uint32_t *image_data = malloc(size);
	if (!image_data) {
		FreeImage_Unload(dib);
		return false;
	}

	memcpy(image_data, FreeImage_GetBits(dib), size);
	FreeImage_Unload(dib);

	if (w) *w = width;
	if (h) *h = height;
	*pixels = image_data;

	return true;
}
