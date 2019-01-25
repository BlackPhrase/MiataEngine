#pragma once

class CNetChannel
{
public:
	void Init();
	
	void Setup(netsrc_t sock, netchan_t *chan, const netadr_t &adr, int qport);

	bool NeedReliable(netchan_t *chan);
	
	void Transmit(netchan_t *chan, int length, const byte *data);
	
	void OutOfBand(int net_socket, netadr_t adr, int length, const byte *data);
	void OutOfBandPrint(int net_socket, netadr_t adr, const char *format, ...);
	
	bool Process(netchan_t *chan, sizebuf_t *msg);

	bool CanPacket(netchan_t *chan);
	bool CanReliable(netchan_t *chan);
};