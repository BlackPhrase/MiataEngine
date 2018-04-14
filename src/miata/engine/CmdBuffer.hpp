/*
Copyright (C) 1996-1997 Id Software, Inc.
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
/// @brief command buffer component

#pragma once

#include "engine/ICmdBuffer.hpp"
#include "SizeBuffer.hpp"

//===========================================================================

/*

Any number of commands can be added in a frame, from several different sources.
Most commands come from either keybindings or console line input, but remote
servers can also send across commands and entire text files can be execed.

The + command line options are also added to the command buffer.

The game starts with a Cbuf_AddText ("exec quake.rc\n"); Cbuf_Execute ();

*/

class CLogger;
class CCmdExecutor;

class CCmdBuffer final : public ICmdBuffer
{
public:
	CCmdBuffer(CLogger *apLogger, CCmdExecutor *apExecutor);
	~CCmdBuffer();
	
	/// Allocates an initial text buffer that will grow as needed
	void Init();
	
	/// As new commands are generated from the console or keybindings,
	/// the text is added to the end of the command buffer
	void AddText(const char *text) override;
	
	/// When a command wants to issue other commands immediately, the text is
	/// inserted at the beginning of the buffer, before any remaining unexecuted
	/// commands
	void InsertText(const char *text) override;
	
	/// Pulls off \n terminated lines of text from the command buffer and sends
	/// them to the command executor.  Stops when the buffer is empty.
	/// Normally called once per frame, but may be explicitly invoked.
	/// Do not call inside a command function!
	void Execute() override;
private:
	CSizeBuffer cmd_text;
	
	CLogger *mpLogger{nullptr};
	CCmdExecutor *mpExecutor{nullptr};
};