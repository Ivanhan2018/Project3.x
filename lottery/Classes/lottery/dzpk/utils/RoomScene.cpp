#include "RoomScene.h"
#include "RoomInfo.h"
#include "RoomPopFrame.h"
#include "RoomLoading.h"
#include "RoomPlayer.h"
#include "RoomLight.h"
#include "StartScene.h"
#include "PlayerModel.h"
#include "MessageModel.h"
//#include "MyControlSlider.h"
//#include "RoomBox.h"
//#include "RoomCharge.h"
//#include "RoomChatView.h"


USING_NS_CC;

static RoomPlayer roomPlayers[9];
static int mySeatID=-1;
static int myGold = 0;
static int myUID =0;
static int banker_uid = -1;
static int sb_banker_uid = 0;
static bool is_exit = false;
static bool chat_mutex = false;
static bool is_room_back=false;
static bool has_promote=false;

static bool chatBtn_mutex=false;
static bool actBtn_mutex=false;
static bool isReInitRoom=false;

//const std::string chatTexts[]={
//    "快点吧,你的筹码等着我赢呢",
//    "淡定淡定,想输快点你就全下吧",
//    "厉害啊,我一不小心就赢了好多",
//    "求别针对,我们这局加注加少点",
//    "你是手滑了么？你敢跟注我",
//    "年轻人,图样图森破",
//    "能留个QQ方便交流心得吗",
//    "别逼我发大招,我已忍无可忍",
//    "晕！连续多少把烂牌了",
//    "哼！有种就跟到底啊",
//    "这牌都能赢,没天理啊",
//    "好样的,我们继续合作,能赢！",
//};



CCScene* RoomScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->setTag(1);
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
  
    RoomScene *layer = RoomScene::create();
   
	layer->setTag(10);
    scene->addChild(layer,0);
    
    //loading
    do{
        RoomLoading *roomLoading = RoomLoading::create();
        CC_BREAK_IF(!roomLoading);
        roomLoading->setTag(11);
        roomLoading->setAnchorPoint(ccp(0,0));
        roomLoading->setPosition(ccp(0+origin.x,0+origin.y));
        scene->addChild(roomLoading,300);
    }while (0);
    
    return scene;
}

RoomPlayer* RoomScene::getRff()
{
	return roomPlayers;
}

int RoomScene::getMySeatID()
{
	return mySeatID;
}

void RoomScene::setMySeatID(int id)
{
	mySeatID = id;
    getRff()[id].user_id = PlayerModel::getInstance()->uid;
    CCLOG("设置我的seatid=%d , uid=%d",id,PlayerModel::getInstance()->uid);
    getRff()[id].sex = PlayerModel::getInstance()->sex;
    getRff()[id].name = PlayerModel::getInstance()->name;
}

int RoomScene::getMyGold()
{
	return myGold;
}

void RoomScene::setMyGold(int num)
{
    myGold = num;
}

int RoomScene::getBankerId()
{
	return banker_uid;
}

void RoomScene::setBankerId(int uid)
{
    banker_uid = uid;
}

void RoomScene::setIsExit(bool var)
{
    is_exit = var;
}

bool RoomScene::isExit()
{
    return is_exit;
}

bool RoomScene::getChatMutex()
{
    return chat_mutex;
}

void RoomScene::setChatMutex(bool var)
{
    chat_mutex = var;
}

bool RoomScene::getBackGround()
{
    return is_room_back;
}

void RoomScene::setBackGround(bool var)
{
    is_room_back=var;
}

bool RoomScene::isExistPromote()
{
    return has_promote;
}

void RoomScene::setExistPromote(bool var)
{
    has_promote=var;
}

bool RoomScene::getActionMutex()
{
    return actBtn_mutex;
}

void RoomScene::setActionMutex(bool var)
{
    actBtn_mutex=var;
}


bool RoomScene::init()
{
	if (!Layer::init())
	{
			return false;
	}

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("rooms/room.plist","rooms/room.png");

	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    originPoint = CCDirector::sharedDirector()->getVisibleOrigin();
    
	CCSprite* room_bg = CCSprite::create("rooms/frame/room_background.png");
	room_bg->setAnchorPoint(ccp(0.5f, 0.5f));
	room_bg->setPosition(ccp(winSize.width/2+originPoint.x,winSize.height/2+originPoint.y));
    room_bg->setScaleY(winSize.height/480);
	this->addChild(room_bg,0);
    
    CCSprite *room_desk = CCSprite::create("rooms/frame/roomdesk_1.png");//CCString::createWithFormat("rooms/frame/roomdesk_%s.png",RoomInfo::getInstance()->deskType.c_str())->getCString());
    room_desk->setAnchorPoint(ccp(0.5f, 0.5f));
    room_desk->setPosition(ccp(winSize.width/2+originPoint.x,winSize.height/2+originPoint.y));
    this->addChild(room_desk,0);

    ////房间id 小盲注，大盲注
    //CCLabelTTF *room_info = CCLabelTTF::create(CCString::createWithFormat("房间ID:%d  盲注:%d/%d",RoomInfo::getInstance()->rmid,
    //                                                                      RoomInfo::getInstance()->s_bet,
    //                                                                      RoomInfo::getInstance()->s_bet*2
    //                                                                      )->getCString(), "dezhou", 18);
    //room_info->setAnchorPoint(ccp(0.5f,0.5f));
    //room_info->setPosition(ccp(winSize.width/2+originPoint.x,winSize.height/2-42+originPoint.y));
    //room_info->setColor(ccc3(19, 181, 55));
    //this->addChild(room_info,0);
    
    
    //筹码彩池
    CCSprite *pot_bg = CCSprite::createWithSpriteFrameName("desk_num_frame.png");
    pot_bg->setAnchorPoint(ccp(0.5f,0.5f));
    pot_bg->setPosition(ccp(winSize.width/2+originPoint.x,winSize.height/2+originPoint.y+85));
    pot_bg->setTag(99);
    pot_bg->setVisible(false);
    this->addChild(pot_bg);
   
    CCLabelTTF *pot_chip_cnt = CCLabelTTF::create("$0","Arial-BoldMT",15);
    pot_chip_cnt->setAnchorPoint(ccp(0.5f,0.5f));
    pot_chip_cnt->setPosition(ccp(winSize.width/2+originPoint.x,winSize.height/2+originPoint.y+83));
    pot_chip_cnt->setTag(100);
    pot_chip_cnt->setVisible(false);
    this->addChild(pot_chip_cnt);
    

    //聊天按钮
	Scale9Sprite* chatBtn_normal = Scale9Sprite::createWithSpriteFrameName("chat_btn1.png");
	Scale9Sprite* chatBtn_selected = Scale9Sprite::createWithSpriteFrameName("chat_btn2.png");
	Scale9Sprite* chatBtn_highLight = Scale9Sprite::createWithSpriteFrameName("chat_btn2.png");
	Scale9Sprite* chatBtn_disabled = Scale9Sprite::createWithSpriteFrameName("chat_btn2.png");

    chatBtn_disabled->setColor(ccc3(131,131,131));
	CCSize chatBtn_size = chatBtn_normal->getContentSize();
	ControlButton* chatBtn = ControlButton::create(chatBtn_normal);
	chatBtn->setPreferredSize(chatBtn_size);

	chatBtn->setZoomOnTouchDown(false);
	chatBtn->setBackgroundSpriteForState(chatBtn_normal,Control::State::NORMAL);
	chatBtn->setBackgroundSpriteForState(chatBtn_selected,Control::State::SELECTED);
	chatBtn->setBackgroundSpriteForState(chatBtn_highLight,Control::State::HIGH_LIGHTED);
	chatBtn->setBackgroundSpriteForState(chatBtn_disabled,Control::State::DISABLED);
	chatBtn->setAnchorPoint(ccp(0.5f,0.5f));
	chatBtn->setPosition(ccp(winSize.width-35+originPoint.x,winSize.height-30+originPoint.y));
    chatBtn->setTag(33);
	this->addChild(chatBtn,1);
	chatBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomScene::chatBtnCallback),Control::EventType::TOUCH_DOWN);
    
    Scale9Sprite* chargeBtn_normal = Scale9Sprite::createWithSpriteFrameName("room_charge.png");
	Scale9Sprite* chargeBtn_selected = Scale9Sprite::createWithSpriteFrameName("room_charge1.png");
	Scale9Sprite* chargeBtn_highLight = Scale9Sprite::createWithSpriteFrameName("room_charge1.png");
	Scale9Sprite* chargeBtn_disabled = Scale9Sprite::createWithSpriteFrameName("room_charge1.png");
    
	CCSize chargeBtn_size = chargeBtn_normal->getContentSize();
	ControlButton* chargeBtn = ControlButton::create(chargeBtn_normal);
	chargeBtn->setPreferredSize(chargeBtn_size);
    
	chargeBtn->setZoomOnTouchDown(false);
	chargeBtn->setBackgroundSpriteForState(chargeBtn_normal,Control::State::NORMAL);
	chargeBtn->setBackgroundSpriteForState(chargeBtn_selected,Control::State::SELECTED);
	chargeBtn->setBackgroundSpriteForState(chargeBtn_highLight,Control::State::HIGH_LIGHTED);
	chargeBtn->setBackgroundSpriteForState(chargeBtn_disabled,Control::State::DISABLED);
	chargeBtn->setAnchorPoint(ccp(0.5f,0.5f));
	chargeBtn->setPosition(ccp(winSize.width-95+originPoint.x,winSize.height-30+originPoint.y));
    chargeBtn->setTag(34);
	this->addChild(chargeBtn,1);
    chargeBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomScene::chargeBtnCallback),Control::EventType::TOUCH_DOWN);

    //设置->(站起)
	Scale9Sprite* setBtn_normal = Scale9Sprite::createWithSpriteFrameName("stand_up.png");//("set_open_btn.png");
	Scale9Sprite* setBtn_selected = Scale9Sprite::createWithSpriteFrameName("stand_up1.png");//("set_open_btn.png");
	Scale9Sprite* setBtn_highLight = Scale9Sprite::createWithSpriteFrameName("stand_up1.png");//("set_open_btn.png");
	Scale9Sprite* setBtn_disabled = Scale9Sprite::createWithSpriteFrameName("stand_up1.png");//("set_open_btn.png");

	CCSize setBtn_size = setBtn_normal->getContentSize();
	ControlButton* setBtn = ControlButton::create(setBtn_normal);
	setBtn->setPreferredSize(setBtn_size);

	setBtn->setZoomOnTouchDown(false);
	setBtn->setBackgroundSpriteForState(setBtn_normal,Control::State::NORMAL);
	setBtn->setBackgroundSpriteForState(setBtn_selected,Control::State::SELECTED);
	setBtn->setBackgroundSpriteForState(setBtn_highLight,Control::State::HIGH_LIGHTED);
	setBtn->setBackgroundSpriteForState(setBtn_disabled,Control::State::DISABLED);
	setBtn->setAnchorPoint(ccp(0.5f,0.5f));
	setBtn->setPosition(ccp(originPoint.x+98,winSize.height-30+originPoint.y));
	this->addChild(setBtn,1);
	setBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomScene::setBtnCallback),Control::EventType::TOUCH_DOWN);
 	
    
    //返回
    Scale9Sprite* returnBtn_normal = Scale9Sprite::createWithSpriteFrameName("icon_return.png");
    Scale9Sprite* returnBtn_selected = Scale9Sprite::createWithSpriteFrameName("icon_return1.png");
    Scale9Sprite* returnBtn_highLight = Scale9Sprite::createWithSpriteFrameName("icon_return1.png");
    Scale9Sprite* returnBtn_disabled = Scale9Sprite::createWithSpriteFrameName("icon_return1.png");
    
    CCSize returnBtn_size = setBtn_normal->getContentSize();
    ControlButton* returnBtn = ControlButton::create(returnBtn_normal);
    returnBtn->setPreferredSize(returnBtn_size);
    
    returnBtn->setZoomOnTouchDown(false);
    returnBtn->setBackgroundSpriteForState(returnBtn_normal,Control::State::NORMAL);
    returnBtn->setBackgroundSpriteForState(returnBtn_selected,Control::State::SELECTED);
    returnBtn->setBackgroundSpriteForState(returnBtn_highLight,Control::State::HIGH_LIGHTED);
    returnBtn->setBackgroundSpriteForState(returnBtn_disabled,Control::State::DISABLED);
    returnBtn->setAnchorPoint(ccp(0.5f,0.5f));
    returnBtn->setPosition(ccp(originPoint.x+35,winSize.height-30+originPoint.y));
    returnBtn->setTag(158);
    this->addChild(returnBtn,1);
    returnBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomScene::returnBtnCallback),Control::EventType::TOUCH_DOWN);
    
	initRoom();

    //CCTcpClient::getInstance()->set_host_port(RoomInfo::getInstance()->ip.c_str(), RoomInfo::getInstance()->port);
    ////CCTcpClient::getInstance()->set_host_port("210.14.156.54",7701);
    //CCTcpClient::getInstance()->connect();
	schedule(schedule_selector(RoomScene::updateGame), 0.3f);
    CCLOG("ip:%s,port:%d",RoomInfo::getInstance()->ip.c_str(), RoomInfo::getInstance()->port);
    
    this->setKeypadEnabled(true);
    
    return true; 
}

