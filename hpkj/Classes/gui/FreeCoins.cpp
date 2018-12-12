//
//  FreeCoins.cpp
//  Game
//
// Created by wangjun on 15-1-7.
//
//

#include "FreeCoins.h"
#include "CocosUnits.h"
#include "cocos-ext.h"
#include "AnsString.h"
#include "utf-8.h"
#include "ConfigMgr.h"
#include "EntityMgr.h"
#include "RoomLayer.h"
#include "LoginLayer.h"
#include "MovingLabelLayer.h"
#include "FreeLayer.h"
#include "OverRegistLayer.h"
#include "JsonHelper.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

#include "network/HttpClient.h"

USING_NS_CC;

FreeCoins::FreeCoins()
{
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(FreeCoins::onCheckVersion), MSG_UI_ANS_VERSIONNEW, NULL);
	cancelOrNot = false;
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(FreeCoins::cancelHttpRequest),"CancelHttpRequest", NULL);
	
}

FreeCoins::~FreeCoins()
{
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_VERSIONNEW);
	NotificationCenter::getInstance()->removeObserver(this, "CancelHttpRequest");
}

void FreeCoins::cancelHttpRequest(Object *obj)
{
	cancelOrNot = true;
}

bool FreeCoins::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	//����
	Sprite *background = Sprite::create("bg2.png");
	background->setPosition(Vec2(winSize.width/2, winSize.height/2));
	this->addChild(background);	

	//����
	Sprite* backNormal= Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	Sprite* backSelect= Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	MenuItemSprite* backItemSprite = MenuItemSprite::create(backNormal,backSelect,CC_CALLBACK_1(FreeCoins::back,this));
	backSelect->setScale(0.9f);
	backSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	Menu* backMenu= Menu::create(backItemSprite,NULL);
	backMenu->setPosition(Vec2(winSize.width * 0.93,winSize.height * 0.93));
	this->addChild(backMenu);

	//�ȼý��Ƽ�׬��ң��ҵ��Ƽ���
	float itemGap = 10;
	currentItem = FreeCoinsTabAlms;
	Sprite *almsItemImage1 = Sprite::createWithSpriteFrame(spriteFrame("Tab_Alms2.png"));
	Sprite *almsItemImage2 = Sprite::createWithSpriteFrame(spriteFrame("Tab_Alms2.png"));
	Sprite *recommendItemImage1 = Sprite::createWithSpriteFrame(spriteFrame("Tab_Recommend1.png"));
	Sprite *recommendItemImage2 = Sprite::createWithSpriteFrame(spriteFrame("Tab_Recommend1.png"));
	Sprite *recommenderItemImage1 = Sprite::createWithSpriteFrame(spriteFrame("Tab_MyRecommender1.png"));
	Sprite *recommenderItemImage2 = Sprite::createWithSpriteFrame(spriteFrame("Tab_MyRecommender1.png"));
	almsItem = MenuItemSprite::create(almsItemImage1,almsItemImage2,CC_CALLBACK_1(FreeCoins::getAlms,this));
	recommendItem = MenuItemSprite::create(recommendItemImage1,recommendItemImage2,CC_CALLBACK_1(FreeCoins::recommend,this));
	recommenderItem = MenuItemSprite::create(recommenderItemImage1,recommenderItemImage2,CC_CALLBACK_1(FreeCoins::myRecommender,this));
	Menu* menu = Menu::create(almsItem,recommendItem,recommenderItem,NULL);
	menu->setAnchorPoint(Vec2(0.5,0.5));
	float menuWidth = 4*itemGap+almsItemImage1->getContentSize().width*3;
	menu->setPosition(Vec2(menuWidth/2,winSize.height*0.9f-2));
	menu->alignItemsHorizontallyWithPadding(itemGap);//�������з�ʽ��������20
	this->addChild(menu);

	Sprite *girl = Sprite::createWithSpriteFrame(spriteFrame("Sprite_FreeCoinsGirl.png"));
	girl->setAnchorPoint(Vec2(0,0.5f));
	girl->setPosition(Vec2(15, winSize.height/2-20));
	this->addChild(girl);	

	float textWidth = 500;
	almsText = LabelTTF::create(pConfigMgr->text("t552"),"",18,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	almsText->setColor(ccc3(189,154,97));
	almsText->setAnchorPoint(Vec2(0,1));
	almsText->setPosition(Vec2(266,363));
	this->addChild(almsText);

	recommendText = LabelTTF::create(pConfigMgr->text("t553"),"",18,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	recommendText->setColor(ccc3(189,154,97));
	recommendText->setAnchorPoint(Vec2(0,1));
	recommendText->setPosition(Vec2(266,363));
	this->addChild(recommendText);

	recommenderText = LabelTTF::create(pConfigMgr->text("t554"),"",18,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	recommenderText->setColor(ccc3(189,154,97));
	recommenderText->setAnchorPoint(Vec2(0,1));
	recommenderText->setPosition(Vec2(266,247));
	this->addChild(recommenderText);
	almsText->setVisible(true);
	recommendText->setVisible(false);
	recommenderText->setVisible(false);

	Sprite * getCoinsNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_FreeCoinGet1.png"));
	Sprite * getCoinsSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_FreeCoinGet2.png"));
	Sprite * getCoinsDisSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_FreeCoinGet2.png"));
	getCoinsDisSprite->setColor(ccc3(100,100,100));
	MenuItemSprite *getCoinsMenuItem = MenuItemSprite::create(getCoinsNormalSprite, 
		getCoinsSelectSprite,getCoinsDisSprite, CC_CALLBACK_1(FreeCoins::pressGetCoins,this));
	getCoinsMenu = Menu::create(getCoinsMenuItem,NULL);
	getCoinsMenu->setAnchorPoint(Vec2(0,0));
	getCoinsMenu->setPosition(Vec2(446, 48));
	this->addChild(getCoinsMenu);

	//�Ƽ���
	recommenderLabel = LabelTTF::create(pConfigMgr->text("t555"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	recommenderLabel->setColor(ccc3(255,255,255));
	recommenderLabel->setAnchorPoint(Vec2(0,0.5f));
	recommenderLabel->setPosition(Vec2(261,299));
	this->addChild(recommenderLabel);
	//�Ƽ��������
	recommenderName = EditBox::create(cocos2d::Size(389, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_Notice2.png")));
	recommenderName->setPosition(Vec2(363, 301));
	recommenderName->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	recommenderName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	recommenderName->setAnchorPoint(Vec2(0,0.5f));
	recommenderName->setFontColor(ccc3(0,0,0));
	recommenderName->setFontSize(30);
	recommenderName->setPlaceHolder(pConfigMgr->text(""));
	this->addChild(recommenderName);
	recommenderLabel->setVisible(false);
	recommenderName->setVisible(false);
	this->setKeypadEnabled(true);
	//setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	promptBox = NULL;
	return true;
}

void FreeCoins::onEnter()
{
	Layer::onEnter();
	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-1000,true);
}

void FreeCoins::onExit()
{
	//
	Layer::onExit();
}

bool FreeCoins::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	//if(m_pMusicControlSprite->boundingBox().containsPoint(touchPoint))
	//{
	//	m_nChangeVoice = 1;
	//	return true;
	//}
	return false;
}

void FreeCoins::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
}

void FreeCoins::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
}

void FreeCoins::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
}

void FreeCoins::back(Object* obj)
{
	playButtonSound();
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);
}

