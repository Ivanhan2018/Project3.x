#ifndef __Game__StartScene__
#define __Game__StartScene__

#include "Inc.h"
#include "BaseNode.h"
//#include "AnimLoading.h"
//#include "PopupExitGameLayer.h"
//#include "PopupDailyLoginLayer.h"
//#include "PopupTipLayer.h"
//#include "WeixinShareLayer.h"
//#include "ActiveScene.h"
//#include "MiniGameLayer.h"
//#include "GuaGuaMachine.h"
//#include "TigerMachineLayer.h"
//#include "GlobalSchedule.h"
//#include "IconTextSprite.h"


class StartScene : public BaseNode
{
    
public:
    StartScene();
	virtual ~StartScene();
	CREATE_FUNC(StartScene);

public:
    static cocos2d::CCScene* scene();
	virtual bool init();
	void onEnterTransitionDidFinish();
    //virtual void keyBackClicked();
    void onEnter();
    void onExit();
    
public:
    virtual void ctrlBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event);
	virtual void menuCallback(CCObject* pSender){};
	virtual void popupTipLayerCallback(CCNode* pSender);
   
private:
    void initUI();
	void runLoadingAnim();
	void noRunLoadingAnim();
	void stopLoadingAnim();
    
public:
    void showUserInfo();
    
private:
    //void getPlayerInfoWithJson(CCHttpPacket& packet);
    //void getConfigWithJson(CCHttpPacket& packet);
    
private:
    void quickStart();

private:
	void registerOrLogin();
	void registerORloginCallback(float dt);
	void getConfigCallback(float dt);
    
    void dailyLoginCallback(float dt);
    void dailyLoginShowTiggerCallback(float dt);
    
public:
	void updateRichInfo();
    void clickHall(bool isBack);
    
private:
	//PopupExitGameLayer* createPopupExitGameLayer();
	//PopupDailyLoginLayer* createPopupDailyLoginLayer();
	//MiniGameLayer* createMiniGameLayer();
	//GuaGuaMachine* createGuaguaGameLayer();
	//TigerMachineLayer* createTigerMachineLayer();
 //   WeixinShareLayer* createWeixinShareLayer();
	void popLayerExitGameCallback(CCNode* pSender);
	void miniGameCallback(CCNode* pSender);
	void tigerMachineLayerCallback(CCNode* pSender);
	void guaguaMachineLayerCallback(CCNode* pSender);
	void weixinShareLayerCallback(CCNode* pSender);
    void clickCtrlBtnCallback(float dt);
    
public:
    void notifiUpdateRichCallback(CCObject* pCCObjData);
    void notifidailyLoginShowCallback(CCObject* pCCObjData);
    
private:
    CCNode* pContentNode;
    
private:
    float loadingAnimDelay;
    bool isAutoLgoin;
    CCSprite* pUserHeader;
    CCPoint userHeaderPoint;
    CCLabelTTF* pUserName;
 //   AnimLoading* pAnimLoading;
 //   IconTextSprite* pShipIconText;
	//IconTextSprite* pRmbIconText;
    CCNode* pHallNode;
    CCNode* pQuickGameNode;
	bool iskeyBackClicked;
    //在玩迷你游戏的时候，禁止心跳更新
    bool isMiniGameing;
    //限制按钮连续快速点击
    const float safeClickTime;
    bool isClickCtrl;
    //房间列表是否显示
    bool isVenueListShow;
    //迷你游戏(老虎机，刮刮乐。。)是否显示
    bool isTigetMachineShow;
    bool isGuagualeMachineShow;
    bool isWeixinShareShow;

};


#endif




