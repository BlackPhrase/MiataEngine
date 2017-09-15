

void	Cmd_AddCommand (char *cmd_name, xcommand_t function);
// if function is NULL, the command will be forwarded to the server
// as a clc_stringcmd instead of executed locally

void	Cmd_ExecuteString (char *text);
// Parses a single line of text into arguments and tries to execute it
// as if it was typed at the console

void Cmd_StuffCmds_f ();
