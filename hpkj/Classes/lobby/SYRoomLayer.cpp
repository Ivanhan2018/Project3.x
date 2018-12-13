#include "roomLayer.h"
#include "GBEventIDs.h"
#include "TableViewLayer.h"
#include "ConfigMgr.h"
#include "utf-8.h"
#include "GBEvent.h"
#include "PlayerInfoLayer.h"
#include "RoomOption.h"
#include "EmailLayer.h"
#include "ActivityLayer.h"
#include "ExitLayer.h"
#include "CustomerServiceLayer.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "SceneControl.h"
#include "GetCharge.h"
#include "GroupSprite.h"
#include "TaskInfo.h"
#include "RuningBar.h"
#include "CShaderSprite.h"
#include "FreeLayer.h"
#include "packet.h"
#include "CMD_GameServer.h"
#include "FreeCoins.h"
#include "MissionLayer.h"
#include "FriendSystemLayer.h"
#include "HelpLayer.h"
#include "ShopSystemLayer.h"
#include "MissionLayer.h"
#include "ExchangerLayer.h"
#include "LuckyDraw.h"
#include "LoginLayer.h"
#include "VIPSystemLayer.h"
#include "RankSystemLayer.h"
#include "GuessCoinSide.h"
#include "JsonHelper.h"
#include "MyNSString.h"
#include "ScrollAdd.h"
#include "LotteryKindScrollView.h"
#include "BJLSceneControl.h"
#include "SceneView.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "AppController.h"
#import "ConfigHeader.h"
#endif

#include "network/HttpClient.h"

using namespace cocos2d;
using namespace cocos2d::ui;

Scene* RoomLayer::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    Layer *layer = RoomLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

RoomLayer::RoomLayer()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //[YouMiNewSpot showYouMiSpotAction:nil];
#endif
    pWarningLayer=NULL;
    isGoingRoomLayer = 0;
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::showDailyGetCoins),MSG_UI_ANS_CANNELRECHANGER , NULL);
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(RoomLayer::showDailyGetCoinsResult),MSG_UI_ANS_GETSAVECOINS,NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onlandcardResult),MSG_UI_ANS_LANDCARDRESULT , NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onPhoneCardResult),MSG_UI_ANS_PHONECARDRESULT , NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onLinkResult),MSG_UI_ANS_GAMELINK , NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onChange),MSG_UI_ANS_CHARGE, NULL);
    NotificationCenter::getInstance()->postNotification("CancelHttpRequest");
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::updateArticle), "UpdateUserPacket", NULL);
}

RoomLayer::~RoomLayer()
{
    //GBEVENT_ARG_UNREGISTER(GBEVENT_UI_LOGINFINISH, RoomLayer::onLoginFinish);
    NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_REGNFAIL);
    NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_CANNELRECHANGER);
    NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_GETSAVECOINS);
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_UPDATESCOREANDGOLDEGG);
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LANDCARDRESULT);
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_PHONECARDRESULT);
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFAIL);
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_CHARGE);
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_REFRESHTOTALNUM);
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_GAMELINK);
    NotificationCenter::getInstance()->removeObserver(this, "UpdateUserPacket");
	this->unschedule(schedule_selector(RoomLayer::getSystemTime));
}

void RoomLayer::updateArticle(Object *obj)
{
    Array *data = (Array *)obj;
    for(int i = 0 ; i < data->count(); i+=2)
    {
        Integer *number1 = (Integer *)data->objectAtIndex(i);
        Integer *number2 = (Integer *)data->objectAtIndex(i+1);
        int key = number1->getValue();
        int value = number2->getValue();
        pMsgDispatch->m_wArticleNum[key-2] = value;
    }
}

