#pragma once

#include "INetwork.hpp"

class CNetwork final : public INetwork
{
public:
	CNetwork(IEngineInterface *apEngine);
	~CNetwork();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
private:
	IEngineInterface *mpEngine{nullptr};
};