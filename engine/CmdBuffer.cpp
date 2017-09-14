/*
Copyright (C) 1996-1997 Id Software, Inc.
Copyright (C) 2017 Sh1ft0x0EF

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

#include "quakedef.hpp"
#include "CmdBuffer.hpp"
#include "CmdExecutor.hpp"

//=============================================================================

bool cmd_wait{false};

/*
============
Cmd_Wait_f

Causes execution of the remainder of the command buffer to be delayed until
next frame.  This allows commands like:
bind g "impulse 5 ; +attack ; wait ; -attack ; impulse 2"
============
*/
void Cmd_Wait_f()
{
	cmd_wait = true;
};

/*
=============================================================================

						COMMAND BUFFER

=============================================================================
*/

CCmdBuffer::CCmdBuffer(CCmdExecutor *apExecutor) : mpExecutor(apExecutor){}
CCmdBuffer::~CCmdBuffer() = default;

/*
============
Cbuf_Init
============
*/
void CCmdBuffer::Init()
{
	//SZ_Alloc(&cmd_text, 8192); // space for commands and script files
};

/*
============
Cbuf_AddText

Adds command text at the end of the buffer
============
*/
void CCmdBuffer::AddText(const char *text)
{
	auto l{Q_strlen(text)};

	if(cmd_text.cursize + l >= cmd_text.maxsize)
	{
		//Con_Printf("Cbuf_AddText: overflow\n"); // Com_Printf
		return;
	};

	cmd_text.Write(text, l);
};

/*
============
Cbuf_InsertText

Adds command text immediately after the current command
Adds a \n to the text
FIXME: actually change the command buffer to do less copying
============
*/
void CCmdBuffer::InsertText(const char *text)
{
	char *temp{nullptr};

	// copy off any commands still remaining in the exec buffer
	int templen{cmd_text.cursize};
	
	if(templen)
	{
		temp = (char*)Z_Malloc(templen);
		Q_memcpy(temp, cmd_text.data, templen);
		cmd_text.Clear();
	};

	// add the entire text of the file
	AddText(text);

	// add the copied off data
	if(templen)
	{
		cmd_text.Write(temp, templen);
		Z_Free(temp);
	};
};

/*
============
Cbuf_Execute
============
*/
void CCmdBuffer::Execute()
{
	int i{0};
	char *text{nullptr};
	char line[1024];
	int quotes{0};

	while(cmd_text.cursize)
	{
		// find a \n or ; line break
		text = (char *)cmd_text.data;

		quotes = 0;
		
		for(i = 0; i < cmd_text.cursize; i++)
		{
			if(text[i] == '"')
				quotes++;
			
			if(!(quotes & 1) && text[i] == ';')
				break; // don't break if inside a quoted string
			
			if(text[i] == '\n')
				break;
		};

		Q_memcpy(line, text, i);
		line[i] = 0;

		// delete the text from the command buffer and move remaining commands down
		// this is necessary because commands (exec, alias) can insert data at the
		// beginning of the text buffer

		if(i == cmd_text.cursize)
			cmd_text.cursize = 0;
		else
		{
			i++;
			cmd_text.cursize -= i;
			Q_memcpy(text, text + i, cmd_text.cursize);
		};

		// execute the command line
		mpExecutor->ExecString(line, src_command);

		if(cmd_wait)
		{
			// skip out while text still remains in buffer, leaving it
			// for next frame
			cmd_wait = false;
			break;
		};
	};
};