#include "commontypes.hpp"
#include "Sound.hpp"

C_EXPORT ISound *GetSound()
{
	static CSound Sound;
	return &Sound;
};