void RoomScene::returnSelector(bool var)
{
    this->unscheduleAllSelectors();
    RoomScene::setIsExit(true);
    
    errorFrameCallback(true);
    
    //JPacket jpacket;
    //jpacket.val["cmd"] = CMD_LOGOUT_REQUEST;
    //jpacket.end();
    //CCTcpClient::getInstance()->send_data(jpacket.tostring());
}

void RoomScene::standUpSelector(bool bar)
{
    //JPacket jpacket;
    //jpacket.val["cmd"] = CMD_STAND_REQUEST;
    //jpacket.end();
    //CCTcpClient::getInstance()->send_data(jpacket.tostring());
    
    stand_me=true;
}


void RoomScene::returnBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
    if (RoomScene::getMySeatID()<0)
    {
        this->returnSelector(true);
        
        ControlButton *returnBtn = dynamic_cast<ControlButton*>(getChildByTag(158));
        returnBtn->setTouchEnabled(false);
        
        return;
    }
    if (isExistPromote())
    {
        return;
    }
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    
    RoomPopFrame *popFrame = RoomPopFrame::create();
    popFrame->setAnchorPoint(ccp(0,0));
    popFrame->setPosition(ccp(0,0));
    popFrame->setSelector(this, common_selector(RoomScene::returnSelector));
    scene->addChild(popFrame,6);
    
    //string title = "退出当局游戏？";
    //string info = "您当局投入的筹码将不可取回，是否继续？";
    //popFrame->createDialog(title.c_str(),info.c_str());
}

void RoomScene::chargeBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    
    //RoomCharge *charge = RoomCharge::create();
    //charge->setAnchorPoint(ccp(0,0));
    //charge->setPosition(ccp(0+originPoint.x,0+originPoint.y));
    //charge->setTag(88);
    //scene->addChild(charge,5);
}

void RoomScene::returnHallScene(bool var)
{
    this->unscheduleAllSelectors();
    //返回大厅
    RoomScene::setIsExit(true);
    
    //JPacket jpacket;
    //jpacket.val["cmd"] = CMD_LOGOUT_REQUEST;
    //jpacket.end();
    //CCTcpClient::getInstance()->send_data(jpacket.tostring());
    
    errorFrameCallback(false);
}

void RoomScene::newNotice(CCObject* message)
{
    if (getChildByTag(110)){
        removeChildByTag(110, true);
    }
    //接收房间广播
    do{
        CCSprite *msgBk = CCSprite::createWithSpriteFrameName("chat_target_notice.png");
        msgBk->setPosition(ccp(CCDirector::sharedDirector()->getVisibleOrigin().x+
                               CCDirector::sharedDirector()->getVisibleSize().width/2,
                               CCDirector::sharedDirector()->getVisibleOrigin().y+
                               CCDirector::sharedDirector()->getVisibleSize().height+
                               msgBk->getContentSize().height/2
                               ));
        msgBk->setTag(110);
        CC_BREAK_IF(!msgBk);
        addChild(msgBk,100);
    
        CCSprite *icon = CCSprite::createWithSpriteFrameName("chat_notice_icon.png");
        icon->setPosition(ccp(icon->getContentSize().width, msgBk->getContentSize().height/2+5));
        msgBk->addChild(icon);
    
        HronMsgModel *msgStr = static_cast<HronMsgModel*>(message);
        if (msgStr->uid == myUID){
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVNET_ROOM_UPDATE,NULL);
        }
        
        CCLabelTTF *name = CCLabelTTF::create(msgStr->who.c_str(),"dezhou",18);
        name->setAnchorPoint(ccp(0,0.5f));
        name->setPosition(ccp(icon->getContentSize().width*2-10,msgBk->getContentSize().height/2+5));
        CC_BREAK_IF(!name);
        msgBk->addChild(name);
    
        if (msgStr->sex==0){
            name->setColor(msgStr->sysColor);
        }else if (msgStr->sex==1){
            name->setColor(msgStr->manColor);
        }else{
            name->setColor(msgStr->wanColor);
        }
    
        CCLabelTTF *content = CCLabelTTF::create(msgStr->msg.c_str(),"dezhou",18);
        content->setAnchorPoint(ccp(0,0.5f));
        content->setPosition(ccp(icon->getContentSize().width*2+name->getContentSize().width, msgBk->getContentSize().height/2+5));
        CC_BREAK_IF(!content);
        msgBk->addChild(content);
    
        CCCallFuncN *cf = CCCallFuncN::create(this, callfuncN_selector(RoomScene::removeNotice));
    
        msgBk->runAction(CCSequence::create(CCEaseExponentialOut::create(CCMoveBy::create(0.3f, ccp(0,msgBk->getContentSize().height*(-1)))),
                                            CCDelayTime::create(5.0f),
                                            CCEaseExponentialIn::create(CCMoveBy::create(0.3f, ccp(0,msgBk->getContentSize().height))),
                                            cf,
                                            NULL));
    }while (0);
}

void RoomScene::removeNotice(CCNode *pSender)
{
    pSender->removeFromParentAndCleanup(true);
}

void RoomScene::listenHome(CCObject* var)
{
    //updateGame(0.4f);
    if (is_room_back)
    {
        //JPacket jpacket;
        //jpacket.val["cmd"] = CMD_UPDATE_ROOM;
        //jpacket.end();
        //CCTcpClient::getInstance()->send_data(jpacket.tostring());
    }
}

void RoomScene::listenFore(CCObject* var )
{
    isReInitRoom=true;
}

void RoomScene::reInitRoom()
{
    isReInitRoom=false;
    if (getChildByTag(11)) {
        removeChildByTag(11, true);
    }
    
    RoomLoading *roomLoading = RoomLoading::create();
    roomLoading->setTag(11);
    roomLoading->setAnchorPoint(ccp(0,0));
    roomLoading->setPosition(ccp(0+originPoint.x,0+originPoint.y));
    if (getParent())
        getParent()->addChild(roomLoading,300);
    
    //roomChip->init();
    //roomCardAction->init();
    
    mutex=true;
    //mySeatID = -1;
    for (int i=0;i<50;i++)
    {
        Players::getInstance()[i].init();
    }
    for (int i=0;i<9;i++)
    {
        getRff()[i].reset();
    }
    mutex=false;
    //JPacket jpacket;
    //jpacket.val["cmd"] = CMD_TABLE_INFO;
    //jpacket.end();
    //CCTcpClient::getInstance()->send_data(jpacket.tostring());
}

