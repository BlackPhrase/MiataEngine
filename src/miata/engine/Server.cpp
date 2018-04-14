/// @file

#include "quakedef.hpp"
#include "Server.hpp"

CServer::CServer() = default;
CServer::~CServer() = default;

/*
=====================
SV_DropClient

Called when the player is getting totally kicked off the host
if (crash = true), don't bother sending signofs
=====================
*/
void CServer::DropClient(client_t *pclient, bool abCrash, const char *asReason, ...)
{
	client_t *client;

	if(!abCrash)
	{
		// send any final messages (don't check for errors)
		if(NET_CanSendMessage(pclient->netconnection))
		{
			MSG_WriteByte(&pclient->message, svc_disconnect);
			NET_SendMessage(pclient->netconnection, &pclient->message);
		};

		if(pclient->edict && pclient->spawned)
		{
			// call the prog function for removing a client
			// this will set the body to a dead frame, among other things
			int saveSelf = pr_global_struct->self;
			pr_global_struct->self = EDICT_TO_PROG(pclient->edict);
			PR_ExecuteProgram(pr_global_struct->ClientDisconnect);
			pr_global_struct->self = saveSelf;
		};

		Sys_Printf("Client %s removed\n", pclient->name);
	};

	// break the net connection
	NET_Close(pclient->netconnection);
	pclient->netconnection = NULL;

	// free the client (the body stays around)
	pclient->active = false;
	pclient->name[0] = 0;
	pclient->old_frags = -999999;
	net_activeconnections--;

	// send notification to all clients
	for(int i = 0, client = svs.clients; i < svs.maxclients; i++, client++)
	{
		if(!client->active)
			continue;
		
		MSG_WriteByte(&client->message, svc_updatename);
		MSG_WriteByte(&client->message, pclient - svs.clients);
		MSG_WriteString(&client->message, "");
		MSG_WriteByte(&client->message, svc_updatefrags);
		MSG_WriteByte(&client->message, pclient - svs.clients);
		MSG_WriteShort(&client->message, 0);
		MSG_WriteByte(&client->message, svc_updatecolors);
		MSG_WriteByte(&client->message, pclient - svs.clients);
		MSG_WriteByte(&client->message, 0);
	};
};

/*
=================
SV_ClientPrintf

Sends text across to be displayed 
FIXME: make this just a stuffed echo?
=================
*/
void CServer::ClientPrintf(client_t *pclient, const char *fmt, ...)
{
	va_list argptr;
	char string[1024]{};

	va_start(argptr, fmt);
	vsprintf(string, fmt, argptr);
	va_end(argptr);

	MSG_WriteByte(&pclient->message, svc_print);
	MSG_WriteString(&pclient->message, string);
};

/*
=================
Host_ClientCommands

Send text over to the client to be executed
=================
*/
void CServer::ClientCommands(client_t *pclient, const char *fmt, ...)
{
	va_list argptr;
	char string[1024]{};

	va_start(argptr, fmt);
	vsprintf(string, fmt, argptr);
	va_end(argptr);

	MSG_WriteByte(&pclient->message, svc_stufftext);
	MSG_WriteString(&pclient->message, string);
};

/*
=================
SV_BroadcastPrintf

Sends text to all active clients
=================
*/
void CServer::BroadcastPrintf(const char *fmt, ...)
{
	va_list argptr;
	char string[1024]{};
	
	va_start(argptr, fmt);
	vsprintf(string, fmt, argptr);
	va_end(argptr);

	for(int i = 0; i < svs.maxclients; i++)
		if(svs.clients[i].active && svs.clients[i].spawned)
		{
			MSG_WriteByte(&svs.clients[i].message, svc_print);
			MSG_WriteString(&svs.clients[i].message, string);
		};
};