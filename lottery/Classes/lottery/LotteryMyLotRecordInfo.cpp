#include "LotteryMyLotRecordInfo.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "comm.h"
#include "EntityMgr.h"
#include "LotteryMyLot.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryMyLotRecordInfo::scene()
{
	Scene *scene = Scene::create();
	LotteryMyLotRecordInfo *layer = LotteryMyLotRecordInfo::create();
	scene->addChild(layer);
	return scene;
}

LotteryMyLotRecordInfo::LotteryMyLotRecordInfo()
{
	touZhuNumberTable = NULL;
	for (int i = 0;i < 7;i++)
	{
		zhuangTailabel[i] = NULL;
	}
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryMyLotRecordInfo::getTouZhuXXRet), "GetTouZhuXXRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryMyLotRecordInfo::cancelTouZhuRet), "CancelTouZhuRet", NULL);
}
LotteryMyLotRecordInfo::~LotteryMyLotRecordInfo()
{
	NotificationCenter::getInstance()->removeObserver(this, "GetTouZhuXXRet");
	NotificationCenter::getInstance()->removeObserver(this, "CancelTouZhuRet");
//解决了隐藏很久的BUG，去掉一下release,因为他的存在，可能导致被release2次。BY DAVID,20160302
	//for (int i = 0; i < 7; i++)
	//{
	//	if (zhuangTailabel[i] != NULL)
	//		zhuangTailabel[i]->release();
	//}
	//去掉retain,也去掉release，因为有addchild
		if (touZhuNumberTable != NULL)
			touZhuNumberTable->release();
}

void LotteryMyLotRecordInfo::cancelTouZhuRet(Object *obj)
{
	int iRet = ((Integer *)obj)->getValue();
	if(iRet == 0){
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t587"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer,255);
		cheDanButton->setVisible(false);
	}else{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t588"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer,255);
	}
}

