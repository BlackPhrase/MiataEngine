/// @file

#pragma once

class CServer
{
public:
	void Shutdown();
	
	void ClientPrintf(client_t *host_client, const char *fmt, ...);
	void SendClientCmd(client_t *host_client, const char *fmt, ...);
	
	void BroadcastPrintf(const char *fmt, ...);
private:
};