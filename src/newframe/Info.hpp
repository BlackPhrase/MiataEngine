#pragma once

class CInfo
{
public:
	const char *GetValueForKey(const char *s, const char *key);
	
	void RemoveKey(const char *s, const char *key);
	void RemovePrefixedKeys(const char *start, char prefix);
	
	void SetValueForKey(const char *s, const char *key, const char *value, int maxsize);
	void SetValueForStarKey(const char *s, const char *key, const char *value, int maxsize);
	
	void Print(const char *s);
};