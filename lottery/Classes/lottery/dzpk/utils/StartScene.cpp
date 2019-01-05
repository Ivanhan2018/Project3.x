#include "StartScene.h"
#include "PlayerModel.h"
#include "UserInfoScene.h"
#include "StringFormat.h"
#include "DzConfig.h"
#include "Venue.h"
//#include "VenueListLayer.h"
//#include "RankScene.h"
//#include "MailScene.h"
//#include "DropGoldLayer.h"
//#include "RechargeScene.h"
//#include "SettingScene.h"
//#include "GuaGuaMachine.h"
//#include "TcpHornClient.h"
//#include "HornLayer.h"
//#include "RoomInfo.h"

//#include "GlobalHttpReq.h"

StartScene::StartScene():
isAutoLgoin(true),
//pAnimLoading(NULL),
pUserHeader(NULL),
pUserName(NULL),
//pShipIconText(NULL),
//pRmbIconText(NULL),
iskeyBackClicked(false),
isMiniGameing(false),
pContentNode(NULL),
isClickCtrl(true),
safeClickTime(0.2f),
loadingAnimDelay(1.0f),
pHallNode(NULL),
pQuickGameNode(NULL),
isVenueListShow(false),
isTigetMachineShow(false),
isGuagualeMachineShow(false),
isWeixinShareShow(false)
{
    
}

StartScene::~StartScene()
{
	//CC_SAFE_RELEASE_NULL(pAnimLoading);
	CC_SAFE_RELEASE_NULL(pUserHeader);
	CC_SAFE_RELEASE_NULL(pUserName);
	//CC_SAFE_RELEASE_NULL(pShipIconText);
	//CC_SAFE_RELEASE_NULL(pRmbIconText);
	CC_SAFE_RELEASE_NULL(pContentNode);
	CC_SAFE_RELEASE_NULL(pHallNode);
	CC_SAFE_RELEASE_NULL(pQuickGameNode);
}

bool StartScene::init()
{
    bool bRect = false;

    do {
        CC_BREAK_IF(!BaseNode::init());

        this->initUI();

        bRect = true;

    } while (0);

    return  bRect;
}

void StartScene::initUI()
{
    //this->setBackground(this->pStartBgTexture);
    
    pContentNode = CCNode::create();
    CC_SAFE_RETAIN(pContentNode);
    pContentNode->setContentSize(CCSizeMake(this->visibleW, this->visibleH));
    pContentNode->setAnchorPoint(Vec2::ZERO);
    pContentNode->setPosition(this->originPoint);
    this->addChild(pContentNode);
    
    CCSprite* pBgSprite = CCSprite::createWithTexture(pStartBgTexture);
    CCSize bgSpriteSize = pBgSprite->getContentSize();
    pBgSprite->setScaleY((float)this->visibleH/(float)bgSpriteSize.height);
	pBgSprite->setAnchorPoint(Vec2::ZERO);
	pBgSprite->setPosition(Vec2::ZERO);
	pContentNode->addChild(pBgSprite);
    
    if (PlayerModel::getInstance()->is_login_succ)
    {
        isClickCtrl = false;
        
        this->noRunLoadingAnim();
    }
    
}

void StartScene::onEnterTransitionDidFinish()
{
    if (!PlayerModel::getInstance()->is_login_succ)
    {
        //pAnimLoading = AnimLoading::create();
        //CC_SAFE_RETAIN(pAnimLoading);
        //pAnimLoading->setAnchorPoint(Vec2::ZERO);
        //pAnimLoading->setPosition(this->originPoint);
        //this->addChild(pAnimLoading,50);
        
        std::string playerAcount = CCUserDefault::sharedUserDefault()->getStringForKey(PLAYER_ACOUNT_KEY,"");
        if (playerAcount=="")
        {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            std::string accountStr = getUserNameOrPwdWithSDcard(0);
            if ( accountStr.compare("null") != 0 )
            {
                isAutoLgoin = true;
            }
            else
            {
                isAutoLgoin = false;
            }
        #else
            isAutoLgoin = false;
        #endif
        }
        else
        {
            isAutoLgoin = true;
        }
        this->registerOrLogin();
    }
    
    //务必调用
    Layer::onEnterTransitionDidFinish();
    
	CCTextureCache::sharedTextureCache()->addImage("background/other_bg.png");
}

void StartScene::ctrlBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event)
{
    Sound::getInstance()->playEffect("sound/sound_dianji.ogg");
    
    ControlButton* ctrlBtn = (ControlButton*)pSender;
    UNS_INT ctrlBtnTag = ctrlBtn->getTag();
    //DLog::showLog("ctrl btn tag: %d",ctrlBtnTag);
    
    if (isClickCtrl)
    {
        return;
    }
    isClickCtrl = true;
    this->scheduleOnce(schedule_selector(StartScene::clickCtrlBtnCallback), safeClickTime);
    
    if (ctrlBtnTag == PLAYERINFO_CONTRL_TAG)
	{
        CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, UserInfoScene::scene(),Color3B::BLACK);
        CCDirector::sharedDirector()->replaceScene(reScene);
	}
	else if(ctrlBtnTag == QUICK_GAME_CONTRL_TAG)
	{
        this->quickStart();
	}
	else if(ctrlBtnTag == SETTINGS_CONTRL_TAG)
	{
        //CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, SettingScene::scene(),ccBLACK);
        //CCDirector::sharedDirector()->replaceScene(reScene);
	}
	else if(ctrlBtnTag == GAME_ACTIVITY_CONTRL_TAG)
	{
		//CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, ActiveScene::scene(),ccBLACK);
  //      CCDirector::sharedDirector()->replaceScene(reScene);
	}
	else if(ctrlBtnTag == MINI_GAME_CONTRL_TAG)
	{
  //      pContentNode->runAction(CCMoveTo::create(0.3f, CCPointMake(this->originPoint.x-161, this->originPoint.y)));
		//this->addChild(this->createMiniGameLayer(),100);
	}
	else if(ctrlBtnTag == HALL_CONTRL_TAG)
	{
        this->clickHall(false);
	}
	else if(ctrlBtnTag == RANKING_CONTRL_TAG)
	{
		//CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, RankScene::scene(),ccBLACK);
  //      CCDirector::sharedDirector()->replaceScene(reScene);
	}
	else if(ctrlBtnTag == EMAIL_CONTRL_TAG)
	{
		//CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, MailScene::scene(),ccBLACK);
  //      CCDirector::sharedDirector()->replaceScene(reScene);
	}
	else if(ctrlBtnTag == RECHARGE_CONTRL_TAG)
	{
		//CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, RechargeScene::scene(),ccBLACK);
  //      CCDirector::sharedDirector()->replaceScene(reScene);
	}
    else if(ctrlBtnTag == SHARE_BTN_TAG)
    {
        //this->addChild(this->createWeixinShareLayer(),100,WEIXIN_SHARE_LAYER_TAG);
    }
}

void StartScene::clickCtrlBtnCallback(float dt)
{
    isClickCtrl = false;
}

