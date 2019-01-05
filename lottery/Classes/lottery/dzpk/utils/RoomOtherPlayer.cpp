#include "RoomOtherPlayer.h"
#include "RoomPlayer.h"
#include "Sound.h"
#include "PlayerModel.h"
#include "Venue.h"
#include "StringFormat.h"
#include "Players.h"
//#include "RoomCardAction.h"
//#include "RoomScene.h"
//#include "RoomInfo.h"
//#include "RoomPopFrame.h"
//#include "RoomOtherInfo.h"
//#include "GlobalHttpReq.h"


RoomOtherPlayer::RoomOtherPlayer():
click_index(0),
my_sound_id(-1),
seatAction(false)
{

}


RoomOtherPlayer::~RoomOtherPlayer()
{
    do{
        CC_BREAK_IF(my_sound_id==-1);
        Sound::getInstance()->stopEffect(my_sound_id);
    }while(0);
}

bool RoomOtherPlayer::init()
{
	 if (!Layer::init())
    {
        return false;
    }
	return true;
		
}


CCPoint RoomOtherPlayer::getPostBySeatID(int id , bool bvar)
{
	int x,y;
   
    CCSize WinSize = CCDirector::sharedDirector()->getVisibleSize();
   
    int X[9] = {
                WinSize.width/2+0,
                WinSize.width/2-173,
                WinSize.width/2-340,
                WinSize.width/2-340,
                WinSize.width/2-173,
                WinSize.width/2+173,
                WinSize.width/2+340,
                WinSize.width/2+340,
                WinSize.width/2+173,
               };
    int Y[9] = {
                WinSize.height/2-110,
                WinSize.height/2-110,
                WinSize.height/2-33,
                WinSize.height/2+107,
                WinSize.height/2+165,
                WinSize.height/2+165,
                WinSize.height/2+107,
                WinSize.height/2-33,
                WinSize.height/2-110,
               };

    int mySeatID =0;//RoomScene::getMySeatID();
    if (bvar)
        mySeatID = -1;
    if (mySeatID!=-1)
	{
        if (mySeatID==id)
        {
            x=X[0];
			y=Y[0];
        }else if ( mySeatID<id )
		{
			x=X[id-mySeatID];
			y=Y[id-mySeatID];
		}else
		{
			x=X[9-mySeatID+id];
			y=Y[9-mySeatID+id];
		}
	}
    else
    {
		x=X[id];
		y=Y[id];
    }

	return ccp(x,y);
}

CCSprite *RoomOtherPlayer::getPlayerAvatar(int seatid)
{
    ////玩家头像
    //const char* genderPicName = RoomScene::getRff()[seatid].sex==1 ? "rooms/photo/room_photo_1.png" : "rooms/photo/room_photo_2.png";
    //std::string headerFile = "no_custom_head";
    //CCHttpAgent::getInstance()->getImagePath(RoomScene::getRff()[seatid].user_id,headerFile);
    //CCSprite* photo = CCSprite::create(genderPicName);
    //
    //CCLog("座位id:%d , uid: %d , headerFile = %s " , seatid , RoomScene::getRff()[seatid].user_id , headerFile.c_str());
    //if (headerFile.compare("no_custom_head")==0 )
    //{
    //    return photo;
    //}else{
    //    do{
    //        CCLOG("headPath=%s",CCHttpAgent::getInstance()->getImagePath(RoomScene::getRff()[seatid].user_id).c_str());
    //        
    //        CCSize sc_sz = photo->getContentSize();
    //        CCSprite *header = CCSprite::create(CCHttpAgent::getInstance()->getImagePath(RoomScene::getRff()[seatid].user_id).c_str());
    //        CC_BREAK_IF(!header);
    //        CCSize tx_sz = header->getContentSize();
    //        header->setScaleX(sc_sz.width/tx_sz.width);
    //        header->setScaleY(sc_sz.height/tx_sz.height);
    //        
    //        return header;
    //    }while(0);
    //}
    return NULL;
}


