/*
Copyright (C) 2018 BlackPhrase

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

#include "PhysicsBodyQuake.hpp"

void CPhysicsBodyQuake::Update(float afTimeStep)
{
	mvPos += mvVelocity * afTimeStep;
};

void CPhysicsBodyQuake::SetVelocity(vec3_t avVelocity)
{
	mvVelocity = avVelocity;
};

vec3_t CPhysicsBodyQuake::GetVelocity() const
{
	return mvVelocity;
};

void CPhysicsBodyQuake::SetMass(float afMass)
{
	mfMass = afMass;
};

float CPhysicsBodyQuake::GetMass() const
{
	return mfMass;
};

void CPhysicsBodyQuake::SetPos(vec3_t avPos)
{
	mvPos = avPos;
};

vec3_t CPhysicsBodyQuake::GetPos() const
{
	return mvPos;
};

void CPhysicsBodyQuake::ApplyForce(vec3_t avForce)
{
	mvVelocity += avForce;
};