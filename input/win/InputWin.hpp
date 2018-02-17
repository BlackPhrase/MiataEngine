#pragma once

#include "IInputImpl.hpp"

class CMouseWin;

class CInputWin final : public IInputImpl
{
public:
	CInputWin();
	~CInputWin();
	
	void Init() override;
	void Shutdown() override;
	
	void HandleMessage(void *apData) override;
private:
	CMouseWin *mpMouse;
};