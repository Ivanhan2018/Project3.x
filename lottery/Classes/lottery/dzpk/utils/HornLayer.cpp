//
//  HornLayer.cpp
//  TexasPoker
//
//  Created by peniel on 12/26/13.
//
//

#include "HornLayer.h"
#include "CtrlManager.h"
#include "PlayerModel.h"
#include "Sound.h"
#include "DzConfig.h"
//#include "ClipLayer.h"
//#include "PopupMsgListLayer.h"
//#include "TcpHornClient.h"

HornLayer::HornLayer():
pMsgTTF(NULL),
pNoticeNode(NULL),
isMsgListShow(false),
pWhoTTF(NULL),
//pMsgEditBox(NULL),
pNoticeBg(NULL),
isEditBoxShow(false),
maxInputSize(50)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	this->visibleW = visibleSize.width;
	this->visibleH = visibleSize.height;
}

HornLayer::~HornLayer()
{
	CC_SAFE_RELEASE_NULL(pMsgTTF);
	CC_SAFE_RELEASE_NULL(pNoticeNode);
	CC_SAFE_RELEASE_NULL(pWhoTTF);
	CC_SAFE_RELEASE_NULL(pNoticeBg);
}

HornLayer* HornLayer::createWith(CCPoint hronPoint)
{
    HornLayer* pRet = new HornLayer();
    if (pRet)
    {
        pRet->hronPoint = hronPoint;
    }
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool HornLayer::init()
{
	bool bRec = false;
    
	do
	{
		CC_BREAK_IF(!Layer::create());
		
		this->initUI();
        
		bRec = true;
        
	} while (0);
    
	return bRec;
}

void HornLayer::initUI()
{
    this->setContentSize(CCSizeMake(this->visibleW, this->visibleH));
    
    pNoticeNode = this->createMsgNode();
    CC_SAFE_RETAIN(pNoticeNode);
    CCSize noticeSize = pNoticeNode->getContentSize();
    pNoticeNode->setAnchorPoint(Vec2::ZERO);
    pNoticeNode->setPosition(hronPoint);
    this->addChild(pNoticeNode);
    
}

CCNode* HornLayer::createMsgNode()
{

	pNoticeBg = CCSprite::create("msg/msg_bg.png");
    CC_SAFE_RETAIN(pNoticeBg);
    CCSize bgSize = pNoticeBg->getContentSize();
    
    CCNode* pNoticeNode = CCNode::create();
    pNoticeNode->setContentSize(bgSize);
    
    pNoticeBg->setAnchorPoint(Vec2::ZERO);
    pNoticeBg->setPosition(Vec2::ZERO);
    pNoticeNode->addChild(pNoticeBg);
    
    ControlButton* hornCtrlBtn = CtrlManager::createCtrlBtnWithFile("msg/horn_icon.png","");
    hornCtrlBtn->setAnchorPoint(Vec2::ZERO);
    CCSize hornSize = hornCtrlBtn->getPreferredSize();
    hornCtrlBtn->setPosition(ccp(10,bgSize.height/2-hornSize.height/2));
    hornCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HornLayer::ctrlBtnCallbck),Control::EventType::TOUCH_UP_INSIDE);
    pNoticeNode->addChild(hornCtrlBtn,1,SEND_MSG_CONTRL_TAG);
    
    ControlButton* msgListCtrlBtn = CtrlManager::createCtrlBtnWithFile("popup/transparent_bg.png","");
    msgListCtrlBtn->setContentSize(CCSizeMake(bgSize.width-hornCtrlBtn->getPositionX()-hornSize.width, bgSize.height));
    msgListCtrlBtn->setScaleX(bgSize.width-hornCtrlBtn->getPositionX()-hornSize.width);
    msgListCtrlBtn->setScaleY(bgSize.height);
    msgListCtrlBtn->setAnchorPoint(Vec2::ZERO);
    msgListCtrlBtn->setPosition(ccp(hornCtrlBtn->getPositionX()+hornSize.width+5,0));
    msgListCtrlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HornLayer::ctrlBtnCallbck), Control::EventType::TOUCH_UP_INSIDE);
    pNoticeNode->addChild(msgListCtrlBtn,1,MSG_LIST_CONTRL_TAG);
    
    //ClipLayer* pClipLayer = ClipLayer::create(CCSizeMake(bgSize.width-(hornCtrlBtn->getPositionX()+hornSize.width+5)+40, bgSize.height-8));
    //pClipLayer->setAnchorPoint(Vec2::ZERO);
    //pClipLayer->setAnchorPoint(ccp(hornCtrlBtn->getPositionX()+hornSize.width+5,4));
    //pNoticeNode->addChild(pClipLayer);
    
    HronMsgModel* currHronMsg = (HronMsgModel*)PlayerModel::getInstance()->hronMsgList->objectAtIndex(PlayerModel::getInstance()->hronMsgList->count()-1);
    pWhoTTF = CCLabelTTF::create(currHronMsg->who.c_str(), "Arial", 16);
    CC_SAFE_RETAIN(pWhoTTF);
    if (currHronMsg->sex==0)
    {
        pWhoTTF->setColor(currHronMsg->sysColor);
    }
    else if (currHronMsg->sex==1)
    {
        pWhoTTF->setColor(currHronMsg->manColor);
    }
    else if (currHronMsg->sex==2)
    {
        pWhoTTF->setColor(currHronMsg->wanColor);
    }
    else
    {
        pWhoTTF->setColor(currHronMsg->sysColor);
    }
    pWhoTTF->setAnchorPoint(Vec2::ZERO);
    whoPoint = ccp(hornCtrlBtn->getPositionX()+hornSize.width+2,bgSize.height/2-pWhoTTF->getContentSize().height/2-1);
    pWhoTTF->setPosition(whoPoint);
    //pClipLayer->addChild(pWhoTTF);
    
    pMsgTTF = CCLabelTTF::create(currHronMsg->msg.c_str(), "Arial", 16);
    CC_SAFE_RETAIN(pMsgTTF);
    pMsgTTF->setAnchorPoint(Vec2::ZERO);
    msgPoint = ccp(pWhoTTF->getPositionX()+pWhoTTF->getContentSize().width+5,pWhoTTF->getPositionY());
    pMsgTTF->setPosition(msgPoint);
    //pClipLayer->addChild(pMsgTTF);
    
    msgEditBoxPoint = ccp(pWhoTTF->getPositionX(),pWhoTTF->getPositionY()+2);
    
    return pNoticeNode;
}

