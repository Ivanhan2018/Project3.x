//
//  SwitchSprite.h
//  TexasPoker
//
//  Created by peniel on 1/3/14.
//
//

#ifndef __TexasPoker__SwitchSprite__
#define __TexasPoker__SwitchSprite__

#include "Inc.h"

class SwitchSprite : public Layer
{
    
public:
    SwitchSprite();
    virtual ~SwitchSprite();
    CREATE_FUNC(SwitchSprite);
    
public:
    static SwitchSprite* createWith(const char* on,const char* off,const char* thumb);
    static SwitchSprite* createWith(CCSprite* pOnSprite,CCSprite* pOffSprite,CCSprite* pThumbSprite);
    virtual bool init();
    const bool isOn() const;
    void setSwitchStatus(bool onOrOff);
    
public:
    void setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun);
    bool addMenu(const char *normalImage, const char *selectedImage,int tag);
    void menuCallback(CCObject* pSender);
    
private:
    void initUI();
    void switchAnimFinishCallback(float dt);
    
    
private:
    CCSprite* pOnSprite;
    CCSprite* pOffSprite;
    CCSprite* pThumbSprite;
    CCLabelTTF* pOnTTF;
    CCLabelTTF* pOffTTF;
    CCSize switchSize;
    bool status;
    bool isSwitchAnimFinish;
   
private:
    CCMenu* pMenu;
    CCObject* m_callbackListener;
    SEL_CallFuncN m_callback;
    
};

#endif /* defined(__TexasPoker__SwitchSprite__) */