void LotteryMyLotRecordInfo::getTouZhuXXRet(Object *obj)
{
	CCLOG("LotteryMyLotRecordInfo::getTouZhuXXRet");
	float fontSize = 27;
	ccColor3B blackColor = ccc3(150,150,150);
	ccColor3B redColor = ccc3(61,8,40);

#ifdef VER_369
	blackColor = ccc3(50, 62, 123);
#endif

	Array *data = (Array *)obj;
	if(data->count()<1){
		return;
	}
	str_qihao = ((String *)data->objectAtIndex(2))->getCString();//qihao
	int gameType = atoi(((String *)data->objectAtIndex(3))->getCString());	//typeid
	int gameKind = atoi(((String *)data->objectAtIndex(4))->getCString());	//kindid
	int moshi = atoi(((String *)data->objectAtIndex(9))->getCString()); 
	int status = atoi(((String *)data->objectAtIndex(7))->getCString()); 
	int nWinZhuShu = atoi(((String *)data->objectAtIndex(12))->getCString());
	n_gamekindid = gameType;

	int lastNumber = 0;

	for(int i = 0 ; i < 7 ; i++){
		String *labelStr = (String *)data->objectAtIndex(i+1); 
		if(i == 2){  //全部彩种
			labelStr = String::create(ConfigMgr::instance()->text("display_config.xml",String::createWithFormat("t%d", atoi(labelStr->getCString())+20)->getCString()));
		}
		if(i == 3){//玩法名称
			
			labelStr = String::create(BetLimit::GetWanFaNameFromId(gameType, gameKind)); 
		}
		if(i == 6) //中奖开奖撤单
		{
			labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
				String::createWithFormat("t%d", atoi(labelStr->getCString())+579)->getCString()));
		}
		if (i == 5) //
			fontSize = 22;
		else
			fontSize = 27;
		if(i == 4)
		{
			if(gameType == CZ_LUCKY_28)
			{			
				labelStr = String::create(BetLimit::GetXingYunResultStr(labelStr->getCString()));
				lastNumber = BetLimit::GetLastNumberFromStr(labelStr->getCString());
			}else
			if(gameType == CZKUAILE8)
			{				
				labelStr = String::create(BetLimit::GetKuai8ResultStr(labelStr->getCString()));
			}
		}
		zhuangTailabel[i] = LabelTTF::create(labelStr->getCString(),"", fontSize);
		zhuangTailabel[i]->setAnchorPoint(Vec2(0,0.5f));
		zhuangTailabel[i]->setPosition(Vec2(labelStartX1+gapX, labelStartY-gapY*i));
		zhuangTailabel[i]->setColor(blackColor);
		this->addChild(zhuangTailabel[i], 1);
	}
	for(int i = 0 ; i < 10 ; i++)
	{
        if (i == 6) {
            continue;
        }
		String *labelStr = (String *)data->objectAtIndex(8+i); 
		if(i == 1){
			float danZhuJine = 2;
			if(moshi == 1){
				danZhuJine = 0.2;
			}
			else if(moshi == 2){
				danZhuJine = 0.02;
			}
			else if (moshi == 3)
			{	
				danZhuJine = 0.002;
			}
			
			if(gameType == CZ_LUCKY_28 || gameType == CZ_HK6HeCai) danZhuJine = 1;

			labelStr = String::createWithFormat("%.3f%s", danZhuJine, ConfigMgr::instance()->text("display_text.xml",
				String::createWithFormat("t%d", 582+0)->getCString()));
		}
		if(i == 0||i == 4){ //XX注
			labelStr = String::createWithFormat("%s%s",labelStr->getCString(),
				ConfigMgr::instance()->text("display_text.xml", "t586"));	
		}
		if(i == 2){//XX倍
			labelStr = String::createWithFormat("%s%s",labelStr->getCString(),
				ConfigMgr::instance()->text("display_text.xml", "t585"));	
		}
		if(i == 3||i == 5||i == 6||i == 8||i == 9){ //XX元
			if((gameType == CZ_LUCKY_28) && (i == 5) && (nWinZhuShu > 0) && ((lastNumber == 13 && gameKind != xy28_tm13)||(lastNumber == 14 && gameKind != xy28_tm14))) //单注奖金
			{
				labelStr = String::create("1.000");
			}
			labelStr = String::createWithFormat("%s%s",labelStr->getCString(),
				ConfigMgr::instance()->text("display_text.xml", "t582"));	
		}
		if(i == 9)
		{//很据盈亏判定是否中奖
			//float temp =atof(labelStr->getCString());
			if (nWinZhuShu>0)
			{
				String *labelStr = String::create(ConfigMgr::instance()->text("display_DuangDong.xml",
				String::createWithFormat("t%d",9)->getCString()));
				zhuangTailabel[6]->setString(labelStr->getCString());
			}
		}

		LabelTTF *label = LabelTTF::create(labelStr->getCString(),"", fontSize);
        label->setAnchorPoint(Vec2(0,0.5f));
        if (i>6) {
            label->setPosition(Vec2(labelStartX2+gapX, labelStartY-gapY*(i-1)));
        }else{
            label->setPosition(Vec2(labelStartX2+gapX, labelStartY-gapY*i));
        }
		label->setColor(blackColor);
		this->addChild(label, 1);
	}

	//投注号码；应该把大小单双解释一下。
	touZhuHaoMaStr = ((String *)data->objectAtIndex(0))->getCString(); 	
	
	if(gameType == CZChongQingSSC || gameType == CZXinJiangSSC ||gameType == CZ_FENFEN_CAI || gameType == CZ_WUFEN_CAI || gameType == CZ_KOREA_FEN_CAI ||
		gameType == CZ_TENCENT_FEN_CAI || gameType == CZ_QQ_FEN_CAI || gameType == CZ_TENCENT_2FC ||//腾讯二分彩
		gameType == CZ_QQ_2FEN_CAI||//QQ二分彩
		gameType == CZ_TIANJIN_SSC || gameType == CZ_BEIJING_5FEN_CAI||gameType == CZ_CANADA_FEN_CAI || gameType == CZ_JILI_2FEN_CAI || gameType == CZ_BEIJING_5FEN_CAI)
	{
		if(gameKind == DaXiaoDanShuang_DW  || gameKind == Zonghe_DXDSLh || gameKind == DaXiaoDanShuang)
		{
			touZhuHaoMaStr = BetLimit::GetDXDSHStr(touZhuHaoMaStr.c_str());
		}else
			if(gameKind == SSC_LongHu)
			{
				touZhuHaoMaStr = BetLimit::GetLongHuHeStr(touZhuHaoMaStr.c_str());
			}else
				if(gameKind == Ren3Hunhe_Zuxuan || gameKind == Ren2Zuxuan_Fushi || gameKind == Ren2Zuxuan_Fushi || gameKind == Ren3Zu3_Fushi || gameKind == Ren3Zu6_Fushi)
				{
					touZhuHaoMaStr = BetLimit::GetGSBQWStr(touZhuHaoMaStr.c_str());
				}		
	} else
	if(gameType == CZ_LUCKY_28)
	{
		touZhuHaoMaStr = BetLimit::GetXingYunStr(gameKind,touZhuHaoMaStr.c_str());
	} else
	if(gameType == CZKUAILE8)
	{
		if(gameKind == enBJK8_JiOu)
		{
			touZhuHaoMaStr = BetLimit::GetKuai8Str(touZhuHaoMaStr.c_str(), 3);		
		}else
			if(gameKind == enBJK8_ShangXia)
			{
				touZhuHaoMaStr = BetLimit::GetKuai8Str(touZhuHaoMaStr.c_str());
			}else
				if(gameKind == enBJK8_HeZhiDXDS)
				{
					touZhuHaoMaStr = BetLimit::GetKuai8Str(touZhuHaoMaStr.c_str(), -10);
				}
	} else
	if(gameType == CZ_HK6HeCai)
	{
		if(gameKind == LiuHeCai_Tmsx) //生肖特殊可能要发生变化
		{
			touZhuHaoMaStr = BetLimit::GetLiuHeShengXiaoStr(touZhuHaoMaStr.c_str());
		}else
		if(gameKind == LiuHeCai_Tmds)  //单双
		{
			touZhuHaoMaStr = BetLimit::GetLiuHeCaiStr(touZhuHaoMaStr.c_str());
		}else
		if(gameKind == LiuHeCai_Tmbs)	//波色 红波蓝波绿波262728
		{
			touZhuHaoMaStr = BetLimit::GetLiuHeCaiStr(touZhuHaoMaStr.c_str(), 25); 
		}
	}

	if(touZhuHaoMaStr.size() > 10000) touZhuHaoMaStr = touZhuHaoMaStr.substr(0,10000).append("......");//屏蔽后面的，显示不出来
	handleTouZhuHaoMa(touZhuHaoMaStr);

	if (touZhuNumberTable)
		touZhuNumberTable->reloadData();

	if(status != 0){
		cheDanButton->setVisible(false);
	}
	else
		cheDanButton->setVisible(true);
}

