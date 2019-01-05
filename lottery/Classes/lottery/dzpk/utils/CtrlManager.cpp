//
//  CtrlManager.cpp
//  TexasPoker
//
//  Created by peniel on 12/26/13.
//
//

#include "CtrlManager.h"

ControlButton* CtrlManager::createCtrlBtnWithFile(const char* btnBgName,const char* title,bool isColor)
{
	ControlButton* controlBtn = NULL;
    
	Scale9Sprite* btnNormal = Scale9Sprite::create(btnBgName);
	Scale9Sprite* btnSelected = Scale9Sprite::create(btnBgName);
	Scale9Sprite* btnHighlighted = Scale9Sprite::create(btnBgName);
    if (isColor)
    {
        btnHighlighted->setColor(ccc3(131,131,131));
    }
	Scale9Sprite* btnDisabled = Scale9Sprite::create(btnBgName);
	CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(CTRL_BTN_TOUCH_PRIORITY);
	controlBtn->setPreferredSize(btnSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFile(const char* btnBgName,int touchPriority)
{
	ControlButton* controlBtn = NULL;
    
	Scale9Sprite* btnNormal = Scale9Sprite::create(btnBgName);
	Scale9Sprite* btnSelected = Scale9Sprite::create(btnBgName);
	Scale9Sprite* btnHighlighted = Scale9Sprite::create(btnBgName);
	btnHighlighted->setColor(ccc3(131,131,131));
	Scale9Sprite* btnDisabled = Scale9Sprite::create(btnBgName);
	CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create("", "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(btnSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFile(const char* btnBgName,const char* title,CCSize preferredSize,int touchPriority)
{
	ControlButton* controlBtn = NULL;
    
	Scale9Sprite* btnNormal = Scale9Sprite::create(btnBgName);
	Scale9Sprite* btnSelected = Scale9Sprite::create(btnBgName);
	Scale9Sprite* btnHighlighted = Scale9Sprite::create(btnBgName);
	btnHighlighted->setColor(ccc3(131,131,131));
	Scale9Sprite* btnDisabled = Scale9Sprite::create(btnBgName);
	CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(preferredSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFile(const char* btnBgName,const char* title,CCSize preferredSize,ccColor3B selectColor,int touchPriority)
{
	ControlButton* controlBtn = NULL;
    
	Scale9Sprite* btnNormal = Scale9Sprite::create(btnBgName);
	Scale9Sprite* btnSelected = Scale9Sprite::create(btnBgName);
	Scale9Sprite* btnHighlighted = Scale9Sprite::create(btnBgName);
	btnHighlighted->setColor(selectColor);
	Scale9Sprite* btnDisabled = Scale9Sprite::create(btnBgName);
	//CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(preferredSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFile(const char* btnBgName,const char* title,ccColor3B selectColor,int touchPriority)
{
    ControlButton* controlBtn = NULL;
    
	Scale9Sprite* btnNormal = Scale9Sprite::create(btnBgName);
	Scale9Sprite* btnSelected = Scale9Sprite::create(btnBgName);
	Scale9Sprite* btnHighlighted = Scale9Sprite::create(btnBgName);
	btnHighlighted->setColor(selectColor);
	Scale9Sprite* btnDisabled = Scale9Sprite::create(btnBgName);
	CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(btnSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFreame(const char* btnBgName,const char* title,int touchPriority)
{
	ControlButton* controlBtn = NULL;
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
	Scale9Sprite* btnNormal = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	Scale9Sprite* btnSelected = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	Scale9Sprite* btnHighlighted = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	btnHighlighted->setColor(ccc3(131,131,131));
	Scale9Sprite* btnDisabled = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(btnSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFreame(const char* btnBgName,const char* title,CCSize preferredSize,int touchPriority)
{
	ControlButton* controlBtn = NULL;
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
	Scale9Sprite* btnNormal = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	Scale9Sprite* btnSelected = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	Scale9Sprite* btnHighlighted = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	btnHighlighted->setColor(ccc3(131,131,131));
	Scale9Sprite* btnDisabled = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	//CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(preferredSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFreame(const char* btnBgName,const char* title,CCSize preferredSize,ccColor3B selectColor,int touchPriority)
{
    ControlButton* controlBtn = NULL;
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
	Scale9Sprite* btnNormal = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	Scale9Sprite* btnSelected = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	Scale9Sprite* btnHighlighted = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	btnHighlighted->setColor(selectColor);
	Scale9Sprite* btnDisabled = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	//CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(preferredSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFreame(const char* btnBgName,const char* title,ccColor3B selectColor,int touchPriority)
{
    ControlButton* controlBtn = NULL;
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
	Scale9Sprite* btnNormal = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	Scale9Sprite* btnSelected = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	Scale9Sprite* btnHighlighted = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	btnHighlighted->setColor(selectColor);
	Scale9Sprite* btnDisabled = Scale9Sprite::createWithSpriteFrame(frameCache->spriteFrameByName(btnBgName));
	CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(btnSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFreame(CCSpriteFrame* sf,const char* title,ccColor3B selectColor,int touchPriority)
{
    ControlButton* controlBtn = NULL;
    
	Scale9Sprite* btnNormal = Scale9Sprite::createWithSpriteFrame(sf);
	Scale9Sprite* btnSelected = Scale9Sprite::createWithSpriteFrame(sf);
	Scale9Sprite* btnHighlighted = Scale9Sprite::createWithSpriteFrame(sf);
	btnHighlighted->setColor(selectColor);
	Scale9Sprite* btnDisabled = Scale9Sprite::createWithSpriteFrame(sf);
	CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(btnSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFreame(CCSpriteFrame* sf,const char* title,int touchPriority)
{
    ControlButton* controlBtn = NULL;
    
	Scale9Sprite* btnNormal = Scale9Sprite::createWithSpriteFrame(sf);
	Scale9Sprite* btnSelected = Scale9Sprite::createWithSpriteFrame(sf);
	Scale9Sprite* btnHighlighted = Scale9Sprite::createWithSpriteFrame(sf);
	btnHighlighted->setColor(ccc3(131,131,131));
	Scale9Sprite* btnDisabled = Scale9Sprite::createWithSpriteFrame(sf);
	CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", 22);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(btnSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}

ControlButton* CtrlManager::createCtrlBtnWithFreame(CCSpriteFrame* sf,const char* title,int fontSize,int touchPriority)
{
    ControlButton* controlBtn = NULL;
    
	Scale9Sprite* btnNormal = Scale9Sprite::createWithSpriteFrame(sf);
	Scale9Sprite* btnSelected = Scale9Sprite::createWithSpriteFrame(sf);
	Scale9Sprite* btnHighlighted = Scale9Sprite::createWithSpriteFrame(sf);
	btnHighlighted->setColor(ccc3(131,131,131));
	Scale9Sprite* btnDisabled = Scale9Sprite::createWithSpriteFrame(sf);
	CCSize btnSize = btnNormal->getContentSize();
	CCLabelTTF* pTitleTTF = CCLabelTTF::create(title, "Arial-BoldMT", fontSize);
    
	controlBtn = ControlButton::create(pTitleTTF, btnNormal);
    //controlBtn->setTouchPriority(touchPriority);
	controlBtn->setPreferredSize(btnSize);
	controlBtn->setZoomOnTouchDown(false);
	controlBtn->setBackgroundSpriteForState(btnNormal, Control::State::NORMAL);
	controlBtn->setBackgroundSpriteForState(btnSelected, Control::State::SELECTED);
	controlBtn->setBackgroundSpriteForState(btnHighlighted, Control::State::HIGH_LIGHTED);
	controlBtn->setBackgroundSpriteForState(btnDisabled, Control::State::DISABLED);
	controlBtn->setAnchorPoint(ccp(0.5,0.5));
    
	return controlBtn;
}







