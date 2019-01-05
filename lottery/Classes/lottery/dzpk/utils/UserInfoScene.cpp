//
//  UserInfoScene.cpp
//  TexasPoker
//
//  Created by peniel on 12/31/13.
//
//

#include "UserInfoScene.h"
//#include "StartScene.h"
//#include "RegisterScene.h"
//#include "ClipLayer.h"
//#include "ExpToolSprite.h"
#include "StringFormat.h"
#include "PlayerModel.h"

UserInfoScene::UserInfoScene():
playerNameTTF(NULL),
pManTag(NULL),
pWManTag(NULL),
pUserHeader(NULL),
planCtrlBtn(NULL),
isMan(true),
isClickBack(false),
//pEditBox(NULL),
pEditBoxFrame(NULL),
//pRow1(NULL),
//pShipIconText(NULL),
//pRmbIconText(NULL),
winCountTTF(NULL),
failureCountTTF(NULL),
winSumTTF(NULL),
isPopupTipLayerShow(false)
{
    
}

UserInfoScene::~UserInfoScene()
{
	CC_SAFE_RELEASE_NULL(playerNameTTF);
	CC_SAFE_RELEASE_NULL(pManTag);
	CC_SAFE_RELEASE_NULL(pWManTag);
	CC_SAFE_RELEASE_NULL(pUserHeader);
	CC_SAFE_RELEASE_NULL(planCtrlBtn);

	CC_SAFE_RELEASE_NULL(pEditBoxFrame);
	//CC_SAFE_RELEASE_NULL(pRow1);
    
	//CC_SAFE_RELEASE_NULL(pShipIconText);
	//CC_SAFE_RELEASE_NULL(pRmbIconText);
	CC_SAFE_RELEASE_NULL(winCountTTF);
	CC_SAFE_RELEASE_NULL(failureCountTTF);
	CC_SAFE_RELEASE_NULL(winSumTTF);
}

CCScene* UserInfoScene::scene()
{
    cocos2d::CCScene* scene = NULL;
    do {
        scene = cocos2d::CCScene::create();
        CC_BREAK_IF(!scene);
        
        UserInfoScene* layer = UserInfoScene::create();
        CC_BREAK_IF(!layer);
        
        scene->addChild(layer);
        
    } while (0);
    return scene;
}

bool UserInfoScene::init()
{
    bool bRect = false;
    
    do {
        CC_BREAK_IF(!BaseNode::init());
        
        this->initUI();
        
        bRect = true;
        
    } while (0);
    
    return  bRect;
}

