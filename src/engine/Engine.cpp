/// @file

#include "quakedef.h"
#include "Engine.hpp"

#ifdef _WIN32
HINSTANCE global_hInstance{};
#endif

const char *argv[MAX_NUM_ARGVS]{};
static const char *empty_string{""};

#ifdef _WIN32
void SleepUntilInput (int time)
{
	MsgWaitForMultipleObjects(1, &tevent, FALSE, time, QS_ALLINPUT);
};
#endif

CEngine::CEngine() = default;
CEngine::~CEngine() = default;

bool CEngine::Init(const InitProps &apInitProps)
{
#ifdef _WIN32
	MSG				msg;
	MEMORYSTATUS	lpBuffer;
#endif

	quakeparms_t	parms{};
	static	char	cwd[1024]{};
	int				t{};

#ifdef _WIN32
	global_hInstance = apInitProps.PlatformSpecific.hInstance;
	
	lpBuffer.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus (&lpBuffer);

	if (!GetCurrentDirectory (sizeof(cwd), cwd))
		Sys_Error ("Couldn't determine current directory");
#endif

	if (cwd[Q_strlen(cwd)-1] == '/')
		cwd[Q_strlen(cwd)-1] = 0;

	parms.basedir = cwd;
	parms.cachedir = nullptr;

	parms.argc = 1;
	argv[0] = empty_string;

	while (*lpCmdLine && (parms.argc < MAX_NUM_ARGVS))
	{
		while (*lpCmdLine && ((*lpCmdLine <= 32) || (*lpCmdLine > 126)))
			lpCmdLine++;

		if (*lpCmdLine)
		{
			argv[parms.argc] = lpCmdLine;
			parms.argc++;

			while (*lpCmdLine && ((*lpCmdLine > 32) && (*lpCmdLine <= 126)))
				lpCmdLine++;

			if (*lpCmdLine)
			{
				*lpCmdLine = 0;
				lpCmdLine++;
			};
		};
	};

	parms.argv = argv;

	COM_InitArgv (parms.argc, parms.argv);

	parms.argc = com_argc;
	parms.argv = com_argv;

	isDedicated = (COM_CheckParm ("-dedicated") != 0);

// take the greater of all the available memory or half the total memory,
// but at least 8 Mb and no more than 16 Mb, unless they explicitly
// request otherwise
#ifdef _WIN32
	parms.memsize = lpBuffer.dwAvailPhys;
#endif

	if (parms.memsize < MINIMUM_WIN_MEMORY)
		parms.memsize = MINIMUM_WIN_MEMORY;

#ifdef _WIN32
	if (parms.memsize < (lpBuffer.dwTotalPhys >> 1))
		parms.memsize = lpBuffer.dwTotalPhys >> 1;
#endif

	if (parms.memsize > MAXIMUM_WIN_MEMORY)
		parms.memsize = MAXIMUM_WIN_MEMORY;

	if (COM_CheckParm ("-heapsize"))
	{
		t = COM_CheckParm("-heapsize") + 1;

		if (t < com_argc)
			parms.memsize = Q_atoi (com_argv[t]) * 1024;
	};

	parms.membase = malloc (parms.memsize);

	if (!parms.membase)
		Sys_Error ("Not enough memory free; check disk space\n");

	Sys_PageIn (parms.membase, parms.memsize);

#ifdef _WIN32
	tevent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	if (!tevent)
		Sys_Error ("Couldn't create event");
#endif

	if (isDedicated)
	{
		if (!AllocConsole ())
			Sys_Error ("Couldn't create dedicated server console");

		hinput = GetStdHandle (STD_INPUT_HANDLE);
		houtput = GetStdHandle (STD_OUTPUT_HANDLE);

		// give QHOST a chance to hook into the console
		if ((t = COM_CheckParm ("-HFILE")) > 0)
			if (t < com_argc)
				hFile = (HANDLE)Q_atoi (com_argv[t+1]);
			
		if ((t = COM_CheckParm ("-HPARENT")) > 0)
			if (t < com_argc)
				heventParent = (HANDLE)Q_atoi (com_argv[t+1]);
			
		if ((t = COM_CheckParm ("-HCHILD")) > 0)
			if (t < com_argc)
				heventChild = (HANDLE)Q_atoi (com_argv[t+1]);

		InitConProc (hFile, heventParent, heventChild);
	};

	Sys_Init ();

	// because sound is off until we become active
	S_BlockSound ();

	Sys_Printf ("Host_Init\n");
	Host_Init (&parms);

	oldtime = Sys_FloatTime ();
};

void CEngine::Frame()
{
	if (isDedicated)
	{
		newtime = Sys_FloatTime ();
		time = newtime - oldtime;

		while (time < sys_ticrate.value )
		{
			Sys_Sleep();
			newtime = Sys_FloatTime ();
			time = newtime - oldtime;
		};
	}
	else
	{
#ifdef _WIN32
		// yield the CPU for a little while when paused, minimized, or not the focus
		if ((cl.paused && (!ActiveApp && !DDActive)) || Minimized || block_drawing)
		{
			SleepUntilInput (PAUSE_SLEEP);
			scr_skipupdate = 1;		// no point in bothering to draw
		}
		else if (!ActiveApp && !DDActive)
			SleepUntilInput (NOT_FOCUS_SLEEP);
#endif

		newtime = Sys_FloatTime ();
		time = newtime - oldtime;
	};

	Host_Frame (time);
	oldtime = newtime;
};