/// @file

#include "Engine.hpp"

C_EXPORT IEngine *GetEngine()
{
	CEngine Engine();
	return &Engine;
};