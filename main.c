#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include "extensions.h"

#include "mumps.h"
#include "object.h"
#include "rtt.h"
#include "blur.h"
#include "particles.h"

#include <bass.h>

void error(char *string){
	MessageBox(NULL,string,NULL,MB_OK);
	exit(1);
}

#define WIDTH 640
#define HEIGHT 480

#define ASPECT (((float)WIDTH)/((float)HEIGHT))

#ifdef _DEBUG
#define FULLSCREEN false
#else
#define FULLSCREEN true
#endif

float black_color[4] = {0,0,0,0};
float white_color[4] = {1,1,1,1};


void fullscreen_image(int texture){
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D,texture);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,1,0,1,0,1);

	glTranslatef(0.5f,0.5f,0);

	glBegin(GL_QUADS);
		glTexCoord2f(0,1);
		glVertex2f(-0.5, -0.5);
		glTexCoord2f(1,1);
		glVertex2f( 0.5, -0.5);
		glTexCoord2f(1,0);
		glVertex2f( 0.5,  0.5);
		glTexCoord2f(0,0);
		glVertex2f(-0.5,  0.5);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glPopAttrib();
}

void superflash(float alpha){
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glEnable(GL_BLEND);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,1,0,1,0,1);
	glColor4f(1,1,1,alpha);

	glTranslatef(0.5f,0.5f,0);

	glBegin(GL_QUADS);
		glTexCoord2f(0,1);
		glVertex2f(-0.5, -0.5);
		glTexCoord2f(1,1);
		glVertex2f( 0.5, -0.5);
		glTexCoord2f(1,0);
		glVertex2f( 0.5,  0.5);
		glTexCoord2f(0,0);
		glVertex2f(-0.5,  0.5);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopAttrib();
}

void superflash2(float alpha){
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_BLEND);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,1,0,1,0,1);
	glColor4f(0,0,0,alpha);

	glTranslatef(0.5f,0.5f,0);

	glBegin(GL_QUADS);
		glTexCoord2f(0,1);
		glVertex2f(-0.5, -0.5);
		glTexCoord2f(1,1);
		glVertex2f( 0.5, -0.5);
		glTexCoord2f(1,0);
		glVertex2f( 0.5,  0.5);
		glTexCoord2f(0,0);
		glVertex2f(-0.5,  0.5);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopAttrib();
}

void fullscreen_image_flip(int texture, int mip){
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D,texture);
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,mip);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,1,0,1,0,1);

	glTranslatef(0.5f,0.5f,0);

	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(-0.5, -0.5);
		glTexCoord2f(1,0);
		glVertex2f( 0.5, -0.5);
		glTexCoord2f(1,1);
		glVertex2f( 0.5,  0.5);
		glTexCoord2f(0,1);
		glVertex2f(-0.5,  0.5);
	glEnd();

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glPopAttrib();
}


void overlay(int texture, float alpha){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glColor4f(1,1,1,alpha);
	glEnable(GL_BLEND);
	fullscreen_image(texture);
	glPopAttrib();
}

void set_light( unsigned int light, float x, float y, float z, bool on ){
	GLfloat temp[4] = {x,y,z,1};
	glLightfv(GL_LIGHT0+light,GL_POSITION,temp);

	if(on) glEnable(GL_LIGHT0+light);
	else glDisable(GL_LIGHT0+light);
}

void fuss(const int count, const float time, float xsize, float ysize, const float alpha){
	int i;
	if(xsize==0.f) return;
	if(ysize==0.f) return;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	xsize = 1.f/xsize;
	ysize = 1.f/ysize;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-xsize,xsize,-ysize,ysize,0,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor4f(0,0,0,alpha);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_BLEND);

	glBegin(GL_QUADS);
	for(i=0;i<count;i++){
		float x = (float)sin(i*10+sin(time-i));
		float w = (float)sin(sin(i*10)-time)*0.2f;
		glVertex2f(x-w,-1);
		glVertex2f(x+w,-1);
		glVertex2f(x+w,+1);
		glVertex2f(x-w,+1);
	}
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	

	glPopAttrib();
}

