
#ifndef __CCControl__Button__
#define __CCControl__Button__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

typedef void (CCObject::*SEL_COMMON)(int);

#define common_selector(_SELECTOR) (SEL_COMMON)(&_SELECTOR)

USING_NS_CC;

class Button : public CCSprite//, public CCTargetedTouchDelegate
{
public:
    Button(void);
    virtual ~Button(void);
    
    static Button *create(const char *fileName);
    static Button *createWithSpriteFrameName(const char *fileName);
    
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    bool isTouchInside(CCTouch* touch);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    void setSelector(CCObject* obj, SEL_COMMON selector);
    void setDisplayRect(CCRect rect);
    
public:
    CCPoint beginPoint;
    
    SEL_COMMON m_pfnCallFunc;
    
    CCObject* m_pobj;
    
    CCRect m_rect;
	
	int status;
};

#endif /* defined(__CCControl__Button__) */
