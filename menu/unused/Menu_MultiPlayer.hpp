#pragma once

#include "IMenuState.hpp"

//=============================================================================
/* MULTIPLAYER MENU */

int m_multiplayer_cursor;
#define MULTIPLAYER_ITEMS 3

void M_Menu_MultiPlayer_f()
{
	key_dest = key_menu;
	m_state = m_multiplayer;
	m_entersound = true;
}

class CMenu_MultiPlayer final : public IMenuState
{
public:
	CMenu_MultiPlayer() = default;
	~CMenu_MultiPlayer() = default;
	
	void Draw() override
	{
		int f;
		qpic_t *p;

		M_DrawTransPic(16, 4, Draw_CachePic("gfx/qplaque.lmp"));
		p = Draw_CachePic("gfx/p_multi.lmp");
		M_DrawPic((320 - p->width) / 2, 4, p);
		M_DrawTransPic(72, 32, Draw_CachePic("gfx/mp_menu.lmp"));

		f = (int)(host_time * 10) % 6;

		M_DrawTransPic(54, 32 + m_multiplayer_cursor * 20, Draw_CachePic(va("gfx/menudot%i.lmp", f + 1)));

		if(serialAvailable || ipxAvailable || tcpipAvailable)
			return;
		M_PrintWhite((320 / 2) - ((27 * 8) / 2), 148, "No Communications Available");
	};
	
	void HandleKey(int key) override
	{
		switch(key)
		{
		case K_ESCAPE:
			M_Menu_Main_f();
			break;

		case K_DOWNARROW:
			S_LocalSound("misc/menu1.wav");
			if(++m_multiplayer_cursor >= MULTIPLAYER_ITEMS)
				m_multiplayer_cursor = 0;
			break;

		case K_UPARROW:
			S_LocalSound("misc/menu1.wav");
			if(--m_multiplayer_cursor < 0)
				m_multiplayer_cursor = MULTIPLAYER_ITEMS - 1;
			break;

		case K_ENTER:
			m_entersound = true;
			switch(m_multiplayer_cursor)
			{
			case 0:
				if(serialAvailable || ipxAvailable || tcpipAvailable)
					M_Menu_Net_f();
				break;

			case 1:
				if(serialAvailable || ipxAvailable || tcpipAvailable)
					M_Menu_Net_f();
				break;

			case 2:
				M_Menu_Setup_f();
				break;
			};
		};
	};
};