void RoomScene::sitDownBefore()
{
    //CCLOG("初始化桌面");
	if (RoomInfo::getInstance()->round_state>0)
    {

        RoomInfo::getInstance()->isGamming=true;
        //CCLOG("桌面游戏中。。。。");
        
        //roomCardAction->init();
        //roomCardAction->unscheduleAllSelectors();
        
        //mycard
        if (mySeatID!=-1)
        {
            getRff()[mySeatID].card_one = RoomInfo::getInstance()->my_cards[0];
            getRff()[mySeatID].card_two = RoomInfo::getInstance()->my_cards[1];
            CCLOG("我在桌面游戏中。。。手中的牌为:%s , %s" , getRff()[mySeatID].card_one.c_str(),getRff()[mySeatID].card_two.c_str());
        }
        //roomCardAction->initCard(); //cards
        
        //chips
        for (int i=0; i<9;i++)
        {
            if (getRff()[i].seatID!=-1 && getRff()[i].s_bet>0)
            {
                //roomChip->init();
                //roomChip->createChip(i, getRff()[i].s_bet);
                CCLOG("座位id:%d 的玩家已经投注：%d",i ,getRff()[i].s_bet);
            }
        }
        
        //bb or sb
        getRff()[RoomInfo::getInstance()->bb].bborsb = 2;
        getRff()[RoomInfo::getInstance()->sb].bborsb = 1;
        
        ////banker_tag
        //roomTag->init();
        //roomTag->addBankerTag(Players::getSeatidByUid(banker_uid));
        //CCLOG("庄家uid:%d",banker_uid);
        //筹码池
        
        //commCards
        if (RoomInfo::getInstance()->round_state>1)
        {
            for (int i=1;i<RoomInfo::getInstance()->round_state;i++)
            {
                string card[3];
                if (i==1)
                {
                    card[0]=RoomInfo::getInstance()->comm_card[0];
                    card[1]=RoomInfo::getInstance()->comm_card[1];
                    card[2]=RoomInfo::getInstance()->comm_card[2];
                    
                    CCLOG("游戏进入第1轮，桌上第一，二，三张牌：%s , %s , %s" ,card[0].c_str() , card[1].c_str() , card[2].c_str() );
                }else if (i==2)
                {
                    card[0]=RoomInfo::getInstance()->comm_card[3];
                    CCLOG("游戏进入第2轮，桌上第四张牌：%s ," ,card[0].c_str());
                }else if (i==3)
                {
                    card[0]=RoomInfo::getInstance()->comm_card[4];
                    CCLOG("游戏进入第2轮，桌上第五张牌：%s ," ,card[0].c_str());
                }
                //roomCardAction->Charge_card(i,card);
            }
        }
    }
}

void RoomScene::initRoom()  //刚进房间时房间的情况
{
    do{
        roomOtherPlayer = RoomOtherPlayer::create();
        CC_BREAK_IF(!roomOtherPlayer);
        roomOtherPlayer->setTag(0);
        roomOtherPlayer->setAnchorPoint(ccp(0,0));
        roomOtherPlayer->setPosition(ccp(0+originPoint.x,0+originPoint.y));
        this->addChild(roomOtherPlayer,3);

        roomBottom = RoomBottom::create();
        CC_BREAK_IF(!roomBottom);
        roomBottom->setTag(1);
        roomBottom->setAnchorPoint(ccp(0,0));
        roomBottom->setPosition(ccp(0+originPoint.x,0+originPoint.y));
        this->addChild(roomBottom,10);


        //roomCardAction = RoomCardAction::create();
        //CC_BREAK_IF(!roomCardAction);
        //roomCardAction->setTag(2);
        //roomCardAction->setAnchorPoint(ccp(0,0));
        //roomCardAction->setPosition(ccp(0+originPoint.x,0+originPoint.y));
        //this->addChild(roomCardAction,6);

        //roomChip = RoomChip::create();
        //CC_BREAK_IF(!roomChip);
        //roomChip->setTag(3);
        //roomChip->setAnchorPoint(ccp(0,0));
        //roomChip->setPosition(ccp(0+originPoint.x,0+originPoint.y));
        //this->addChild(roomChip,7);
    
        ////roomFastBuy = RoomFastBuy::create();
        ////CC_BREAK_IF(!roomFastBuy);
        ////roomFastBuy->setTag(4);
        ////roomFastBuy->setAnchorPoint(ccp(0.5f,0.5f));
        ////roomFastBuy->setPosition(ccp(0+originPoint.x,0+originPoint.y));
        ////this->addChild(roomFastBuy,9);
    
        //roomTag = RoomTag::create();
        //CC_BREAK_IF(!roomTag);
        //roomTag->setTag(5);
        //roomTag->setAnchorPoint(ccp(0,0));
        //roomTag->setPosition(ccp(0+originPoint.x,0+originPoint.y));
        //this->addChild(roomTag,7);
/*
    RoomBox* box = RoomBox::create();
    box->setTag(6);
    box->setAnchorPoint(ccp(0, 0));
    box->setPosition(ccp(0+originPoint.x,0+originPoint.y));
    this->addChild(box,8);
  */  
        //roomWarnning = RoomWarnning::create();
        //CC_BREAK_IF(!roomWarnning);
        //roomWarnning->setTag(7);
        //roomWarnning->setAnchorPoint(ccp(0, 0));
        //roomWarnning->setPosition(ccp(0+originPoint.x,0+originPoint.y));
        //this->addChild(roomWarnning,2);
    
        roomLight = RoomLight::create();
        CC_BREAK_IF(!roomLight);
        roomLight->setTag(8);
        roomLight->setAnchorPoint(ccp(0, 0));
        roomLight->setPosition(ccp(0+originPoint.x,0+originPoint.y));
        this->addChild(roomLight,2);
    

        //VALUE INIT
        
        mySeatID = -1;
        for (int i=0;i<50;i++)
        {
            Players::getInstance()[i].init();
        }
        for (int i=0;i<9;i++)
        {
            getRff()[i].reset();
        }
        
        myUID = PlayerModel::getInstance()->uid;
        is_exit = false;
        mutex = false;
        chat_mutex = false;
        stand_me = false;
        chatBtn_mutex=false;
        actBtn_mutex=false;
        is_room_back=true;
        isReInitRoom=false;
        has_promote=false;
        is_relogin=false;
        CCLOG("房间ui初始化成功！！！");
        
        ////初始化聊天容器
        //RoomChatMsg::getInstance()->init();
        
    }while (0);
    /*
     Strength *strength = new Strength();
     strength->init();
     
     char hold_c1[] = "Td";
     char hold_c2[] = "6d";
     
     strength->set_hold_card(hold_c1, hold_c2);
     
     char community_c1[] = "3d";
     char community_c2[] = "Ts";
     char community_c3[] = "5d";
     char community_c4[] = "4d";
     char community_c5[] = "7d";
     
     strength->set_card_flop(community_c1, community_c2, community_c3);
     std::cout << strength->get_cards_rank() << '\n';
     
     strength->set_card_rurn(community_c4);
     std::cout << strength->get_cards_rank() << '\n';
     
     strength->set_card_river(community_c5);
     std::cout << strength->get_cards_rank() << '\n';
     */
}


void RoomScene::onEnter()
{
    Layer::onEnter();
    
    Sound::getInstance()->StopBgSound();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(RoomScene::listenHome),
                                                                  EVNET_ROOM_UPDATE,
                                                                  NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(RoomScene::listenFore),
                                                                  EVNET_ROOM_FOREGROUND,
                                                                  NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(RoomScene::newNotice),
                                                                  NOTIFI_SCROLLING_HORN_MSG_KEY,
                                                                  NULL);
    
}

void RoomScene::onExit()
{
    Layer::onExit();
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVNET_ROOM_UPDATE);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVNET_ROOM_FOREGROUND);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTIFI_SCROLLING_HORN_MSG_KEY);

}

void RoomScene::errorFrame()
{
    if (isExistPromote())
    {
        //CCLOG("提示框已经存在");
        return;
    }
    
    this->unscheduleAllSelectors();
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    RoomPopFrame *popFrame = RoomPopFrame::create();
    popFrame->setAnchorPoint(ccp(0,0));
    popFrame->setAnchorPoint(ccp(0,0));
    popFrame->setSelector(this, common_selector(RoomScene::errorFrameCallback));
    scene->addChild(popFrame,6);
    
    CCString *str = CCString::createWithFormat("%s","网络异常已经断开");
    popFrame->createDialog(str->getCString());
}

void RoomScene::reLogin()
{
    unschedule(schedule_selector(RoomScene::updateGame));
    //CCTcpClient::getInstance()->set_host_port(RoomInfo::getInstance()->ip.c_str(), RoomInfo::getInstance()->port);
    //CCTcpClient::getInstance()->connect();
	schedule(schedule_selector(RoomScene::updateGame), 0.3f);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    showToast("您的网络不太好哦，正在拼命帮您连接中。。。");
#else
    //CCMessageBox("您的网络不太好哦，正在拼命帮您连接中。。。","提示");
#endif
}

void RoomScene::errorFrameCallback(bool var)
{
    
    mySeatID = -1;
    for (int i=0;i<50;i++)
    {
        Players::getInstance()[i].init();
    }
    for (int i=0;i<9;i++)
    {
        getRff()[i].reset();
    }
    
    RoomInfo::getInstance()->init(); //初始化房间
    CCLOG("==============我离开房间，房间数据初始化,桌子信息初始化，玩家信息初始化");
    
    //CCSpriteFrameCache::purgeSharedSpriteFrameCache();
    //CCTextureCache::purgeSharedTextureCache();
    
    CCTransitionScene*  reScene;
    if (var)
    {
        reScene= CCTransitionFade::create(1.0f, StartScene::scene(),Color3B::BLACK);
    }else{
        reScene= CCTransitionFade::create(1.0f, StartScene::scene(),Color3B::BLACK);
    }
    CCDirector::sharedDirector()->replaceScene(reScene);
    is_room_back=false;
}


void RoomScene::updateGame(float dt)
{
    //CCTcpResponse *response = CCTcpClient::getInstance()->get();
    //if (response)
    //{
    //    CCLog("Room cmd %d data %s",response->cmd, response->data.c_str());
    //    if (response->cmd == DISCONNECT_RES) //关闭
    //    {
    //        errorFrame();
    //        //is_relogin=true;
    //        //reLogin();
    //    }
    //    else if (response->cmd == CONNECT_OK_RES)
    //    {
    //        loginGame(is_relogin);
    //    }
    //    else if (response->cmd == CONNECT_ERROR_RES)
    //    {
    //        errorFrame();
    //        //is_relogin=true;
    //        //reLogin();
    //    }
    //    else if (response->cmd == RECV_DATA_OK_RES)
    //    {
    //        JPacket jpacket;
    //        if (jpacket.parse(response->data) < 0)
    //        {
    //            
    //        }
    //        else
    //        {
    //            handler_cmd(jpacket);
    //        }
    //    }
    //    CC_SAFE_DELETE(response);
    //} else {
    //    // CCLog("have not message!");
    //}

    //站起状态，屏蔽聊天；反之，恢复聊天
    
    if (getMySeatID()>=0){
        if (getChildByTag(33))
        {
            ControlButton* chatBtn = (ControlButton*)getChildByTag(33);
            chatBtn->setEnabled(true);
        }
    }else{
        if (getChildByTag(33))
        {
            ControlButton* chatBtn = (ControlButton*)getChildByTag(33);
            chatBtn->setEnabled(false);
        }
    }
    

    if (isReInitRoom){
        reInitRoom();
    }
    
    //更新显示wifi和time
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    updateWifiAndTime();
#endif
}

