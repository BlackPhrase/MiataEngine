/// @file

#include "Game.hpp"
#include "game.h"

CGame gGame;

bool Game_Init()
{
	return gGame.Init();
};

void Game_Shutdown()
{
	gGame.Shutdown();
};

void Game_Frame(float afFrameTime)
{
	gGame.Frame(afFrameTime);
};

game_export_t gGame =
{
	GAME_INTERFACE_VERSION,
	
	Game_Init,
	Game_Shutdown,
	Game_Frame
};

bool CGame::Init()
{
	// TODO
	return true;
};

void CGame::Shutdown()
{
	// TODO
};

void CGame::Frame(float afFrameTime)
{
};

void G_PlayerPreThink(edict_t *ent)
{
};

void G_PlayerPostThink(edict_t *ent)
{
};

void G_ClientKill(edict_t *ent)
{
};

void G_ClientConnect(edict_t *ent)
{
};

void G_PutClientInServer(edict_t *ent)
{
};

void G_ClientDisconnect(edict_t *ent)
{
};

void G_SetNewParms()
{
};

void G_SetChangeParms()
{
};