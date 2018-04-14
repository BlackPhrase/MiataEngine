#pragma once

#include "engine/IEngineInterface.hpp"

class CEngineInterface final : public IEngineInterface
{
public:
	CEngineInterface(ILogger *apLogger, ICmdBuffer *apCmdBuffer, ICvarList *apCvarList, ICvarController *apCvarController, ICmdList *apCmdList);
	~CEngineInterface();
	
	int GetVersion() const override {return IEngineInterface::Version;}
	
	ILogger *GetLogger() const override {return mpLogger;}
	ICmdBuffer *GetCmdBuffer() const override {return mpCmdBuffer;}
	ICvarList *GetCvarList() const override {return mpCvarList;}
	ICvarController *GetCvarController() const override {return mpCvarController;}
	ICmdList *GetCmdList() const override {return mpCmdList;}
private:
	ILogger *mpLogger{nullptr};
	ICmdBuffer *mpCmdBuffer{nullptr};
	ICvarList *mpCvarList{nullptr};
	ICvarController *mpCvarController{nullptr};
	ICmdList *mpCmdList{nullptr};
};