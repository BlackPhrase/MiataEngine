/// @file

#pragma once

struct ILocalClient;

struct IEngineClient
{
	///
	virtual ILocalClient *GetLocalClient() const = 0; // TODO: return IGameClient?
};