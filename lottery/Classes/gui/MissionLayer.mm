#include "MissionLayer.h"
#include "RoomLayer.h"
#include "CocosUnits.h"
#include "JsonHelper.h"
#include "DBHandler.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
// 引入相关的头文件
//#include "CCUMSocialSDK.h"
//#include "UMShareButton.h"
// 使用友盟命令空间
//USING_NS_UM_SOCIAL;
#import "AppController.h"
#endif

#include "network/HttpClient.h"

#define minInWarning(a,b) (((a) < (b)) ? (a) : (b))

using namespace cocos2d;

MissionLayer::MissionLayer()
{

}
MissionLayer::~MissionLayer()
{

}
Scene* MissionLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		MissionLayer *layer = MissionLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


MissionLayer *MissionLayer::create()
{
	Array *normalName = Array::create();
	Array *selectedName = Array::create();
	normalName->addObject(String::create("Tab_EverydayMission1.png"));
	selectedName->addObject(String::create("Tab_EverydayMission2.png"));
	normalName->addObject(String::create("Tab_BegginerMission1.png"));
	selectedName->addObject(String::create("Tab_BegginerMission2.png"));
	//normalName->addObject(String::create("Tab_ScoreBoard1.png"));
	//selectedName->addObject(String::create("Tab_ScoreBoard2.png"));
	//normalName->addObject(String::create("Tab_ReferenceBoard1.png"));
	//selectedName->addObject(String::create("Tab_ReferenceBoard2.png"));
	return MissionLayer::createWithTabItems(normalName, selectedName);
}

MissionLayer *MissionLayer::createWithTabItems(Array *itemNormalName, Array *itemSelectedName)
{
	MissionLayer *tabLayer = new MissionLayer();
	if (tabLayer && tabLayer->initWithTabItems(itemNormalName, itemSelectedName))
	{
		tabLayer->initView();
		tabLayer->autorelease();
		return tabLayer;
	}
	else
	{
		CC_SAFE_DELETE(tabLayer);
		return NULL;
	}
}
//void MissionLayer::initView()
//{
//	cocos2d::Size winSize = Director::getInstance()->getWinSize();
//	/**���ư�͸���ɰ�**/
//	LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255*0.5f), 854.0f, SCREEN_WIDTH);
//	layer->ignoreAnchorPointForPosition(false);
//	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
//	this->addChild(layer);
//
//	/**���Ʊ���ͼƬ**/
//	Sprite * pBJ  = Sprite::create("background/bg2.png");
//	pBJ->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
//	this->addChild(pBJ,0);
//
//	maskSprite = Sprite::createWithSpriteFrame(spriteFrame("bg2_2.png"));
//	maskSprite->setPosition(Vec2(winSize.width/2,winSize.height-maskSprite->getContentSize().height/2));
//	this->addChild(maskSprite,2);
//	//tab�˵� ÿ������ �������� ѫ��
//	float itemGap = 10;
//	currentItem = MissionLayerTabEveryday;
//
//	Sprite *everydayImage = Sprite::createWithSpriteFrame(spriteFrame("Tab_EverydayMission2.png"));
//	Sprite *beginnerImage = Sprite::createWithSpriteFrame(spriteFrame("Tab_BegginerMission1.png"));
//	Sprite *awardImage = Sprite::createWithSpriteFrame(spriteFrame("Tab_Achievement1.png"));
//	everydayItem = MenuItemSprite::create(everydayImage,everydayImage,CC_CALLBACK_1(MissionLayer::pressEveryday));
//	beginnerItem = MenuItemSprite::create(beginnerImage,beginnerImage,CC_CALLBACK_1(MissionLayer::pressBeginner));
//	awardItem = MenuItemSprite::create(awardImage,awardImage,CC_CALLBACK_1(MissionLayer::pressAward));
//	Menu* menu = Menu::create(everydayItem,beginnerItem,awardItem,NULL);
//	menu->setAnchorPoint(Vec2(0.5,0.5));
//	float menuWidth = 4*itemGap+everydayImage->getContentSize().width*3;
//	menu->setPosition(Vec2(menuWidth/2,winSize.height*0.9f-2));
//	menu->alignItemsHorizontallyWithPadding(itemGap);//�������з�ʽ��������20
//	this->addChild(menu, 2);
//	/**�رհ�ť**/
//	Sprite *pbuttonNormalClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
//	Sprite *pbuttonSelectClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
//	MenuItemSprite *pButtonClose = MenuItemSprite::create
//		(pbuttonNormalClose,pbuttonSelectClose,CC_CALLBACK_1(MissionLayer::dismiss,this));
//	pbuttonSelectClose->setScale(0.9f);
//	pbuttonSelectClose->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
//	Menu *pButton0 = Menu::create(pButtonClose,NULL);
//	pButton0->setPosition(Vec2(winSize.width * 0.93,winSize.height * 0.93)); 
//	this->addChild(pButton0,2);
//	this->initView();
//	this->setKeypadEnabled(true);
//}