void RoomLayer::automaticLogin(bool automatic,bool overResgist /* = false */)
{
    if(automatic)
    {
        
        bool bRet = pMsgDispatch->connectLoginServer();
        if(bRet)
        {
            if(EntityMgr::instance()->login()->getAccout().length() > 0 && !overResgist)
            {
                onLoginFinish(NULL);
            }
            else
            {
                std::string userAccount = UserDefault::getInstance()->getStringForKey("NEWZJD_ACCOUNT");
                std::string userPassword = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");
                if(userAccount.length() > 1 &&userPassword.length() > 1)
                {
                    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onLoginFaild), MSG_UI_ANS_LOGINFAIL, NULL);
                    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onLoginFinish), MSG_UI_ANS_LOGINFINISH, NULL);
                
                    bool bRet = pMsgDispatch->connectLoginServer();
                    if(!bRet)
                    {
                        MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t26"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
                       this->addChild(layer,100);
                    }
                    else
                    {
                        EntityMgr::instance()->login()->setUserLogin(userAccount.c_str(), userPassword);
                    }
                }
                else
                {
                    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onRegisterFaild), MSG_UI_ANS_REGNFAIL, NULL);
                    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RoomLayer::onRegistSucc), MSG_UI_ANS_REGSUCC, NULL);
                    int nGender = 0;
                    int nFaceId = 0;
                    string szAccount = createAccount();
                    string szPassword = createPassword();
                    
                    bool bRet = pMsgDispatch->connectLoginServer();
                    if (!bRet)
                    {
                        PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeServerShut);
                       this->addChild(box,100);
                    }
                    EntityMgr::instance()->login()->setUserRegister(szAccount.c_str(),szPassword.c_str(),nFaceId,nGender,1);
                    
                }
            }
        }
        else
        {
            PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeServerShut);
           this->addChild(box,100);
        }
    }
}

void RoomLayer::onLoginFinish(Object *obj)
{
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFINISH);
    showLoginAward();
}

void RoomLayer::onLoginFaild(Object* obj)
{
    NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFAIL);
    CCLOG("login faied");
    
    PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeLoginFaild);
   this->addChild(box,100);
}

void RoomLayer::onEnter()
{
    if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
    {
        playBGM("roomback.mp3");
    }

	BaseLayer::onEnter();
}

void RoomLayer::onExit()
{
	BaseLayer::onExit();
}

bool RoomLayer::init()
{
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }

	Widget* Root = static_cast<Widget*>(CSLoader::createNode("ScenePlaza.csb"));

	addChild(Root, 0, "box1");

	ScenePlazaRoot = dynamic_cast<Widget*>(Root->getChildByName("lobbyPanel"));

	Size size = Director::getInstance()->getWinSize();
	Root->setScale(size.width/ ScenePlazaRoot->getContentSize().width, size.height/ ScenePlazaRoot->getContentSize().height);

	Sprite* bgBackLobby = Sprite::create("Share/LoginBackView.jpg");
	Size size1 = Director::getInstance()->getWinSizeInPixels();
	float fScaleX = size1.width / bgBackLobby->getContentSize().width;
	float fScaleY = size1.height / bgBackLobby->getContentSize().height;
	bgBackLobby->setScaleX(fScaleX);
	bgBackLobby->setScaleY(fScaleY);
	bgBackLobby->setPosition(size1.width / 2, size1.height / 2);
	this->addChild(bgBackLobby, -10);

	Node* pNode1 = Helper::seekWidgetByName(ScenePlazaRoot, "btn_mahjong_invite");
	pNode1->setVisible(false);

	const char *szBtnNameArr[] = { "btn_game_bjl" ,"btn_game_ddz","btn_game_by","btn_game_zzmj","btn_game_brnn","btn_game_ppc"};
	int nNum = sizeof(szBtnNameArr) / sizeof(szBtnNameArr[0]);
	for (int i = 0; i < nNum; i++)
	{
		Button* fksx = dynamic_cast<Button*>(Helper::seekWidgetByName(ScenePlazaRoot, szBtnNameArr[i]));
		fksx->setTag(i);
		fksx->addTouchEventListener(CC_CALLBACK_2(RoomLayer::btnCallBack,this,i));
	}

    mMenuCloseOrOpen = false;
    isGoingRoomLayer =0;

    winSize = Director::getInstance()->getWinSize();

	pMsgDispatch->SendPacketWithGetSystemTime();
	this->schedule(schedule_selector(RoomLayer::getSystemTime), 10);

	static bool isFirstRun = true;

	//控制程序只运行一次
	if (isFirstRun) {
		sendHttpRequest();
		isFirstRun = false;
	}

    return  true;
}

