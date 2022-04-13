#ifndef BLUR_H
#define BLUR_H

#include <stdbool.h>

void init_blur(int width, int height);
int blur(float rotation, float distance, float alpha, bool additive, int texture);

#endif