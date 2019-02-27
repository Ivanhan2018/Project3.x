#include "LotteryGameResult.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "LotterySceneControl.h"
#include "LotteryGameResultDetail.h"
#include "bjpk10Layer.h"
#include "FC3DPL3Layer.h"
#include "ChongQingGambleLayer.h"
#include "GuangDong11Xuan5Layer.h"
#include "BetLimit.h"
#include "LotteryGambleLayer.h"
#include "XingYunGambleLayer.h"
#include "BJK8GambleLayer.h"
#include "LiuHeGambleLayer.h"
#include "VersionControl.h"

using namespace CocosDenshion;
using namespace extension;

Scene* LotteryGameResult::scene()
{
	Scene *scene = Scene::create();
	LotteryGameResult *layer = LotteryGameResult::create();
	scene->addChild(layer);
	return scene;
}

LotteryGameResult::LotteryGameResult()
{
	RefreshSprite = NULL;
	//startPoint = Vec2(0,0);
	scrollLen = 0;

	m_bRefreshEnable = true;
	m_bLoadData = false;
	m_needLoad = 0;
	m_data = Array::create();
	m_data->retain();
	m_iCount = 0;
	//SpeefTable =true;
	ScrollTest =false;
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryGameResult::queryGameRet), "QueryGameRet", NULL);
}
LotteryGameResult::~LotteryGameResult()
{
	clear();
}

void LotteryGameResult::queryGameRet(Object *obj)
{	
	Array *receivedData = (Array *)obj;
	if (receivedData->count() == 0)	return;

	m_needLoad ++;
	//CCLOG("````````````````````````````````%d", m_needLoad);
	//static int temp = 0;
	//String* tempStr = String::createWithFormat("load_data_%d", temp ++);
	//CCLOG("```````````````````````````````````````%s", tempStr->getCString());

	//添加进数组
	m_data->addObjectsFromArray(receivedData);
	m_iCount = (m_data->count() - 50 * 4) / 16  + 1 ;//幸运28  总50条记录

	if(m_iCount > 5 )
	{	
		//刷新列表
		gameResultTable->reloadData();	
		m_bRefreshEnable = false;
		this->runAction(		
			Sequence::create(				
			TargetedAction::create(RefreshSprite,RotateTo::create(0.2,0)),
			DelayTime::create(30.0f),
			CallFunc::create(this,callfunc_selector(LotteryGameResult::ZhongJiangUpdate)),		
			NULL));
	}

	//this->scheduleOnce([=](float dt){
	//	m_data->addObjectsFromArray(receivedData);
	//	receivedData->release();
	//	m_needLoad --;
	//	//CCLOG("````````````````shedule````````````````%d", m_needLoad);
	//	if(m_needLoad == 0)
	//	{
	//		m_iCount = (m_data->count() - 50 * 4) / 16  + 1 ;//幸运28  总50条记录
	//		gameResultTable->reloadData();	

	//		m_bRefreshEnable = false;

	//		this->runAction(		
	//			Sequence::create(				
	//			TargetedAction::create(RefreshSprite,RotateTo::create(0.2,0)),
	//			DelayTime::create(30.0f),
	//			CallFunc::create(this,callfunc_selector(LotteryGameResult::ZhongJiangUpdate)),		
	//			NULL));
	//	}
	//},m_needLoad / 10.0f,tempStr->getCString());

	//如果已经加载数据
	//if(m_bLoadData) 
	//{
	//	return;
	//}
	//m_bLoadData = true;

	//m_bRefreshEnable = true;
}

bool LotteryGameResult::init()
{
	if (!Layer::init())
	{
		return false;
	}
	winSize = Director::getInstance()->getWinSize();
	cellHeight = 120;
	this->initView();
	m_data->removeAllObjects();
	m_iCount = 0;
	m_needLoad = 0;
	EntityMgr::instance()->getDispatch()->SendPacketQueryGameResult(0);
	this->setKeypadEnabled(true);
	return true;
}

