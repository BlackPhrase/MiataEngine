/*
==================
main

==================
*/
char	*newargv[256];

int main (int argc, char **argv)
{
	quakeparms_t	parms;
	double			newtime, time, oldtime;
	static	char	cwd[1024];
	struct timeval	timeout;
	fd_set			fdset;
	int				t;

	COM_InitArgv (argc, argv);
	
	parms.argc = com_argc;
	parms.argv = com_argv;

	parms.memsize = 16*1024*1024;

	if ((t = COM_CheckParm ("-heapsize")) != 0 &&
		t + 1 < com_argc)
		parms.memsize = Q_atoi (com_argv[t + 1]) * 1024;

	if ((t = COM_CheckParm ("-mem")) != 0 &&
		t + 1 < com_argc)
		parms.memsize = Q_atoi (com_argv[t + 1]) * 1024 * 1024;

	parms.membase = malloc (parms.memsize);

	if (!parms.membase)
		Sys_Error("Insufficient memory.\n");

	parms.basedir = ".";
	parms.cachedir = NULL;

	SV_Init (&parms);

// run one frame immediately for first heartbeat
	SV_Frame (0.1);		

//
// main loop
//
	oldtime = Sys_DoubleTime () - 0.1;
	while (1)
	{
	// select on the net socket and stdin
	// the only reason we have a timeout at all is so that if the last
	// connected client times out, the message would not otherwise
	// be printed until the next event.
		FD_ZERO(&fdset);
		FD_SET(net_socket, &fdset);
		timeout.tv_sec = 0;
		timeout.tv_usec = 100;
		if (select (net_socket+1, &fdset, NULL, NULL, &timeout) == -1)
			continue;

	// find time passed since last cycle
		newtime = Sys_DoubleTime ();
		time = newtime - oldtime;
		oldtime = newtime;
		
		SV_Frame (time);				
	}	

	return true;
}

/*
==================
main

==================
*/
char	*newargv[256];

int main (int argc, char **argv)
{
    MSG        msg;
	quakeparms_t	parms;
	double			time, oldtime;
	static	char	cwd[1024];

	memset (&parms, 0, sizeof(parms));

	parms.memsize = 16384*1024;
	parms.membase = malloc (parms.memsize);

	_getcwd (cwd, sizeof(cwd));
	if (cwd[Q_strlen(cwd)-1] == '\\')
		cwd[Q_strlen(cwd)-1] = 0;
	parms.basedir = cwd; //"f:/quake";
//	parms.basedir = "f:\\quake";

	COM_InitArgv (argc, argv);

	// dedicated server ONLY!
	if (!COM_CheckParm ("-dedicated"))
	{
		memcpy (newargv, argv, argc*4);
		newargv[argc] = "-dedicated";
		argc++;
		argv = newargv;
		COM_InitArgv (argc, argv);
	}

	parms.argc = argc;
	parms.argv = argv;

	printf ("Host_Init\n");
	Host_Init (&parms);

	oldtime = Sys_FloatTime ();

    /* main window message loop */
	while (1)
	{
		time = Sys_FloatTime();
		if (time - oldtime < sys_ticrate.value )
		{
			Sleep(1);
			continue;
		}

		Host_Frame ( time - oldtime );
		oldtime = time;
	}

    /* return success of application */
    return TRUE;
}