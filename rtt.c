#include <stdlib.h>
#include <string.h>

#include <glad/gl.h>
#include <GL/glu.h>
#include "rtt.h"

rtt_target *init_rtt(int width, int height, bool wrap, bool mipmap){
	rtt_target *target = (rtt_target*)malloc(sizeof(rtt_target));

	glGenTextures(1, (GLuint*)&target->texture);
	glBindTexture(GL_TEXTURE_2D, target->texture);

	glTexImage2D( GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	if(wrap){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	if(mipmap){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
	}else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	target->width = width;
	target->height = height;
	return target;
}

void start_rtt(rtt_target *target){
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, target->width, target->height);
}

void end_rtt(rtt_target *target){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, target->texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, target->width, target->height);
	glPopAttrib();
}
