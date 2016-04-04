#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <math.h>

#include <vorbis/vorbisfile.h>

//#include "pest.h"
#include "file.h"

#ifdef _DEBUG
#include <stdio.h>
#endif


DWORD WINAPI pest_thread_proc( LPVOID jalla );
HANDLE soundthread;

file *pest_music_file = NULL;
OggVorbis_File pest_vorbis_file;

float pest_time_offset = 0.f;

int pest_load( char *filename, float time_offset ){
	ov_callbacks mem_callbacks = {
		file_read,
		file_seek,
		file_close,
		file_tell
	};

	pest_music_file = file_open(filename);
	if(!pest_music_file) return FALSE;

	if(ov_open_callbacks(pest_music_file, &pest_vorbis_file, NULL, 0, mem_callbacks)<0)
		return FALSE;

	if(time_offset!=0.f){
		pest_time_offset = time_offset;
		ov_time_seek(&pest_vorbis_file, time_offset);
	}
	return TRUE;
}

void pest_play(){
	DWORD tmp;
	soundthread = (HANDLE)CreateThread( NULL, 0, pest_thread_proc, NULL, 0, &tmp);
}

void pest_internal_close();

void pest_close(){
	pest_internal_close();
	ov_clear(&pest_vorbis_file);
}

int fill_buffer( void* buffer, int bytes ){
	int bitstream;

	int read;
	int size = bytes;
	int ret = 0;
	char *data = (char*)buffer;
	
	while(size>4096){
		read=ov_read(&pest_vorbis_file, data, size, 0, 2, 1, &bitstream);
		
		if(read==0){
			memset(data,0,size);
			read = size;
		}else if(read<0){
			printf("error in bitstream\n");
		}
		
		ret += read;
		size -= read;
		data += read;
	}
	
	return ret;
}

/*
int fill_buffer( void* buffer, int bytes ){
	const float urin = 0.02513453452f;
	static int pikk=0;
	static float kuk=0.0f;
	short *ptr = (short*)buffer;
	int counter = (bytes/4);
	int written = 0;
	for(;counter;counter--){
		kuk = (float)sin(pikk*0.0001f)*0.5f;
		*ptr++ = (short)(sin(pikk*urin)*32000*kuk);
		*ptr++ = (short)(sin(pikk*urin*2)*32000);
		pikk++;
		written += 4;
	}

	return written;
}
*/