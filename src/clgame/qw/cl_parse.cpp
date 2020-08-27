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
/// @brief parse a message received from the server

/*
==================
CL_ParseClientdata

Server information pertaining to this client only
==================
*/
void CL_ParseClientdata (int bits)
{
	int		i, j;
	
	if (bits & SU_VIEWHEIGHT)
		cl.viewheight = MSG_ReadChar (net_message);
	else
		cl.viewheight = DEFAULT_VIEWHEIGHT;

	if (bits & SU_IDEALPITCH)
		cl.idealpitch = MSG_ReadChar (net_message);
	else
		cl.idealpitch = 0;
	
	VectorCopy (cl.mvelocity[0], cl.mvelocity[1]);
	for (i=0 ; i<3 ; i++)
	{
		if (bits & (SU_PUNCH1<<i) )
			cl.punchangle[i] = MSG_ReadChar(net_message);
		else
			cl.punchangle[i] = 0;
		if (bits & (SU_VELOCITY1<<i) )
			cl.mvelocity[0][i] = MSG_ReadChar(net_message)*16;
		else
			cl.mvelocity[0][i] = 0;
	}

// [always sent]	if (bits & SU_ITEMS)
		i = MSG_ReadLong (net_message);

	if (cl.items != i)
	{	// set flash times
		Sbar_Changed ();
		for (j=0 ; j<32 ; j++)
			if ( (i & (1<<j)) && !(cl.items & (1<<j)))
				cl.item_gettime[j] = cl.time;
		cl.items = i;
	}
		
	cl.onground = (bits & SU_ONGROUND) != 0;
	cl.inwater = (bits & SU_INWATER) != 0;

	if (bits & SU_WEAPONFRAME)
		cl.stats[STAT_WEAPONFRAME] = MSG_ReadByte (net_message);
	else
		cl.stats[STAT_WEAPONFRAME] = 0;

	if (bits & SU_ARMOR)
		i = MSG_ReadByte (net_message);
	else
		i = 0;
	if (cl.stats[STAT_ARMOR] != i)
	{
		cl.stats[STAT_ARMOR] = i;
		Sbar_Changed ();
	}

	if (bits & SU_WEAPON)
		i = MSG_ReadByte (net_message);
	else
		i = 0;
	if (cl.stats[STAT_WEAPON] != i)
	{
		cl.stats[STAT_WEAPON] = i;
		Sbar_Changed ();
	}
	
	i = MSG_ReadShort (net_message);
	if (cl.stats[STAT_HEALTH] != i)
	{
		cl.stats[STAT_HEALTH] = i;
		Sbar_Changed ();
	}

	i = MSG_ReadByte (net_message);
	if (cl.stats[STAT_AMMO] != i)
	{
		cl.stats[STAT_AMMO] = i;
		Sbar_Changed ();
	}

	for (i=0 ; i<4 ; i++)
	{
		j = MSG_ReadByte (net_message);
		if (cl.stats[STAT_SHELLS+i] != j)
		{
			cl.stats[STAT_SHELLS+i] = j;
			Sbar_Changed ();
		}
	}

	i = MSG_ReadByte (net_message);

	if (standard_quake)
	{
		if (cl.stats[STAT_ACTIVEWEAPON] != i)
		{
			cl.stats[STAT_ACTIVEWEAPON] = i;
			Sbar_Changed ();
		}
	}
	else
	{
		if (cl.stats[STAT_ACTIVEWEAPON] != (1<<i))
		{
			cl.stats[STAT_ACTIVEWEAPON] = (1<<i);
			Sbar_Changed ();
		}
	}
}

