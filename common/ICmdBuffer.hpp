#pragma once

struct ICmdBuffer
{
	/// As new commands are generated from the console or keybindings,
	/// the text is added to the end of the command buffer
	virtual void AddText(const char *text) = 0;
	
	/// When a command wants to issue other commands immediately, the text is
	/// inserted at the beginning of the buffer, before any remaining unexecuted
	/// commands
	virtual void InsertText(const char *text) = 0;
	
	/// Pulls off \n terminated lines of text from the command buffer and sends
	/// them to the command executor.  Stops when the buffer is empty.
	/// Normally called once per frame, but may be explicitly invoked.
	/// Do not call inside a command function!
	virtual void Execute() = 0;
};