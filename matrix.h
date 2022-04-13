#ifndef MATRIX_H
#define MATRIX_H

#ifdef __cplusplus
extern "C"{
#endif 

#include "vector.h"

typedef float matrix[16];

void matrix_identity( matrix m );
void matrix_rotate( matrix m, vector v );
void matrix_translate( matrix m, vector v );
vector matrix_get_translation( matrix m );
void matrix_scale( matrix m, vector v );
void matrix_lookat( matrix m, vector position, vector target, float roll );

void matrix_multiply(matrix target, matrix m1, matrix m2);
vector matrix_transformvector( matrix m, vector v );
vector matrix_rotatevector( matrix m, vector v );
void matrix_transpose( matrix target, matrix source );
void matrix_inverse(matrix dest, matrix source);
vector matrix_inverserotatevector( matrix m, vector v );

#ifdef __cplusplus
}
#endif 

#endif /* MATRIX_H */