/*
=====================
CL_ParseServerMessage
=====================
*/
bool CL_ParseServerMessage (int cmd, sizebuf_t &net_message)
{
	int i, j;
	
	// other commands
	switch(cmd)
	{
	default:
		return false;

	case svc_disconnect:
		// TODO: qw
		//if (cls.state == ca_connected)
			//Host_EndGame ("Server disconnected\n"
				//"Server version may not be compatible");
		//else
			Host_EndGame ("Server disconnected\n");

	case svc_print:
		i = MSG_ReadByte (net_message);
		if (i == PRINT_CHAT)
		{
			S_LocalSound ("misc/talk.wav");
			//con_ormask = 128; // TODO
		}
		Con_Printf ("%s", MSG_ReadString (net_message));
		//con_ormask = 0; // TODO
		break;

	case svc_centerprint:
		SCR_CenterPrint (MSG_ReadString (net_message));
		break;
		
	case svc_stufftext:
		s = MSG_ReadString(net_message);
		//Con_DPrintf ("stufftext: %s\n", s); // TODO: qw
		Cbuf_AddText (s);
		break;

	case svc_damage:
		V_ParseDamage ();
		break;

	case svc_serverdata:
		Cbuf_Execute ();		// make sure any stuffed commands are done
		CL_ParseServerData ();
		vid.recalc_refdef = true;	// leave full screen intermission
		break;

	case svc_setangle:
		for (i=0 ; i<3 ; i++)
			cl.viewangles[i] = MSG_ReadAngle (net_message);
		//cl.viewangles[PITCH] = cl.viewangles[ROLL] = 0;
		break;

	case svc_lightstyle:
		i = MSG_ReadByte (net_message);
		if (i >= MAX_LIGHTSTYLES)
			Sys_Error ("svc_lightstyle > MAX_LIGHTSTYLES");
		Q_strcpy (cl_lightstyle[i].map,  MSG_ReadString(net_message));
		cl_lightstyle[i].length = Q_strlen(cl_lightstyle[i].map);
		break;

	case svc_sound:
		CL_ParseStartSoundPacket();
		break;
		
	case svc_stopsound:
		i = MSG_ReadShort(net_message);
		S_StopSound(i>>3, i&7);
		break;

	case svc_updatefrags:
		Sbar_Changed ();
		i = MSG_ReadByte (net_message);
		if (i >= cl.maxclients) // TODO: MAX_CLIENTS in qw
			Host_Error ("CL_ParseServerMessage: svc_updatefrags > MAX_SCOREBOARD"); // TODO: Host_EndGame in qw
		cl.scores[i].frags = MSG_ReadShort (net_message); // TODO: cl.players[i].frags in qw
		break;

	case svc_updateping:
		i = MSG_ReadByte (net_message);
		if (i >= MAX_CLIENTS)
			Host_EndGame ("CL_ParseServerMessage: svc_updateping > MAX_SCOREBOARD");
		cl.players[i].ping = MSG_ReadShort (net_message);
		break;

	case svc_updatepl:
		i = MSG_ReadByte (net_message);
		if (i >= MAX_CLIENTS)
			Host_EndGame ("CL_ParseServerMessage: svc_updatepl > MAX_SCOREBOARD");
		cl.players[i].pl = MSG_ReadByte (net_message);
		break;
		
	case svc_updateentertime:
	// time is sent over as seconds ago
		i = MSG_ReadByte (net_message);
		if (i >= MAX_CLIENTS)
			Host_EndGame ("CL_ParseServerMessage: svc_updateentertime > MAX_SCOREBOARD");
		cl.players[i].entertime = realtime - MSG_ReadFloat (net_message);
		break;

	case svc_spawnbaseline:
		i = MSG_ReadShort (net_message);
		CL_ParseBaseline (&cl_baselines[i]);
		break;
	case svc_spawnstatic:
		CL_ParseStatic ();
		break;			
	case svc_temp_entity:
		CL_ParseTEnt ();
		break;

	case svc_killedmonster:
		cl.stats[STAT_MONSTERS]++;
		break;

	case svc_foundsecret:
		cl.stats[STAT_SECRETS]++;
		break;

	case svc_updatestat:
		i = MSG_ReadByte (net_message);
		//if (i < 0 || i >= MAX_CL_STATS)
			//Sys_Error ("svc_updatestat: %i is invalid", i);
		//cl.stats[i] = MSG_ReadByte (net_message);
		j = MSG_ReadByte (net_message);
		CL_SetStat (i, j);
		break;
	case svc_updatestatlong:
		i = MSG_ReadByte (net_message);
		j = MSG_ReadLong (net_message);
		CL_SetStat (i, j);
		break;

	case svc_spawnstaticsound:
		CL_ParseStaticSound ();
		break;

	case svc_cdtrack:
		cl.cdtrack = MSG_ReadByte (net_message);
		CDAudio_Play ((byte)cl.cdtrack, true);
		break;

	case svc_intermission:
		cl.intermission = 1;
		cl.completed_time = cl.time; // TODO: realtime in QW
		vid.recalc_refdef = true;	// go to full screen
		for (i=0 ; i<3 ; i++)
			cl.simorg[i] = MSG_ReadCoord (net_message);			
		for (i=0 ; i<3 ; i++)
			cl.simangles[i] = MSG_ReadAngle (net_message);
		VectorCopy (vec3_origin, cl.simvel);
		break;

	case svc_finale:
		cl.intermission = 2;
		cl.completed_time = cl.time; // TODO: realtime in QW
		vid.recalc_refdef = true;	// go to full screen
		SCR_CenterPrint (MSG_ReadString (net_message));			
		break;

	case svc_sellscreen:
		Cmd_ExecuteString ("help", src_command); // TODO: Cmd_ExecuteString ("help"); in qw
		break;

	case svc_smallkick:
		cl.punchangle = -2;
		break;
	case svc_bigkick:
		cl.punchangle = -4;
		break;

	case svc_muzzleflash:
		CL_MuzzleFlash ();
		break;

	case svc_updateuserinfo:
		CL_UpdateUserinfo ();
		break;

	case svc_setinfo:
		CL_SetInfo ();
		break;

	case svc_serverinfo:
		CL_ServerInfo ();
		break;

	case svc_download:
		CL_ParseDownload ();
		break;

	case svc_playerinfo:
		CL_ParsePlayerinfo ();
		break;

	case svc_nails:
		CL_ParseProjectiles ();
		break;

	case svc_chokecount:		// some preceding packets were choked
		i = MSG_ReadByte (net_message);
		for (j=0 ; j<i ; j++)
			cl.frames[ (cls.netchan.incoming_acknowledged-1-j)&UPDATE_MASK ].receivedtime = -2;
		break;

	case svc_modellist:
		CL_ParseModellist ();
		break;

	case svc_soundlist:
		CL_ParseSoundlist ();
		break;

	case svc_packetentities:
		CL_ParsePacketEntities (false);
		break;

	case svc_deltapacketentities:
		CL_ParsePacketEntities (true);
		break;

	case svc_maxspeed :
		movevars.maxspeed = MSG_ReadFloat(net_message);
		break;

	case svc_entgravity :
		movevars.entgravity = MSG_ReadFloat(net_message);
		break;

	case svc_setpause:
		cl.paused = MSG_ReadByte (net_message);
		if (cl.paused)
			CDAudio_Pause ();
		else
			CDAudio_Resume ();
		break;

	//case svc_setview:
		//cl.viewentity = MSG_ReadShort (net_message);
		//break;
	};
	
	return true;
};