#pragma once

constexpr auto MAX_BEAMS{24};

struct beam_t
{
	int entity{0};
	struct model_s *model{nullptr};
	float endtime{0.0f};
	vec3_t start, end;
};