#pragma once

#include <list>
#include "cmd.hpp"

typedef struct cmd_function_s
{
	cmd_function_s(const char *aname, xcommand_t afunction, const char *adesc = "")
		: name(aname), function(afunction), desc(adesc){}
	~cmd_function_s() = default;
	
	const char *GetName() const {return name;}
	const char *GetDesc() const {return desc;}
	
	void Exec(const CCmdArgs &aArgs)
	{
		if(function)
			function(aArgs);
	};
	
	cmd_function_s *next{nullptr};
	
	const char *name{""};
	const char *desc{""};
	
	xcommand_t function{nullptr};
} cmd_function_t;

using tCmdList = std::list<cmd_function_t*>;

class CCmdList final
{
public:
	CCmdList();
	~CCmdList();
	
	/// Called by the init functions of other parts of the program to
	/// register commands and functions to call for them.
	/// The cmd_name is referenced later, so it should not be in temp memory
	bool Add(const char *cmd_name, xcommand_t function, const char *desc = "");
	
	///
	bool Remove(const char *cmd_name);
	
	///
	cmd_function_t *Find(const char *cmd_name) const;
	
	/// Used by the cvar code to check for cvar / command name overlap
	bool Exists(const char *cmd_name) const;
private:
	tCmdList mlstCmds; ///< possible commands to execute
};