void RoomOtherPlayer::updateOtherPlayerUI(int seatid)
{
     if (seatAction)
         return;
    
     int begin=seatid;
     int end=seatid+1;
    
     CCLOG("update UI");
     if (seatid==10) //刷新全部ui
     {
         begin=0;
         end=9;
     }
    
	 for (int i=begin;i<end;i++)
	 {
		if (this->getChildByTag(i))
		{
			this->removeChildByTag(i,true);
		}
        if (this->getChildByTag(100+i))
        {
            this->removeChildByTag(100+i,true);
        }
	 }
     
     if (seatid>=0 && seatid<9){ //清除操作警告！
         updateWarnning(seatid); 
     }
 
	//for (int i=begin;i<end;i++)
	//{
 //       if ( RoomScene::getMySeatID()==-1 && RoomScene::getRff()[i].player_status==12) //12表示没人,则播放坐下动画
 //       {
 //           Scale9Sprite* sitBtn_normal = Scale9Sprite::createWithSpriteFrameName("sit_down_frame.png");
 //           Scale9Sprite* sitBtn_selected = Scale9Sprite::createWithSpriteFrameName("sit_down_frame.png");
 //           Scale9Sprite* sitBtn_highLight = Scale9Sprite::createWithSpriteFrameName("sit_down_frame.png");
 //           Scale9Sprite* sitBtn_disabled = Scale9Sprite::createWithSpriteFrameName("sit_down_frame.png");
 //           
 //           sitBtn_highLight->setColor(ccc3(131,131,131));
 //           CCSize sitBtn_size = sitBtn_normal->getContentSize();
 //           ControlButton* sitBtn = ControlButton::create(sitBtn_normal);
 //           sitBtn->setPreferredSize(sitBtn_size);
 //           
 //           sitBtn->setZoomOnTouchDown(false);
 //           sitBtn->setBackgroundSpriteForState(sitBtn_normal,CCControlStateNormal);
 //           sitBtn->setBackgroundSpriteForState(sitBtn_selected,CCControlStateSelected);
 //           sitBtn->setBackgroundSpriteForState(sitBtn_highLight,CCControlStateHighlighted);
 //           sitBtn->setBackgroundSpriteForState(sitBtn_disabled,CCControlStateDisabled);
 //           sitBtn->setAnchorPoint(ccp(0.5f,0.5f));
 //           sitBtn->setPosition(getPostBySeatID(i));
 //           sitBtn->setTag(100+i);
 //           this->addChild(sitBtn,2);
 //           sitBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomOtherPlayer::ctrlBtnCallback), CCControlEventTouchDown);
 //           
 //           CCLabelTTF *status_label = CCLabelTTF::create("","Arial-BoldMT",18);
 //           status_label->setAnchorPoint(ccp(0.5f,0.5f));
 //           status_label->setPosition(ccp(sitBtn_size.width/2,78));
 //           sitBtn->addChild(status_label);
 //           string title = "就坐";
 //           status_label->setString(title.c_str());
 //       }
	//	else if (RoomScene::getRff()[i].player_status!=12 )
	//	{
	//		if ( 13 == RoomScene::getRff()[i].player_status || 3 == RoomScene::getRff()[i].player_status  ) //13等待下一场的玩家,3弃牌
	//		{
 //               //头像框
 //               do{
 //                   Scale9Sprite* player_icon_normal = Scale9Sprite::createWithSpriteFrameName("player_frame_1.png");
 //                   Scale9Sprite* player_icon_selected = Scale9Sprite::createWithSpriteFrameName("player_frame_1.png");
 //                   Scale9Sprite* player_icon_highLight = Scale9Sprite::createWithSpriteFrameName("player_frame_1.png");
 //                   Scale9Sprite* player_icon_disabled = Scale9Sprite::createWithSpriteFrameName("player_frame_1.png");
 //               
 //                   player_icon_normal->setColor(ccc3(131,131,131));
 //                   player_icon_highLight->setColor(ccc3(131,131,131));
 //                   CCSize player_icon_size = player_icon_normal->getContentSize();
 //                   ControlButton* player_icon = ControlButton::create(player_icon_normal);
 //                   CC_BREAK_IF(!player_icon);
 //                   player_icon->setPreferredSize(player_icon_size);
 //               
 //                   player_icon->setZoomOnTouchDown(false);
 //                   player_icon->setBackgroundSpriteForState(player_icon_normal,CCControlStateNormal);
 //                   player_icon->setBackgroundSpriteForState(player_icon_selected,CCControlStateSelected);
 //                   player_icon->setBackgroundSpriteForState(player_icon_highLight,CCControlStateHighlighted);
 //                   player_icon->setBackgroundSpriteForState(player_icon_disabled,CCControlStateDisabled);
 //                   player_icon->setAnchorPoint(ccp(0.5f,0.5f));
 //                   player_icon->setPosition(getPostBySeatID(i));
 //                   player_icon->setTag(i);
 //                   this->addChild(player_icon,2);
 //                   player_icon->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomOtherPlayer::ctrlBtnCallback), CCControlEventTouchDown);

 //                   CCLabelTTF *status_label = CCLabelTTF::create("","Arial-BoldMT",14);
 //                   CC_BREAK_IF(!status_label);
 //                   status_label->setAnchorPoint(ccp(0.5f,0.5f));
 //                   status_label->setPosition(ccp(player_icon_size.width/2,player_icon_size.height-10));
 //                   status_label->setTag(0);
 //                   player_icon->addChild(status_label);
 //                   string title = RoomPlayer::getTitleByStatus(RoomScene::getRff()[i].player_status,RoomScene::getRff()[i].name);
 //                   status_label->setString(title.c_str());
 //               
 //                   CCLabelTTF *chipNum = CCLabelTTF::create(Players::getInstance()->convertToChipNum(RoomScene::getRff()[i].own_gold),"Arial-BoldMT",14);
 //                   CC_BREAK_IF(!chipNum);
 //                   chipNum->setAnchorPoint(ccp(0.5f,0.5f));
 //                   chipNum->setPosition(ccp(player_icon_size.width/2,10));
 //                   chipNum->setTag(1);
 //                   player_icon->addChild(chipNum);
 //               
 //                   CCSprite *photo = getPlayerAvatar(i);
 //                   CC_BREAK_IF(!photo);
 //                   photo->setColor(ccc3(131,131,131));
 //                   photo->setPosition(ccp(player_icon_size.width/2, player_icon_size.height/2));
 //                   player_icon->addChild(photo);
 //               }while (0);
	//		}
	//		else 
	//		{	
 //               do{
 //                   //头像
 //                   Scale9Sprite* player_icon_normal = Scale9Sprite::createWithSpriteFrameName("player_frame_1.png");
 //                   Scale9Sprite* player_icon_selected = Scale9Sprite::createWithSpriteFrameName("player_frame_1.png");
 //                   Scale9Sprite* player_icon_highLight = Scale9Sprite::createWithSpriteFrameName("player_frame_1.png");
 //                   Scale9Sprite* player_icon_disabled = Scale9Sprite::createWithSpriteFrameName("player_frame_1.png");
 //               
 //                   player_icon_highLight->setColor(ccc3(131,131,131));
 //                   CCSize player_icon_size = player_icon_normal->getContentSize();
 //                   ControlButton* player_icon = ControlButton::create(player_icon_normal);
 //                   CC_BREAK_IF(!player_icon);
 //                   player_icon->setPreferredSize(player_icon_size);
 //               
 //                   player_icon->setZoomOnTouchDown(false);
 //                   player_icon->setBackgroundSpriteForState(player_icon_normal,CCControlStateNormal);
 //                   player_icon->setBackgroundSpriteForState(player_icon_selected,CCControlStateSelected);
 //                   player_icon->setBackgroundSpriteForState(player_icon_highLight,CCControlStateHighlighted);
 //                   player_icon->setBackgroundSpriteForState(player_icon_disabled,CCControlStateDisabled);
 //                   player_icon->setAnchorPoint(ccp(0.5f,0.5f));
 //                   player_icon->setPosition(getPostBySeatID(i));
 //                   player_icon->setTag(i);
 //                   this->addChild(player_icon,2);
 //                   player_icon->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomOtherPlayer::ctrlBtnCallback), CCControlEventTouchDown);
 //               
 //                   CCLabelTTF *status_label = CCLabelTTF::create("","Arial-BoldMT",14,CCSizeMake(80, 20) , kCCTextAlignmentCenter);
 //                   CC_BREAK_IF(!status_label);
 //                   status_label->setAnchorPoint(ccp(0.5f,0.5f));
 //                   status_label->setPosition(ccp(player_icon_size.width/2,player_icon_size.height-10));
 //                   status_label->setTag(0);
 //                   player_icon->addChild(status_label);
 //               
 //                   if (RoomScene::getRff()[i].bborsb==0)
 //                   {
 //                       string title = RoomPlayer::getTitleByStatus(RoomScene::getRff()[i].player_status,RoomScene::getRff()[i].name);
 //                       status_label->setString(title.c_str());
 //                   }else if (RoomScene::getRff()[i].bborsb==1){
 //                       status_label->setString("小盲注");
 //                   }else if (RoomScene::getRff()[i].bborsb==2){
 //                       status_label->setString("大盲注");
 //                   }

 //                   CCLabelTTF *chipNum = CCLabelTTF::create(Players::getInstance()->convertToChipNum(RoomScene::getRff()[i].own_gold),"Arial-BoldMT",14);
 //                   CC_BREAK_IF(!chipNum);
 //                   chipNum->setAnchorPoint(ccp(0.5f,0.5f));
 //                   chipNum->setPosition(ccp(player_icon_size.width/2,10));
 //                   chipNum->setTag(1);
 //                   player_icon->addChild(chipNum);
 //               
 //                   //玩家头像
 //                   CCSprite *photo = getPlayerAvatar(i);
 //                   CC_BREAK_IF(!photo);
 //                   photo->setPosition(ccp(player_icon_size.width/2, player_icon_size.height/2));
 //                   player_icon->addChild(photo);
 //               
 //               
 //                   if  ( 10 == RoomScene::getRff()[i].player_status )
 //                   {
 //                       status_label->setString("下注中..");
 //                   
 //                       m_index = i;

 //                       CCSprite *time_sp = CCSprite::createWithSpriteFrameName("player_frame_3.png");
 //                       time_bar = CusProgressTimer::createWith(time_sp , 20.0f);
 //                       CC_BREAK_IF(!time_bar);
 //                       time_bar->setPosition(ccp(player_icon_size.width/2, player_icon_size.height/2));
 //                       float left_time = RoomScene::getRff()[i].left_time-getServerTime()>20? 20:RoomScene::getRff()[i].left_time-getServerTime();
 //                       time_bar->setLeftTime(left_time);
 //                       time_bar->setSelector(this, common_selector(RoomOtherPlayer::updateStatus));
 //                       time_bar->setSelector3(this, common_selector(RoomOtherPlayer::timeWarnning));
 //                       player_icon->addChild(time_bar);
 //                   
 //                       if (i==RoomScene::getMySeatID()){
 //                           time_bar->setSelector2(this, common_selector(RoomOtherPlayer::cardCallBack));
 //                       }
 //                   }
 //               }while(0);
	//		}
	//	}
	//}
 }

