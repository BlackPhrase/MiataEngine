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
void CClientGame::CreateMove(usercmd_t &cmd)
{
	// get basic movement from keyboard
	CL_BaseMove(&cmd);
	
	// allow mice or other external controllers to add to the move
	IN_Move(&cmd);
};

{
};

{
};