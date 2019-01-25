#pragma once

class CHost
{
public:
	void Init(quakeparms_t *parms);
	void Shutdown();
	
	void InitCommands();
	void ShutdownServer(bool crash);
	
	void ClearMemory();
	
	void Frame(float time);
	
	void ServerFrame();
	
	void Error(const char *error, ...);
	
	void EndGame(const char *message, ...);
	
	void ClientCommands(const char *fmt, ...);
	
	void Host_Quit_f();
};