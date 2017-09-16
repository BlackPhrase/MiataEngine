#pragma once

#include "IGame.hpp"

class CGame final : public IGame
{
public:
	CGame(IEngineInterface *apEngine);
	~CGame();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
private:
	IEngineInterface *mpEngine{nullptr};
};