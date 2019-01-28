#include "Cmd.hpp"

void CCmd::Init()
{
};

void CCmd::AddCommand(const char *cmd_name, xcommand_t function)
{
};

bool CCmd::Exists(const char *cmd_name)
{
};

char *CCmd::CompleteCommand(const char *partial)
{
};

int CCmd::GetArgc()
{
};

char *CCmd::GetArgv(int arg)
{
};

char *CCmd::GetArgs()
{
};

int CCmd::CheckParm(const char *parm)
{
};

void CCmd::TokenizeString(const char *text)
{
};

void CCmd::ExecuteString(const char *text, cmd_source_t src)
{
};

void CCmd::ForwardToServer()
{
};

void CCmd::Print(const char *text)
{
};