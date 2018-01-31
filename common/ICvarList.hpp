/*
Copyright (C) 2017-2018 BlackPhrase

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

/// @file
/// @brief cvar list interface

#pragma once

struct IConVar;

struct ICvarList
{
	/// Registers a cvar that already has the name, string, and optionally the
	/// archive elements set.
	virtual bool Register(IConVar *apVar) = 0;
	
	///
	virtual IConVar *Find(const char *name) const = 0;
protected:
	virtual ~ICvarList() = default;
};