void StartScene::runLoadingAnim()
{
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
 //   ControlButton* shopCtrlBtn = CtrlManager::createCtrlBtnWithFreame("shop_icon.png","");
	//shopCtrlBtn->setAnchorPoint(ccp(0.0f,1.0f));
	//shopCtrlBtn->setPosition(ccp(0,0+this->visibleH+shopCtrlBtn->getPreferredSize().height));
	//shopCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//this->pContentNode->addChild(shopCtrlBtn,1,RECHARGE_CONTRL_TAG);
    
    CCParticleSystem* ignore1 = CCParticleSystemQuad::create("start/starEffect.plist");
    ignore1->setPosition(ccp(25,80));
    ignore1->setPositionType(kCCPositionTypeRelative);
    //shopCtrlBtn->addChild(ignore1);
    
    CCParticleSystem* ignore2 = CCParticleSystemQuad::create("start/starEffect.plist");
    ignore2->setPosition(ccp(100,75));
    ignore1->setPositionType(kCCPositionTypeRelative);
    //shopCtrlBtn->addChild(ignore2);
    
    CCParticleSystem* ignore3 = CCParticleSystemQuad::create("start/starEffect.plist");
    ignore3->setPosition(ccp(150,150));
    ignore1->setPositionType(kCCPositionTypeRelative);
    //shopCtrlBtn->addChild(ignore3);
    
    CCPoint endPoint = ccp(0,0+this->visibleH+1);
    //shopCtrlBtn->runAction(CCSequence::create(CCEaseBounceOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------
    //ControlButton* quickGameCtrlBtn = CtrlManager::createCtrlBtnWithFreame("quick_game_icon.png","");
    
    pQuickGameNode = CCNode::create();
    CC_SAFE_RETAIN(pQuickGameNode);
    //pQuickGameNode->setContentSize(quickGameCtrlBtn->getPreferredSize());
    pQuickGameNode->setAnchorPoint(ccp(1.0f,0.0f));
	//pQuickGameNode->setPosition(ccp(0+this->visibleW+quickGameCtrlBtn->getPreferredSize().width,0+30));
	this->pContentNode->addChild(pQuickGameNode);
    
	//quickGameCtrlBtn->setAnchorPoint(Vec2::ZERO);
	//quickGameCtrlBtn->setPosition(Vec2::ZERO);
	//quickGameCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//pQuickGameNode->addChild(quickGameCtrlBtn,0,QUICK_GAME_CONTRL_TAG);
    
    CCSprite* quickText = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("quick_game_text.png"));
    quickText->setAnchorPoint(ccp(0.5f,0.5f));
    quickText->setPosition(ccp(92,111));
    pQuickGameNode->addChild(quickText,2);
    
    CCSprite* quickTextFrame = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("quick_game_rotate_1.png"));
    quickTextFrame->setAnchorPoint(ccp(0.5f,0.5f));
    quickTextFrame->setPosition(ccp(92,111));
    pQuickGameNode->addChild(quickTextFrame,1);
    //动画
    endPoint = ccp(0+this->visibleW-5,0+30);
    pQuickGameNode->runAction(CCSequence::create(CCDelayTime::create(loadingAnimDelay+0.3f),CCEaseBounceOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    
    quickText->runAction(CCRepeatForever::create(CCRotateBy::create(8.0f, 360)));
    
    //CCArray* pAnimArray = CCArray::create();
	Vector<SpriteFrame*> animFrames;
    int k = 1;
    for (int i=1; i<=16; ++i)
    {
        CCString* file = CCString::createWithFormat("quick_game_rotate_%d.png",k);
        //pAnimArray->addObject(frameCache->spriteFrameByName(file->getCString()));
		animFrames.pushBack(frameCache->spriteFrameByName(file->getCString()));
        if (i>8)
        {
            --k;
        }
        else if (i<8)
        {
            ++k;
        }
        else
        {
            for (int j=0; j<6; ++j)
            {
                //pAnimArray->addObject(frameCache->spriteFrameByName("quick_game_rotate_8.png"));
				animFrames.pushBack(frameCache->spriteFrameByName("quick_game_rotate_8.png"));
            }
        }
    }
    //CCAnimation* pRotateAnim = CCAnimation::createWithSpriteFrames(pAnimArray, 0.15f);
	CCAnimation* pRotateAnim = CCAnimation::createWithSpriteFrames(animFrames, 0.15f);
    CCFiniteTimeAction* rotateAction = CCAnimate::create(pRotateAnim);
    quickTextFrame->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(2.0f),rotateAction,NULL)));
    //------------------------------------------------------------------------
    CCSprite* pLBottomBg = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("left_bottom_bg.png"));
    CCSize lbottomSize = pLBottomBg->getContentSize();
    
    CCNode* pLBottomNode = CCNode::create();
    pLBottomNode->setContentSize(lbottomSize);
    pLBottomNode->setAnchorPoint(Vec2::ZERO);
    pLBottomNode->setPosition(ccp(0-lbottomSize.width/2,0-lbottomSize.height/2));
    this->pContentNode->addChild(pLBottomNode,0,LEFT_BOTTOM_NODE_TAG);
    
    pLBottomBg->setAnchorPoint(Vec2::ZERO);
    pLBottomBg->setPosition(Vec2::ZERO);
    pLBottomNode->addChild(pLBottomBg);
    
    //ControlButton* rankCtrlBtn = CtrlManager::createCtrlBtnWithFreame("cricle_btn.png","");
    //rankCtrlBtn->setAnchorPoint(Vec2::ZERO);
    //rankCtrlBtn->setPosition(ccp(lbottomSize.width/2-rankCtrlBtn->getPreferredSize().width*1.2f,0));
    //rankCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
    //pLBottomNode->addChild(rankCtrlBtn,0,RANKING_CONTRL_TAG);
    
    CCSprite* rankIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("rank_icon.png"));
    rankIcon->setAnchorPoint(ccp(0.5f,0.5f));
    //rankIcon->setPosition(ccp(rankCtrlBtn->getPreferredSize().width/2,rankCtrlBtn->getPreferredSize().height/2));
    //rankCtrlBtn->addChild(rankIcon);
    
    //ControlButton* settingCtrlBtn = CtrlManager::createCtrlBtnWithFreame("cricle_btn.png","");
    //settingCtrlBtn->setAnchorPoint(Vec2::ZERO);
    //settingCtrlBtn->setPosition(ccp(0,rankCtrlBtn->getPositionY()+settingCtrlBtn->getPreferredSize().height-10));
    //settingCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
    //pLBottomNode->addChild(settingCtrlBtn,0,SETTINGS_CONTRL_TAG);
    
    CCSprite* settingIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("setting_icon.png"));
    settingIcon->setAnchorPoint(ccp(0.5f,0.5f));
    //settingIcon->setPosition(ccp(settingCtrlBtn->getPreferredSize().width/2,settingCtrlBtn->getPreferredSize().height/2));
    //settingCtrlBtn->addChild(settingIcon);
    
    endPoint = Vec2::ZERO;
    pLBottomNode->runAction(CCSequence::create(CCDelayTime::create(loadingAnimDelay+0.6f),CCEaseBounceOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------
 //   ControlButton* miniCtrlBtn = CtrlManager::createCtrlBtnWithFreame("mini_icon.png","");
	//miniCtrlBtn->setAnchorPoint(ccp(1.0f,1.0f));
	//miniCtrlBtn->setPosition(ccp(0+this->visibleW,0+this->visibleH+miniCtrlBtn->getPreferredSize().height));
	//miniCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//this->pContentNode->addChild(miniCtrlBtn,0,MINI_GAME_CONTRL_TAG);
    
    endPoint = ccp(0+this->visibleW,0+this->visibleH);
    //miniCtrlBtn->runAction(CCSequence::create(CCDelayTime::create(loadingAnimDelay+0.9f),CCEaseBounceOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------
    CCSprite* pMsgBg = CCSprite::create("msg/msg_bg.png");
    CCSize msgBgSize = pMsgBg->getContentSize();
 //   HornLayer* pHornLayer = HornLayer::createWith(CCPointMake((this->visibleW-miniCtrlBtn->getPreferredSize().width)/2+shopCtrlBtn->getPreferredSize().width/2-msgBgSize.width/2, this->visibleH-msgBgSize.height-20));
 //   pHornLayer->setAnchorPoint(Vec2::ZERO);
	//pHornLayer->setPosition(ccp(0,0+msgBgSize.height+20));
	//this->pContentNode->addChild(pHornLayer,20);
    
    endPoint = Vec2::ZERO;
    //pHornLayer->runAction(CCSequence::create(CCDelayTime::create(loadingAnimDelay+1.2f),CCEaseBounceOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------
 //   ControlButton* noticeCtrlBtn = CtrlManager::createCtrlBtnWithFreame("notice_icon.png","");
	//noticeCtrlBtn->setAnchorPoint(ccp(0.0f,1.0f));
	//noticeCtrlBtn->setPosition(ccp(0-noticeCtrlBtn->getPreferredSize().width,0+this->visibleH-shopCtrlBtn->getPreferredSize().height+10));
	//noticeCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//this->pContentNode->addChild(noticeCtrlBtn,0,GAME_ACTIVITY_CONTRL_TAG);
    
    //endPoint = ccp(0+shopCtrlBtn->getPreferredSize().width/2-noticeCtrlBtn->getPreferredSize().width/2,0+this->visibleH*0.6f);
    //noticeCtrlBtn->runAction(CCSequence::create(CCDelayTime::create(loadingAnimDelay+1.5f),CCEaseBounceOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------
    CCSprite* hallFrame = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("hall_icon_frame.png"));
    CCSize hallSize = hallFrame->getContentSize();
    
    pHallNode = CCNode::create();
    CC_SAFE_RETAIN(pHallNode);
    pHallNode->setContentSize(hallSize);
    pHallNode->setAnchorPoint(ccp(0.5f,0.5f));
	pHallNode->setPosition(ccp(0+this->visibleW/2,0+this->visibleH*2.0f));
	this->pContentNode->addChild(pHallNode);
    
 //   ControlButton* hallCtrlBtn = CtrlManager::createCtrlBtnWithFreame("hall_icon.png","");
	//hallCtrlBtn->setAnchorPoint(ccp(0.5f,0.5f));
	//hallCtrlBtn->setPosition(ccp(hallSize.width/2,hallSize.height/2));
	//hallCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//pHallNode->addChild(hallCtrlBtn,1,HALL_CONTRL_TAG);
    
	hallFrame->setAnchorPoint(Vec2::ZERO);
	hallFrame->setPosition(Vec2::ZERO);
	pHallNode->addChild(hallFrame,3,HALL_CONTRL_TAG+1);
    
    endPoint = ccp(0+this->visibleW/2,0+this->visibleH/2+22);
    pHallNode->runAction(CCSequence::create(CCDelayTime::create(loadingAnimDelay+1.8f),CCMoveTo::create(0.0f,endPoint),CCRepeat::create(CCFadeIn::create(0.5f),1),NULL));
    hallFrame->runAction(CCSequence::create(CCDelayTime::create(loadingAnimDelay+1.8f),CCRepeat::create(CCFadeIn::create(0.5f),1),NULL));
    //hallCtrlBtn->runAction(CCSequence::create(CCDelayTime::create(loadingAnimDelay+1.8f),CCRepeat::create(CCFadeIn::create(0.5f),1),NULL));
}

