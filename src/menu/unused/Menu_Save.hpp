#pragma once

#include "IMenuState.hpp"

//=============================================================================
/* LOAD/SAVE MENU */

#define MAX_SAVEGAMES 12
char m_filenames[MAX_SAVEGAMES][SAVEGAME_COMMENT_LENGTH + 1];
int loadable[MAX_SAVEGAMES];

void M_ScanSaves()
{
	int i, j;
	char name[MAX_OSPATH];
	FILE *f;
	int version;

	for(i = 0; i < MAX_SAVEGAMES; i++)
	{
		strcpy(m_filenames[i], "--- UNUSED SLOT ---");
		loadable[i] = false;
		sprintf(name, "%s/s%i.sav", com_gamedir, i);
		f = fopen(name, "r");
		if(!f)
			continue;
		fscanf(f, "%i\n", &version);
		fscanf(f, "%79s\n", name);
		strncpy(m_filenames[i], name, sizeof(m_filenames[i]) - 1);

		// change _ back to space
		for(j = 0; j < SAVEGAME_COMMENT_LENGTH; j++)
			if(m_filenames[i][j] == '_')
				m_filenames[i][j] = ' ';
		loadable[i] = true;
		fclose(f);
	}
}

void M_Menu_Save_f()
{
	if(!sv.active)
		return;
	if(cl.intermission)
		return;
	if(svs.maxclients != 1)
		return;
	m_entersound = true;
	m_state = m_save;
	key_dest = key_menu;
	M_ScanSaves();
}

class CMenu_Save final : public IMenuState
{
public:
	CMenu_Save() = default;
	~CMenu_Save() = default;
	
	void Draw() override
	{
		int i;
		qpic_t *p;

		p = Draw_CachePic("gfx/p_save.lmp");
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
			m_state = m_none;
			key_dest = key_game;
			Cbuf_AddText(va("save s%i\n", load_cursor));
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