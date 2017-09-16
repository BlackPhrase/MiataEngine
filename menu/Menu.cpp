#include "Menu.hpp"

CMenu::CMenu(IEngineInterface *apEngine) : mpEngine(apEngine){}
CMenu::~CMenu() = default;

bool CMenu::Init()
{
	mpEngine->GetLogger()->Printf("Menu initialization...");
	
	return true;
};

void CMenu::Shutdown()
{
	mpEngine->GetLogger()->Printf("Menu shutdown...");
};

void CMenu::Frame()
{
	
};