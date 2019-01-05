//
//  RoomProgress.h
//  ffff
//
//  Created by xavier on 13-11-26.
//
//

#ifndef _Game_RoomProgress_H__
#define _Game_RoomProgress_H__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class RoomProgress : public cocos2d::Layer
{
public:
    virtual bool init ();
    CREATE_FUNC(RoomProgress);
    
    void initLayer();

    
    void okBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);
    
    void betBtnCallback(int tag);
    
    void updateNum(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);
    
    //void registerWithTouchDispatcher();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
public:
    int count;
    int getCurrCount();

};

#endif
