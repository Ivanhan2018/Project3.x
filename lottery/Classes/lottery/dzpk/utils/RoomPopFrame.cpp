//
//  RoomPopFrame.cpp
//  gamePoker
//
//  Created by xavier on 13-12-11.
//
//

#include "RoomPopFrame.h"
//#include "RoomScene.h"
#include "Sound.h"

RoomPopFrame::RoomPopFrame()
{
    //RoomScene::setExistPromote(false);
    m_pfnCallFunc=NULL;
    m_pobj=NULL;

    m_pfnCallFuncs=NULL;
    m_pobjs=NULL;
}

RoomPopFrame::~RoomPopFrame()
{
    m_pfnCallFunc=NULL;
    m_pobj=NULL;
    
    m_pfnCallFuncs=NULL;
    m_pobjs=NULL;
    //RoomScene::setExistPromote(false);
}



bool RoomPopFrame::init()
{
    if (!Layer::init())
    {
        return false;
    }
    //RoomScene::setExistPromote(false);
    setAnchorPoint(ccp(0,0));
    setTouchEnabled(true);
    m_click=false;
    return true;
}

//只有确定
void RoomPopFrame::createDialog(const char *info)
{
    //RoomScene::setExistPromote(false);
    this->removeAllChildrenWithCleanup(true);
    
    CCPoint originPoint = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLayerColor* bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 100), size.width, size.height);
	bgLayer->setAnchorPoint(ccp(0,0));
	bgLayer->setPosition(ccp(0+originPoint.x,0+originPoint.y));
	this->addChild(bgLayer);
    
    CCSprite *pop_bg = CCSprite::create("common/comm_pop_frame.png");
    pop_bg->setAnchorPoint(ccp(0.5f,0.5f));
    pop_bg->setTag(223);
    pop_bg->setPosition(ccp(size.width/2+originPoint.x,size.height/2+originPoint.y));
    this->addChild(pop_bg);
    
    CCSize bg_size = pop_bg->getContentSize();
    
    CCLabelTTF *info_title = CCLabelTTF::create("温馨提示", "Arial-BoldMT", 24);
    info_title->setAnchorPoint(ccp(0.5f,0.5f));
    info_title->setPosition(ccp(bg_size.width/2,bg_size.height-47));
    pop_bg->addChild(info_title);
    
    if (info!=NULL)
    {
        CCLabelTTF *info_content = CCLabelTTF::create(info, "Arial-BoldMT", 20, CCSizeMake(330, 0), kCCTextAlignmentCenter);
        info_content->setAnchorPoint(ccp(0.5f,0.5f));
        info_content->setPosition(ccp(bg_size.width/2,bg_size.height/2+20));
        pop_bg->addChild(info_content);
    }

    Scale9Sprite* okBtn_normal = Scale9Sprite::create("common/btn_common.png");
    Scale9Sprite* okBtn_selected = Scale9Sprite::create("common/btn_common.png");
    Scale9Sprite* okBtn_highLight = Scale9Sprite::create("common/btn_common.png");
    Scale9Sprite* okBtn_disabled = Scale9Sprite::create("common/btn_common.png");
    
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
    okBtn->setPosition(ccp(bg_size.width/2,70));
    //okBtn->setTouchPriority(-200);
    pop_bg->addChild(okBtn,1);
    okBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomPopFrame::okBtnCallback), Control::EventType::TOUCH_DOWN);
    
    pop_bg->setScale(0.5f);
    pop_bg->runAction(CCScaleTo::create(0.15f,1.0f));
    
    //RoomScene::setExistPromote(true);
}


