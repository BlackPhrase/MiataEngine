#pragma once

#include "INetwork.hpp"

class CNetwork final : public INetwork
{
public:
	CNetwork();
	~CNetwork();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
};