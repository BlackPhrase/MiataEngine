/// @file

#pragma once

class CGame
{
public:
	bool Init();
	void Shutdown();
	
	void Frame(float afFrameTime);
};