//
//  CommonLayer.cpp
//  TexasPoker
//
//  Created by peniel on 12/28/13.
//
//

#include "CommonLayer.h"
//#include "StartScene.h"
//#include "SettingScene.h"
//#include "RmbRechargeLayer.h"
#include "PlayerModel.h"
#include "StringFormat.h"

CommonLayer::CommonLayer():
pTopNode(NULL),
//pShipIconText(NULL),
//pRmbIconText(NULL),
isChangeingRich(false),
currScene(UNKNOWN_SCENE)
{
    
}

CommonLayer::~CommonLayer()
{
	CC_SAFE_RELEASE_NULL(pTopNode);
	//CC_SAFE_RELEASE_NULL(pShipIconText);
	//CC_SAFE_RELEASE_NULL(pRmbIconText);
}

bool CommonLayer::init()
{
    bool bRect = false;
    
    do {
        CC_BREAK_IF(!BaseNode::init());
        
        bRect = true;
        
    } while (0);
    
    return  bRect;
}

void CommonLayer::addTopNode(const char* title)
{
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    CCSprite* pTopBg = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("top_bg.png"));
    CCSize topSize = pTopBg->getContentSize();
    
    pTopNode = CCNode::create();
    CC_SAFE_RETAIN(pTopNode);
    pTopNode->setContentSize(topSize);
    pTopNode->setAnchorPoint(ccp(0.0f,1.0f));
    pTopNode->setPosition(ccp(this->originPoint.x,this->originPoint.y+this->visibleH+1));
    this->addChild(pTopNode,10);
    
	pTopBg->setAnchorPoint(Vec2::ZERO);
    pTopBg->setPosition(Vec2::ZERO);
    pTopNode->addChild(pTopBg);
    
    CCLabelTTF* pTitle = CCLabelTTF::create(title,"Arial",30);
	pTitle->setAnchorPoint(ccp(0.5f,0.5f));
	pTitle->setPosition(ccp(topSize.width/2,topSize.height/2+10));
	pTopNode->addChild(pTitle);
    
    //ControlButton* backCtrlBtn = CtrlManager::createCtrlBtnWithFreame("cricle_btn.png","");
    //backCtrlBtn->setAnchorPoint(ccp(1.0f,0.5f));
    //backCtrlBtn->setPosition(ccp(topSize.width-20,topSize.height/2+10));
    //backCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(CommonLayer::backBtnCallbck), CCControlEventTouchUpInside);
    //pTopNode->addChild(backCtrlBtn,0,BACK_PREV_SCENE_TAG);
    
    //CCSprite* backIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("back_icon.png"));
    //backIcon->setAnchorPoint(ccp(0.5f,0.5f));
    //backIcon->setPosition(ccp(backCtrlBtn->getPreferredSize().width/2,backCtrlBtn->getPreferredSize().height/2));
    //backCtrlBtn->addChild(backIcon);
}

void CommonLayer::addTopNodeUserInfo()
{
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    const char* genderPicName = PlayerModel::getInstance()->sex==1 ? "def_man_header.png" : "def_wman_header.png";
    std::string headerFile = "no_custom_head";
    //CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid,headerFile);
    CCSprite* pUserHeader = NULL;
    if (headerFile.compare("no_custom_head")==0)
    {
        pUserHeader = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName(genderPicName));
    }
    else
    {
        CCSprite* pTemp = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName(genderPicName));
        CCSize tempSize = pTemp->boundingBox().size;
        
        //CCTexture2D* pHeaderTexture = CCTextureCache::sharedTextureCache()->addImage(CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid).c_str());
        //pUserHeader = CCSprite::createWithTexture(pHeaderTexture);
        //CCSize avatarSize = pHeaderTexture->getContentSize();
        //float scaleX = tempSize.width / avatarSize.width;
        //float scaleY = tempSize.height / avatarSize.height;
        //pUserHeader->setScaleX(scaleX);
        //pUserHeader->setScaleY(scaleY);
    }
    pUserHeader->setAnchorPoint(Point::ZERO);
	pUserHeader->setPosition(ccp(0,20));
	pTopNode->addChild(pUserHeader);
    
    const char* shipStr = StringFormat::intsert(PlayerModel::getInstance()->money,3,",");
	CCSprite* pShipIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("ship_icon.png"));
	//pShipIconText = IconTextSprite::createWith(pShipIcon,shipStr);
 //   CC_SAFE_RETAIN(pShipIconText);
 //   pShipIconText->setAnchorPoint(Point::ZERO);
 //   pShipIconText->setFontSize(18);
 //   pShipIconText->setPosition(ccp(pUserHeader->boundingBox().size.width+10,30));
 //   pTopNode->addChild(pShipIconText);
    
    CCLabelTTF* pUserName = CCLabelTTF::create(PlayerModel::getInstance()->name.c_str(),"Arial",18);
	pUserName->setAnchorPoint(Point::ZERO);
	//pUserName->setPosition(ccp(pShipIconText->getPositionX(),pShipIconText->getPositionY()+pShipIconText->getContentSize().height+6));
	pTopNode->addChild(pUserName);

}

