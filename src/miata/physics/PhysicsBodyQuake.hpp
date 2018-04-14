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
/// @brief Quake physics body

#pragma once

#include "physics/IPhysicsBody.hpp"

class CPhysicsBodyQuake final : public IPhysicsBody
{
public:
	CPhysicsBodyQuake() = default;
	~CPhysicsBodyQuake() = default;
	
	void Update(float afTimeStep) override;
	
	void SetVelocity(vec3_t avVelocity) override;
	vec3_t GetVelocity() const override;
	
	void SetMass(float afMass) override;
	float GetMass() const override;
	
	void SetPos(vec3_t avPos) override;
	vec3_t GetPos() const override;
	
	void ApplyForce(vec3_t avForce) override;
private:
	vec3_t mvPos{};
	vec3_t msVelocity{};
	
	float mfMass{0.0f};
};