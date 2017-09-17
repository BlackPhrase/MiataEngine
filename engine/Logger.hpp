#pragma once

#include "ILogger.hpp"

class CLogger final : public ILogger
{
public:
	CLogger();
	~CLogger();
	
	void Printf(const char *msg, ...) override;
	void DPrintf(const char *msg, ...) override;
	void Warning(const char *msg, ...) override;
	void Error(const char *msg, ...) override;
private:
	void Print(const char *msg);
};