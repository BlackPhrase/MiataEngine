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

#pragma once

constexpr auto PORT_ANY = -1;

//extern cvar_t hostname;

extern int net_socket;

//bool NET_CompareAdr(netadr_t a, netadr_t b);
//bool NET_CompareBaseAdr(netadr_t a, netadr_t b);

//char *NET_BaseAdrToString(netadr_t a);

//============================================================================

#define OLD_AVG 0.99 // total = oldtotal*OLD_AVG + new*(1-OLD_AVG)

typedef struct
{

	int			last_received;

	byte		message_buf[MAX_MSGLEN-16];		// leave space for header

	byte		reliable_buf[MAX_MSGLEN-16];	// unacked reliable message
} netchan_t;

extern	netadr_t	net_from;
extern	sizebuf_t	net_message;
extern	byte		net_message_buffer[MAX_MSGLEN];

//extern CNetAdr net_local_adr;
//extern CNetAdr net_from; // address of who sent the packet
//extern CSizeBuffer net_message;

qboolean Netchan_NeedReliable (netchan_t *chan);

extern int net_drop; // packets dropped before this one