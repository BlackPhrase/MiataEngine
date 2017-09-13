#pragma once

struct IFileSystem
{
	///
	virtual void Init() = 0;
	
	///
	virtual void Shutdown() = 0;
	
	///
	virtual bool CreatePath(const char *path) = 0;
	
	/// a null buffer will just return the file length without loading
	/// a -1 length is not present
	virtual int LoadFile(const char *path, void **buffer) = 0;
	
	///
	virtual void FreeFile(void *buffer) = 0;
};