void RoomOtherPlayer::ctrlBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
    ControlButton* ctrl_btn = (ControlButton*)sender;
    
    int btn_tag = ctrl_btn->getTag();
    
    //if (PlayerModel::getInstance()->money<RoomInfo::getInstance()->min_take && btn_tag>=100)
    //{
    //    //自身筹码小于最小携带
    //    if (RoomScene::isExistPromote())
    //    {
    //        return;
    //    }
    //    
    //    CCString *str = CCString::createWithFormat("很遗憾，筹码达不到该房间的最小限制，当然您也可以充值获得更多筹码。");
    //    
    //    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    //    RoomPopFrame *popFrame = RoomPopFrame::create();
    //    popFrame->setAnchorPoint(ccp(0,0));
    //    popFrame->setAnchorPoint(ccp(0,0));
    //    scene->addChild(popFrame,6);
    //    
    //    popFrame->createDialog(str->getCString(),"继续旁观","进入房间");
    //    popFrame->setClickOnce(true);
    //    popFrame->setReturnSelector(this, common_selector(RoomOtherPlayer::quicklyEnterRoom));
    //    
    //    return;
    //}
    //
    //if (RoomInfo::getInstance()->max_limit!=-1)
    //{
    //    if (PlayerModel::getInstance()->money>RoomInfo::getInstance()->max_limit && btn_tag>=100)
    //    {
    //        //自身筹码大于最大携带
    //        if (RoomScene::isExistPromote())
    //        {
    //            return;
    //        }
    //        
    //        CCString *str = CCString::createWithFormat("你的筹码已超过该场的最大上限，可以去更高档的桌子比赛了！");
    //        
    //        CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    //        RoomPopFrame *popFrame = RoomPopFrame::create();
    //        popFrame->setAnchorPoint(ccp(0,0));
    //        popFrame->setAnchorPoint(ccp(0,0));
    //        scene->addChild(popFrame,6);
    //        
    //        popFrame->createDialog(str->getCString(),"继续旁观","进入房间");
    //        popFrame->setClickOnce(true);
    //        popFrame->setReturnSelector(this, common_selector(RoomOtherPlayer::quicklyEnterRoom));
    //        
    //        return;
    //    }
    //}
    //
    //
    //if (btn_tag>=100)
    //{
    //    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    //    if (!scene->getChildByTag(10))
    //    {
    //        return;
    //    }
    //    RoomFastBuy *roomFastBuy = (RoomFastBuy*)scene->getChildByTag(10)->getChildByTag(4);
    //    if (roomFastBuy)
    //    {
    //        roomFastBuy->openDialog(btn_tag-100,"携带筹码");
    //    }
    //}else{
    //
    //    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    //    
    //    int index=-1;
    //    for (int i=0;i<50;i++)
    //    {
    //        if (Players::getInstance()[i].seatid==btn_tag)
    //        {
    //            index=i;
    //            break;
    //        }
    //    }
    //    if (index==-1)
    //        return;

    //    RoomOtherInfo *otherPlayerInfo = RoomOtherInfo::create();
    //    otherPlayerInfo->setAnchorPoint(CCPointZero);
    //    otherPlayerInfo->setPosition(CCPointZero);
    //    otherPlayerInfo->initUI(index);
    //    scene->addChild(otherPlayerInfo,5);
    //}
}

