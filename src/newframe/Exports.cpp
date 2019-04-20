/// @file

#include "Engine.hpp"

C_EXPORT IEngine *GetEngine()
{
	static CEngine Engine();
	return &Engine;
};