#include "Cvar.hpp"

void CCvar::RegisterVariable(cvar_t *variable)
{
};

void CCvar::Set(const char *var_name, const char *value)
{
};

void CCvar::SetValue(const char *var_name, float value)
{
};

float CCvar::VariableValue(const char *var_name)
{
};

char *CCvar::VariableString(const char *var_name)
{
};

char *CCvar::CompleteVariable(const char *partial)
{
};

bool CCvar::HandleCommand()
{
};

void CCvar::WriteVariables(FILE *f)
{
};

cvar_t *CCvar::FindVar(const char *var_name)
{
};