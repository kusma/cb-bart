#ifndef RTT_H
#define RTT_H

#include <stdbool.h>

typedef struct{
	int texture;
	int width;
	int height;
}rtt_target;

rtt_target *init_rtt(int width, int height, bool wrap, bool mipmap);
void start_rtt(rtt_target *target);
void end_rtt(rtt_target *target);

#endif
