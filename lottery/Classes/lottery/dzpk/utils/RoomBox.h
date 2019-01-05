//
//  RoomBox.h
//  gamePoker
//
//  Created by xavier on 14-1-9.
//
//

#ifndef __gamePoker__RoomBox__
#define __gamePoker__RoomBox__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class RoomBox : public cocos2d::Layer   //站起时不显示
{
    
public:
	virtual bool init();
	CREATE_FUNC(RoomBox);
    
	void registerWithTouchDispatcher();
    
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
	void onTouchCallback();
    
	void boxBtnCallback(CCObject *sender,CCControlEvent controlEvent);
    void getBtnCallback(CCObject *sender,CCControlEvent controlEvent);
    
    void openBoxList();
    
    void close();
    
    bool getStatusByPlayNum(int num);
};

#endif /* defined(__gamePoker__RoomBox__) */