void RoomScene::updateWifiAndTime()
{
    for (int i=225;i<230;i++)
    {
        if (getChildByTag(i))
        {
            removeChildByTag(i, true);
        }
    }

    CCLabelTTF* phoneTime = CCLabelTTF::create(RoomInfo::getInstance()->time.c_str(),"dezhou",21);
    phoneTime->setAnchorPoint(ccp(0,0));
    phoneTime->setPosition(ccp(50+originPoint.x,67+originPoint.y));
    phoneTime->setTag(225);
    this->addChild(phoneTime);
    
    if (RoomInfo::getInstance()->Wifi<=0 || RoomInfo::getInstance()->Wifi>4)
        return;
    
    for (int i=1; i<=RoomInfo::getInstance()->Wifi; i++) {
        CCSprite* sp_wifi = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("room_wifi_%d.png",i)->getCString());
        sp_wifi->setAnchorPoint(ccp(0,0));
        sp_wifi->setPosition(ccp(10+7*(i-1)+originPoint.x,70+originPoint.y));
        sp_wifi->setTag(225+i);
        this->addChild(sp_wifi);
    }
}

void RoomScene::loginGame(bool isReLogin)
{

    //JPacket jpacket;
    //jpacket.val["cmd"] = CMD_LOGIN_ROOM;
    //jpacket.val["uid"] = myUID;
    //jpacket.val["tid"] = is_relogin? RoomInfo::getInstance()->tableid:RoomInfo::getInstance()->tid;
    //jpacket.val["skey"] = PlayerModel::getInstance()->skey;
    //jpacket.end();
    //CCTcpClient::getInstance()->send_data(jpacket.tostring());
    
    //PlayerModel::getInstance()->cur_time_s = getCurrentTime();
}

/*
int RoomScene::handler_cmd(JPacket &jpacket)
{
    if (mutex)
    {
        return 0;
    }
	
    Json::Value &val = jpacket.tojson();
    int cmd = val["cmd"].asInt();
    
    switch (cmd) {
            
        case SERVER_LOGIN_SUCC:
            handler_server_login_succ(jpacket);
            break;
        case SERVER_LOGIN_ERROR:
            handler_server_login_error(jpacket);
            break;
        case SERVER_LOGIN_SUCC_RE:
            handler_server_login_succ(jpacket);
            break;
        case SERVER_LOGIN_ERROR_RE:
            handler_server_login_error(jpacket);
            break;
        case SERVER_TABLE_INFO:
            handler_server_table_info(jpacket);
            break;
        case SERVER_MY_INFO:
            handler_sever_my_info(jpacket);
            break;
        case SERVER_SIT_DOWN:
            handler_server_sit_down(jpacket);
            break;
        case SERVER_SIT_ERROR:
            handler_server_sit_error(jpacket);
            break;
        case SERVER_STAND_UP:
            handler_server_stand_up(jpacket);
            break;
        case SERVER_STAND_ERROR:
            handler_server_stand_error(jpacket);
            break;
        case SERVER_LOGOUT_SUCC:
            handler_server_logout_succ(jpacket);
            break;
        case SERVER_LOGOUT_ERROR:
            handler_server_logout_error(jpacket);
            break;
        case SERVER_GAME_START:
            handler_server_game_start(jpacket);
            break;
        case SERVER_GAME_END:
            handler_server_game_end(jpacket);
            break;
        case SERVER_MY_CARD:
            handler_server_my_card(jpacket);
            break;
        case SERVER_SEE_CARD_FIRST:
            handler_server_seeCard_first(jpacket);
            break;
        case SERVER_SEE_CARD_SECOND:
            handler_server_seeCard_second(jpacket);
            break;
        case SERVER_SEE_CARD_THIRD:
            handler_server_seeCard_third(jpacket);
            break;
        case SERVER_PLAYER_ACTION:
            handler_server_player_actions(jpacket);
            break;
        case SERVER_PLAYER_ACTION_ERR:
            handler_server_player_actions_error(jpacket);
            break;
        case SERVER_SHOW_SIDE_POTS:
            handler_server_side_pots(jpacket);
            break;
        case SERVER_NEXT_BET_PLAYER:
            handler_server_next_player(jpacket);
            break;
        case SEVER_CHAT_SUCC:
            handler_chat_succ(jpacket);
            break;
        case SERVER_LOGIN_AND_SIT_SUCC:
            handler_server_quickly_login_succ(jpacket);
            break;
        case SERVER_LOGIN_AND_SIT_ERR:
            handler_server_quickly_login_error(jpacket);
            break;
        case SERVER_UPDATE_ROOM:
            handler_update_room_succ(jpacket);
            break;
        case SERVER_UPDATE_ROOM_ERR:
            handler_update_room_error(jpacket);
            break;
        default:
            break;
             
    }
    
    return 0;
}
*/

const char* RoomScene::getStrFromXml(const char* key)
{
    CCDictionary* pDictionary = CCDictionary::createWithContentsOfFile("language/room_error.xml");
    return "";//((CCString*)pDictionary->objectForKey(key))->m_sString.c_str();
}


void RoomScene::setBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{

    if (RoomScene::getMySeatID()>=0){
        if (isExistPromote())
        {
            //CCLOG("提示框已经存在");
            return;
        }
        
        CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
        
        RoomPopFrame *popFrame = RoomPopFrame::create();
        popFrame->setAnchorPoint(ccp(0,0));
        popFrame->setAnchorPoint(ccp(0,0));
        scene->addChild(popFrame,6);
        
        //string title = "退出当局游戏？";
        //string info = "您当局投入的筹码将不可取回，是否继续？";
        //popFrame->createDialog(title.c_str(),info.c_str());
        //popFrame->setSelector(this, common_selector(RoomScene::standUpSelector));
    }else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showToast("你已经站着了。");
#endif
    }
   
}

void RoomScene::chatBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
    if (chatBtn_mutex)
        return;
    
    chatBtn_mutex=true;

    onChatCallfunc();
    
    scheduleOnce(schedule_selector(RoomScene::resumeBtnClicked), 2.0f);
}

void RoomScene::resumeBtnClicked(float dt)
{

    chatBtn_mutex=false;
    CCLOG("chatBtn resume!!!");
}

void RoomScene::onChatCallfunc()
{
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
 //   if (scene->getChildByTag(3)) {
 //       RoomChat *roomChat = dynamic_cast<RoomChat*>(scene->getChildByTag(3));
 //       roomChat->open();
 //       return;
 //   }
 //   
	//roomChat = RoomChat::create();
	//roomChat->setPosition(ccp(0+originPoint.x,0+originPoint.y));
	//roomChat->setTag(3);
	//scene->addChild(roomChat,5);
}

void RoomScene::keyBackSelector(bool var)
{
    setIsExit(true);
    
    this->unscheduleAllSelectors();
    //JPacket jpacket;
    //jpacket.val["cmd"] = CMD_LOGOUT_REQUEST;
    //jpacket.end();
    //CCTcpClient::getInstance()->send_data(jpacket.tostring());
    
    errorFrameCallback(true);
}

//void RoomScene::keyBackClicked()
//{
//    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
//    
//    if (scene->getChildByTag(20) || isExistPromote())
//    {
//        CCLog("提示框已经存在");
//        return;
//    }
//    roomFastBuy->close();
//    if (scene->getChildByTag(88)){
//        RoomCharge *charge = dynamic_cast<RoomCharge*>(scene->getChildByTag(88));
//        charge->close();
//    }
//    
//    RoomPopFrame *popFrame = RoomPopFrame::create();
//    popFrame->setAnchorPoint(ccp(0,0));
//    popFrame->setAnchorPoint(ccp(0,0));
//    popFrame->setTag(20);
//    popFrame->setSelector(this, common_selector(RoomScene::keyBackSelector));
//    scene->addChild(popFrame,6);
//    
//    string title = "退出当局游戏？";
//    string info = "您当局投入的筹码将不可取回，是否继续？";
//    popFrame->createDialog(title.c_str(),info.c_str());
//}

