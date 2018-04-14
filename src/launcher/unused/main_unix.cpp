int main(int c, char **v)
{
	double time, oldtime, newtime;
	quakeparms_t parms;
	extern int vcrFile;
	extern int recording;
	int j;

	//	static char cwd[1024];

	//	signal(SIGFPE, floating_point_exception_handler);
	signal(SIGFPE, SIG_IGN);

	memset(&parms, 0, sizeof(parms));

	COM_InitArgv(c, v);
	parms.argc = com_argc;
	parms.argv = com_argv;

#ifdef GLQUAKE
	parms.memsize = 16 * 1024 * 1024;
#else
	parms.memsize = 8 * 1024 * 1024;
#endif

	j = COM_CheckParm("-mem");
	if(j)
		parms.memsize = (int)(Q_atof(com_argv[j + 1]) * 1024 * 1024);
	parms.membase = malloc(parms.memsize);

	parms.basedir = basedir;
	// caching is disabled by default, use -cachedir to enable
	//	parms.cachedir = cachedir;

	fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) | FNDELAY);

	Host_Init(&parms);

	Sys_Init();

	if(COM_CheckParm("-nostdout"))
		nostdout = 1;
	else
	{
		fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) | FNDELAY);
		printf("Linux Quake -- Version %0.3f\n", LINUX_VERSION);
	}

	oldtime = Sys_FloatTime() - 0.1;
	while(1)
	{
		// find time spent rendering last frame
		newtime = Sys_FloatTime();
		time = newtime - oldtime;

		if(cls.state == ca_dedicated)
		{ // play vcrfiles at max speed
			if(time < sys_ticrate.value && (vcrFile == -1 || recording))
			{
				usleep(1);
				continue; // not time to run a server only tic yet
			}
			time = sys_ticrate.value;
		}

		if(time > sys_ticrate.value * 2)
			oldtime = newtime;
		else
			oldtime += time;

		Host_Frame(time);

		// graphic debugging aids
		if(sys_linerefresh.value)
			Sys_LineRefresh();
	}
}