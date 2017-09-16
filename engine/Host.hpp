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
struct IVideo;
class CModuleLoader;
class CLogger;
class CEngineInterface;
class CCvarList;
class CCvarController;
class CCvarDispatcher;
class CCmdList;
class CCmdExecutor;
class CCmdBuffer;
class CCmdArgs;

class CHost : public IHost
{
public:
	CHost();
	~CHost();
	
	bool Init(quakeparms_t *parms) override;
	void Shutdown() override;
	
	void Frame() override;
	
	void ClearMemory();
	
	void ClientCommands(const char *fmt, ...);
	void ShutdownServer(bool crash);
	void Error(const char *error, ...);
	void EndGame(const char *message, ...);
private:
	void InitCommands();
	
	void Update(double frametime);
	void Render();
	
	quakeparms_t host_parms{};
	
	std::unique_ptr<CModuleLoader> mpModuleLoader;
	std::unique_ptr<CLogger> mpLogger;
	std::unique_ptr<CEngineInterface> mpEngineInterface;
	std::unique_ptr<CCvarList> mpCvarList;
	std::unique_ptr<CCvarController> mpCvarController;
	std::unique_ptr<CCvarDispatcher> mpCvarDispatcher;
	std::unique_ptr<CCmdList> mpCmdList;
	std::unique_ptr<CCmdExecutor> mpCmdExecutor;
	std::unique_ptr<CCmdBuffer> mpCmdBuffer;
	std::unique_ptr<CCmdArgs> mpCmdLine;
	
	IGame *mpGame{nullptr};
	IClientGame *mpClGame{nullptr};
	IMenu *mpMenu{nullptr};
	IInput *mpInput{nullptr};
	ISound *mpSound{nullptr};
	INetwork *mpNetwork{nullptr};
	IVideo *mpVideo{nullptr};
	
	bool host_initialized{false}; ///< true if into command execution
};