/// @file

#include "quakedef.h"
#include "in_linux.h"

/*****************************************************************************/
/* INPUT                                                                     */
/*****************************************************************************/

/** MOUSE *****************************************************************/

in_state_t in_state{};

cvar_t	*in_joystick;

// This if fake, it's acutally done by the Refresh load
void IN_Init (void)
{
	in_joystick	= Cvar_Get ("in_joystick", "0", CVAR_ARCHIVE);
};

void Real_IN_Init (void)
{
	if (RW_IN_Init_fp)
		RW_IN_Init_fp(&in_state);
};

void IN_Shutdown (void)
{
	if (RW_IN_Shutdown_fp)
		RW_IN_Shutdown_fp();
};

void IN_Commands (void)
{
	if (RW_IN_Commands_fp)
		RW_IN_Commands_fp();
};

void IN_Move (usercmd_t *cmd)
{
	if (RW_IN_Move_fp)
		RW_IN_Move_fp(cmd);
};

void IN_Frame (void)
{
	if (RW_IN_Frame_fp)
		RW_IN_Frame_fp();
};

void IN_Activate (qboolean active)
{
	if (RW_IN_Activate_fp)
		RW_IN_Activate_fp(active);
};

void Do_Key_Event(int key, qboolean down)
{
	Key_Event(key, down, Sys_Milliseconds());
};