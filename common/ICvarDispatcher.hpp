/*
Copyright (C) 2017 Sh1ft0x0EF

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

#pragma once

struct IConVar;

struct ICvarChangeListener
{
	virtual void OnChange(IConVar *apCvar, const char *asOldValue) = 0;
	
	virtual void Release() = 0; //{delete this;}
protected:
	virtual ~ICvarChangeListener() = default;
};

struct ICvarDispatcher
{
	///
	virtual void AddListener(ICvarChangeListener *listener) = 0;
	
	///
	virtual void RemoveListener(ICvarChangeListener *listener) = 0;
	
	///
	virtual void BroadcastCvarChange(IConVar *var, const char *oldvalue) = 0;
};