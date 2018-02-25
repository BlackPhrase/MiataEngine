/*
Copyright (C) 2017-2018 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief entity interface

#pragma once

#include "mathlib.hpp"

struct IEntityComponent;

struct IEntity
{
	///
	virtual void SetName(const char *asName) = 0;
	
	///
	virtual const char *GetName() const = 0;
	
	///
	virtual int GetID() const = 0;
	
	///
	virtual void SetPos(const vec3_t &avPos) = 0;
	
	///
	virtual const vec3_t &GetPos() const = 0;
	
	///
	//virtual void SetParent(IEntity *apParent) = 0;
	
	///
	//virtual IEntity *GetParent() const = 0;
	
	///
	//virtual void AddChild(IEntity *apEntity) = 0;
	
	///
	//virtual void RemoveChild(IEntity *apEntity) = 0;
	
	///
	//virtual IEntity *GetChild(int anID) = 0;
	
	///
	//virtual void AddComponent(IEntityComponent *apComponent) = 0;
	
	///
	//virtual void RemoveComponent(IEntityComponent *apComponent) = 0;
	
	///
	//virtual IEntityComponent *GetComponentByIndex(int anID) const = 0;
	
	///
	//virtual IEntityComponent *GetComponentByName(const char *asName) const = 0;
};