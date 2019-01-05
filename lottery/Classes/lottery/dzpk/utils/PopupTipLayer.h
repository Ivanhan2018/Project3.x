#ifndef __Game_PopupTipLayer_h
#define __Game_PopupTipLayer_h

#include "PopupNode.h"

class PopupTipLayer : public PopupNode
{
    
public:
    PopupTipLayer();
    virtual ~PopupTipLayer();
    CREATE_FUNC(PopupTipLayer);

public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
public:
    virtual void ctrlBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event){};
	virtual void menuCallback(CCObject* pSender);
    void closeCallback(/*CCObject* pSender*/);
    
public:
    void setTitle(const char* title);
    void setContent(const char* content);
    void setContent(const char* content,CCVerticalTextAlignment vTextAlignment,CCTextAlignment hTextAlignment);
    
private:
    void initUI();
    
};

#endif