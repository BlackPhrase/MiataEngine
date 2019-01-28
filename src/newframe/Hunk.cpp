#include "Hunk.hpp"

void *CHunk::Alloc(int size)
{
};

void *CHunk::AllocName(int size, const char *name)
{
};

void *CHunk::HighAllocName(int size, const char *name)
{
};

void *CHunk::TempAlloc(int size)
{
};

void CHunk::FreeToHighMark(int mark)
{
};

void CHunk::FreeToLowMark(int mark)
{
};

void CHunk::Check()
{
};

int	CHunk::GetHighMark()
{
};

int	CHunk::GetLowMark()
{
};