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
/// @brief server component

#pragma once

#include "usercmd.hpp"
#include "SizeBuffer.hpp"

#define NUM_PING_TIMES 16
#define NUM_SPAWN_PARMS 16

//=============================================================================

typedef struct client_s
{
	bool active;     // false = client is free
	bool spawned;    // false = don't send datagrams
	bool dropasap;   // has been told to go to another level
	bool privileged; // can execute any host command
	bool sendsignon; // only valid before spawned

	double last_message; // reliable messages must be sent
	                     // periodically

	struct qsocket_s *netconnection; // communications handle

	usercmd_t cmd;  // movement
	vec3_t wishdir; // intended motion calced from cmd

	CSizeBuffer message; // can be added to at any time,
	                   // copied and clear once per frame
	byte msgbuf[MAX_MSGLEN];
	edict_t *edict; // EDICT_NUM(clientnum+1)
	char name[32];  // for printing to other people
	int colors;

	float ping_times[NUM_PING_TIMES];
	int num_pings; // ping_times[num_pings%NUM_PING_TIMES]

	// spawn parms are carried from level to level
	float spawn_parms[NUM_SPAWN_PARMS];

	// client known data for deltas
	int old_frags;
} client_t;

//=============================================================================

class CServer final
{
public:
	CServer();
	~CServer();
	
	void Init();
	
	void Update();
	
	void DropClient(client_t *apClient, bool abCrash, const char *asReason, ...);
	void ClientPrintf(client_t *apClient, const char *fmt, ...);
	
	client_t *GetClientByIndex(int id) const;
	
	void BroadcastPrintf(const char *fmt, ...);
	
	void CheckForNewClients();
	
	void SendClientMessages();
	
	int GetMaxClients() const;
private:
	struct client_s *clients{nullptr}; ///< [maxclients]
	
	int maxclients{0};
	//int maxclientslimit{0};
};

extern CServer sv; // local server