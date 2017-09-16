#include "Input.hpp"

CInput::CInput(IEngineInterface *apEngine)
	: mpEngine(apEngine){}
CInput::~CInput() = default;

bool CInput::Init()
{
	mpEngine->GetLogger()->Printf("Input initialization...");
	
	return true;
};

void CInput::Shutdown()
{
	mpEngine->GetLogger()->Printf("Input shutdown...");
};

void CInput::Frame()
{
};