void UserInfoScene::initUI()
{
    this->setBackground(this->pStartBgTexture);
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    CCSprite* pTopBg = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("top_bg.png"));
    
    CCSize topSize = pTopBg->getContentSize();
    infoNodeSize = CCSizeMake(this->visibleW, this->visibleH-topSize.height+20);
    
    CCNode* pTopNode = CCNode::create();
    pTopNode->setContentSize(topSize);
    pTopNode->setAnchorPoint(ccp(0.0f,1.0f));
    pTopNode->setPosition(ccp(this->originPoint.x,this->originPoint.y+this->visibleH+1));
    this->addChild(pTopNode);
    
    CCNode* pUserInfoNode = CCNode::create();
    pUserInfoNode->setContentSize(infoNodeSize);
    pUserInfoNode->setAnchorPoint(Point::ZERO);
    pUserInfoNode->setPosition(this->originPoint);
    this->addChild(pUserInfoNode);
    //---------------------------------------------------------------------------------------------------------
    pTopBg->setAnchorPoint(Point::ZERO);
    pTopBg->setPosition(Point::ZERO);
    pTopNode->addChild(pTopBg);

    CCLabelTTF* pTitle = CCLabelTTF::create("玩家信息","Arial",30);
	pTitle->setAnchorPoint(ccp(0.5f,0.5f));
	pTitle->setPosition(ccp(topSize.width/2,topSize.height/2+10));
	pTopNode->addChild(pTitle);
    
    //ControlButton* backCtrlBtn = CtrlManager::createCtrlBtnWithFreame("cricle_btn.png","");
    //backCtrlBtn->setAnchorPoint(ccp(1.0f,0.5f));
    //backCtrlBtn->setPosition(ccp(topSize.width-20,topSize.height/2+10));
    //backCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(UserInfoScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
    //pTopNode->addChild(backCtrlBtn,0,BACK_PREV_SCENE_TAG);
    
    CCSprite* backIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("back_icon.png"));
    backIcon->setAnchorPoint(ccp(0.5f,0.5f));
    //backIcon->setPosition(ccp(backCtrlBtn->getPreferredSize().width/2,backCtrlBtn->getPreferredSize().height/2));
    //backCtrlBtn->addChild(backIcon);
    
    //---------------------------------------------------------------------------------------------------------
    CCSprite* pUserHeadFrame = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("header_frame.png"));
    CCSize headerFrameSize = pUserHeadFrame->boundingBox().size;
    pUserHeadFrame->setAnchorPoint(ccp(0.0f,1.0f));
    pUserHeadFrame->setPosition(ccp(16,infoNodeSize.height-16));
    pUserInfoNode->addChild(pUserHeadFrame,1);
    
    const char* genderPicName = PlayerModel::getInstance()->sex==1 ? "default_man_header.png" : "default_wman_header.png";
    std::string headerFile = "no_custom_head";
    //CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid,headerFile);
    if (headerFile.compare("no_custom_head")==0)
    {
        isCustomHeader = false;
        pUserHeader = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName(genderPicName));
        CCSize headerSize = pUserHeader->boundingBox().size;
        pUserHeader->setAnchorPoint(ccp(0.0f,1.0f));
        pUserHeader->setPosition(ccp(20,infoNodeSize.height-20));
    }
    else
    {
        isCustomHeader = true;
        
        CCSprite* pTemp = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName(genderPicName));
        CCSize tempSize = pTemp->boundingBox().size;
        tempSize = CCSizeMake(tempSize.width-4, tempSize.height-4);
        
        //CCTexture2D* pHeaderTexture = CCTextureCache::sharedTextureCache()->addImage(CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid).c_str());
        //pUserHeader = CCSprite::createWithTexture(pHeaderTexture);
        //CCSize avatarSize = pHeaderTexture->getContentSize();
        //float scaleX = tempSize.width / avatarSize.width;
        //float scaleY = tempSize.height / avatarSize.height;
        //pUserHeader->setScaleX(scaleX);
        //pUserHeader->setScaleY(scaleY);
        //CCSize headerSize = pUserHeader->boundingBox().size;
        //pUserHeader->setAnchorPoint(ccp(0.0f,1.0f));
        //pUserHeader->setPosition(ccp(22,infoNodeSize.height-22));
    }
    CC_SAFE_RETAIN(pUserHeader);
    pUserInfoNode->addChild(pUserHeader,2);
    
    CCSprite* pModifyHeaderTextBg = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("modify_header_text_bg.png"));
    pModifyHeaderTextBg->setAnchorPoint(ccp(0.0f,1.0f));
	pModifyHeaderTextBg->setPosition(ccp(15+headerFrameSize.width/2-pModifyHeaderTextBg->getContentSize().width/2,infoNodeSize.height-10-headerFrameSize.height+pModifyHeaderTextBg->getContentSize().height-2));
	pUserInfoNode->addChild(pModifyHeaderTextBg,2);
    
    CCLabelTTF* changeHeaderText = CCLabelTTF::create("更换头像","Arial",20);
	changeHeaderText->setAnchorPoint(ccp(0.5f,0.5f));
	changeHeaderText->setPosition(ccp(pModifyHeaderTextBg->getContentSize().width/2,pModifyHeaderTextBg->getContentSize().height/2));
	pModifyHeaderTextBg->addChild(changeHeaderText,2);
    
    //ControlButton* changeHeaderCtrlBtn = CtrlManager::createCtrlBtnWithFreame("header_frame.png", "");
    //changeHeaderCtrlBtn->setAnchorPoint(ccp(0.0f,1.0f));
    //changeHeaderCtrlBtn->setPosition(pUserHeadFrame->getPosition());
    //changeHeaderCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(UserInfoScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
    //pUserInfoNode->addChild(changeHeaderCtrlBtn,0,CHANGE_USER_HEADER_TAG);
    //------------------------------------------------------------------------------------------------
    UNS_INT rowHeight = 50;
    UNS_INT rowWidth = 500;
    //pRow1 = ClipLayer::create(CCSizeMake(rowWidth, rowHeight));
    //CC_SAFE_RETAIN(pRow1);
    //pRow1->setAnchorPoint(ccp(0.0f,1.0f));
    //pRow1->setPosition(ccp(20+headerFrameSize.width+30,pUserHeader->getPositionY()-rowHeight));
    //pUserInfoNode->addChild(pRow1);
    
    CCSprite* pSpLine = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("h_line.png"));
    pSpLine->setAnchorPoint(Point::ZERO);
    pSpLine->setPosition(Point::ZERO);
    //pRow1->addChild(pSpLine);
    
    const char* playerName = PlayerModel::getInstance()->name.c_str();
    playerNameTTF = CCLabelTTF::create(playerName,"Arial",20);
    CC_SAFE_RETAIN(playerNameTTF);
	CCSize textSize = playerNameTTF->getContentSize();
	playerNameTTF->setAnchorPoint(Point::ZERO);
	playerNameTTF->setPosition(ccp(5,rowHeight/2-textSize.height/2));
	//pRow1->addChild(playerNameTTF);
    
    editboxPoint = CCPointMake(this->originPoint.x+20+headerFrameSize.width+30+5, this->originPoint.y+infoNodeSize.height-rowHeight-20);
    pEditBoxFrame = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("edit_box.png"));
    CC_SAFE_RETAIN(pEditBoxFrame);
    pEditBoxFrame->setAnchorPoint(Point::ZERO);
    pEditBoxFrame->setPosition(ccp(0,rowHeight/2-pEditBoxFrame->getContentSize().height/2));
    pEditBoxFrame->setVisible(false);
    //pRow1->addChild(pEditBoxFrame);
    
    //planCtrlBtn = CtrlManager::createCtrlBtnWithFile("common/transparent_bg.png", "", CCSizeMake(50, 50));
    //CC_SAFE_RETAIN(planCtrlBtn);
    //planCtrlBtn->setAnchorPoint(Point::ZERO);
    //planCtrlBtn->setPosition(ccp(playerNameTTF->getPositionX()+textSize.width+5,rowHeight/2-planCtrlBtn->getPreferredSize().height/2));
    //planCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(UserInfoScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
    //pRow1->addChild(planCtrlBtn,0,POPUP_MODIFY_NAME_TAG);
    
    CCSprite* planSprite = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("modify_plan.png"));
    planSprite->setAnchorPoint(ccp(0.5f,0.5f));
	planSprite->setPosition(ccp(planCtrlBtn->getPreferredSize().width/2,planCtrlBtn->getPreferredSize().height/2));
	planCtrlBtn->addChild(planSprite);
    
    //ControlButton* manTagCtrlBtn = CtrlManager::createCtrlBtnWithFile("common/transparent_bg.png", "", CCSizeMake(50, 50));
    //manTagCtrlBtn->setAnchorPoint(Point::ZERO);
    //manTagCtrlBtn->setPosition(ccp(300,rowHeight/2-manTagCtrlBtn->getPreferredSize().height/2));
    //manTagCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(UserInfoScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
    //pRow1->addChild(manTagCtrlBtn,0,USER_MAN_HEADER_TAG);
    
    pManTag = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("radio_btn_check.png"));
    CC_SAFE_RETAIN(pManTag);
    pManTag->setAnchorPoint(ccp(0.5f,0.5f));
	//pManTag->setPosition(ccp(manTagCtrlBtn->getPreferredSize().width/2,manTagCtrlBtn->getPreferredSize().height/2));
	//manTagCtrlBtn->addChild(pManTag);
    
    /*CCLabelTTF* manTagText = CCLabelTTF::create("男","Arial",20);
	manTagText->setAnchorPoint(ccp(0.5f,0.5f));*/
	//manTagText->setPosition(ccp(manTagCtrlBtn->getPreferredSize().width/2+30,manTagCtrlBtn->getPreferredSize().height/2));
	//manTagCtrlBtn->addChild(manTagText);
    
    //ControlButton* wmanTagCtrlBtn = CtrlManager::createCtrlBtnWithFile("common/transparent_bg.png", "", CCSizeMake(50, 50));
    //wmanTagCtrlBtn->setAnchorPoint(Point::ZERO);
    //wmanTagCtrlBtn->setPosition(ccp(430,rowHeight/2-wmanTagCtrlBtn->getPreferredSize().height/2));
    //wmanTagCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(UserInfoScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
    //pRow1->addChild(wmanTagCtrlBtn,0,USER_WMAN_HEADER_TAG);
    
    pWManTag = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("radio_btn_check.png"));
    CC_SAFE_RETAIN(pWManTag);
    pWManTag->setAnchorPoint(ccp(0.5f,0.5f));
	//pWManTag->setPosition(ccp(manTagCtrlBtn->getPreferredSize().width/2,manTagCtrlBtn->getPreferredSize().height/2));
	//wmanTagCtrlBtn->addChild(pWManTag);
    
    /*CCLabelTTF* wmanTagText = CCLabelTTF::create("女","Arial",20);
	wmanTagText->setAnchorPoint(ccp(0.5f,0.5f));*/
	//wmanTagText->setPosition(ccp(wmanTagCtrlBtn->getPreferredSize().width/2+30,wmanTagCtrlBtn->getPreferredSize().height/2));
	//wmanTagCtrlBtn->addChild(wmanTagText);
    
    if (PlayerModel::getInstance()->auto_register == 1)
    {
        //ControlButton* finishAccountCtrlBtn = CtrlManager::createCtrlBtnWithFreame(frameCache->spriteFrameByName("btn_finish_account.png"), "完善账号",16,-1);
        //finishAccountCtrlBtn->setAnchorPoint(Point::ZERO);
        //finishAccountCtrlBtn->setPosition(ccp(410,rowHeight/2-finishAccountCtrlBtn->getPreferredSize().height/2));
        //finishAccountCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(UserInfoScene::ctrlBtnCallbck), CCControlEventTouchUpInside);
        //pRow1->addChild(finishAccountCtrlBtn,0,FINISH_ACCOUNT_TAG);

        //manTagCtrlBtn->setPosition(ccp(200,rowHeight/2-manTagCtrlBtn->getPreferredSize().height/2));
        //wmanTagCtrlBtn->setPosition(ccp(300,rowHeight/2-wmanTagCtrlBtn->getPreferredSize().height/2));
    }
    
    PlayerModel::getInstance()->sex==1 ? pWManTag->initWithSpriteFrame(frameCache->spriteFrameByName("radio_btn.png")) : pManTag->initWithSpriteFrame(frameCache->spriteFrameByName("radio_btn.png"));
    isMan = PlayerModel::getInstance()->sex==1 ? true : false;
    //------------------------------------------------------------------------------------------------
    //ClipLayer* pRow2 = ClipLayer::create(CCSizeMake(rowWidth, rowHeight));
    //pRow2->setAnchorPoint(ccp(0.0f,1.0f));
    //pRow2->setPosition(ccp(20+headerFrameSize.width+30,pUserHeader->getPositionY()-rowHeight*2.0f));
    //pUserInfoNode->addChild(pRow2);
    
    pSpLine = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("h_line.png"));
    pSpLine->setAnchorPoint(Point::ZERO);
    pSpLine->setPosition(Point::ZERO);
    //pRow2->addChild(pSpLine);
    
    const char* uidStr = CCString::createWithFormat("ID: %d",PlayerModel::getInstance()->uid)->getCString();
    CCLabelTTF* uidTTF = CCLabelTTF::create(uidStr,"Arial",20);
    textSize = uidTTF->getContentSize();
	uidTTF->setAnchorPoint(Point::ZERO);
	uidTTF->setPosition(ccp(5,rowHeight/2-textSize.height/2));
	//pRow2->addChild(uidTTF);
    
    //const char* lvStr = CCString::createWithFormat("LV: %d",PlayerModel::getInstance()->vlevel)->getCString();
    const char* lvStr = CCString::createWithFormat("LV: %d",PlayerModel::getInstance()->getLevelByExp(PlayerModel::getInstance()->exp))->getCString();
    CCLabelTTF* lvTTF = CCLabelTTF::create(lvStr,"Arial",20);
    textSize = lvTTF->getContentSize();
	lvTTF->setAnchorPoint(Point::ZERO);
	lvTTF->setPosition(ccp(200,rowHeight/2-textSize.height/2));
	//pRow2->addChild(lvTTF);
    
 //   ExpToolSprite* expSprite = ExpToolSprite::createWithFrame(frameCache->spriteFrameByName("exp_tool_bg.png"), frameCache->spriteFrameByName("exp_tool.png"));
	//expSprite->setAnchorPoint(Point::ZERO);
	////expSprite->updatePercent(0.8f);
    int nextExp = 0;
    float percent = PlayerModel::getInstance()->getCurrExpPercentage(nextExp,PlayerModel::getInstance()->exp);
	//expSprite->updatePercent(percent,nextExp,PlayerModel::getInstance()->exp);
	//expSprite->setPosition(ccp(rowWidth-expSprite->getContentSize().width-5,rowHeight/2-expSprite->getContentSize().height/2));
	//pRow2->addChild(expSprite);
    //------------------------------------------------------------------------------------------------
    /*ClipLayer* pRow3 = ClipLayer::create(CCSizeMake(rowWidth, rowHeight));
    pRow3->setAnchorPoint(ccp(0.0f,1.0f));
    pRow3->setPosition(ccp(20+headerSize.width+30,pUserHeader->getPositionY()-rowHeight*3.0f));
    pUserInfoNode->addChild(pRow3);
    
    pSpLine = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("h_line.png"));
    pSpLine->setAnchorPoint(Point::ZERO);
    pSpLine->setPosition(Point::ZERO);
    pRow3->addChild(pSpLine);
    
    const char* maxRichStr = CCString::createWithFormat("获得最高财富: %s",StringFormat::intsert(PlayerModel::getInstance()->max_ship,3,","))->getCString();
    CCLabelTTF* maxRichTTF = CCLabelTTF::create(maxRichStr,"Arial",20);
    textSize = maxRichTTF->getContentSize();
	maxRichTTF->setAnchorPoint(Point::ZERO);
	maxRichTTF->setPosition(ccp(5,rowHeight/2-textSize.height/2));
	pRow3->addChild(maxRichTTF);*/
    //------------------------------------------------------------------------------------------------
    //ClipLayer* pRow4 = ClipLayer::create(CCSizeMake(rowWidth, rowHeight));
    //pRow4->setAnchorPoint(ccp(0.0f,1.0f));
    //pRow4->setPosition(ccp(20+headerFrameSize.width+30,pUserHeader->getPositionY()-rowHeight*3.0f));
    //pUserInfoNode->addChild(pRow4);
    
    pSpLine = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("h_line.png"));
    pSpLine->setAnchorPoint(Point::ZERO);
    pSpLine->setPosition(Point::ZERO);
    //pRow4->addChild(pSpLine);
    
    CCString* winCount = CCString::createWithFormat("胜局: %d",PlayerModel::getInstance()->total_win);
    winCountTTF = CCLabelTTF::create(winCount->getCString(),"Arial",18);
    CC_SAFE_RETAIN(winCountTTF);
	winCountTTF->setAnchorPoint(Point::ZERO);
    textSize = winCountTTF->getContentSize();
	winCountTTF->setPosition(ccp(5,rowHeight/2-textSize.height/2));
	//pRow4->addChild(winCountTTF);
    
    CCString* failureCount = CCString::createWithFormat("败局: %d",PlayerModel::getInstance()->total_board-PlayerModel::getInstance()->total_win);
    CCLabelTTF* failureCountTTF = CCLabelTTF::create(failureCount->getCString(),"Arial",18);
    CC_SAFE_RETAIN(failureCountTTF);
	failureCountTTF->setAnchorPoint(Point::ZERO);
    textSize = failureCountTTF->getContentSize();
	failureCountTTF->setPosition(ccp(200,rowHeight/2-textSize.height/2));
	//pRow4->addChild(failureCountTTF);
    
    CCString* winSum = PlayerModel::getInstance()->total_board<=0 ?
                       CCString::createWithFormat("胜率: %.0f%s",0.0f,"%") :
                       CCString::createWithFormat("胜率: %.0f%s",(float)PlayerModel::getInstance()->total_win*100.0f/(float)PlayerModel::getInstance()->total_board,"%");
    CCLabelTTF* winSumTTF = CCLabelTTF::create(winSum->getCString(),"Arial",18);
    CC_SAFE_RETAIN(winSumTTF);
	winSumTTF->setAnchorPoint(Point::ZERO);
    textSize = winSumTTF->getContentSize();
	winSumTTF->setPosition(ccp(400,rowHeight/2-textSize.height/2));
	//pRow4->addChild(winSumTTF);
    //------------------------------------------------------------------------------------------------
    rowWidth = rowWidth+headerFrameSize.width;
    //ClipLayer* pRow5 = ClipLayer::create(CCSizeMake(rowWidth, rowHeight));
    //pRow5->setAnchorPoint(ccp(0.0f,1.0f));
    //pRow5->setPosition(ccp(50,pUserHeader->getPositionY()-rowHeight*4.0f));
    //pUserInfoNode->addChild(pRow5);
    
    pSpLine = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("h_line.png"));
    pSpLine->setAnchorPoint(Point::ZERO);
    pSpLine->setPosition(Point::ZERO);
    //pRow5->addChild(pSpLine);
    
    CCSprite* richIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("ship_icon.png"));
    
 //   pShipIconText = IconTextSprite::createWith(richIcon,StringFormat::intsert(PlayerModel::getInstance()->money,3,","));
 //   CC_SAFE_RETAIN(pShipIconText);
 //   pShipIconText->setFontSize(18);
	//pShipIconText->setAnchorPoint(Point::ZERO);
	//pShipIconText->setPosition(ccp(pRow1->getPositionX()+20,rowHeight/2-pShipIconText->getContentSize().height/2));
	//pRow5->addChild(pShipIconText);
    
    richIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("gold_icon.png"));
	//pRmbIconText = IconTextSprite::createWith(richIcon,StringFormat::intsert(PlayerModel::getInstance()->rmb,3,","));
 //   CC_SAFE_RETAIN(pRmbIconText);
 //   pRmbIconText->setFontSize(18);
	//pRmbIconText->setAnchorPoint(ccp(0.0f,0.0f));
	//pRmbIconText->setPosition(ccp(rowWidth-pRmbIconText->getContentSize().width-100,rowHeight/2-pRmbIconText->getContentSize().height/2));
	//pRow5->addChild(pRmbIconText);
    
    /*richIcon = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("reward_card_icon.png"));
	IconTextSprite* pRewardIconText = IconTextSprite::createWith(richIcon,StringFormat::intsert(PlayerModel::getInstance()->reward_card_count,3,","));
    pRewardIconText->setFontSize(18);
	pRewardIconText->setAnchorPoint(Point::ZERO);
	pRewardIconText->setPosition(ccp(rowWidth-pRewardIconText->getContentSize().width-10,rowHeight/2-pRewardIconText->getContentSize().height/2));
	pRow5->addChild(pRewardIconText);*/
    //------------------------------------------------------------------------------------------------
    /*CCSize pRow6Size = CCSizeMake(rowWidth, infoNodeSize.height-rowHeight*4.0-20);
    ClipLayer* pRow6 = ClipLayer::create(pRow6Size);
    pRow6->setAnchorPoint(Point::ZERO);
    pRow6->setPosition(ccp(50,0));
    pUserInfoNode->addChild(pRow6);*/

    CCSprite* cardSprite = CCSprite::create(CCString::createWithFormat("rooms/card/card_%s.png","00")->getCString());
    CCSize cardSize = cardSprite->getContentSize();
    
    CCLabelTTF* peakCardsTextTTF = CCLabelTTF::create("最佳牌型:","Arial-BoldMT",18);

    /*float cardMargin = (pRow6Size.width-peakCardsTextTTF->getContentSize().width-5)/5.0f;
	CCPoint cardsPanelStart = CCPointMake(cardMargin,pRow6Size.height/2-cardSize.height/2);
    
    peakCardsTextTTF->setAnchorPoint(Point::ZERO);
	peakCardsTextTTF->setPosition(ccp(5,cardsPanelStart.y+cardSize.height-peakCardsTextTTF->getContentSize().height));
	pRow6->addChild(peakCardsTextTTF);*/
    
	for (int i=0;i<5;++i)
	{
		CCSprite* cardSprite = CCSprite::create(CCString::createWithFormat("rooms/card/card_%s.png",PlayerModel::getInstance()->best_cards[i].c_str())->getCString());
		cardSprite->setAnchorPoint(Point::ZERO);
		//cardSprite->setPosition(ccp(cardsPanelStart.x+i*(peakCardsTextTTF->getContentSize().width+30),cardsPanelStart.y));
		//pRow6->addChild(cardSprite);
	}
}

