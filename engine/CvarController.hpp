/*
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

#include <list>
#include "ICvarController.hpp"

struct IConVar;
class CCvarList;
class CCmdList;

class CCvarController final : public ICvarController
{
public:
	CCvarController(CCvarList *apCvarList, CCmdList *apCmdList);
	~CCvarController();
	
	/// Attempts to match a partial variable name for command line completion
	/// Returns NULL if nothing fits
	const char *CompleteVariable(const char *partial);

	/// Equivelant to "<name> <variable>" typed at the console
	void SetCvarString(const char *var_name, const char *value) override;

	/// Expands value to a string and calls Cvar_Set
	void SetCvarFloat(const char *var_name, float value) override;
	
	/// Returns an empty string if not defined
	const char *GetCvarString(const char *var_name, const char *defval) override;
	
	/// Returns 0 if not defined or non numeric
	float GetCvarFloat(const char *var_name, float defval) override;
private:
	static constexpr auto cvar_null_string{""};
	
	CCvarList *mpCvarList{nullptr};
	CCmdList *mpCmdList{nullptr};
};