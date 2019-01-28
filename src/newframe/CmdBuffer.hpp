#pragma once

class CCmdBuffer
{
public:
	/// Allocates an initial text buffer that will grow as needed
	void Init();

	/// As new commands are generated from the console or keybindings,
	/// the text is added to the end of the command buffer
	void AddText(const char *text);

	/// When a command wants to issue other commands immediately, the text is
	/// inserted at the beginning of the buffer, before any remaining unexecuted
	/// commands
	void InsertText(const char *text);

	/// Pulls off \n terminated lines of text from the command buffer and sends
	/// them through Cmd_ExecuteString.  Stops when the buffer is empty.
	/// Normally called once per frame, but may be explicitly invoked.
	/// Do not call inside a command function!
	void Execute();
};