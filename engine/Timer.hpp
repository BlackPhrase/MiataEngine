/// @file

#pragma once

class CTimer final
{
public:
	CTimer(int maxups);
	~CTimer();
	
	void Update();
	
	bool WantUpdate();
	
	void SetMaxUpdates(int count);
	int GetMaxUpdates() const;
	
	float GetTimeStep() const {return timestep;}
private:
	float oldtime, newtime;
	float timestep;
	float timeaccum;
};