/*
void RoomScene::handler_server_login_succ(JPacket &jpackets)
{
    Json::Value &val = jpackets.tojson();
    
    
    //同步服务器时间
    PlayerModel::getInstance()->cur_time = val["timestamp"].asInt();
    if (PlayerModel::getInstance()->cur_time>0){
        setDeltaT(getCurrentTime()-PlayerModel::getInstance()->cur_time);
    }

    CCLog("==========当前设备与服务器时差==========deltaT=%f",getDeltaT());
    
    if (val["uid"].asInt()==myUID || val["cmd"].asInt()==4003)
    {
        //保存桌子id
        RoomInfo::getInstance()->tableid=val["tid"].asInt();
        
        JPacket jpacket;
        jpacket.val["cmd"] = CMD_TABLE_INFO;
        jpacket.end();
        CCTcpClient::getInstance()->send_data(jpacket.tostring());
    }else{
        for (int i=0;i<50;i++) //若该玩家信息已存在，则清除已有的玩家信息
        {
            if (Players::getInstance()[i].uid == val["uid"].asInt())
            {
                Players::getInstance()[i].init();
                break;
            }
        }
        
        int index;
        for (int i=0;i<50;i++)
        {
            if (Players::getInstance()[i].is_player==false) //未保存用户信息
            {
                index = i;
                break;
            }
        }
        
        Players::getInstance()[index].is_player = true;
        Players::getInstance()[index].avatar = val["avatar"].asString();
        string cards = val["best_cards"].asString();
        if (cards.size()==10)
        {
            Players::getInstance()[index].best_cards[0] = CCString::createWithFormat("%c%c",cards.at(0),cards.at(1))->getCString();
            Players::getInstance()[index].best_cards[1] = CCString::createWithFormat("%c%c",cards.at(2),cards.at(3))->getCString();
            Players::getInstance()[index].best_cards[2] = CCString::createWithFormat("%c%c",cards.at(4),cards.at(5))->getCString();
            Players::getInstance()[index].best_cards[3] = CCString::createWithFormat("%c%c",cards.at(6),cards.at(7))->getCString();
            Players::getInstance()[index].best_cards[4] = CCString::createWithFormat("%c%c",cards.at(8),cards.at(9))->getCString();
        }
        Players::getInstance()[index].birthday=val["birthday"].asString();
        Players::getInstance()[index].max_chip=val["max_ship"].asInt();
        Players::getInstance()[index].coin=val["coin"].asInt();
        Players::getInstance()[index].contact=val["contact"].asString();
        Players::getInstance()[index].exp=val["exp"].asInt();
        Players::getInstance()[index].money=val["money"].asInt();
        Players::getInstance()[index].name=val["name"].asString();
        Players::getInstance()[index].pcount=val["pcount"].asInt();
        Players::getInstance()[index].ps=val["ps"].asString();
        Players::getInstance()[index].rmb=val["rmb"].asInt();
        Players::getInstance()[index].seatid=-1;
        Players::getInstance()[index].sex=val["sex"].asInt();
        Players::getInstance()[index].uid=val["uid"].asInt();
        Players::getInstance()[index].total_board=val["total_board"].asInt();
        Players::getInstance()[index].total_win=val["total_win"].asInt();
        Players::getInstance()[index].vlevel=val["vlevel"].asInt();
        Players::getInstance()[index].vtime=val["vtime"].asInt();
        Players::getInstance()[index].zone=val["zone"].asString();

    }
    
    CCLog("登陆成功＝＝＝＝end!");
}
*/
/*
void RoomScene::handler_server_table_info(JPacket jpacket)
{
    CCLog("获得桌子信息");
    
    Json::Value &val = jpacket.tojson();
    
    if (val["cur_players"].asInt()>1)//当前玩家大于1时，说明桌上有玩家在游戏
    {
        RoomInfo::getInstance()->cur_players = val["cur_players"].asInt();
        RoomInfo::getInstance()->round_state = val["round_state"].asInt();
        RoomInfo::getInstance()->dealer = val["dealer"].asInt();
        RoomInfo::getInstance()->bb = val["bb"].asInt();
        RoomInfo::getInstance()->sb = val["sb"].asInt();
        
        for (int i=0;i<2;i++)
        {
            RoomInfo::getInstance()->my_cards[i] = val["holes"][i].asString();
        }
        for (int i=0;i<5;i++)
        {
            if (val["comm"][i].asString()!="0"){
                RoomInfo::getInstance()->comm_card[i] = val["comm"][i].asString();
            }
        }
    }
    
    RoomInfo::getInstance()->roomNum =  val["table_players"].asInt();
    
    CCLog("获得桌子信息==end!!");
}
*/
/*
void RoomScene::handler_sever_my_info(JPacket jpackets)
{
    Json::Value &val = jpackets.tojson();
    for (int i=0;i<50;i++) //若该玩家信息已存在，则清除已有的玩家信息
    {
        if (Players::getInstance()[i].uid == val["uid"].asInt())
        {
            Players::getInstance()[i].init();
            break;
        }
    }
    CCLog("=========接收 uid=%d 的数据!!!玩家 seatid=%d,",val["uid"].asInt(),val["seatid"].asInt());
    int index;
    for (int i=0;i<50;i++)
    {
        if (Players::getInstance()[i].is_player==false) //未保存用户信息
        {
            index = i;
            break;
        }
    }

    Players::getInstance()[index].is_player = true;
    Players::getInstance()[index].avatar = val["avatar"].asString();
    //char cards1[10];
    //sprintf(cards1, "%s" , val["best_cards"].asString().c_str());
    //CCLog("%c%c%c%c%c%c%c%c%c%c",*cards1,*(cards1+1),*(cards1+2),*(cards1+3),*(cards1+4),*(cards1+5),*(cards1+6),*(cards1+7),*(cards1+8),*(cards1+9));
    string cards = val["best_cards"].asString();
    if (cards.size()==10)
    {
        Players::getInstance()[index].best_cards[0] = CCString::createWithFormat("%c%c",cards.at(0),cards.at(1))->getCString();
        Players::getInstance()[index].best_cards[1] = CCString::createWithFormat("%c%c",cards.at(2),cards.at(3))->getCString();
        Players::getInstance()[index].best_cards[2] = CCString::createWithFormat("%c%c",cards.at(4),cards.at(5))->getCString();
        Players::getInstance()[index].best_cards[3] = CCString::createWithFormat("%c%c",cards.at(6),cards.at(7))->getCString();
        Players::getInstance()[index].best_cards[4] = CCString::createWithFormat("%c%c",cards.at(8),cards.at(9))->getCString();
    }
    Players::getInstance()[index].birthday=val["birthday"].asString();
    Players::getInstance()[index].max_chip=val["max_ship"].asInt();
    Players::getInstance()[index].coin=val["coin"].asInt();
    Players::getInstance()[index].contact=val["contact"].asString();
    Players::getInstance()[index].exp=val["exp"].asInt();
    Players::getInstance()[index].money=val["money"].asInt();
    Players::getInstance()[index].name=val["name"].asString();
    Players::getInstance()[index].pcount=val["pcount"].asInt();
    Players::getInstance()[index].ps=val["ps"].asString();
    Players::getInstance()[index].rmb=val["rmb"].asInt();
    Players::getInstance()[index].seatid=val["seatid"].asInt();
    Players::getInstance()[index].sex=val["sex"].asInt();
    Players::getInstance()[index].uid=val["uid"].asInt();
    Players::getInstance()[index].total_board=val["total_board"].asInt();
    Players::getInstance()[index].total_win=val["total_win"].asInt();
    Players::getInstance()[index].vlevel=val["vlevel"].asInt();
    Players::getInstance()[index].vtime=val["vtime"].asInt();
    Players::getInstance()[index].zone=val["zone"].asString();
    
    if (Players::getInstance()[index].seatid!=-1)
    {
        getRff()[val["seatid"].asInt()].user_id = val["uid"].asInt();
        getRff()[val["seatid"].asInt()].s_bet = val["bet"].asInt();
        getRff()[val["seatid"].asInt()].seatID = val["seatid"].asInt();
        getRff()[val["seatid"].asInt()].name = val["name"].asString();
        getRff()[val["seatid"].asInt()].own_gold = val["stake"].asInt();
        getRff()[val["seatid"].asInt()].total_gold = val["money"].asInt();
        getRff()[val["seatid"].asInt()].player_status =  Players::convertToStatus(val["state"].asInt());
        if (getRff()[val["seatid"].asInt()].player_status==0) {
            getRff()[val["seatid"].asInt()].setStatus(11);
        }
        getRff()[val["seatid"].asInt()].sex = val["sex"].asInt();

      
        if (Players::getInstance()[index].seatid==RoomInfo::getInstance()->dealer)
        {
            banker_uid = val["uid"].asInt();
        }
        
        //下载头像
        getOtherHeader(val["uid"].asInt(),val["seatid"].asInt());
    }
    
    //保存我的座位id
    if (Players::getInstance()[index].uid == myUID && Players::getInstance()[index].seatid!=-1 ) 
    {
        mySeatID = Players::getInstance()[index].seatid;
    }

    if (RoomInfo::getInstance()->roomNum==index+1)
    {
        roomBottom->updateMyPlayerUI(mySeatID);
        roomOtherPlayer->updateOtherPlayerUI(10);
        //remove Loading
        scheduleOnce(schedule_selector(RoomScene::removeLoading), 1.5f);
    }
    
    //初始化桌面（已有玩家在游戏中）
    if (RoomInfo::getInstance()->cur_players>1 && RoomInfo::getInstance()->roomNum==index+1)
    {
        sitDownBefore();
    }
    
    CCLog("=========接收uid=%d 数据!!==end!",val["uid"].asInt());
}
*/

void RoomScene::removeLoading(float dt)
{
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    if (scene->getChildByTag(11))
    {
        RoomLoading *roomLoading = dynamic_cast<RoomLoading*>(scene->getChildByTag(11));
        roomLoading->close();
    }
    
    PlayerModel::getInstance()->getBrokeChip();
}
/*
void RoomScene::handler_server_sit_down(JPacket jpacket)
{
    Json::Value &val = jpacket.tojson();
    //下载头像
    getOtherHeader(val["uid"].asInt(),val["seatid"].asInt());
    CCLog("==============uid=%d 的玩家坐下成功===更新玩家头像===",val["uid"].asInt());
    
    for (int i=0; i<50;i++)
    {
        if (Players::getInstance()[i].uid == val["uid"].asInt() )
        {
            Players::getInstance()[i].seatid = val["seatid"].asInt();
            getRff()[val["seatid"].asInt()].reset();
            getRff()[val["seatid"].asInt()].own_gold = val["stake"].asInt();
            getRff()[val["seatid"].asInt()].seatID = val["seatid"].asInt();
            getRff()[val["seatid"].asInt()].total_gold = val["money"].asInt();
            getRff()[val["seatid"].asInt()].user_id = val["uid"].asInt();
            getRff()[val["seatid"].asInt()].name = Players::getInstance()[i].name;
            getRff()[val["seatid"].asInt()].setStatus(13);
            getRff()[val["seatid"].asInt()].sex = Players::getInstance()[i].sex;
            
            if (Players::getInstance()[i].uid==myUID)
            {
                if (RoomInfo::getInstance()->tid==0 and mySeatID==-1){  //自动坐下
                    RoomInfo::getInstance()->tid=-1;
                    setMySeatID(val["seatid"].asInt());
                    roomOtherPlayer->updateOtherPlayerUI(val["seatid"].asInt());
                    roomOtherPlayer->sitAction(val["seatid"].asInt());
                }
                if (mySeatID != val["seatid"].asInt())
                {
                    setMySeatID(val["seatid"].asInt());
                    roomOtherPlayer->updateOtherPlayerUI(val["seatid"].asInt());
                    roomOtherPlayer->sitAction(val["seatid"].asInt());
                }
                
                //开局自己携带筹码
                RoomInfo::getInstance()->stake = val["stake"].asInt();
            }else{
                roomOtherPlayer->updateOtherPlayerUI(val["seatid"].asInt());
            }
            
            break;
        }
    }
    
    CCLog("玩家坐下＝＝＝＝＝end!");
}
*/

