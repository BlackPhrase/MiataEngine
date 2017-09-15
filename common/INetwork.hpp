/*
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
/// @brief engine's interface to the networking layer

#pragma once

#include "commontypes.hpp"

/*
==============================================================

NET

==============================================================
*/

enum class netadrtype_t : int
{
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX
};

struct netadr_t
{
	netadrtype_t type{netadrtype_t::NA_LOOPBACK};
	
	byte ip[4]{};
	byte ipx[10]{};
	
	unsigned short port{0};
};

struct INetAdr
{
	///
	virtual bool IsLocal() const = 0;
	
	///
	virtual const char *ToString() const = 0;
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
	//virtual void Sleep(int msec) = 0;
	
	///
	//virtual void SendPacket(const void *data, int length, INetAdr *to) = 0;
	
	///
	//virtual bool GetPacket() = 0;

	///
	//virtual INetAdr *StringToAdr(const char *asAdr) = 0;
	//virtual bool StringToAdr(const char *asAdr, INetAdr &aAdr) = 0;
};

using pfnGetNetwork = INetwork *(*)();

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
	virtual bool Process() = 0;

	///
	virtual bool CanPacket() = 0;

	///
	virtual bool CanReliable() = 0;
};