void MissionLayer::initView()
{
	beMissionNum = 4;
	float gapX = 11;
	float gapY = 10;
	winSize = Director::getInstance()->getWinSize();
	winSize.width -= gapX*2;

	Sprite *tempSprite;
	tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend1.png"));
	tableItemSize = tempSprite->getContentSize();
	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = 380;

	//ÿ������
	MissionTableHelper *everydayHelper = new MissionTableHelper(this, 5, MissionLayerTabEveryday);
	everydayTable = TableView::create(everydayHelper, tableViewSize);
	everydayTable->setDelegate(everydayHelper);
	everydayTable->setAnchorPoint(Vec2(0.5,0.5));
	everydayTable->setPosition(Vec2(gapX, gapY));
	everydayTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	everydayTable->setDirection(ScrollView::Direction::VERTICAL);
	everydayTable->retain();
	this->addChild(everydayTable, 1);

	//��������
	MissionTableHelper *beMissionHelper = new MissionTableHelper(this, 5, MissionLayerTabBeginner);
	beMissionTable = TableView::create(this, tableViewSize);
	beMissionTable->setDelegate(this);
	beMissionTable->setAnchorPoint(Vec2(0.5,0.5));
	beMissionTable->setPosition(Vec2(gapX+5, gapY));
	beMissionTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	beMissionTable->setDirection(ScrollView::Direction::VERTICAL);
	beMissionTable->retain();
	this->addChild(beMissionTable, 1);
	beMissionTable->setVisible(false);

	this->setKeypadEnabled(true);
	//this->setTouchEnabled(true);
}


void MissionLayer::doSomething()
{
	CCLOG("RankSystemLayer::doSomething");
	if(currentItem == TabLayerBaseTag){
		everydayTable->setVisible(true);
		beMissionTable->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+1){
		everydayTable->setVisible(false);
		beMissionTable->setVisible(true);
	}else if(currentItem == TabLayerBaseTag+2){
		everydayTable->setVisible(false);
		beMissionTable->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+3){
		everydayTable->setVisible(false);
		beMissionTable->setVisible(false);
	}
}


void MissionLayer::reloadData(MissionLayerTab tag)
{
	if(tag == MissionLayerTabEveryday)
	{
		everydayTable->reloadData();
	}else if(tag == MissionLayerTabBeginner)
	{
		beMissionTable->reloadData();
	}
}

void MissionLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		RoomLayer* layer = (RoomLayer*)this->getParent();
		layer->permitButtonClick();
		this->removeFromParentAndCleanup(true);
	}
}

void MissionLayer::dismiss(Object *obj){
	//playButtonSound();
	//RoomLayer* layer = (RoomLayer*)this->getParent();
	//layer->permitButtonClick();
	//this->removeFromParentAndCleanup(true);

	playButtonSound();
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);
}

cocos2d::Size MissionLayer::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = tableItemSize.height+10;
	return size;
}

/**����е�Ԫ��ĸ���
������table��Ԫ������ı��
*/
ssize_t MissionLayer::numberOfCellsInTableView(TableView *table)
{
	return beMissionNum = 2;
}

