// UINotificationCenter.h : main header file for the UINotificationCenter
//
#if !defined(_UINOTIFICATIONCENTER_H__INCLUDED_)
#define _UINOTIFICATIONCENTER_H__INCLUDED_

#include <memory>
#include "cocos2d.h"

#include "GameMessageDefine.h"

class CUINotificationBuffer: public cocos2d::Ref
{
public:

	CUINotificationBuffer() : m_nID(0),m_pData(nullptr),m_nSize(0),m_i64Code(0) {}
	CUINotificationBuffer(std::int32_t nID, std::int64_t i64Code, void* pData, std::size_t nSize)
	{
		m_nID = nID;
		m_i64Code = i64Code;
		m_nSize = nSize;		
		if (nSize>0 && pData !=nullptr)
		{
			m_pData = new std::int8_t[nSize];
			memcpy(m_pData, pData, nSize);
		}
		else
		{
			m_pData = nullptr;
		}
	}
	virtual ~CUINotificationBuffer()
	{
		if (m_pData != nullptr)
		{
			delete []m_pData;
			m_pData = nullptr;
		}
	}

	static CUINotificationBuffer *create(std::int32_t nID,std::int64_t i64Code=0 ,void* pData=nullptr, std::size_t nSize=0)
	{
		CUINotificationBuffer *pUINotif= new CUINotificationBuffer(nID, i64Code, pData, nSize);
		return pUINotif;
	}

	bool init(std::int32_t nID, std::int64_t i64Code, void* pData, std::size_t nSize)
	{
		if (m_pData != nullptr)
		{
			delete []m_pData;
			m_pData = nullptr;
		}
		m_nID = nID;
		m_i64Code = i64Code;
		m_nSize = nSize;		
		if (nSize>0 && pData !=nullptr)
		{
			m_pData = new std::int8_t[nSize];
			memcpy(m_pData, pData, nSize);
		}
		return true;
	}
	void SetCode(std::int64_t i64Code){m_i64Code=i64Code;}
	void SetID(std::int32_t nID){m_nID=nID;}
	std::int32_t GetID()const{return m_nID;}
	std::int64_t GetCode()const{return m_i64Code;}
	void* GetData()const{return m_pData;}
	std::size_t GetSize()const{return m_nSize;}


private:
	std::int32_t m_nID;
	std::int64_t m_i64Code;
	std::size_t  m_nSize;
	std::int8_t* m_pData;
};

// AppDelegate
//director->getScheduler()->scheduleSelector( 
//	schedule_selector(CUINotificationCenter::scheduleSelector), 
//	CUINotificationCenter::getInstance(), 1.0 / 60.0, false);

// UI同步通知中心
class CUINotificationCenter : public cocos2d::Ref
{
protected:
	CUINotificationCenter();

public:
	virtual ~CUINotificationCenter();

	// Gets the single instance of CUINotificationCenter.
	static CUINotificationCenter *getInstance();

	// Destroys the single instance of CUINotificationCenter.
	static void destroyInstance();

	// @brief Posts one notification event by name.
	// @param name The name of this notification.
	void postNotification(const std::string& name);

	// @brief Posts one notification event by name.
	// @param name The name of this notification.
	// @param sender The object posting the notification. Can be nullptr
	void postNotification(const std::string& name, Ref *sender);

	// Invokes the callback function of this 
	void scheduleSelector(float dt);

	// @brief Adds an observer for the specified target.
	// @param target The target which wants to observe notification events.
	// @param selector The callback function which will be invoked when the specified notification event was posted.
	// @param name The name of this notification.
	// @param sender The object whose notifications the target wants to receive. Only notifications sent by this sender are delivered to the target. nullptr means that the sender is not used to decide whether to deliver the notification to target.
	void addObserver(Ref *target, cocos2d::SEL_CallFuncO selector, const std::string& name, Ref *sender);

	// @brief Removes the observer by the specified target and name.
	// @param target The target of this notification.
	// @param name The name of this notification. 
	void removeObserver(Ref *target,const std::string& name);

	// @brief Removes all notifications registered by this target
	// @param target The target of this notification.
	// @returns the number of observers removed
	int removeAllObservers(Ref *target);

protected:

	typedef struct UINotificationArg
	{
		std::string name;
		cocos2d::Ref* sender;
	}_UINotificationArg;

	// Check whether the observer exists by the specified target and name.
	bool observerExisted(Ref *target,const std::string& name, Ref *sender);

	// variables
	cocos2d::__Array *_observers;
	std::vector<_UINotificationArg> _args;
};

typedef std::shared_ptr<CUINotificationBuffer> CUINotificationBufferPtr;

#endif // !defined(_UINOTIFICATIONCENTER_H__INCLUDED_)
