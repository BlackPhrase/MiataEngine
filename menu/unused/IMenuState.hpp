#pragma once

struct IMenuState
{
	virtual void Draw() = 0;
	
	virtual void HandleKey(int key) = 0;
};