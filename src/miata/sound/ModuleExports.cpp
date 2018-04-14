#include "commontypes.hpp"
#include "Sound.hpp"

C_EXPORT ISound *GetSound(IEngineInterface *apEngineInterface)
{
	static CSound Sound(apEngineInterface);
	return &Sound;
};