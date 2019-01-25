#pragma once

class CEdict
{
public:
	edict_t *Alloc();
	void Free(edict_t *ed);

	/// returns a copy of the string allocated from the server's string heap
	char *NewString(const char *string);

	void Print(edict_t *ed);
	void Write(FILE *f, edict_t *ed);
	
	char *ParseEdict(char *data, edict_t *ent);

	void WriteGlobals(FILE *f);
	void ParseGlobals(char *data);

	void LoadFromFile(char *data);

	void PrintEdicts();
	void PrintNum(int ent);

	eval_t *GetEdictFieldValue(edict_t *ed, const char *field);
};