//
//  RoomOtherInfo.cpp
//  gamePoker
//
//  Created by xavier on 14-1-9.
//
//

#include "RoomOtherInfo.h"
#include "Button.h"
#include "Players.h"
#include "PlayerModel.h"
#include "StringFormat.h"
#include "Sound.h"


bool RoomOtherInfo::init()
{
	if (!Layer::init())
	{
		return false;
	}
    
    ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(0,0));
   
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint originPoint = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCLayerColor* bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 100), size.width, size.height);
	bgLayer->setAnchorPoint(ccp(0,0));
	bgLayer->setPosition(ccp(0+originPoint.x,0+originPoint.y));
	this->addChild(bgLayer);
    
    CCSprite* info_bk = CCSprite::create("rooms/frame/room_otherInfo_bk1.png");
    info_bk->setAnchorPoint(ccp(0.5f,0.5f));
    info_bk->setPosition(ccp(size.width/2+originPoint.x,size.height/2+originPoint.y));
    info_bk->setTag(123);
    this->addChild(info_bk,1);
    
    f_size = info_bk->getContentSize();
    
    //closeBtn
    Button* closeBtn = Button::createWithSpriteFrameName("close_btn.png");
    closeBtn->setAnchorPoint(ccp(0.5f, 0.5f));
    closeBtn->setPosition(ccp(f_size.width-15, f_size.height-15));
    closeBtn->setTag(0);
    closeBtn->setSelector(this, common_selector(RoomOtherInfo::close));
    info_bk->addChild(closeBtn);
    

	this->setTouchEnabled(true);
    
    info_bk->setScale(0.5f);
    info_bk->runAction(CCScaleTo::create(0.15f,1.0f));
    
	return true;
}

void RoomOtherInfo::initUI(int index)
{
    do{
        //玩家头像
        const char* genderPicName = Players::getInstance()[index].sex==1 ? "rooms/photo/room_other_photo_1.png" : "rooms/photo/room_other_photo_2.png";
        std::string headerFile = "no_custom_head";
        //CCHttpAgent::getInstance()->getImagePath(Players::getInstance()[index].uid,headerFile);
        //if (headerFile.compare("no_custom_head")==0)
        //{
        //    CCSprite* photo = CCSprite::create(genderPicName);
        //    CC_BREAK_IF(!photo);
        //    photo->setAnchorPoint(ccp(0.5f,0.5f));
        //    photo->setPosition(ccp(73,75));
        //    this->getChildByTag(123)->addChild(photo);
        //}else{
        //    CCSprite* photo = CCSprite::create(genderPicName);
        //    CC_BREAK_IF(!photo);
        //    CCSize sc_sz = photo->getContentSize();
        //    
        //    CCSprite *header = CCSprite::create(CCHttpAgent::getInstance()->getImagePath(Players::getInstance()[index].uid).c_str());
        //    CC_BREAK_IF(!header);
        //    CCSize tx_sz = header->getContentSize();
        //    header->setScaleX(sc_sz.width/tx_sz.width);
        //    header->setScaleY(sc_sz.height/tx_sz.height);
        //    header->setAnchorPoint(ccp(0.5f,0.5f));
        //    header->setPosition(ccp(73,75));
        //    this->getChildByTag(123)->addChild(header);
        //}
        
    
        CCLabelTTF* name = CCLabelTTF::create(Players::getInstance()[index].name.c_str(), "dezhou", 20 , CCSizeMake(120, 25) , kCCTextAlignmentLeft);
        CC_BREAK_IF(!name);
        name->setAnchorPoint(ccp(0.0f,0.5f));
        name->setPosition(ccp(140,105));
        this->getChildByTag(123)->addChild(name);
    
        //CCLabelTTF* sex = CCLabelTTF::create(Players::getInstance()[index].sex==2? "女":"男", "dezhou", 20);
        //CC_BREAK_IF(!sex);
        //sex->setAnchorPoint(ccp(0.5f,0.5f));
        //sex->setPosition(ccp(265,105));
        //this->getChildByTag(123)->addChild(sex);
    
        CCLabelTTF* level = CCLabelTTF::create(CCString::createWithFormat("LV  %d",PlayerModel::getInstance()->getLevelByExp(Players::getInstance()[index].exp))->getCString(),
                                           "dezhou", 20);
        CC_BREAK_IF(!level);
        level->setAnchorPoint(ccp(0.0f,0.5f));
        level->setPosition(ccp(140,70));
        this->getChildByTag(123)->addChild(level);
    
        CCSprite* icon_momey = CCSprite::createWithSpriteFrameName("room_small_chip.png");
        CC_BREAK_IF(!icon_momey);
        icon_momey->setAnchorPoint(ccp(0.0f,0.5f));
        icon_momey->setPosition(ccp(139,35));
        this->getChildByTag(123)->addChild(icon_momey);
    
        CCLabelTTF* money = CCLabelTTF::create(StringFormat::intsert(Players::getInstance()[index].money, 3, ","),"dezhou", 20);
        CC_BREAK_IF(!money);
        money->setAnchorPoint(ccp(0.0f,0.5f));
        money->setPosition(ccp(170,35));
        this->getChildByTag(123)->addChild(money);
        
        Sound::getInstance()->playEffect("sound/sound_chuangkou.ogg");
    }while(0);
}


//void RoomOtherInfo::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
//}

bool RoomOtherInfo::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void RoomOtherInfo::onTouchCallback()
{
	//this->getParent()->removeChildByTag(2,true);
}

void RoomOtherInfo::close()
{
    
    Sound::getInstance()->playEffect("sound/sound_guanbi.ogg");
    
	if (!getChildByTag(123))
        return;
    CCSprite *infoBk = dynamic_cast<CCSprite*>(getChildByTag(123));
    CCCallFunc *cf = CCCallFunc::create(this, callfunc_selector(RoomOtherInfo::closeCallback));
    infoBk->runAction(CCSequence::create(CCScaleTo::create(0.15f, 0.5f),cf,NULL));
}

void RoomOtherInfo::closeCallback()
{
    this->removeFromParentAndCleanup(true);
}

