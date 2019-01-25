#pragma once

class CCache
{
public:
	/// Returns nullptr if all purgable data was tossed and there still
	/// wasn't enough room
	void *Alloc(cache_user_t *c, int size, const char *name);

	void Free(cache_user_t *c);

	void *Check(cache_user_t *c);
	/// returns the cached data, and moves to the head of the LRU list
	/// if present, otherwise returns nullptr

	void Report();
	void Flush();
};