/*
Copyright (C) 1996-1997 Id Software, Inc.
Copyright (C) 2017-2018 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.hpp"
#include "SizeBuffer.hpp"

CSizeBuffer::CSizeBuffer(const char *name, int length)
{
	Init(name, length);
};

CSizeBuffer::CSizeBuffer() = default;

CSizeBuffer::~CSizeBuffer()
{
	if(this->data)
		delete [] data;
};

void CSizeBuffer::Init(const char *name, int startsize)
{
	this->name = name;
	this->data = new byte[startsize];
	this->maxsize = startsize; // length
	
	Clear();
};

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