void StartScene::showUserInfo()
{
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    //------------------------------------------------------------------------
 //   ClipLayer* pBestCardsLayer = ClipLayer::create(pHallNode->getContentSize());
 //   pBestCardsLayer->setAnchorPoint(Vec2::ZERO);
	//pBestCardsLayer->setPosition(ccp(20,15));
 //   pHallNode->addChild(pBestCardsLayer,2,HALL_CONTRL_TAG+2);
    
    int rotate[5] = {-15,0,15,30,45};
    std::vector<CCPoint> cardPoints;
    cardPoints.push_back(CCPointMake(45, -5));
    cardPoints.push_back(CCPointMake(50, -10));
    cardPoints.push_back(CCPointMake(55, -15));
    cardPoints.push_back(CCPointMake(60, -20));
    cardPoints.push_back(CCPointMake(65, -25));
    for (int i=0;i<5;++i)
	{
        CCString* cardStr = CCString::createWithFormat("rooms/card/card_%s.png",PlayerModel::getInstance()->best_cards[i].c_str());
		CCSprite* cardSprite = CCSprite::create(cardStr->getCString());
        cardSprite->setScale(0.8f);
		cardSprite->setAnchorPoint(ccp(0.5f,0.0f));
		cardSprite->setPosition(CCPointMake(0, -100));
        cardSprite->setRotation(90);
		//pBestCardsLayer->addChild(cardSprite,2,i);
        
        cardSprite->runAction(CCSequence::create(CCDelayTime::create((i+1)*0.2f),CCMoveTo::create(0.0f, *(cardPoints.begin()+i)),CCRotateTo::create(0.5f, rotate[i]),NULL));
	}
    //------------------------------------------------------------------------
    CCSprite* pUserInfoFrame = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("line_frame.png"));
    CCSize userInfoFrameSize = pUserInfoFrame->getContentSize();
    pUserInfoFrame->setAnchorPoint(Vec2::ZERO);
    pUserInfoFrame->setPosition(ccp(0+261,0-1));
    this->pContentNode->addChild(pUserInfoFrame,0,USER_INFO_FRAME_TAG);
    
    CCNode* pUserInfoNode = CCNode::create();
    pUserInfoNode->setContentSize(userInfoFrameSize);
    pUserInfoNode->setAnchorPoint(Vec2::ZERO);
    pUserInfoNode->setPosition(ccp(0+261,0-1-userInfoFrameSize.height));
    this->pContentNode->addChild(pUserInfoNode,0,USER_INFO_NODE_TAG);
    
 //   ControlButton* eamilCtrlBtn = CtrlManager::createCtrlBtnWithFreame("email_icon.png","");
	//eamilCtrlBtn->setAnchorPoint(Vec2::ZERO);
	//eamilCtrlBtn->setPosition(ccp(37,25));
	//eamilCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//pUserInfoNode->addChild(eamilCtrlBtn,0,EMAIL_CONTRL_TAG);
    
    const char* lvStr = CCString::createWithFormat("LV: %d",PlayerModel::getInstance()->getLevelByExp(PlayerModel::getInstance()->exp))->getCString();
    CCLabelTTF* pLVText = CCLabelTTF::create(lvStr, "Arial", 18);
    pLVText->setAnchorPoint(ccp(0.5f,0.5f));
    //pLVText->setPosition(ccp(eamilCtrlBtn->getPositionX()+eamilCtrlBtn->getPreferredSize().width/2,10));
    pUserInfoNode->addChild(pLVText);
    
 //   ControlButton* userInfoCtrlBtn = CtrlManager::createCtrlBtnWithFreame("start_header_frame.png","");
 //   userInfoCtrlBtn->setAnchorPoint(Vec2::ZERO);
	//userInfoCtrlBtn->setPosition(ccp(eamilCtrlBtn->getPositionX()+eamilCtrlBtn->getPreferredSize().width+20,0));
	//userInfoCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//pUserInfoNode->addChild(userInfoCtrlBtn,0,PLAYERINFO_CONTRL_TAG);
    
    const char* genderPicName = PlayerModel::getInstance()->sex==1 ? "def_man_header.png" : "def_wman_header.png";
    std::string headerFile = "no_custom_head";
    //CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid,headerFile);
    if (headerFile.compare("no_custom_head")==0)
    {
        pUserHeader = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName(genderPicName));
        pUserHeader->setAnchorPoint(Vec2::ZERO);
        //userHeaderPoint = ccp(userInfoCtrlBtn->getPositionX()+userInfoCtrlBtn->getPreferredSize().width/2-pUserHeader->getContentSize().width/2-1,userInfoCtrlBtn->getPositionY()+userInfoCtrlBtn->getPreferredSize().height/2-pUserHeader->getContentSize().height/2-3);
        pUserHeader->setPosition(userHeaderPoint);
    }
    else
    {
        CCSprite* pTemp = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName(genderPicName));
        CCSize tempSize = pTemp->boundingBox().size;
        tempSize = CCSizeMake(tempSize.width-4, tempSize.height-4);
        
        //CCTexture2D* pHeaderTexture = CCTextureCache::sharedTextureCache()->addImage(CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid).c_str());
        //pUserHeader = CCSprite::createWithTexture(pHeaderTexture);
        //pUserHeader->setContentSize(tempSize);
        //CCSize avatarSize = pHeaderTexture->getContentSize();
        //float scaleX = tempSize.width / avatarSize.width;
        //float scaleY = tempSize.height / avatarSize.height;
        //pUserHeader->setScaleX(scaleX);
        //pUserHeader->setScaleY(scaleY);
        //
        //pUserHeader->setAnchorPoint(Vec2::ZERO);
        //userHeaderPoint = ccp(userInfoCtrlBtn->getPositionX()+userInfoCtrlBtn->getPreferredSize().width/2-pUserHeader->getContentSize().width/2,userInfoCtrlBtn->getPositionY()+userInfoCtrlBtn->getPreferredSize().height/2-pUserHeader->getContentSize().height/2-2);
        //pUserHeader->setPosition(userHeaderPoint);
    }
    CC_SAFE_RETAIN(pUserHeader);
	pUserInfoNode->addChild(pUserHeader);
    
    pUserName = CCLabelTTF::create(PlayerModel::getInstance()->name.c_str(),"Arial",18);
    CC_SAFE_RETAIN(pUserName);
	pUserName->setAnchorPoint(Vec2::ZERO);
	//pUserName->setPosition(ccp(userInfoCtrlBtn->getPositionX()+userInfoCtrlBtn->getPreferredSize().width+10,userInfoCtrlBtn->getPositionY()+userInfoCtrlBtn->getPreferredSize().height-pUserName->getContentSize().height-3));
	pUserInfoNode->addChild(pUserName,0,PLAYER_NAME_TAG);
    
    const char* shipStr = StringFormat::intsert(PlayerModel::getInstance()->money,3,",");
	const char* rmbStr = StringFormat::intsert(PlayerModel::getInstance()->rmb,3,",");
	CCSprite* pShipIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("ship_icon.png"));
	CCSprite* pRMBIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("gold_icon.png"));
	//pShipIconText = IconTextSprite::createWith(pShipIcon,shipStr);
	//pRmbIconText = IconTextSprite::createWith(pRMBIcon,rmbStr);
	//CC_SAFE_RETAIN(pShipIconText);
	//CC_SAFE_RETAIN(pRmbIconText);
    
    //pRmbIconText->setAnchorPoint(Vec2::ZERO);
    //pRmbIconText->setFontSize(18);
    //pRmbIconText->setPosition(ccp(pUserName->getPositionX()+2,pUserName->getPositionY()-pShipIconText->getContentSize().height-2));
    //pUserInfoNode->addChild(pRmbIconText);
    //
    //pShipIconText->setAnchorPoint(Vec2::ZERO);
    //pShipIconText->setFontSize(18);
    //pShipIconText->setPosition(ccp(pUserName->getPositionX()+2,pRmbIconText->getPositionY()-pShipIconText->getContentSize().height-5));
    //pUserInfoNode->addChild(pShipIconText);
    
    CCPoint endPoint = ccp(0+261,0-1);
    pUserInfoNode->runAction(CCSequence::create(CCMoveTo::create(1.0f,endPoint),NULL));
    //------------------------------------------------------------------------
    CCLabelTTF* pOnlineText = CCLabelTTF::create(CCString::createWithFormat("在线: %ld",DzConfig::getInstance()->online_count)->getCString(), "Arial", 18);
    pOnlineText->setAnchorPoint(Vec2::ZERO);
    pOnlineText->setPosition(ccp(0+264+userInfoFrameSize.width+15,0-userInfoFrameSize.height));
    this->pContentNode->addChild(pOnlineText,0,ONLINE_TEXT_TAG);
    
    endPoint = ccp(0+264+userInfoFrameSize.width+15,0);
    //pOnlineText->runAction(CCSequence::create(CCEaseBounceOut::create(CCMoveTo::create(animDuration,endPoint)),NULL));
    pOnlineText->runAction(CCSequence::create(CCMoveTo::create(1.0f,endPoint),NULL));
    
    //------------------------------------------------------------------------
 //   ControlButton* shareCtrlBtn = CtrlManager::createCtrlBtnWithFile("share/weixin_icon.png","");
	//shareCtrlBtn->setAnchorPoint(ccp(0.0f,0.0f));
	//shareCtrlBtn->setPosition(ccp(160,40));
	//shareCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//this->pContentNode->addChild(shareCtrlBtn,0,SHARE_BTN_TAG);
 //   shareCtrlBtn->runAction(CCSequence::create(CCDelayTime::create(0.0f),CCRepeat::create(CCFadeIn::create(1.0f),1),NULL));
    //------------------------------------------------------------------------
    //VenueListLayer* pVenueListLayer = VenueListLayer::create();
    //pVenueListLayer->setAnchorPoint(Vec2::ZERO);
    //pVenueListLayer->setPosition(ccp(this->visibleW+1,0));
    //this->pContentNode->addChild(pVenueListLayer,0,VENUE_LIST_TAG);
    //------------------------------------------------------------------------
    //是否领取了连续登陆奖励
    if (!PlayerModel::getInstance()->is_get_login_packer)
    {
        this->scheduleOnce(schedule_selector(StartScene::dailyLoginCallback), 1.1f);
    }
    else if(PlayerModel::getInstance()->solt_num>0)
    {
        this->scheduleOnce(schedule_selector(StartScene::dailyLoginShowTiggerCallback), 1.1f);
    }
    else
    {
        //破产领取
        PlayerModel::getInstance()->getBrokeChip();
    }
    
    //动画没加载完成之前，不能点击
    this->scheduleOnce(schedule_selector(StartScene::clickCtrlBtnCallback), 2.0f);
    
    PlayerModel::getInstance()->is_login_succ = true;
    
    //启动全局定时器
    //GlobalSchedule::start(HEART_BEAT_INTERVAL);
    
    //大喇叭tcp初始化
    //TcpHornClient::getInstance()->set_host_port(DzConfig::getInstance()->pBigHornConfig->ip.c_str(), DzConfig::getInstance()->pBigHornConfig->port);
    //TcpHornClient::getInstance()->connect();
    
    //初始化音乐
    Sound::getInstance()->PlayBgSound();
}

void StartScene::dailyLoginCallback(float dt)
{
    //this->addChild(this->createPopupDailyLoginLayer(),100);
}

void StartScene::dailyLoginShowTiggerCallback(float dt)
{
    //this->addChild(this->createTigerMachineLayer(),100);
}

