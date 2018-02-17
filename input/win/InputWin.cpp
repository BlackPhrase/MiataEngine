#include "InputWin.hpp"

/*
===========
IN_Init
===========
*/
void CInputWin::Init()
{
	// joystick variables
	Cvar_RegisterVariable(&in_joystick);
	Cvar_RegisterVariable(&joy_name);
	Cvar_RegisterVariable(&joy_advanced);
	Cvar_RegisterVariable(&joy_advaxisx);
	Cvar_RegisterVariable(&joy_advaxisy);
	Cvar_RegisterVariable(&joy_advaxisz);
	Cvar_RegisterVariable(&joy_advaxisr);
	Cvar_RegisterVariable(&joy_advaxisu);
	Cvar_RegisterVariable(&joy_advaxisv);
	Cvar_RegisterVariable(&joy_forwardthreshold);
	Cvar_RegisterVariable(&joy_sidethreshold);
	Cvar_RegisterVariable(&joy_pitchthreshold);
	Cvar_RegisterVariable(&joy_yawthreshold);
	Cvar_RegisterVariable(&joy_forwardsensitivity);
	Cvar_RegisterVariable(&joy_sidesensitivity);
	Cvar_RegisterVariable(&joy_pitchsensitivity);
	Cvar_RegisterVariable(&joy_yawsensitivity);
	Cvar_RegisterVariable(&joy_wwhack1);
	Cvar_RegisterVariable(&joy_wwhack2);

	Cmd_AddCommand("force_centerview", Force_CenterView_f);
	Cmd_AddCommand("joyadvancedupdate", Joy_AdvancedUpdate_f);

	mpMouse->Startup();
	IN_StartupJoystick();
};

/*
===========
IN_Shutdown
===========
*/
void CInputWin::Shutdown()
{
	mpMouse->Deactivate();
	mpMouse->Show();
};

void CInputWin::HandleMessage(void *apData)
{
	MSG *pMsg = (MSG*)apData;
	
	switch(pMsg->uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		Key_Event(MapKey(lParam), true);
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		Key_Event(MapKey(lParam), false);
		break;
	
	// this is complicated because Win32 seems to pack multiple mouse events into
	// one update sometimes, so we always check all states and look for events
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
		temp = 0;

		if(wParam & MK_LBUTTON)
			temp |= 1;

		if(wParam & MK_RBUTTON)
			temp |= 2;

		if(wParam & MK_MBUTTON)
			temp |= 4;

		mpMouse->HandleEvent(temp);

		break;

	// JACK: This is the mouse wheel with the Intellimouse
	// Its delta is either positive or neg, and we generate the proper
	// Event.
	case WM_MOUSEWHEEL:
		if((short)HIWORD(wParam) > 0)
		{
			Key_Event(K_MWHEELUP, true);
			Key_Event(K_MWHEELUP, false);
		}
		else
		{
			Key_Event(K_MWHEELDOWN, true);
			Key_Event(K_MWHEELDOWN, false);
		};
		break;
	};
};