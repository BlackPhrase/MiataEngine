#pragma once

#include "commontypes.hpp"

/*
==========================================================

CVARS (console variables)

==========================================================
*/

enum class eConVarFlags : int
{
	Archive = BIT(0),   ///< set to cause it to be saved to vars.rc
	UserInfo = BIT(1),  ///< added to userinfo  when changed
	ServerInfo = BIT(2) ///< added to serverinfo when changed
};

struct IConVar
{
	///
	virtual void Release() = 0;
	
	///
	virtual const char *GetName() const = 0;
	
	///
	virtual const char *GetDesc() const = 0;
	
	///
	virtual int GetFlags() const = 0;
	
	///
	virtual void SetString(const char *value) = 0;
	
	///
	virtual void SetInt(int value) = 0;
	
	///
	virtual void SetFloat(float value) = 0;
	
	///
	virtual void SetBool(bool value) = 0;
	
	///
	virtual const char *GetString() const = 0;
	
	///
	virtual int GetInt() const = 0;
	
	///
	virtual float GetFloat() const = 0;
	
	///
	virtual bool GetBool() const = 0;
protected:
	virtual ~IConVar() = default;
};