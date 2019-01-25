#pragma once

class CNet
{
public:
	void Init();
	void Shutdown();

	void Config(bool multiplayer);

	void Sleep(int msec);
	
	bool GetPacket(netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message);
	void SendPacket(netsrc_t sock, int length, const void *data, netadr_t to);

	bool CompareAdr(const netadr_t &a, const netadr_t &b);
	bool CompareBaseAdr(const netadr_t &a, const netadr_t &b);
	
	bool IsLocalAddress(const netadr_t &adr);
	
	char *AdrToString(const netadr_t &a);
	bool StringToAdr(const char *s, netadr_t *a);
};