void LotteryMyLotRecordInfo::handleTouZhuHaoMa(string &originStr)
{
	CCLOG("LotteryMyLotRecordInfo::handleTouZhuHaoMa");
	int num = 0;
	int i = 0;
	do{
		if((i+huanHandNum)>=originStr.length())
		{
			break;
		}			
		
		if(originStr.at(i + huanHandNum) < '0' || originStr.at(i + huanHandNum) > '9')
		{
			i ++;
			continue;
		}

		i+=huanHandNum;

		originStr.insert(i, "\n");
	}while(true);
}

bool LotteryMyLotRecordInfo::init()
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

void LotteryMyLotRecordInfo::initData()
{
	winSize = Director::getInstance()->getWinSize();
	touZhuHaoMaStr="";
	huanHandNum = 46;
}

void LotteryMyLotRecordInfo::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//背景图头部
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
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryMyLotRecordInfo::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	//投注信息
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t560"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	//投注内容
	fontSize = 27;
	LabelTTF *contentLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t561"), "", fontSize);
	contentLabel->setAnchorPoint(Vec2(0,0.5f));
	contentLabel->setPosition(Vec2(10, SCREEN_HEIGHT-162-40));
	contentLabel->setColor(blackColor);
	this->addChild(contentLabel, 1);

	contentBk = Sprite::createWithSpriteFrame(spriteFrame("beting03.png"));
	contentBk->setAnchorPoint(Vec2(0.5f,1));
	contentBk->setPosition(Vec2(SCREEN_WIDTH/2, contentLabel->getPositionY() - 40));
	contentBk->setScaleX(SCREEN_WIDTH / contentBk->getContentSize().width);
	contentBk->setScaleY(0.5f);
	this->addChild(contentBk, 1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, contentBk->getPositionY()));
	this->addChild(gap1,1);

	//撤单的处理 撤单按钮
	Sprite *cheDanNormalButton = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_16.png"));
	Sprite *cheDanSelectButton = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_17.png"));
	MenuItemSprite *cheDanItemSprite = MenuItemSprite::create
		(cheDanNormalButton,cheDanSelectButton,CC_CALLBACK_1(LotteryMyLotRecordInfo::cheDan,this));
	cheDanButton = Menu::create(cheDanItemSprite,NULL);
	cheDanButton->setPosition(Vec2(SCREEN_WIDTH - contentLabel->getPositionX() - cheDanNormalButton->getContentSize().width / 2,
		contentLabel->getPositionY()));
	cheDanButton->setVisible(false);
	this->addChild(cheDanButton,1);

	//订单号等等其它信息
	labelStartY = contentBk->getPositionY() - contentBk->getContentSize().height/2 - 30;
	gapX = 120;		//KEY-VALUE的X轴间距
	gapY = 60;
	labelStartX1 = 10;
	labelStartX2 = winSize.width / 2;

	for(int i = 0 ; i < 7 ; i++){
		String *labelStr = String::createWithFormat("t%d", 562+i); 
		LabelTTF *label = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", 
			labelStr->getCString()),"", fontSize);
		label->setAnchorPoint(Vec2(0,0.5f));
		label->setPosition(Vec2(labelStartX1, labelStartY-gapY*i));
		label->setColor(blackColor);
		this->addChild(label, 1);
	}

	for(int i = 0 ; i < 10 ; i++){
        if(i == 6){
            continue;
        }
		String *labelStr = String::createWithFormat("t%d", 569+i); 
		LabelTTF *label = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", 
			labelStr->getCString()),"", fontSize);
		label->setAnchorPoint(Vec2(0,0.5f));
        if (i>6) {
            label->setPosition(Vec2(labelStartX2, labelStartY-gapY*(i-1)));
        }else{
            label->setPosition(Vec2(labelStartX2, labelStartY-gapY*i));
        }
		label->setColor(blackColor);
		this->addChild(label, 1);
	}

	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = 200;
	touZhuNumberTable = TableView::create(this, tableViewSize);
	touZhuNumberTable->setDelegate(this);
	touZhuNumberTable->setAnchorPoint(Vec2(0.5, 0.5));
	touZhuNumberTable->setPosition(Vec2(0, SCREEN_HEIGHT - 450));
	touZhuNumberTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	touZhuNumberTable->setDirection(ScrollView::Direction::VERTICAL);
	touZhuNumberTable->retain();		//有addchild 无retain
	touZhuNumberTable -> setSwallowsTouches(true);
	this->addChild(touZhuNumberTable, 1);
	
	CCLOG("LotteryMyLotRecordInfo::initView");
}


