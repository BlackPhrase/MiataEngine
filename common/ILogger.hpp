#pragma once

struct ILogger
{
	///
	virtual void Printf(const char *msg, ...) = 0;
	
	///
	virtual void DPrintf(const char *msg, ...) = 0;
	
	///
	virtual void Warning(const char *msg, ...) = 0;
	
	///
	virtual void Error(const char *msg, ...) = 0;
};