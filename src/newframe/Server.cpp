#include "quakedef.h"
#include "Server.hpp"


/*
=================
SV_ClientPrintf

Sends text across to be displayed 
FIXME: make this just a stuffed echo?
=================
*/
void CServer::ClientPrintf(client_t *host_client, const char *fmt, ...)
{
	va_list argptr;
	char string[1024]{};
	
	va_start(argptr, fmt);
	vsprintf(string, fmt, argptr);
	va_end(argptr);
	
	MSG_WriteByte(&host_client->netchan.message, svc_print);
	MSG_WriteString(&host_client->netchan.message, string);
};

/*
=================
Host_ClientCommands

Send text over to the client to be executed
=================
*/
void CServer::SendClientCmd(client_t *host_client, const char *fmt, ...)
{
	va_list argptr;
	char string[1024]{};
	
	va_start(argptr, fmt);
	vsprintf(string, fmt, argptr);
	va_end(argptr);
	
	MSG_WriteByte(&host_client->netchan.message, svc_stufftext);
	MSG_WriteString(&host_client->netchan.message, string);
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
	
	for(int i = 0; i < svs.maxclients; ++i)
		if(svs.clients[i].active && svs.clients[i].spawned)
		{
			MSG_WriteByte(&svs.clients[i].message, svc_print);
			MSG_WriteString(&svs.clients[i].message, string);
		};
};