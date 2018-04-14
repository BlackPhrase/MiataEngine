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

#include "PhysicsWorldQuake.hpp"
#include "PhysicsBodyQuake.hpp"

void CPhysicsWorldQuake::Update(float afTimeStep)
{
	for(auto It : mvBodies)
		It->Update(afTimeStep);
};

void CPhysicsWorldQuake::SetGravity(vec3_t avGravity)
{
	mvGravity = avGravity;
};

vec3_t CPhysicsWorldQuake::GetGravity() const
{
	return mvGravity;
};

IPhysicsBody *CPhysicsWorldQuake::CreateBody()
{
	return new CPhysicsBodyQuake();
};

void CPhysicsWorldQuake::DestroyBody(IPhysicsBody *apBody)
{
	if(apBody)
	{
		delete apBody;
		apBody = nullptr;
	};
};