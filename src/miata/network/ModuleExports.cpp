#include "commontypes.hpp"
#include "Network.hpp"

C_EXPORT INetwork *GetNetwork(IEngineInterface *apEngineInterface)
{
	static CNetwork Network(apEngineInterface);
	return &Network;
};