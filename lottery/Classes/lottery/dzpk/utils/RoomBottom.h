#ifndef __Game__RoomBottom__
#define __Game__RoomBottom__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "RoomOtherPlayer.h"
#include "CusProgressTimer.h"

using namespace cocos2d;
using namespace cocos2d::extension;



class RoomBottom : public cocos2d::Layer
{

public:

    virtual bool init();

	CREATE_FUNC(RoomBottom);

	void updateMyPlayerUI(int seatid);

	void timer_bar_scheduler(float dt);
    
    void checkMyInfo();

	void updateStatus(int status);
    
    void send_player_action(int idx,int bet);

	void tigerMachineCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);

	void icon_cardCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);

	void ctrlBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);

	int getNextPlayerSeatidOfGaming(int seatid);
    
    void hide();
    
    void show();

public:
	//CCProgressTimer *time_bar;

    CusProgressTimer *time_bar;
    
	CCSprite *selected_tag1;

	CCSprite *selected_tag2;

	int sec;
};

#endif