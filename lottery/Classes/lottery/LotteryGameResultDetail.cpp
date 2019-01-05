#include "LotteryGameResultDetail.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "LotterySceneControl.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;
using namespace extension;

Scene* LotteryGameResultDetail::scene()
{
	Scene *scene = Scene::create();
	LotteryGameResultDetail *layer = LotteryGameResultDetail::create();
	scene->addChild(layer);
	return scene;
}

LotteryGameResultDetail::LotteryGameResultDetail()
{
	data = Array::create();
	data->retain();
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryGameResultDetail::queryGameRet), "QueryMoreGameRet", NULL);
}
LotteryGameResultDetail::~LotteryGameResultDetail()
{
	if(data != nullptr)
	{
		data->removeAllObjects();
		data->release();
		data = nullptr;
	}
	
	gameResultTable->release();
	NotificationCenter::getInstance()->removeObserver(this, "QueryMoreGameRet");
}

void LotteryGameResultDetail::queryGameRet(Object *obj)
{
	Array *receivedData = (Array *)obj;
	data->addObjectsFromArray(receivedData);
	gameResultTable->reloadData();
}

bool LotteryGameResultDetail::init()
{
	if (!Layer::init())
	{
		return false;
	}
	winSize = Director::getInstance()->getWinSize();
	cellHeight = 120;
	this->initView();
	//	EntityMgr::instance()->getDispatch()->SendPacketQueryMoreGameResult(1);
	this->setKeypadEnabled(true);
	return true;
}

void LotteryGameResultDetail::initView()
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

	//返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryGameResultDetail::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	//pCloseButton->setPosition(Vec2(32, SCREEN_HEIGHT - 120));
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	//当前页面名称
	float fontSize = 38;
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t901"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	//列表
	cocos2d::Size tableViewSize;
	tableViewSize.width = SCREEN_WIDTH;
	tableViewSize.height = SCREEN_HEIGHT - 162;
	gameResultTable = TableView::create(this, tableViewSize);
	gameResultTable->setDelegate(this);
	gameResultTable->setAnchorPoint(Vec2(0.5, 0.5));
	gameResultTable->setPosition(Vec2(0, 0));
	gameResultTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	gameResultTable->setDirection(ScrollView::Direction::VERTICAL);
	gameResultTable->retain();
	this->addChild(gameResultTable, 1);
}

void LotteryGameResultDetail::onEnter()
{
	Layer::onEnter();
}

void LotteryGameResultDetail::onExit()
{
	Layer::onExit();
}

void LotteryGameResultDetail::back(Object *obj)
{
	playButtonSound();
	Director::getInstance()->popScene();
}

void LotteryGameResultDetail::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

cocos2d::Size LotteryGameResultDetail::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = 60;//cellHeight;
	if (data->count() > 0)
	{
		String *num = (String *)data->objectAtIndex(2);
		string temp = num->getCString();
		if(temp.length() > 16) size.height = 100;
	}
	return size;
}

