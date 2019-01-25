#pragma once

class CWAD
{
public:
	void LoadWadFile(const char *filename);
	
	void CleanupName(const char *in, char *out);
	
	lumpinfo_t GetLumpInfo(const char *name);
	void *GetLumpName(const char *name);
	void *GetLumpNum(int num);
};