#include "quakedef.h"
#include "Menu.hpp"

void CMenu::Init()
{
	M_Init();
};

void CMenu::Draw()
{
	M_Draw();
};

void CMenu::Keydown(int key)
{
	M_Keydown(key);
};

void M_ToggleMenu_f()
{
};