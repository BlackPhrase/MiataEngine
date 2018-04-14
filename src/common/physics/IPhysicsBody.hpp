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
/// @brief physics body interface

#pragma once

#include "mathlib.hpp"

struct IPhysicsBody
{
	///
	virtual void Update(float afTimeStep) = 0;
	
	///
	virtual void SetVelocity(const vec3_t &avVelocity) = 0;
	
	///
	virtual const vec3_t &GetVelocity() const = 0;
	
	///
	virtual void SetMass(float afMass) = 0;
	
	///
	virtual float GetMass() const = 0;
	
	///
	virtual void SetPos(const vec3_t &avPos) = 0;
	
	///
	virtual const vec3_t &GetPos() const = 0;
	
	///
	virtual void ApplyForce(const vec3_t &avForce) = 0;
};