//
//  RoomPopFrame.h
//  gamePoker
//
//  Created by xavier on 13-12-11.
//
//

#ifndef gamePoker_RoomPopFrame_h
#define gamePoker_RoomPopFrame_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Button.h"

using namespace cocos2d;
using namespace cocos2d::extension;


class RoomPopFrame : public cocos2d::Layer
{
public:
    RoomPopFrame();
    ~RoomPopFrame();
    virtual bool init();
    CREATE_FUNC(RoomPopFrame);
    
    void createDialog(const char *info);
    void createDialog(const char *title ,const char *info);
    void createDialog(const char *info,const char *btn_title1 ,const char *btn_title2);
	//void registerWithTouchDispatcher();
	virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void okBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);
    void canelBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);
    void close();
    void closeCallback();
    void setClickOnce(bool var);
    
    void setSelector(CCObject* obj, SEL_COMMON selector);
    SEL_COMMON m_pfnCallFunc;
    CCObject* m_pobj;
    
    void setReturnSelector(CCObject* obj, SEL_COMMON selector);
    SEL_COMMON m_pfnCallFuncs;
    CCObject* m_pobjs;
    
private:
    bool m_click;

};

#endif
