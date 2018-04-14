/// @file

#pragma once

#include "video/IVideo.hpp"

class CVideoNull final : public IVideo
{
public:
	CVideoNull();
	~CVideoNull();
	
	void Init() override;
	void Shutdown() override;
	
	void CheckChanges() override;
};