void RoomPopFrame::createDialog(const char *title ,const char *info)
{
    //RoomScene::setExistPromote(false);
    this->removeAllChildrenWithCleanup(true);
    
    CCPoint originPoint = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLayerColor* bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 100), size.width, size.height);
	bgLayer->setAnchorPoint(ccp(0,0));
	bgLayer->setPosition(ccp(0+originPoint.x,0+originPoint.y));
	this->addChild(bgLayer);
    
    CCSprite *pop_bg = CCSprite::create("common/comm_pop_frame.png");
    pop_bg->setAnchorPoint(ccp(0.5f,0.5f));
    pop_bg->setPosition(ccp(size.width/2+originPoint.x,size.height/2+originPoint.y));
    pop_bg->setTag(223);
    this->addChild(pop_bg);
    
    CCSize bg_size = pop_bg->getContentSize();
    
    CCLabelTTF *info_title = CCLabelTTF::create(title, "Arial-BoldMT", 24);
    info_title->setAnchorPoint(ccp(0.5f,0.5f));
    info_title->setPosition(ccp(bg_size.width/2,bg_size.height-47));
    pop_bg->addChild(info_title);
    
    CCLabelTTF *info_content = CCLabelTTF::create(info, "Arial-BoldMT", 20, CCSizeMake(330, 0), kCCTextAlignmentLeft);
    info_content->setAnchorPoint(ccp(0.5f,0.5f));
    info_content->setPosition(ccp(bg_size.width/2,bg_size.height/2));
    pop_bg->addChild(info_content);
    
    //确认
    Scale9Sprite* okBtn_normal = Scale9Sprite::create("common/pop_btn_right_1.png");
    Scale9Sprite* okBtn_selected = Scale9Sprite::create("common/pop_btn_right_2.png");
    Scale9Sprite* okBtn_highLight = Scale9Sprite::create("common/pop_btn_right_2.png");
    Scale9Sprite* okBtn_disabled = Scale9Sprite::create("common/pop_btn_right_2.png");
    
    CCSize okBtn_size = okBtn_normal->getContentSize();
    CCLabelTTF *okBtn_label = CCLabelTTF::create("确认","Arial-BoldMT",24);
    ControlButton* okBtn = ControlButton::create(okBtn_label,okBtn_normal);
    okBtn->setPreferredSize(okBtn_size);
    
    okBtn->setZoomOnTouchDown(false);
    okBtn->setBackgroundSpriteForState(okBtn_normal,Control::State::NORMAL);
    okBtn->setBackgroundSpriteForState(okBtn_selected,Control::State::SELECTED);
    okBtn->setBackgroundSpriteForState(okBtn_highLight,Control::State::HIGH_LIGHTED);
    okBtn->setBackgroundSpriteForState(okBtn_disabled,Control::State::DISABLED);
    okBtn->setAnchorPoint(ccp(1,0));
    okBtn->setPosition(ccp(bg_size.width-10,10));
    //okBtn->setTouchPriority(-200);
    pop_bg->addChild(okBtn,1);
    okBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomPopFrame::okBtnCallback),Control::EventType::TOUCH_DOWN);
    
    //取消
    Scale9Sprite* cancelBtn_normal = Scale9Sprite::create("common/pop_btn_left_1.png");
    Scale9Sprite* cancelBtn_selected = Scale9Sprite::create("common/pop_btn_left_2.png");
    Scale9Sprite* cancelBtn_highLight = Scale9Sprite::create("common/pop_btn_left_2.png");
    Scale9Sprite* cancelBtn_disabled = Scale9Sprite::create("common/pop_btn_left_2.png");
    
    CCSize cancelBtn_size = cancelBtn_normal->getContentSize();
    CCLabelTTF *cancelBtn_label = CCLabelTTF::create("取消","Arial-BoldMT",24);
    ControlButton* cancelBtn = ControlButton::create(cancelBtn_label,cancelBtn_normal);
    cancelBtn->setPreferredSize(cancelBtn_size);
    
    cancelBtn->setZoomOnTouchDown(false);
    cancelBtn->setBackgroundSpriteForState(cancelBtn_normal,Control::State::NORMAL);
    cancelBtn->setBackgroundSpriteForState(cancelBtn_selected,Control::State::SELECTED);
    cancelBtn->setBackgroundSpriteForState(cancelBtn_highLight,Control::State::HIGH_LIGHTED);
    cancelBtn->setBackgroundSpriteForState(cancelBtn_disabled,Control::State::DISABLED);
    cancelBtn->setAnchorPoint(ccp(0,0));
    cancelBtn->setPosition(ccp(10,10));
    //cancelBtn->setTouchPriority(-200);
    pop_bg->addChild(cancelBtn,1);
    cancelBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomPopFrame::canelBtnCallback),Control::EventType::TOUCH_DOWN);
    
    pop_bg->setScale(0.5f);
    pop_bg->runAction(CCScaleTo::create(0.15f,1.0f));
    
    //RoomScene::setExistPromote(true);
    
    Sound::getInstance()->playEffect("sound/sound_chuangkou.ogg");
    
    setClickOnce(true);
}

