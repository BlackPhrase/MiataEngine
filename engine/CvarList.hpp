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
#include "ICvarList.hpp"

struct IConVar;
using tCvarList = std::list<IConVar*>;

class CCvarList final : public ICvarList
{
public:
	CCvarList();
	~CCvarList();
	
	bool Register(IConVar *variable) override;
	
	IConVar *Create(const char *name, const char *defvalue, int flags = 0, const char *desc = "");
	
	IConVar *Find(const char *var_name /*bool partial = false*/) const override;
	IConVar *FindPartial(const char *var_name, int length) const;
private:
	tCvarList mlstCvars;
};