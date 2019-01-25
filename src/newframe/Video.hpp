#pragma once

class CVideo
{
public:
	/// Called at startup to set up translation tables, takes 256 8 bit RGB values
	/// the palette data will go away after the call, so it must be copied off if
	/// the video driver will need it again
	void Init(unsigned char *palette);

	/// Called at shutdown
	void Shutdown();

	void CheckChanges();
};