/*
** implement event delegate
** author: zhouwei
** date: 2012.06.05
*/


#ifndef __EVENT_H__
#define __EVENT_H__

#include <vector>
#include "AnsMacros.h"

namespace ans {

// -----------------------------------------------------------------------
// event argument class
// -----------------------------------------------------------------------
typedef class EventArg
{
private:
	EventArg(const EventArg&);
	EventArg& operator=(const EventArg&);

public:
	EventArg(): m_bSenderInvalid(false){}
	bool isSenderInvalid() const{return m_bSenderInvalid;}
	void dashSender() const {m_bSenderInvalid=true;} 
private:
	mutable bool m_bSenderInvalid;
} const EventConstArg;

#define GEN_EVENT_ARG_1(cls, type1, name1) \
	ANS_SMP_STRUCT_1(cls, ans::EventArg, type1, name1)

#define GEN_EVENT_ARG_2(cls, type1, name1, type2, name2) \
	ANS_SMP_STRUCT_2(cls, ans::EventArg, type1, name1, type2, name2)

#define GEN_EVENT_ARG_3(cls, type1, name1, type2, name2, type3, name3) \
	ANS_SMP_STRUCT_3(cls, ans::EventArg, type1, name1, type2, name2, type3, name3)

#define GEN_EVENT_ARG_4(cls, type1, name1, type2, name2, type3, name3, type4, name4) \
	ANS_SMP_STRUCT_4(cls, ans::EventArg, type1, name1, type2, name2, type3, name3, type4, name4)

#define GEN_EVENT_ARG_5(cls, type1, name1, type2, name2, type3, name3, type4, name4, type5, name5) \
	ANS_SMP_STRUCT_5(cls, ans::EventArg, type1, name1, type2, name2, type3, name3, type4, name4, type5, name5)


// -----------------------------------------------------------------------
// implement event information
// -----------------------------------------------------------------------
template<typename T>
class Event;

template<typename T>
class EventInfo
{
public:
	typedef void (T::*Handler)(T*, ans::EventConstArg&);
	typedef std::vector<EventInfo*> Events;
	typedef typename Events::iterator EventsIter;
	typedef typename Events::const_iterator EventsCIter;

private:
	EventInfo(T* pReceiver, Handler pHandler)
		: m_pReceiver(pReceiver)
		, m_pHandler(pHandler)
	{
	}

	inline void fire(T* pSender, ans::EventConstArg& arg)
	{
		(m_pReceiver->*m_pHandler)(pSender, arg);
	}

public:
	inline bool isEqual(T* pReceiver, Handler pHandler) const
	{
		return (pReceiver == m_pReceiver) && (pHandler == m_pHandler);
	}

private:
	T* m_pReceiver;
	Handler m_pHandler;

	friend class Event<T>;
};

// -----------------------------------------------------------------------
// event delegate
// -----------------------------------------------------------------------
template<typename T>
class Event
{
public:
	Event(T* pSender)
		: m_pSender(pSender)
	{
		m_events.reserve(2);
	}

	virtual ~Event();

	inline void bind(T* pReciver, typename EventInfo<T>::Handler pHandler) 
	{
		m_events.push_back(new EventInfo<T>(pReciver, pHandler));
	}

	void unbind(T*, typename EventInfo<T>::Handler);
	
	template<typename Arg> void fire(const Arg &arg);
	
	inline int binderCount() const { return m_events.size(); }

private:
	T* m_pSender;
	typename EventInfo<T>::Events m_events;
};

// -----------------------------------------------------------------------
// implements
// -----------------------------------------------------------------------
template<typename T>
Event<T>::~Event()
{
	typename EventInfo<T>::EventsCIter iter = m_events.begin();
	while(iter != m_events.end())
		delete (*iter++);
	m_events.clear();
}

template<typename T>
void Event<T>::unbind(T* pReciver, typename EventInfo<T>::Handler pHandler) 
{
	typename EventInfo<T>::EventsIter iter = m_events.begin();
	for(; iter != m_events.end(); ++iter)
	{
		if ((*iter)->isEqual(pReciver, pHandler))
			break;
	}
	if (iter != m_events.end())
	{
		m_events.erase(iter);
	}
}

template<typename T>
template<typename Arg>
void Event<T>::fire(const Arg& arg)
{
	typename EventInfo<T>::Events es(m_events.begin(), m_events.end());
	typename EventInfo<T>::EventsCIter iter = es.begin();
	for(; iter != es.end(); ++iter)
	{
		(*iter)->fire(m_pSender, (EventConstArg&)arg);
	}
}

}
#endif