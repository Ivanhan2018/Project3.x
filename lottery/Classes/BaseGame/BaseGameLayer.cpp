#include "BaseGameLayer.h"
#include "PromptBox.h"
#include "MyNSString.h"
#include "GlobalDef.h"

BaseGameLayer::BaseGameLayer()
{
}

BaseGameLayer::~BaseGameLayer()
{
	removeNotification();
}

bool BaseGameLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		addNotification();

		bRet = true;
	} while (0);

	return bRet;
}

void BaseGameLayer::dealMessageScene(Ref* sender)
{
	tagSceneBuffer* tag = (tagSceneBuffer*)sender;
	onMessageScene(tag->cbState, tag->dateBuffer, tag->nDataSize);
}

//	ÍøÂç¶Ï¿ª
void BaseGameLayer::dealShutNetWork(Ref* sender)
{
	Scene* pScene = Director::getInstance()->getRunningScene();
	Size sVSize = Director::getInstance()->getVisibleSize();
	PromptBox * pPromptBox = PromptBox::PromptBoxWith(Vec2(sVSize.width * 0.5, sVSize.height * 0.5), mPromptTypeGameServerShut);
	pScene->addChild(pPromptBox);
	pPromptBox->setName("BaseGameLayer::dealLoginRoomFailed");
}

//	µÇÂ¼·¿¼äÊ§°Ü
void BaseGameLayer::dealLoginRoomFailed(Ref* sender)
{
	Scene* pScene = Director::getInstance()->getRunningScene();
	Size sVSize = Director::getInstance()->getVisibleSize();
	if (pScene->getChildByName("BaseGameLayer::dealLoginRoomFailed"))
	{
		return;
	}

	tagRoomLogonFailObj* pObj = (tagRoomLogonFailObj*)sender;
	int DataSize = 128;
	char str[1000] = {0};
	MyNSString::wc2mbs(pObj->szDescribeString, DataSize, str);

	PromptBox* pPromptBox = PromptBox::PromptBoxWith(Vec2(sVSize.width * 0.5, sVSize.height * 0.5), mPromptTypeGetError);
	pScene->addChild(pPromptBox);
	pPromptBox->setPromptText(str);
	pPromptBox->setName("BaseGameLayer::dealLoginRoomFailed");
}

//	×øÏÂÊ§°Ü
void BaseGameLayer::dealSitDownFaild(Ref* sender)
{
	Scene* pScene = Director::getInstance()->getRunningScene();
	Size sVSize = Director::getInstance()->getVisibleSize();
	if (getChildByName("BaseGameLayer::dealSitDownFaild"))
	{
		return;
	}
	tagRequestFailureObj* pObj = (tagRequestFailureObj*)sender;
	int DataSize=256;
	char str[1000]={0};
	MyNSString::wc2mbs(pObj->szDescribeString,DataSize, str);

	PromptBox* pPromptBox = PromptBox::PromptBoxWith(Vec2(sVSize.width * 0.5, sVSize.height * 0.5), mPromptTypeGetError);
	this->addChild(pPromptBox);
	pPromptBox->setPromptText(str);
	pPromptBox->setName("BaseGameLayer::dealSitDownFaild");
}

void BaseGameLayer::addNotification()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseGameLayer::dealEnterGame), NOTIFICATION_GAME_ENTERGAME, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseGameLayer::dealShutNetWork), MSG_UI_ANS_NETWORKSHUT, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseGameLayer::dealPlayerInfo), MSG_UI_ANS_PLAYERUPDATE, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseGameLayer::dealMessageGame), GBEVENT_UI_GAMEMESSAGE, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseGameLayer::dealMessageScene), GBEVENT_UI_SCENEMESSAGE, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseGameLayer::dealLoginRoomFailed), MSG_UI_ANS_ROOMLOGINFAIL, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseGameLayer::dealSitDownFaild), MSG_UI_ANS_SITFAIL, NULL);
}

void BaseGameLayer::removeNotification()
{
	NotificationCenter::getInstance()->removeObserver(this, NOTIFICATION_GAME_ENTERGAME);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_NETWORKSHUT);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_PLAYERUPDATE);
	NotificationCenter::getInstance()->removeObserver(this,GBEVENT_UI_GAMEMESSAGE);
	NotificationCenter::getInstance()->removeObserver(this,GBEVENT_UI_SCENEMESSAGE);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_ROOMLOGINFAIL);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_SITFAIL);
}

