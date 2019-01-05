#ifndef __Game__RoomOtherPlayer__
#define __Game__RoomOtherPlayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CusProgressTimer.h"


using namespace cocos2d;
using namespace cocos2d::extension;

class RoomOtherPlayer : public cocos2d::Layer
{

public:
    
    RoomOtherPlayer();
    virtual ~RoomOtherPlayer();

    virtual bool init();

	CREATE_FUNC(RoomOtherPlayer);

	void updateOtherPlayerUI(int seatid);

	CCPoint getPostBySeatID(int seatid , bool bvar=false);
    
    void ctrlbtn_scheduler(float dt);

	void timer_bar_scheduler(float dt);

	void updateStatus(int status);
    
    void timeWarnning(float dt);
    
    CCSprite *getPlayerAvatar(int seatid);
    
    void updateWarnning(int seatid);
    
    void cardCallBack(float dt);

	int getNextPlayerSeatidOfGaming(int seatid);
    
    void ctrlBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent);
    
    void quicklyEnterRoom(bool bvar);
    void enterOtherRoom();
    
    void sitAction(int seatid);
    
    void showActionTag(CCNode *pSender);
    
    void addWinFrame(int seatid);
    void removeWinStar(CCNode *psender);

public:

	//CCProgressTimer *time_bar;
    CusProgressTimer *time_bar;
	int sec;

	int m_index;
    
    int seat_ac;

private:
    int click_index;
    int my_sound_id;
    
    bool seatAction;
};

#endif