void HornLayer::ctrlBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event)
{
    Sound::getInstance()->playEffect("sound/sound_dianji.ogg");
    
    ControlButton* ctrlBtn = (ControlButton*)pSender;
	UNS_INT ctrlBtnTag = ctrlBtn->getTag();
	//DLog::showLog("ctrlBtn tag: %d",ctrlBtnTag);
    
	if(ctrlBtnTag == MSG_LIST_CONTRL_TAG && !isEditBoxShow)
	{
        isMsgListShow = true;
        
        pNoticeBg->initWithFile("msg/msg_bg_1.png");
        pNoticeBg->setAnchorPoint(Vec2::ZERO);
        pNoticeBg->setPosition(Vec2::ZERO);
        
		//PopupMsgListLayer* popupMsgListLayer = PopupMsgListLayer::createWith(ccp(pNoticeNode->getPositionX()+38,pNoticeNode->getPositionY()));
  //      popupMsgListLayer->setAnchorPoint(Vec2::ZERO);
  //      popupMsgListLayer->setPosition(Vec2::ZERO);
  //      this->addChild(popupMsgListLayer,100);
	}
    else if(ctrlBtnTag == SEND_MSG_CONTRL_TAG)
	{
        if (!isEditBoxShow)
        {
            isEditBoxShow = true;
            
            pWhoTTF->setVisible(false);
            pMsgTTF->setVisible(false);
            
            //pMsgEditBox = TextField::textFieldWithPlaceHolder(CCString::createWithFormat("在此输入...(限16字,一条信息消耗%d筹码)",DzConfig::getInstance()->pBigHornConfig->money)->getCString(), "Arial", 16);
            //CC_SAFE_RETAIN(pMsgEditBox);
            //pMsgEditBox->setMaxLength(maxInputSize);
            //pMsgEditBox->setColor(ccWHITE);
            //pMsgEditBox->setAnchorPoint(Vec2::ZERO);
            //pMsgEditBox->setPosition(msgEditBoxPoint);
            //pMsgEditBox->setStringDisplaySize(CCSizeMake(350, 20));
            //pMsgEditBox->setString("");
            //pMsgEditBox->setReturnMode(kUIReturnKeySend);
            //pMsgEditBox->setSelector(this, callfuncN_selector(HornLayer::showEditBoxCallback));
            //pNoticeNode->addChild(pMsgEditBox);
            //this->schedule(schedule_selector(HornLayer::checkInputSizeCallback), 0.1f);
        }
        else
        {
            this->setMsgTTFVisibile(true);

            //pMsgEditBox->removeFromParentAndCleanup(true);
            //CC_SAFE_RELEASE_NULL(pMsgEditBox);
            //isEditBoxShow = false;
        } 
    }
}

