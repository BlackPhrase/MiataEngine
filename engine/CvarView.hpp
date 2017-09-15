#pragma once

class CCvarList;

class CCvarView
{
public:
	CCvarView(CCvarList *apCvarList);
	~CCvarView();
	
	/// Writes lines containing "set variable value" for all variables
	/// with the archive flag set to true
	//void WriteVariables(FILE *f);
	void DumpToFile(FILE *f);
private:
	CCvarList *mpCvarList{nullptr};
};