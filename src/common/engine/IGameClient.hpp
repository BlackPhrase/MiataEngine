/// @file

#pragma once

struct IGameClient
{
	///
	virtual void Disconnect(const char *reason, ...) = 0;
	
	///
	virtual IEntity *GetEntity() const = 0;
};