void CommonLayer::addRechargeTopNode(const char* title)
{
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    //--------------------------------------------------------------------
    CCSprite* pTopBg = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("top_bg.png"));
    CCSize topSize = pTopBg->getContentSize();
    
    pTopNode = CCNode::create();
    CC_SAFE_RETAIN(pTopNode);
    pTopNode->setContentSize(topSize);
    pTopNode->setAnchorPoint(ccp(0.0f,1.0f));
    pTopNode->setPosition(ccp(this->originPoint.x,this->originPoint.y+this->visibleH+1));
    this->addChild(pTopNode,10);
    
    pTopBg->setAnchorPoint(Point::ZERO);
    pTopBg->setPosition(Point::ZERO);
    pTopNode->addChild(pTopBg);
    
    CCLabelTTF* pTitle = CCLabelTTF::create(/*title*/"","Arial",30);
	pTitle->setAnchorPoint(ccp(0.5f,0.5f));
	pTitle->setPosition(ccp(topSize.width/2,topSize.height/2+10));
	pTopNode->addChild(pTitle);
    
    //ControlButton* backCtrlBtn = CtrlManager::createCtrlBtnWithFreame("cricle_btn.png","");
    //backCtrlBtn->setAnchorPoint(ccp(1.0f,0.5f));
    //backCtrlBtn->setPosition(ccp(topSize.width-20,topSize.height/2+10));
    //backCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(CommonLayer::backBtnCallbck), CCControlEventTouchUpInside);
    //pTopNode->addChild(backCtrlBtn,0,BACK_PREV_SCENE_TAG);
    
    CCSprite* backIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("back_icon.png"));
    backIcon->setAnchorPoint(ccp(0.5f,0.5f));
    //backIcon->setPosition(ccp(backCtrlBtn->getPreferredSize().width/2,backCtrlBtn->getPreferredSize().height/2));
    //backCtrlBtn->addChild(backIcon);
    //---------------------------------------------------------------------
    const char* genderPicName = PlayerModel::getInstance()->sex==1 ? "def_man_header.png" : "def_wman_header.png";
    std::string headerFile = "no_custom_head";
    //CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid,headerFile);
    CCSprite* pUserHeader = NULL;
    if (headerFile.compare("no_custom_head")==0)
    {
        pUserHeader = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName(genderPicName));
    }
    else
    {
        CCSprite* pTemp = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName(genderPicName));
        CCSize tempSize = pTemp->boundingBox().size;
        
        //CCTexture2D* pHeaderTexture = CCTextureCache::sharedTextureCache()->addImage(CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid).c_str());
        //pUserHeader = CCSprite::createWithTexture(pHeaderTexture);
        //CCSize avatarSize = pHeaderTexture->getContentSize();
        //float scaleX = tempSize.width / avatarSize.width;
        //float scaleY = tempSize.height / avatarSize.height;
        //pUserHeader->setScaleX(scaleX);
        //pUserHeader->setScaleY(scaleY);
    }
    pUserHeader->setAnchorPoint(Point::ZERO);
	pUserHeader->setPosition(ccp(0,20));
	pTopNode->addChild(pUserHeader);
    
    const char* shipStr = StringFormat::intsert(PlayerModel::getInstance()->money,3,",");
	CCSprite* pShipIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("ship_icon.png"));
	//pShipIconText = IconTextSprite::createWith(pShipIcon,shipStr);
 //   CC_SAFE_RETAIN(pShipIconText);
 //   pShipIconText->setAnchorPoint(Point::ZERO);
 //   pShipIconText->setFontSize(18);
 //   pShipIconText->setPosition(ccp(pUserHeader->boundingBox().size.width+10,30));
 //   pTopNode->addChild(pShipIconText);
    
    const char* rmbStr = StringFormat::intsert(PlayerModel::getInstance()->rmb,3,",");
	CCSprite* pRmbIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("gold_icon.png"));
	//pRmbIconText = IconTextSprite::createWith(pRmbIcon,rmbStr);
 //   CC_SAFE_RETAIN(pRmbIconText);
 //   pRmbIconText->setAnchorPoint(Point::ZERO);
 //   pRmbIconText->setFontSize(18);
 //   pRmbIconText->setPosition(ccp(pShipIconText->getPositionX()+pShipIconText->getContentSize().width+20,33));
 //   pTopNode->addChild(pRmbIconText);
    
    CCLabelTTF* moneyPercent = CCLabelTTF::create("1元 = 1钻石 = 5000筹码","Arial",18);
    //moneyPercent->setColor(ccc3(255, 0, 0));
	moneyPercent->setAnchorPoint(Point::ZERO);
	//moneyPercent->setPosition(ccp(this->visibleW/2,pRmbIconText->getPositionY()-4));
	pTopNode->addChild(moneyPercent);
    
    CCLabelTTF* pUserName = CCLabelTTF::create(PlayerModel::getInstance()->name.c_str()/*"恋上你的床"*/,"Arial",22);
	pUserName->setAnchorPoint(Point::ZERO);
	//pUserName->setPosition(ccp(pShipIconText->getPositionX(),pShipIconText->getPositionY()+pShipIconText->getContentSize().height+6));
	pTopNode->addChild(pUserName);
}

