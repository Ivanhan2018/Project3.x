//
//  ButtonSprite.h
//  TexasPoker
//
//  Created by peniel on 1/8/14.
//
//

#ifndef __TexasPoker__ButtonSprite__
#define __TexasPoker__ButtonSprite__

#include "Inc.h"

typedef  void  (CCObject::*ButtonClick_Func)();
typedef  void  (CCObject::*ButtonClick_FuncN)(CCObject* obj);
typedef  void  (CCObject::*ButtonClick_FuncND)(CCObject* obj,void* param);


#define click_selector(_SELECTOR) (ButtonClick_Func)(&_SELECTOR)
#define click_selectorN(_SELECTOR) (ButtonClick_FuncN)(&_SELECTOR)
#define click_selectorND(_SELECTOR) (ButtonClick_FuncND)(&_SELECTOR)

class ButtonSprite : public CCSprite//,public CCTouchDelegate
{
    int m_touchPriority;
    bool m_isDown;
    ButtonClick_Func m_BtnClick_Func;
public:
    ButtonSprite();
    virtual ~ButtonSprite();
    
    // 调整按钮的默认优先级,设置为最高优先级
    static ButtonSprite* createButtonSprite(const char* fileName,int tPriority = -128);
    
    static ButtonSprite* createButtonSpriteWithSpriteFrame(CCSpriteFrame *pSpriteFrame,int tPriority = -128);
    
    /* 触摸相关 */
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    // optional
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void clicked();
    
    void setOnClickListener(ButtonClick_Func tBtnClickFunc);
};

#endif /* defined(__TexasPoker__ButtonSprite__) */
