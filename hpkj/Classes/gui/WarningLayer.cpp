#include "WarningLayer.h"
#include "RoomLayer.h"
#include "GroupSprite.h"
#include "TaskInfo.h"
#include "PromptBox.h"
#include "MovingLabelLayer.h"
#include "SceneControl.h"
#include "CocosUnits.h"
#include "JsonHelper.h"
#include "DBHandler.h"
#include <string>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

#include "network/HttpClient.h"


#define minInWarning(a,b) (((a) < (b)) ? (a) : (b))

using namespace cocos2d;

Vec2 signInImagePos = Vec2(400,381);
Vec2 signInButtonPos = Vec2(625, 135);
Vec2 awardImagePos[7] = {Vec2(165,282), Vec2(327,282),Vec2(475,282),
	Vec2(632,282),Vec2(165,147),Vec2(327,147),Vec2(475,147)};
char awardImageName1[7][30]={"Sprite_SignIn1Day1.png", "Sprite_SignIn2Day1.png", "Sprite_SignIn3Day1.png","Sprite_SignIn4Day1.png",
	"Sprite_SignIn5Day1.png","Sprite_SignIn6Day1.png","Sprite_SignIn7Day1.png"};
char awardImageName2[7][30]={"Sprite_SignIn1Day2.png", "Sprite_SignIn2Day2.png", "Sprite_SignIn3Day2.png","Sprite_SignIn4Day2.png",
	"Sprite_SignIn5Day2.png","Sprite_SignIn6Day2.png","Sprite_SignIn7Day2.png"};

WarningLayer::WarningLayer()
{
	//NotificationCenter::getInstance()->addObserver(
	//	this, callfuncO_selector(WarningLayer::onGetLoginAward), MSG_UI_ANS_GETAWARDRESULT, NULL);
	//NotificationCenter::getInstance()->addObserver(
	//	this, callfuncO_selector(WarningLayer::onGetTaskAward), MSG_UI_ANS_TASKGETSUCC, NULL);
	//NotificationCenter::getInstance()->addObserver(
	//	this, callfuncO_selector(WarningLayer::onGetTaskFailed), MSG_UI_ANS_TASKGETFAIL, NULL);
	signInDays = 0;
	cancelOrNot = false;
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(WarningLayer::cancelHttpRequest),"CancelHttpRequest", NULL);

}
WarningLayer::~WarningLayer()
{
	if(awardImage != NULL)
	{
		CC_SAFE_DELETE(awardImage);
		awardImage = NULL;
	}

	NotificationCenter::getInstance()->removeObserver(this, "CancelHttpRequest");
	//NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_GETAWARDRESULT);
	//NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_TASKGETSUCC);
	//NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_TASKGETFAIL);
}
Scene* WarningLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		WarningLayer *layer = WarningLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool WarningLayer::init()
{
	bool bRet = false;
	do 
	{
		winSize = Director::getInstance()->getWinSize();
		signInDays = 0;
		/**���ư�͸���ɰ�**/
		LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), 854.0f, SCREEN_WIDTH);
		layer->ignoreAnchorPointForPosition(false);
		layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(layer);

		/**���Ʊ���ͼƬ**/
		//Sprite * pBJ = Sprite::createWithSpriteFrame(spriteFrame("dt_warning_1000.png"));
		Sprite * pBJ = Sprite::create("Bk_Notice.png");
		pBJ->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(pBJ,0);

		Sprite *signInImage = Sprite::createWithSpriteFrame(spriteFrame("Sprite_SignIn1.png"));
		signInImage->setPosition(signInImagePos);
		this->addChild(signInImage);

		Sprite *girl = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Notice3.png"));
		girl->setPosition(Vec2(111, 399));
		this->addChild(girl);
		//ǩ������ͼƬ
		awardImage = Array::create();
		awardImage->retain();
		for(int i = 0 ; i < 7 ; i++)
		{
			Sprite *sprite = Sprite::createWithSpriteFrame(spriteFrame(awardImageName1[i]));
			if(sprite)
			{
				sprite->setPosition(awardImagePos[i]);
				awardImage->addObject(sprite);
				this->addChild(sprite);
			}
		}

		/**ǩ����ť**/
		Sprite *normalSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_SignIn1.png"));
		Sprite *selectSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_SignIn2.png"));
		MenuItemSprite *menuSprite = MenuItemSprite::create
			(normalSprite,selectSprite,CC_CALLBACK_1(WarningLayer::onGetTaskAward,this));

		signInMenu = Menu::create(menuSprite,NULL);
		signInMenu->setPosition(signInButtonPos);  
		this->addChild(signInMenu);
		/**�رհ�ť**/
		Sprite *pbuttonNormalClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		Sprite *pbuttonSelectClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		MenuItemSprite *pButtonClose = MenuItemSprite::create
			(pbuttonNormalClose,pbuttonSelectClose,CC_CALLBACK_1(WarningLayer::dismiss,this));
		pbuttonSelectClose->setScale(0.9f);
		pbuttonSelectClose->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pButton0 = Menu::create(pButtonClose,NULL);
		pButton0->setPosition(Vec2(pBJ->getPositionX()+pBJ->getContentSize().width/2-pbuttonNormalClose->getContentSize().width/2+10,
			pBJ->getPositionY()+pBJ->getContentSize().height/2-pbuttonNormalClose->getContentSize().height/2+10));  
		this->addChild(pButton0,2);
		signInMenu->setVisible(false);
		promptBox = NULL;
		this->requestSignInDays();

		bRet = true;
		this->setKeypadEnabled(true);
	} while (0);

	return bRet;

}

