/// @file

#include "CvarDispatcher.hpp"

CCvarDispatcher::CCvarDispatcher() = default;

CCvarDispatcher::~CCvarDispatcher()
{
	for(auto It : mlstListeners)
		It->Release();
	
	mlstListeners.clear();
};

void CCvarDispatcher::AddListener(ICvarChangeListener *apListener)
{
	mlstListeners.push_back(apListener);
};

void CCvarDispatcher::RemoveListener(ICvarChangeListener *apListener)
{
	//mlstListeners.erase();
	// TODO
};

void CCvarDispatcher::BroadcastCvarChange(IConVar *apVar, const char *asOldValue)
{
	for(auto It : mlstListeners)
		It->OnChange(apVar, asOldValue);
};