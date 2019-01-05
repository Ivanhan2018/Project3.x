#include "LotteryBasicUserInfo.h"
#include "LotteryActiveCenter.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "RoomLayer.h"

//新加
#include "LotterySign.h"
#include "LotterySignEx.h"
#include "LotteryHappyHouse.h"
#include "LotteryRetBack.h"
#include "LotteryNewerBack.h"
#include "VersionControl.h"

#include "MyNSString.h"
//所有代理功能
Scene* LotteryActiveCenter::scene()
{
	Scene *scene = Scene::create();
	LotteryActiveCenter *layer = LotteryActiveCenter::create();
	scene->addChild(layer);
	return scene;
}

LotteryActiveCenter::LotteryActiveCenter()
{
    //userInfoTable = nullptr;
	m_list = nullptr;

#ifdef VER_369
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryActiveCenter::getActiveRet), "GetActiveRet", NULL);
#endif
}
LotteryActiveCenter::~LotteryActiveCenter()
{
	clear();
}

bool LotteryActiveCenter::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	return true;
}

void LotteryActiveCenter::refreshLayer()
{
	//
}

void LotteryActiveCenter::initData()
{
	winSize = Director::getInstance()->getWinSize();
}

void LotteryActiveCenter::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//背景图
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	//返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryActiveCenter::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	//pCloseButton->setPosition(Vec2(32, SCREEN_HEIGHT - 120));
	pCloseButton->setAnchorPoint(Vec2(0,1));
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	//文字 会员中心
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t1119"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 30;
	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = title->getPositionY() - 70;
	//tableViewSize.height = 700;
	auto userInfoTable = TableView::create(this, tableViewSize);
	userInfoTable->setDelegate(this);
	userInfoTable->setAnchorPoint(Vec2(0.5, 0.5));
	userInfoTable->setPosition(Vec2(0, 0));
	userInfoTable->setTag(9527);
	userInfoTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	userInfoTable->setDirection(ScrollView::Direction::VERTICAL);
	//userInfoTable->retain();			//有addchild无需retain, 当然也就无需release了
	
	this->addChild(userInfoTable, 1);

#ifdef VER_369
	EntityMgr::instance()->getDispatch()->SendPacketWithGetActive();
	getActiveRetEx();
#endif
}

void LotteryActiveCenter::onEnter()
{
	BaseLayer::onEnter();
}

void LotteryActiveCenter::onExit()
{
	BaseLayer::onExit();
}

void LotteryActiveCenter::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryActiveCenter::back(Object *obj)
{
	playButtonSound();
	/*Scene *scene = Scene::create();
	RoomLayer *layer = RoomLayer::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));*/
//	Director::getInstance()->popScene();		//2018.10.29
}


cocos2d::Size LotteryActiveCenter::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = 100; //高度调整为100
	return size;
}

ssize_t LotteryActiveCenter::numberOfCellsInTableView(TableView *table)
{
#ifdef VER_369
	if(m_list == nullptr) return 0;
	return m_list->count();
#endif
	return ROWCOUNT;
}
TableViewCell *LotteryActiveCenter::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 


	ccColor3B redColor = ccc3(61,8,40);
	ccColor3B blackColor = ccc3(150,150,150);
	ccColor3B blackColor1 = ccc3(0,0,0);
	float fontSize = 27;
	float gap = 120;
	float textWidth = 200;

	//处理列表的图标
	Texture2D *pTexture = TextureCache::getInstance()->addImage("t1_01C.png");

	cocos2d::Rect logoRect = cocos2d::Rect((index)* 59, 0, 59, 59);
	
	String* tempStr = String::create(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d", 50+index)->getCString()));
	if(index == 3)
		tempStr = String::create(ConfigMgr::instance()->text("display_config.xml","t5030"));

	int count = ROWCOUNT;

#ifdef VER_369
	if(m_list->count() <= index) return cell;

	Dictionary* dic = (Dictionary*) m_list->objectAtIndex(index);
	String* tempRemark = (String *)dic->objectForKey("s_t_Remark");
	int iStr = getIdFromText(tempRemark->getCString()) - 1;

	logoRect = cocos2d::Rect((iStr)* 64, 0, 64, 63);

	tempStr = String::create(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d", 50+iStr)->getCString()));
	if(iStr == 3)
		tempStr = String::create(ConfigMgr::instance()->text("display_config.xml","t5030"));

	count = m_list->count();	
#endif

	if (index < count && pTexture)
	{
		Sprite *ballSprite = Sprite::createWithSpriteFrame(SpriteFrame::createWithTexture(pTexture, logoRect));
		//ballSprite->setAnchorPoint(Vec2(0, 0.5f));
		ballSprite->setPosition(Vec2(30 + 34 + 5, 100 / 2));		
		cell->addChild(ballSprite, 8);
	}	

	LabelTTF *timeLabel = LabelTTF::create(tempStr->getCString()
		,"",fontSize,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	timeLabel->setColor(blackColor1);
	timeLabel->setAnchorPoint(Vec2(0,0.5f));
	timeLabel->setPosition(Vec2(gap, 100/2));
	cell->addChild(timeLabel, 8);

	//右箭头
	Sprite *more = Sprite::createWithSpriteFrame(spriteFrame("setting_01.png"));
	more->setAnchorPoint(Vec2(1,0.5f));
	more->setPosition(Vec2(SCREEN_WIDTH, 100 / 2));
	cell->addChild(more, 8);

	//分隔线
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5f));
	gap1->setPosition(Vec2(0, 0));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);	
	cell->addChild(gap1,9);

	cell->setTag(index + 1);

	return cell;  
}

