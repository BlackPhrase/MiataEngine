/// @file

#pragma once

#include "clgame/IClientGame.hpp"

class CClientGameNull final : public IClientGame
{
public:
	CClientGameNull();
	~CClientGameNull();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
};