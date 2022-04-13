#ifndef FILE_H
#define FILE_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>
#include <stdio.h>

#define RAR_SUPPORT
#define ARCHIVE "mordi.tmp"
#define PASSWORD "fuckbergkrystallen"

typedef struct{
	unsigned long size;
	unsigned char *data;
	unsigned int pos;
}file;

file* file_open( char *filename );
int file_close( file *file );

int file_tell( file *file );
size_t file_read( void *buffer, size_t size, size_t number, file *file );
int file_seek( file *file, long offset, int mode );

#ifdef __cplusplus
}
#endif

#endif
