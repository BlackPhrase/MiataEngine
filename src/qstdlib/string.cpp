/*
This file is part of Miata Engine
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2022 BlackPhrase

Miata Engine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Miata Engine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Miata Engine. If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#include <qstdlib/string.h>

// FIXME: replace all Q_stricmp with Q_strcasecmp
int Q_stricmp(const char *s1, const char *s2)
{
#ifdef _WIN32
	return _stricmp(s1, s2);
#else
	return strcasecmp(s1, s2);
#endif
};

int Q_strncasecmp(const char *s1, const char *s2, int n)
{
	int c1, c2;
	
	do
	{
		c1 = *s1++;
		c2 = *s2++;

		if(!n--)
			return 0; // strings are equal until end point
		
		if(c1 != c2)
		{
			if(c1 >= 'a' && c1 <= 'z')
				c1 -= ('a' - 'A');
			if(c2 >= 'a' && c2 <= 'z')
				c2 -= ('a' - 'A');
			if(c1 != c2)
				return -1; // strings not equal
		};
	}
	while(c1);
	
	return 0; // strings are equal
};

int Q_strcasecmp(const char *s1, const char *s2)
{
	return Q_strncasecmp(s1, s2, 99999);
};