void StartScene::clickHall(bool isBack)
{
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    //------------------------------------------------------------------------
    CCPoint endPoint;
    float delay = 0.0f;
    
    ControlButton* shopCtrlBtn = (ControlButton*)this->pContentNode->getChildByTag(RECHARGE_CONTRL_TAG);
    /*if (isBack)
    {
        endPoint = ccp(0,0+this->visibleH+1);
    }
    else
    {
        endPoint = ccp(0,0+this->visibleH+shopCtrlBtn->getPreferredSize().height);
    }
    shopCtrlBtn->runAction(CCSequence::create(CCEaseBounceOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));*/
    //------------------------------------------------------------------------
    if (isBack)
    {
        endPoint = ccp(0+this->visibleW-5,0+30);
        delay = 1.1f;
    }
    else
    {
        endPoint = ccp(0+this->visibleW+pQuickGameNode->getContentSize().width,0+30);
        delay = 0.0f;
    }
    pQuickGameNode->runAction(CCSequence::create(CCDelayTime::create(delay),CCEaseExponentialOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------
    CCSprite* pLBottomBg = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("left_bottom_bg.png"));
    CCSize lbottomSize = pLBottomBg->getContentSize();
    
    CCNode* pLBottomNode = this->pContentNode->getChildByTag(LEFT_BOTTOM_NODE_TAG);
    if (isBack)
    {
        endPoint = Vec2::ZERO;
        delay = 1.1f;
    }
    else
    {
        endPoint = ccp(0-lbottomSize.width/2,0-lbottomSize.height/2);
        delay = 0.0f;
    }
    pLBottomNode->runAction(CCSequence::create(CCDelayTime::create(delay),CCEaseExponentialOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------
    ControlButton* miniCtrlBtn = (ControlButton*)this->pContentNode->getChildByTag(MINI_GAME_CONTRL_TAG);
    if (isBack)
    {
        endPoint = ccp(0+this->visibleW,0+this->visibleH);
        delay = 1.1f;
    }
    else
    {
        endPoint = ccp(0+this->visibleW,0+this->visibleH+miniCtrlBtn->getPreferredSize().height);
        delay = 0.0f;
    }
    miniCtrlBtn->runAction(CCSequence::create(CCDelayTime::create(delay),CCEaseExponentialOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------
    ControlButton* noticeCtrlBtn = (ControlButton*)this->pContentNode->getChildByTag(GAME_ACTIVITY_CONTRL_TAG);
    if (isBack)
    {
        endPoint = ccp(0+shopCtrlBtn->getPreferredSize().width/2-noticeCtrlBtn->getPreferredSize().width/2,0+this->visibleH*0.6f);
        delay = 1.1f;
    }
    else
    {
        endPoint = ccp(0-noticeCtrlBtn->getPreferredSize().width,0+this->visibleH*0.6f);
        delay = 0.0f;
    }
    noticeCtrlBtn->runAction(CCSequence::create(CCDelayTime::create(delay),CCEaseExponentialOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------
    pHallNode->setAnchorPoint(ccp(0.5f,0.5f));
    ControlButton* hallCtrlBtn = (ControlButton*)pHallNode->getChildByTag(HALL_CONTRL_TAG);
    CCSprite* hallFrame = (CCSprite*)pHallNode->getChildByTag(HALL_CONTRL_TAG+1);
    //ClipLayer* pBestCardsLayer = (ClipLayer*)pHallNode->getChildByTag(HALL_CONTRL_TAG+2);
    if (isBack)
    {
        endPoint = ccp(0+this->visibleW/2,0+this->visibleH/2+22);
        delay = 1.1f;
    }
    else
    {
        endPoint = ccp(0+this->visibleW/2,0+this->visibleH*2.0f);
        delay = 0.0f;
    }
    pHallNode->runAction(CCSequence::create(CCDelayTime::create(delay),CCMoveTo::create(0.0f,endPoint),CCRepeat::create(CCFadeIn::create(0.5f),1),NULL));
    hallFrame->runAction(CCSequence::create(CCDelayTime::create(delay),CCRepeat::create(CCFadeIn::create(0.5f),1),NULL));
    hallCtrlBtn->runAction(CCSequence::create(CCDelayTime::create(delay),CCRepeat::create(CCFadeIn::create(0.5f),1),NULL));
    
    int rotate[5] = {-15,0,15,30,45};
    std::vector<CCPoint> cardPoints;
    cardPoints.push_back(CCPointMake(45, -5));
    cardPoints.push_back(CCPointMake(50, -10));
    cardPoints.push_back(CCPointMake(55, -15));
    cardPoints.push_back(CCPointMake(60, -20));
    cardPoints.push_back(CCPointMake(65, -25));
    for (int i=0;i<5;++i)
	{
        //CCSprite* cardSprite = (CCSprite*)pBestCardsLayer->getChildByTag(i);
        //cardSprite->setPosition(CCPointMake(0, -100));
        //cardSprite->setRotation(90);
        //cardSprite->runAction(CCSequence::create(CCDelayTime::create(delay),CCDelayTime::create((i+1)*0.1f),CCMoveTo::create(0.0f, *(cardPoints.begin()+i)),CCRotateTo::create(0.2f, rotate[i]),NULL));
	}
    //---------------------------------------------------------------------------------------
    CCSprite* pUserInfoFrame = (CCSprite*)this->pContentNode->getChildByTag(USER_INFO_FRAME_TAG);
    CCSize userInfoFrameSize = pUserInfoFrame->getContentSize();
    if (isBack)
    {
        endPoint = ccp(0+261,0-1);
        delay = 1.1f;
    }
    else
    {
        endPoint = ccp(0+261,0-1-userInfoFrameSize.height);
        delay = 0.0f;
    }
    pUserInfoFrame->runAction(CCSequence::create(CCDelayTime::create(delay),CCEaseExponentialOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------------------------
    CCNode* pUserInfoNode = this->pContentNode->getChildByTag(USER_INFO_NODE_TAG);
    if (isBack)
    {
        endPoint = ccp(0+261,0-1);
        delay = 1.1f;
    }
    else
    {
        endPoint = ccp(0+261,0-1-userInfoFrameSize.height);
        delay = 0.0f;
    }
    pUserInfoNode->runAction(CCSequence::create(CCDelayTime::create(delay),CCEaseExponentialOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //------------------------------------------------------------------------------------------
    CCLabelTTF* pOnlineText = (CCLabelTTF*)this->pContentNode->getChildByTag(ONLINE_TEXT_TAG);
    if (isBack)
    {
        endPoint = ccp(0+264+userInfoFrameSize.width+15,0);
        delay = 1.1f;
    }
    else
    {
        endPoint = ccp(0+264+userInfoFrameSize.width+15,0-userInfoFrameSize.height);
        delay = 0.0f;
    }
    pOnlineText->runAction(CCSequence::create(CCDelayTime::create(delay),CCEaseExponentialOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
    //-----------------------------------------------------------------------------------------
    ControlButton* shareCtrlBtn = (ControlButton*)this->pContentNode->getChildByTag(SHARE_BTN_TAG);
    if (isBack)
    {
        delay = 1.1f;
        shareCtrlBtn->runAction(CCSequence::create(CCDelayTime::create(delay),CCRepeat::create(CCFadeIn::create(1.0f),1),NULL));
    }
    else
    {
        delay = 0.0f;
        shareCtrlBtn->runAction(CCSequence::create(CCDelayTime::create(delay),CCRepeat::create(CCFadeOut::create(1.0f),1),NULL));
    }
    //------------------------------------------------------------------------------------------
    //VenueListLayer* pVenueListLayer = (VenueListLayer*)this->pContentNode->getChildByTag(VENUE_LIST_TAG);
    if (isBack)
    {
        endPoint = ccp(this->visibleW,0);
        delay = 0.0f;
        
        isVenueListShow = false;
    }
    else
    {
        endPoint = ccp(-43,0);
        delay = 0.5f;
        
        //更新房间列表
        //pVenueListLayer->changeRoomTypeRefreshVenueList();
        
        isVenueListShow = true;
    }
    //pVenueListLayer->runAction(CCSequence::create(CCDelayTime::create(delay),CCEaseExponentialOut::create(CCMoveTo::create(1.0f,endPoint)),NULL));
}

void StartScene::noRunLoadingAnim()
{
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
 //   ControlButton* shopCtrlBtn = CtrlManager::createCtrlBtnWithFreame("shop_icon.png","");
	//shopCtrlBtn->setAnchorPoint(ccp(0.0f,1.0f));
	//shopCtrlBtn->setPosition(ccp(0,0+this->visibleH+1));
	//shopCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//this->pContentNode->addChild(shopCtrlBtn,1,RECHARGE_CONTRL_TAG);
    
    CCParticleSystem* ignore1 = CCParticleSystemQuad::create("start/starEffect.plist");
    ignore1->setPosition(ccp(25,80));
    ignore1->setPositionType(kCCPositionTypeRelative);
    //shopCtrlBtn->addChild(ignore1);
    
    CCParticleSystem* ignore2 = CCParticleSystemQuad::create("start/starEffect.plist");
    ignore2->setPosition(ccp(100,75));
    ignore1->setPositionType(kCCPositionTypeRelative);
    //shopCtrlBtn->addChild(ignore2);
    
    CCParticleSystem* ignore3 = CCParticleSystemQuad::create("start/starEffect.plist");
    ignore3->setPosition(ccp(150,150));
    ignore1->setPositionType(kCCPositionTypeRelative);
    //shopCtrlBtn->addChild(ignore3);
    
 //   ControlButton* miniCtrlBtn = CtrlManager::createCtrlBtnWithFreame("mini_icon.png","");
	//miniCtrlBtn->setAnchorPoint(ccp(1.0f,1.0f));
	//miniCtrlBtn->setPosition(ccp(0+this->visibleW,0+this->visibleH));
	//miniCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//this->pContentNode->addChild(miniCtrlBtn,0,MINI_GAME_CONTRL_TAG);
    //-------------------------------------------------------------------------------------
 //   ControlButton* quickGameCtrlBtn = CtrlManager::createCtrlBtnWithFreame("quick_game_icon.png","");
 //   
 //   pQuickGameNode = CCNode::create();
 //   CC_SAFE_RETAIN(pQuickGameNode);
 //   pQuickGameNode->setContentSize(quickGameCtrlBtn->getPreferredSize());
 //   pQuickGameNode->setAnchorPoint(ccp(1.0f,0.0f));
	//pQuickGameNode->setPosition(ccp(0+this->visibleW-5,0+30));
	//this->pContentNode->addChild(pQuickGameNode);
    
	//quickGameCtrlBtn->setAnchorPoint(Vec2::ZERO);
	//quickGameCtrlBtn->setPosition(Vec2::ZERO);
	//quickGameCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//pQuickGameNode->addChild(quickGameCtrlBtn,0,QUICK_GAME_CONTRL_TAG);
    
    CCSprite* quickText = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("quick_game_text.png"));
    quickText->setAnchorPoint(ccp(0.5f,0.5f));
    quickText->setPosition(ccp(92,111));
    pQuickGameNode->addChild(quickText,2);
    
    CCSprite* quickTextFrame = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("quick_game_rotate_1.png"));
    quickTextFrame->setAnchorPoint(ccp(0.5f,0.5f));
    quickTextFrame->setPosition(ccp(92,111));
    pQuickGameNode->addChild(quickTextFrame,1);
    //动画
    
    quickText->runAction(CCRepeatForever::create(CCRotateBy::create(8.0f, 360)));
    
    //CCArray* pAnimArray = CCArray::create();
	Vector<SpriteFrame*> pAnimArray;
    int k = 1;
    for (int i=1; i<=16; ++i)
    {
        CCString* file = CCString::createWithFormat("quick_game_rotate_%d.png",k);
        //pAnimArray->addObject(frameCache->spriteFrameByName(file->getCString()));
		pAnimArray.pushBack(frameCache->spriteFrameByName(file->getCString()));
        if (i>8)
        {
            --k;
        }
        else if (i<8)
        {
            ++k;
        }
        else
        {
            for (int j=0; j<6; ++j)
            {
                //pAnimArray->addObject(frameCache->spriteFrameByName("quick_game_rotate_8.png"));
				pAnimArray.pushBack(frameCache->spriteFrameByName("quick_game_rotate_8.png"));
            }
        }
    }
    CCAnimation* pRotateAnim = CCAnimation::createWithSpriteFrames(pAnimArray, 0.15f);
    CCFiniteTimeAction* rotateAction = CCAnimate::create(pRotateAnim);
    quickTextFrame->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(2.0f),rotateAction,NULL)));
    //--------------------------------------------------------------------------
    CCSprite* hallFrame = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("hall_icon_frame.png"));
    CCSize hallSize = hallFrame->getContentSize();
    
    pHallNode = CCNode::create();
    CC_SAFE_RETAIN(pHallNode);
    pHallNode->setContentSize(hallSize);
    pHallNode->setAnchorPoint(ccp(0.5f,0.5f));
	pHallNode->setPosition(ccp(0+this->visibleW/2,0+this->visibleH/2+22));
	this->pContentNode->addChild(pHallNode);
    
 //   ControlButton* hallCtrlBtn = CtrlManager::createCtrlBtnWithFreame("hall_icon.png","");
	//hallCtrlBtn->setAnchorPoint(ccp(0.5f,0.5f));
	//hallCtrlBtn->setPosition(ccp(hallSize.width/2,hallSize.height/2));
	//hallCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//pHallNode->addChild(hallCtrlBtn,1,HALL_CONTRL_TAG);
    
	hallFrame->setAnchorPoint(Vec2::ZERO);
	hallFrame->setPosition(Vec2::ZERO);
	pHallNode->addChild(hallFrame,3,HALL_CONTRL_TAG+1);
    
 //   ClipLayer* pBestCardsLayer = ClipLayer::create(pHallNode->getContentSize());
 //   pBestCardsLayer->setAnchorPoint(Vec2::ZERO);
	//pBestCardsLayer->setPosition(ccp(20,15));
 //   pHallNode->addChild(pBestCardsLayer,2,HALL_CONTRL_TAG+2);
    
    int rotate[5] = {-15,0,15,30,45};
    std::vector<CCPoint> cardPoints;
    cardPoints.push_back(CCPointMake(45, -5));
    cardPoints.push_back(CCPointMake(50, -10));
    cardPoints.push_back(CCPointMake(55, -15));
    cardPoints.push_back(CCPointMake(60, -20));
    cardPoints.push_back(CCPointMake(65, -25));
    for (int i=0;i<5;++i)
	{
        CCString* cardStr = CCString::createWithFormat("rooms/card/card_%s.png",PlayerModel::getInstance()->best_cards[i].c_str());
		CCSprite* cardSprite = CCSprite::create(cardStr->getCString());
        cardSprite->setScale(0.8f);
		cardSprite->setAnchorPoint(ccp(0.5f,0.0f));
		cardSprite->setPosition(ccp(0,-100));
        cardSprite->setRotation(90);
		//pBestCardsLayer->addChild(cardSprite,2,i);
        
        cardSprite->runAction(CCSequence::create(CCDelayTime::create(0.2f+(i+1)*0.1f),CCMoveTo::create(0.0f, *(cardPoints.begin()+i)),CCRotateTo::create(0.2f, rotate[i]),NULL));
	}
    //----------------------------------------------------------------------------
 //   ControlButton* noticeCtrlBtn = CtrlManager::createCtrlBtnWithFreame("notice_icon.png","");
	//noticeCtrlBtn->setAnchorPoint(ccp(0.0f,1.0f));
	//noticeCtrlBtn->setPosition(ccp(0+shopCtrlBtn->getPreferredSize().width/2-noticeCtrlBtn->getPreferredSize().width/2,0+this->visibleH*0.6f));
	//noticeCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//this->pContentNode->addChild(noticeCtrlBtn,0,GAME_ACTIVITY_CONTRL_TAG);
    
    //------------------------------------------------------------------------
    CCSprite* pLBottomBg = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("left_bottom_bg.png"));
    CCSize lbottomSize = pLBottomBg->getContentSize();
    
    CCNode* pLBottomNode = CCNode::create();
    pLBottomNode->setContentSize(lbottomSize);
    pLBottomNode->setAnchorPoint(Vec2::ZERO);
    pLBottomNode->setPosition(Vec2::ZERO);
    this->pContentNode->addChild(pLBottomNode,0,LEFT_BOTTOM_NODE_TAG);
    
    pLBottomBg->setAnchorPoint(Vec2::ZERO);
    pLBottomBg->setPosition(Vec2::ZERO);
    pLBottomNode->addChild(pLBottomBg);
    
    //ControlButton* rankCtrlBtn = CtrlManager::createCtrlBtnWithFreame("cricle_btn.png","");
    //rankCtrlBtn->setAnchorPoint(Vec2::ZERO);
    //rankCtrlBtn->setPosition(ccp(lbottomSize.width/2-rankCtrlBtn->getPreferredSize().width*1.2f,0));
    //rankCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
    //pLBottomNode->addChild(rankCtrlBtn,0,RANKING_CONTRL_TAG);
    
    CCSprite* rankIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("rank_icon.png"));
    rankIcon->setAnchorPoint(ccp(0.5f,0.5f));
    //rankIcon->setPosition(ccp(rankCtrlBtn->getPreferredSize().width/2,rankCtrlBtn->getPreferredSize().height/2));
    //rankCtrlBtn->addChild(rankIcon);
    
    //ControlButton* settingCtrlBtn = CtrlManager::createCtrlBtnWithFreame("cricle_btn.png","");
    //settingCtrlBtn->setAnchorPoint(Vec2::ZERO);
    //settingCtrlBtn->setPosition(ccp(0,rankCtrlBtn->getPositionY()+settingCtrlBtn->getPreferredSize().height-10));
    //settingCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
    //pLBottomNode->addChild(settingCtrlBtn,0,SETTINGS_CONTRL_TAG);
    
    CCSprite* settingIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("setting_icon.png"));
    settingIcon->setAnchorPoint(ccp(0.5f,0.5f));
    //settingIcon->setPosition(ccp(settingCtrlBtn->getPreferredSize().width/2,settingCtrlBtn->getPreferredSize().height/2));
    //settingCtrlBtn->addChild(settingIcon);
    
    //------------------------------------------------------------------------
    CCSprite* pNoticeBg = CCSprite::create("msg/msg_bg.png");
    CCSize noticeBgSize = pNoticeBg->getContentSize();
 //   HornLayer* pHornLayer = HornLayer::createWith(CCPointMake((this->visibleW-miniCtrlBtn->getPreferredSize().width)/2+shopCtrlBtn->getPreferredSize().width/2-noticeBgSize.width/2, this->visibleH-noticeBgSize.height-20));
 //   pHornLayer->setAnchorPoint(Vec2::ZERO);
	//pHornLayer->setPosition(Vec2::ZERO);
	//this->pContentNode->addChild(pHornLayer,20);
    
    //------------------------------------------------------------------------
    CCSprite* pUserInfoFrame = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("line_frame.png"));
    CCSize userInfoFrameSize = pUserInfoFrame->getContentSize();
    pUserInfoFrame->setAnchorPoint(Vec2::ZERO);
    pUserInfoFrame->setPosition(ccp(0+261,0-1));
    this->pContentNode->addChild(pUserInfoFrame,0,USER_INFO_FRAME_TAG);
    
    CCNode* pUserInfoNode = CCNode::create();
    pUserInfoNode->setContentSize(userInfoFrameSize);
    pUserInfoNode->setAnchorPoint(Vec2::ZERO);
    pUserInfoNode->setPosition(ccp(0+lbottomSize.width-3,0-1));
    this->pContentNode->addChild(pUserInfoNode,0,USER_INFO_NODE_TAG);
    
 //   ControlButton* eamilCtrlBtn = CtrlManager::createCtrlBtnWithFreame("email_icon.png","");
	//eamilCtrlBtn->setAnchorPoint(Vec2::ZERO);
	//eamilCtrlBtn->setPosition(ccp(37,25));
	//eamilCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//pUserInfoNode->addChild(eamilCtrlBtn,0,EMAIL_CONTRL_TAG);
    
    const char* lvStr = CCString::createWithFormat("LV: %d",PlayerModel::getInstance()->getLevelByExp(PlayerModel::getInstance()->exp))->getCString();
    CCLabelTTF* pLVText = CCLabelTTF::create(lvStr, "Arial", 18);
    pLVText->setAnchorPoint(ccp(0.5f,0.5f));
    //pLVText->setPosition(ccp(eamilCtrlBtn->getPositionX()+eamilCtrlBtn->getPreferredSize().width/2,10));
    pUserInfoNode->addChild(pLVText);
    
 //   ControlButton* userInfoCtrlBtn = CtrlManager::createCtrlBtnWithFreame("start_header_frame.png","");
 //   userInfoCtrlBtn->setAnchorPoint(Vec2::ZERO);
	//userInfoCtrlBtn->setPosition(ccp(eamilCtrlBtn->getPositionX()+eamilCtrlBtn->getPreferredSize().width+20,0));
	//userInfoCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//pUserInfoNode->addChild(userInfoCtrlBtn,0,PLAYERINFO_CONTRL_TAG);
    
    const char* genderPicName = PlayerModel::getInstance()->sex==1 ? "def_man_header.png" : "def_wman_header.png";
    std::string headerFile = "no_custom_head";
    //CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid,headerFile);
    if (headerFile.compare("no_custom_head")==0)
    {
        pUserHeader = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName(genderPicName));
        pUserHeader->setAnchorPoint(Vec2::ZERO);
        //userHeaderPoint = ccp(userInfoCtrlBtn->getPositionX()+userInfoCtrlBtn->getPreferredSize().width/2-pUserHeader->getContentSize().width/2-1,userInfoCtrlBtn->getPositionY()+userInfoCtrlBtn->getPreferredSize().height/2-pUserHeader->getContentSize().height/2-3);
        pUserHeader->setPosition(userHeaderPoint);
    }
    else
    {
        CCSprite* pTemp = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName(genderPicName));
        CCSize tempSize = pTemp->boundingBox().size;
        tempSize = CCSizeMake(tempSize.width-4, tempSize.height-4);
        
        //CCTexture2D* pHeaderTexture = CCTextureCache::sharedTextureCache()->addImage(CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid).c_str());
        //pUserHeader = CCSprite::createWithTexture(pHeaderTexture);
        //pUserHeader->setContentSize(tempSize);
        //CCSize avatarSize = pHeaderTexture->getContentSize();
        //float scaleX = tempSize.width / avatarSize.width;
        //float scaleY = tempSize.height / avatarSize.height;
        //pUserHeader->setScaleX(scaleX);
        //pUserHeader->setScaleY(scaleY);
        //
        //pUserHeader->setAnchorPoint(Vec2::ZERO);
        //userHeaderPoint = ccp(userInfoCtrlBtn->getPositionX()+userInfoCtrlBtn->getPreferredSize().width/2-pUserHeader->getContentSize().width/2,userInfoCtrlBtn->getPositionY()+userInfoCtrlBtn->getPreferredSize().height/2-pUserHeader->getContentSize().height/2-2);
        //pUserHeader->setPosition(userHeaderPoint);
    }
    CC_SAFE_RETAIN(pUserHeader);
	pUserInfoNode->addChild(pUserHeader);
    
    pUserName = CCLabelTTF::create(PlayerModel::getInstance()->name.c_str(),"Arial",18);
    CC_SAFE_RETAIN(pUserName);
	pUserName->setAnchorPoint(Vec2::ZERO);
	//pUserName->setPosition(ccp(userInfoCtrlBtn->getPositionX()+userInfoCtrlBtn->getPreferredSize().width+10,userInfoCtrlBtn->getPositionY()+userInfoCtrlBtn->getPreferredSize().height-pUserName->getContentSize().height-3));
	pUserInfoNode->addChild(pUserName,0,PLAYER_NAME_TAG);
    
    const char* shipStr = StringFormat::intsert(PlayerModel::getInstance()->money,3,",");
	const char* rmbStr = StringFormat::intsert(PlayerModel::getInstance()->rmb,3,",");
	CCSprite* pShipIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("ship_icon.png"));
	CCSprite* pRMBIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("gold_icon.png"));
	//pShipIconText = IconTextSprite::createWith(pShipIcon,shipStr);
	//pRmbIconText = IconTextSprite::createWith(pRMBIcon,rmbStr);
	//CC_SAFE_RETAIN(pShipIconText);
	//CC_SAFE_RETAIN(pRmbIconText);
    
    //pRmbIconText->setAnchorPoint(Vec2::ZERO);
    //pRmbIconText->setFontSize(18);
    //pRmbIconText->setPosition(ccp(pUserName->getPositionX()+2,pUserName->getPositionY()-pShipIconText->getContentSize().height-2));
    //pUserInfoNode->addChild(pRmbIconText);
    
    //pShipIconText->setAnchorPoint(Vec2::ZERO);
    //pShipIconText->setFontSize(18);
    //pShipIconText->setPosition(ccp(pUserName->getPositionX()+2,pRmbIconText->getPositionY()-pShipIconText->getContentSize().height-5));
    //pUserInfoNode->addChild(pShipIconText);
    //------------------------------------------------------------------------
    CCLabelTTF* pOnlineText = CCLabelTTF::create(CCString::createWithFormat("在线: %ld",DzConfig::getInstance()->online_count)->getCString(), "Arial", 18);
    pOnlineText->setAnchorPoint(Vec2::ZERO);
    pOnlineText->setPosition(ccp(0+lbottomSize.width+userInfoFrameSize.width+15,0));
    this->pContentNode->addChild(pOnlineText,0,ONLINE_TEXT_TAG);
    
    //------------------------------------------------------------------------
 //   ControlButton* shareCtrlBtn = CtrlManager::createCtrlBtnWithFile("share/weixin_icon.png","");
	//shareCtrlBtn->setAnchorPoint(ccp(0.0f,0.0f));
	//shareCtrlBtn->setPosition(ccp(160,40));
	//shareCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
	//this->pContentNode->addChild(shareCtrlBtn,0,SHARE_BTN_TAG);
 //   shareCtrlBtn->runAction(CCSequence::create(CCDelayTime::create(0.0f),CCRepeat::create(CCFadeIn::create(1.0f),1),NULL));
    
    //------------------------------------------------------------------------
    //VenueListLayer* pVenueListLayer = VenueListLayer::create();
    //pVenueListLayer->setAnchorPoint(Vec2::ZERO);
    //pVenueListLayer->setPosition(ccp(this->visibleW+1,0));
    //this->pContentNode->addChild(pVenueListLayer,0,VENUE_LIST_TAG);
    //------------------------------------------------------------------------
    
    //破产领取
    PlayerModel::getInstance()->getBrokeChip();
    
    //初始化音乐
    Sound::getInstance()->PlayBgSound();
}

