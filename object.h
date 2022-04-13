#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
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

	bool doublesided;
	bool shaded;
	bool smoothshaded;
	bool wireframe;

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
	bool recalculate_normals;

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
int load_texture(char *filename, bool wrap);

#endif