/*
void RoomScene::handler_server_stand_up(JPacket jpacket)
{
    Json::Value &val = jpacket.tojson();
    
    int stand_id = mySeatID;  //站起消息前我的座位id
    CCLog("==============uid=%d 的玩家站起=",val["uid"].asInt());
    
    if (val["uid"].asInt()==myUID)
    {
        getRff()[val["seatid"].asInt()].reset();
        mySeatID=-1;
    }else
    {
        int seatid = val["seatid"].asInt();
        getRff()[seatid].reset();
    }
    
    if (!isExit())
    {
        if (val["uid"].asInt()==myUID)//是我站起
        {
            roomBottom->updateMyPlayerUI(mySeatID);
            roomOtherPlayer->updateOtherPlayerUI(10);
            
            //更改roomTag AND roomCard Position and roomChip pos
            if (stand_id!=0)
            {
                roomTag->showTag();
                roomCardAction->showPlayerCards();
                roomWarnning->showWarnning();
                roomLight->showLight();
                
                //更新其他玩家已下筹码位置
                roomChip->hideAllChips();
                roomChip->showAllChips();
            }
            
            PlayerModel::getInstance()->money = val["money"].asInt(); 
 
            //如果是我站起，弹出框判断
            if (val["money"].asInt()>=DzConfig::getInstance()->brokeMoney && !stand_me) //快速购买
            {
                if (val["stake"].asInt()<=RoomInfo::getInstance()->s_bet*2)
                {
                    roomFastBuy->openDialog(stand_id,NULL);
                }
            }//else if (stand_me){
                PlayerModel::getInstance()->getBrokeChip();  //手动站起需监测是否满足破产条件
            //}
            stand_me=false;
    
        }else{
            roomOtherPlayer->updateOtherPlayerUI(val["seatid"].asInt());
        }
    }
    
    //去除bankerTag
    if (val["uid"].asInt()==banker_uid)
    {
        roomTag->bankerTagRemove(val["seatid"].asInt());
    }
    
    //删除站起玩家的牌
    for (int i=1;i<5;i++)
    {
        if (roomCardAction->getChildByTag(10*val["seatid"].asInt()+i))
        {
            roomCardAction->removeChildByTag(10*val["seatid"].asInt()+i);
        }
    }
    
    CCLog("玩家站起======end!");
}
*/

/*
void RoomScene::handler_server_logout_succ(JPacket jpacket)
{
    
    Json::Value &val = jpacket.tojson();
    
    CCLog("==============uid=%d 的玩家离开房间=",val["uid"].asInt());
    if (val["uid"].asInt()==myUID)
    {
        this->unscheduleAllSelectors();
        mutex = true;
    }
    CCLog("玩家离开房间====end!");
}
*/

/*
void RoomScene::handler_server_game_start(JPacket jpacket)
{
    CCLog("游戏开始＝＝开始发牌");
    RoomInfo::getInstance()->isGamming=true;
    
    //pot init
    CCLabelTTF *chipTotalNum = (CCLabelTTF*)this->getChildByTag(100);
    chipTotalNum->setString("$0");
    
    //游戏开始，所有坐下的玩家初始化
    for (int k=0;k<9;k++)
    {
        if (getRff()[k].seatID!=-1)
        {
            getRff()[k].player_status = 11;
        }
    }
    
    //card init
    roomCardAction->init();
    
    //chip init
    roomChip->init();
    
	Json::Value &val = jpacket.tojson();

    int bb_uid,bb_bet,bb_money;
    int sb_uid,sb_bet,sb_money;
    
    for (int i=0;i<3;i++)
    {
        if (i==0)
        {
            bb_uid = val["big"][i].asInt();
            sb_uid = val["small"][i].asInt();
        }
        else if (i==1)
        {
            bb_bet = val["big"][i].asInt();
            sb_bet = val["small"][i].asInt();
        }
        else if (i==2)
        {
            bb_money = val["big"][i].asInt();
            sb_money = val["small"][i].asInt();
        }
    }
    
    //大盲注uid
    sb_banker_uid = sb_uid;
    RoomInfo::getInstance()->bb = Players::getSeatidByUid(bb_uid);
    RoomInfo::getInstance()->sb =  Players::getSeatidByUid(sb_uid);
    getRff()[RoomInfo::getInstance()->bb].bborsb=2; //大盲注
    getRff()[RoomInfo::getInstance()->sb].bborsb=1; //小盲注
    
    //游戏开始，所有坐下的玩家初始化
    for (int k=0;k<9;k++)
    {
        if (getRff()[k].seatID!=-1)
        {
            getRff()[k].player_status = 11;
        }
    }

    //房间小盲注下注数量
    //RoomInfo::getInstance()->s_bet = sb_bet;
    
    //小盲注 大盲注下注
    //roomChip->createChip(b_seatid, bb_bet);
    //roomChip->createChip(s_seatid, sb_bet);
    roomChip->sendBet(RoomInfo::getInstance()->bb, bb_bet);
    roomChip->sendBet(RoomInfo::getInstance()->sb, sb_bet);
    
    //刷新大盲注小盲注筹码
    getRff()[RoomInfo::getInstance()->bb].own_gold = bb_money;
    getRff()[RoomInfo::getInstance()->sb].own_gold = sb_money;
  

    //回合数，自动操作初始化
    if (mySeatID!=-1)
    {
        getRff()[mySeatID].current_round =1;
        getRff()[mySeatID].auto_cmd = 0;
    }
    
    //庄家
    banker_uid = val["dealer"].asInt();
    getRff()[Players::getSeatidByUid(banker_uid)].is_banker=true;

    //庄家标记
    roomTag->addBankerTag(Players::getSeatidByUid(banker_uid));
    
    
    //刷新全部玩家ui及头像
    roomOtherPlayer->updateOtherPlayerUI(10);
    
    //roomCardAction 如果我没有坐下，直接发牌
    if (mySeatID==-1)
    {
        roomCardAction->initCard();   
    }
 
    CCLog("游戏开始＝＝end");
}
*/

/*
void RoomScene::handler_server_game_end(JPacket jpacket)
{
    CCLog("＝＝游戏结束＝＝");
    RoomInfo::getInstance()->isGamming=false;

    roomLight->reset();//灯初始化
    
    //玩家比牌，胜利动画，筹码回收
    Json::Value &val = jpacket.tojson();
    
    bool my_fail=true;
    float delay=0;
    int winerNum = val["winlist"].size();
    int winerPot;
    for (int i=0;i<winerNum;i++)
    {
        winerPot = val["winlist"][i]["winers"].size();
        
        for (int j=0;j<winerPot;j++)
        {
            //赢者座位id
            winer_seatid= val["winlist"][i]["winers"][j]["seatid"].asInt();
            //彩池剩余筹码数
            //pot_num -= val["winlist"][i]["winers"][j]["stake"].asInt()-getRff()[winer_seatid].own_gold;
            pot_num -= val["winlist"][i]["pot"].asInt();
            
            //getRff()[winer_seatid].own_gold = val["winlist"][i]["winers"][j]["stake"].asInt();
            getRff()[winer_seatid].user_id = val["winlist"][i]["winers"][j]["uid"].asInt();
            //胜利玩家操作中状态初始化
            getRff()[winer_seatid].player_status=11;
           
            //筹码回收,发放给胜利玩家
            CCCallFuncND *callback = CCCallFuncND::create(this, callfuncND_selector(RoomScene::callbackChip),(void*)(pot_num*10+winer_seatid));
            runAction(CCSequence::create(CCDelayTime::create(delay+1.0f),callback,NULL));
            
            
            //显示赢家牌,保证相同的筹码池
            CCCallFuncND *cfs = CCCallFuncND::create(this, callfuncND_selector(RoomScene::showWinerCards),(void*)(10*i+winer_seatid));
            runAction(CCSequence::create(CCDelayTime::create(delay),cfs,NULL));
        
            
            //刷新赢家信息
            if (winer_seatid==mySeatID)
            {
                my_fail = false;
                roomBottom->updateMyPlayerUI(mySeatID);
                
                roomCardAction->stopMyCardAction();  //终止手中牌的震动动画
            }
            roomOtherPlayer->updateOtherPlayerUI(winer_seatid);
            
            CCCallFuncND *cf = CCCallFuncND::create(this, callfuncND_selector(RoomScene::changeWinerChipNum),(void*)(val["winlist"][i]["winers"][j]["stake"].asInt()*10+winer_seatid));
            runAction(CCSequence::create(CCDelayTime::create(delay+2.0f),cf,NULL));
            
            //胜利动画
            roomTag->winAnimate(winer_seatid);
            roomWarnning->winAnimate(winer_seatid);
            roomOtherPlayer->addWinFrame(winer_seatid);
        }
        delay += 2.0;
    }
    
    
    if (my_fail){ //如果我失败了，播放失败音乐
        Sound::getInstance()->playEffect("sound/sound_shibai.ogg");
    }else{  //胜利音乐
        Sound::getInstance()->playEffect("sound/sound_guzhang.ogg");
    }
    
    //如果比牌(winer_cards>0)
    
    int winCards_id;
    int winer_list;
    winer_list=val["winers"].size();
    if (winer_list>0)
    {
        for (int i=0; i<winer_list ; i++) {
            winCards_id = val["winers"][i]["seatid"].asInt();
            if (winCards_id==-1)
                return;
            
            getRff()[winCards_id].user_id = val["winers"][i]["uid"].asInt();
            int index1=0;
            getRff()[winCards_id].card_one = val["winers"][i]["holecards"][index1].asString();
            getRff()[winCards_id].card_two = val["winers"][i]["holecards"][index1+1].asString();
            getRff()[winCards_id].winCardType = val["winers"][i]["cardtype"].asInt();
            
            for (int j=0;j<5;j++)
            {
                getRff()[winCards_id].winCards[j] =  val["winers"][i]["wincards"][j].asString();
            }
            
            CCLog("该玩家的uid：%d ， 最好的五张牌：%s , %s , %s , %s , %s ， 最佳牌型：%d" , val["winers"][i]["uid"].asInt(),
                                                                                      getRff()[winCards_id].winCards[0].c_str(),
                                                                                      getRff()[winCards_id].winCards[1].c_str(),
                                                                                      getRff()[winCards_id].winCards[2].c_str(),
                                                                                      getRff()[winCards_id].winCards[3].c_str(),
                                                                                      getRff()[winCards_id].winCards[4].c_str(),
                                                                                      getRff()[winCards_id].winCardType
                  );
            //显示赢家牌型
            if (roomOtherPlayer->getChildByTag(winCards_id)) {
                do{
                    CC_BREAK_IF(!roomOtherPlayer->getChildByTag(winCards_id));
                    CCLabelTTF *cardType = (CCLabelTTF*)roomOtherPlayer->getChildByTag(winCards_id)->getChildByTag(0);
                    CC_BREAK_IF(!cardType);
                    cardType->setString(RoomPlayer::getCardType(getRff()[winCards_id].winCardType).c_str());
                }while(0);
            }
            
            //翻开赢家牌
            if (winCards_id!=getMySeatID())
            {
                roomCardAction->openWinerCards(winCards_id);
            }
            
            //去除bankerTag
            if (val["winers"][i]["uid"].asInt()==banker_uid)
            {
                roomTag->bankerTagRemove(winCards_id);
            }
            
        }
    }
    
    int loser_list=val["loses"].size() ;
    int loser_id;
    
    if (loser_list>0){
        for (int i=0;i<loser_list;i++){
            loser_id = val["loses"][i]["seatid"].asInt();
            getRff()[loser_id].user_id = val["loses"][i]["uid"].asInt();
            
            int index2=0;
            getRff()[loser_id].card_one = val["loses"][i]["holecards"][index2].asString();
            getRff()[loser_id].card_two = val["loses"][i]["holecards"][index2+1].asString();
            getRff()[loser_id].winCardType = val["loses"][i]["cardtype"].asInt();

            
            //显示输家牌型
            if (roomOtherPlayer->getChildByTag(loser_id)) {
                do{
                    CC_BREAK_IF(!roomOtherPlayer->getChildByTag(loser_id));
                    CCLabelTTF *cardType = (CCLabelTTF*)roomOtherPlayer->getChildByTag(loser_id)->getChildByTag(0);
                    CC_BREAK_IF(!cardType);
                    cardType->setString(RoomPlayer::getCardType(getRff()[loser_id].winCardType).c_str());
                }while(0);
            }
            
            //翻开输家牌
            if (loser_id!=getMySeatID())
            {
                roomCardAction->openWinerCards(loser_id);
            }
            
            //去除bankerTag
            if (val["loses"][i]["uid"].asInt()==banker_uid)
            {
                roomTag->bankerTagRemove(loser_id);
            }
        }
    }
    
    //更新桌上的玩家信息
    int userSize = val["users"].size();
    if (userSize!=0){
        for (unsigned int i=0;i!=userSize;++i){
            for (int j=0;j!=50;++j)//更新玩家信息
            {
                if (Players::getInstance()[j].uid==val["users"][i]["uid"].asInt())
                {
                    Players::getInstance()[j].exp=val["users"][i]["exp"].asInt();
                    Players::getInstance()[j].money = val["users"][i]["money"].asInt();
                    Players::getInstance()[j].seatid = val["users"][i]["seatid"].asInt();
            
                    if (Players::getInstance()[j].seatid!=-1)
                        getRff()[Players::getInstance()[j].seatid].own_gold = val["users"][i]["stake"].asInt();
                    break;
                }
            }
            //更新自己的钱,和开局携带的筹码
            if (PlayerModel::getInstance()->uid==val["users"][i]["uid"].asInt()){
                PlayerModel::getInstance()->money = val["users"][i]["money"].asInt();
                RoomInfo::getInstance()->stake = val["users"][i]["stake"].asInt();
                
                CCLog("我的筹码：%ld，我携带的筹码：%d",PlayerModel::getInstance()->money,RoomInfo::getInstance()->stake);
                PlayerModel::getInstance()->getBrokeChip();
            }
        }
    }
    
    CCLog("游戏结束＝＝＝end!");
}
*/

