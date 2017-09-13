#pragma once

#include "IInput.hpp"

class CInput final : public IInput
{
public:
	CInput();
	~CInput();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
};