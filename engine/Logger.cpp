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
	std::cout << msg << std::endl;
};