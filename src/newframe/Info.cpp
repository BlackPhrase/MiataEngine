#include "Info.hpp"

const char *CInfo::GetValueForKey(const char *s, const char *key)
{
	return Info_GetValueForKey(s, key);
};

void CInfo::RemoveKey(const char *s, const char *key)
{
	Info_RemoveKey(s, key);
};

void CInfo::RemovePrefixedKeys(const char *start, char prefix)
{
	Info_RemovePrefixedKeys(start, prefix);
};

void CInfo::SetValueForKey(const char *s, const char *key, const char *value, int maxsize)
{
	Info_SetValueForKey(s, key, value, maxsize);
};

void CInfo::SetValueForStarKey(const char *s, const char *key, const char *value, int maxsize)
{
	Info_SetValueForStarKey(s, key, value, maxsize);
};

void CInfo::Print(const char *s)
{
	Info_Print(s);
};