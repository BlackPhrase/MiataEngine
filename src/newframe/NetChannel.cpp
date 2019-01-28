#include "NetChannel.hpp"

void CNetChannel::Init()
{
};

void CNetChannel::Setup(netsrc_t sock, netchan_t *chan, const netadr_t &adr, int qport)
{
};

bool CNetChannel::NeedReliable(netchan_t *chan)
{
};

void CNetChannel::Transmit(netchan_t *chan, int length, const byte *data)
{
};

void CNetChannel::OutOfBand(int net_socket, netadr_t adr, int length, const byte *data)
{
};

void CNetChannel::OutOfBandPrint(int net_socket, netadr_t adr, const char *format, ...)
{
};

bool CNetChannel::Process(netchan_t *chan, sizebuf_t *msg)
{
};

bool CNetChannel::CanPacket(netchan_t *chan)
{
};

bool CNetChannel::CanReliable(netchan_t *chan)
{
};