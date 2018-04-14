#pragma once

#include "network/INetwork.hpp"

class CNetwork final : public INetwork
{
public:
	CNetwork(IEngineInterface *apEngine);
	~CNetwork();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
	
	void Config(bool multiplayer) override;
private:
	IEngineInterface *mpEngine{nullptr};
};