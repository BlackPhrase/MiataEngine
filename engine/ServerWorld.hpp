/*
Copyright (C) 1996-1997 Id Software, Inc.
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

#pragma once

class CEdict;
struct IPhysicsWorld;

class CServerWorld final
{
public:
	CServerWorld() = default;
	~CServerWorld() = default;
	
	void Update();
	
	void UpdatePhysics();
	
	void StartParticle(vec3_t org, vec3_t dir, int color, int count);
	void StartSound(CEdict *entity, int channel, const char *sample, int volume, float attenuation);

	void SetPhysWorld(IPhysicsWorld *apWorld);
	IPhysicsWorld *GetPhysWorld() const {return mpPhysWorld;}
private:
	IPhysicsWorld *mpPhysWorld{nullptr};
};