void blob_distort( object *source, object *destination, vector wobble, vector dill, vector strength ){
	int i;
	vector *src = source->vertices;
	vector *dst = destination->vertices;
	if(!destination) return;
	if(!source) return;
	if(source->vertex_count!=destination->vertex_count) return;

	for(i=source->vertex_count;i;i--){
		vector temp = *src;
		float mul = 1.f;
		mul += (float)sin((temp.x*dill.z)+wobble.x)*strength.x;
		mul += (float)sin((temp.y*dill.y)+wobble.y)*strength.y;
		mul += (float)sin((temp.z*dill.z)+wobble.z)*strength.z;
		*dst = vector_scale(temp, mul);
		src++;
		dst++;
	}
	destination->recalculate_normals = true;
}

void make_random_particles( vector *particles, int particle_count, float field_scale){
	int i;
	for(i=0;i<particle_count;i++){
		particles[i] = vector_make(
				(float)(rand()%RAND_MAX-(RAND_MAX/2))*(1.f/RAND_MAX)*field_scale,
				(float)(rand()%RAND_MAX-(RAND_MAX/2))*(1.f/RAND_MAX)*field_scale,
				(float)(rand()%RAND_MAX-(RAND_MAX/2))*(1.f/RAND_MAX)*field_scale
			);
	}
}

__inline set_camera( vector pos, vector look_at){
	glLoadIdentity();
	gluLookAt(
		pos.x,pos.y,pos.z,
		look_at.x, look_at.y, look_at.z, // look at
		0, 1, 0 // up
	);	
}

void draw_dilldallscene(object* room, object *sphere, object *sphere_copy, float time, float flash, float flash2){
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glClearColor(1,1,1,0);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, ASPECT, 1.f, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR,black_color);
	glFogf(GL_FOG_START, 100.f);
	glFogf(GL_FOG_END, 250.f);
	glEnable(GL_FOG);

	gluLookAt(
		sin(time)*90,-50,cos(time)*90,
		0,-30,0,

		0,1,0
		);

	set_light(0, (float)sin(time*5)*50,0,(float)cos(time*5)*50, true);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	glTranslatef(100,0,-100);
	draw_object(room);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,-30+ sin(time*flash)*flash*20+flash2,0);
	glScalef(0.7f,0.7f,0.7f);
	glRotatef(time*55,1,0,1);
	glRotatef(time*55,1,0,0);
	glRotatef(time*55.2f,0,1,0);

	glRotatef(time*55,0,0,1);
	glRotatef(time*75,1,0,0);
	glRotatef(time*55.2f,0,1,0);

	blob_distort(sphere, sphere_copy, vector_make((float)sin(time),time,-time), vector_make(
		(float)(1+sin(time))*0.1f,
		(float)(1+sin(time))*0.1f,
		(float)(1+sin(time))*0.1f
		),vector_make(0.3f,0.3f,0.3f) );
	draw_object(sphere_copy);
	glPopMatrix();

	glPopAttrib();
}

