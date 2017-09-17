#pragma once

#include "ICmdArgs.hpp"

using pfnCmdCallback = void (*)(const ICmdArgs &aArgs);

struct ICmdList
{
	/// Called by the init functions of other parts of the program to
	/// register commands and functions to call for them.
	/// The cmd_name is referenced later, so it should not be in temp memory
	virtual bool Add(const char *cmd_name, pfnCmdCallback function, const char *desc = "") = 0;
	
	/// Used by the cvar code to check for cvar / command name overlap
	virtual bool Exists(const char *cmd_name) const = 0;
};