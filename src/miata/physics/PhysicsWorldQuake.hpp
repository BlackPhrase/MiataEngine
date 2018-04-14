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
/// @brief Quake physics world

#pragma once

#include "physics/IPhysicsWorld.hpp"

class CPhysicsWorldQuake final : public IPhysicsWorld
{
public:
	CPhysicsWorldQuake() = default;
	~CPhysicsWorldQuake() = default;
	
	void Update(float afTimeStep) override;
	
	void SetGravity(vec3_t avGravity) override;
	vec3_t GetGravity() const override;
	
	IPhysicsBody *CreateBody() override;
	void DestroyBody(IPhysicsBody *apBody) override;
private:
	vec3_t mvGravity{};
};