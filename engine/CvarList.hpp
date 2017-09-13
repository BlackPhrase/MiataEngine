#pragma once

#include <list>

struct IConVar;
using tCvarList = std::list<IConVar*>;

class CCvarList final
{
public:
	CCvarList();
	~CCvarList();
	
	/// Registers a cvar that already has the name, string, and optionally the
	/// archive elements set.
	bool Register(IConVar *variable);
	
	IConVar *Create(const char *name, const char *defvalue, int flags = 0, const char *desc = "");
	
	IConVar *Find(const char *var_name);
private:
	tCvarList mlstCvars;
};