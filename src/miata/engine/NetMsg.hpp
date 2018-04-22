/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/// @file
/// @brief network message

#pragma once

class CNetMsg
{
public:
	CNetMsg();
	~CNetMsg();
	
	void WriteChar(sizebuf_t *sb, int c);
	void WriteByte(sizebuf_t *sb, int c);
	void WriteShort(sizebuf_t *sb, int c);
	void WriteLong(sizebuf_t *sb, int c);
	void WriteFloat(sizebuf_t *sb, float f);
	void WriteString(sizebuf_t *sb, const char *s);
	void WriteCoord(sizebuf_t *sb, float f);
	void WriteAngle(sizebuf_t *sb, float f);

	void BeginReading();

	int ReadChar();
	int ReadByte();
	int ReadShort();
	int ReadLong();
	float ReadFloat();
	char *ReadString();

	float ReadCoord();
	float ReadAngle();
private:
	int msg_readcount{0};
	
	bool msg_badread{false}; ///< Set if a read goes beyond end of message
};