//拥有两个按钮
void RoomPopFrame::createDialog(const char *info,const char *btn_title1 ,const char *btn_title2)
{
    this->removeAllChildrenWithCleanup(true);
    
    CCPoint originPoint = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLayerColor* bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 100), size.width, size.height);
	bgLayer->setAnchorPoint(ccp(0,0));
	bgLayer->setPosition(ccp(0+originPoint.x,0+originPoint.y));
	this->addChild(bgLayer);
    
    CCSprite *pop_bg = CCSprite::create("common/comm_pop_frame.png");
    pop_bg->setAnchorPoint(ccp(0.5f,0.5f));
    pop_bg->setPosition(ccp(size.width/2+originPoint.x,size.height/2+originPoint.y));
    pop_bg->setTag(223);
    this->addChild(pop_bg);
    
    CCSize bg_size = pop_bg->getContentSize();
    
    CCLabelTTF *info_title = CCLabelTTF::create("提示", "Arial-BoldMT", 24);
    info_title->setAnchorPoint(ccp(0.5f,0.5f));
    info_title->setPosition(ccp(bg_size.width/2,bg_size.height-47));
    pop_bg->addChild(info_title);
    
    CCLabelTTF *info_content = CCLabelTTF::create(info, "Arial-BoldMT", 20, CCSizeMake(330, 0), kCCTextAlignmentLeft);
    info_content->setAnchorPoint(ccp(0.5f,0.5f));
    info_content->setPosition(ccp(bg_size.width/2,bg_size.height/2+30));
    pop_bg->addChild(info_content);
    
    //左边按钮
    Scale9Sprite* okBtn_normal = Scale9Sprite::create("common/pop_btn_left_1.png");
    Scale9Sprite* okBtn_selected = Scale9Sprite::create("common/pop_btn_left_2.png");
    Scale9Sprite* okBtn_highLight = Scale9Sprite::create("common/pop_btn_left_2.png");
    Scale9Sprite* okBtn_disabled = Scale9Sprite::create("common/pop_btn_left_2.png");
    
    CCSize okBtn_size = okBtn_normal->getContentSize();
    CCLabelTTF *okBtn_label = CCLabelTTF::create(btn_title1,"Arial-BoldMT",24);
    ControlButton* okBtn = ControlButton::create(okBtn_label,okBtn_normal);
    okBtn->setPreferredSize(okBtn_size);
    
    okBtn->setZoomOnTouchDown(false);
    okBtn->setBackgroundSpriteForState(okBtn_normal,Control::State::NORMAL);
    okBtn->setBackgroundSpriteForState(okBtn_selected,Control::State::SELECTED);
    okBtn->setBackgroundSpriteForState(okBtn_highLight,Control::State::HIGH_LIGHTED);
    okBtn->setBackgroundSpriteForState(okBtn_disabled,Control::State::DISABLED);
    okBtn->setAnchorPoint(ccp(0,0));
    okBtn->setPosition(ccp(10,10));
    //okBtn->setTouchPriority(-200);
    pop_bg->addChild(okBtn,1);
    okBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomPopFrame::okBtnCallback), Control::EventType::TOUCH_DOWN);
    
    //右边按钮
    Scale9Sprite* cancelBtn_normal = Scale9Sprite::create("common/pop_btn_right_1.png");
    Scale9Sprite* cancelBtn_selected = Scale9Sprite::create("common/pop_btn_right_2.png");
    Scale9Sprite* cancelBtn_highLight = Scale9Sprite::create("common/pop_btn_right_2.png");
    Scale9Sprite* cancelBtn_disabled = Scale9Sprite::create("common/pop_btn_right_2.png");
    
    CCSize cancelBtn_size = cancelBtn_normal->getContentSize();
    CCLabelTTF *cancelBtn_label = CCLabelTTF::create(btn_title2,"Arial-BoldMT",24);
    ControlButton* cancelBtn = ControlButton::create(cancelBtn_label,cancelBtn_normal);
    cancelBtn->setPreferredSize(cancelBtn_size);
    
    cancelBtn->setZoomOnTouchDown(false);
    cancelBtn->setBackgroundSpriteForState(cancelBtn_normal,Control::State::NORMAL);
    cancelBtn->setBackgroundSpriteForState(cancelBtn_selected,Control::State::SELECTED);
    cancelBtn->setBackgroundSpriteForState(cancelBtn_highLight,Control::State::HIGH_LIGHTED);
    cancelBtn->setBackgroundSpriteForState(cancelBtn_disabled,Control::State::DISABLED);
    cancelBtn->setAnchorPoint(ccp(1,0));
    cancelBtn->setPosition(ccp(bg_size.width-10,10));
    //cancelBtn->setTouchPriority(-200);
    pop_bg->addChild(cancelBtn,1);
    cancelBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomPopFrame::canelBtnCallback),Control::EventType::TOUCH_DOWN);
    
    pop_bg->setScale(0.5f);
    pop_bg->runAction(CCScaleTo::create(0.15f,1.0f));
    
    Sound::getInstance()->playEffect("sound/sound_chuangkou.ogg");
}


