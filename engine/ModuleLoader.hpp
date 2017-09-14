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

#include <cstdlib>
#include <vector>
#include <memory>
#include "shared_lib.hpp"

using tSharedLibVec = std::vector<shiftutil::shared_lib*>;

class CModuleLoader final
{
public:
	CModuleLoader();
	~CModuleLoader();
	
	template<typename T>
	T *LoadModule(const char *name, const char *ExportFunc)
	{
		using pfnFactory = T *(*)();
		
		std::unique_ptr<shiftutil::shared_lib> ModuleLib = std::make_unique<shiftutil::shared_lib>(name);
		
		if(!ModuleLib)
			return nullptr;
		
		pfnFactory fnGetFactory{ModuleLib->getexport<pfnFactory>(ExportFunc)};
		
		if(!fnGetFactory)
			return nullptr;
		
		T *pInterface{fnGetFactory()};
		
		if(!pInterface)
			return nullptr;
		
		mvModuleLibs.push_back(ModuleLib.release());
		
		return pInterface;
	};
private:
	tSharedLibVec mvModuleLibs;
};