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
/// @brief module loader component

#pragma once

#include <cstdlib>
#include <vector>
#include <memory>
#include "shared_lib.hpp"
#include "IEngineInterface.hpp"

using tSharedLibVec = std::vector<shiftutil::shared_lib*>;

class CModuleLoader final
{
public:
	CModuleLoader(IEngineInterface *apEngineInterface)
		: mpEngineInterface(apEngineInterface){}
	~CModuleLoader() = default;
	
	template<typename T>
	T *LoadModule(const char *asName, const char *asExportFunc)
	{
		using pfnFactory = T *(*)(IEngineInterface *apEngineInterface);
		
		std::unique_ptr<shiftutil::shared_lib> ModuleLib = std::make_unique<shiftutil::shared_lib>(asName);
		
		if(!ModuleLib)
			return nullptr;
		
		pfnFactory fnGetFactory{ModuleLib->getexport<pfnFactory>(asExportFunc)};
		
		if(!fnGetFactory)
			return nullptr;
		
		T *pInterface{fnGetFactory(mpEngineInterface)};
		
		if(!pInterface)
			return nullptr;
		
		mvModuleLibs.push_back(ModuleLib.release());
		
		return pInterface;
	};
	
	template<typename T>
	T *LoadModule(const char *asName)
	{
		return LoadModule<T>(asName, "GetModule"); // "ModuleMain"
	};
private:
	IEngineInterface *mpEngineInterface{nullptr};
	
	tSharedLibVec mvModuleLibs;
};