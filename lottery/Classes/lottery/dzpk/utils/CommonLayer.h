//
//  CommonLayer.h
//  TexasPoker
//
//  Created by peniel on 12/28/13.
//
//

#ifndef __TexasPoker__CommonLayer__
#define __TexasPoker__CommonLayer__

#include "Inc.h"
#include "BaseNode.h"
//#include "IconTextSprite.h"

class CommonLayer : public BaseNode
{
    
public:
    CommonLayer();
	virtual ~CommonLayer();
	CREATE_FUNC(CommonLayer);
    
public:
	virtual bool init();
    virtual void onEnter();
    virtual void onExit();
	void onEnterTransitionDidFinish();
    
public:
    virtual void ctrlBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event){};
	virtual void menuCallback(CCObject* pSender){};
    void backBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event);
    
public:
    void addTopNode(const char* title);
    void addTopNodeUserInfo();
    CCNode* getTopNode();
    void addRechargeTopNode(const char* title);
    void setCurrScene(SCENE currScene);
    
public:
    void notifiUpdateRichCallback(CCObject* pCCObjData);
    void updateTopNodeRich();
    void isChangeingRichCallback(float dt);
    
private:
    void updateRich();
    
public:
    CCNode* pTopNode;
    //IconTextSprite* pRmbIconText;
    //IconTextSprite* pShipIconText;
    bool isChangeingRich;
    SCENE currScene;  
};

#endif /* defined(__TexasPoker__CommonLayer__) */