void shareCallback(int platform, int stCode,
                   const string& errorMsg)
{
    
}
/**��Ԫ���ʼ��
������table��Ԫ������ı��
*/
TableViewCell *MissionLayer::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	//����ͼ
	Sprite *bk;
	bk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend1.png"));
	bk->setAnchorPoint(Vec2(0,0));
	bk->setPosition(Vec2(0,0.5f));
	cell->addChild(bk);

	Sprite *awardSprite;
	if(index == 0){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board1.png"));
	}else if(index == 1){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board2.png"));
	}else if(index == 2){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board3.png"));
	}else if(index == 3){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board3.png"));
	}else{
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board4.png"));
	}
	awardSprite->setAnchorPoint(Vec2(0,0.5f));
	awardSprite->setPosition(Vec2(17,tableItemSize.height/2+5));
	cell->addChild(awardSprite);

    if (index == 0) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        // 创建分享按钮, 参数1为按钮正常情况下的图片, 参数2为按钮选中时的图片,参数3为友盟appkey, 参数4为分享回调
    /*    UMShareButton *shareButton = UMShareButton::create("Sprite_Friend1.png","Sprite_Friend1.png", "551ca696fd98c548c9000084", share_selector(shareCallback)) ;
        // 显示在友盟分享面板上的平台
        vector<int>* platforms = new vector<int>();
        platforms->push_back(SINA);
        platforms->push_back(WEIXIN) ;
        platforms->push_back(RENREN) ;
        platforms->push_back(DOUBAN) ;
        platforms->push_back(QZONE) ;
        platforms->push_back(QQ) ;
        // 设置友盟分享面板上显示的平台
        shareButton->setPlatforms(platforms);
        // 设置文本分享内容
        shareButton->setShareContent("umeng social cocos2d-x sdk.") ;
        // 设置要分享的图片, 图片支持本地图片和url图片, 但是url图片必须以http://或者https://开头
        shareButton->setShareImage("HelloWorld.png") ;
        // 设置按钮的位置
        shareButton->setPosition(Vec2(tableItemSize.width/2,tableItemSize.height/2+5));
        // 然后开发者需要将该按钮添加到游戏场景中
        Menu* pMenu = Menu::create(shareButton, NULL);
        pMenu->setPosition(Vec2::ZERO);
        cell->addChild(pMenu, 1);
        
        // ********************** 设置平台信息 ***************************
        CCUMSocialSDK *sdk = shareButton->getSocialSDK();
        sdk->setQQAppIdAndAppKey("设置QQ的app id", "appkey");
        sdk->setWeiXinAppInfo("设置微信和朋友圈的app id","app key");
        sdk->setYiXinAppKey("设置易信和易信朋友圈的app id");
        sdk->setLaiwangAppInfo("设置来往和来往动态的app id","设置来往和来往动态的app key", "我的应用名");
        sdk->setFacebookAppId("你的facebook appid");
        //设置用户点击一条图文分享时用户跳转到的目标页面, 一般为app主页或者下载页面
        sdk->setTargetUrl("http://www.umeng.com/social");
        // 打开或者关闭log
        sdk->setLogEnable(true) ;

	*/
        //     **********************	END	***************************
#endif
    }
	Color3B redColor = ccc3(61,8,40);
	float textWidth = 600;
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text(String::createWithFormat("t%d", 594+index)->getCString()),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	title->setColor(redColor);
	title->setAnchorPoint(Vec2(0,0.5f));
	title->setPosition(Vec2(120, 52));
	cell->addChild(title, 1);

	LabelTTF *descibeLabel = LabelTTF::create(ConfigMgr::instance()->text(String::createWithFormat("t%d", 600+index)->getCString()),"",26,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	descibeLabel->setColor(redColor);
	descibeLabel->setAnchorPoint(Vec2(0,0.5f));
	descibeLabel->setPosition(Vec2(120, 22));
	cell->addChild(descibeLabel, 1);
	cell->setTag(index);
	return cell;  
}

/**�����Ԫ��
������table����Ԫ������ı��
cell����Ԫ��
*/
void MissionLayer::tableCellTouched(TableView *table, TableViewCell *cell)
{
    int tag = cell->getTag();
    if (tag == 1) {
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        [[AppController sharedAppController] pressPingFen];
		#endif
    }
}

void MissionLayer::scrollViewDidScroll(ScrollView* view)
{

}
void MissionLayer::scrollViewDidZoom(ScrollView* view)
{

}

void MissionTableHelper::cancelHttpRequest(Object *obj)
{
	cancelOrNot = true;
}

