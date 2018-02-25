#pragma once

class CEdict;

class CEdictPool final
{
public:
	CEdictPool() = default;
	~CEdictPool() = default;
	
	CEdict *Alloc(); //const
	void Free(CEdict *ed);
private:
};