#include <glad/gl.h>
#include <GL/glu.h>

#include <string.h>

#include "object.h"
#include "file.h"
#include "image.h"

char *get_dirname(char *filename){
	char temp[256];
	int i = strlen(filename);
	while(i--){
		if((filename[i]=='\\')||(filename[i]=='/')){
			break;
		}
	}
	strncpy(temp,filename,i+1);
	temp[i+1] = '\0';
	return strdup(temp);
}

char *loadstring(file* file){
	char *temp = strdup((char*)(file->data+file->pos));
	file->pos += strlen(temp)+1;
	return temp;
}


int load_texture(char *filename, bool wrap){
	unsigned int id;
	unsigned int* pixels;
	unsigned int width;
	unsigned int height;

	if(!image_load(filename,&pixels,&width,&height)) return -1;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST);

	if(wrap){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/* mipmapping er sweet */
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}

material *load_material(char * filename){
	char *texture_filename;
	material *temp;
	char test[4];
	file *fp = file_open(filename);
	if(!fp) return NULL;

	file_read(test,1,4,fp);
	if(memcmp(test,"KRM0",4)!=0){
		file_close(fp);
		return NULL;
	}

	temp = (material*)malloc(sizeof(material));
	if(!temp){
		file_close(fp);
		return NULL;
	}

	temp->doublesided = false;
	temp->shaded = true;
	temp->smoothshaded = true;
	temp->wireframe = false;

	temp->alpha = 1.f;

	file_read(&temp->ambient,1,sizeof(color),fp);
	file_read(&temp->diffuse,1,sizeof(color),fp);
	file_read(&temp->specular,1,sizeof(color),fp);

	file_read(&temp->specular_level,1,sizeof(float),fp);
	file_read(&temp->shininess,1,sizeof(float),fp);

	texture_filename = loadstring(fp);
	temp->texturemap = load_texture(texture_filename,true);
	free(texture_filename);

	texture_filename = loadstring(fp);
	temp->environmentmap = load_texture(texture_filename,true);
	free(texture_filename);

	file_close(fp);
	return temp;
}

void generate_normals(object *obj){
	unsigned int mesh,i;
	vector* normal;
	unsigned int *face;

	if(!obj) return;

	memset( obj->normals, 0, sizeof(vector)*obj->vertex_count );

	for(mesh=0;mesh<obj->submesh_count;mesh++){
		face = obj->submeshes[mesh].triangles;
		for(i=0;i<obj->submeshes[mesh].triangle_count;i++){

			vector facenormal =
						vector_scale(
							vector_crossproduct(
								vector_sub( obj->vertices[face[i*3+2]], obj->vertices[face[i*3+0]] ),
								vector_sub( obj->vertices[face[i*3+1]], obj->vertices[face[i*3+0]] )
								)
							,-1);
			obj->normals[face[i*3+0]] =  vector_add( obj->normals[face[i*3+0]], facenormal );
			obj->normals[face[i*3+1]] =  vector_add( obj->normals[face[i*3+1]], facenormal );
			obj->normals[face[i*3+2]] =  vector_add( obj->normals[face[i*3+2]], facenormal );
		}
	}

	normal = obj->normals;
	for( i=0; i<obj->vertex_count; i++ ){
		*normal = vector_normalize( *normal );
		normal++;
	}

	obj->recalculate_normals = false;
}

object *load_object( char *filename ){
	object *temp;
	file *fp;
	unsigned int submesh_counter;
	char test[4];

	if(!filename) return NULL;

	fp = file_open(filename);
	if(!fp) return NULL;

	temp = (object*)malloc(sizeof(object));
	if(!temp) return NULL;

	file_read(test,1,4,fp);
	if(memcmp(test,"KRO0",4)!=0){
		file_close(fp);
		free(temp);
		return NULL;
	}

	file_read(&temp->vertex_count,1,4,fp);
	temp->vertices = (vector*)malloc(sizeof(vector)*temp->vertex_count);
	file_read(temp->vertices,sizeof(vector),temp->vertex_count,fp);
	temp->texcoords = (texcoord*)malloc(sizeof(texcoord)*temp->vertex_count);
	file_read(temp->texcoords,sizeof(texcoord),temp->vertex_count,fp);

	temp->normals = (vector*)malloc(sizeof(vector)*temp->vertex_count);
//	file_read(temp->normals,sizeof(vector),temp->vertex_count,fp);

	file_read(&temp->submesh_count,1,4,fp);
	temp->submeshes = (submesh*)malloc(sizeof(submesh)*temp->submesh_count);

