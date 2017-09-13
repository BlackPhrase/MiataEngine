#include "quakedef.hpp"
#include "SizeBuffer.hpp"

CSizeBuffer::CSizeBuffer(const char *name)
{
	this->name = name;
};

CSizeBuffer::CSizeBuffer() = default;
CSizeBuffer::~CSizeBuffer() = default;

void CSizeBuffer::Clear()
{
	this->cursize = 0;
};

void *CSizeBuffer::GetSpace(int length)
{
	if(this->cursize + length > this->maxsize)
	{
		//if(!this->allowoverflow)
			//Sys_Error("SZ_GetSpace: overflow without allowoverflow set");

		//if(length > this->maxsize)
			//Sys_Error("SZ_GetSpace: %i is > full buffer size", length);

		this->overflowed = true;
		//Con_Printf("SZ_GetSpace: overflow");
		Clear();
	};

	void *data = this->data + this->cursize;
	this->cursize += length;

	return data;
};

void CSizeBuffer::Write(const void *data, int length)
{
	Q_memcpy(GetSpace(length), data, length);
};

void CSizeBuffer::Print(const char *data)
{
	auto len{Q_strlen(data) + 1};

	// byte * cast to keep VC++ happy
	if(this->data[this->cursize - 1])
		Q_memcpy((byte *)GetSpace(len), data, len); // no trailing 0
	else
		Q_memcpy((byte *)GetSpace(len - 1) - 1, data, len); // write over trailing 0
};