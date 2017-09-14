

/*
============
Cvar_WriteVariables

Writes lines containing "set variable value" for all variables
with the archive flag set to true.
============
*/
void Cvar_WriteVariables(FILE *f)
{
	for(auto var = cvar_vars; var; var = var->next)
		if(var->GetFlags() & (int)eConVarFlags::Archive)
			fprintf(f, "%s \"%s\"\n", var->GetName(), var->GetString());
};