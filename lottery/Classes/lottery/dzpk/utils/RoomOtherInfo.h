//
//  RoomOtherInfo.h
//  gamePoker
//
//  Created by xavier on 14-1-9.
//
//

#ifndef __gamePoker__RoomOtherInfo__
#define __gamePoker__RoomOtherInfo__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class RoomOtherInfo : public cocos2d::Layer   //站起时不显示
{
    
public:
	virtual bool init();
	CREATE_FUNC(RoomOtherInfo);
    
	//void registerWithTouchDispatcher();
    
	virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
	void onTouchCallback();
    
	void getBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);
    
    void initUI(int index);
    
    void close();
    
    void closeCallback();
    
private:
    CCSize f_size;
};

#endif /* defined(__gamePoker__RoomOtherInfo__) */
