
/*
==============================================================================

 WINDOWS CRAP

==============================================================================
*/

/*
==================
WinMain
==================
*/
void SleepUntilInput(int time)
{
	MsgWaitForMultipleObjects(1, &tevent, FALSE, time, QS_ALLINPUT);
}

/*
==================
WinMain
==================
*/
HINSTANCE global_hInstance;
int global_nCmdShow;
char *argv[MAX_NUM_ARGVS];
static char *empty_string = "";
HWND hwnd_dialog;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	quakeparms_t parms;
	double time, oldtime, newtime;
	MEMORYSTATUS lpBuffer;
	static char cwd[1024];
	int t;
	RECT rect;

	/* previous instances do not exist in Win32 */
	if(hPrevInstance)
		return 0;

	global_hInstance = hInstance;
	global_nCmdShow = nCmdShow;

	lpBuffer.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&lpBuffer);

	if(!GetCurrentDirectory(sizeof(cwd), cwd))
		Sys_Error("Couldn't determine current directory");

	if(cwd[Q_strlen(cwd) - 1] == '/')
		cwd[Q_strlen(cwd) - 1] = 0;

	parms.basedir = cwd;
	parms.cachedir = NULL;

	parms.argc = 1;
	argv[0] = empty_string;

	while(*lpCmdLine && (parms.argc < MAX_NUM_ARGVS))
	{
		while(*lpCmdLine && ((*lpCmdLine <= 32) || (*lpCmdLine > 126)))
			lpCmdLine++;

		if(*lpCmdLine)
		{
			argv[parms.argc] = lpCmdLine;
			parms.argc++;

			while(*lpCmdLine && ((*lpCmdLine > 32) && (*lpCmdLine <= 126)))
				lpCmdLine++;

			if(*lpCmdLine)
			{
				*lpCmdLine = 0;
				lpCmdLine++;
			}
		}
	}

	parms.argv = argv;

	COM_InitArgv(parms.argc, parms.argv);

	parms.argc = com_argc;
	parms.argv = com_argv;

	isDedicated = (COM_CheckParm("-dedicated") != 0);

	if(!isDedicated)
	{
		hwnd_dialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, NULL);

		if(hwnd_dialog)
		{
			if(GetWindowRect(hwnd_dialog, &rect))
			{
				if(rect.left > (rect.top * 2))
				{
					SetWindowPos(hwnd_dialog, 0,
					             (rect.left / 2) - ((rect.right - rect.left) / 2),
					             rect.top, 0, 0,
					             SWP_NOZORDER | SWP_NOSIZE);
				}
			}

			ShowWindow(hwnd_dialog, SW_SHOWDEFAULT);
			UpdateWindow(hwnd_dialog);
			SetForegroundWindow(hwnd_dialog);
		}
	}

	// take the greater of all the available memory or half the total memory,
	// but at least 8 Mb and no more than 16 Mb, unless they explicitly
	// request otherwise
	parms.memsize = lpBuffer.dwAvailPhys;

	if(parms.memsize < MINIMUM_WIN_MEMORY)
		parms.memsize = MINIMUM_WIN_MEMORY;

	if(parms.memsize < (lpBuffer.dwTotalPhys >> 1))
		parms.memsize = lpBuffer.dwTotalPhys >> 1;

	if(parms.memsize > MAXIMUM_WIN_MEMORY)
		parms.memsize = MAXIMUM_WIN_MEMORY;

	if(COM_CheckParm("-heapsize"))
	{
		t = COM_CheckParm("-heapsize") + 1;

		if(t < com_argc)
			parms.memsize = Q_atoi(com_argv[t]) * 1024;
	}

	parms.membase = malloc(parms.memsize);

	if(!parms.membase)
		Sys_Error("Not enough memory free; check disk space\n");

	Sys_PageIn(parms.membase, parms.memsize);

	tevent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if(!tevent)
		Sys_Error("Couldn't create event");

	if(isDedicated)
	{
		if(!AllocConsole())
		{
			Sys_Error("Couldn't create dedicated server console");
		}

		hinput = GetStdHandle(STD_INPUT_HANDLE);
		houtput = GetStdHandle(STD_OUTPUT_HANDLE);

		// give QHOST a chance to hook into the console
		if((t = COM_CheckParm("-HFILE")) > 0)
		{
			if(t < com_argc)
				hFile = (HANDLE)Q_atoi(com_argv[t + 1]);
		}

		if((t = COM_CheckParm("-HPARENT")) > 0)
		{
			if(t < com_argc)
				heventParent = (HANDLE)Q_atoi(com_argv[t + 1]);
		}

		if((t = COM_CheckParm("-HCHILD")) > 0)
		{
			if(t < com_argc)
				heventChild = (HANDLE)Q_atoi(com_argv[t + 1]);
		}

		InitConProc(hFile, heventParent, heventChild);
	}

	Sys_Init();

	// because sound is off until we become active
	S_BlockSound();

	Sys_Printf("Host_Init\n");
	Host_Init(&parms);

	oldtime = Sys_FloatTime();

	/* main window message loop */
	while(1)
	{
		if(isDedicated)
		{
			newtime = Sys_FloatTime();
			time = newtime - oldtime;

			while(time < sys_ticrate.value)
			{
				Sys_Sleep();
				newtime = Sys_FloatTime();
				time = newtime - oldtime;
			}
		}
		else
		{
			// yield the CPU for a little while when paused, minimized, or not the focus
			if((cl.paused && (!ActiveApp && !DDActive)) || Minimized || block_drawing)
			{
				SleepUntilInput(PAUSE_SLEEP);
				scr_skipupdate = 1; // no point in bothering to draw
			}
			else if(!ActiveApp && !DDActive)
			{
				SleepUntilInput(NOT_FOCUS_SLEEP);
			}

			newtime = Sys_FloatTime();
			time = newtime - oldtime;
		}

		Host_Frame(time);
		oldtime = newtime;
	}

	/* return success of application */
	return TRUE;
}
