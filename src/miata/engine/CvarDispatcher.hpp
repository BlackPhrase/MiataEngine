/// @file

#pragma once

#include <list>
#include "engine/ICvarDispatcher.hpp"

//struct ICvarChangeListener;
using tCvarChangeListenerList = std::list<ICvarChangeListener*>;

class CCvarDispatcher final : public ICvarDispatcher
{
public:
	CCvarDispatcher();
	~CCvarDispatcher();
	
	///
	void AddListener(ICvarChangeListener *apListener) override;
	
	///
	void RemoveListener(ICvarChangeListener *apListener) override;
	
	///
	void BroadcastCvarChange(IConVar *apVar, const char *asOldValue) override;
private:
	tCvarChangeListenerList mlstListeners;
};