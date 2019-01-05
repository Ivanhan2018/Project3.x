//
//  RoomBox.cpp
//  gamePoker
//
//  Created by xavier on 14-1-9.
//
//

#include "RoomBox.h"
#include "Inc.h"


bool RoomBox::init()
{
	if (!Layer::init())
	{
		return false;
	}
    
    ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(0,0));
    
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite *box_bk = CCSprite::createWithSpriteFrameName("room_box_frame.png");
    box_bk->setAnchorPoint(ccp(0,0));
    box_bk->setPosition(ccp(size.width-80,60));
    box_bk->setTag(1);
    this->addChild(box_bk);
    
    //boxBtn
    Scale9Sprite* boxBtn_normal = Scale9Sprite::createWithSpriteFrameName("room_box.png");
    Scale9Sprite* boxBtn_selected = Scale9Sprite::createWithSpriteFrameName("room_box.png");
    Scale9Sprite* boxBtn_highLight = Scale9Sprite::createWithSpriteFrameName("room_box.png");
    Scale9Sprite* boxBtn_disabled = Scale9Sprite::createWithSpriteFrameName("room_box.png");
    
    boxBtn_highLight->setColor(ccc3(131,131,131));
    CCSize boxBtnSize = boxBtn_highLight->getContentSize();
    ControlButton* boxBtn = ControlButton::create(boxBtn_normal);
    boxBtn->setPreferredSize(boxBtnSize);
        
    boxBtn->setZoomOnTouchDown(false);
    boxBtn->setBackgroundSpriteForState(boxBtn_normal,Control::State::NORMAL);
    boxBtn->setBackgroundSpriteForState(boxBtn_selected,CCControlStateSelected);
    boxBtn->setBackgroundSpriteForState(boxBtn_highLight,CCControlStateHighlighted);
    boxBtn->setBackgroundSpriteForState(boxBtn_disabled,CCControlStateDisabled);
    boxBtn->setAnchorPoint(ccp(0.5f,0.5f));
    boxBtn->setPosition(ccp(40,30));
    box_bk->addChild(boxBtn,1);
    boxBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBox::boxBtnCallback), CCControlEventTouchDown);
    
	this->setTouchEnabled(false);
    
	return true;
}

void RoomBox::openBoxList()
{
	int num[]={3,5};
    int reward_count[]={500,1000};
    
    for (int i=0;i!=2;++i)
    {
        CCLabelTTF* label_num = CCLabelTTF::create(CCString::createWithFormat("玩\n%d局",num[i])->getCString(), "dezhou", 18);
        label_num->setAnchorPoint(ccp(0.5f,0.5f));
        label_num->setPosition(ccp(108*(i+1),30));
        label_num->setTag(10+i);
        this->getChildByTag(1)->addChild(label_num);
        
        
        Scale9Sprite* getBtn_normal = Scale9Sprite::createWithSpriteFrameName("room_box_btn.png");
        Scale9Sprite* getBtn_selected = Scale9Sprite::createWithSpriteFrameName("room_box_btn.png");
        Scale9Sprite* getBtn_highLight = Scale9Sprite::createWithSpriteFrameName("room_box_btn.png");
        Scale9Sprite* getBtn_disabled = Scale9Sprite::createWithSpriteFrameName("room_box_btn.png");
        
        getBtn_highLight->setColor(ccc3(131,131,131));
        CCSize getBtnSize = getBtn_highLight->getContentSize();
        ControlButton* getBtn = ControlButton::create(getBtn_normal);
        getBtn->setPreferredSize(getBtnSize);
        
        getBtn->setZoomOnTouchDown(false);
        getBtn->setTouchPriority(-10);
        getBtn->setBackgroundSpriteForState(getBtn_normal,CCControlStateNormal);
        getBtn->setBackgroundSpriteForState(getBtn_selected,CCControlStateSelected);
        getBtn->setBackgroundSpriteForState(getBtn_highLight,CCControlStateHighlighted);
        getBtn->setBackgroundSpriteForState(getBtn_disabled,CCControlStateDisabled);
        getBtn->setAnchorPoint(ccp(0.5f,0.5f));
        getBtn->setPosition(ccp(150+108*i,30));
        getBtn->setTag(20+i);
        this->getChildByTag(1)->addChild(getBtn);
        getBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBox::getBtnCallback), CCControlEventTouchDown);
        
        CCSize btnSize = getBtn->getContentSize();
        CCLabelTTF* title = CCLabelTTF::create("领取", "dezhou", 16);
        title->setAnchorPoint(ccp(0.5f,0.5f));
        title->setPosition(ccp(btnSize.width/2,btnSize.height*2/3+2));
        getBtn->addChild(title);
        
        CCLabelTTF* chipNum = CCLabelTTF::create(CCString::createWithFormat("%d",reward_count[i])->getCString(), "dezhou", 16);
        chipNum->setAnchorPoint(ccp(0.5f,0.5f));
        chipNum->setPosition(ccp(btnSize.width/2,btnSize.height/3-2));
        getBtn->addChild(chipNum);
    }
}


void RoomBox::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -2, true);
}

bool RoomBox::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //0-290
    CCPoint point = pTouch->getLocation();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint originPoint = CCDirector::sharedDirector()->getVisibleOrigin();
    if (point.x>=size.width+originPoint.x-210 && point.y>=70+originPoint.y && point.x<=size.width+originPoint.x && point.y<=70+60+originPoint.y)
    {
        return true;
    }
        
    close();
    return true;
}

void RoomBox::onTouchCallback()
{
    if (this->getChildByTag(1) && this->getChildByTag(1)->getChildByTag(10))
    {
        this->getChildByTag(1)->removeChildByTag(10, true);
        this->getChildByTag(1)->removeChildByTag(11, true);
        this->getChildByTag(1)->removeChildByTag(20, true);
        this->getChildByTag(1)->removeChildByTag(21, true);
    }
}

void RoomBox::close()
{
    this->setTouchEnabled(false);
    
    CCCallFunc *cf = CCCallFunc::create(this, callfunc_selector(RoomBox::onTouchCallback));
    this->getChildByTag(1)->runAction(CCSequence::create(CCMoveBy::create(0.3f, ccp(210,0)),
                                                         cf,NULL));
}


void RoomBox::boxBtnCallback(CCObject *sender,CCControlEvent controlEvent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    showToast("此功能暂未开放");
#else
    CCMessageBox("此功能暂未开放","提示");
#endif
    /*
    this->setTouchEnabled(true);
    this->getChildByTag(1)->runAction(CCMoveBy::create(0.3f, ccp(-210,0)));
    openBoxList();
     */
}

void RoomBox::getBtnCallback(CCObject *sender,CCControlEvent controlEvent)
{
    int tag =dynamic_cast<ControlButton*>(sender)->getTag();
    
    CCLog("btn tag::::%d",tag);
}

bool RoomBox::getStatusByPlayNum(int num)
{
    return false;
}

