/*
Copyright (C) 2019-2020 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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