

#define		CON_TEXTSIZE	16384
typedef struct
{
	char	text[CON_TEXTSIZE];
	int		current;		// line where next message will be printed
	int		x;				// offset in current line for next print
	int		display;		// bottom of console displays this line
} console_t;

extern	console_t	con_main;
extern	console_t	con_chat;
extern	console_t	*con;			// point to either con_main or con_chat

extern	int			con_ormask;

extern int con_totallines;
extern qboolean con_initialized;
extern byte *con_chars;
extern	int	con_notifylines;		// scan lines to clear for notify lines

void Con_DrawConsole (int lines);
