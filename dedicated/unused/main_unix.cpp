
void main(int argc, char *argv[])
{
	double time, oldtime, newtime;

	extern int net_socket;

	int j;

	j = COM_CheckParm("-mem");
	if(j)
		parms.memsize = (int)(Q_atof(com_argv[j + 1]) * 1024 * 1024);
	if((parms.membase = malloc(parms.memsize)) == NULL)
		Sys_Error("Can't allocate %ld\n", parms.memsize);

	parms.basedir = ".";

	/*
	if (Sys_FileTime ("id1/pak0.pak") != -1)
	else
		parms.basedir = "/raid/quake/v2";
*/

	while(1)
	{
		if(do_stdin)
			FD_SET(0, &fdset);
		FD_SET(net_socket, &fdset);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if(select(net_socket + 1, &fdset, NULL, NULL, &timeout) == -1)
			continue;
		stdin_ready = FD_ISSET(0, &fdset);

		SV_Frame(time);

		// extrasleep is just a way to generate a fucked up connection on purpose
		if(sys_extrasleep.value)
			usleep(sys_extrasleep.value);
	}
}

int main(int argc, char **argv)
{
	MSG msg;
	double time, oldtime;
	static char cwd[1024];

	parms.memsize = 16384 * 1024;
	parms.membase = malloc(parms.memsize);

	_getcwd(cwd, sizeof(cwd));
	if(cwd[Q_strlen(cwd) - 1] == '\\')
		cwd[Q_strlen(cwd) - 1] = 0;
	parms.basedir = cwd; //"f:/quake";
	                     //	parms.basedir = "f:\\quake";

	COM_InitArgv(argc, argv);

	// dedicated server ONLY!
	if(!COM_CheckParm("-dedicated"))
	{
		memcpy(newargv, argv, argc * 4);
		newargv[argc] = "-dedicated";
		argc++;
		argv = newargv;
		COM_InitArgv(argc, argv);
	}

	parms.argc = argc;
	parms.argv = argv;

	oldtime = Sys_FloatTime();

	/* main window message loop */
	while(1)
	{
		time = Sys_FloatTime();
		if(time - oldtime < sys_ticrate.value)
		{
			Sleep(1);
			continue;
		}

		Host_Frame(time - oldtime);
		oldtime = time;
	}

	/* return success of application */
	return TRUE;
}