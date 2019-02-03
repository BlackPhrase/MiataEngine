#include "Wad.hpp"

void CWAD::LoadWadFile(const char *filename)
{
	WAD_LoadWadFile(filename);
};

void CWAD::CleanupName(const char *in, char *out)
{
	WAD_CleanupName(in, out);
};

lumpinfo_t CWAD::GetLumpInfo(const char *name)
{
	return WAD_GetLumpInfo(name);
};

void *CWAD::GetLumpName(const char *name)
{
	return WAD_GetLumpName(name);
};

void *CWAD::GetLumpNum(int num)
{
	return WAD_GetLumpNum(num);
};