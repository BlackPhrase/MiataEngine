#include "commontypes.hpp"
#include "Menu.hpp"

C_EXPORT IMenu *GetMenu()
{
	static CMenu Menu;
	return &Menu;
};