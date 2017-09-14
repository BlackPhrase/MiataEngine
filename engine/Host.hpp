/*
Copyright (C) 1996-1997 Id Software, Inc.
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

#pragma once

#include <memory>
#include "IHost.hpp"

struct IGame;
struct IClientGame;
struct IMenu;
struct IInput;
struct ISound;
struct INetwork;
class CModuleLoader;
class CCvarList;
class CCvarManager;
class CCmdList;
class CCmdExecutor;
class CCmdBuffer;

class CHost : public IHost
{
public:
	CHost();
	~CHost();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame(double frametime) override;
	
	void ClearMemory();
	
	void ClientCommands(const char *fmt, ...);
	void ShutdownServer(bool crash);
	void Error(const char *error, ...);
	void EndGame(const char *message, ...);
private:
	void InitCommands();
	
	void _Frame(double frametime);
	
	std::unique_ptr<CModuleLoader> mpModuleLoader;
	std::unique_ptr<CCvarList> mpCvarList;
	std::unique_ptr<CCvarManager> mpCvarManager;
	std::unique_ptr<CCmdList> mpCmdList;
	std::unique_ptr<CCmdExecutor> mpCmdExecutor;
	std::unique_ptr<CCmdBuffer> mpCmdBuffer;
	
	IGame *mpGame{nullptr};
	IClientGame *mpClGame{nullptr};
	IMenu *mpMenu{nullptr};
	IInput *mpInput{nullptr};
	ISound *mpSound{nullptr};
	INetwork *mpNetwork{nullptr};
	
	bool host_initialized{false}; ///< true if into command execution
};