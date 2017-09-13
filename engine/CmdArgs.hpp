#pragma once

#include <string>
#include <vector>

using tStringVec = std::vector<std::string>;

class CCmdArgs final
{
public:
	CCmdArgs(int argc, char **argv);
	CCmdArgs(const char *args);
	~CCmdArgs();
	
	// The functions that execute commands get their parameters with these
	// functions.
	
	// Will return an empty string, not a nullptr
	// if arg > argc, so string operations are always safe
	const char *GetByIndex(int id) const;
	
	int GetCount() const {return mvArgs.size();}
	
	const char *ToString() const;
private:
	tStringVec mvArgs;
	//static char *cmd_args{nullptr};
};