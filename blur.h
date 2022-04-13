#ifndef BLUR_H
#define BLUR_H

void init_blur(int width, int height);
int blur(float rotation, float distance, float alpha, BOOL additive, int texture);

#endif