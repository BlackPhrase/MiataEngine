#pragma once

#include "IEngineInterface.hpp"

class CEngineInterface final : public IEngineInterface
{
public:
	CEngineInterface(ILogger *apLogger);
	~CEngineInterface();
	
	ILogger *GetLogger() const override {return mpLogger;}
private:
	ILogger *mpLogger{nullptr};
};