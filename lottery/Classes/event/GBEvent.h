/*
** declare global event center
** author: zhouwei
** date: 2013.06.10
*/

#ifndef __GB_EVENT__
#define __GB_EVENT__

#include <vector>
#include <map>
#include "AnsSingleton.h"
#include "GBEventIDs.h"
#include "BaseObject.h"


// -------------------------------------------------------------------
// argument for event trigger
// -------------------------------------------------------------------
class GBEventArg
{
private:
	GBEventArg(const GBEventArg&);
	GBEventArg& operator=(const GBEventArg&);
public:
	GBEventArg(){}
};

typedef const GBEventArg GBEventConstArg;

#define GEN_GBEVENT_ARG_1(cls, type1, name1) \
	ANS_SMP_STRUCT_1(cls, GBEventArg, type1, name1)

#define GEN_GBEVENT_ARG_2(cls, type1, name1, type2, name2) \
	ANS_SMP_STRUCT_2(cls, GBEventArg, type1, name1, type2, name2)

#define GEN_GBEVENT_ARG_3(cls, type1, name1, type2, name2, type3, name3) \
	ANS_SMP_STRUCT_3(cls, GBEventArg, type1, name1, type2, name2, type3, name3)

#define GEN_GBEVENT_ARG_4(cls, type1, name1, type2, name2, type3, name3, type4, name4) \
	ANS_SMP_STRUCT_4(cls, GBEventArg, type1, name1, type2, name2, type3, name3, type4, name4)

#define GEN_GBEVENT_ARG_5(cls, type1, name1, type2, name2, type3, name3, type4, name4, type5, name5) \
	ANS_SMP_STRUCT_5(cls, GBEventArg, type1, name1, type2, name2, type3, name3, type4, name4, type5, name5)

//GEN_GBEVENT_ARG_1(GBEventJsonArg, Json::Value, value)
//GEN_GBEVENT_ARG_1(GBEventVoidArg, void*, value);
//GEN_GBEVENT_ARG_1(GBEventIntArg, int, value);
//GEN_GBEVENT_ARG_1(GBEventStringArg, string, value);
// -------------------------------------------------------------------
// global event center
// -------------------------------------------------------------------
using namespace std;


class GBEvent : public ans::Singleton<GBEvent>
{
public:
	typedef void (BaseObject::*NoArgHandler)();					// event handler take no argument
	typedef void (BaseObject::*ArgHandler)(GBEventConstArg&);	// event handler containa a argument
	
private:
	class EventInfo
	{
	public:
		EventInfo(BaseObject* pReceiver, NoArgHandler pHandler)
			: m_pReceiver(pReceiver)
			, m_pNoArgHandler(pHandler)
		{}

		EventInfo(BaseObject* pReceiver, ArgHandler pHandler)
			: m_pReceiver(pReceiver)
			, m_pArgHandler(pHandler)
		{}

		inline void trigger()
		{
			(m_pReceiver->*m_pNoArgHandler)();
		}
		inline void trigger(GBEventConstArg &arg)
		{
			(m_pReceiver->*m_pArgHandler)(arg);
		}

		inline bool operator==(const EventInfo& info)
		{
			return m_pReceiver == info.m_pReceiver && 
				(m_pNoArgHandler == info.m_pNoArgHandler ||
				m_pArgHandler == info.m_pArgHandler);
		}

	private:
		BaseObject* m_pReceiver;
		union
		{
			NoArgHandler m_pNoArgHandler;
			ArgHandler m_pArgHandler;
		};
	};

public:
	typedef vector<EventInfo> EInfos;						// event handler list
	typedef map<int, EInfos> EvenTable;						// all events

private:
	SINGLETON_FRIEND_SUB(GBEvent);
	GBEvent(){};
	~GBEvent(){};

public:
	void regist(GBEVENT_ID, BaseObject*, NoArgHandler);
	void unregist(GBEVENT_ID, BaseObject*, NoArgHandler);
	void fire(GBEVENT_ID);

	void regist(GBEVENT_ID, BaseObject*, ArgHandler);
	void unregist(GBEVENT_ID, BaseObject*, ArgHandler);
	void fire(GBEVENT_ID, GBEventConstArg&);					

private:
	EvenTable m_Handlers;
};


// -------------------------------------------------------------------
// definations
// -------------------------------------------------------------------
#define GBEVENT_ARG_REGISTER(eid, func)\
	GBEvent::instance()->regist(eid, COMPATIBLE_CAST<BaseObject*>(this), (GBEvent::ArgHandler)&func)

#define GBEVENT_ARG_UNREGISTER(eid, func)\
	GBEvent::instance()->unregist(eid, COMPATIBLE_CAST<BaseObject*>(this), (GBEvent::ArgHandler)&func)

#define GBEVENT_NOARG_REGISTER(eid, func)\
    GBEvent::instance()->regist(eid, COMPATIBLE_CAST<BaseObject*>(this), (GBEvent::NoArgHandler)&func)

#define GBEVENT_NOARG_UNREGISTER(eid, func)\
	GBEvent::instance()->unregist(eid, COMPATIBLE_CAST<BaseObject*>(this), (GBEvent::NoArgHandler)&func)

#endif