void UserInfoScene::onEnterTransitionDidFinish()
{
    //务必调用
    Layer::onEnterTransitionDidFinish();
}

void UserInfoScene::onEnter()
{
	Layer::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(UserInfoScene::notifiUpdateRichCallback),NOTIFI_UPDATE_RICHINFO_KEY,NULL);
}

void UserInfoScene::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTIFI_UPDATE_RICHINFO_KEY);
	Layer::onExit();
}

void UserInfoScene::notifiUpdateRichCallback(CCObject* pCCObjData)
{
    //DLog::showLog("userinfo scene heart beat callback");
    
    this->updateTopNodeRich();
    
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
    
    CCString* winCount = CCString::createWithFormat("胜局: %d",PlayerModel::getInstance()->total_win);
    if (winCountTTF != NULL)
    {
        winCountTTF->setString(winCount->getCString());
    }
    
    CCString* failureCount = CCString::createWithFormat("败局: %d",PlayerModel::getInstance()->total_board-PlayerModel::getInstance()->total_win);
    if (failureCountTTF != NULL)
    {
        failureCountTTF->setString(failureCount->getCString());
    }
    
    CCString* winSum = PlayerModel::getInstance()->total_board<=0 ?
    CCString::createWithFormat("胜率: %.0f%s",0.0f,"%") :
    CCString::createWithFormat("胜率: %.0f%s",(float)PlayerModel::getInstance()->total_win*100.0f/(float)PlayerModel::getInstance()->total_board,"%");
    if (winSumTTF != NULL)
    {
        winSumTTF->setString(winSum->getCString());
    }
}

