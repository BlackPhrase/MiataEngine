#pragma once

class CSizeBuffer
{
public:
	void Alloc(sizebuf_t *buf, int startsize);
	void Free(sizebuf_t *buf);
	
	void Clear(sizebuf_t *buf);
	
	void Write(sizebuf_t *buf, const void *data, int length);
	void Print(sizebuf_t *buf, char *data); // strcats onto the sizebuf
	
	void *GetSpace(sizebuf_t *buf, int length);
};