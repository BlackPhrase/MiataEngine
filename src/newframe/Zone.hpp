#pragma once

class CZone
{
public:
	void *Malloc(int size); // returns 0 filled memory
	void *TagMalloc(int size, int tag);

	void Free(void *ptr);

	void CheckHeap();
	void DumpHeap();
	
	int FreeMemory();
};