MissionTableHelper::MissionTableHelper(MissionLayer *fath, int num, MissionLayerTab tag)
{
	DBHandler *handler = DBHandler::sharedDBHandler();
	sqlite3 *DB = handler->getDB();
	sqlite3_stmt *stmt;
	char sql[200]={0};
	sprintf(sql, "select * from UserInfo where dwUserID=%ld",EntityMgr::instance()->getDispatch()->m_dwUserID);
	int rv = sqlite3_prepare(DB, sql, 100, &stmt, NULL);
	rv = sqlite3_step(stmt);
	if(rv == SQLITE_ROW)
	{
		cbMemberOrder = sqlite3_column_int(stmt, 3);
		lScore = sqlite3_column_int(stmt, 4);
		LONG lWinCount = sqlite3_column_int(stmt, 5);
		LONG lLostCount = sqlite3_column_int(stmt, 6);
		LONG lDrawCount = sqlite3_column_int(stmt, 7);
		LONG lFleeCount = sqlite3_column_int(stmt, 8);
		totalCount=lWinCount+lLostCount+lDrawCount+lFleeCount;
	}else
	{
		totalCount = 0;
	}
	sqlite3_finalize(stmt);
	cancelOrNot = false;
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MissionTableHelper::cancelHttpRequest),"CancelHttpRequest", NULL);

	field4ValueArray = Array::create();
	field4ValueArray->retain();
	float gapX = 11;
	float gapY = 10;
	father = fath;
	winSize = Director::getInstance()->getWinSize();
	winSize.width -= gapX*2;
	Sprite *tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend1.png"));
	tableItemSize.height = tempSprite->getContentSize().height;
	tableItemSize.height+=10;
	tableItemSize.width = winSize.width;
	itemsNum = num;
	tableTag = tag;
	if(tag == MissionLayerTabEveryday)
	{
        char str[33];
        sprintf(str, "%lu", EntityMgr::instance()->getDispatch()->m_dwUserID);
        string userId = string(str);
		url = "http://117.18.5.229:999//mobile/GetWealthJob.aspx?uid="+userId+"&Rwid=1&Kid=200&Time=65466";
		//string id = "4";
		//url = url + id;
	}
	network::HttpRequest* request = new network::HttpRequest();//����request����,����new�����Ķ�����ʹ��autorelease()��ԭ�����  
	request->setUrl(url.c_str());//����url  
	request->setRequestType(network::HttpRequest::Type::GET);//��������ʽ  
	request->setResponseCallback(this, callfuncND_selector(MissionTableHelper::onHttpRequestCompleted));//���ǻص�����ͻص�����  
	request->setTag("GET test1");//�����û���ʶ������ͨ��response��ȡ  
	network::HttpClient::getInstance()->send(request);//ʹ��network::HttpClient����ʵ��������request  
	request->release();//����release()  
}

MissionTableHelper::~MissionTableHelper()
{
	if(field4ValueArray!=NULL)
		field4ValueArray->release();
	NotificationCenter::getInstance()->removeObserver(this, "CancelHttpRequest");
}

void MissionTableHelper::onHttpRequestCompleted(Node *sender, void *data)  
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
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());  
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
	char dataBuffer[32768]; 
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
	Json::Value taskArray = value["task"];
	int size = taskArray.size();
	for(Json::ValueIterator it = taskArray.begin() ; it!=taskArray.end() ; it++)
	{
		Json::Value childValue = *it;
		int field4Str = childValue["always"].asInt();
		this->addTableData(field4Str);
	}
	father->reloadData(tableTag);
} 

void MissionTableHelper::addTableData(int field4)
{
	if(field4ValueArray == NULL){
		field4ValueArray = Array::create();
	}
	Integer *field4CCStr = Integer::create(field4);
	field4ValueArray->addObject(field4CCStr);
}

cocos2d::Size MissionTableHelper::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = tableItemSize.height;
	return size;
}

/**����е�Ԫ��ĸ���
������table��Ԫ������ı��
*/
ssize_t MissionTableHelper::numberOfCellsInTableView(TableView *table)
{
	return field4ValueArray->count();
}