void WarningLayer::setSignInDays(int days)
{
	for(int i = 0 ; i < days ; i++)
	{
		Sprite *sprite = (Sprite *)awardImage->objectAtIndex(i);
		sprite->setSpriteFrame(spriteFrame(awardImageName2[i]));
	}
}

void WarningLayer::requestSignInDays()
{
	char str[33];
	std::sprintf(str, "%lu", pMsgDispatch->m_dwUserID);
	string userId;
	userId.assign(str, sizeof(str));
	string url = "http://117.18.5.229:999/phone/Registration.aspx?parama=0&paramb=";
	url = url+userId;
	network::HttpRequest* request = new network::HttpRequest();//����request����,����new�����Ķ�����ʹ��autorelease()��ԭ�����  
	request->setUrl(url.c_str());//����url  
	request->setRequestType(network::HttpRequest::Type::GET);//��������ʽ  
	request->setResponseCallback(this, callfuncND_selector(WarningLayer::onHttpRequestCompleted));//���ǻص�����ͻص�����  
	request->setTag("GET test1");//�����û���ʶ������ͨ��response��ȡ  
	network::HttpClient::getInstance()->send(request);//ʹ��network::HttpClient����ʵ��������request  
	request->release();//����release()  
}

void WarningLayer::signIn()
{
	char str[33];
	std::sprintf(str, "%lu", pMsgDispatch->m_dwUserID);
	string userId;
	userId.assign(str, sizeof(str));
	promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
	promptBox->setPromptText(pConfigMgr->text("t556"));
	this->addChild(promptBox,1);
	string url = "http://117.18.5.229:999/phone/Registration.aspx?parama=1&paramb=";
	url = url+userId;
	network::HttpRequest* request = new network::HttpRequest();//����request����,����new�����Ķ�����ʹ��autorelease()��ԭ�����  
	request->setUrl(url.c_str());//����url  
	request->setRequestType(network::HttpRequest::Type::GET);//��������ʽ  
	request->setResponseCallback(this, callfuncND_selector(WarningLayer::onSignInCompleted));//���ǻص�����ͻص�����  
	request->setTag("GET test1");//�����û���ʶ������ͨ��response��ȡ  
	network::HttpClient::getInstance()->send(request);//ʹ��network::HttpClient����ʵ��������request  
	
	request->release();//����release()  
}

