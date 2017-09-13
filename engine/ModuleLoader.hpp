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