bool UserInfoScene::isCheckCommit()
{
    if (PlayerModel::getInstance()->sex==1 && !isMan)
    {
        return true;
    }
    else if (PlayerModel::getInstance()->sex!=1 && isMan)
    {
        return true;
    }
    else if (PlayerModel::getInstance()->name != playerNameTTF->getString())
    {
        return true;
    }
    return false;
}

//void UserInfoScene::keyBackClicked()
//{
//    Sound::getInstance()->playEffect("sound/sound_dianji.ogg");
//    
//    if (isPopupTipLayerShow)
//    {
//        return;
//    }
//    
//    if (isClickBack)
//    {
//        return;
//    }
//    isClickBack = true;
//    
//    if (this->isCheckCommit())
//    {
//        isPopupTipLayerShow = true;
//        this->createPopupTipLayer("修改信息", "返回前,你要保存修改的信息么?", MODIFY_USER_INFO_TIP);
//    }
//    else
//    {
//        CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, StartScene::scene(),ccBLACK);
//        CCDirector::sharedDirector()->replaceScene(reScene);
//    }
//}

void UserInfoScene::popupTipLayerCallback(CCNode* pSender)
{
    int popBtnTag = pSender->getTag();
    if (this->currTipTag == MODIFY_USER_INFO_TIP)
    {
        if (POPUP_CTRLBTN_OK_TAG == popBtnTag)
        {
            //DLog::showLog("modify user info.....");
            
            int sex = isMan ? 1 : 2;
            //CCHttpAgent::getInstance()->doModifyInfo(playerNameTTF->getString(), sex);
            //this->createLoading();
            //this->schedule(schedule_selector(UserInfoScene::modifyUserInfoCallback), 0.1f);
        }
        else
        {
            //DLog::showLog("back.....");
            
            //CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, StartScene::scene(),ccBLACK);
            //CCDirector::sharedDirector()->replaceScene(reScene);
        }
    }
    isPopupTipLayerShow = false;
}

