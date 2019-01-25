#pragma once

class CCDAudio
{
public:
	int Init();
	void Shutdown();
	
	void Update();
	
	void Play(byte track, bool looping);
	void Stop();
	
	void Pause();
	void Resume();
};