void HornLayer::checkInputSizeCallback(float dt)
{
    //if (pMsgEditBox == NULL)
    //{
    //    return;
    //}
    //std::string sInsert = pMsgEditBox->getString();
    //if (sInsert.size()>maxInputSize)
    //{
    //    pMsgEditBox->editReturn();
    //    this->unschedule(schedule_selector(HornLayer::checkInputSizeCallback));
    //}
}

void HornLayer::showEditBoxCallback(CCNode* pSender)
{
    //this->setMsgTTFVisibile(true);
    //std::string msg = pMsgEditBox->getString();
    //this->sendMsg(msg);
    //
    //pMsgEditBox->removeFromParentAndCleanup(true);
    //CC_SAFE_RELEASE_NULL(pMsgEditBox);
    //isEditBoxShow = false;
}

bool HornLayer::getIsMsgListShow()
{
    return isMsgListShow;
}

void HornLayer::setMsgTTFVisibile(bool bVisible)
{
    pWhoTTF->setVisible(bVisible);
    pMsgTTF->setVisible(bVisible);
}

void HornLayer::setIsMsgListShow(bool showIs)
{
    pNoticeBg->initWithFile("msg/msg_bg.png");
    pNoticeBg->setAnchorPoint(Vec2::ZERO);
    pNoticeBg->setPosition(Vec2::ZERO);
    
    isMsgListShow = showIs;
}

void HornLayer::onEnter()
{
	Layer::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(HornLayer::notifiScrollingCallback),NOTIFI_SCROLLING_HORN_MSG_KEY,NULL);
}

void HornLayer::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTIFI_SCROLLING_HORN_MSG_KEY);
	Layer::onExit();
}

void HornLayer::notifiScrollingCallback(CCObject* pCCObjData)
{
    HronMsgModel* currHronMsg = (HronMsgModel*)pCCObjData;

    this->msgScrollAnim(currHronMsg);
}

void HornLayer::msgScrollAnim(HronMsgModel* msg)
{
    currRecvMsg = *msg;
    
    if (msg->sex==0)
    {
        pWhoTTF->setColor(msg->sysColor);
    }
    else if (msg->sex==1)
    {
        pWhoTTF->setColor(msg->manColor);
    }
    else if (msg->sex==2)
    {
        pWhoTTF->setColor(msg->wanColor);
    }
    pWhoTTF->runAction(CCSequence::create(
          CCMoveTo::create(0.4f, CCPointMake(whoPoint.x, whoPoint.y+30)),
          CCCallFuncN::create(this, callfuncN_selector(HornLayer::whoScrollAnimCallback)),
          CCMoveTo::create(0.4f, whoPoint),NULL));
    
    msgPoint = CCPointMake(whoPoint.x+CCLabelTTF::create(msg->who.c_str(),"Arial",16)->getContentSize().width+5,msgPoint.y);
    pMsgTTF->runAction(CCSequence::create(
          CCMoveTo::create(0.4f, CCPointMake(msgPoint.x, msgPoint.y+30)),
          CCCallFuncN::create(this, callfuncN_selector(HornLayer::msgScrollAnimCallback)),
          CCMoveTo::create(0.4f, msgPoint),NULL));
}