void UserInfoScene::modifyUserInfoCallback(float dt)
{
    //CCHttpPacket& modifyinfo = CCHttpAgent::getInstance()->packets[atoi(string(CMD_MODIFYINFO).c_str()) - 1000];
    //
    //if (modifyinfo.status != 0)
    //{
    //    if (modifyinfo.status != 3)
    //    {
    //        this->stopLoading();
    //        isClickBack = false;
    //        isPopupTipLayerShow = true;
    //        std::string contentStr = "修改用户信息错误!";
    //        this->createPopupTipLayer("错误提示", contentStr.c_str(),MODIFY_USER_ERROR_TIP);
    //        this->unschedule(schedule_selector(UserInfoScene::modifyUserInfoCallback));
    //        return;
    //    }
    //    int ret = modifyinfo.recvVal["ret"].asInt();
    //    if (ret != 0)
    //    {
    //        this->stopLoading();
    //        isClickBack = false;
    //        isPopupTipLayerShow = true;
    //        std::string contentStr = "修改用户信息错误!";
    //        if (ret == 313)
    //        {
    //            contentStr = "用户名已经存在，请输入其他名字!";
    //        }
    //        this->createPopupTipLayer("错误提示", contentStr.c_str(),MODIFY_USER_ERROR_TIP);
    //        this->unschedule(schedule_selector(UserInfoScene::modifyUserInfoCallback));
    //        return;
    //    }
    //    
    //    PlayerModel::getInstance()->sex = modifyinfo.recvVal["data"]["sex"].asInt();
    //    PlayerModel::getInstance()->name = modifyinfo.recvVal["data"]["name"].asString();
    //    
    //    this->stopLoading();
    //    this->unschedule(schedule_selector(UserInfoScene::modifyUserInfoCallback));
    //    isClickBack = false;
    //    
    //    CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, StartScene::scene(),ccBLACK);
    //    CCDirector::sharedDirector()->replaceScene(reScene);
    //}
}

