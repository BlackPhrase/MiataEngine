#pragma once

#define MAX_BEAMS 24

typedef struct
{
	int entity;
	struct model_s *model;
	float endtime;
	vec3_t start, end;
} beam_t;