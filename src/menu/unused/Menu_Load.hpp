#pragma once

#include "IMenuState.hpp"

//=============================================================================
/* LOAD/SAVE MENU */

int load_cursor; // 0 < load_cursor < MAX_SAVEGAMES

void M_Menu_Load_f()
{
	m_entersound = true;
	m_state = m_load;
	key_dest = key_menu;
	M_ScanSaves();
}

class CMenu_MultiPlayer final : public IMenuState
{
public:
	CMenu_MultiPlayer() = default;
	~CMenu_MultiPlayer() = default;
	
	void Draw() override
	{
		int i;
		qpic_t *p;

		p = Draw_CachePic("gfx/p_load.lmp");
		M_DrawPic((320 - p->width) / 2, 4, p);

		for(i = 0; i < MAX_SAVEGAMES; i++)
			M_Print(16, 32 + 8 * i, m_filenames[i]);

		// line cursor
		M_DrawCharacter(8, 32 + load_cursor * 8, 12 + ((int)(realtime * 4) & 1));
	};
	
	void HandleKey(int key) override
	{
		switch(k)
		{
		case K_ESCAPE:
			M_Menu_SinglePlayer_f();
			break;

		case K_ENTER:
			S_LocalSound("misc/menu2.wav");
			if(!loadable[load_cursor])
				return;
			m_state = m_none;
			key_dest = key_game;

			// Host_Loadgame_f can't bring up the loading plaque because too much
			// stack space has been used, so do it now
			SCR_BeginLoadingPlaque();

			// issue the load command
			Cbuf_AddText(va("load s%i\n", load_cursor));
			return;

		case K_UPARROW:
		case K_LEFTARROW:
			S_LocalSound("misc/menu1.wav");
			load_cursor--;
			if(load_cursor < 0)
				load_cursor = MAX_SAVEGAMES - 1;
			break;

		case K_DOWNARROW:
		case K_RIGHTARROW:
			S_LocalSound("misc/menu1.wav");
			load_cursor++;
			if(load_cursor >= MAX_SAVEGAMES)
				load_cursor = 0;
			break;
		};
	};
};