void UserInfoScene::ctrlBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event)
{
    Sound::getInstance()->playEffect("sound/sound_dianji.ogg");
    
    ControlButton* ctrlBtn = (ControlButton*)pSender;
	UNS_INT ctrlBtnTag = ctrlBtn->getTag();
	//DLog::showLog("ctrlBtn tag: %d",ctrlBtnTag);
    
    if (ctrlBtnTag == BACK_PREV_SCENE_TAG)
    {
        keyBackClicked();
    }
    else if (ctrlBtnTag == CHANGE_USER_HEADER_TAG)
    {
        //DLog::showLog("change header.......");
        this->modifyUserHeader();
    }
    else if (ctrlBtnTag == FINISH_ACCOUNT_TAG)
    {
        //CCTransitionScene*  reScene = CCTransitionFade::create(0.5f, RegisterScene::sceneWithComeUserInfo(),ccBLACK);
        //CCDirector::sharedDirector()->replaceScene(reScene);
    }
    else if (ctrlBtnTag == USER_MAN_HEADER_TAG)
    {
        //DLog::showLog("man header.......");
        if (!isCustomHeader)
        {
            pUserHeader->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("default_man_header.png"));
            pUserHeader->setAnchorPoint(ccp(0.0f,1.0f));
        }
        
        pManTag->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("radio_btn_check.png"));
        pWManTag->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("radio_btn.png"));
        isMan = true;
    }
    else if (ctrlBtnTag == USER_WMAN_HEADER_TAG)
    {
        //DLog::showLog("wman header.......");
        if (!isCustomHeader)
        {
            pUserHeader->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("default_wman_header.png"));
            pUserHeader->setAnchorPoint(ccp(0.0f,1.0f));
        }
        
        pManTag->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("radio_btn.png"));
        pWManTag->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("radio_btn_check.png"));
        isMan = false;
    }
    else if (ctrlBtnTag == POPUP_MODIFY_NAME_TAG)
    {
        //DLog::showLog("plan.......");
        
        this->showEditBox();
    }
}

