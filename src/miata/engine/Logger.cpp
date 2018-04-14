#include <iostream>
#include "quakedef.hpp"
#include "Logger.hpp"

CLogger::CLogger() = default;
CLogger::~CLogger() = default;

void CLogger::Printf(const char *msg, ...)
{
	char fmt[256]{};
	va_list arglist;
	
	va_start(arglist, msg);
	vsprintf(fmt, msg, arglist);
	va_end(arglist);
	
	Print(fmt);
};

void CLogger::DPrintf(const char *msg, ...)
{
	// don't confuse non-developers with techie stuff...
	//if(!developer.value)
		//return;
	
	char fmt[256]{};
	va_list arglist;
	
	va_start(arglist, msg);
	vsprintf(fmt, msg, arglist);
	va_end(arglist);
	
	Print(fmt);
};

void CLogger::Warning(const char *msg, ...)
{
	char fmt[256]{};
	va_list arglist;
	
	va_start(arglist, msg);
	vsprintf(fmt, msg, arglist);
	va_end(arglist);
	
	Print(fmt);
};

void CLogger::Error(const char *msg, ...)
{
	char fmt[256]{};
	va_list arglist;
	
	va_start(arglist, msg);
	vsprintf(fmt, msg, arglist);
	va_end(arglist);
	
	Print(fmt);
};

void CLogger::Print(const char *msg)
{
	// also echo to debugging console
	//Sys_Printf("%s", msg);

	// log all messages to file
	//if(con_debuglog)
		//Con_DebugLog(va("%s/qconsole.log", com_gamedir), "%s", msg);
	
	// write it to the scrollable buffer
	//mpConsole->Print(msg);
	
	std::cout << msg << std::endl;
};