void LotteryActiveCenter::tableCellTouched(TableView *table, TableViewCell *cell)
{
	int cellTag = cell->getTag();

#ifdef VER_369	
	if(m_list->count() <= cellTag - 1) return;

	Dictionary* dic = (Dictionary*) m_list->objectAtIndex(cellTag - 1);
	String* tempStr = (String *)dic->objectForKey("s_t_Remark");
	cellTag = getIdFromText(tempStr->getCString());
#endif

	switch(cellTag)
	{	
	case 1: //1 是签到 2 是 欢乐送 3佣金大回馈 4充值送
        {
			playButtonSound();
			//Scene *scene = LotteryBasicUserInfo::scene();
            //Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			EntityMgr::instance()->getDispatch()->SendPacketWithGetUserInfo();
			//clear();
			break;
		}
	case 2: //1 是签到 2 是 欢乐送 3佣金大回馈 4充值送
        {
			playButtonSound();    
	        Scene *scene = LotteryHappyHouse::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

			clear();
			break;
		}
	case 3: //1 是签到 2 是 欢乐送 3佣金大回馈 4充值送
        {
			playButtonSound();
            Scene *scene = LotteryRetBack::scene();
            Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

			clear();
			break;
		}
	case 4: //1 是签到 2 是 欢乐送 3佣金大回馈 4充值送
		{
			playButtonSound();
			Scene *scene = LotteryNewerBack::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

			clear();
			break;
		}	
	default:
		break;
	}
}

void LotteryActiveCenter::scrollViewDidScroll(ScrollView* view)
{

}
void LotteryActiveCenter::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryActiveCenter::getActiveRetEx()
{
	Array *dataArray = Array::create();

	TCHART s_t_Remark_new[4][200];
	for (int i = 0; i < 4; i++)
	{
		memset(s_t_Remark_new[i], 0, sizeof(TCHART)* 200);
	}
	String str1 = "DayQiandao";
	memcpy(s_t_Remark_new[0], str1.getCString(), sizeof(TCHART)* 200);
	memcpy(s_t_Remark_new[1], &"huanlesong", sizeof(TCHART)* 200);
	memcpy(s_t_Remark_new[2], &"yongjinhuikui", sizeof(TCHART)* 200);
	memcpy(s_t_Remark_new[3], &"NewChongzhiSong", sizeof(TCHART)* 200);

	std::string strTxt[4];
	strTxt[0] = "DayQiandao";
	strTxt[1] = "huanlesong";
	strTxt[2] = "yongjinhuikui";
	strTxt[3] = "NewChongzhiSong";
	for (int i = 0; i < 4; i++)
	{
		Dictionary* dic = new Dictionary();
		char s_t_Remark[3100] = { 0 };
		MyNSString::wc2mbs(s_t_Remark_new[i], sizeof(s_t_Remark_new[i]), s_t_Remark);
		dic->setObject(String::create(strTxt[i]), "s_t_Remark");

		dataArray->addObject(dic);
	}

	if (m_list != nullptr)
	{
		m_list = nullptr;
	}
	m_list = Array::create();
	m_list->retain();
	m_list->addObjectsFromArray((Array *)dataArray);

	CCLOG("active array count%d", m_list->count());

	TableView* table = (TableView*)this->getChildByTag(9527);

	if (m_list->count() > 0 && table != nullptr)
		table->reloadData();
}

void LotteryActiveCenter::getActiveRet(Object* obj)
{
	if(m_list != nullptr)
	{		
		m_list = nullptr;
	}
	m_list = Array::create();
	m_list->retain();
	m_list->addObjectsFromArray((Array *)obj);	

	CCLOG("active array count%d",m_list->count());

	TableView* table = (TableView* )this->getChildByTag(9527);

	if(m_list->count() > 0 && table != nullptr)
	table->reloadData();
}

int LotteryActiveCenter::getIdFromText(const char* name)
{
	//1 是签到 2 是 欢乐送 3佣金大回馈 4充值送
	std::string tempStr = name;

	if(tempStr.compare("DayQiandao") == 0)
	{
		return 1;
	}else
	if(tempStr.compare("huanlesong") == 0)
	{
		return 2;
	}else
	if(tempStr.compare("yongjinhuikui") == 0)
	{
		return 3;
	}
	else
	if(tempStr.compare("NewChongzhiSong") == 0)
	{
		return 4;
	}else
	{
		return -1;
	}
}

void LotteryActiveCenter::clear()
{

#ifdef VER_369

	NotificationCenter::getInstance()->removeObserver(this,"GetActiveRet");

	if(m_list != nullptr)
	{
		if(m_list->count() > 0)
			m_list->removeAllObjects();
		m_list->release();
		m_list = nullptr;
	}
#endif
}