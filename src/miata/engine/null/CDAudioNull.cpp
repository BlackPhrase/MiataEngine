/*
Copyright (C) 1996-2001 Id Software, Inc.
Copyright (C) 2017-2018 BlackPhrase

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/// @file

#include "quakedef.hpp"

CCDAudio::CCDAudio() = default;
CCDAudio::~CCDAudio() = default;

int CCDAudio::Init()
{
	return 0;
};

void CCDAudio::Shutdown(){};

void CCDAudio::Update(){};

void CCDAudio::Play(byte track, bool looping){}; // TODO: int track?

void CCDAudio::Stop(){};

void CCDAudio::Pause(){};

void CCDAudio::Resume(){};