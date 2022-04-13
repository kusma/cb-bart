#ifndef OBJECT_H
#define OBJECT_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "vector.h"

typedef struct{
	float r, g, b;
}color;

typedef struct{
	float u, v;
}texcoord;

typedef struct{
	unsigned int id;
	char *name;

	BOOL doublesided;
	BOOL shaded;
	BOOL smoothshaded;
	BOOL wireframe;

	float alpha;
	color ambient;
	color diffuse;
	color specular;
	float specular_level;
	float shininess;

	int texturemap;
	int environmentmap;
}material;


typedef struct{
	material *mat;
	unsigned int triangle_count;
	unsigned int *triangles;
}submesh;

typedef struct{
	BOOL recalculate_normals;

	unsigned int vertex_count;
	vector *vertices;
	vector *normals;
	texcoord *texcoords;

	unsigned int submesh_count;
	submesh *submeshes;	
}object;


object *load_object( char *filename );
object *copy_object( object *source );

void draw_object( object *obj );
int load_texture(char *filename, BOOL wrap);

#endif
