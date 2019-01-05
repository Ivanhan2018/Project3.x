//
//  RoomLoading.cpp
//  gamePoker
//
//  Created by xavier on 13-12-24.
//
//

#include "RoomLoading.h"
#include "RoomInfo.h"
#include "RoomScene.h"
#include "StartScene.h"

//const string prompt_text[]={
//    "全押需要谨慎再谨慎，除非你有必胜的把握。",
//    "游戏规则不明白的话可以去“帮助”界面了解。",
//    "连续登录可以最高可以一次获得5000筹码！",
//    "老虎机和刮刮乐也是不错的赢取筹码地点。",
//    "时刻关注活动，获得最新游戏优惠资讯。",
//    "昨日充值榜冠，亚，季军可以获得充值数100%，50%，20%的奖励！",
//};

RoomLoading::RoomLoading():isPlayEffect(NULL){
    
}


RoomLoading::~RoomLoading(){
    CCUserDefault::sharedUserDefault()->setBoolForKey(SYSTEM_VOLUME_EFFECT_SWITCH_KEY,isPlayEffect);
    CCUserDefault::sharedUserDefault()->flush();
}


bool RoomLoading::init()
{
    if (!Layer::init())
    {
        return false;
    }
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite *loading_bk = CCSprite::create("rooms/frame/room_background.png");
    loading_bk->setAnchorPoint(ccp(0.5f, 0.5f));
    loading_bk->setPosition(ccp(winSize.width/2,winSize.height/2));
    loading_bk->setScaleY(winSize.height/480);
	this->addChild(loading_bk);
    
    CCSprite *room_desk = CCSprite::create("rooms/frame/roomdesk_1.png");//(CCString::createWithFormat("rooms/frame/roomdesk_%s.png",RoomInfo::getInstance()->deskType.c_str())->getCString());
    room_desk->setAnchorPoint(ccp(0.5f, 0.5f));
    room_desk->setPosition(ccp(winSize.width/2,winSize.height/2));
    this->addChild(room_desk,0);
    
    CCSprite *room_load = CCSprite::create("common/loading_icon.png");
    room_load->setAnchorPoint(ccp(0.5f, 0.5f));
    room_load->setPosition(ccp(winSize.width/2+8,winSize.height/2+15));
    this->addChild(room_load,1);
    
    room_load->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360.0f)));
    
   
    //CCLabelTTF *prompt = CCLabelTTF::create(prompt_text[abs(rand()%6)].c_str(), "dezhou", 20);
    //prompt->setPosition(ccp(winSize.width/2+8, winSize.height/2-35));
    //addChild(prompt,2);
   
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
    isPlayEffect=CCUserDefault::sharedUserDefault()->getBoolForKey(SYSTEM_VOLUME_EFFECT_SWITCH_KEY,true);;
    CCUserDefault::sharedUserDefault()->setBoolForKey(SYSTEM_VOLUME_EFFECT_SWITCH_KEY,false);
    CCUserDefault::sharedUserDefault()->flush();
    
    scheduleOnce(schedule_selector(RoomLoading::timeOut), 40.0f);
    
    this->setTouchEnabled(true);
    
    return true;
}

//void RoomLoading::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -130, true);
//}

bool RoomLoading::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void RoomLoading::timeOut(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    showToast("网络繁忙，请重新进入房间。");
#else
    //CCMessageBox("网络繁忙，请重新进入房间。","错误提示");
#endif
    
    close();
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, StartScene::scene(),Color3B::BLACK));
}

void RoomLoading::close()
{
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    CCUserDefault::sharedUserDefault()->setBoolForKey(SYSTEM_VOLUME_EFFECT_SWITCH_KEY,isPlayEffect);
    CCUserDefault::sharedUserDefault()->flush();
    
    this->removeFromParentAndCleanup(true);
}

