/// @file
/// @brief engine's interface to the networking layer

#pragma once

struct netadr_t
{
	byte ip[4];
	unsigned short port;
	unsigned short pad;
};

struct INetwork
{
	///
	virtual bool Init() = 0;
	
	///
	virtual void Shutdown() = 0;
	
	///
	virtual void Frame() = 0;
	
	///
	virtual INetAdr *StringToAdr(const char *asAdr) = 0;
	//virtual bool StringToAdr(const char *asAdr, INetAdr &aAdr) = 0;
};

struct INetMsg
{
};

struct INetClient
{
	///
	virtual bool Connect(const char *asHostName) = 0;
	
	///
	virtual void Disconnect(const char *asReason, ...) = 0;
	
	///
	virtual bool Reconnect() = 0;
	
	///
	virtual void SendMsg(const INetMsg &aMsg) = 0;
	
	///
	virtual bool IsConnected() const = 0;
};

struct INetServer
{
	///
	virtual void BroadcastMsg(const INetMsg &aMsg) = 0;
};

struct INetChannel
{
	///
	virtual void Init() = 0;
	
	///
	virtual void Setup(netadr_t adr, int qport) = 0;
	
	///
	virtual void Transmit(int length, byte *data) = 0;
	
	///
	bool Process() = 0;
	
	///
	virtual bool CanPacket() = 0;
	
	///
	virtual bool CanReliable() = 0;
};

struct INetAdr
{
	///
	virtual const char *ToString() const = 0;
	
	///
	virtual bool IsLocal() const = 0;
};