void RoomOtherPlayer::ctrlbtn_scheduler(float dt)
{
    ControlButton *player = dynamic_cast<ControlButton*>(getChildByTag(click_index));
    player->setTouchEnabled(true);
}

void RoomOtherPlayer::quicklyEnterRoom(bool bvar)
{
    this->getParent()->unscheduleAllSelectors();
    //RoomScene::setIsExit(true);
    //
    //JPacket jpacket;
    //jpacket.val["cmd"] = CMD_LOGOUT_REQUEST;
    //jpacket.end();
    //CCTcpClient::getInstance()->send_data(jpacket.tostring());
    
    
    for (int i=0;i<50;i++)
    {
        Players::getInstance()[i].init();
    }
    for (int i=0;i<9;i++)
    {
        //RoomScene::getRff()[i].reset();
    }
    
    //RoomInfo::getInstance()->init(); //初始化房间
    
    
    enterOtherRoom();
    
    Sound::getInstance()->playEffect("sound/sound_gangjinru.ogg");
    
}

void RoomOtherPlayer::enterOtherRoom()
{
    std::vector<Venue> allList;
    for (int i=1; i<=5; ++i)
    {
        std::vector<Venue> tempList;
        if (i==1)
        {
            tempList = PlayerModel::getInstance()->venueList_1;
        }
        else if (i==2)
        {
            tempList = PlayerModel::getInstance()->venueList_2;
        }
        else if (i==3)
        {
            tempList = PlayerModel::getInstance()->venueList_3;
        }
        else if (i==4)
        {
            tempList = PlayerModel::getInstance()->venueList_4;
        }
        else if (i==5)
        {
            tempList = PlayerModel::getInstance()->venueList_5;
        }
        for (std::vector<Venue>::iterator iter=tempList.begin(); iter!=tempList.end(); ++iter)
        {
            allList.push_back(*iter);
        }
    }
    
    //用于保存可以进入的桌子id
    std::vector<int> room_id;
    
    room_id.clear();
    
    for (std::vector<Venue>::iterator iter=allList.begin(); iter!=allList.end(); ++iter)
    {
        if (PlayerModel::getInstance()->money>=(*iter).min_money)
        {
            if ((*iter).limit==-1){
                room_id.push_back((*iter).room_id);
            }else if (PlayerModel::getInstance()->money<=(*iter).limit)
            {
                room_id.push_back((*iter).room_id);
            }
        }
    }
    
    if (room_id.empty())
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showToast("筹码不足，没有可进入的房间!");
#else
        //CCMessageBox("筹码不足，没有可进入的房间!","错误提示");
#endif
        return;
    }
    
    //随机一个桌子id
    int index = rand()%room_id.size();
    int table_id = room_id[index];
    //取出房间信息
    for (std::vector<Venue>::iterator iter=allList.begin(); iter!=allList.end(); ++iter)
    {
        if ((*iter).room_id==table_id)
        {
            //RoomInfo::getInstance()->s_bet=(*iter).sblind;
            //RoomInfo::getInstance()->b_bet=(*iter).bblind;
            //RoomInfo::getInstance()->min_take=(*iter).min_money;
            //RoomInfo::getInstance()->max_take=(*iter).max_money;
            //RoomInfo::getInstance()->port = (*iter).port;
            //RoomInfo::getInstance()->ip = (*iter).ip;
            //RoomInfo::getInstance()->max_limit = (*iter).limit;
            //RoomInfo::getInstance()->tid = 0;
            //RoomInfo::getInstance()->rmid = (*iter).showVenueId;
            
            CCArray* vect = CCArray::create();
            StringFormat::Split((*iter).quickRefuelItems.c_str(), ",", vect);
            for (int i=0; i<vect->count(); ++i)
            {
                //RoomInfo::getInstance()->quick_addBet[i] = StringFormat::strToInt(((CCString*)vect->objectAtIndex(i))->getCString());
                //DLog::showLog("-------quick_addBet[%d]: %d",i,RoomInfo::getInstance()->quick_addBet[i]);
            }
            
            break;
        }
    }
    
    CCTextureCache::purgeSharedTextureCache();
    CCSpriteFrameCache::purgeSharedSpriteFrameCache();
    
    //CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, RoomScene::scene(),ccBLACK);
    //CCDirector::sharedDirector()->replaceScene(reScene);

}



