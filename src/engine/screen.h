/*
Copyright (C) 1996-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// screen.h

//extern	vrect_t		scr_vrect;		// position of render window // TODO: q2

//extern	char		crosshair_pic[MAX_QPATH]; // TODO: q2
//extern	int			crosshair_width, crosshair_height; // TODO: q2

void SCR_Init (void);

void SCR_UpdateScreen (void);

void SCR_SizeUp (void);
void SCR_SizeDown (void);

//void SCR_BringDownConsole (void); // TODO: non-q2

void SCR_CenterPrint (const char *str);

void SCR_BeginLoadingPlaque (void);
void SCR_EndLoadingPlaque (void);

//int SCR_ModalMessage (const char *text); // TODO: non-q2

extern	float		scr_con_current;
extern	float		scr_conlines;		// lines of console to display

//extern	int			scr_fullupdate;	// set to 0 to force full redraw // TODO: non-q2
extern	int			sb_lines;

//extern	int			clearnotify;	// set to 0 whenever notify text is drawn // TODO: non-q2
//extern	bool	scr_disabled_for_loading; // TODO: non-q2
//extern	bool	scr_skipupdate; // TODO: non-q2

extern cvar_t scr_viewsize;
//extern cvar_t *crosshair; // TODO: q2

// only the refresh window will be updated unless these variables are flagged 
//extern	int			scr_copytop; // TODO: non-q2
//extern	int			scr_copyeverything; // TODO: non-q2

//extern bool		block_drawing; // TODO: non-q2

//void SCR_UpdateWholeScreen (void); // TODO: non-q2

//void	SCR_DebugGraph (float value, int color); // TODO: q2

//void	SCR_TouchPics (void); // TODO: q2

//void	SCR_RunConsole (void); // TODO: q2

//void SCR_AddDirtyPoint (int x, int y); // TODO: q2
//void SCR_DirtyScreen (void); // TODO: q2

//
// scr_cin.c
//
//void SCR_PlayCinematic (char *name); // TODO: q2
//qboolean SCR_DrawCinematic (void); // TODO: q2
//void SCR_RunCinematic (void); // TODO: q2
//void SCR_StopCinematic (void); // TODO: q2
//void SCR_FinishCinematic (void); // TODO: q2