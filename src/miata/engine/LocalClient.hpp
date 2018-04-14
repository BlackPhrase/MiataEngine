/// @file

#pragma once

#include "engine/ILocalClient.hpp"

class CLocalClient final : public ILocalClient
{
public:
	CLocalClient();
	~CLocalClient();
	
	void Disconnect() override;
	
	bool IsConnected() const override;
private:
};