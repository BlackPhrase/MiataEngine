#pragma once

#include "IFileSystem.hpp"

//
// file IO
//

// returns the file size
// return -1 if file is not present
// the file should be in BINARY mode for stupid OSes that care
int Sys_FileOpenRead(char *path, int *hndl);

int Sys_FileOpenWrite(char *path);
void Sys_FileClose(int handle);
void Sys_FileSeek(int handle, int position);
int Sys_FileRead(int handle, void *dest, int count);
int Sys_FileWrite(int handle, void *data, int count);
int Sys_FileTime(char *path);

void Sys_mkdir(char *path);

class CFileSystem final : public IFileSystem
{
public:
	CFileSystem();
	~CFileSystem();
};