/// @file

#pragma once

struct IGameClient;

struct IEngineServer
{
	///
	virtual IGameClient *GetClient(int id) const = 0;
};