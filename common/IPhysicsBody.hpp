/*
Copyright (C) 2017 Sh1ft0x0EF

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

struct IPhysicsBody
{
	///
	virtual void SetVelocity(vec3_t avVelocity) = 0;
	
	///
	virtual vec3_t GetVelocity() const = 0;
	
	///
	virtual void SetMass(float afMass) = 0;
	
	///
	virtual float GetMass() const = 0;
	
	///
	virtual void ApplyForce(vec3_t avForce) = 0;
};