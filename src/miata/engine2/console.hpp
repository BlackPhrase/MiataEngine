

//
// console
//

extern int con_totallines;

//extern int con_backscroll;
//extern bool con_forcedup; // because no entities to refresh

extern byte *con_chars;
extern int con_notifylines; // scan lines to clear for notify lines

void Con_DrawCharacter(int cx, int line, int num);

void Con_CheckResize();
void Con_DrawConsole(int lines, bool drawinput);

void Con_SafePrintf(const char *fmt, ...);

void Con_Clear_f();
void Con_ToggleConsole_f();

void Con_DrawNotify();
void Con_ClearNotify();

void Con_NotifyBox(const char *text); // during startup for sound / cd warnings