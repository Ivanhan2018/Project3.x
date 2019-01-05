//////////////////////////////////////////////////////////////////////////
//

#include "UINotificationCenter.h"

USING_NS_CC;
std::mutex g_UINotificationMutex;
static CUINotificationCenter *s_sharedUINotifCenter = nullptr;

// CUINotificationObserver
class CUINotificationObserver : public Ref
{
	friend class  CUINotificationCenter;
protected:
    CUINotificationObserver(Ref *target, 
                           SEL_CallFuncO selector,
                           const std::string& name,
                           Ref *sender);
public:
    virtual ~CUINotificationObserver();      
    
    // Invokes the callback function of this observer 
    void performSelector(Ref *sender);
    
    // Getters / Setters
    inline Ref* getTarget() const;
   
    inline SEL_CallFuncO getSelector() const;
   
    inline const std::string& getName() const;
    
    inline Ref* getSender() const;

private:
	class CUINotiGM
	{
	public:
		virtual ~CUINotiGM()
		{
			if (CUINotificationCenter::getInstance())
			{
				CUINotificationCenter::destroyInstance();
			}
		}
	};

	typedef struct UINotificationArg
	{
		std::string name;
		cocos2d::Ref* sender;
	}_UINotificationArg;

	static CUINotiGM s_UINotiGM;

    Ref* _target;
    SEL_CallFuncO _selector;
    std::string _name;
    Ref* _sender;
};

// static 
CUINotificationObserver::CUINotiGM CUINotificationObserver::s_UINotiGM;

//////////////////////////////////////////////////////////////////////////
CUINotificationObserver::CUINotificationObserver(Ref *target, 
						SEL_CallFuncO selector,
						const std::string& name,
						Ref *sender)
{
	_target = target;
	_selector = selector;
	_sender = sender;
	_name = name;
}

CUINotificationObserver::~CUINotificationObserver()
{

}

// Invokes the callback function of this observer 
void CUINotificationObserver::performSelector(Ref *sender)
{
	if (_target)
	{
		if (sender) {
			(_target->*_selector)(sender);
		} else {
			(_target->*_selector)(_sender);
		}
	}
}

// Getters / Setters
inline Ref* CUINotificationObserver::getTarget() const
{
	return _target;
}

inline SEL_CallFuncO CUINotificationObserver::getSelector() const
{
	return _selector;
}

inline const std::string& CUINotificationObserver::getName() const
{
	return _name;
}

inline Ref* CUINotificationObserver::getSender() const
{
	return _sender;
}

//////////////////////////////////////////////////////////////////////////
// CUINotificationCenter
CUINotificationCenter::CUINotificationCenter()
{
	_observers = __Array::createWithCapacity(3);
	_observers->retain();
}

CUINotificationCenter::~CUINotificationCenter()
{
	_observers->release();
}

// Gets the single instance of CUINotificationCenter.
CUINotificationCenter *CUINotificationCenter::getInstance()
{
	if (!s_sharedUINotifCenter)
	{
		s_sharedUINotifCenter = new CUINotificationCenter();
	}
	return s_sharedUINotifCenter;
}

// Destroys the single instance of CUINotificationCenter.
void CUINotificationCenter::destroyInstance()
{
	CC_SAFE_RELEASE_NULL(s_sharedUINotifCenter);
}

// @brief Posts one notification event by name.
// @param name The name of this notification.
void CUINotificationCenter::postNotification(const std::string& name)
{
	this->postNotification(name,nullptr);
}

// @brief Posts one notification event by name.
// @param name The name of this notification.
// @param sender The object posting the notification. Can be nullptr
void CUINotificationCenter::postNotification(const std::string& name, Ref *sender)
{	
	_UINotificationArg arg;
	arg.name = name;

	if(sender != nullptr)
		arg.sender = sender;
	else
		arg.sender = nullptr;

	std::lock_guard<std::mutex> ul(g_UINotificationMutex);
	_args.push_back(arg);
}

// Invokes the callback function of this 
void CUINotificationCenter::scheduleSelector(float dt)
{
	__Array* ObserversCopy = __Array::createWithCapacity(_observers->count());
	ObserversCopy->addObjectsFromArray(_observers);
	Ref* obj = nullptr;
	std::vector<_UINotificationArg> _args_dest;
	// locker
	{
		std::lock_guard<std::mutex> ul(g_UINotificationMutex);
		std::copy(_args.begin(), _args.end(), std::back_inserter(_args_dest));
		_args.clear();
	}
	
	for(uint32_t i = 0; i < _args_dest.size(); ++i) {
		UINotificationArg &arg = _args_dest[i]; 
		CCARRAY_FOREACH(ObserversCopy, obj)
		{
			NotificationObserver* observer = static_cast<NotificationObserver*>(obj);
			if (!observer)
				continue;

			if (observer->getName() == arg.name && (observer->getSender() == arg.sender || observer->getSender() == nullptr || arg.sender == nullptr))
			{
				observer->performSelector(arg.sender);
			}
		}
	}
}

// @brief Adds an observer for the specified target.
// @param target The target which wants to observe notification events.
// @param selector The callback function which will be invoked when the specified notification event was posted.
// @param name The name of this notification.
// @param sender The object whose notifications the target wants to receive. Only notifications sent by this sender are delivered to the target. nullptr means that the sender is not used to decide whether to deliver the notification to target.
void CUINotificationCenter::addObserver(Ref *target, SEL_CallFuncO selector, const std::string& name, Ref *sender)
{
	if (this->observerExisted(target, name, sender))
		return;

	CUINotificationObserver *observer = new CUINotificationObserver(target, selector, name, sender);
	if (!observer)
		return;

	observer->autorelease();
	_observers->addObject(observer);
}

// @brief Removes the observer by the specified target and name.
// @param target The target of this notification.
// @param name The name of this notification. 
void CUINotificationCenter::removeObserver(Ref *target,const std::string& name)
{
	Ref* obj = nullptr;
	CCARRAY_FOREACH(_observers, obj)
	{
		CUINotificationObserver* observer = static_cast<CUINotificationObserver*>(obj);
		if (!observer)
			continue;

		if (observer->getName() == name && observer->getTarget() == target)
		{
			_observers->removeObject(observer);
			return;
		}
	}
}

// @brief Removes all notifications registered by this target
// @param target The target of this notification.
// @returns the number of observers removed
int CUINotificationCenter::removeAllObservers(Ref *target)
{
	Ref *obj = nullptr;
	__Array *toRemove = __Array::create();

	CCARRAY_FOREACH(_observers, obj)
	{
		CUINotificationObserver *observer = static_cast<CUINotificationObserver *>(obj);
		if (!observer)
			continue;

		if (observer->getTarget() == target)
		{
			toRemove->addObject(observer);
		}
	}

	_observers->removeObjectsInArray(toRemove);
	return static_cast<int>(toRemove->count());
}

// Check whether the observer exists by the specified target and name.
bool CUINotificationCenter::observerExisted(Ref *target,const std::string& name, Ref *sender)
{
	Ref* obj = nullptr;
	CCARRAY_FOREACH(_observers, obj)
	{
		CUINotificationObserver* observer = (CUINotificationObserver*) obj;
		if (!observer)
			continue;

		if (observer->getName() == name && observer->getTarget() == target && observer->getSender() == sender)
			return true;
	}
	return false;
}