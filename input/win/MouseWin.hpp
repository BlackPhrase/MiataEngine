#pragma once

#include <dinput.h>

class CMouseWin
{
public:
	CMouseWin();
	~CMouseWin();
	
	void Startup();
	
	void ClearStates();
	
	void Activate();
	void Deactivate();
	
	void Show();
	void Hide();
	
	void Accumulate();
	
	void HandleEvent(int mstate);
private:
	bool InitDInput();
	
	LPDIRECTINPUT g_pdi;
	LPDIRECTINPUTDEVICE g_pMouse;
	
	HINSTANCE hInstDI;
	
	bool dinput;
	
	bool mouseinitialized;
	bool mouseactive;
};