/// @file

#pragma once

#include "sound/ISound.hpp"

class CSoundNull final : public ISound
{
public:
	CSoundNull();
	~CSoundNull();
	
	void Init() override;
	void Shutdown() override;
};