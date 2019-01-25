#pragma once

class CHunk
{
public:
	void *Alloc(int size); // returns 0 filled memory
	void *AllocName(int size, const char *name);

	void *HighAllocName(int size, const char *name);

	void *TempAlloc(int size);

	void FreeToHighMark(int mark);
	void FreeToLowMark(int mark);

	void Check();

	int	GetHighMark();
	int	GetLowMark();
};