CCNode* CommonLayer::getTopNode()
{
    return this->pTopNode;
}

void CommonLayer::setCurrScene(SCENE currScene)
{
    this->currScene = currScene;
}

void CommonLayer::onEnterTransitionDidFinish()
{
    //务必调用
    Layer::onEnterTransitionDidFinish();
}

void CommonLayer::backBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event)
{
    
    Sound::getInstance()->playEffect("sound/sound_dianji.ogg");
    
 //   if (currScene == UNKNOWN_SCENE)
 //   {
 //       CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, StartScene::scene(),ccBLACK);
 //       CCDirector::sharedDirector()->replaceScene(reScene);
 //   }
	//else if (currScene == SETTING_SCENE)
 //   {
 //       CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, SettingScene::scene(),ccBLACK);
 //       CCDirector::sharedDirector()->replaceScene(reScene);
 //   }
}

void CommonLayer::onEnter()
{
	Layer::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(CommonLayer::notifiUpdateRichCallback),NOTIFI_UPDATE_RICHINFO_KEY,NULL);
}

void CommonLayer::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTIFI_UPDATE_RICHINFO_KEY);
	Layer::onExit();
}

void CommonLayer::notifiUpdateRichCallback(CCObject* pCCObjData)
{
    //DLog::showLog("CommonLayer scene heart beat callback");
    
    this->updateRich();
}

void CommonLayer::updateTopNodeRich()
{
    //if (pRmbIconText != NULL)
    //{
    //    const char* rmbStr = StringFormat::intsert(PlayerModel::getInstance()->rmb,3,",");
    //    pRmbIconText->setText(rmbStr);
    //}
    //
    //if (pShipIconText != NULL)
    //{
    //    const char* shipStr = StringFormat::intsert(PlayerModel::getInstance()->money,3,",");
    //    pShipIconText->setText(shipStr);
    //}
    
}

void CommonLayer::updateRich()
{
    if (isChangeingRich)
    {
        return;
    }

    //if (pShipIconText != NULL)
    //{
    //    const char* shipStr = StringFormat::intsert(PlayerModel::getInstance()->money,3,",");
    //    pShipIconText->setText(shipStr);
    //}
    //
    //if (pRmbIconText != NULL)
    //{
    //    const char* rmbStr = StringFormat::intsert(PlayerModel::getInstance()->rmb,3,",");
    //    pRmbIconText->setText(rmbStr);
    //    pRmbIconText->setAnchorPoint(Point::ZERO);
    //    pRmbIconText->setPosition(ccp(pShipIconText->getPositionX()+pShipIconText->getContentSize().width+20,33));
    //}

}

void CommonLayer::isChangeingRichCallback(float dt)
{
    isChangeingRich = false;
}