void RoomOtherPlayer::timer_bar_scheduler(float dt)
{
     
}

void RoomOtherPlayer::updateStatus(int status)
{
    //RoomScene::getRff()[m_index].player_status = status;
    //RoomScene::getRff()[m_index].bborsb = 0;

}

void RoomOtherPlayer::timeWarnning(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (CCUserDefault::sharedUserDefault()->getBoolForKey(SYSTEM_VOLUME_SNAKE_SWITCH_KEY,false)) {
        setVibrate();
    }
#endif
    //RoomWarnning *roomWarnning = dynamic_cast<RoomWarnning*>(this->getParent()->getChildByTag(7));
    //do{
    //    CC_BREAK_IF(!roomWarnning);
    //    roomWarnning->addWarnning(m_index);
    //    
    //    if(m_index==RoomScene::getMySeatID()){
    //        my_sound_id=Sound::getInstance()->playEffect("sound/sound_daojishi.ogg",true);
    //    }
    //}while (0);
}

void RoomOtherPlayer::updateWarnning(int seatid)
{
    //RoomWarnning *roomWarnning = dynamic_cast<RoomWarnning*>(this->getParent()->getChildByTag(7));
    //do{
    //    CC_BREAK_IF(!roomWarnning);
    //    roomWarnning->removeWarnning(seatid);
    //    
    //    CC_BREAK_IF(my_sound_id==-1);
    //    Sound::getInstance()->stopEffect(my_sound_id);
    //}while (0);
}

