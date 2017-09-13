#pragma once

#include <string>
#include <stdexcept>

namespace shiftutil
{

using string = std::string;

#ifdef _WIN32
	#include <windows.h>

	using libhandle_t = HMODULE;
#else
	#include <dlfcn.h>

	using libhandle_t = void *;
#endif

class shared_lib
{
public:
	shared_lib(const string &path) : m_path(path)
	{
		if(!load(m_path))
			throw std::runtime_error(string("Unable to load library \"").append(m_path).append("\"!"));
	};
	
	shared_lib() = default;
	
	~shared_lib(){unload();}
	
	///
	bool load(const string &path)
	{
		unload();
		
		m_path = path;
		
#ifdef _WIN32
		m_handle = LoadLibrary(path.c_str());
#else
		m_handle = dlopen(path.c_str(), RTLD_NOW);
#endif
		
		if(!m_handle)
			return false;
		
		return true;
	};
	
	///
	template<typename T>
	T getexport(const string &name)
	{
#ifdef _WIN32
		return reinterpret_cast<T>(GetProcAddress(m_handle, name.c_str()));
#else
		return reinterpret_cast<T>(dlsym(m_handle, name.c_str()));
#endif
	};
	
	///
	bool is_valid() const {return m_handle != nullptr;}
	
	///
	operator bool(){return is_valid();}
	
	///
	const string &getpath() const {return m_path;}
private:
	///
	void unload()
	{
		if(!m_handle)
			return;
		
		m_path = "";

#ifdef _WIN32
		FreeLibrary(m_handle);
#else
		dlclose(m_handle);
#endif
	};
	
	string m_path{""};
	
	libhandle_t m_handle{nullptr};
};

}; // namespace shiftutil