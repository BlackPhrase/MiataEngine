/*
=============
main
=============
*/
void main(int argc, char *argv[])
{
	double			time, oldtime, newtime;
	quakeparms_t	parms;
	fd_set	fdset;
	extern	int		net_socket;
    struct timeval timeout;
	int j;

	memset (&parms, 0, sizeof(parms));

	COM_InitArgv (argc, argv);	
	parms.argc = com_argc;
	parms.argv = com_argv;

	parms.memsize = 16*1024*1024;

	j = COM_CheckParm("-mem");
	if (j)
		parms.memsize = (int) (Q_atof(com_argv[j+1]) * 1024 * 1024);
	if ((parms.membase = malloc (parms.memsize)) == NULL)
		Sys_Error("Can't allocate %ld\n", parms.memsize);

	parms.basedir = ".";

/*
	if (Sys_FileTime ("id1/pak0.pak") != -1)
	else
		parms.basedir = "/raid/quake/v2";
*/

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
		if (do_stdin)
			FD_SET(0, &fdset);
		FD_SET(net_socket, &fdset);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if (select (net_socket+1, &fdset, NULL, NULL, &timeout) == -1)
			continue;
		stdin_ready = FD_ISSET(0, &fdset);

	// find time passed since last cycle
		newtime = Sys_DoubleTime ();
		time = newtime - oldtime;
		oldtime = newtime;
		
		SV_Frame (time);		
		
	// extrasleep is just a way to generate a fucked up connection on purpose
		if (sys_extrasleep.value)
			usleep (sys_extrasleep.value);
	}	
}

