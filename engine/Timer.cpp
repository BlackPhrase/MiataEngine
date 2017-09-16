/// @file

#include "Timer.hpp"

CTimer::CTimer(int maxups)
{
	SetMaxUpdates(maxups);
};

CTimer::~CTimer() = default;

void CTimer::Update()
{
	//newtime = curtime();
	timeaccum += newtime - oldtime;
	oldtime = newtime;
};

bool CTimer::WantUpdate()
{
	Update();
	
	if(timeaccum > timestep)
	{
		timeaccum -= timestep;
		return true;
	};
	
	return false;
};

void CTimer::SetMaxUpdates(int count)
{
	timestep = 1.0f / (float)count;
};