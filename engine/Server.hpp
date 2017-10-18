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

/// @file

#pragma once

class CServer final
{
public:
	CServer();
	~CServer();
	
	void Update();
	
	void DropClient(client_t *pclient, bool crash, const char *reason, ...);
	void ClientPrintf(client_t *pclient, const char *fmt, ...);
	
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