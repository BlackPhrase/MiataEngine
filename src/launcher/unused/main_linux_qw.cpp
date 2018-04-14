int main(int c, char **v)
{
	double time, oldtime, newtime;
	quakeparms_t parms;
	int j;

	//	static char cwd[1024];

	//	signal(SIGFPE, floating_point_exception_handler);
	signal(SIGFPE, SIG_IGN);

	memset(&parms, 0, sizeof(parms));

	COM_InitArgv(c, v);
	parms.argc = com_argc;
	parms.argv = com_argv;

	parms.memsize = 16 * 1024 * 1024;

	j = COM_CheckParm("-mem");
	if(j)
		parms.memsize = (int)(Q_atof(com_argv[j + 1]) * 1024 * 1024);
	parms.membase = malloc(parms.memsize);

	parms.basedir = basedir;
	// caching is disabled by default, use -cachedir to enable
	//	parms.cachedir = cachedir;

	noconinput = COM_CheckParm("-noconinput");
	if(!noconinput)
		fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) | FNDELAY);

	if(COM_CheckParm("-nostdout"))
		nostdout = 1;

	Sys_Init();

	Host_Init(&parms);

	oldtime = Sys_DoubleTime();
	while(1)
	{
		// find time spent rendering last frame
		newtime = Sys_DoubleTime();
		time = newtime - oldtime;

		Host_Frame(time);
		oldtime = newtime;
	}
}