void UserInfoScene::modifyUserHeader()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        MiscValue::avatar = 0;
        showAvatarDialog(PlayerModel::getInstance()->uid);
        this->schedule(schedule_selector(UserInfoScene::modifyUserHeaderCallback), 0.1f);
    #else
        this->createToastLayer("sorry,目前IOS不支持该功能.", 2.0f);
    #endif
}

void UserInfoScene::modifyUserHeaderCallback(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (MiscValue::avatar != 0)
    {
        CCLog("MiscValue::avatar[%d]", MiscValue::avatar);
        if (MiscValue::avatar == 2)
        {
            std::string path = geAvatarPath(PlayerModel::getInstance()->uid);
            
            CCSize headerFrameSize = pUserHeader->boundingBox().size;
            
            CCTextureCache::sharedTextureCache()->removeTextureForKey(CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid).c_str());
            CCTexture2D* pHeaderTexture = CCTextureCache::sharedTextureCache()->addImage(CCHttpAgent::getInstance()->getImagePath(PlayerModel::getInstance()->uid).c_str());
            
            CCSize avatarSize = pHeaderTexture->getContentSize();
            float scaleX = headerFrameSize.width / avatarSize.width;
            float scaleY = headerFrameSize.height / avatarSize.height;
            pUserHeader->initWithTexture(pHeaderTexture);
            pUserHeader->setScaleX(scaleX);
            pUserHeader->setScaleY(scaleY);
            pUserHeader->setAnchorPoint(ccp(0.0f,1.0f));
            pUserHeader->setPosition(ccp(22,infoNodeSize.height-22));
            
            isCustomHeader = true;
            
            doPostUserHeader(path);
        }
        this->unschedule(schedule_selector(UserInfoScene::modifyUserHeaderCallback));
    }
