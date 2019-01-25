#pragma once

class CKeys
{
public:
	void Init();
	
	void ClearStates();
	
	void Key_Event(int key, bool down);
	
	void SetBinding(int keynum, const char *binding);
	
	void WriteBindings(FILE *f);
};