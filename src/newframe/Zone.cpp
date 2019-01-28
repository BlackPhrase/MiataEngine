#include "Zone.hpp"

void *CZone::Malloc(int size)
{
};

void *CZone::TagMalloc(int size, int tag)
{
};

void CZone::Free(void *ptr)
{
};

void CZone::CheckHeap()
{
};

void CZone::DumpHeap()
{
};

int CZone::FreeMemory()
{
};