void LotteryMyLotRecordInfo::cheDan(Object *obj)
{
	playButtonSound();

	EntityMgr::instance()->getDispatch()->SendPacketWithCancelTouZhu(n_t_id, n_gamekindid, str_qihao);
}


void LotteryMyLotRecordInfo::back(Object *obj)
{
	playButtonSound();
	Director::getInstance()->popScene();

	//Scene *scene = Scene::create();
	//LotteryMyLot *layer = LotteryMyLot::create();
	//layer->ShowButtonRecord();
	//scene->addChild(layer,3);
	//Director::getInstance()->replaceScene(scene);
	NotificationCenter::getInstance()->postNotification("RemoveRecordInfoLayer");

}

void LotteryMyLotRecordInfo::onEnter()
{
	BaseLayer::onEnter();
}

void LotteryMyLotRecordInfo::onExit()
{
	BaseLayer::onExit();
}

void LotteryMyLotRecordInfo::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

cocos2d::Size LotteryMyLotRecordInfo::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = 200;
	int len = touZhuHaoMaStr.length();
	if(len>=huanHandNum*4){
		size.height = (len/huanHandNum+1)*40;
	}
	return size;
}

ssize_t LotteryMyLotRecordInfo::numberOfCellsInTableView(TableView *table)
{
	return 1;
}
TableViewCell *LotteryMyLotRecordInfo::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 


	cocos2d::Size size;
	size.width = winSize.width;
	size.height = 200;
	int len = touZhuHaoMaStr.length();
	if(len>=huanHandNum*4){
		size.height = (len/huanHandNum+1)*40;
	}
	float fontSize = 27;

	ccColor3B blackColor = ccc3(150,150,150);	
	LabelTTF *touZhuHaoMa = LabelTTF::create(touZhuHaoMaStr.c_str(),"", fontSize);	
	touZhuHaoMa->setAnchorPoint(Vec2(0,1));
	touZhuHaoMa->setPosition(Vec2(15, size.height));
	touZhuHaoMa->setColor(blackColor);
	touZhuHaoMa->setHorizontalAlignment(kCCTextAlignmentLeft);
	cell->addChild(touZhuHaoMa, 1);

	return cell;  
}

void LotteryMyLotRecordInfo::tableCellTouched(TableView *table, TableViewCell *cell)
{

}

void LotteryMyLotRecordInfo::scrollViewDidScroll(ScrollView* view)
{

}
void LotteryMyLotRecordInfo::scrollViewDidZoom(ScrollView* view)
{

}