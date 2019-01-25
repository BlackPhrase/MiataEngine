#pragma once

class CCvar
{
public:
	/// registers a cvar that already has the name, string, and optionally the
	/// archive elements set.
	void RegisterVariable(cvar_t *variable);

	/// equivelant to "<name> <variable>" typed at the console
	void Set(const char *var_name, const char *value);

	/// expands value to a string and calls Cvar_Set
	void SetValue(const char *var_name, float value);

	/// returns 0 if not defined or non numeric
	float VariableValue(const char *var_name);

	/// returns an empty string if not defined
	char *VariableString(const char *var_name);

	/// attempts to match a partial variable name for command line completion
	/// returns nullptr if nothing fits
	char *CompleteVariable(const char *partial);

	/// called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
	/// command.  Returns true if the command was a variable reference that
	/// was handled. (print or change)
	bool HandleCommand();

	/// Writes lines containing "set variable value" for all variables
	/// with the archive flag set to true.
	void WriteVariables(FILE *f);

	cvar_t *FindVar(const char *var_name);
};