void LotteryGameResult::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//顶部背景
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	//默认返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryGameResult::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));	
	this->addChild(pCloseButton,1);

	//线位2
	//Sprite *bottomBk = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	//bottomBk->setPosition(Vec2(0, SCREEN_HEIGHT - 170));
	//bottomBk->setScaleX(10);
	//bottomBk->setScaleY(1);
	//this->addChild(bottomBk,2);

	//开奖结果文字
	float fontSize = 38;
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t14"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	//m_bRefreshEnable = true;
	//开奖结果table
	cocos2d::Size tableViewSize;
	tableViewSize.width = SCREEN_WIDTH;
	tableViewSize.height = SCREEN_HEIGHT-162;
	gameResultTable = TableView::create(this, tableViewSize);
	gameResultTable->setDelegate(this);
	gameResultTable->setAnchorPoint(Vec2(0.5, 0.5));
	gameResultTable->setPosition(Vec2(0, 0));
	gameResultTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	gameResultTable->setDirection(ScrollView::Direction::VERTICAL);
	gameResultTable->retain();
	this->addChild(gameResultTable, 1);
}

void LotteryGameResult::onEnter()
{
	BaseLayer::onEnter();
}

void LotteryGameResult::onExit()
{
	BaseLayer::onExit();
}

void LotteryGameResult::back(Object *obj)
{
	playButtonSound();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	Scene* scene = Scene::create();
	scene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void LotteryGameResult::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

cocos2d::Size LotteryGameResult::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = 440;//cellHeight*7;		//*7
	return size;
}

