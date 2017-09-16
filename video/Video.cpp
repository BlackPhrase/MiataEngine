#include "Video.hpp"

CVideo::CVideo(IEngineInterface *apEngine) : mpEngine(apEngine){}
CVideo::~CVideo() = default;

bool CVideo::Init()
{
	mpEngine->GetLogger()->Printf("Video initialization...");
	
	return true;
};

void CVideo::Shutdown()
{
	mpEngine->GetLogger()->Printf("Video shutdown...");
};

//void CVideo::Frame()
//{
//};

void CVideo::CheckChanges()
{
};