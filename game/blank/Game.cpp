#include "Game.hpp"

CGame::CGame(IEngineInterface *apEngine) : mpEngine(apEngine){}
CGame::~CGame() = default;

bool CGame::Init()
{
	mpEngine->GetLogger()->Printf("Game initialization...");
	
	return true;
};

void CGame::Shutdown()
{
	mpEngine->GetLogger()->Printf("Game shutdown...");
};

void CGame::Frame()
{
};