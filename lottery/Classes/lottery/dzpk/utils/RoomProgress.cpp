//
//  RoomProgress.cpp
//  ffff
//
//  Created by xavier on 13-11-26.
//
//

#include "RoomProgress.h"
#include "RoomScene.h"
#include "RoomInfo.h"
//#include "MyScrollView.h"
//#include "MyControlSlider.h"
//#include "MYControlButton.h"
#include "Sound.h"

bool RoomProgress::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    initLayer();
    
    return true;
}

void RoomProgress::initLayer()
{
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    
    for (int i=0;i!=5;++i)
    {
        CCString *numTitle = CCString::createWithFormat("%d",RoomInfo::getInstance()->quick_addBet[i]);
        //MYControlButton *quicklyAddBetBtn = MYControlButton::createWithTitle(numTitle->getCString(),"room_addbet_btn1.png", "room_addbet_btn2.png",24);
        //quicklyAddBetBtn->setTouchPriority(-10);
        //quicklyAddBetBtn->setPosition(ccp(3+quicklyAddBetBtn->getContentSize().width*(i+0.5),27));
        //quicklyAddBetBtn->setTag(10+i);
        //quicklyAddBetBtn->setSelector(this, common_selector(RoomProgress::betBtnCallback));
        //this->addChild(quicklyAddBetBtn,10);
        //
        //if (RoomScene::getRff()[RoomScene::getMySeatID()].own_gold<RoomInfo::getInstance()->quick_addBet[i])
        //{
        //    quicklyAddBetBtn->setEnabled(false);
        //    quicklyAddBetBtn->setColor(ccc3(131, 131, 131));
        //}
    }
    
    //确认
    Scale9Sprite* okBtn_normal = Scale9Sprite::createWithSpriteFrameName("room_addbet_btn1.png");
    Scale9Sprite* okBtn_selected = Scale9Sprite::createWithSpriteFrameName("room_addbet_btn2.png");
    Scale9Sprite* okBtn_highLight = Scale9Sprite::createWithSpriteFrameName("room_addbet_btn2.png");
    Scale9Sprite* okBtn_disabled = Scale9Sprite::createWithSpriteFrameName("room_addbet_btn2.png");
    
    okBtn_highLight->setColor(ccc3(131,131,131));
    CCSize okBtn_size = okBtn_normal->getContentSize();
    CCLabelTTF *okBtn_label = CCLabelTTF::create("确认","Arial-BoldMT",24);
    ControlButton* okBtn = ControlButton::create(okBtn_label,okBtn_normal);
    okBtn->setPreferredSize(okBtn_size);
    
    okBtn->setZoomOnTouchDown(false);
    okBtn->setBackgroundSpriteForState(okBtn_normal,Control::State::NORMAL);
    okBtn->setBackgroundSpriteForState(okBtn_selected,Control::State::SELECTED);
    okBtn->setBackgroundSpriteForState(okBtn_highLight,Control::State::HIGH_LIGHTED);
    okBtn->setBackgroundSpriteForState(okBtn_disabled,Control::State::DISABLED);
    okBtn->setAnchorPoint(ccp(0.5f,0.5f));
    okBtn->setPosition(ccp(winSize.width-3-0.5*okBtn_normal->getContentSize().width,27));
    //okBtn->setTouchPriority(-10);
    okBtn->setTag(2);
    this->addChild(okBtn,10);
    okBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomProgress::okBtnCallback),Control::EventType::TOUCH_DOWN);
   
    //加注background
    CCSprite* slider_background = CCSprite::createWithSpriteFrameName("room_addbet_bk.png");
    slider_background->setAnchorPoint(ccp(1,0));
    slider_background->setPosition(ccp(winSize.width,okBtn->getContentSize().height/2+7));
    this->addChild(slider_background,1);
   
    //加注数量
    CCLabelTTF *chipNum = CCLabelTTF::create("0","Arial-BoldMT",20);
    chipNum->setAnchorPoint(ccp(0.5f,0.5f));
    chipNum->setPosition(ccp(winSize.width+2-slider_background->getContentSize().width/2,
                             okBtn->getContentSize().height/2+slider_background->getContentSize().height-58));
    chipNum->setColor(ccc3(119, 255, 148));
    chipNum->setTag(3);
    this->addChild(chipNum,2);
    
    //加注scroll
    CCSprite *backgroundSprite  = CCSprite::createWithSpriteFrameName("room_addbet_bar.png");
    CCSprite *progressSprite    = CCSprite::createWithSpriteFrameName("room_addbet_pro.png");
    CCSprite *thumbSprite       = CCSprite::createWithSpriteFrameName("room_addbet_btn.png");
    //MyControlSlider *slider = MyControlSlider::create(backgroundSprite,progressSprite,thumbSprite);
    //slider->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomProgress::updateNum),CCControlEventValueChanged);
    //slider->setAnchorPoint(ccp(0.5,0));
    //slider->setTouchPriority(-10);
    //slider->setScaleY(0.9f);
    //slider->setPosition(ccp(winSize.width-slider_background->getContentSize().width/2,
    //                        okBtn->getContentSize().height/2+68));
    //silder的最小值和最大值
    //int min_raise = (RoomScene::getRff()[RoomScene::getMySeatID()].min_raise)/(RoomInfo::getInstance()->s_bet); //最小加注
    //int max_raise = (RoomScene::getRff()[RoomScene::getMySeatID()].own_gold)/(RoomInfo::getInstance()->s_bet);
    //slider->setMinimumValue(min_raise);
    //slider->setMaximumValue(max_raise);
    
    //slider的当前值
    //slider->setValue(min_raise);
    //this->addChild(slider,3);
    
    this->setTouchEnabled(true);
 
}