void RoomLayer::btnCallBack(Ref * pSender, cocos2d::ui::Widget::TouchEventType type, int tag)
{
	CCLOG("tag=%d\n",tag);
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{

	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		if(tag==0)//百家乐
		{
			bool ret=pMsgDispatch->connectGameServer(KindId_BJL);
			if (!ret)
			{
				PromptBox* box = PromptBox::PromptBoxWith(ccp(427,240),mPromptTypeServerShut);
				this->addChild(box,20);
				return;
			}
			pMsgDispatch->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
			pMsgDispatch->SendPacketWithEnterRoomPageCount(6);
			pMsgDispatch->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
			pMsgDispatch->setStartType(true);	
		} 
		else if(tag==2)//2捕鱼
		{
			bool ret=pMsgDispatch->connectGameServer(GameId_dzpk);
			if (!ret)
			{
				PromptBox* box = PromptBox::PromptBoxWith(ccp(427,240),mPromptTypeServerShut);
				this->addChild(box,20);
				return;
			}
			pMsgDispatch->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
			pMsgDispatch->SendPacketWithEnterRoomPageCount(1);
			pMsgDispatch->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
			pMsgDispatch->setStartType(true);
			//Director::getInstance()->replaceScene(DZPKLayer::scene());
		}
		else{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("display_text.xml", "t900"), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 4));
			Director::getInstance()->getRunningScene()->addChild(layer,255);	
		}
	}
}


void RoomLayer::getSystemTime(float dt)
{
	pMsgDispatch->SendPacketWithGetSystemTime();
}

void RoomLayer::showLoginAward()
{

}
void RoomLayer::closeLoginAward(){

}

void RoomLayer::showPlayerInformation(Object *obj){
    playButtonSound();
    Scene *scene = Scene::create();
    //LotteryUserInfo *layer = LotteryUserInfo::create();
    //scene->addChild(layer);
    //Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void RoomLayer::toRoom(Object* obj)
{
  
}

void RoomLayer::toGamble(Object* obj)
{

}

void RoomLayer::toRecord(Object* obj)
{

}

void RoomLayer::toGame(Object* obj)
{	
	resetSelect();
	MenuItemSprite* temp = static_cast<MenuItemSprite*>(obj);
	temp->selected();

	std::string name = "game_kind_";
	int count = 4;

	static_cast<LotteryKindScrollView* >(pHelpView)->resetTable(name.c_str(), count, 0);
}

void RoomLayer::toLottery(Object* obj)
{
	resetSelect();
	MenuItemSprite* temp = static_cast<MenuItemSprite*>(obj);
	temp->selected();	

	std::string name = "lottery_kind_";
	int count = 12;
	static_cast<LotteryKindScrollView* >(pHelpView)->resetTable(name.c_str(), count, 1);
}

void RoomLayer::toPhysic(Object* obj)
{
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("display_text.xml", "t900"), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 4));
	this->addChild(layer,20);

	return;

	resetSelect();
	MenuItemSprite* temp = static_cast<MenuItemSprite*>(obj);
	temp->selected();

	std::string name = ""; //目前尚未开放
	int count = 4;
	static_cast<LotteryKindScrollView* >(pHelpView)->resetTable(name.c_str(), count, 2);
}

void RoomLayer::resetSelect()
{
	int count = selectMenu->getChildrenCount();
	for(int i = 0;i < count;i ++)
	{
		MenuItemSprite* temp = (MenuItemSprite *)selectMenu->getChildren().at(i);
		if(temp != NULL)
			temp->unselected();
	}
}

void RoomLayer::refreshLayer()
{
	pMsgDispatch->SendPacketWithGetSystemTime();
}

void RoomLayer::toMyLottery(Object* obj)
{

}

void RoomLayer::sendHttpRequest()
{	
	//宏定义手机平台
	string url = pConfigMgr->text("display_text.xml", "t958"); 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	url = pConfigMgr->text("display_text.xml", "t957"); 
#endif
	CCLOG("%s completed", url.c_str());
	network::HttpRequest* request = new network::HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, callfuncND_selector(RoomLayer::onHttpRequestCompleted));
	request->setTag("GET test1");	
	network::HttpClient::getInstance()->send(request); 
	network::HttpClient::getInstance()->setTimeoutForConnect(10);
	network::HttpClient::getInstance()->setTimeoutForRead(10);
	request->release();
}

