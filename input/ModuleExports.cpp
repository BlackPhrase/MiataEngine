#include "commontypes.hpp"
#include "Input.hpp"

C_EXPORT IInput *GetInput()
{
	static CInput Input;
	return &Input;
};