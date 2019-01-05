#ifndef _COM_RESULT_H_
#define _COM_RESULT_H_

#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;


class ComResult : public Layer
{
public:
	ComResult(void);
	virtual ~ComResult(void);
	virtual bool init();
	CREATE_FUNC(ComResult);
	void setResult(bool bwin, int repeatTimes = 1, int delayTime = -1);
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *event);
private:
	EventListenerTouchOneByOne* _touchListener;
};


#endif //_COM_RESULT_H_
