#pragma once

class CScreen
{
public:
	void Init();

	void Update();

	void SizeUp();
	void SizeDown();

	void BringDownConsole();
	void CenterPrint(const char *str);

	void BeginLoadingPlaque();
	void EndLoadingPlaque();

	int ModalMessage(const char *text);

#ifdef _WIN32
	void UpdateWholeScreen();
#endif
};