void StartScene::stopLoadingAnim()
{
    //pAnimLoading->stopUpdate();
}

cocos2d::CCScene* StartScene::scene()
{
    cocos2d::CCScene* scene = NULL;
    do {
        scene = cocos2d::CCScene::create();
        CC_BREAK_IF(!scene);
        
        StartScene* layer = StartScene::create();
        CC_BREAK_IF(!layer);
        
        scene->addChild(layer);
        
    } while (0);
    return scene;
}

void StartScene::registerOrLogin()
{
	this->runLoadingAnim();
    
    //CCHttpAgent::getInstance()->setUrl(HTTP_SERVER_ADDRESS);
    if (isAutoLgoin)
    {
        std::string acount = CCUserDefault::sharedUserDefault()->getStringForKey(PLAYER_ACOUNT_KEY);
        std::string pwd = CCUserDefault::sharedUserDefault()->getStringForKey(PLAYER_PASSWORD_KEY);
        if (acount.empty())
        {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            acount = getUserNameOrPwdWithSDcard(0);
            pwd = getUserNameOrPwdWithSDcard(1);
        #endif
        }
        //CCHttpAgent::getInstance()->doLogin(acount.c_str(), pwd.c_str());
    }
    else
    {
        //CCHttpAgent::getInstance()->doAutoRegister();
    }
    this->schedule(schedule_selector(StartScene::registerORloginCallback), 0.5f);
}

