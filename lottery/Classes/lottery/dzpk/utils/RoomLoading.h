//
//  RoomLoading.h
//  gamePoker
//
//  Created by xavier on 13-12-24.
//
//

#ifndef __gamePoker__RoomLoading__
#define __gamePoker__RoomLoading__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class RoomLoading : public cocos2d::Layer
{
    
public:
    RoomLoading();
    ~RoomLoading();
    
	virtual bool init();
	CREATE_FUNC(RoomLoading);
    
	//void registerWithTouchDispatcher();
    
	virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    void close();
    
    void timeOut(float dt);
    
private:
    bool isPlayEffect;
};


#endif /* defined(__gamePoker__RoomLoading__) */
