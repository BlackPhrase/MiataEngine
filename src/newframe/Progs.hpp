#pragma once

class CProgs
{
public:
	void Init();

	void LoadProgs();

	void ExecuteProgram(func_t fnum);

	void RunError(const char *error, ...);

	int SetString(const char *s);
	char *GetString(int num);
	
	void PR_Profile_f();
};