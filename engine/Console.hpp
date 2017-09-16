/// @file

#pragma once

class CConsole final //: public IConsole
{
public:
	CConsole();
	~CConsole();
	
	void Print(const char *msg);
};