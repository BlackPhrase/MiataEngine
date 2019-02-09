#include "quakedef.h"
#include "Screen.hpp"

void CScreen::Init()
{
	SCR_Init();
};

void CScreen::Update()
{
	SCR_UpdateScreen();
};

void CScreen::SizeUp()
{
	SCR_SizeUp();
};

void CScreen::SizeDown()
{
	SCR_SizeDown();
};

void CScreen::BringDownConsole()
{
	SCR_BringDownConsole();
};

void CScreen::CenterPrint(const char *str)
{
	SCR_CenterPrint(str);
};

void CScreen::BeginLoadingPlaque()
{
	SCR_BeginLoadiingPlaque();
};

void CScreen::EndLoadingPlaque()
{
	SCR_EndLoadingPlaque();
};

int CScreen::ModalMessage(const char *text)
{
	return SCR_ModalMessage(text);
};

#ifdef _WIN32
void CScreen::UpdateWholeScreen()
{
	SCR_UpdateWholeScreen();
};
#endif