void RoomOtherPlayer::cardCallBack(float dt)
{
    //int mySeatID =0;//RoomScene::getMySeatID();
    //RoomCardAction* roomCardAction = dynamic_cast<RoomCardAction*>(this->getParent()->getChildByTag(2));
    //roomCardAction->cardAction(10*mySeatID+1);
    //roomCardAction->cardAction(10*mySeatID+2);
}

int RoomOtherPlayer::getNextPlayerSeatidOfGaming(int seatid)
{
	int nextSeatid;
	int index=0;
	for (int i=0;i<9;i++)
	{
		//if ( RoomScene::getRff()[i].player_status<=11 && RoomScene::getRff()[i].player_status!=3 )
		//{
		//	index++;
		//}
	}
	if ( 1==index )
	{
		return -1;
	}	

	for (int i=seatid+1;i<9;i++)
	{
		//if (RoomScene::getRff()[i].player_status<=11 && RoomScene::getRff()[i].player_status!=3 )
		//{
		//	nextSeatid = i;
		//	return nextSeatid;
		//}
	}
	for (int i=0;i<seatid;i++)
	{
		//if (RoomScene::getRff()[i].player_status<=11 && RoomScene::getRff()[i].player_status!=3 )
		//{
		//	nextSeatid = i;
		//	return nextSeatid;
		//}
		
	}

	return -1;
}

