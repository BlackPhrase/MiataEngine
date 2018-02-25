/// @file

#pragma once

struct usercmd_t
{
	vec3_t viewangles{};

	/// intended velocities
	float forwardmove{0.0f};
	float sidemove{0.0f};
	float upmove{0.0f};

	byte lightlevel{0};
};