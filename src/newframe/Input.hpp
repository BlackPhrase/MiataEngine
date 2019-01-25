#pragma once

class CInput
{
public:
	void Init();

	void Shutdown();

	/// Oportunity for devices to stick commands on the script buffer
	void Commands();

	/// Add additional movement on top of the keyboard move cmd
	void Move(usercmd_t *cmd);

	/// Restores all button and position states to defaults
	void ClearStates();

	/// Called whenever screen dimensions change
	void ModeChanged();
};