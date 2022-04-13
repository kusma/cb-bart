#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct{
	float x, y, z;
}vector;


static inline vector vector_make( float x, float y, float z ){
	vector temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	return temp;
}

static inline vector vector_add( vector v1, vector v2 ){
	vector temp;
	temp.x = v1.x + v2.x;
	temp.y = v1.y + v2.y;
	temp.z = v1.z + v2.z;
	return temp;
}

static inline vector vector_sub( vector v1, vector v2 ){
	vector temp;
	temp.x = v1.x - v2.x;
	temp.y = v1.y - v2.y;
	temp.z = v1.z - v2.z;
	return temp;
}

static inline vector vector_scale( vector v, float scalar ){
	vector temp;
	temp.x = v.x * scalar;
	temp.y = v.y * scalar;
	temp.z = v.z * scalar;
	return temp;
}

static inline float vector_magnitude( vector v ){
	return (float)sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

static inline vector vector_normalize(vector v){
	vector temp;
	float scale = 1.f/vector_magnitude(v);
	temp.x = v.x*scale;
	temp.y = v.y*scale;
	temp.z = v.z*scale;
	return temp;
}

static inline float vector_dotproduct( vector v1, vector v2 ){
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

static inline vector vector_crossproduct( const vector v1, const vector v2 ){
	vector temp;
	temp.x = (v1.y*v2.z)-(v1.z*v2.y);
	temp.y = (v1.z*v2.x)-(v1.x*v2.z);
	temp.z = (v1.x*v2.y)-(v1.y*v2.x);
	return temp;
}

#endif /* VECTOR_H */