void RoomScene::changeWinerChipNum(CCNode* psender,void* pData)
{
    int tag = (int)pData;
    int seatid = tag%10;
    getRff()[seatid].own_gold = tag/10;
    
    if (roomOtherPlayer->getChildByTag(seatid)&&roomOtherPlayer->getChildByTag(seatid)->getChildByTag(1))
    {
        CCLabelTTF *winerStake = (CCLabelTTF*)roomOtherPlayer->getChildByTag(seatid)->getChildByTag(1);
        winerStake->setString(Players::getInstance()->convertToChipNum(getRff()[seatid].own_gold));
    }
 
}

/*
void RoomScene::handler_server_my_card(JPacket jpacket)
{
    
  	Json::Value &val = jpacket.tojson();
    CCLog("＝=====＝＝＝＝系统给我发牌＝＝＝＝=======");
    if (mySeatID==-1) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showToast("获取房间信息失败！");
#else
        CCMessageBox("获取房间信息失败！","错误提示");
#endif
        errorFrameCallback(false);
        return;
    }
    
    for (int i=0;i<2;i++)
    {
        if (i==0)
            getRff()[mySeatID].card_one = val["cards"][i].asString();
        else 
            getRff()[mySeatID].card_two = val["cards"][i].asString();
    }
    
    roomCardAction->initCard();
    
    CCLog("我手中的牌--end：%s , %s",getRff()[mySeatID].card_one.c_str() , getRff()[mySeatID].card_two.c_str());
}
*/

/*
void RoomScene::handler_server_seeCard_first(JPacket jpacket)
{
    CCLog("handler_server_seeCard_first（第一次翻公共牌）");
    Json::Value &val = jpacket.tojson();
    
    string cards[3];
    for (int i=0;i<3;i++)
    {
        cards[i]= val["cards"][i].asString();
        RoomInfo::getInstance()->comm_card[i]=val["cards"][i].asString();
    }
    
    roomCardAction->Charge_card(1 ,cards);
    CCLog("第一次公共牌为--end：%s ， %s ， %s",cards[0].c_str(),cards[1].c_str(),cards[2].c_str());
}
*/

/*
void RoomScene::handler_server_seeCard_second(JPacket jpacket)
{
    CCLog("handler_server_seeCard_second（第二次翻公共牌）");
    Json::Value &val = jpacket.tojson();
    
    string cards[1];

    cards[0]= val["card"].asString();
    RoomInfo::getInstance()->comm_card[3]=val["card"].asString();
    
    roomCardAction->Charge_card(2 ,cards);
    CCLog("第二次公共牌为--end：%s ",cards[0].c_str());
}
*/

/*
void RoomScene::handler_server_seeCard_third(JPacket jpacket)
{
    CCLog("handler_server_seeCard_third（第三次翻公共牌）");
    Json::Value &val = jpacket.tojson();
    
    string cards[1];
    
    cards[0]= val["card"].asString();
    RoomInfo::getInstance()->comm_card[4]=val["card"].asString();
    
    roomCardAction->Charge_card(3 ,cards);
    CCLog("第三次公共牌为---end：%s ",cards[0].c_str());
}
*/

/*
void RoomScene::handler_server_player_actions(JPacket jpacket)
{
    Json::Value &val = jpacket.tojson();
    CCLog("玩家成功操作 ， uid为：%d ，seatid为：%d",val["uid"].asInt(),val["seatid"].asInt());
    int seat_id = val["seatid"].asInt();
    int last_status = getRff()[seat_id].player_status;
    getRff()[seat_id].seatID = seat_id;
    getRff()[seat_id].user_id = val["uid"].asInt();
    getRff()[seat_id].own_gold = val["stake"].asInt();
    getRff()[seat_id].bet = val["bet"].asInt();
    getRff()[seat_id].player_status = Players::convertToStatus(val["state"].asInt());
    getRff()[seat_id].bborsb=0; //去除盲注title
    
    //actionBtn open mutex
    setActionMutex(false);
    
    if (seat_id==mySeatID && last_status==10)
    {
        getRff()[mySeatID].auto_cmd = 0;     //自动操作刷新
        roomCardAction->stopMyCardAction();  //终止手中牌的震动动画
        roomBottom->updateMyPlayerUI(seat_id);
        roomOtherPlayer->updateOtherPlayerUI(seat_id);
        if (getRff()[seat_id].player_status==3) //弃牌，则播放弃牌动画
        {
            roomCardAction->drawAnimate(seat_id);
            
            Sound::getInstance()->playEffect("sound/sound_qipai.ogg");
        }
        CCLog("玩家成功操作 end!");
        return;
    }else if (seat_id==mySeatID){
        //CCLog("===========----1----==============");
        //if (roomOtherPlayer->getChildByTag(seat_id)&&roomOtherPlayer->getChildByTag(seat_id)->getChildByTag(1))
        //{
        //    CCLabelTTF *winerStake = (CCLabelTTF*)roomOtherPlayer->getChildByTag(seat_id)->getChildByTag(1);
        //    winerStake->setString(CCString::createWithFormat("%d",getRff()[seat_id].own_gold)->getCString());
        //}
        CCLog("玩家成功操作 end!");
        return;
    }
    
    if (getRff()[seat_id].player_status==3) //弃牌，则播放弃牌动画
    {
        roomCardAction->drawAnimate(seat_id);
        Sound::getInstance()->playEffect("sound/sound_qipai.ogg");
    }else if (getRff()[seat_id].player_status==1){ //看牌
        Sound::getInstance()->playEffect("sound/sound_kanpai.ogg");
    }else if (getRff()[seat_id].player_status==5){ //全押
        Sound::getInstance()->playEffect("sound/sound_quanya.ogg");
    }
    
    //筹码动画－－－－－－－－－－－－－－－－－－－－－－－－－加注，跟注
    if (val["ac"].asInt()==2001 || val["ac"].asInt()==2002 )
    {
        roomChip->sendBet(seat_id,getRff()[seat_id].bet);
    }
    roomOtherPlayer->updateOtherPlayerUI(seat_id);
    
    CCLog("玩家成功操作 end!");
}
*/

/*
void RoomScene::handler_server_side_pots(JPacket jpacket)
{
    Json::Value &val = jpacket.tojson();
    //---筹码集合在总彩池(彩池信息)
    
    pot_num=0;
    int index = val["pots"].size();
    for (int i=0;i<index;i++)
    {
        pot_num += val["pots"][i]["pot"].asInt();
    }
    
    CCLog("=======================将筹码回收的筹码池：：回收总筹码：＝%d,清空每个人的已下的筹码数",pot_num);
    //每个人已经下的注清0
    for (int i=0; i!=9; ++i) {
        getRff()[i].bet = 0;
    }
    
    if (pot_num>0)
    {
        //scheduleOnce(schedule_selector(RoomScene::chipTotalCallBack), 0.5f);
        roomChip->sendTotalTip(pot_num);
    }
    CCLog("=========将筹码回收end！");
    
    //显示没个玩家名称
    for (int i=0;i!=9;++i){
        if (roomOtherPlayer->getChildByTag(i)) {
            do{
                CC_BREAK_IF(!roomOtherPlayer->getChildByTag(i));
                CCLabelTTF *name = (CCLabelTTF*)roomOtherPlayer->getChildByTag(i)->getChildByTag(0);
                CC_BREAK_IF(!name);
                name->setString(getRff()[i].name.c_str());
            }while(0);
        }
    }
}
*/


void RoomScene::chipTotalCallBack()
{
    
}

