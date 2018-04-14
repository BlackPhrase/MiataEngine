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

#include "quakedef.hpp"

bool CClient::Connect()
{
	return true;
};

bool CClient::Reconnect()
{
	// don't change when downloading
	if(cls.download)
		return false;

	S_StopAllSounds(true);

	if(cls.state == ca_connected)
	{
		mpLogger->Printf ("reconnecting...\n");
		MSG_WriteChar (&cls.netchan.message, clc_stringcmd);
		MSG_WriteString (&cls.netchan.message, "new");
		return true;
	};

	if(!*cls.servername)
	{
		mpLogger->Printf("No server to reconnect to...\n");
		return false;
	};

	CL_Disconnect();
	CL_BeginServerConnect();
	
	return true;
};

/*
=======================
CL_SendConnectPacket

called by CL_Connect_f and CL_CheckResend
======================
*/
void CClient::SendConnectPacket()
{
	netadr_t	adr;
	char	data[2048];
	double t1, t2;
// JACK: Fixed bug where DNS lookups would cause two connects real fast
//       Now, adds lookup time to the connect time.
//		 Should I add it to realtime instead?!?!

	if (cls.state != ca_disconnected)
		return;

	t1 = Sys_DoubleTime ();

	if (!NET_StringToAdr (cls.servername, &adr))
	{
		mpLogger->Printf ("Bad server address\n");
		connect_time = -1;
		return;
	}

	if (adr.port == 0)
		adr.port = BigShort (27500);
	
	t2 = Sys_DoubleTime ();

	connect_time = realtime+t2-t1;	// for retransmit requests

	cls.qport = Cvar_VariableValue("qport");

	Info_SetValueForStarKey (cls.userinfo, "*ip", NET_AdrToString(adr), MAX_INFO_STRING);

//	mpLogger->Printf ("Connecting to %s...\n", cls.servername);
	sprintf (data, "%c%c%c%cconnect %i %i %i \"%s\"\n",
		255, 255, 255, 255,	PROTOCOL_VERSION, cls.qport, cls.challenge, cls.userinfo);
	NET_SendPacket (strlen(data), data, adr);
};

/*
=================
CL_CheckForResend

Resend a connect message if the last one has timed out

=================
*/
void CClient::CheckForResend ()
{
	netadr_t	adr;
	char	data[2048];
	double t1, t2;

	if (connect_time == -1)
		return;
	if (cls.state != ca_disconnected)
		return;
	if (connect_time && realtime - connect_time < 5.0)
		return;

	t1 = Sys_DoubleTime ();
	
	if (!NET_StringToAdr (cls.servername, &adr))
	{
		mpLogger->Printf ("Bad server address\n");
		connect_time = -1;
		return;
	};

	if (adr.port == 0)
		adr.port = BigShort (27500);
	t2 = Sys_DoubleTime ();

	connect_time = realtime+t2-t1;	// for retransmit requests

	mpLogger->Printf ("Connecting to %s...\n", cls.servername);
	sprintf (data, "%c%c%c%cgetchallenge\n", 255, 255, 255, 255);
	NET_SendPacket (strlen(data), data, adr);
};

void CClient::BeginServerConnect()
{
	connect_time = 0;
	CheckForResend();
};

/*
=====================
CL_Disconnect

Sends a disconnect message to the server
This is also called on Host_Error, so it shouldn't cause any errors
=====================
*/
void CClient::Disconnect()
{
	// stop sounds (especially looping!)
	S_StopAllSounds(true);

	// bring the console down and fade the colors back to normal
	//	SCR_BringDownConsole ();

	// if running a local server, shut it down
	if(cls.demoplayback)
		CL_StopPlayback();
	else if(cls.state == ca_connected)
	{
		if(cls.demorecording)
			CL_Stop_f();

		mpLogger->DPrintf("Sending clc_disconnect\n");
		SZ_Clear(&cls.netchan.message);
		MSG_WriteByte(&cls.netchan.message, clc_disconnect);
		NET_SendUnreliableMessage(cls.netchan, &cls.netchan.message);
		SZ_Clear(&cls.netchan.message);
		NET_Close(cls.netchan);

		cls.state = ca_disconnected;
		
		if(sv.active)
			Host_ShutdownServer(false);
	};

	cls.demoplayback = cls.timedemo = false;
	cls.signon = 0;
};