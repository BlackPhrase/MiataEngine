#pragma once

#include "IEngineInterface.hpp"

class CEngineInterface final : public IEngineInterface
{
public:
	CEngineInterface(ILogger *apLogger);
	~CEngineInterface();
	
	int GetVersion() const override {return IEngineInterface::Version;}
	
	ILogger *GetLogger() const override {return mpLogger;}
private:
	ILogger *mpLogger{nullptr};
};