void StartScene::registerORloginCallback(float dt)
{
    int index = atoi(string(CMD_LOGIN).c_str()) - 1000;
    if (!isAutoLgoin)
    {
        index = atoi(string(CMD_AUTOREGISTER).c_str()) - 1000;
    }
    //CCHttpPacket& loginPacket = CCHttpAgent::getInstance()->packets[index];
    //if (loginPacket.status != 0)
    //{
    //    if (loginPacket.status != 3)
    //    {
    //        this->unschedule(schedule_selector(StartScene::registerORloginCallback));
    //        this->stopLoadingAnim();
    //        this->createPopupTipLayer("错误提示", "登陆错误!", LOGIN_ERROR_TIP);
    //        return;
    //    }
    //    
    //    int ret = loginPacket.recvVal["ret"].asInt();
    //    
    //    if (ret == 100)
    //    {
    //        this->unschedule(schedule_selector(StartScene::registerORloginCallback));
    //        this->stopLoadingAnim();
    //        this->createPopupTipLayer("错误提示", "此账号存在非法操作，已被封号!", LOGIN_ERROR_TIP);
    //        return;
    //    }
    //    
    //    if (ret != 0)
    //    {
    //        this->unschedule(schedule_selector(StartScene::registerORloginCallback));
    //        this->stopLoadingAnim();
    //        this->createPopupTipLayer("错误提示", "登陆错误!", LOGIN_ERROR_TIP);
    //        return;
    //    }
    //    
    //    if (!isAutoLgoin)
    //    {
    //        string userStr = loginPacket.recvVal["data"]["user"].asString();
    //        string passwordStr = loginPacket.recvVal["data"]["password"].asString();
    //        
    //        CCUserDefault::sharedUserDefault()->setStringForKey(PLAYER_ACOUNT_KEY, userStr);
    //        CCUserDefault::sharedUserDefault()->setStringForKey(PLAYER_PASSWORD_KEY, passwordStr);
    //        CCUserDefault::sharedUserDefault()->flush();
    //        
    //        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //        CCString* userAndPwd = CCString::createWithFormat("%s,%s",userStr.c_str(),passwordStr.c_str());
    //        writeUserNameAndPwdToSDcard(userAndPwd->getCString());
    //        #endif
    //    }
    //    
    //    this->getPlayerInfoWithJson(loginPacket);
    //    CCHttpAgent::getInstance()->setUidAndSkey(PlayerModel::getInstance()->uid, PlayerModel::getInstance()->skey);
    //    
    //    //类型id，1表示汇智，2表示MM
    //    int spaceTypeId = 1;
    //    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    spaceTypeId = getSpaceTypeId();
    //    #endif
    //    CCHttpAgent::getInstance()->doConfig(spaceTypeId);
    //    
    //    //后台请求其他数据
    //    GlobalHttpReq::getInstance()->globalHttpReqData();
    //    
    //    this->unschedule(schedule_selector(StartScene::registerORloginCallback));
    //    this->schedule(schedule_selector(StartScene::getConfigCallback), 0.1f);
    //}
}

