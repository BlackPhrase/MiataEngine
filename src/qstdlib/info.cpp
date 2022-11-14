/*
This file is part of Miata Engine
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2022 BlackPhrase

Miata Engine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Miata Engine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Miata Engine. If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#include <qstdlib/info.h>

/*
=====================================================================

  INFO STRINGS

=====================================================================
*/

/*
===============
Info_ValueForKey

Searches the string for the given
key and returns the associated value, or an empty string.
===============
*/
const char *Info_ValueForKey(const char *s, const char *key)
{
	char pkey[512];
	static char value[2][512];	// use two buffers so compares
								// work without stomping on each other
	static int	valueindex;
	char *o;
	
	valueindex ^= 1;
	if(*s == '\\')
		s++;
	while (1)
	{
		o = pkey;
		while(*s != '\\')
		{
			if(!*s)
				return "";
			*o++ = *s++;
		};
		*o = 0;
		s++;

		o = value[valueindex];

		while(*s != '\\' && *s)
		{
			if(!*s)
				return "";
			*o++ = *s++;
		};
		*o = 0;

		if(!strcmp(key, pkey))
			return value[valueindex];

		if(!*s)
			return "";
		s++;
	};
};

void Info_RemoveKey(char *s, const char *key)
{
	char *start;
	char pkey[512];
	char value[512];
	char *o;

	if(strstr(key, "\\"))
	{
//		Com_Printf ("Can't use a key with a \\\n");
		return;
	};

	while(1)
	{
		start = s;
		if(*s == '\\')
			s++;
		o = pkey;
		while(*s != '\\')
		{
			if(!*s)
				return;
			*o++ = *s++;
		};
		*o = 0;
		s++;

		o = value;
		while(*s != '\\' && *s)
		{
			if(!*s)
				return;
			*o++ = *s++;
		};
		*o = 0;

		if(!strcmp(key, pkey))
		{
			strcpy(start, s); // remove this part
			return;
		};

		if(!*s)
			return;
	};
};

/*
==================
Info_Validate

Some characters are illegal in info strings because they
can mess up the server's parsing
==================
*/
qboolean Info_Validate(const char *s)
{
	if(strstr (s, "\""))
		return false;
	if(strstr (s, ";"))
		return false;
	return true;
};

void Info_SetValueForKey(char *s, const char *key, const char *value)
{
	char newi[MAX_INFO_STRING], *v;
	int c;
	int maxsize = MAX_INFO_STRING;

	if(strstr(key, "\\") || strstr (value, "\\") )
	{
		Com_Printf("Can't use keys or values with a \\\n");
		return;
	};

	if(strstr(key, ";") )
	{
		Com_Printf("Can't use keys or values with a semicolon\n");
		return;
	};

	if(strstr(key, "\"") || strstr(value, "\"") )
	{
		Com_Printf("Can't use keys or values with a \"\n");
		return;
	};

	if(strlen(key) > MAX_INFO_KEY-1 || strlen(value) > MAX_INFO_KEY - 1)
	{
		Com_Printf("Keys and values must be < 64 characters.\n");
		return;
	};
	
	Info_RemoveKey(s, key);
	if(!value || !strlen(value))
		return;

	Com_sprintf(newi, sizeof(newi), "\\%s\\%s", key, value);

	if(strlen(newi) + strlen(s) > maxsize)
	{
		Com_Printf("Info string length exceeded\n");
		return;
	};

	// only copy ascii values
	s += strlen(s);
	v = newi;
	while(*v)
	{
		c = *v++;
		c &= 127; // strip high bits
		if(c >= 32 && c < 127)
			*s++ = c;
	};
	*s = 0;
};