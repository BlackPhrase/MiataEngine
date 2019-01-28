#include "Net.hpp"

void CNet::Init()
{
};

void CNet::Shutdown()
{
};

void CNet::Config(bool multiplayer)
{
};

void CNet::Sleep(int msec)
{
};

bool CNet::GetPacket(netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message)
{
};

void CNet::SendPacket(netsrc_t sock, int length, const void *data, netadr_t to)
{
};

bool CNet::CompareAdr(const netadr_t &a, const netadr_t &b)
{
};

bool CNet::CompareBaseAdr(const netadr_t &a, const netadr_t &b)
{
};

bool CNet::IsLocalAddress(const netadr_t &adr)
{
};

char *CNet::AdrToString(const netadr_t &a)
{
};

bool CNet::StringToAdr(const char *s, netadr_t *a)
{
};