void StartScene::getConfigCallback(float dt)
{
    //CCHttpPacket& configPacket = CCHttpAgent::getInstance()->packets[atoi(string(CMD_CONFIG).c_str()) - 1000];
    //
    //if (configPacket.status != 0)
    //{
    //    if (configPacket.status != 3)
    //    {
    //        this->unschedule(schedule_selector(StartScene::getConfigCallback));
    //        this->stopLoadingAnim();
    //        this->createPopupTipLayer("错误提示", "获取配置信息获取错误!", ERROR_INFO_TIP);
    //        return;
    //    }
    //    int ret = configPacket.recvVal["ret"].asInt();
    //    if (ret != 0)
    //    {
    //        this->unschedule(schedule_selector(StartScene::getConfigCallback));
    //        this->stopLoadingAnim();
    //        this->createPopupTipLayer("错误提示", "获取配置信息获取错误!", ERROR_INFO_TIP);
    //        return;
    //    }
    //    
    //    this->getConfigWithJson(configPacket);
    //    
    //    this->unschedule(schedule_selector(StartScene::getConfigCallback));
    //}
}

//void StartScene::getConfigWithJson(CCHttpPacket& configPacket)
//{
//    Json::Value bigHorn = configPacket.recvVal["data"]["bigHorn"];
//    BigHornConfig* pBigHornConfig = new BigHornConfig;
//    pBigHornConfig->ip = bigHorn["ip"].asString();
//    pBigHornConfig->port = bigHorn["port"].asInt();
//    pBigHornConfig->money = bigHorn["money"].asInt();
//    DzConfig::getInstance()->pBigHornConfig = pBigHornConfig;
//    
//    Json::Value tigger = configPacket.recvVal["data"]["solt"];
//    TiggerConfig* pTigerConfig = new TiggerConfig;
//    pTigerConfig->open = tigger["money"].asInt();
//    DzConfig::getInstance()->pTiggerConfig = pTigerConfig;
//    
//    Json::Value happy = configPacket.recvVal["data"]["happy"];
//    HappyConfig* pHappyConfig = new HappyConfig;
//    pHappyConfig->open = happy["open"].asInt();
//    pHappyConfig->money = happy["money"].asInt();
//    DzConfig::getInstance()->pHappyConfig = pHappyConfig;
//    
//    DzConfig::getInstance()->online_count = configPacket.recvVal["data"]["onlineInfo"]["totalCount"].asInt();
//    DzConfig::getInstance()->brokeMoney = configPacket.recvVal["data"]["brokeMoney"].asInt();
//    DzConfig::getInstance()->brokeGiveMoney = configPacket.recvVal["data"]["brokeGiveMoney"].asInt();
//    DzConfig::getInstance()->shareUrl = configPacket.recvVal["data"]["weixin_url"].asString().append(CCString::createWithFormat("%d",PlayerModel::getInstance()->uid)->getCString());
//    //DLog::showLog("brokeGiveMoney: %d",DzConfig::getInstance()->brokeGiveMoney);
//    Json::Value loginReward = configPacket.recvVal["data"]["loginReward"];
//    for (int i=0; i<loginReward.size(); ++i)
//    {
//        DzConfig::getInstance()->login_award_info[i] = loginReward[i].asInt();
//        DLog::showLog("loginReward[%d]=%d",i,DzConfig::getInstance()->login_award_info[i]);
//    }
//    
//    PlayerModel::getInstance()->is_login_succ = true;
//    pAnimLoading->setResumeUpdate();
//}

void StartScene::quickStart()
{
    //if (!GlobalHttpReq::getInstance()->venueList1ReqIsFinish() &&
    //    !GlobalHttpReq::getInstance()->venueList2ReqIsFinish() &&
    //    !GlobalHttpReq::getInstance()->venueList3ReqIsFinish() &&
    //    !GlobalHttpReq::getInstance()->venueList4ReqIsFinish() &&
    //    !GlobalHttpReq::getInstance()->venueList5ReqIsFinish() )
    //{
    //    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //        showToast("正在更新房间信息,请稍后点击进入!");
    //    #else
    //        CCMessageBox("正在更新房间信息,请稍后点击进入!","错误提示");
    //    #endif
    //    return;
    //}
    
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
        //if (PlayerModel::getInstance()->money>=(*iter).min_money)
        //{
        //    if ((*iter).limit==-1){
        //        room_id.push_back((*iter).room_id);
        //    }else if (PlayerModel::getInstance()->money<=(*iter).limit)
        //    {
        //        room_id.push_back((*iter).room_id);
        //    }
        //}
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
        //if ((*iter).room_id==table_id)
        //{
            //RoomInfo::getInstance()->s_bet=(*iter).sblind;
            //RoomInfo::getInstance()->b_bet=(*iter).bblind;
            //RoomInfo::getInstance()->min_take=(*iter).min_money;
            //RoomInfo::getInstance()->max_take=(*iter).max_money;
            //RoomInfo::getInstance()->port = (*iter).port;
            //RoomInfo::getInstance()->ip = (*iter).ip;
            //RoomInfo::getInstance()->max_limit = (*iter).limit;
            //RoomInfo::getInstance()->tid = 0;
            //RoomInfo::getInstance()->rmid = (*iter).showVenueId;
            //
            //CCArray* vect = CCArray::create();
            //StringFormat::Split((*iter).quickRefuelItems.c_str(), ",", vect);
            //for (int i=0; i<vect->count(); ++i)
            //{
            //    RoomInfo::getInstance()->quick_addBet[i] = StringFormat::strToInt(((CCString*)vect->objectAtIndex(i))->getCString());
            //    DLog::showLog("-------quick_addBet[%d]: %d",i,RoomInfo::getInstance()->quick_addBet[i]);
            //}
             
            
        //    break;
        //}
    }
    
    CCSpriteFrameCache::purgeSharedSpriteFrameCache();
    CCTextureCache::purgeSharedTextureCache();
    
    Sound::getInstance()->playEffect("sound/sound_gangjinru.ogg");
    
    //CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, RoomScene::scene(),ccBLACK);
    //CCDirector::sharedDirector()->replaceScene(reScene);
}

//void StartScene::getPlayerInfoWithJson(CCHttpPacket& loginPacket)
//{
//    PlayerModel::getInstance()->uid = loginPacket.recvVal["data"]["uid"].asInt();
//    PlayerModel::getInstance()->name = loginPacket.recvVal["data"]["name"].asString();
//    PlayerModel::getInstance()->sex = loginPacket.recvVal["data"]["sex"].asInt();
//    PlayerModel::getInstance()->vlevel = loginPacket.recvVal["data"]["vlevel"].asInt();
//    PlayerModel::getInstance()->rmb = loginPacket.recvVal["data"]["rmb"].asInt();
//    PlayerModel::getInstance()->money = loginPacket.recvVal["data"]["money"].asInt();
//    PlayerModel::getInstance()->skey = loginPacket.recvVal["data"]["skey"].asString();
//    PlayerModel::getInstance()->user = loginPacket.recvVal["data"]["user"].asString();
//    PlayerModel::getInstance()->login_days = loginPacket.recvVal["data"]["login_days"].asInt();
//    PlayerModel::getInstance()->is_get_login_packer = loginPacket.recvVal["data"]["is_get"].asInt();
//    PlayerModel::getInstance()->total_board = loginPacket.recvVal["data"]["total_board"].asInt();
//    PlayerModel::getInstance()->total_win = loginPacket.recvVal["data"]["total_win"].asInt();
//    PlayerModel::getInstance()->exp = loginPacket.recvVal["data"]["exp"].asInt();
//    PlayerModel::getInstance()->vtime = loginPacket.recvVal["data"]["vtime"].asInt();
//    PlayerModel::getInstance()->avatar = loginPacket.recvVal["data"]["avatar"].asString();
//    PlayerModel::getInstance()->max_ship = loginPacket.recvVal["data"]["max_ship"].asInt();
//    PlayerModel::getInstance()->solt_num = loginPacket.recvVal["data"]["solt_num"].asInt();
//    PlayerModel::getInstance()->broke_num = loginPacket.recvVal["data"]["broke_num"].asInt();
//    PlayerModel::getInstance()->auto_register = loginPacket.recvVal["data"]["auto_register"].asInt();
//    PlayerModel::getInstance()->chanel_id = loginPacket.recvVal["data"]["channel_id"].asInt();
//    
//    std::string bestCardStr = loginPacket.recvVal["data"]["best_cards"].asString();
//    for (int i=0; i<5; ++i)
//    {
//        if (bestCardStr == "00")
//        {
//            PlayerModel::getInstance()->best_cards[i] = "00";
//        }
//        else
//        {
//            PlayerModel::getInstance()->best_cards[i] = CCString::createWithFormat("%c%c",bestCardStr.at(i*2),bestCardStr.at(i*2+1))->getCString();
//        }
//    }
//}

void StartScene::popupTipLayerCallback(CCNode* pSender)
{
    int popBtnTag = pSender->getTag();
    
    if (POPUP_CTRLBTN_OK_TAG == popBtnTag)
    {
        if (currTipTag == LOGIN_ERROR_TIP)
        {
            this->stopLoadingAnim();
            this->exitGame();
        }
        else if (currTipTag == ERROR_INFO_TIP)
        {
            this->stopLoadingAnim();
            this->exitGame();
        }
    }
    else
    {
        this->exitGame();
    }
    
}