ssize_t LotteryGameResult::numberOfCellsInTableView(TableView *table)
{
	//CCLOG("AAAAAAAAAAA = %d", m_data->count()/4/5);

	return m_iCount;
}
TableViewCell *LotteryGameResult::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	string result = "";
	if(m_iCount <= 0 || index >= m_iCount - 1) return cell; 

	int nowIndex = index;

	int tempIndex = 0;
	static int need_index = 999;
	if(nowIndex > need_index) 
	{
		tempIndex = 50 * 4;
	}

	Integer *kind = (Integer *) m_data->objectAtIndex(nowIndex*16 + tempIndex);

	if(kind->getValue() == 17) 
	{
		need_index = nowIndex; //幸运28 总共50条
	}

	kind = (Integer *) m_data->objectAtIndex(nowIndex*16 + tempIndex);

	bool isKuai8 = kind->getValue() == 10; //北京快8
	bool isXingYun28 = kind->getValue() == 17;
	
	result+=ConfigMgr::instance()->text("display_config.xml", String::createWithFormat("t%d", 20+kind->getValue())->getCString());

	ScrollTest = true;
	ccColor3B redColor = ccc3(100,100,100);
	float textWidth = SCREEN_WIDTH-200;

	int nfontsize = 30;

	//下拉立即刷新
	if (index == 0)
	{
		//下拉的箭头
		RefreshSprite = Sprite::createWithSpriteFrame(spriteFrame("beting54.png"));
		RefreshSprite->setColor(redColor);
		RefreshSprite->setTag(9527);
		RefreshSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		RefreshSprite->setPosition(Vec2(SCREEN_WIDTH / 2, 500));
		cell->addChild(RefreshSprite, 3);

		//下拉刷新字样
		LabelTTF *RefreshLabel = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t12"), "", nfontsize, cocos2d::Size(textWidth, 0), kCCTextAlignmentLeft);
		RefreshLabel->setColor(redColor);
		RefreshLabel->setAnchorPoint(Vec2(0, 0.5f));
		RefreshLabel->setPosition(Vec2(SCREEN_WIDTH / 2 + 30, 500));
		cell->addChild(RefreshLabel, 3);
	}

	static int iDistance = 300;

	//彩种名称
	LabelTTF *descibeLabel = LabelTTF::create(result.c_str(), "", nfontsize, cocos2d::Size(textWidth, 0), kCCTextAlignmentLeft);
	descibeLabel->setColor(redColor);
	descibeLabel->setAnchorPoint(Vec2(0,0.5f));
	descibeLabel->setPosition(Vec2(20, 406));//cellHeight*7-cellHeight/2));
	cell->addChild(descibeLabel, 2);

	//第N期 当前期
	String *wPeriodFirst = (String *)m_data->objectAtIndex(nowIndex*16+1 + tempIndex);
	String *szLotNumFirst = (String *)m_data->objectAtIndex(nowIndex*16+2 + tempIndex);

	CCLOG("period %s, numfirst %s,index %d, nowindex %d, tempIndex %d, pos %d", wPeriodFirst->getCString(), szLotNumFirst->getCString(),index, nowIndex, tempIndex, nowIndex*16+1 + tempIndex);

	result=ConfigMgr::instance()->text("display_text.xml", "t454");
	result.append(wPeriodFirst->getCString());
	result.append(ConfigMgr::instance()->text("display_text.xml", "t455"));
	//ccc3(61,8,40);
	//红色的字体 每一期最近一次的期号 字样
	LabelTTF *descibeLabel1First = LabelTTF::create(result.c_str(), "", nfontsize, cocos2d::Size(textWidth, 0), TextHAlignment::CENTER);
	descibeLabel1First->setColor(redColor);	
	descibeLabel1First->setAnchorPoint(Vec2(0.5f,0.5f));
	descibeLabel1First->setPosition(Vec2(SCREEN_WIDTH / 2, descibeLabel->getPositionY()));//cellHeight*7-cellHeight/2));//向右偏差100
	cell->addChild(descibeLabel1First, 3);

	result=" ";
	result=szLotNumFirst->getCString();
	int len = (int)result.length();
	int step = 1;

	if (len>=10) { //两位数
		step = 2;
	
	}
	//Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	//是否因为图片太大，在IOS上面显示黑色。
	//0-80号球
	Texture2D *ballTexture2D1 = TextureCache::getInstance()->addImage("gamble_A1.png");
	Texture2D *ballTexture2D2 = TextureCache::getInstance()->addImage("gamble_A2.png");
	Texture2D *ballTexture2D3 = TextureCache::getInstance()->addImage("gamble_A3.png");
	Texture2D *ballTexture2D4 = TextureCache::getInstance()->addImage("gamble_A4.png");

	Texture2D *ballTexture2D0 = TextureCache::getInstance()->addImage("gamble_02.png");
	Texture2D *ballTexture2D5 = TextureCache::getInstance()->addImage("gamble_00.png");

	float ballGap = 80;
	bool bTeMa = false;	
	for (int i = 0; i < len; i += step)
	{
		string temp = result.substr(i, step);
		int nPos = temp.find("|");			//处理6和彩特码之前的 |
		if (nPos >= 0)
		{
			if (step == 2)
				i--;
			bTeMa = true;
			continue;
		}
		int number = atoi(temp.c_str());
		int nYpos = 1;		
		//判断球的位置
		SpriteFrame* tempFrame = NULL;
		if (number <= 20){
			if(len == 10){
				tempFrame = SpriteFrame::createWithTexture(ballTexture2D0, Rect((number % 12 - 1)* BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS));
			}else
			{	
				if(number != 0)
				{				
					tempFrame = SpriteFrame::createWithTexture(ballTexture2D1, Rect((number - 1)* BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS));
				} else
				{
					tempFrame = SpriteFrame::createWithTexture(ballTexture2D5, Rect(0, BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS));
				}
			}
		} else
		if (number <= 40){
			tempFrame = SpriteFrame::createWithTexture(ballTexture2D2, Rect((number - 21)* BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS));
		}else
		if (number <=60){
			tempFrame = SpriteFrame::createWithTexture(ballTexture2D3, Rect((number - 41)* BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS));
		}else
		if (number <= 80)
		{
			tempFrame = SpriteFrame::createWithTexture(ballTexture2D4, Rect((number - 61)* BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS));
		}

		//球
		if (!tempFrame) tempFrame = SpriteFrame::createWithTexture(ballTexture2D1, Rect((number - 1)* BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS, BALL_RADDIUS));
		Sprite *ballSprite = Sprite::createWithSpriteFrame(tempFrame);
		
		if (len < 16)
		{			
			ballSprite->setPosition(Vec2(40 + 20 + ballGap*(i % 16) / step, 330));//
			if (bTeMa)
				ballSprite->setColor(ccc3(200, 200, 200));
			cell->addChild(ballSprite, 1);
		} 
	}
	
	int qishu = 4;
	float nYpos = 0;
	if (len > 16)
	{
		nYpos = ballGap;		
		qishu = 3;
		//添加球号码
		if (result.length() >= 10)	step = 2;
		if(isKuai8) result = result.substr(0,result.length() - step);
		if(isXingYun28)
		{
			result = BetLimit::GetXingYunResultStr(result.c_str());
		}else
		{
			transferString(result, step);
		}		
		LabelTTF *ballLabel = LabelTTF::create(result.c_str(), "", nfontsize, cocos2d::Size(SCREEN_WIDTH - 60, 0), TextHAlignment::LEFT);
		ballLabel->setColor(redColor);			
		ballLabel->setPosition(Vec2(SCREEN_WIDTH / 2, 330));
		cell->addChild(ballLabel, 1);
	}
	
	//其它期 早期
	for (int i = 1 ; i < qishu; i++) {  //只显示4期 
		if (m_data->count()<nowIndex*16+4*i+2) {
			break;
		}

		//第多少期 字
		String *wPeriod = (String *)m_data->objectAtIndex(nowIndex*16+4*i+1 + tempIndex);
		String *szLotNum = (String *)m_data->objectAtIndex(nowIndex*16+4*i+2 + tempIndex);
		result=ConfigMgr::instance()->text("display_text.xml", "t454");
		result.append(wPeriod->getCString());
		result.append(ConfigMgr::instance()->text("display_text.xml", "t455"));
		redColor = ccc3(61,8,40);
		LabelTTF *descibeLabel1 = LabelTTF::create(result.c_str(), "", nfontsize);
		descibeLabel1->setColor(redColor);
		descibeLabel1->setAnchorPoint(Vec2(0,0.5f));		
		descibeLabel1->setPosition(Vec2(20, 240 - (nYpos == 0 ? 0 : 10) -(60 + nYpos / ballGap * 30) *(i - 1)));//最上面显示位置为240  cellHeight 60
		cell->addChild(descibeLabel1, 3);

		//每一期的球
		redColor = ccc3(100,0,0);
		result=szLotNum->getCString();
		if (result.length() >= 10)
			step = 2;
		if(isKuai8) result = result.substr(0,result.length() - step);
		if(isXingYun28)
		{
			result = BetLimit::GetXingYunResultStr(result.c_str());
		}else
		{
			transferString(result, step);
		}
		LabelTTF *descibeLabel2 = LabelTTF::create(result.c_str(), "", nfontsize, cocos2d::Size(textWidth, 0), kCCTextAlignmentLeft);
		descibeLabel2->setColor(redColor);
		descibeLabel2->setAnchorPoint(Vec2(0,0.5f));
		descibeLabel2->setPosition(Vec2(descibeLabel1->getContentSize().width + 20, descibeLabel1->getPositionY()));
		cell->addChild(descibeLabel2, 3);

#ifdef VER_369
		descibeLabel1->setColor(ccc3(76,76,76));
		descibeLabel2->setColor(ccc3(50, 62, 123));
#endif
	}

	//立即投注   更多记录按钮
	Sprite *normalImage = Sprite::createWithSpriteFrame(spriteFrame("game_result_01.png"));
	Sprite *selectedImage = Sprite::createWithSpriteFrame(spriteFrame("game_result_02.png"));
	Sprite *normalImage1 = Sprite::createWithSpriteFrame(spriteFrame("game_result_03.png"));
	Sprite *selectedImage1 = Sprite::createWithSpriteFrame(spriteFrame("game_result_04.png"));
	MenuItemSprite *item = MenuItemSprite::create(normalImage,selectedImage,CC_CALLBACK_1(LotteryGameResult::pressMore,this));
	MenuItemSprite *item1 = MenuItemSprite::create(normalImage1,selectedImage1,CC_CALLBACK_1(LotteryGameResult::pressBuy,this));
	item->setTag(kind->getValue());
	item1->setTag(kind->getValue());
	Menu *menu = Menu::create(item1,item,NULL);
	menu->setPosition(Vec2(SCREEN_WIDTH/2, 54));//cellHeight / 2));//440-386
	menu->alignItemsHorizontallyWithPadding(150);
	cell->addChild(menu,3);

	//分割线
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, 0));
	cell->addChild(gap1,5);

	//增加背景色区分	
	LayerColor* lyBackTop = LayerColor::create();
	lyBackTop->setContentSize(cocos2d::Size(SCREEN_WIDTH, 268));//每一个cell高度为440  268
	lyBackTop->setColor(ccc3(245,245,245));
	lyBackTop->setAnchorPoint(Vec2(0,0.5f));
	lyBackTop->setOpacity(255);		
	cell->addChild(lyBackTop,0);

