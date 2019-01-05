#ifndef __Game_PopupNode_h
#define __Game_PopupNode_h

#include "Inc.h"
#include "CtrlManager.h"
#include "Sound.h"

class PopupNode: public Layer
{

public:
	PopupNode();
	virtual ~PopupNode();
	virtual bool init();
	//CREATE_FUNC(PopupNode);

public:    
	virtual void onEnter();
	virtual void onExit();
    //void registerWithTouchDispatcher();
    //virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

public:
    virtual void ctrlBtnCallbck(CCObject* pSender,cocos2d::extension::Control::EventType event) = 0;
	virtual void menuCallback(CCObject* pSender) = 0;
    
public:
    void setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun);
    
    bool addMenu(const char *normalImage, const char *selectedImage,int tag);
    bool addMenu(const char *normalImage, const char *selectedImage,const char* title,CCPoint point,int tag,int fontSize=22);
    bool addMenu(const char *normalImage, const char *selectedImage,const char* title,CCPoint point,int tag,CCPoint textPoint,int fontSize=22);
    bool addMenu(CCSprite* normalSprite, CCSprite* selectedSprite,int tag);
    bool addMenu(const char *normalImage, const char *selectedImage,CCPoint& point,int tag);
	bool addMenu(CCSprite* normalSprite, CCSprite* selectedSprite,CCPoint& point,int tag);
    
    
    void setBgColorOpacity(UNS_INT opacity);
    
private:
    void initUI();

public:
	UNS_INT visibleW;
	UNS_INT visibleH;
	CCPoint originPoint;

	CCMenu* pMenu;
    CCObject* m_callbackListener;
    SEL_CallFuncN m_callback;
    CCNode* pContentNode;
    CCSize contentSize;
    CCLayerColor* bgColorLayer;
    
};

#endif