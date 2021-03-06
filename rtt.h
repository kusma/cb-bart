typedef struct{
	int texture;
	int width;
	int height;
	float aspect;
}rtt_target;

rtt_target *init_rtt(int width, int height, float aspect, BOOL wrap, BOOL mipmap);
void start_rtt(rtt_target *target);
void end_rtt(rtt_target *target);
