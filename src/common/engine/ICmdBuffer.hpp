/*
Copyright (C) 2017-2018 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief command buffer interface

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