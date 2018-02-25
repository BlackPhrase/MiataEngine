/// @file

#pragma once

#include "NetChannel.hpp"

class CNetClient final : public INetClient
{
public:
	CNetClient();
	~CNetClient();
	
	bool NET_GetPacket(netadr_t *net_from, CSizeBuffer *net_message) override;
	void NET_SendPacket(int length, const void *data, netadr_t to) override;

	INetChannel *GetChan() const override {return &mChan;}
private:
	CNetChannel mChan;
};