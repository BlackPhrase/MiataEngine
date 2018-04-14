#pragma once

#define MAX_DLIGHTS 32

typedef struct
{
	vec3_t origin;
	float radius;
	float die;      // stop lighting after this time
	float decay;    // drop this each second
	float minlight; // don't add when contributing less
	int key;
#ifdef QUAKE2
	bool dark; // subtracts light instead of adding
#endif
} dlight_t;