void RoomOtherPlayer::sitAction(int seatid)
{
    int index = seatid;
    
    for (int i=100 ; i<109;i++)
    {
		if (this->getChildByTag(i))
		{
			this->removeChildByTag(i,true);
		}
    }
    
    for (int i=0;i!=9;++i){
        if (getChildByTag(i)){
            ControlButton *palyer = dynamic_cast<ControlButton*>(getChildByTag(i));
            palyer->setTouchEnabled(false);
        }
    }
    
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene->getChildByTag(10))
    {
        return;
    }
    //RoomCardAction *roomCardAction = (RoomCardAction*)scene->getChildByTag(10)->getChildByTag(2);
    //RoomTag *roomTag = (RoomTag*)scene->getChildByTag(10)->getChildByTag(5);
    //RoomChip *roomChip = (RoomChip*)scene->getChildByTag(10)->getChildByTag(3);
    //RoomWarnning *roomWarnning = (RoomWarnning*)scene->getChildByTag(10)->getChildByTag(7);
    //RoomLight *roomLight = (RoomLight*)scene->getChildByTag(10)->getChildByTag(8);
    
    if (index==0)
    {

    }
    else if (index<5)
    {
        seatAction=true;
        
        //if (roomCardAction)
        //{
        //    roomCardAction->hidePlayerCards();
        //}
        //if (roomTag)
        //{
        //    roomTag->hideTag();
        //}
        //if (roomChip)
        //{
        //    roomChip->hideAllChips();
        //}
        //if (roomWarnning)
        //{
        //    roomWarnning->hideWarnning();
        //}
        //if (roomLight) {
        //    roomLight->hideLight();
        //}
        
        int controlNum = index+1;
        for (int i=0;i<9;i++)
        {
            if (this->getChildByTag(i))
            {
                CCPointArray *array = CCPointArray::create(10);
                int seat_id_pos = i;
                for (int j=i;j<i+controlNum;j++)
                {
                    array->addControlPoint(getPostBySeatID(seat_id_pos,true));
                    seat_id_pos--;
                    if (seat_id_pos<0)
                    {
                        seat_id_pos=8;
                    }
                }

                CCCatmullRomTo *action = CCCatmullRomTo::create(1, array);
  
                this->getChildByTag(i)->runAction(action);
                
            }
        }
    }else{
        seatAction=true;
        
        //if (roomCardAction)
        //{
        //    roomCardAction->hidePlayerCards();
        //}
        //if (roomTag)
        //{
        //    roomTag->hideTag();
        //}
        //if (roomChip)
        //{
        //    roomChip->hideAllChips();
        //}
        //if (roomWarnning)
        //{
        //    roomWarnning->hideWarnning();
        //}
        //if (roomLight) {
        //    roomLight->hideLight();
        //}
        
        int controlNum = 10-index;
        for (int i=0;i<9;i++)
        {
            if (this->getChildByTag(i))
            {
                CCPointArray *array = CCPointArray::create(10);
                int seat_id_pos = i;
                for (int j=i;j<i+controlNum;j++)
                {
                    array->addControlPoint(getPostBySeatID(seat_id_pos,true));
                    seat_id_pos++;
                    if (seat_id_pos>8)
                    {
                        seat_id_pos=0;
                    }
                }
                
                CCCatmullRomTo *action = CCCatmullRomTo::create(1, array);
                
                this->getChildByTag(i)->runAction(action);
                
            }
        }
    }
    
    CCCallFuncN *cf = CCCallFuncN::create(this, callfuncN_selector(RoomOtherPlayer::showActionTag));
    this->runAction(CCSequence::create(CCDelayTime::create(1.0f),cf,NULL));
    
    if (!scene->getChildByTag(10))
    {
        return;
    }
    //RoomBottom *roomBottom = (RoomBottom*)scene->getChildByTag(10)->getChildByTag(1);
    //roomBottom->updateMyPlayerUI(seatid);
    
    //坐下音效
    Sound::getInstance()->playEffect("sound/sound_gangjinru.ogg");
}

