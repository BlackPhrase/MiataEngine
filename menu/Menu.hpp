#pragma once

#include "Menu.hpp"

class CMenu final : public IMenu
{
public:
	CMenu();
	~CMenu();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
};