#include "quakedef.h"
#include "Input.hpp"

void CInput::Init()
{
	IN_Init();
};

void CInput::Shutdown()
{
	IN_Shutdown();
};

void CInput::Commands()
{
	IN_Commands();
};

void CInput::Move(usercmd_t *cmd)
{
	IN_Move(cmd);
};

void CInput::ClearStates()
{
	IN_ClearStates();
};

void CInput::ModeChanged()
{
	IN_ModeChanged();
};