#ifdef VER_369
	menu->alignItemsHorizontallyWithPadding(40);
#endif

	return cell;  
}
void LotteryGameResult::Refresh(Object *obj)
{
	m_data->removeAllObjects();
	m_iCount = 0;
	m_needLoad = 0;
	EntityMgr::instance()->getDispatch()->SendPacketQueryGameResult(0);
}

void LotteryGameResult::transferString(string &str, int nStep)
{
	bool bTeMa = false;
	int nPos = str.find("|");
	if (nPos >= 0)
	{
		str.erase(nPos, 1);
		bTeMa = true;
	}
	int strLen = (int)str.length();
	for (int i = strLen; i >= 0; i -= nStep)
	{
		str.insert(i, "  ");
		
		if(strLen > 20 && i == strLen / 2)
			str.insert(i,"\n");
	}
	if (bTeMa)
		str.insert(str.length() - 4, "| ");
}
void LotteryGameResult::tableCellHighlight(TableView* table, TableViewCell* cell)
{
	//CCLOG("SHAU XIN Highlight");
}
void LotteryGameResult::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
	//CCLOG("SHAU XIN Unhighlight");
}

void LotteryGameResult::tableCellTouched(TableView *table, TableViewCell *cell)
{
	//CCLOG("SHAU XIN Touched");
}

