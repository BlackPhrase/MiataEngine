#include "Edict.hpp"
#include "IPhysicsBody.hpp"

void CEdict::SetPos(vec3_t avPos)
{
	mvPos = avPos;
	
	if(mpPhysBody)
		mpPhysBody->SetPos(mvPos);
};