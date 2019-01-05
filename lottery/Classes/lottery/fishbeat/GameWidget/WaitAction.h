#ifndef _WAIT_ACTION_LAYER_
#define _WAIT_ACTION_LAYER_

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

class WaitAction : public cocos2d::Layer
{
public:
	WaitAction();
	~WaitAction();
	virtual bool init();
	static WaitAction* create();
	static WaitAction* GetInstance() { return m_waitAction; }
	static WaitAction* m_waitAction;
};

#endif // _WAIT_ACTION_LAYER_
