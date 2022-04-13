#include <glad/gl.h>

#include "vector.h"
#include "matrix.h"

static inline void get_up_right_vectors(vector *up,vector *right){
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	*right = vector_make(modelview[0],modelview[4],modelview[8]);
	*up = vector_make(modelview[1],modelview[5],modelview[9]);
}

void draw_particles(vector *particles, unsigned int particle_count, unsigned int texture, float size){
	vector up;
	vector right;
	int counter;
	float half_size = size*0.5f;

	vector *particle = particles;

	vector v[4];
	float tex_coords[]={
		0.f,+1.f,
		+1.f,+1.f,
		+1.f,0.f,
		0.f,0.f
	};

	get_up_right_vectors(&up,&right);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);

	glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	glBlendFunc(GL_ONE,GL_ONE);

	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D,texture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	for(counter=particle_count;counter;counter--){
		v[0] = vector_add(*particle,vector_scale(vector_sub(vector_scale(right,-1),up),half_size));
		v[1] = vector_add(*particle,vector_scale(vector_sub(right,up),half_size));
		v[2] = vector_add(*particle,vector_scale(vector_add(right,up),half_size));
		v[3] = vector_add(*particle,vector_scale(vector_add(vector_scale(right,-1),up),half_size));
		glTexCoord2fv(&tex_coords[0]);
		glVertex3fv((float*)&v[0]);
		glTexCoord2fv(&tex_coords[2]);
		glVertex3fv((float*)&v[1]);
		glTexCoord2fv(&tex_coords[4]);
		glVertex3fv((float*)&v[2]);
		glTexCoord2fv(&tex_coords[6]);
		glVertex3fv((float*)&v[3]);
		particle++;
	}
	glEnd();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glPopAttrib();
}