	for(submesh_counter=0;submesh_counter<temp->submesh_count;submesh_counter++){
		char mat_name[256];
		char *material_name = loadstring(fp);
		sprintf(mat_name,"%s%s",get_dirname(filename),material_name); 
//		printf("material name: %s\n",mat_name);

		temp->submeshes[submesh_counter].mat = load_material(mat_name);

		file_read(&temp->submeshes[submesh_counter].triangle_count,1,4,fp);
		temp->submeshes[submesh_counter].triangles = (unsigned int*)malloc(sizeof(unsigned int)*3*temp->submeshes[submesh_counter].triangle_count);
		file_read(temp->submeshes[submesh_counter].triangles,sizeof(unsigned int)*3,temp->submeshes[submesh_counter].triangle_count,fp);
	}
	file_close(fp);

	generate_normals(temp);

	return temp;
}

object *copy_object( object *source ){
	object *temp;
	unsigned int submesh_counter;
	if(!source) return NULL;

	temp = malloc(sizeof(object));
	if(!temp) return NULL;

	temp->vertex_count = source->vertex_count;

	temp->vertices = (vector*)malloc(sizeof(vector)*temp->vertex_count);
	memcpy(temp->vertices, source->vertices, sizeof(vector)*temp->vertex_count);
	temp->texcoords = (texcoord*)malloc(sizeof(texcoord)*temp->vertex_count);
	memcpy(temp->texcoords, source->texcoords, sizeof(texcoord)*temp->vertex_count);
	temp->normals = (vector*)malloc(sizeof(vector)*temp->vertex_count);
	memcpy(temp->normals, source->normals, sizeof(vector)*temp->vertex_count);

	temp->submesh_count = source->submesh_count;
	temp->submeshes = (submesh*)malloc(sizeof(submesh)*temp->submesh_count);

	for(submesh_counter=0;submesh_counter<temp->submesh_count;submesh_counter++){
		temp->submeshes[submesh_counter].mat = source->submeshes[submesh_counter].mat;

		temp->submeshes[submesh_counter].triangle_count = source->submeshes[submesh_counter].triangle_count;
		temp->submeshes[submesh_counter].triangles = (unsigned int*)malloc(sizeof(unsigned int)*3*temp->submeshes[submesh_counter].triangle_count);
		memcpy(temp->submeshes[submesh_counter].triangles,source->submeshes[submesh_counter].triangles,sizeof(unsigned int)*3*temp->submeshes[submesh_counter].triangle_count);
	}

	return temp;
}

static inline void set_param(GLenum mode,color param,GLfloat alpha){
	GLfloat temp[4] = { param.r, param.g, param.b, alpha };
	glMaterialfv(GL_FRONT_AND_BACK,mode,temp);
}

void set_material( material* mat ){
	float alpha;
	if(mat==NULL) return;

	alpha = mat->alpha;
	glColor3f(1,1,1);
	set_param(GL_AMBIENT,mat->ambient,alpha);
	set_param(GL_DIFFUSE,mat->diffuse,alpha);
	set_param(GL_SPECULAR,mat->specular,mat->specular_level);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mat->shininess);
	glDisable(GL_COLOR_MATERIAL);

	glActiveTexture(GL_TEXTURE0);
	if(mat->texturemap != -1){
		glBindTexture(GL_TEXTURE_2D,mat->texturemap);
		glEnable(GL_TEXTURE_2D);
	}else{
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
	}

	glActiveTexture(GL_TEXTURE1);
	if(mat->environmentmap != -1){
		glBindTexture(GL_TEXTURE_2D,mat->environmentmap);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		if(mat->texturemap != -1){
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
		}else{
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
	}else{
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
	}
	glActiveTexture(GL_TEXTURE0);

	if(!mat->doublesided) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);
}

void draw_object( object *obj ){
	unsigned int mesh;
	if(!obj) return;

	if(obj->recalculate_normals) generate_normals(obj);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glVertexPointer(3,GL_FLOAT,0,obj->vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2,GL_FLOAT,0,obj->texcoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glNormalPointer(GL_FLOAT,0,obj->normals);
	glEnableClientState(GL_NORMAL_ARRAY);


	for(mesh=0;mesh<obj->submesh_count;mesh++){
		/* set material */
		if(obj->submeshes[mesh].mat) set_material(obj->submeshes[mesh].mat);
		glDrawElements(GL_TRIANGLES, obj->submeshes[mesh].triangle_count*3, GL_UNSIGNED_INT, obj->submeshes[mesh].triangles);
	}

	glPopAttrib();

}