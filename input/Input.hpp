#pragma once

#include "IInput.hpp"

class CInput final : public IInput
{
public:
	CInput(IEngineInterface *apEngine);
	~CInput();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
private:
	IEngineInterface *mpEngine{nullptr};
};