#define PARTICLE_COUNT 1024
int main(){
	MSG message;
	HSTREAM stream = 0;
	object *room;
	object *sphere;
	object *sphere_copy;
	rtt_target *rendertarget;

	rtt_target *fucking_shit;

	int loading;
	int bjork_texture;
	int yo_plus;
	int carlb;
	int i;
	int veldig_kule;
	int greets;
	int mothafuckas;

	vector particles[PARTICLE_COUNT];
	make_random_particles(particles, PARTICLE_COUNT, 200);

	if(!mumps_open("CARL B!!!!1",WIDTH,HEIGHT,32,32,-1,FULLSCREEN)) error("kunne ikke lage bartevindu");
	if(!init_extensions()){
		mumps_close();
		error("kunne ikke bruke fet opengl-extension");
	}
	if (!BASS_Init(-1, 44100, 0, 0, 0)) {
		mumps_close();
		error("kunne ikke åpne fet lyd");
	}
	stream = BASS_StreamCreateFile(FALSE, "data/uglespy.ogg", 0, 0, BASS_MP3_SETPOS | BASS_STREAM_PRESCAN | 0);
	if (!stream) {
		BASS_Free();
		mumps_close();
		error("kunne ikke åpne fet lyd 2.0");
	}

	loading = load_texture("loading.jpg", false);
	if(loading==-1){
		BASS_StreamFree(stream);
		BASS_Free();
		mumps_close();
		error("kunne ikke åpne fett bilde");
	}

	glClearColor(0,0,0,0);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	fullscreen_image(loading);
	glFlush();
	mumps_update();


	room = load_object("cylinder01.kro");
	if(!room){
		BASS_StreamFree(stream);
		BASS_Free();
		mumps_close();
		error("kunne ikke åpne fett objekt");
	}

	sphere = load_object("sphere01.kro");
	if(!sphere){
		BASS_StreamFree(stream);
		BASS_Free();
		mumps_close();
		error("kunne ikke åpne fett objekt");
	}
	sphere_copy = copy_object(sphere);

	bjork_texture = load_texture("bjork.jpg", false);
	if(bjork_texture==-1){
		BASS_StreamFree(stream);
		BASS_Free();
		mumps_close();
		error("kunne ikke åpne fett bilde");
	}
	yo_plus = load_texture("yo_plus.jpg", false);
	if(yo_plus==-1){
		BASS_StreamFree(stream);
		BASS_Free();
		mumps_close();
		error("kunne ikke åpne fett bilde");
	}
	carlb = load_texture("carlb.jpg", false);
	if(carlb==-1){
		BASS_StreamFree(stream);
		BASS_Free();
		mumps_close();
		error("kunne ikke åpne fett bilde");
	}
	veldig_kule = load_texture("veldig_kule.jpg", false);
	if(veldig_kule==-1){
		BASS_StreamFree(stream);
		BASS_Free();
		mumps_close();
		error("kunne ikke åpne fett bilde");
	}
	greets = load_texture("greets.jpg", false);
	if(greets==-1){
		BASS_StreamFree(stream);
		BASS_Free();
		mumps_close();
		error("kunne ikke åpne fett bilde");
	}
	mothafuckas = load_texture("mothefuckas.jpg", false);
	if(mothafuckas==-1){
		BASS_StreamFree(stream);
		BASS_Free();
		mumps_close();
		error("kunne ikke åpne fett bilde");
	}

	rendertarget = init_rtt(512,256,(float)WIDTH/(float)HEIGHT, false, false);
	fucking_shit = init_rtt(512,256,(float)WIDTH/(float)HEIGHT, false, true);
	init_blur(256,256);

	glEnable(GL_NORMALIZE);
	
	BASS_Start();
	BASS_ChannelPlay(stream, FALSE);

	do {
		QWORD pos = BASS_ChannelGetPosition(stream, BASS_POS_BYTE);
		double time = BASS_ChannelBytes2Seconds(stream, pos);

		glViewport(0,0,mumps_width,mumps_height);

		if(time<25.55f){
			glClearColor(1,1,1,0);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			gluPerspective(120, ASPECT, 1.f, 500);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glFogi(GL_FOG_MODE, GL_EXP2);
			glFogfv(GL_FOG_COLOR,black_color);
			glFogf(GL_FOG_DENSITY, 0.02f);
			glEnable(GL_FOG);

			set_camera(
				vector_make( // pos
					(float)sin(time*0.1f)*50,
					(float)sin(time*1.2f*0.1f+1)*50,
					(float)cos(sin(time*0.1f)-time*0.1f)*50
				),
				vector_make( // look_at
					0, 0, 0
				)
			);

			draw_particles(particles,PARTICLE_COUNT,yo_plus, (float)15);

			if(time<2){
				float alpha = 1.f-time*0.5f;
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

				glColor4f(1,1,1, alpha );
				glEnable(GL_BLEND);
				fullscreen_image(loading);
				glPopAttrib();

			}

			if(time>17.5f){
				float alpha = (time-17.5);

				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT);
				glBlendFunc( GL_SRC_ALPHA, GL_ONE );

				glColor4f(1,1,1, alpha );
				glEnable(GL_BLEND);
				fullscreen_image(carlb);
				glPopAttrib();
			}
		}else if(time<51.0f){
			float flash = 1.f-(time-25.55f);
			glClearColor(1,1,1,0);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90, ASPECT, 1.f, 500);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glDisable(GL_FOG);


			gluLookAt(
				sin(time*0.1f)*90,-50,cos(time*0.1f)*90,
				0,-30,0,

				0,1,0
				);

			start_rtt(rendertarget);

			set_light(0, (float)sin(time)*50,0,(float)cos(time)*50,true);
			set_light(1, (float)sin(time+1)*50,0,(float)cos(time+1)*50,true);

			glEnable(GL_LIGHTING);
			glEnable(GL_DEPTH_TEST);

			glPushMatrix();
			glTranslatef(100,0,-100);
			draw_object(room);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0,-30,0);
			glScalef(0.7f,0.7f,0.7f);
			glRotatef(time*15,1,0,1);
			glRotatef(time*15,1,0,0);
			glRotatef(time*15.2f,0,1,0);

			glRotatef(time*15,0,0,1);
			glRotatef(time*25,1,0,0);
			glRotatef(time*15.2f,0,1,0);

			blob_distort(sphere, sphere_copy, vector_make((float)sin(time),time,-time), vector_make(
				(float)(1+sin(time))*0.1f,
				(float)(1+sin(time))*0.1f,
				(float)(1+sin(time))*0.1f
				),vector_make(0.3f,0.3f,0.3f) );
			draw_object(sphere_copy);
			glPopMatrix();

			end_rtt(rendertarget);

			i = blur((float)sin(time*0.3f)*0.3f, 0.1f+(1+(float)sin(time*0.1f))*0.2f, 0.2f, false, rendertarget->texture);

			fullscreen_image(i);

			glEnable(GL_COLOR_MATERIAL);
			glColor4f(1,1,1, (1+(float)sin(time*0.5f))*0.3f );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			glEnable(GL_BLEND);
			fullscreen_image(bjork_texture);
			glDisable(GL_BLEND);


			if(time>38.35f){
				flash = 1.f-(time-38.4f);
				if(time>44.75f){
					flash = 1.f-(time-44.75f);
				}
			}

			if(flash<0) flash = 0;
			superflash(flash);

			if(time>38.35f){
				overlay(veldig_kule, 1);
			}
		}else if(time<(60+16.8f)){
			float flash = (1.f-(time-51.f));
			float flash2 = sin(-(3.1415f/2)+(time-51.f)*(117.5f/60.f)*8);
			if(flash<0) flash = 0;

			flash *= flash;


			flash2 *= (1+flash2)*0.5f;
			flash2 *= 20;

			start_rtt(fucking_shit);
			draw_dilldallscene(room, sphere, sphere_copy,time,flash,flash2);
			end_rtt(fucking_shit);

			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

			draw_dilldallscene(room, sphere, sphere_copy,time,flash,flash2);

			glLoadIdentity();

			glEnable(GL_COLOR_MATERIAL);
			glColor4f(1,1,1, 0.5+(1+(float)sin(time*0.5f))*0.5f );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			glEnable(GL_BLEND);
			fullscreen_image_flip(fucking_shit->texture,1);
			fullscreen_image_flip(fucking_shit->texture,2);
			fullscreen_image_flip(fucking_shit->texture,3);
			glDisable(GL_BLEND);

			fuss(50,time, 1, 1, 0.1f);
			if(time>(60+3.9f)){
				flash=(1.f-(time-(60+3.9f)));
				overlay(greets, 1);
			}
			superflash(flash);
		}else{
			float flash = (1.f-(time-(60+16.8f)));
			fullscreen_image(mothafuckas);
			superflash(flash);
			if(time>(60+23.f)){
				float flash = (time-(60+23.f));
				superflash2(flash*0.5f);
			}
		}
		glFlush();
		mumps_update();

		if(time>60+30) PostQuitMessage(0);

		while(PeekMessage(&message,NULL,0,0,PM_REMOVE)){
			TranslateMessage(&message);
			DispatchMessage(&message);
		    if(message.message == WM_QUIT) break;
		}
	}while(message.message!=WM_QUIT && !GetAsyncKeyState(VK_ESCAPE));
	BASS_StreamFree(stream);
	BASS_Free();
	mumps_close();
	MessageBox(NULL,"Frigi minnet ditt sjøl, taper","In your face",MB_OK);
	return 0;
}