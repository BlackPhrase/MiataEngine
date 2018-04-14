#include "EngineInterface.hpp"

CEngineInterface::CEngineInterface(ILogger *apLogger, ICmdBuffer *apCmdBuffer, ICvarList *apCvarList,
									ICvarController *apCvarController, ICmdList *apCmdList)
	: mpLogger(apLogger), mpCmdBuffer(apCmdBuffer), mpCvarList(apCvarList),
	mpCvarController(apCvarController), mpCmdList(apCmdList){}
CEngineInterface::~CEngineInterface() = default;