void FreeCoins::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
	}
}

void FreeCoins::onCheckVersion(Object* obj)
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptVersionNew);
	this->addChild(box,50);
}

void FreeCoins::getAlms( Object* obj)
{
	if(currentItem == FreeCoinsTabAlms)
	{
		return;
	}
	currentItem = FreeCoinsTabAlms;
	getCoinsMenu->setVisible(true);
	almsItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Alms2.png")));
	almsItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Alms2.png")));
	recommendItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Recommend1.png")));
	recommendItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Recommend1.png")));
	recommenderItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_MyRecommender1.png")));
	recommenderItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_MyRecommender1.png")));
	almsText->setVisible(true);
	recommendText->setVisible(false);
	recommenderText->setVisible(false);
	recommenderLabel->setVisible(false);
	recommenderName->setVisible(false);
}

void FreeCoins::recommend( Object* obj)
{
	if(currentItem == FreeCoinsTabRecommend)
	{
		return;
	}
	currentItem = FreeCoinsTabRecommend;
	getCoinsMenu->setVisible(false);
	almsItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Alms1.png")));
	almsItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Alms1.png")));
	recommendItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Recommend2.png")));
	recommendItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Recommend2.png")));
	recommenderItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_MyRecommender1.png")));
	recommenderItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_MyRecommender1.png")));
	almsText->setVisible(false);
	recommendText->setVisible(true);
	recommenderText->setVisible(false);
	recommenderLabel->setVisible(false);
	recommenderName->setVisible(false);
}

