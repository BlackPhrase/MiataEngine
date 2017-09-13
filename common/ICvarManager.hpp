#pragma once

struct IConVar;
struct ICvarChangeListener;

struct ICvarManager
{
	///
	virtual void AddListener(ICvarChangeListener *listener) = 0;
	
	///
	virtual void RemoveListener(ICvarChangeListener *listener) = 0;
	
	///
	virtual void BroadcastCvarChange(IConVar *var, const char *oldvalue) = 0;
	
	///
	virtual bool RegisterCvar(IConVar *apVar) = 0;
	
	///
	virtual void SetCvarString(const char *var, const char *value) = 0;
	
	/// expands value to a string and calls SetCvarString
	virtual void SetCvarFloat(const char *var, float value) = 0;
	
	/// returns an empty string if not defined
	virtual const char *GetCvarString(const char *var, const char *defval) = 0;
	
	/// returns 0 if not defined or non numeric
	virtual float GetCvarFloat(const char *var, float defval) = 0;
protected:
	virtual ~ICvarManager() = default;
};