//void RoomProgress::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
//}

bool RoomProgress::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint point = pTouch->getLocation();
    CCPoint pt = convertToWorldSpace(point);
    CCRect rect = CCRectMake(0, 60, size.width-160, size.height-60);
    if (rect.containsPoint(pt) ) {
        CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
        do{
            CC_BREAK_IF(!scene->getChildByTag(10));
            RoomBottom *roomBottom = (RoomBottom*)scene->getChildByTag(10)->getChildByTag(1);
            CC_BREAK_IF(!roomBottom);
            roomBottom->show();
            
            this->removeFromParentAndCleanup(true);
        }while (0);
    }
    return true;
}

void RoomProgress::betBtnCallback(int tag)
{
    if (tag<10 || tag>14)
        return;
    
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    do{
        CC_BREAK_IF(!scene->getChildByTag(10));
        RoomBottom *roomBottom = (RoomBottom*)scene->getChildByTag(10)->getChildByTag(1);
        CC_BREAK_IF(!roomBottom);
        roomBottom->send_player_action(2001,RoomInfo::getInstance()->quick_addBet[tag-10]);
        roomBottom->updateStatus(4); //加注
        roomBottom->show();
    }while (0);
}

void RoomProgress::okBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
    //加注
    if (count>0)
    {
        CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
        do{
            CC_BREAK_IF(!scene->getChildByTag(10));
            RoomBottom *roomBottom = (RoomBottom*)scene->getChildByTag(10)->getChildByTag(1);
            CC_BREAK_IF(!roomBottom);
            roomBottom->send_player_action(2001,getCurrCount());
            roomBottom->updateStatus(4); //加注
            roomBottom->show();
        }while (0);
    }
}

void RoomProgress::updateNum(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
    ControlSlider* pSliderCtl = (ControlSlider*)sender;
    count= ((int)pSliderCtl->getValue())*(RoomInfo::getInstance()->s_bet);

    CCString *string ;
    
    //if (count == RoomScene::getRff()[RoomScene::getMySeatID()].own_gold)
    //{
    //    string = CCString::createWithFormat("%s","全下");
    //}else{
    //    string = CCString::createWithFormat("%d",count);
    //}
    
    CCLabelTTF *chipNum = (CCLabelTTF*)this->getChildByTag(3);
    chipNum->setString(string->getCString());
}

int RoomProgress::getCurrCount()
{
    return count;
}

