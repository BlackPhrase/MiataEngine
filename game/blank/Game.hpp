#pragma once

#include "IGame.hpp"

class CGame final : public IGame
{
public:
	CGame();
	~CGame();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
};