void FreeCoins::myRecommender( Object* obj)
{
	if(currentItem == FreeCoinsTabRecommender)
	{
		return;
	}
	currentItem = FreeCoinsTabRecommender;
	getCoinsMenu->setVisible(true);
	almsItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Alms1.png")));
	almsItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Alms1.png")));
	recommendItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Recommend1.png")));
	recommendItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_Recommend1.png")));
	recommenderItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_MyRecommender2.png")));
	recommenderItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Tab_MyRecommender2.png")));
	almsText->setVisible(false);
	recommendText->setVisible(false);
	recommenderText->setVisible(true);
	recommenderLabel->setVisible(true);
	recommenderName->setVisible(true);
}

void FreeCoins::pressGetCoins( Object* obj)
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	char str[33];
	std::sprintf(str, "%lu", pMsgDispatch->m_dwUserID);
	string userId = string(str);
	if(currentItem == FreeCoinsTabAlms)
	{
		promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
		promptBox->setPromptText(pConfigMgr->text("t556"));
		this->addChild(promptBox,1);
		string url = "http://117.18.5.229:999/mobile/GetWealthJob.aspx?uid=";
		url = url+userId;
		network::HttpRequest* request = new network::HttpRequest();//����request����,����new�����Ķ�����ʹ��autorelease()��ԭ�����  
		request->setUrl(url.c_str());//����url  
		request->setRequestType(network::HttpRequest::Type::GET);//��������ʽ  
		request->setResponseCallback(this, callfuncND_selector(FreeCoins::onHttpRequestCompleted));//���ǻص�����ͻص�����  
		request->setTag("GET test1");//�����û���ʶ������ͨ��response��ȡ  
		network::HttpClient::getInstance()->send(request);//ʹ��network::HttpClient����ʵ��������request  
		request->release();//����release()  
	}else if(currentItem == FreeCoinsTabRecommender)
	{
        string tempStr = recommenderName->getText();
        if (tempStr!="") {
		promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
		promptBox->setPromptText(pConfigMgr->text("t556"));
		this->addChild(promptBox,2);
		//string url = "http://183.60.202.73:8011/mobile/GetWealthJob.aspx?uid=";
		string url = "http://117.18.5.229:999/mobile/GetWealthJob.aspx?uid=";
		url = url+userId;
		network::HttpRequest* request = new network::HttpRequest();//����request����,����new�����Ķ�����ʹ��autorelease()��ԭ�����  
		request->setUrl(url.c_str());//����url  
		request->setRequestType(network::HttpRequest::Type::GET);//��������ʽ  
		request->setResponseCallback(this, callfuncND_selector(FreeCoins::onHttpRequestCompleted));//���ǻص�����ͻص�����  
		request->setTag("GET test1");//�����û���ʶ������ͨ��response��ȡ  
		network::HttpClient::getInstance()->send(request);//ʹ��network::HttpClient����ʵ��������request  
		request->release();//����release()
        }else{
            promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
            promptBox->setPromptText(pConfigMgr->text("t1006"));
           this->addChild(promptBox,2);
        }
	}
}

void FreeCoins::onHttpRequestCompleted(Node *sender, void *data)  
{  
	if(cancelOrNot)
	{
		return;
	}
	network::HttpResponse *response = (network::HttpResponse*)data;  

	if (!response)  
	{  
		return;  
	}  

	// ��ȡ��Ӧrequest���ַ�����ʶ  
	if (0 != strlen(response->getHttpRequest()->getTag()))   
	{  
		CCLOG("%s completed", response->getHttpRequest()->getTag());  
	}  
	//��ȡ���ش��룬����200��404��  
	int statusCode = response->getResponseCode();  
	char statusString[64] = {};  
	std::sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());  
	//m_labelStatusCode->setString(statusString);  
	CCLOG("response code: %d", statusCode);  

	if (!response->isSucceed())   
	{  
		CCLOG("response failed");  
		CCLOG("error buffer: %s", response->getErrorBuffer());//���Ե���getErrorBuffer()����ȡ����ԭ��  
		return;  
	}  

	// dump data  
	std::vector<char> *buffer = response->getResponseData();//������ȡ���յ�������  
	char dataBuffer[255]; 
	int dataSize = buffer->size();

	if(dataSize<=0)
	{
	return;
	}

	for (unsigned int i = 0; i < dataSize; i++)  
	{  
		dataBuffer[i] = (*buffer)[i];  
	}   
	Json::Value value = JsonHelper::getJsonFromData(dataBuffer, dataSize);
	
	for(Json::ValueIterator it = value.begin() ; it!=value.end() ; it++)
	{
		Json::Value value1 = *it;
	}


	int index = 0;
	Json::Value noticeValue = value[index];
	string noticeStr = noticeValue["mess"].asString();
	if(promptBox == NULL){
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
		promptBox->setPromptText(noticeStr.c_str());
		this->addChild(promptBox,1);
	}else{
		promptBox->setPromptText(noticeStr.c_str());
	}
} 