void HornLayer::whoScrollAnimCallback(CCNode* pSender)
{
    pWhoTTF->setString(currRecvMsg.who.c_str());
    pSender->setPosition(ccp(pSender->getPositionX(),-30));
}

void HornLayer::msgScrollAnimCallback(CCNode* pSender)
{
    pMsgTTF->setString(currRecvMsg.msg.c_str());
    pSender->setPosition(ccp(pSender->getPositionX(),-30));
}

void HornLayer::sendMsg(std::string msg)
{
    if (""==msg)
    {
        //this->createToastLayer("发送信息不能为空.", 2.0f);
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showToast("发送信息不能为空!");
        #else
        //CCMessageBox("发送信息不能为空!","提示");
        #endif
        return;
    }
    if (msg.size()>maxInputSize)
    {
        this->unschedule(schedule_selector(HornLayer::checkInputSizeCallback));
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showToast("输入字数超出了最大的长度");
        #else
        //CCMessageBox("提示", "输入字数超出了最大的长度");
        #endif
        return;
    }
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        int ret = judgeInputIsLegal(msg.c_str());
        if (ret != 0)
        {
            showToast("你发送的信息中含有特殊字符，不能发送!");
            return;
        }
    #endif
    
    if (PlayerModel::getInstance()->money<DzConfig::getInstance()->pBigHornConfig->money)
    {
        //this->createToastLayer("筹码不足.", 2.0f);
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showToast("筹码不足!");
        #else
        //CCMessageBox("筹码不足!","提示");
        #endif
        return;
    }
    //JPacket jpacket;
    //jpacket.val["cmd"] = HRON_USER_SAY_CMD;
    //jpacket.val["uid"] = PlayerModel::getInstance()->uid;
    //jpacket.val["vid"] = 101;
    //jpacket.val["name"] = PlayerModel::getInstance()->name;
    //jpacket.val["skey"] = PlayerModel::getInstance()->skey;
    //jpacket.val["str"] = msg;
    //jpacket.end();
    //TcpHornClient::getInstance()->send_data(jpacket.tostring());
}

void HornLayer::createToastLayer(const char* msg,float delay)
{
    CCLabelTTF* tip = CCLabelTTF::create(msg, "Arial", 15);
    CCSize tipSize = tip->getContentSize();
    
    CCSize toastSize = CCSizeMake(tipSize.width+20, 50);
    CCLayerColor* pToastLayer = CCLayerColor::create(ccc4(0, 0, 0, 255), toastSize.width, toastSize.height);
    pToastLayer->setAnchorPoint(Vec2::ZERO);
    pToastLayer->setPosition(ccp(this->visibleW/2-toastSize.width/2,+this->visibleH/2-toastSize.height/2));
    
    tip->setAnchorPoint(Vec2::ZERO);
    tip->setPosition(ccp(toastSize.width/2-tipSize.width/2,toastSize.height/2-tipSize.height/2));
    pToastLayer->addChild(tip);
    
    this->addChild(pToastLayer,50);
    
    pToastLayer->runAction(CCSequence::create(CCFadeIn::create(0.5f),CCDelayTime::create(delay-1),CCFadeOut::create(0.5f),CCCallFuncN::create(this, callfuncN_selector(HornLayer::showToastCallback)),NULL));
}

void HornLayer::showToastCallback(CCNode* pSender)
{
    pSender->removeFromParentAndCleanup(true);
}












