#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include "extensions.h"
#include "rtt.h"

#define OUTER_BLUR_LEN 4
#define INNER_BLUR_LEN 4

rtt_target *blur_target;

void init_blur(int width, int height){
	blur_target = init_rtt(width,height,4.f/3.f,FALSE,FALSE);
}

int blur(float rotation, float distance, float alpha, BOOL additive, int texture){
	int inner, outer;

	float vertices[] = {
		-1.f, -1.f,
		 1.f, -1.f,
		 1.f,  1.f,
		-1.f,  1.f
	};

	float uv[2];

	float piss = 1.f/(OUTER_BLUR_LEN*INNER_BLUR_LEN);
	float scale_delta = distance*piss*0.5f;
	float rot_delta = rotation*piss;
	float scale = 1-scale_delta;
	float rot = 0;

	if(!blur_target) return -1;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,0,1);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_CULL_FACE );

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	if(additive){
		if(alpha<1.f) glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		else glBlendFunc( GL_ONE, GL_ONE );
	}else{
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}

	glColor4f( 1,1,1, alpha );

	start_rtt(blur_target);
	glDisable(GL_BLEND);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2fv(&vertices[0]);
		glTexCoord2f(1.0, 0.0);
		glVertex2fv(&vertices[2]);
		glTexCoord2f(1.0, 1.0);
		glVertex2fv(&vertices[4]);
		glTexCoord2f(0.0, 1.0);
		glVertex2fv(&vertices[6]);
	glEnd();
	glEnable(GL_BLEND);
	end_rtt(blur_target);

	for(outer=0;outer<OUTER_BLUR_LEN;outer++){
		start_rtt(blur_target);
		for(inner=0;inner<INNER_BLUR_LEN;inner++){
			float u,v;
			float ru,rv;
			float cr = (float)cos(rot);
			float sr = (float)sin(rot);
			glBegin(GL_QUADS);
				u = -scale;
				v = -scale;
				ru = u*cr-v*sr;
				rv = u*sr+v*cr;
				uv[0] = (ru/2)+0.5f;
				uv[1] = (rv/2)+0.5f;
				glTexCoord2fv((float*)&uv);
				glVertex2fv(&vertices[0]);

				u = scale;
				v = -scale;
				ru = u*cr-v*sr;
				rv = u*sr+v*cr;
				uv[0] = (ru/2)+0.5f;
				uv[1] = (rv/2)+0.5f;
				glTexCoord2fv((float*)&uv);
				glVertex2fv(&vertices[2]);

				u = scale;
				v = scale;
				ru = u*cr-v*sr;
				rv = u*sr+v*cr;
				uv[0] = (ru/2)+0.5f;
				uv[1] = (rv/2)+0.5f;
				glTexCoord2fv((float*)&uv);
				glVertex2fv(&vertices[4]);

				u = -scale;
				v = scale;
				ru = u*cr-v*sr;
				rv = u*sr+v*cr;
				uv[0] = (ru/2)+0.5f;
				uv[1] = (rv/2)+0.5f;
				glTexCoord2fv((float*)&uv);
				glVertex2fv(&vertices[6]);
			glEnd();

			scale -= scale_delta;
			rot += rot_delta;
		}
		end_rtt(blur_target);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
	return blur_target->texture;
}
