#pragma once

class CConsole
{
public:
	void Init();
	
	void DrawCharacter(int cx, int line, int num);
	void DrawConsole(int lines, bool drawinput);
	
	void CheckResize();
	
	void Print(const char *txt);
	void Printf(const char *fmt, ...);
	void DPrintf(const char *fmt, ...);
	void SafePrintf(const char *fmt, ...);
	
	void NotifyBox(const char *text); // during startup for sound / cd warnings
	
	void DrawNotify();
	void ClearNotify();
	
	void Con_Clear_f();
	void Con_ToggleConsole_f();
};