void RoomLayer::onHttpRequestCompleted(Node *sender, void *data)  
{  
	network::HttpResponse *response = (network::HttpResponse*)data;  

	if (!response)  
	{  
		return;  
	}  

	if (0 != strlen(response->getHttpRequest()->getTag()))   
	{  
		CCLOG("%s completed", response->getHttpRequest()->getTag());  
	}  

	int statusCode = response->getResponseCode();  
	char statusString[64] = {};  
	std::sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());  
	CCLOG("response code: %d", statusCode);  

	if (!response->isSucceed())   
	{  
		CCLOG("response failed");  
		CCLOG("error buffer: %s", response->getErrorBuffer());
		return;  
	}  

	// dump data  
	std::vector<char> *buffer = response->getResponseData();
	std::string temp(buffer->begin() + 3,buffer->end());
    String* responseData=String::create(temp);	

	Json::Reader reader;//json解析  
    Json::Value value;//表示一个json格式的对象  
	if(reader.parse(responseData->getCString(),value))//解析出json放到json中区  
    {  		
		string vercode = value["version"].asString();
		string updateUrl = value["packageUrl"].asString();
	
		//获得当前版本号 直接从xml中读取版本号
		string currVercode = pConfigMgr->text("display_text.xml", "t959"); 
		if (vercode.compare(currVercode) != 0)
		{
			string url = pConfigMgr->text("display_text.xml", "t956") + updateUrl;
	
	#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		TCHART w_url[100] = {0};
		MyNSString::mbs2wc(url.c_str(), strlen(url.c_str()), w_url);
		ShellExecute(NULL, L"open", w_url, NULL, NULL, SW_SHOWNORMAL);
	#endif
	#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		Application::getInstance()->openURL(url.c_str());
	#endif
	#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Application::getInstance()->openURL(url.c_str());
		exit(0);
	#endif
		}	
	}
} 

void RoomLayer::setting(Object* obj)
{

}

void RoomLayer::permitButtonClick()
{
	m_pPlayerMenu->setTouchEnabled(true);
	m_pButtonMenu->setTouchEnabled(true);
	pHelpView->setTouchEnabled(true);
	pScrollAdd->setTouchEnabled(true);
	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);
}

void RoomLayer::forbidButtonClick()
{
	m_pPlayerMenu->setTouchEnabled(false);
	m_pButtonMenu->setTouchEnabled(false);
	pHelpView->setTouchEnabled(false);
	pScrollAdd->setTouchEnabled(false);
	this->setKeypadEnabled(false);
}

string RoomLayer::createAccount()
{
    string szAccount;
    int t = 0;
    srand((unsigned)time(NULL));
    for (int i = 0; i < 4; i++)
    {
        //t = rand() % 2 ? 65 : 97;
        t = 97;
        t += rand() % 26;
        szAccount += (char)t;
        
    }
    
    for (int i = 0; i < 4; i++)
    {
        t = rand() % 10;
        szAccount +=  ans::AnsString::int2String(t);
        
    }
    return  szAccount;
}

string RoomLayer::createPassword()
{
    string szPassword;
    int t = 0;
    srand((unsigned)time(NULL));
    
    for (int i = 0; i < 6; i++)
    {
        t = rand() % 10;
        szPassword +=  ans::AnsString::int2String(t);
    }
    return  szPassword;
}

void RoomLayer::onRegistSucc(Object* obj)
{
    NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_REGSUCC);
    showLoginAward();
}

void RoomLayer::onlandcardResult(Object* obj)
{
    String* str = (String*)obj;
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str->getCString(), Vec2(427, SCREEN_WIDTH/2));
   this->addChild(layer,20);
}

void RoomLayer::onPhoneCardResult(Object* obj)
{
    String* str = (String*)obj;
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str->getCString(), Vec2(427, SCREEN_WIDTH/2));
   this->addChild(layer,20);
}