void LotteryGameResult::scrollViewDidScroll(ScrollView* view)
{	
	if (RefreshSprite == NULL) return;	
	//初始化长度
	if (scrollLen == 0) scrollLen = view->getContentOffset().getLength();
	//CCLOG("scroll to:%f",view->getContentOffset().getLength() - scrollLen);
	//下滑距离小于50 不做操作 小于0是向上滑
	if (view->getContentOffset().getLength() - scrollLen < 50) return;
	//CCLOG("gogo:%f",view->getContentOffset().getLength() - scrollLen);

	if (!m_bRefreshEnable) return;

	m_bRefreshEnable = false;
	m_bLoadData = false;

	CCLOG("SHUA XIN ZhongJiangUpdate");
	//动画
	this->runAction(		
		Sequence::create(
		TargetedAction::create(RefreshSprite,RotateTo::create(0.1f,180)),
		CallFunc::create(this,callfunc_selector(LotteryGameResult::ZhongJiangTime)),
		TargetedAction::create(RefreshSprite,RotateTo::create(0.1,0)),
		DelayTime::create(30.0f),
		CallFunc::create(this,callfunc_selector(LotteryGameResult::ZhongJiangUpdate)),		
		NULL));

}

void LotteryGameResult::refreshLayer()
{
	//获得开奖信息
	m_data->removeAllObjects();
	m_iCount = 0;
	m_needLoad = 0;
	EntityMgr::instance()->getDispatch()->SendPacketQueryGameResult(0);
}

