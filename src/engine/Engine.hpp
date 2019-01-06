/// @file

#pragma once

#include "IEngine.hpp"

class CEngine final : public IEngine
{
public:
	CEngine();
	~CEngine();
	
	bool Init(const InitProps &apInitProps) override;
	void Frame() override;
private:
	double time{0.0}, oldtime{0.0}, newtime{0.0};
};