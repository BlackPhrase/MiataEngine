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
/// @brief Quake physics

#pragma once

#include "physics/IPhysics.hpp"

class CPhysicsQuake final : public IPhysics
{
public:
	CPhysicsQuake() = default;
	~CPhysicsQuake() = default;
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
	
	IPhysicsWorld *CreateWorld() override;
	void DestroyWorld(IPhysicsWorld *apWorld) override;
private:
};