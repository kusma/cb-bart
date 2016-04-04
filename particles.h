#ifndef _PARTICLES_H_
#define _PARTICLES_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "vector.h"
void draw_particles(vector *particles, unsigned int particle_count, unsigned int texture, float size);

#ifdef __cplusplus
}
#endif
#endif /* _PARTICLES_H_ */