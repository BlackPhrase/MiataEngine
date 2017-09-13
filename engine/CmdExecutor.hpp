/// @file
/// @brief command execution component

#pragma once

#include "cmd.hpp"

class CCvarList;
class CCmdList;
class CCvarManager;
class CCmdArgs;

class CCmdExecutor final
{
public:
	CCmdExecutor(CCvarList *apCvarList, CCmdList *apCmdList, CCvarManager *apCvarManager);
	~CCmdExecutor();
	
	// Parses a single line of text into arguments and tries to execute it.
	// The text can come from the command buffer, a remote client, or stdin.
	void ExecString(const char *text, cmd_source_t src);
private:
	// Called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
	// command. Returns true if the command was a variable reference that
	// was handled. (print or change)
	bool HandleCvarCommand(const CCmdArgs &aArgs);
	
	CCvarList *mpCvarList{nullptr};
	CCmdList *mpCmdList{nullptr};
	CCvarManager *mpCvarManager{nullptr};
};