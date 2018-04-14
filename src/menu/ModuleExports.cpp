#include "commontypes.hpp"
#include "Menu.hpp"

C_EXPORT IMenu *GetMenu(IEngineInterface *apEngineInterface)
{
	static CMenu Menu(apEngineInterface);
	return &Menu;
};