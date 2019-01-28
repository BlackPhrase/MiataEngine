#include "Screen.hpp"

void CScreen::Init()
{
};

void CScreen::Update()
{
};

void CScreen::SizeUp()
{
};

void CScreen::SizeDown()
{
};

void CScreen::BringDownConsole()
{
};

void CScreen::CenterPrint(const char *str)
{
};

void CScreen::BeginLoadingPlaque()
{
};

void CScreen::EndLoadingPlaque()
{
};

int CScreen::ModalMessage(const char *text)
{
};

#ifdef _WIN32
void CScreen::UpdateWholeScreen()
{
};
#endif