//void RoomPopFrame::registerWithTouchDispatcher()
//{
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -150, true);
//}

bool RoomPopFrame::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    return true;
}

void RoomPopFrame::okBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
    close();
    
    if (m_click) {
        m_click=false;
        ControlButton *okBtn = dynamic_cast<ControlButton*>(sender);
        okBtn->setTouchEnabled(false);
    }
    
    if (m_pobj && m_pfnCallFunc)
    {
        (m_pobj->*m_pfnCallFunc)(true);
    }
}

void RoomPopFrame::canelBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
    close();
    
    if (m_click) {
        m_click=false;
        ControlButton *cancelBtn = dynamic_cast<ControlButton*>(sender);
        cancelBtn->setTouchEnabled(false);
    }
    
    if (m_pobjs && m_pfnCallFuncs)
    {
        (m_pobjs->*m_pfnCallFuncs)(true);
    }
}

void RoomPopFrame::setClickOnce(bool var)
{
    m_click=var;
}


void RoomPopFrame::close()
{
    if (!getChildByTag(223))
        return;
    //RoomScene::setExistPromote(false);
    this->setTouchEnabled(false);
    CCSprite *bg = dynamic_cast<CCSprite*>(getChildByTag(223));
    CCCallFunc *cf = CCCallFunc::create(this, callfunc_selector(RoomPopFrame::closeCallback));
    bg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 0.5f),cf,NULL));
    
    Sound::getInstance()->playEffect("sound/sound_guanbi.ogg");
}

void RoomPopFrame::closeCallback()
{
    this->removeFromParentAndCleanup(true);
}


void RoomPopFrame::setSelector(CCObject* obj, SEL_COMMON selector)
{
    m_pobj=obj;
    m_pfnCallFunc=selector;
}

void RoomPopFrame::setReturnSelector(CCObject* obj, SEL_COMMON selector)
{
    m_pobjs=obj;
    m_pfnCallFuncs=selector;
}