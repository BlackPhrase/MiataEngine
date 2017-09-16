#include "Network.hpp"

CNetwork::CNetwork(IEngineInterface *apEngine) : mpEngine(apEngine){}
CNetwork::~CNetwork() = default;

bool CNetwork::Init()
{
	mpEngine->GetLogger()->Printf("Network initialization...");
	return true;
};

void CNetwork::Shutdown()
{
	mpEngine->GetLogger()->Printf("Network shutdown...");
};

void CNetwork::Frame()
{
};