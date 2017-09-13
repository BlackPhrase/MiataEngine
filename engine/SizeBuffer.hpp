#pragma once

#include "commontypes.hpp"

class CSizeBuffer final
{
public:
	CSizeBuffer(const char *name);
	CSizeBuffer();
	~CSizeBuffer();
	
	void Clear();
	
	void *GetSpace(int length);
	
	void Write(const void *data, int length);
	void Print(const char *data); // strcats onto the sizebuf
//private:
	const char *name{""};
	
	byte *data{nullptr};
	
	int maxsize{0};
	int cursize{0};
	
	bool allowoverflow{false}; ///< if false, do a Sys_Error
	bool overflowed{false}; ///< set to true if the buffer size failed
};