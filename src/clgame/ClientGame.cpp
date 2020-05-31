/// @file

#include "ClientGame.hpp"

CClientGame::CClientGame(IEngineInterface *apEngine) : mpEngine(apEngine){}
CClientGame::~CClientGame() = default;

bool CClientGame::Init()
{
	mpEngine->GetLogger()->Printf("Client game initialization...");
	
	return true;
};

void CClientGame::Shutdown()
{
	mpEngine->GetLogger()->Printf("Client game shutdown...");
};

void CClientGame::Frame()
{
};

void CClientGame::ParseServerMessage(int cmd, sizebuf_t &net_message)
{
};

void CClientGame::BaseMove(usercmd_t *cmd)
{
};