/**��Ԫ���ʼ��
������table��Ԫ������ı��
*/
TableViewCell *MissionTableHelper::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	//����ͼ
	Sprite *bk;
	bk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend1.png"));
	bk->setAnchorPoint(Vec2(0,0));
	bk->setPosition(Vec2(0,5));
	cell->addChild(bk);
	//����
	Sprite *awardSprite;
	if(index == 0){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Mission2.png"));
	}else{
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Mission1.png"));
	}
	awardSprite->setAnchorPoint(Vec2(0,0.5f));
	awardSprite->setPosition(Vec2(17,tableItemSize.height/2));
	cell->addChild(awardSprite);

	Color3B redColor = ccc3(61,8,40);
	float textWidth = 600;
	float textSize1 = 30;
	float textSize2 = 25;
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text(String::createWithFormat("t%d", 700+index)->getCString()),"",textSize1,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	title->setColor(redColor);
	title->setAnchorPoint(Vec2(0,0.5f));
	title->setPosition(Vec2(110, 57));
	cell->addChild(title, 1);

	LabelTTF *descibeLabel = LabelTTF::create(ConfigMgr::instance()->text(String::createWithFormat("t%d", 706+index)->getCString()),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	descibeLabel->setColor(ccc3(45,137,187));
	descibeLabel->setAnchorPoint(Vec2(0,0.5f));
	descibeLabel->setPosition(Vec2(110, 27));
	cell->addChild(descibeLabel, 1);

	int always = ((Integer *)field4ValueArray->objectAtIndex(index))->getValue();
	LabelTTF *progress = LabelTTF::create(ConfigMgr::instance()->text("t712"),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	progress->setColor(redColor);
	progress->setAnchorPoint(Vec2(0,0.5f));
	progress->setPosition(Vec2(460, 57));
	cell->addChild(progress, 1);
	LabelTTF *progress1;
	if(index == 3)
	{
		progress1 = LabelTTF::create(String::createWithFormat("%d/50", totalCount)->getCString(),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	}else if(index == 4) 
	{
		progress1 = LabelTTF::create(String::createWithFormat("%d/100", totalCount)->getCString(),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	}else if(index == 5) 
	{
		progress1 = LabelTTF::create(String::createWithFormat("%d/500", totalCount)->getCString(),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	}else{
		if(index == 2)
		{
			if(always == 0){
				progress1 = LabelTTF::create("0/3","",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
			}else if(always == 1){
				progress1 = LabelTTF::create("1/3","",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
			}else if(always == 2){
				progress1 = LabelTTF::create("2/3","",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
			}else{
				progress1 = LabelTTF::create("3/3","",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
			}
		}else
		{
			if(always == 0){
				progress1 = LabelTTF::create("0/1","",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
			}else{
				progress1 = LabelTTF::create("1/1","",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
			}
		}
	}
	progress1->setColor(redColor);
	progress1->setAnchorPoint(Vec2(0,0.5f));
	progress1->setPosition(Vec2(530, 57));
	cell->addChild(progress1, 1);

	LabelTTF *descibeLabel1;
	if(index == 0)
	{
		if(cbMemberOrder<1){
			descibeLabel1 = LabelTTF::create(ConfigMgr::instance()->text("t715"),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
		}else
		{
			if(always < 1){
				descibeLabel1 = LabelTTF::create(ConfigMgr::instance()->text("t716"),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
			}else
			{
				descibeLabel1 = LabelTTF::create(ConfigMgr::instance()->text("t717"),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
			}
		}
	}else
	{
		if(always < 1){
			descibeLabel1 = LabelTTF::create(ConfigMgr::instance()->text("t718"),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
		}else if(always == 1){
			descibeLabel1 = LabelTTF::create(ConfigMgr::instance()->text("t716"),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
		}else{
			descibeLabel1 = LabelTTF::create(ConfigMgr::instance()->text("t717"),"",textSize2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
		}
	}
	descibeLabel1->setColor(ccc3(255, 0, 0));
	descibeLabel1->setAnchorPoint(Vec2(0,0.5f));
	descibeLabel1->setPosition(Vec2(460, 27));
	cell->addChild(descibeLabel1, 1);

	if((index == 0&&cbMemberOrder>1&&always == 1)||(index==1&&always>0&&always<4&&lScore<1000)||(index>1&&always == 1))
	{
		Sprite *getItemImage1 = Sprite::createWithSpriteFrame(spriteFrame("Button_MissonAchieve1.png"));
		Sprite *getItemImage2 = Sprite::createWithSpriteFrame(spriteFrame("Button_MissonAchieve2.png"));
		MenuItemSprite *menuItem = MenuItemSprite::create(getItemImage1,getItemImage2,CC_CALLBACK_1(MissionTableHelper::pressGet,this));
		Menu *menu = Menu::createWithItem(menuItem);
		menu->setPosition(Vec2(tableItemSize.width-80,tableItemSize.height/2));
		cell->addChild(menu);
	}
	return cell;  
}

/**�����Ԫ��
������table����Ԫ������ı��
cell����Ԫ��
*/
void MissionTableHelper::tableCellTouched(TableView *table, TableViewCell *cell)
{
	//LabelTTF *nameLabel = (LabelTTF *)cell->getChildByTag(nameLabelTag);
	//const char *name = nameLabel->getString();
	//textField->setString(name);
	//this->removeChild(_tableView, true);
	//flag = 0;
}

void MissionTableHelper::scrollViewDidScroll(ScrollView* view)
{

}
void MissionTableHelper::scrollViewDidZoom(ScrollView* view)
{

}

void MissionTableHelper::pressGet(Object *obj)
{

}