#endif
}

void UserInfoScene::doPostUserHeader(std::string pathKey)
{
    //CCHttpAgent::getInstance()->doPostImage(pathKey);
    
    this->schedule(schedule_selector(UserInfoScene::doPostUserHeaderCallback), 0.0f);
    //this->createLoading();
}

void UserInfoScene::doPostUserHeaderCallback(float dt)
{
    //if (CCHttpAgent::getInstance()->postImageStatus != 0)
    //{
    //    this->unschedule(schedule_selector(UserInfoScene::doPostUserHeaderCallback));
    //    this->stopLoading();
    //    
    //    string tip = "头像上传成功";
    //    if (CCHttpAgent::getInstance()->postImageStatus == 2)
    //    {
    //        tip = "头像上传失败";
    //    }
    //    //this->createToastLayer(tip.c_str(), 2.0f);
    //    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    showToast(tip.c_str());
    //    #else
    //    CCMessageBox(tip.c_str(),"提示");
    //    #endif
    //}
}

void UserInfoScene::showEditBox()
{
    playerNameTTF->setVisible(false);
    planCtrlBtn->setVisible(false);
    
    pEditBoxFrame->setVisible(true);

    //pEditBox = TextField::textFieldWithPlaceHolder("", "Arial", 18);
    //CC_SAFE_RETAIN(pEditBox);
    //pEditBox->setMaxLength(12);
    //pEditBox->setColor(ccWHITE);
    //pEditBox->setAnchorPoint(Point::ZERO);
    //pEditBox->setPosition(ccp(5,0));
    //pEditBox->setStringDisplaySize(CCSizeMake(182, 33));
    //pEditBox->setString(playerNameTTF->getString());
    //pEditBox->setReturnMode(kUIReturnKeySend);
    //pEditBox->setSelector(this, callfuncN_selector(UserInfoScene::hiddeEditBox));
    //pEditBox->openIME();
    //pRow1->addChild(pEditBox);
}


void UserInfoScene::hiddeEditBox(CCNode* pSender)
{
    //std::string name = pEditBox->getString();
    //if (name.empty())
    //{
    //    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    showToast("昵称不能为空");
    //    #else
    //    CCMessageBox("提示", "昵称不能为空");
    //    #endif
    //}
    //else if(name.length()>12)
    //{
    //    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    showToast("输入字数超出了最大的长度");
    //    #else
    //    CCMessageBox("提示", "输入字数超出了最大的长度");
    //    #endif
    //}
    //
    //if (!name.empty() && name.length()<=12)
    //{
    //    playerNameTTF->setString(name.c_str());
    //}
    //else
    //{
    //    playerNameTTF->setString(PlayerModel::getInstance()->name.c_str());
    //}
    //playerNameTTF->setVisible(true);
    //
    //planCtrlBtn->setPosition(ccp(playerNameTTF->getPositionX()+playerNameTTF->getContentSize().width+5,50/2-planCtrlBtn->getPreferredSize().height/2));
    //planCtrlBtn->setVisible(true);
    //
    //pEditBoxFrame->setVisible(false);
    //
    //pEditBox->removeFromParentAndCleanup(true);
    //CC_SAFE_RELEASE_NULL(pEditBox);
}









