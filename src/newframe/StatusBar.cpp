#include "StatusBar.hpp"

void CStatusBar::Init()
{
	Sbar_Init();
};

void CStatusBar::Draw()
{
	Sbar_Draw();
};

void CStatusBar::Changed()
{
	Sbar_Changed();
};

void CStatusBar::IntermissionOverlay()
{
	Sbar_IntermissionOverlay();
};

void CStatusBar::FinaleOverlay()
{
	Sbar_FinaleOverlay();
};