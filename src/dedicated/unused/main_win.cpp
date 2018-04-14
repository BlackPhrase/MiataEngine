/*
==================
main
==================
*/
char *newargv[256];

int main(int argc, char **argv)
{
	double newtime, time, oldtime;
	static char cwd[1024];
	struct timeval timeout;
	fd_set fdset;
	int t;

	COM_InitArgv(argc, argv);

	parms.memsize = 16 * 1024 * 1024;

	if((t = COM_CheckParm("-heapsize")) != 0 &&
	   t + 1 < com_argc)
		parms.memsize = Q_atoi(com_argv[t + 1]) * 1024;

	if((t = COM_CheckParm("-mem")) != 0 &&
	   t + 1 < com_argc)
		parms.memsize = Q_atoi(com_argv[t + 1]) * 1024 * 1024;

	parms.membase = malloc(parms.memsize);

	if(!parms.membase)
		Sys_Error("Insufficient memory.\n");

	parms.basedir = ".";
	parms.cachedir = NULL;

	

	
	oldtime = Sys_DoubleTime() - 0.1;
	while(1)
	{
		// select on the net socket and stdin
		// the only reason we have a timeout at all is so that if the last
		// connected client times out, the message would not otherwise
		// be printed until the next event.
		FD_ZERO(&fdset);
		FD_SET(net_socket, &fdset);

		timeout.tv_sec = 0;
		timeout.tv_usec = 100;

		if(select(net_socket + 1, &fdset, NULL, NULL, &timeout) == -1)
			continue;

		// find time passed since last cycle
		newtime = Sys_DoubleTime();
		time = newtime - oldtime;
		oldtime = newtime;

		Host_Frame(time);
	};

	return true;
};