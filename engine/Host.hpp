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