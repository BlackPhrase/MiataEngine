#include "quakedef.h"
#include "Host.hpp"

void CHost::Init(quakeparms_t *parms)
{
	Host_Init(parms);
};

void CHost::Shutdown()
{
	Host_Shutdown();
};

void CHost::InitCommands()
{
	Host_InitCommands();
};

void CHost::ShutdownServer(bool crash)
{
	Host_ShutdownServer(crash);
};

void CHost::ClearMemory()
{
	Host_ClearMemory();
};

void CHost::Frame(float time)
{
	Host_Frame(time);
};

void CHost::ServerFrame()
{
	Host_ServerFrame();
};

void CHost::Error(const char *error, ...)
{
	Host_Error(error);
};

void CHost::EndGame(const char *message, ...)
{
	Host_EndGame(message);
};

void CHost::ClientCommands(const char *fmt, ...)
{
	Host_ClientCommands(fmt);
};

void Host_Quit_f()
{
};