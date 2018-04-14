#pragma once

struct IInputImpl
{
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	
	virtual void HandleMessage(void *apData) = 0;
};