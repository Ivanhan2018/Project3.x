//
//  RoomRule.h
//  gamePoker
//
//  Created by xavier on 13-11-28.
//
//

#ifndef gamePoker_RoomRule_h
#define gamePoker_RoomRule_h

#include "cocos2d.h"

using namespace cocos2d;


class RoomRule : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(RoomRule);

    //void registerWithTouchDispatcher();
    
	virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
	void onTouchCallback();

public:
    CCSize size;
};

#endif
