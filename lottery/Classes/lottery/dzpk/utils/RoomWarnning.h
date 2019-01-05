//
//  RoomWarnning.h
//  gamePoker
//
//  Created by xavier on 14-2-14.
//
//

#ifndef __gamePoker__RoomWarnning__
#define __gamePoker__RoomWarnning__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

class RoomWarnning : public cocos2d::Layer
{
public:
    void onEnter();
    void onExit();
    
    virtual bool init();
    
    void cashAnimate();
    
    CREATE_FUNC(RoomWarnning);
    
    CCPoint getAnimatePos(int seatid);
    
    void winAnimate(int seatid);
    void winAnimateCallBack(float dt);
    void addWarnning(int seatid);
    void removeWarnning(int seatid);
    
    void hideWarnning();
    void showWarnning();
};

#endif /* defined(__gamePoker__RoomWarnning__) */
