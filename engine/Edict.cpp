#include "Edict.hpp"
#include "IPhysicsBody.hpp"
#include "IEntityComponent.hpp"

void CEdict::SetPos(const vec3_t &avPos)
{
	VectorCopy(avPos, mvPos);
	
	if(mpPhysBody)
		mpPhysBody->SetPos(mvPos);
};