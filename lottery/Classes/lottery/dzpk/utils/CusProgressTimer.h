#ifndef __TexasPoker__CusProgressTimer__
#define __TexasPoker__CusProgressTimer__

//#include "Inc.h"
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

typedef void (CCObject::*SEL_COMMON)(int);

#define common_selector(_SELECTOR) (SEL_COMMON)(&_SELECTOR)

USING_NS_CC;

class CusProgressTimer : public CCNode
{

public:
    CusProgressTimer();
    virtual ~CusProgressTimer();
    CREATE_FUNC(CusProgressTimer);
    
public:
    static CusProgressTimer* createWith(const char* progressPic,float duration);
    static CusProgressTimer* createWith(CCSprite* progressSprite,float duration);
    virtual bool init();
    void updateProgressCallback(float dt);
    void setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun);
    
    void setSelector(CCObject* obj, SEL_COMMON selector);

    SEL_COMMON m_pfnCallFunc;
    
    CCObject* m_pobj;
    
    void setSelector2(CCObject* obj, SEL_COMMON selector);
    
    CCObject*  m_pobj2;
    SEL_COMMON m_pfnCallFunc2;
    
    void setSelector3(CCObject* obj, SEL_COMMON selector);
    
    CCObject*  m_pobj3;
    SEL_COMMON m_pfnCallFunc3;
    
    float time;
    
    void setLeftTime(float times);
    float m_left_time;
    
    void updateProgress();
    
private:
	void initUI();
    void progressEndCallback(cocos2d::CCObject *pSender);

private:
    CCSprite* progressSprite;
    float duration;
    float offerset_r;
    float offerset_g;
    
    float m_percent;
};

#endif