void RoomOtherPlayer::showActionTag(CCNode *pSender)
{
    for (int i=0; i!=9; ++i) {
        if (getChildByTag(i)){
            ControlButton *palyer = dynamic_cast<ControlButton*>(getChildByTag(i));
            palyer->setTouchEnabled(true);
        }
    }
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene->getChildByTag(10))
    {
        return;
    }
    //RoomCardAction *roomCardAction = (RoomCardAction*)scene->getChildByTag(10)->getChildByTag(2);
    //if (roomCardAction)
    //{
    //    roomCardAction->showPlayerCards();
    //}
    
    if (!scene->getChildByTag(10))
    {
        return;
    }
    //RoomTag *roomTag = (RoomTag*)scene->getChildByTag(10)->getChildByTag(5);
    //if (roomTag)
    //{
    //    roomTag->showTag();
    //}
    
    if (!scene->getChildByTag(10))
    {
        return;
    }
    //RoomChip *roomChip = (RoomChip*)scene->getChildByTag(10)->getChildByTag(3);
    //if (roomChip)
    //{
    //    roomChip->showAllChips();
    //}
    
    if (!scene->getChildByTag(10))
    {
        return;
    }
    //RoomWarnning *roomWarnning = (RoomWarnning*)scene->getChildByTag(10)->getChildByTag(7);
    //if (roomWarnning)
    //{
    //    roomWarnning->showWarnning();
    //}
    
    
    if (!scene->getChildByTag(10))
    {
        return;
    }
    //RoomLight *roomLight = (RoomLight*)scene->getChildByTag(10)->getChildByTag(8);
    //if (roomLight) {
    //    roomLight->showLight();
    //}
    
    if (seatAction){
        seatAction=false;
        updateOtherPlayerUI(10);
    }
}

void RoomOtherPlayer::addWinFrame(int seatid)
{
    do{
        CC_BREAK_IF(!getChildByTag(seatid));
        CC_BREAK_IF(getChildByTag(seatid)->getChildByTag(321));
        CCSprite *frame = CCSprite::createWithSpriteFrameName("room_win_frame.png");
        CC_BREAK_IF(!frame);
        frame->setPosition(ccp(getChildByTag(seatid)->getContentSize().width/2,
                               getChildByTag(seatid)->getContentSize().height/2));
        frame->setTag(321);
        getChildByTag(seatid)->addChild(frame,5);
        
        vector<CCPoint> vecPos;
        vecPos.push_back(ccp(21,3));
        vecPos.push_back(ccp(3,107));
        vecPos.push_back(ccp(65,117));
        vecPos.push_back(ccp(83,13));
        
        float delay=0.5f;
        CCCallFuncN *cf = CCCallFuncN::create(this, callfuncN_selector(RoomOtherPlayer::removeWinStar));
        for (int i=0; i!=4; ++i) {
            CCSprite *star = CCSprite::createWithSpriteFrameName("room_win_star.png");
            star->setPosition(vecPos[i]);
            star->setOpacity(0);
            frame->addChild(star);
            star->runAction(CCSequence::create(CCDelayTime::create(delay),CCFadeIn::create(0.5f),CCFadeOut::create(0.5f),cf,NULL));
            delay+=1.0f;
        }
        
    }while(0);
}

void RoomOtherPlayer::removeWinStar(CCNode *pSender)
{
    do{
        CCSprite *sp = dynamic_cast<CCSprite*>(pSender);
        CC_BREAK_IF(!sp);
        sp->removeFromParentAndCleanup(true);
    }while (0);
}


