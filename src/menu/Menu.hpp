#pragma once

#include "menu/IMenu.hpp"

class CMenu final : public IMenu
{
public:
	CMenu(IEngineInterface *apEngine);
	~CMenu();

	bool Init() override;
	void Shutdown() override;

	void Frame() override;
private:
	IEngineInterface *mpEngine{nullptr};
};