ssize_t LotteryGameResultDetail::numberOfCellsInTableView(TableView *table)
{
	return data->count()/4+1;
}
TableViewCell *LotteryGameResultDetail::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 

	if(index == 1){
		return cell;
	}

	int cellHeight = 60;//屏蔽掉全局

	ccColor3B blackColor = ccc3(150,150,150);
	float fontSize = 30;
	if(data->count() == 0){
		fontSize = 30;
		//暂无开奖信息
		LabelTTF *gameTypeLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t589"),"",fontSize);
		gameTypeLabel->setColor(blackColor);
		gameTypeLabel->setAnchorPoint(Vec2(0.5f,0.5f));
		gameTypeLabel->setPosition(Vec2(SCREEN_WIDTH/2, cellHeight / 2.0f));
		cell->addChild(gameTypeLabel, 1);
		return cell;
	}

	Integer *kind = (Integer *) data->objectAtIndex(index);
	static bool isKuai8 = false;
	static bool isXingYun28 = false;

	static int tempInt = 0;	
	int step = 1;
	string result = "";
	ccColor3B redColor = ccc3(100,100,100);
	float textWidth = SCREEN_WIDTH - 200;	

	if(index == 0)
	{
		Integer *kindID = (Integer *)data->objectAtIndex(index);
		isKuai8 = kindID->getValue() == 10;
		isXingYun28 = kindID->getValue() == 17;
		result += ConfigMgr::instance()->text("display_config.xml", String::createWithFormat("t%d", 20 + kindID->getValue())->getCString());
		//彩票类别
		LabelTTF *descibeLabel = LabelTTF::create(result.c_str(), "", fontSize, cocos2d::Size(textWidth, 0), kCCTextAlignmentLeft);
		descibeLabel->setColor(redColor);
		descibeLabel->setAnchorPoint(Vec2(0,0.5f));
		descibeLabel->setPosition(Vec2(20, cellHeight/2));
		cell->addChild(descibeLabel, 1);

#ifdef VER_QIANBO
		descibeLabel->setColor(ccc3(68, 185, 110));
#endif

		String *wPeriodFirst = (String *)data->objectAtIndex(index+1);
		String *szLotNumFirst = (String *)data->objectAtIndex(index+2);
		result=ConfigMgr::instance()->text("display_text.xml", "t454");
		result.append(wPeriodFirst->getCString());
		result.append(ConfigMgr::instance()->text("display_text.xml", "t455"));

		redColor = ccc3(61, 8, 40);
		LabelTTF *descibeLabel1First = LabelTTF::create(result.c_str(), "", fontSize, cocos2d::Size(textWidth, 0), kCCTextAlignmentRight);
		descibeLabel1First->setColor(redColor);
		descibeLabel1First->setAnchorPoint(Vec2(1,0.5f));
		descibeLabel1First->setPosition(Vec2(460, cellHeight/2));
		cell->addChild(descibeLabel1First, 1);

		result=" ";
		result=szLotNumFirst->getCString();
		int len = (int)result.length();	

		int nCurrentKindID = kindID->getValue();
		//if ((nCurrentKindID >= 6 && nCurrentKindID <= 10) || nCurrentKindID == 13 || nCurrentKindID == 14) 
		//{
		//	step = 2;
		//}	
		if (len>=10) { //两位数
			step = 2;

		}
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
		if (len <= 16)
		{
			ballSprite->setPosition(Vec2(40 +20 + ballGap*(i % 20 ) / step, 0 - nYpos - cellHeight / 2));
			if (bTeMa)
				ballSprite->setColor(ccc3(200,200,200));
			cell->addChild(ballSprite, 1);
			tempInt = 0;
		} else
		{
			tempInt = 80;
		}
	}
		if (len > 16)
		{
			//添加球号码
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
			LabelTTF *ballLabel = LabelTTF::create(result.c_str(), "", fontSize, cocos2d::Size(SCREEN_WIDTH - 60, 0), TextHAlignment::LEFT);
			ballLabel->setColor(redColor);			
			ballLabel->setPosition(Vec2(SCREEN_WIDTH / 2, 0 - cellHeight / 2));
			cell->addChild(ballLabel, 1);
		}
		return cell;
	}		

	if (data->count()<index*4+2) {
		return cell;
	}
	String *wPeriod = (String *)data->objectAtIndex((index-1)*4+1);
	String *szLotNum = (String *)data->objectAtIndex((index-1)*4+2);
	result=ConfigMgr::instance()->text("display_text.xml", "t454");
	result.append(wPeriod->getCString());
	result.append(ConfigMgr::instance()->text("display_text.xml", "t455"));

	//第N期
	redColor = ccc3(61,8,40);
	LabelTTF *descibeLabel1 = LabelTTF::create(result.c_str(), "", fontSize);
	descibeLabel1->setColor(redColor);
	descibeLabel1->setAnchorPoint(Vec2(0,0.5f));
	descibeLabel1->setPosition(Vec2(20, cellHeight/2));
	cell->addChild(descibeLabel1, 1);

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
	LabelTTF *descibeLabel2 = LabelTTF::create(result.c_str(), "", fontSize, cocos2d::Size(textWidth, 0), kCCTextAlignmentLeft);
	descibeLabel2->setColor(redColor);
	descibeLabel2->setAnchorPoint(Vec2(0,0.5f));
	descibeLabel2->setPosition(Vec2(descibeLabel1->getContentSize().width + 20, cellHeight/2));
	cell->addChild(descibeLabel2, 1);

#ifdef VER_QIANBO
	descibeLabel1->setColor(ccc3(76,76,76));
	descibeLabel2->setColor(ccc3(68, 185, 110));
#endif

#ifdef VER_369
	descibeLabel1->setColor(ccc3(76,76,76));
	descibeLabel2->setColor(ccc3(50, 62, 123));
#endif

	return cell;  
}

void LotteryGameResultDetail::transferString(string &str, int nStep)
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

void LotteryGameResultDetail::tableCellTouched(TableView *table, TableViewCell *cell)
{

}

void LotteryGameResultDetail::scrollViewDidScroll(ScrollView* view)
{

}
void LotteryGameResultDetail::scrollViewDidZoom(ScrollView* view)
{

}