void LotteryGameResult::ZhongJiangUpdate()
{
	m_bRefreshEnable = true;
}
void LotteryGameResult::ZhongJiangTime()
{
	Refresh(NULL);
}

void LotteryGameResult::scrollViewDidZoom(ScrollView* view)
{
		CCLOG("SHUA XIN DidZoom");
}

//void LotteryGameResult::onTouchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent)
//{
//	Touch *touch = (Touch *)pTouches.at(0);
//	Vec2 pos = Director::getInstance()->convertToGL(touch->getLocationInView());
//
//	startPoint = pos;
//}
//
//void LotteryGameResult::onTouchEnded(Touch *pTouch, Event *pEvent)
//{
//	Touch *touch = (Touch *)pTouches.at(0);
//	Vec2 pos = Director::getInstance()->convertToGL(touch->getLocationInView());
//
//	CCLOG("scroll to:%f-->%f",pos.y,startPoint.y - pos.y);
//}

//立即购买
void LotteryGameResult::pressBuy(Object *obj)
{
	playButtonSound();

	int objTag = ((MenuItemSprite *)obj)->getTag();	
	
	int pressedGameKind = objTag;

	if(pressedGameKind == CZChongQingSSC||
		pressedGameKind == CZXinJiangSSC||
		pressedGameKind == CZJiangXiSSC ||
		pressedGameKind == CZ_FENFEN_CAI||
		pressedGameKind == CZ_WUFEN_CAI||
		pressedGameKind == CZ_TIANJIN_SSC||
		pressedGameKind == CZ_KOREA_FEN_CAI||
		pressedGameKind == CZ_JILI_2FEN_CAI||
		pressedGameKind == CZ_CANADA_FEN_CAI||		
		pressedGameKind == CZ_TENCENT_FEN_CAI||
		pressedGameKind == CZ_QQ_FEN_CAI||
		pressedGameKind == CZ_TENCENT_2FC||//腾讯二分彩
		pressedGameKind == CZ_QQ_2FEN_CAI||//QQ二分彩
		pressedGameKind == CZ_BEIJING_5FEN_CAI)

	{
		ChongQingGambleLayer *layer = ChongQingGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZGD11Xuan5||pressedGameKind == CZSD11Xuan5 || pressedGameKind == CZJX11Xuan5 || pressedGameKind ==CZ_HLJ11Xuan5)
	{
		GuangDong11Xuan5Layer *layer = GuangDong11Xuan5Layer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZ_BJPK10||pressedGameKind == CZ_XYFT)
	{
		bjpk10Layer *layer = bjpk10Layer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZ3D || pressedGameKind == CZPailie3)
	{
		FC3DPL3Layer *layer = FC3DPL3Layer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZ_SEVENSTAR)
	{
		LotteryGambleLayer *layer = LotteryGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}else if (pressedGameKind == CZ_LUCKY_28)
	{
		XingYunGambleLayer *layer = XingYunGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZKUAILE8)
	{
		BJK8GambleLayer *layer = BJK8GambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZ_HK6HeCai)
	{
		LiuHeGambleLayer *layer = LiuHeGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t900"), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 4));
		Director::sharedDirector()->getRunningScene()->addChild(layer,255);	
	}
}

//更多
void LotteryGameResult::pressMore(Object* obj)
{
	playButtonSound();

	int objTag = ((MenuItemSprite *)obj)->getTag();
	
	LotteryGameResultDetail *layer = LotteryGameResultDetail::create();
	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

	clear();

	EntityMgr::instance()->getDispatch()->SendPacketQueryMoreGameResult(objTag);
}

void LotteryGameResult::clear()
{
	if(m_data)
	{
		m_data->release();
		m_data = NULL;
	}
	if(gameResultTable)
	{
		gameResultTable->release();
		//gameResultTable = NULL;
	}
	NotificationCenter::getInstance()->removeObserver(this, "QueryGameRet");
}