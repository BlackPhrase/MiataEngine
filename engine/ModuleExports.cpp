#include "commontypes.hpp"
#include "Host.hpp"

C_EXPORT IHost *GetHost()
{
	static CHost Host;
	return &Host;
};