void WarningLayer::cancelHttpRequest(Object *obj)
{
	cancelOrNot = true;
}

void WarningLayer::onHttpRequestCompleted(Node *sender, void *data)
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
	char dataBuffer[4048]; 
	memset(dataBuffer, 0, sizeof(dataBuffer));
	int dataSize = buffer->size();
	if(dataSize<=0)
	{
		return;
	}
	for (unsigned int i = 0; i < dataSize; i++)  
	{  
		dataBuffer[i] = (*buffer)[i];  
	}   
	CCLOG("%s", dataBuffer);
	char *slpitPointer;
	slpitPointer = strtok(dataBuffer, "|");
	char result = (char)*slpitPointer;
	int days = result - '0';
	if(days>7)
	{
		days = 0;
	}
	this->setSignInDays(days);
	if(slpitPointer!=NULL)
	{
		slpitPointer = strtok(NULL, "|");
	}
	result = 0;
	if(slpitPointer!=NULL){
		result = (char)*slpitPointer;
	}
	if(result == '1'){
		signInMenu->setVisible(true);
	}
}

void WarningLayer::onSignInCompleted(Node *sender, void *data)
{
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
	memset(dataBuffer, 0, sizeof(dataBuffer));
	int dataSize = buffer->size();
	for (unsigned int i = 0; i < dataSize; i++)  
	{  
		dataBuffer[i] = (*buffer)[i];  
	}   
	char *slpitPointer;
	slpitPointer = strtok(dataBuffer, "|");
	char result = (char)*slpitPointer;
	int days = result - '0';
	this->setSignInDays(days);
	if(slpitPointer!=NULL)
	{
		slpitPointer = strtok(NULL, "|");
	}
	result = (char)*slpitPointer;
	signInMenu->setVisible(false);
	if(result == '3'){
		if(promptBox == NULL){
			cocos2d::Size winSize = Director::getInstance()->getWinSize();
			promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
			promptBox->setPromptText(pConfigMgr->text("t575"));
			this->addChild(promptBox,1);
		}else{
			promptBox->setPromptText(pConfigMgr->text("t575"));
		}	
		char message[1024];
		time_t tt = time(NULL);//��䷵�ص�ֻ��һ��ʱ��cuo
		tm* t= localtime(&tt);
		std::sprintf(message, "insert into PersonalMsg(msg) values(\'%s     %d-%02d-%02d %02d:%02d:%02d\')", pConfigMgr->text("t575"),
			t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		DBHandler::sharedDBHandler()->execute(message);
	}else{
		if(promptBox == NULL){
			cocos2d::Size winSize = Director::getInstance()->getWinSize();
			promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
			promptBox->setPromptText(pConfigMgr->text("t576"));
			this->addChild(promptBox,1);
		}else{
			promptBox->setPromptText(pConfigMgr->text("t576"));
		}
	}

}

void WarningLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		RoomLayer* layer = (RoomLayer*)this->getParent();
		layer->permitButtonClick();
		this->removeFromParentAndCleanup(true);
	}
}

void WarningLayer::dismiss(Object *obj){
	playButtonSound();
	RoomLayer* layer = (RoomLayer*)this->getParent();
	layer->permitButtonClick();
	this->removeFromParentAndCleanup(true);

}

void WarningLayer::onGetTaskAward(Object* obj)
{
	//MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(String::createWithFormat("%s%d!",pConfigMgr->text("t128"), TaskInfo::ShareTaskInfo()->m_nTaskConfig[1])->getCString(),Vec2(427,240));
	//this->addChild(layer,20);
	//TaskInfo::ShareTaskInfo()->m_nGetState[0] = 1;
	this->signIn();
}