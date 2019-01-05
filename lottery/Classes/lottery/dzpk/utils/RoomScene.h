#ifndef __Game__RoomScene__
#define __Game__RoomScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Players.h"
#include "RoomCard.h"
#include "RoomOtherPlayer.h"
#include "RoomBottom.h"
//#include "RoomCardAction.h"
//#include "RoomChip.h"
#include "RoomLight.h"
#include "Inc.h"
//#include "RoomSystemSet.h"
//#include "RoomChat.h"
//#include "RoomFastBuy.h"
//#include "RoomTag.h"
//#include "RoomWarnning.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class RoomPlayer;
class RoomLight;

class RoomScene : public cocos2d::Layer
{

public:
    void onEnter();
    void onExit();
    
	virtual bool init();  

    static cocos2d::CCScene* scene();

    void keyBackSelector(bool var);
    
	//void keyBackClicked();

	CREATE_FUNC(RoomScene);
    
    void returnSelector(bool var);
    void standUpSelector(bool bar);
    void returnBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);
    void chargeBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);
    void returnHallScene(bool var=true);
    
    void newNotice(CCObject* message);
    void removeNotice(CCNode *pSender);
    void listenHome(CCObject* var);
    void listenFore(CCObject* var);
    void reInitRoom();

	void loginGame(bool isReLogin);
    void reLogin();

	void sitDownBefore();

	void initRoom();
    
    void errorFrame();

    void errorFrameCallback(bool var);

	//int handler_cmd(JPacket &jpacket);

    void updateWifiAndTime();
	void updateGame(float dt);
    
    void removeLoading(float dt);

	void setBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);

	void chatBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);

	void onChatCallfunc();
    
    void changeWinerChipNum(CCNode* psender,void* pData);

    
    const char* getStrFromXml(const char* key);
    
    void chipTotalCallBack();
    
    void resumeBtnClicked(float dt);
    
    //void handler_server_login_succ(JPacket &jpacket);
    //void handler_sever_my_info(JPacket jpacket);
    //void handler_server_table_info(JPacket jpacket);
    //void handler_server_sit_down(JPacket jpacket);
    //void handler_server_stand_up(JPacket jpacket);
    //void handler_server_logout_succ(JPacket jpacket);
    //void handler_server_game_start(JPacket jpacket);
    //void handler_server_game_end(JPacket jpacket);
    //void handler_server_my_card(JPacket jpacket);
    //void handler_server_seeCard_first(JPacket jpacket);
    //void handler_server_seeCard_second(JPacket jpacket);
    //void handler_server_seeCard_third(JPacket jpacket);
    //void handler_server_side_pots(JPacket jpacket);
    //void handler_server_next_player(JPacket jpacket);
    //void handler_server_player_actions(JPacket jpacket);
    //void handler_chat_succ(JPacket jpacket);
    //
    //void handler_server_login_error(JPacket jpacket);
    //void handler_server_sit_error(JPacket jpacket);
    //void handler_server_stand_error(JPacket jpacket);
    //void handler_server_logout_error(JPacket jpacket);
    //void handler_server_player_actions_error(JPacket jpacket);
    //void handler_server_quickly_login_succ(JPacket jpacket);
    //void handler_server_quickly_login_error(JPacket jpacket);
    //void handler_update_room_succ(JPacket jpacket);
    //void handler_update_room_error(JPacket jpacket);
    
public:
	RoomBottom *roomBottom;
	RoomOtherPlayer *roomOtherPlayer;
	//RoomSystemSet *roomSystemSet;
	//RoomChat *roomChat;
	//RoomCardAction *roomCardAction;
    //RoomChip *roomChip;
    //RoomFastBuy *roomFastBuy;
    //RoomTag *roomTag;
    //RoomWarnning *roomWarnning;
    RoomLight *roomLight;
    
	static RoomPlayer* getRff();
    static int getMySeatID();
    static void setMySeatID(int id);
    static int getMyGold();
    static void setMyGold(int num);
    static int getBankerId();
    static void setBankerId(int uid);
    static void setIsExit(bool var);
    static bool isExit();
    static void setChatMutex(bool var);
    static bool getChatMutex();
    static bool getBackGround();
    static void setBackGround(bool bar);
    static bool isExistPromote();
    static void setExistPromote(bool var);
    static bool getActionMutex();
    static void setActionMutex(bool var);

public:
    int winer_seatid;
    int pot_num;  //彩池筹码数
    bool mutex; //数据锁
    bool stand_me; //是否是我自身操作站起
    CCPoint originPoint;
    bool is_relogin;
};

#endif