void StartScene::onEnter()
{
	Layer::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(StartScene::notifiUpdateRichCallback),NOTIFI_UPDATE_RICHINFO_KEY,NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(StartScene::notifidailyLoginShowCallback),NOTIFI_DAILY_SHOW_TIGER_KEY,NULL);
}

void StartScene::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTIFI_UPDATE_RICHINFO_KEY);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTIFI_DAILY_SHOW_TIGER_KEY);
	Layer::onExit();
}

void StartScene::notifiUpdateRichCallback(CCObject* pCCObjData)    
{
    //DLog::showLog("start scene heart beat callback");
    
    if (isMiniGameing)
    {
        return;
    }
    
    const char* chipStr = StringFormat::intsert(PlayerModel::getInstance()->money,3,",");
    const char* paperMoneyStr = StringFormat::intsert(PlayerModel::getInstance()->rmb,3,",");
    //if (pShipIconText != NULL)
    //{
    //    pShipIconText->setText(chipStr);
    //}
    //
    //if (pRmbIconText != NULL)
    //{
    //    pRmbIconText->setText(paperMoneyStr);
    //}
    
}

void StartScene::notifidailyLoginShowCallback(CCObject* pCCObjData)
{
    this->scheduleOnce(schedule_selector(StartScene::dailyLoginShowTiggerCallback), 0.1f);
}

//void StartScene::keyBackClicked()
//{
//    if (isVenueListShow)
//    {
//        Sound::getInstance()->playEffect("sound/sound_dianji.ogg");
//        
//        this->clickHall(true);
//        isVenueListShow = false;
//        return;
//    }
//    
//    if (isTigetMachineShow)
//    {
//        isTigetMachineShow = true;
//        
//        TigerMachineLayer* pTigerMachineLayer =  (TigerMachineLayer*)this->getChildByTag(TIGER_MACHINE_LAYER_TAG);
//        if (pTigerMachineLayer != NULL && !pTigerMachineLayer->isTipPopupShow())
//        {
//            if(pTigerMachineLayer->closeTigerMachine())
//            {
//                isTigetMachineShow = false;
//            }
//        }
//        else
//        {
//            isTigetMachineShow = true;
//        }
//        return;
//    }
//    
//    if (isGuagualeMachineShow)
//    {
//        isGuagualeMachineShow = true;
//        
//        GuaGuaMachine* pGuaGuaMachine =  (GuaGuaMachine*)this->getChildByTag(GUAGUALE_MACHINE_LAYER_TAG);
//        if (pGuaGuaMachine != NULL && !pGuaGuaMachine->isTipPopupShow())
//        {
//            if(pGuaGuaMachine->closeGuaguale())
//            {
//                isGuagualeMachineShow = false;
//            }
//        }
//        else
//        {
//            isGuagualeMachineShow = true;
//        }
//        return;
//    }
//    
//    if (isWeixinShareShow)
//    {
//        WeixinShareLayer* pWeixinShareLayer =  (WeixinShareLayer*)this->getChildByTag(WEIXIN_SHARE_LAYER_TAG);
//        if (pWeixinShareLayer != NULL)
//        {
//            pWeixinShareLayer->closeShare();
//        }
//        isWeixinShareShow = false;
//        return;
//    }
//    
//	if (iskeyBackClicked)
//	{
//		return;
//	}
//    
//	iskeyBackClicked = true;
//    
//	this->addChild(this->createPopupExitGameLayer(),100);
//}

void StartScene::popLayerExitGameCallback(CCNode* pSender)
{
	int popBtnTag = pSender->getTag();
	if (POPUP_CTRLBTN_OK_TAG == popBtnTag)
	{
		this->exitGame();
	}
	else
	{
		iskeyBackClicked = false;
	}
}

void StartScene::miniGameCallback(CCNode* pSender)
{
	int popBtnTag = pSender->getTag();
    
	if (POPUP_TIGER_MACHINE_TAG == popBtnTag)
	{
        isMiniGameing = true;
        pContentNode->runAction(CCMoveTo::create(0.3f, CCPointMake(this->originPoint.x, this->originPoint.y)));
		//this->addChild(this->createTigerMachineLayer(),100,TIGER_MACHINE_LAYER_TAG);
	}
	else if (POPUP_GUAGUALE_MACHINE_TAG == popBtnTag)
	{
        isMiniGameing = true;
        pContentNode->runAction(CCMoveTo::create(0.3f, CCPointMake(this->originPoint.x, this->originPoint.y)));
		//this->addChild(this->createGuaguaGameLayer(),100,GUAGUALE_MACHINE_LAYER_TAG);
	}
	else if(popBtnTag == CLOSE_TIGER_MACHINE_TAG)
	{
        pContentNode->runAction(CCMoveTo::create(0.3f, CCPointMake(this->originPoint.x, this->originPoint.y)));
	}
}

void StartScene::tigerMachineLayerCallback(CCNode* pSender)
{
    int popBtnTag = pSender->getTag();
    if (CLOSE_TIGER_MACHINE_TAG == popBtnTag)
    {
        isMiniGameing = false;
        
        isTigetMachineShow = false;
        
        const char* chipStr = StringFormat::intsert(PlayerModel::getInstance()->money,3,",");
        const char* paperMoneyStr = StringFormat::intsert(PlayerModel::getInstance()->rmb,3,",");
        //pShipIconText->setText(chipStr);
        //pRmbIconText->setText(paperMoneyStr);
    }
}

void StartScene::guaguaMachineLayerCallback(CCNode* pSender)
{
    int popBtnTag = pSender->getTag();
    if (CLOSE_GUAGUALE_MACHINE_TAG == popBtnTag)
    {
        isMiniGameing = false;
        
        isGuagualeMachineShow = false;
        
        const char* chipStr = StringFormat::intsert(PlayerModel::getInstance()->money,3,",");
        const char* paperMoneyStr = StringFormat::intsert(PlayerModel::getInstance()->rmb,3,",");
        //pShipIconText->setText(chipStr);
        //pRmbIconText->setText(paperMoneyStr);
    }
}

//PopupExitGameLayer* StartScene::createPopupExitGameLayer()
//{
//	PopupExitGameLayer* popupExitGameLayer = PopupExitGameLayer::create();
//	popupExitGameLayer->setCallbackFunc(this, callfuncN_selector(StartScene::popLayerExitGameCallback));
//    popupExitGameLayer->addMenu("common/pop_btn_left_1.png", "common/pop_btn_left_1.png","确 定",CCPointMake(10,11), POPUP_CTRLBTN_OK_TAG);
//    popupExitGameLayer->addMenu("common/pop_btn_right_1.png", "common/pop_btn_right_1.png","取 消",CCPointMake(popupExitGameLayer->contentSize.width-168,11), POPUP_CTRLBTN_CANCEL_TAG);
//	popupExitGameLayer->setAnchorPoint(Vec2::ZERO);
//	popupExitGameLayer->setPosition(this->originPoint);
//    
//	return popupExitGameLayer;
//}
//
//MiniGameLayer* StartScene::createMiniGameLayer()
//{
//    MiniGameLayer* miniGameLayer = MiniGameLayer::create();
//	miniGameLayer->setCallbackFunc(this, callfuncN_selector(StartScene::miniGameCallback));
//	miniGameLayer->setAnchorPoint(Vec2::ZERO);
//	miniGameLayer->setPosition(this->originPoint);
//    
//	return miniGameLayer;
//}
//
//WeixinShareLayer* StartScene::createWeixinShareLayer()
//{
//    isWeixinShareShow = true;
//    
//    WeixinShareLayer* weixinShareLayer = WeixinShareLayer::create();
//    weixinShareLayer->setCallbackFunc(this, callfuncN_selector(StartScene::weixinShareLayerCallback));
//	weixinShareLayer->setAnchorPoint(Vec2::ZERO);
//	weixinShareLayer->setPosition(this->originPoint);
//    
//	return weixinShareLayer;
//}

void StartScene::weixinShareLayerCallback(CCNode* pSender)
{
    isWeixinShareShow = false;
}

//GuaGuaMachine* StartScene::createGuaguaGameLayer()
//{
//    GuaGuaMachine* guaGuaMachine = GuaGuaMachine::create();
//	guaGuaMachine->setCallbackFunc(this, callfuncN_selector(StartScene::guaguaMachineLayerCallback));
//	guaGuaMachine->setAnchorPoint(Vec2::ZERO);
//	guaGuaMachine->setPosition(this->originPoint);
//    
//    isGuagualeMachineShow = true;
//    
//	return guaGuaMachine;
//}
//
//TigerMachineLayer* StartScene::createTigerMachineLayer()
//{
//    TigerMachineLayer* tigerMachineLayer = TigerMachineLayer::create();
//	tigerMachineLayer->setCallbackFunc(this, callfuncN_selector(StartScene::tigerMachineLayerCallback));
//	tigerMachineLayer->setAnchorPoint(Vec2::ZERO);
//	tigerMachineLayer->setPosition(this->originPoint);
//    
//    isTigetMachineShow = true;
//    
//	return tigerMachineLayer;
//}
//
//PopupDailyLoginLayer* StartScene::createPopupDailyLoginLayer()
//{
//	PopupDailyLoginLayer* popupDailyLoginLayer = PopupDailyLoginLayer::create();
//	popupDailyLoginLayer->setAnchorPoint(Vec2::ZERO);
//	popupDailyLoginLayer->setPosition(this->originPoint);
//    
//	return popupDailyLoginLayer;
//}

void StartScene::updateRichInfo()
{
    if (!PlayerModel::getInstance()->is_get_login_packer)
    {
        return;
    }
    
	const char* shipStr = StringFormat::intsert(PlayerModel::getInstance()->money,3,",");
    //if (pShipIconText != NULL)
    //{
    //    pShipIconText->setText(shipStr);
    //}
	
	const char* rmbStr = StringFormat::intsert(PlayerModel::getInstance()->rmb,3,",");
    //if (pRmbIconText != NULL)
    //{
    //    pRmbIconText->setText(rmbStr);
    //}
	
}