/*
void RoomScene::handler_server_next_player(JPacket jpacket)
{
    Json::Value &val = jpacket.tojson();
    
    CCLog("当前操作玩家uid = %d ， 操作时间 ：%.2f s",val["uid"].asInt(),val["remain"].asInt()-getServerTime());
    int ac_seatid = val["seatid"].asInt();
    getRff()[ac_seatid].seatID = ac_seatid;
    getRff()[ac_seatid].user_id = val["uid"].asInt();
    getRff()[ac_seatid].player_status = 10;
    getRff()[ac_seatid].min_bet = val["min_bet"].asInt()-val["bet"].asInt();
    getRff()[ac_seatid].bet = val["bet"].asInt();
    getRff()[ac_seatid].min_raise = val["min_raise"].asInt();
    getRff()[ac_seatid].own_gold = val["stake"].asInt();
    getRff()[ac_seatid].player_status = Players::convertToStatus(val["state"].asInt());
    getRff()[ac_seatid].left_time = val["remain"].asInt();
    
    if (val["uid"].asInt()==sb_banker_uid || RoomInfo::getInstance()->sb==ac_seatid)
    {
        if (mySeatID!=-1)
        {
            getRff()[mySeatID].current_round +=1; 
        }
    }
    
    //轮到我操作播放音效
    if (val["uid"].asInt()==PlayerModel::getInstance()->uid){
        Sound::getInstance()->playEffect("sound/sound_chupai.ogg");
    }
    
    roomLight->LightAnimate(ac_seatid);

    roomBottom->updateMyPlayerUI(mySeatID);
    roomOtherPlayer->updateOtherPlayerUI(ac_seatid);
    
    CCLog("=========当前操作玩家 end！");
}
*/

/*
void RoomScene::handler_chat_succ(JPacket jpacket)
{
    Json::Value &val = jpacket.tojson();
    
    CCLog("=====聊天成功=该玩家座位id=%d====",val["seatid"].asInt());
    if (val["seatid"].asInt()==-1)
    {

    }else{
        if (val["tag"].asInt()==1)//快速文本
        {   
            roomTag->playerChatText(val["seatid"].asInt(),chatTexts[val["str"].asInt()]);
            
        }else if (val["tag"].asInt()==2)//表情
        {
            string str = val["str"].asString();
            roomTag->playerChatFace(val["seatid"].asInt(), str.c_str());
        }else //普通文本
        {
            roomTag->playerChatText(val["seatid"].asInt(),val["str"].asString());
            
            //通知chatview 有新消息
            RoomChatMsg::getInstance()->pushMsg(Players::getInstance()->getNameByUid(val["uid"].asInt()),
                                                val["str"].asString(), val["uid"].asInt(), val["seatid"].asInt());
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ROOMCHAT_UPTATE_VIEW",NULL);
        }
    }
    CCLog("=====聊天成功 end");
}
*/

/*
void RoomScene::handler_server_quickly_login_succ(JPacket jpacket)
{
    
    // "tid" : 1
    CCLog("=====快速进入房间成功=====");
    Json::Value &val = jpacket.tojson();
    
    RoomInfo::getInstance()->tableid = val["tid"].asInt();
    
    //同步服务器时间
    PlayerModel::getInstance()->cur_time = val["timestamp"].asInt();
    if (PlayerModel::getInstance()->cur_time>0){
        setDeltaT(getCurrentTime()-PlayerModel::getInstance()->cur_time);
    }

    CCLog("=========当前设备与服务器时差===========deltaT=%f",getDeltaT());
    
    for (int i=0;i<50;i++) //若该玩家信息已存在，则清除已有的玩家信息
    {
        if (Players::getInstance()[i].uid == val["uid"].asInt())
        {
            Players::getInstance()[i].init();
            break;
        }
    }
    
    int index;
    for (int i=0;i<50;i++)
    {
        if (Players::getInstance()[i].is_player==false) //未保存用户信息
        {
            index = i;
            break;
        }
    }
    
    Players::getInstance()[index].is_player = true;
    Players::getInstance()[index].avatar = val["avatar"].asString();
    string cards = val["best_cards"].asString();
    if (cards.size()==10)
    {
        Players::getInstance()[index].best_cards[0] = CCString::createWithFormat("%c%c",cards.at(0),cards.at(1))->getCString();
        Players::getInstance()[index].best_cards[1] = CCString::createWithFormat("%c%c",cards.at(2),cards.at(3))->getCString();
        Players::getInstance()[index].best_cards[2] = CCString::createWithFormat("%c%c",cards.at(4),cards.at(5))->getCString();
        Players::getInstance()[index].best_cards[3] = CCString::createWithFormat("%c%c",cards.at(6),cards.at(7))->getCString();
        Players::getInstance()[index].best_cards[4] = CCString::createWithFormat("%c%c",cards.at(8),cards.at(9))->getCString();
    }
    Players::getInstance()[index].birthday=val["birthday"].asString();
    Players::getInstance()[index].max_chip=val["max_ship"].asInt();
    Players::getInstance()[index].coin=val["coin"].asInt();
    Players::getInstance()[index].contact=val["contact"].asString();
    Players::getInstance()[index].exp=val["exp"].asInt();
    Players::getInstance()[index].money=val["money"].asInt();
    Players::getInstance()[index].name=val["name"].asString();
    Players::getInstance()[index].pcount=val["pcount"].asInt();
    Players::getInstance()[index].ps=val["ps"].asString();
    Players::getInstance()[index].rmb=val["rmb"].asInt();
    Players::getInstance()[index].seatid=-1;
    Players::getInstance()[index].sex=val["sex"].asInt();
    Players::getInstance()[index].uid=val["uid"].asInt();
    Players::getInstance()[index].total_board=val["total_board"].asInt();
    Players::getInstance()[index].total_win=val["total_win"].asInt();
    Players::getInstance()[index].vlevel=val["vlevel"].asInt();
    Players::getInstance()[index].vtime=val["vtime"].asInt();
    Players::getInstance()[index].zone=val["zone"].asString();

    CCLog("=====快速进入房间成功===end==");
}
*/

/*
void RoomScene::handler_server_login_error(JPacket jpacket)
{
    CCLog("=====登陆失败=====");
    Json::Value &val = jpacket.tojson();
    
    if (isExistPromote())
    {
        return;
    }
    
    CCString *str = CCString::createWithFormat("%d%d",SERVER_LOGIN_ERROR,val["err"].asInt());
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    RoomPopFrame *popFrame = RoomPopFrame::create();
    popFrame->setAnchorPoint(ccp(0,0));
    popFrame->setAnchorPoint(ccp(0,0));
    scene->addChild(popFrame,6);
    
    popFrame->createDialog(getStrFromXml(str->getCString()));
    CCLog("=====登陆失败==end===");
}
*/

/*
void RoomScene::handler_server_sit_error(JPacket jpacket)
{
    CCLog("=====坐下失败=====");
    //坐下失败，刷新自己的座位id，和桌面头像
    JPacket jpackets;
    jpackets.val["uid"] = myUID;
    jpackets.val["seatid"] = mySeatID;
    jpackets.val["menoy"] = static_cast<int>(PlayerModel::getInstance()->money);
    jpackets.val["stake"] = getRff()[mySeatID].own_gold;
    jpackets.end();
    
    if (mySeatID!=-1){
        handler_server_stand_up(jpackets);
    }
    
    Json::Value &val = jpacket.tojson();
    
    if (isExistPromote())
    {
        return;
    }
    
    CCString *str = CCString::createWithFormat("%d%d",SERVER_SIT_ERROR,val["err"].asInt());
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    RoomPopFrame *popFrame = RoomPopFrame::create();
    popFrame->setAnchorPoint(ccp(0,0));
    popFrame->setAnchorPoint(ccp(0,0));
    scene->addChild(popFrame,6);
    
    popFrame->createDialog(getStrFromXml(str->getCString()));
    CCLog("=====坐下失败==end===");
}
*/

/*
void RoomScene::handler_server_stand_error(JPacket jpacket)
{
    CCLog("=====站起失败=====");
    Json::Value &val = jpacket.tojson();
    
    if (isExistPromote())
    {
        return;
    }
    
    CCString *str = CCString::createWithFormat("%d%d",SERVER_STAND_ERROR,val["err"].asInt());
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    RoomPopFrame *popFrame = RoomPopFrame::create();
    popFrame->setAnchorPoint(ccp(0,0));
    popFrame->setAnchorPoint(ccp(0,0));
    scene->addChild(popFrame,6);
    
    popFrame->createDialog(getStrFromXml(str->getCString()));
    CCLog("=====站起失败===end==");
}
*/

/*
void RoomScene::handler_server_logout_error(JPacket jpacket)
{
    CCLog("=====登出房间失败=====");
    Json::Value &val = jpacket.tojson();
    
    if (isExistPromote())
    {
        return;
    }
    
    CCString *str = CCString::createWithFormat("%d%d",SERVER_LOGOUT_ERROR,val["err"].asInt());
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    RoomPopFrame *popFrame = RoomPopFrame::create();
    popFrame->setAnchorPoint(ccp(0,0));
    popFrame->setAnchorPoint(ccp(0,0));
    scene->addChild(popFrame,6);
    
    popFrame->createDialog(getStrFromXml(str->getCString()));
    
    CCLog("=====登出房间失败===end==");
}
*/

/*
void RoomScene::handler_server_player_actions_error(JPacket jpacket)
{
    Json::Value &val = jpacket.tojson();
    CCLog("=====操作失败=====");
    if (isExistPromote())
    {
        return;
    }
    
    CCString *str = CCString::createWithFormat("%d%d",SERVER_PLAYER_ACTION_ERR,val["err"].asInt());
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    RoomPopFrame *popFrame = RoomPopFrame::create();
    popFrame->setAnchorPoint(ccp(0,0));
    popFrame->setAnchorPoint(ccp(0,0));
    scene->addChild(popFrame,6);
    
    popFrame->createDialog(getStrFromXml(str->getCString()));
    
    CCLog("=====操作失败===end==");
}
*/

/*
void RoomScene::handler_server_quickly_login_error(JPacket jpacket)
{
    
}
*/

/*
void RoomScene::handler_update_room_succ(JPacket jpacket)
{
//Cocos2d: Room cmd 4 data {
//    "cmd" : 4032,
//    "coin" : 0,
//    "exp" : 5197,
//    "money" : 167739,
//    "rmb" : 152,
//    "seatid" : 8,
//    "total_board" : 7297,
//    "total_win" : 529,
//    "uid" : 313
//}
    CCLog("更新房间数据 succ , 服务器返回成功!!!!");
    Json::Value &val = jpacket.tojson();
    
    PlayerModel::getInstance()->money = val["money"].asInt();
    
    CCLog("更新房间数据＝＝end");
}
*/

/*
void RoomScene::handler_update_room_error(JPacket jpacket)
{
    CCLog("更新房间数据 room error -- end!!!!");
}
*/

