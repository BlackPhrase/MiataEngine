/// @file

#pragma once

struct IEntity;

struct ILocalClient
{
	///
	virtual void Disconnect(const char *reason, ...) = 0;
	
	///
	virtual IEntity *GetEntity() const = 0;
};