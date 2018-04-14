#include "commontypes.hpp"
#include "Input.hpp"

C_EXPORT IInput *GetInput(IEngineInterface *apEngineInterface)
{
	static CInput Input(apEngineInterface);
	return &Input;
};