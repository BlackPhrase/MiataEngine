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

#include "NetMsg.hpp"

/*
==============================================================================

			MESSAGE IO FUNCTIONS

Handles byte ordering and avoids alignment errors
==============================================================================
*/

//
// writing functions
//

void CNetMsg::WriteChar(sizebuf_t *sb, int c)
{
	byte *buf;

#ifdef PARANOID
	if(c < -128 || c > 127)
		Sys_Error("MSG_WriteChar: range error");
#endif

	buf = SZ_GetSpace(sb, 1);
	buf[0] = c;
};

void CNetMsg::WriteByte(sizebuf_t *sb, int c)
{
	byte *buf;

#ifdef PARANOID
	if(c < 0 || c > 255)
		Sys_Error("MSG_WriteByte: range error");
#endif

	buf = SZ_GetSpace(sb, 1);
	buf[0] = c;
};

void CNetMsg::WriteShort(sizebuf_t *sb, int c)
{
	byte *buf;

#ifdef PARANOID
	if(c < ((short)0x8000) || c > (short)0x7fff)
		Sys_Error("MSG_WriteShort: range error");
#endif

	buf = SZ_GetSpace(sb, 2);
	buf[0] = c & 0xff;
	buf[1] = c >> 8;
};

void CNetMsg::WriteLong(sizebuf_t *sb, int c)
{
	byte *buf;

	buf = SZ_GetSpace(sb, 4);
	buf[0] = c & 0xff;
	buf[1] = (c >> 8) & 0xff;
	buf[2] = (c >> 16) & 0xff;
	buf[3] = c >> 24;
};

void CNetMsg::WriteFloat(sizebuf_t *sb, float f)
{
	union
	{
		float f;
		int l;
	} dat;

	dat.f = f;
	dat.l = LittleLong(dat.l);

	SZ_Write(sb, &dat.l, 4);
};

void CNetMsg::WriteString(sizebuf_t *sb, const char *s)
{
	if(!s)
		SZ_Write(sb, "", 1);
	else
		SZ_Write(sb, s, Q_strlen(s) + 1);
};

void CNetMsg::WriteCoord(sizebuf_t *sb, float f)
{
	WriteShort(sb, (int)(f * 8));
};

void CNetMsg::WriteAngle(sizebuf_t *sb, float f)
{
	WriteByte(sb, ((int)f * 256 / 360) & 255);
};

//
// reading functions
//

void CNetMsg::BeginReading()
{
	msg_readcount = 0;
	msg_badread = false;
};

// returns -1 and sets msg_badread if no more characters are available
int CNetMsg::ReadChar()
{
	int c;

	if(msg_readcount + 1 > net_message.cursize)
	{
		msg_badread = true;
		return -1;
	};

	c = (signed char)net_message.data[msg_readcount];
	msg_readcount++;

	return c;
};

int CNetMsg::ReadByte()
{
	int c;

	if(msg_readcount + 1 > net_message.cursize)
	{
		msg_badread = true;
		return -1;
	};

	c = (unsigned char)net_message.data[msg_readcount];
	msg_readcount++;

	return c;
};

int CNetMsg::ReadShort()
{
	int c;

	if(msg_readcount + 2 > net_message.cursize)
	{
		msg_badread = true;
		return -1;
	};

	c = (short)(net_message.data[msg_readcount] + (net_message.data[msg_readcount + 1] << 8));

	msg_readcount += 2;

	return c;
};

int CNetMsg::ReadLong()
{
	int c;

	if(msg_readcount + 4 > net_message.cursize)
	{
		msg_badread = true;
		return -1;
	};

	c = net_message.data[msg_readcount] + (net_message.data[msg_readcount + 1] << 8) + (net_message.data[msg_readcount + 2] << 16) + (net_message.data[msg_readcount + 3] << 24);

	msg_readcount += 4;

	return c;
};

float CNetMsg::ReadFloat()
{
	union
	{
		byte b[4];
		float f;
		int l;
	} dat;

	dat.b[0] = net_message.data[msg_readcount];
	dat.b[1] = net_message.data[msg_readcount + 1];
	dat.b[2] = net_message.data[msg_readcount + 2];
	dat.b[3] = net_message.data[msg_readcount + 3];
	msg_readcount += 4;

	dat.l = LittleLong(dat.l);

	return dat.f;
};

char *CNetMsg::ReadString()
{
	static char string[2048]{};
	int l, c;

	l = 0;
	
	do
	{
		c = ReadChar();
		if(c == -1 || c == 0)
			break;
		string[l] = c;
		l++;
	}
	while(l < sizeof(string) - 1);

	string[l] = 0;

	return string;
};

float CNetMsg::ReadCoord()
{
	return ReadShort() * (1.0 / 8);
};

float CNetMsg::ReadAngle()
{
	return ReadChar() * (360.0 / 256);
};