//
//  roomLayer.h
//  Game
//
//  Created by zhouwei on 13-6-17.
//
//

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

#define MASKD_TAG 20
#define MASKDMENU_TAG 21
#define PAIDUI_TAG 22
#define LOTTERY_KIND_SPRITE_TAG 9527

//彩票种类个数 
#define LOTTERY_COUNT 18
#define RANK_COUNT 5

USING_NS_CC;
USING_NS_CC_EXT;

//	大厅层级枚举
enum EHallLayerZOrder
{
	EHALLLayerZOrder_Bg=1,	        //	顶部的颜色部分、九亿娱乐平台图片、顶部滚动图片
	EHALLLayerZOrder_SelectMenu=1,	//	分页按钮
	EHALLLayerZOrder_ButtonMenu=7,	//	底部按钮
	EHALLLayerZOrder_PlayerMenu=5,	//	人物头像 左上角、静音按钮
	EHALLLayerZOrder_RoomList=6,	//	房间列表层
	EHALLLayerZOrder_Dialog,	//	对话框
};

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
    void toRoom(Object* obj);						//
    void toGamble(Object* obj);							//
    void toRecord(Object* obj);
    void toMyLottery(Object* obj);
	void toActive(Object* obj);

	// add by hxh 20161213
	static int getidx(int KindId);
	static int getKindId(int idx);

	void toGame(Object* obj);
	void toLottery(Object* obj);
	void toPhysic(Object* obj);

	void sendHttpRequest();
	//获得网络数据
	void onHttpRequestCompleted(Node *sender, void *data);    
    
    void automaticLogin(bool automatic,bool overResgist = false);				//
    
    void onLoginFinish(Object *obj);							//
    void onLoginFaild(Object* obj);					//
   
    string createAccount();								//
    string createPassword();							//
    void onRegistSucc(Object* obj);					//
	void onRegisterFaild(Object* obj);				//
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
    
    //�Ͻ𿨳�ֵ���
    void onlandcardResult(Object* obj);
    //�绰����ֵ���
    void onPhoneCardResult(Object* obj);
    
    void onLinkResult(Object* obj);

	void getMessageCountRet(Object *obj);
    
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
	void updateRankList(Object* obj);
	void toConnectGameRoomServer(Object* obj);//by hxh
	void toUpdateDownload(Object* obj);//by hxh

	void getSystemTime(float dt);

	void pressKindButton(int tag);
	void resetSelect();//重置按钮

	void getCaiZhongList(Object *obj);

	void refreshLayer();

private:
    MenuLayer  *mMenuLayer;
    Layer *pWarningLayer;
    CustomerServiceLayer *pCustomerService;
	static	int m_nowIndex;
private:
    bool       mMenuCloseOrOpen;
    int       isGoingRoomLayer;

private:
    cocos2d::Size winSize;
    Menu *m_pPlayerMenu;	
    Menu *m_pButtonMenu;	
	Menu* selectMenu;
    
    int m_nGameType;	
    
    PromptBox* m_pRuberBox;
    Layer *pHelpView;
	Layer *pScrollAdd;

	std::string m_strRank;
	int m_iRankCount;

	bool m_bIsShowExitDialog;

	void clear();
};

#endif