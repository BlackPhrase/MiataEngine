#include "commontypes.hpp"
#include "Network.hpp"

C_EXPORT INetwork *GetNetwork()
{
	static CNetwork Network;
	return &Network;
};