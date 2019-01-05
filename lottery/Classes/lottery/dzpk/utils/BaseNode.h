#ifndef __Game__BaseNode__
#define __Game__BaseNode__

#include "Inc.h"
//#include "Loading.h"
//#include "CtrlManager.h"
#include "Sound.h"

class BaseNode : public Layer
{
	
public:
	BaseNode();
	virtual ~BaseNode();
	//CREATE_FUNC(BaseNode);

public:
	virtual bool init();
	//virtual void keyBackClicked();
	//virtual void keyMenuClicked();
	void exitGame();
    
public:
	void setBackground(CCTexture2D* pBgTexture);
    //const char* getStrFromXml(const char* key);
    //Loading* createLoading();
    void stopLoading();
    
public:
    void createPopupTipLayer(const char* title,const char* content,TIP_TAG currTipTag);
    void createPopupTipLayer(const char* title,const char* content,CCVerticalTextAlignment vTextAlignment,CCTextAlignment hTextAlignment,TIP_TAG currTipTag);
    virtual void popupTipLayerCallback(CCNode* pSender){};
    void createToastLayer(const char* msg,float delay);
    void createToastLayer(const char* msg,float delay,CCPoint point);
    void showToastCallback(CCNode* pSender);
    
public:
    virtual void ctrlBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event) = 0;
	virtual void menuCallback(CCObject* pSender) = 0;

private:
	void initUI();

public:
	UNS_INT visibleW;
	UNS_INT visibleH;
	CCPoint originPoint;

public:
    TIP_TAG currTipTag;
	CCTexture2D* pStartBgTexture;
    //Loading* pLoading;
};

#endif