void RoomLayer::onLinkResult(Object* obj)
{
    Integer* Interger = (Integer*)obj;
    
    switch (Interger->getValue())
    {
        case 0:
            break;
            
        case 1:
        {
			DWORD m_dwKindID=pMsgDispatch->m_dwKindID;
			if(m_dwKindID==KindId_BJL){
				BJLSceneControl* pSceneControl = BJLSceneControl::sharedSceneControl();
				Scene* pScene = pSceneControl->getScene(SCENE_game,true);
				Director::getInstance()->replaceScene(pScene);
			}
			//else if (m_dwKindID == GameId_dzpk)
			//{
			//	Scene* pScene = DZPKLayer::scene();
			//	Director::getInstance()->replaceScene(pScene);
			//}
        }
            break;
            
        case 2:
        {
            PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeMakeSureModifyNickName);
            box->setPromptText(pConfigMgr->text("t475"));
           this->addChild(box,100);
        }
            break;
        case 3:
        {
            gameToBankrupt(true);
        }
            
        default:
            break;
    }
}
void RoomLayer::setGoingRoomLayer(){
    isGoingRoomLayer++;
}

void RoomLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		forbidButtonClick();
		playButtonSound();
		PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptexitSelect);
		this->addChild(box,100,123);
	}
}

void RoomLayer::onRegisterFaild(Object* obj)
{
    int nGender = 1;
    int nFaceId = 0;
    string szAccount = createAccount();
    string szPassword = createPassword();
    
    bool bRet = pMsgDispatch->connectLoginServer();
    EntityMgr::instance()->login()->setUserRegister(szAccount.c_str(),szPassword.c_str(),nFaceId,nGender,1);
}

void RoomLayer::showMoveingLabelTips(const char* str)
{
    MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str,Vec2(winSize.width * 0.5,winSize.height * 0.5));
   this->addChild(layer,100);
}

void RoomLayer::onPesent(Object* obj)
{
    tagScoreGifObj* obj1 = (tagScoreGifObj*)obj;
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
    PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypePasswordFind);
    this->addChild(promptBox,20);
    promptBox->setPromptText((const char*)obj1->szErrorDescribe);
}

void RoomLayer::onChange(Object* obj)
{
    tagChargeObj* obj1 = (tagChargeObj*)obj;
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
    PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypePasswordFind);
    this->addChild(promptBox,20);
    promptBox->setPromptText((const char*)obj1->szErrorDescribe);
}

void RoomLayer::gameToBankrupt(bool isBankrupt)
{
    if(pConfigMgr->m_Config.m_isGameBankrupcy)
    {
        pConfigMgr->m_Config.m_isGameBankrupcy = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showAndroidRechageLayer(String::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"1","4");
#endif
        return;
    }

    if(isBankrupt)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showAndroidRechageLayer(String::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"1","4");
#endif
    }
}

void RoomLayer::showDailyGetCoins(Object* obj)
{
    if(g_GlobalUnits.getGolbalUserData()->nRelieveCount > 0)
    {
        bool ret = pMsgDispatch->connectLoginServer();
        if (!ret)
        {
			PromptBox* box = PromptBox::PromptBoxWith(Vec2(427, SCREEN_WIDTH/2), mPromptTypeServerShut);
            this->addChild(box,20);
            return;
        }
        pMsgDispatch->getRelieve(g_GlobalUnits.getGolbalUserData()->dwUserID);
    }
    else
    {
        Scene* scene = Scene::create();
        FreeLayer* layer = FreeLayer::create();
        scene->addChild(layer);
        Director::getInstance()->replaceScene(scene);
    }
}

void RoomLayer::showDailyGetCoinsResult(Object* obj)
{
    Integer* inter = (Integer*)obj;
    PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeLackCoins);
    box->setPromptText(String::createWithFormat("%s%d%s%d%s",pConfigMgr->text("t476"),inter->getValue(),pConfigMgr->text("t477"),g_GlobalUnits.getGolbalUserData()->nRelieveCount,pConfigMgr->text("t478"))->getCString());
    this->addChild(box,50);
}