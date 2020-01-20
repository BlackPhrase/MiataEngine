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

class CHost
{
public:
	void Init(quakeparms_t *parms);
	void Shutdown();
	
	void InitCommands();
	void ShutdownServer(bool crash);
	
	void ClearMemory();
	
	void Frame(float time);
	
	void ServerFrame();
	
	void Error(const char *error, ...);
	
	void EndGame(const char *message, ...);
	
	void ClientCommands(const char *fmt, ...);
	
	void Host_Quit_f();
private:
	void _Frame(float time);
};