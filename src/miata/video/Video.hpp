/// @file

#pragma once

#include "video/IVideo.hpp"

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
	bool LoadRenderModule(const char *name);
	void FreeRenderModule();
	
	IEngineInterface *mpEngine{nullptr};
	
	HINSTANCE reflib_library{nullptr}; ///< Handle to render DLL
	
	bool reflib_active{false};
};