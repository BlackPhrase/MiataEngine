#pragma once

class CStatusBar
{
public:
	void Init();

	/// called every frame by screen
	void Draw();

	/// call whenever any of the client stats represented on the sbar changes
	void Changed();

	/// called each frame after the level has been completed
	void IntermissionOverlay();

	void FinaleOverlay();
};