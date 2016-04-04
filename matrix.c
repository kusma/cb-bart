#include "matrix.h"
#include "vector.h"
#include <math.h>

#pragma intrinsic( cos, sin )

void matrix_identity( matrix m ){
	m[0 ] = 1; m[1 ] = 0; m[2 ] = 0; m[3 ] = 0;
	m[4 ] = 0; m[5 ] = 1; m[6 ] = 0; m[7 ] = 0;
	m[8 ] = 0; m[9 ] = 0; m[10] = 1; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void matrix_rotate( matrix m, vector v ){
	float cx, cy, cz, sx, sy, sz;
	cx = (float)cos(v.x);
	cy = (float)cos(v.y);
	cz = (float)cos(v.z);
	sx = (float)sin(v.x);
	sy = (float)sin(v.y);
	sz = (float)sin(v.z);
	m[0 ] = cy*cz;			m[1 ] = cy*sz;			m[2 ] = -sy;	m[3 ] = 0;
	m[4 ] = sx*sy*cz-cx*sz;	m[5 ] = sx*sy*sz+cx*cz;	m[6 ] = sx*cy;	m[7 ] = 0;
	m[8 ] = cx*sy*cz+sx*sz;	m[9 ] = cx*sy*sz-sx*cz;	m[10] = cx*cy;	m[11] = 0;
	m[12] = 0;				m[13] = 0;				m[14] = 0;		m[15] = 1;
}

void matrix_translate( matrix m, vector v ){
	m[0 ] = 1;		m[1 ] = 0;		m[2 ] = 0;		m[3 ] = 0;
	m[4 ] = 0;		m[5 ] = 1;		m[6 ] = 0;		m[7 ] = 0;
	m[8 ] = 0;		m[9 ] = 0;		m[10] = 1;		m[11] = 0;
	m[12] = v.x;	m[13] = v.y;	m[14] = v.z;	m[15] = 1;
}

vector matrix_get_translation( matrix m ){
	return vector_make( m[12], m[13], m[14] );
}

void matrix_lookat( matrix m, vector position, vector target, float roll ){
	vector up, forward, right;
	matrix temp;
    up = vector_make( (float)sin(roll), (float)-cos(roll), 0 );
	forward = vector_normalize(vector_sub(target,position));
	right = vector_normalize(vector_crossproduct(up,forward));
	up = vector_normalize(vector_crossproduct(right,forward));

	m[0 ] = right.x;	m[1 ] = up.x;	m[2 ] = forward.x;	m[3 ] = 0;
	m[4 ] = right.y;	m[5 ] = up.y;	m[6 ] = forward.y;	m[7 ] = 0;
	m[8 ] = right.z;	m[9 ] = up.z;	m[10] = forward.z;	m[11] = 0;
	m[12] = 0;			m[13] = 0;		m[14] = 0;			m[15] = 1;

	matrix_translate( temp, position );
	matrix_multiply( m, m, temp );

}

void matrix_scale( matrix m, vector v ){
	m[0 ] = v.x;	m[1 ] = 0;		m[2 ] = 0;		m[3 ] = 0;
	m[4 ] = 0;		m[5 ] = v.y;	m[6 ] = 0;		m[7 ] = 0;
	m[8 ] = 0;		m[9 ] = 0;		m[10] = v.z;	m[11] = 0;
	m[12] = 0;		m[13] = 0;		m[14] = 0;		m[15] = 1;
}

vector matrix_transformvector( matrix m, vector v ){
	vector temp;
	temp.x = v.x*m[0] + v.y*m[4] + v.z*m[8] + m[12];
	temp.y = v.x*m[1] + v.y*m[5] + v.z*m[9] + m[13];
	temp.z = v.x*m[2] + v.y*m[6] + v.z*m[10] + m[14];
	return temp;
}

vector matrix_rotatevector( matrix m, vector v ){
	vector temp;
	temp.x = v.x*m[0] + v.y*m[4] + v.z*m[8];
	temp.y = v.x*m[1] + v.y*m[5] + v.z*m[9];
	temp.z = v.x*m[2] + v.y*m[6] + v.z*m[10];
	return temp;
}

vector matrix_inverserotatevector( matrix m, vector v ){
	vector temp;
	temp.x = v.x*m[0] + v.y*m[1] + v.z*m[2];
	temp.y = v.x*m[4] + v.y*m[5] + v.z*m[6];
	temp.z = v.x*m[8] + v.y*m[9] + v.z*m[10];
	return temp;
}

void matrix_transpose( matrix dest, matrix src ){
	dest[0 ] = src[0];	dest[1 ] = src[4];	dest[2 ] = src[8];	dest[3 ] = src[12];
	dest[4 ] = src[1];	dest[5 ] = src[5];	dest[6 ] = src[9];	dest[7 ] = src[13];
	dest[8 ] = src[2];	dest[9 ] = src[6];	dest[10] = src[10];	dest[11] = src[14];
	dest[12] = src[3];	dest[13] = src[7];	dest[14] = src[11];	dest[15] = src[15];
}

void matrix_multiply(matrix target, matrix m1, matrix m2){
	int i, j, counter;
	matrix temp;
	for( i=0; i<4; i++ )
		for ( j=0; j<4; j++ )
			temp[i+(j<<2)] = (m1[i]*m2[(j<<2)])
							+(m1[i+(1<<2)]*m2[1+(j<<2)])
							+(m1[i+(2<<2)]*m2[2+(j<<2)])
							+(m1[i+(3<<2)]*m2[3+(j<<2)]);

	for( counter=0; counter<(4*4); counter++ ) target[counter] = temp[counter];
}

void matrix_inverse(matrix dest, matrix source){
#if 1
	float	a,b,c,d,e,f,g,h,i,j,k,l;
	float	W;

	a=source[0+0*4];b=source[0+1*4];c=source[0+2*4];
	d=source[1+0*4];e=source[1+1*4];f=source[1+2*4];
	g=source[2+0*4];h=source[2+1*4];i=source[2+2*4];
	j=source[3+0*4];k=source[3+1*4];l=source[3+2*4];

	W=1.0f/(a*(e*i-f*h)-(b*(d*i-f*g)+c*(e*g-d*h)));

	matrix_identity(dest);

	dest[0+0*4]=(e*i-f*h)*W;
	dest[0+1*4]=(c*h-b*i)*W;
	dest[0+2*4]=(b*f-c*e)*W;

	dest[1+0*4]=(f*g-d*i)*W;
	dest[1+1*4]=(a*i-c*g)*W;
	dest[1+2*4]=(c*d-a*f)*W;

	dest[2+0*4]=(d*h-e*g)*W;
	dest[2+1*4]=(b*g-a*h)*W;
	dest[2+2*4]=(a*e-b*d)*W;

	dest[3+0*4]=(e*(g*l-i*j)+f*(h*j-g*k)-d*(h*l-i*k))*W;
	dest[3+1*4]=(a*(h*l-i*k)+b*(i*j-g*l)+c*(g*k-h*j))*W;
	dest[3+2*4]=(b*(d*l-f*j)+c*(e*j-d*k)-a*(e*l-f*k))*W;

#else
	float tmp[12];
	matrix src;
	matrix dst;
	float det;
	int i;

	/* transpose */
	for (i = 0; i < 4; i++) {
		src[i     ] = source[i*4+0];
		src[i +  4] = source[i*4+1];
		src[i +  8] = source[i*4+2];
		src[i + 12] = source[i*4+2];
	}

	tmp[ 0] = src[10] * src[15];
	tmp[ 1] = src[11] * src[14];
	tmp[ 2] = src[ 9] * src[15];
	tmp[ 3] = src[11] * src[13];
	tmp[ 4] = src[ 9] * src[14];
	tmp[ 5] = src[10] * src[13];
	tmp[ 6] = src[ 8] * src[15];
	tmp[ 7] = src[11] * src[12];
	tmp[ 8] = src[ 8] * src[14];
	tmp[ 9] = src[10] * src[12];
	tmp[10] = src[ 8] * src[13];
	tmp[11] = src[ 9] * src[12];

	dst[0]  = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	dst[1]  = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	dst[2]  = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	dst[3]  = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	dst[4]  = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	dst[5]  = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	dst[6]  = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	dst[7]  = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

	tmp[ 0] = src[2]*src[7];
	tmp[ 1] = src[3]*src[6];
	tmp[ 2] = src[1]*src[7];
	tmp[ 3] = src[3]*src[5];
	tmp[ 4] = src[1]*src[6];
	tmp[ 5] = src[2]*src[5];
	tmp[ 6] = src[0]*src[7];
	tmp[ 7] = src[3]*src[4];
	tmp[ 8] = src[0]*src[6];
	tmp[ 9] = src[2]*src[4];
	tmp[10] = src[0]*src[5];
	tmp[11] = src[1]*src[4];

	dst[ 8]  = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	dst[ 8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	dst[ 9]  = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	dst[ 9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	dst[10]  = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	dst[10] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	dst[11]  = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	dst[11] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	dst[12]  = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	dst[12] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	dst[13]  = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	dst[13] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	dst[14]  = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	dst[14] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	dst[15]  = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	dst[15] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

	det = 1.0f /
		(src[0] * dst[0] + src[1] * dst[1] +
		 src[2] * dst[2] + src[3] * dst[3]);
	for (i = 0; i < 16; i++) {
		dst[i] *= det;
	}
#endif
}

