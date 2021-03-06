#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include "extensions.h"
#include "rtt.h"

rtt_target *init_rtt(int width, int height, float aspect, BOOL wrap, BOOL mipmap){
	rtt_target *target = (rtt_target*)malloc(sizeof(rtt_target));
	unsigned int *data = (unsigned int*)malloc(sizeof(unsigned int)*width*height);

    glGenTextures(1, (GLuint*)&target->texture);
	memset( data, 0, sizeof(unsigned int)*width*height );
    glBindTexture(GL_TEXTURE_2D, target->texture);

	glTexImage2D( GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	if(wrap){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	if(mipmap){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );
	}else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	target->width = width;
	target->height = height;
	target->aspect = aspect;
	return target;
}

void start_rtt(rtt_target *target){
	glViewport(0, 0, target->width, target->height);
}

void end_rtt(rtt_target *target){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, target->texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, target->width, target->height);
	glViewport(0, 0, 640, 480);
}
