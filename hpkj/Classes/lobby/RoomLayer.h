#ifndef __Game__roomLayer__
#define __Game__roomLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "MenuLayer.h"
#include "WarningLayer.h"
#include "BaseLayer.h"
#include "GroupSprite.h"
#include "CustomerServiceLayer.h"
#include "PromptBox.h"
#include "FMGameItemLayer.h"

#include "cocostudio/CocoStudio.h"
#include "ui/UIHelper.h"

#define MASKD_TAG 20
#define MASKDMENU_TAG 21
#define PAIDUI_TAG 22
#define LOTTERY_KIND_SPRITE_TAG 9527

USING_NS_CC;
USING_NS_CC_EXT;

class RoomLayer : public BaseLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static Scene* scene();
     
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(RoomLayer);
    
    virtual void onEnter();
    virtual void onExit();   

    RoomLayer();
    ~RoomLayer();
public:
    //setting menu
    void setting(Object* obj);
    //player infomation menu
    void showPlayerInformation(Object *obj);
    //bottom menu
    void toRoom(Object* obj);
    void toGamble(Object* obj);
    void toRecord(Object* obj);
    void toMyLottery(Object* obj);
	void toGame(Object* obj);
	void toLottery(Object* obj);
	void toPhysic(Object* obj);
	void sendHttpRequest();
	//获得网络数据
	void onHttpRequestCompleted(Node *sender, void *data);
    void automaticLogin(bool automatic,bool overResgist = false);
    void onLoginFinish(Object *obj);
    void onLoginFaild(Object* obj);
    string createAccount();
    string createPassword();
    void onRegistSucc(Object* obj);	
    void onRegisterFaild(Object* obj);
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
    void onlandcardResult(Object* obj);
    void onPhoneCardResult(Object* obj); 
    void onLinkResult(Object* obj);
    void showLoginAward();
    void closeLoginAward();
    void permitButtonClick();
    void forbidButtonClick();
    void showMoveingLabelTips(const char* str);
    void onChange(Object* obj);
    void onPesent(Object* obj);
    void setGoingRoomLayer();
    void gameToBankrupt(bool isBankrupt);
    void showDailyGetCoins(Object* obj);
    void showDailyGetCoinsResult(Object* obj);
    void updateArticle(Object *obj);
	void getSystemTime(float dt);
	void pressKindButton(int tag);
	void resetSelect();//重置按钮
	void refreshLayer();
	void btnCallBack(Ref * pSender, cocos2d::ui::Widget::TouchEventType type,int tag);//按钮回调
private:
    bool       mMenuCloseOrOpen;
    int        isGoingRoomLayer;
    int        m_nGameType;
    cocos2d::Size winSize;
    Menu *m_pPlayerMenu;	
    Menu *m_pButtonMenu;	
	Menu* selectMenu;
    MenuLayer  *mMenuLayer;
    Layer *pWarningLayer;
    CustomerServiceLayer *pCustomerService;
    PromptBox* m_pRuberBox;
    Layer *pHelpView;
	Layer *pScrollAdd;
	ui::Widget*   ScenePlazaRoot;
};

#endif