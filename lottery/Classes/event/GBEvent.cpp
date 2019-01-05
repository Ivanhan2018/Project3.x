/*
** implement global event center
** 
** date:
*/

#include <algorithm>
#include "GBEvent.h"

SINGLETON_DEFINE_INST(GBEvent);


void GBEvent::regist(GBEVENT_ID eid, BaseObject* pReceiver, NoArgHandler pHandler)
{
	EvenTable::iterator iter = m_Handlers.find(eid);
	if (iter == m_Handlers.end()) 
	{
		m_Handlers[eid] = EInfos();
		m_Handlers[eid].reserve(4);
	}
	m_Handlers[eid].push_back(EventInfo(pReceiver, pHandler));
}

void GBEvent::unregist(GBEVENT_ID eid, BaseObject* pReceiver, NoArgHandler pHandler)
{
	EvenTable::iterator iter = m_Handlers.find(eid);
	if (iter == m_Handlers.end()) return;
	EInfos& vinfos = m_Handlers[eid];

	EInfos::iterator viter = find(vinfos.begin(), vinfos.end(), EventInfo(pReceiver, pHandler));
	if (viter != vinfos.end())
	{
		vinfos.erase(viter);
		if (vinfos.size() == 0)
			m_Handlers.erase(eid);
	}
}

void GBEvent::fire(GBEVENT_ID eid)
{
	EvenTable::const_iterator iter = m_Handlers.find(eid);
	if (iter == m_Handlers.end()) return;

	EInfos vHInfo(iter->second.begin(), iter->second.end());
	for(EInfos::iterator viter = vHInfo.begin();
		viter != vHInfo.end(); ++viter)
	{
		viter->trigger();
	}
}

void GBEvent::regist(GBEVENT_ID eid, BaseObject* pReceiver, ArgHandler pHandler)
{
	EvenTable::iterator iter = m_Handlers.find(eid);
	if (iter == m_Handlers.end()) 
	{
		m_Handlers[eid] = EInfos();
		m_Handlers[eid].reserve(4);
	}
	m_Handlers[eid].push_back(EventInfo(pReceiver, pHandler));
}

void GBEvent::unregist(GBEVENT_ID eid, BaseObject* pReceiver, ArgHandler pHandler)
{
	EvenTable::iterator iter = m_Handlers.find(eid);
	if (iter == m_Handlers.end()) return;
	EInfos&vinfos = m_Handlers[eid]; //modified by zzg 

	EInfos::iterator viter = find(vinfos.begin(), vinfos.end(), EventInfo(pReceiver, pHandler));
	if (viter != vinfos.end())
	{
		vinfos.erase(viter);
		if (vinfos.size() == 0)
			m_Handlers.erase(eid);
	}
}

void GBEvent::fire(GBEVENT_ID eid, GBEventConstArg& arg)
{
	EvenTable::const_iterator iter = m_Handlers.find(eid);
	if (iter == m_Handlers.end()) return;

	EInfos vHInfo(iter->second.begin(), iter->second.end());
	for(EInfos::iterator viter = vHInfo.begin();
		viter != vHInfo.end(); ++viter)
	{
		viter->trigger(arg);
	}
}
