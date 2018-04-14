/*
Copyright (C) 2017-2018 BlackPhrase

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
/// @brief console variable manager interface

#pragma once

/*
Console variables are used to hold scalar or string variables that can be changed or displayed at the console or prog code as well as accessed directly
in C code.

it is sufficient to initialize a cvar with just the first two fields, or
you can add a ,true flag for variables that you want saved to the configuration
file when the game is quit:

cvar_t r_draworder("r_draworder", "1");
cvar_t scr_screensize("screensize", "1", true);

Cvars must be registered before use, or they will have a 0 value instead of the float interpretation of the string.  Generally, all cvar_t declarations should be registered in the apropriate init function before any console commands are executed:
Cvar_RegisterVariable (&host_framerate);

C code usually just references a cvar in place:
if ( r_draworder.value )

It could optionally ask for the value to be looked up for a string name:
if (Cvar_VariableValue ("r_draworder"))

The user can access cvars from the console in two ways:
r_draworder			prints the current value
r_draworder 0		sets the current value to 0
Cvars are restricted from having the same names as commands to keep this
interface from being ambiguous.
*/

struct ICvarController
{
	///
	virtual void SetCvarString(const char *var, const char *value) = 0;
	
	/// expands value to a string and calls SetCvarString
	virtual void SetCvarFloat(const char *var, float value) = 0;
	
	/// returns an empty string if not defined
	virtual const char *GetCvarString(const char *var, const char *defval) = 0;
	
	/// returns 0 if not defined or non numeric
	virtual float GetCvarFloat(const char *var, float defval) = 0;
protected:
	virtual ~ICvarController() = default;
};