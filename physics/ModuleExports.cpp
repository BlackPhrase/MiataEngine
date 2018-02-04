#include "commontypes.hpp"
#include "PhysicsQuake.hpp"

C_EXPORT IPhysics *GetPhysics(IEngineInterface *apEngineInterface)
{
	static CPhysicsQuake Physics(apEngineInterface);
	return &Physics;
};