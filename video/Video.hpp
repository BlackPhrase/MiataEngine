#pragma once

#include "IVideo.hpp"

class CVideo final : public IVideo
{
public:
	CVideo(IEngineInterface *apEngine);
	~CVideo();
	
	bool Init() override;
	void Shutdown() override;
	
	//void Frame() override;
	
	void CheckChanges() override;
private:
	IEngineInterface *mpEngine{nullptr};
};