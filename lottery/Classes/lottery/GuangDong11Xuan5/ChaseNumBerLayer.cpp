#include "ChaseNumBerLayer.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "LotterySettingView.h"
#include "DBHandler.h"
#include "time.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* ChaseNumBerLayer::scene()
{
	Scene *scene = Scene::create();
	ChaseNumBerLayer *layer = ChaseNumBerLayer::create();
	scene->addChild(layer);
	return scene;
}

ChaseNumBerLayer::ChaseNumBerLayer()
{
	fandianStatus = 0;
	ZhuiHaoBool =0;
	BeiTouBool = 0; 
	nMoshiNumber=1;
	BaoCunQiShu = 5;
	nMoshi=0;
	ZhuiHao = 4;
	QuanJunBeiShu = 1;
	TouZhuJinENumber =0.00;
	memset(chaseStatus, 0, sizeof(chaseStatus));
	zhuiHaoQiShu = 5;
	cellHeight = 20; 
	betZhuiHaoZhuShu =1;
	tempItemInpu = NULL;

	fandianTable = nullptr;
	touZhuTable = nullptr;

	for(int index = 0 ; index < zhuiHaoQiShu ; index++)
	{		
		chaseStatus[index]=1;
		chaseInputStatus[index] = 1;
		chaseInputMonreny[index] = nMoshiNumber*betZhuiHaoZhuShu;
		TouZhuJinENumber += chaseInputMonreny[index];
	}
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ChaseNumBerLayer::TouZhuZhuiHaoButton), "TouZhuChaseNumBer", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ChaseNumBerLayer::changeKind), "GDselectedItemTagNumberCartFailed", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ChaseNumBerLayer::touZhuContinueRet), "CQSSCRetZhuiHao", NULL);

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ChaseNumBerLayer::IsFendanHourRet), "IsFendanHourRet", NULL);
}
ChaseNumBerLayer::~ChaseNumBerLayer()
{
	if(fandianTable != nullptr)
	{
		fandianTable->release();
	}

	if(touZhuTable != nullptr)
	{
		touZhuTable->release();
	}
	NotificationCenter::getInstance()->removeObserver(this, "TouZhuChaseNumBer");
	NotificationCenter::getInstance()->removeObserver(this, "GDselectedItemTagNumberCartFailed");
	NotificationCenter::getInstance()->removeObserver(this, "CQSSCRetZhuiHao");
	NotificationCenter::getInstance()->removeObserver(this, "IsFendanHourRet");
}
bool ChaseNumBerLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	ZhuiHaoQiShuBool = false;
	this->initData();
	this->initView();
	setTouchEnabled(true);
	this->setKeypadEnabled(true);
	return true;
}

void ChaseNumBerLayer::initData()
{
	winSize = Director::getInstance()->getWinSize();
	gameKyTe = NULL;
}

void ChaseNumBerLayer::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->onTouchBegan = [=](Touch*, Event*){	
		if(tempItemInpu != NULL)
		{
			editBoxEditingDidEnd(tempItemInpu);
			tempItemInpu = NULL;
		}

		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layer);

	//背景图
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);
	float fontSize = 38;

	//返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(ChaseNumBerLayer::back,this));
	pCloseButton = Menu::create(pCloseItemSprite,NULL);
	//pCloseButton->setPosition(Vec2(32, SCREEN_HEIGHT - 120));
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);
	float menuGap = 10;

	//追号字样
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t175"), "", fontSize);	
	title->setPosition(Vec2(SCREEN_WIDTH / 2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255, 255, 255));
	this->addChild(title, 1);

	fontSize = 28;
	static int fontSize2 = 20;	
	static int upMove = 20;

	char szBuf2[100] = {0};  
	sprintf(szBuf2, "%.3f ",0.00);
	String *TouZhuYuEString = String::createWithFormat("%s %s%s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t3"),szBuf2,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
	
	char szBuf[100] = {0};  
	sprintf(szBuf, "%.3f ",TouZhuJinENumber);
	String *TouZhuJinECherString = String::createWithFormat("%s %s %s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t1"),szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));

	//投注金额
	TouZhuJinEChaseNumBerLabel = LabelTTF::create(TouZhuJinECherString->getCString(), "", fontSize);
	TouZhuJinEChaseNumBerLabel->setPosition(Vec2(winSize.width*0.03,140 + upMove));//
	TouZhuJinEChaseNumBerLabel->setAnchorPoint(Vec2(0, 0.5f));
	TouZhuJinEChaseNumBerLabel->setColor(ccc3(0,0,0));
	this->addChild(TouZhuJinEChaseNumBerLabel, 2);
	char szBuf3[100] = {0};  
	sprintf(szBuf3, "%.3f ", EntityMgr::instance()->getDispatch()->f_qipai_yue);
	String *GameYuEString = String::createWithFormat("%s %s%s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t6"),szBuf3,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));

	//追号期数
	haoMaLabel = LabelTTF::create(String::createWithFormat("%s", ConfigMgr::instance()->text("display_DuangDong.xml", "t171"))->getCString(), "", fontSize);
	haoMaLabel->setColor(ccc3(0,0,0));
	haoMaLabel->setPosition(Vec2(winSize.width*0.1,winSize.height*0.83));
	this->addChild(haoMaLabel, 1);
	//外框 combobox
	Sprite *beilvBk = Sprite::createWithSpriteFrame(spriteFrame("home_11.png"));
	beilvBk->setAnchorPoint(Vec2(0,0.5f));
	beilvBk->setPosition(Vec2(winSize.width*0.2+1,winSize.height*0.83));	
	this->addChild(beilvBk,2);

	//下拉
	Sprite *ButtonCodedImage = Sprite::createWithSpriteFrame(spriteFrame("beting17.png"));
	Sprite *ButtonCodeNormalImage = Sprite::createWithSpriteFrame(spriteFrame("beting18.png"));
	ButtonCodeRecordItem = MenuItemSprite::create(ButtonCodedImage,ButtonCodeNormalImage,CC_CALLBACK_1(ChaseNumBerLayer::XuanZeZhuiHaoQiShu,this));
	Menu *ButtonCodeMenu = Menu::create(ButtonCodeRecordItem,NULL);
	ButtonCodeMenu->setPosition(Vec2(winSize.width*0.45-22,winSize.height*0.83));	//向左移动20个像素
	this->addChild(ButtonCodeMenu,2);

	beilvBk->setScaleX((ButtonCodeMenu->getPositionX()-beilvBk->getPositionX() + ButtonCodedImage->getContentSize().width / 2) / beilvBk->getContentSize().width); //框缩短1/4
	
	
	//追号输入框
	beilvInput = EditBox::create(cocos2d::Size(120,32), "blank.png");
	float beilvInputMidX = winSize.width*0.2;
	beilvInput->setPosition(Vec2(winSize.width*0.2, beilvBk->getPositionY()));
	beilvInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	beilvInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	beilvInput->setInputMode(ui::EditBox::InputMode::NUMERIC);
	beilvInput->setAnchorPoint(Vec2(0,0.5f));
	beilvInput->setFontColor(ccc3(0,0,0));
	beilvInput->setFontName("");beilvInput->setFontSize(fontSize);
	beilvInput->setDelegate(this);
	beilvInput->setText("5");
	beilvInput->setColor(ccc3(112,112,112));
	beilvInput->setMaxLength(20);
	this->addChild(beilvInput, 2);
	//追号倍数
	timeLabel = LabelTTF::create(String::createWithFormat("%s", ConfigMgr::instance()->text("display_DuangDong.xml", "t172"))->getCString(), "", fontSize);
	timeLabel->setColor(ccc3(0,0,0));
	timeLabel->setPosition(Vec2(winSize.width*0.7 + 20,haoMaLabel->getPositionY()));//右移20
	this->addChild(timeLabel, 1);
	//追号输入 外框
	Sprite *beilvBk2 = Sprite::createWithSpriteFrame(spriteFrame("home_11_2.png"));
	beilvBk2->setAnchorPoint(Vec2(0,0.5f));
	beilvBk2->setScaleX(0.8f);
	beilvBk2->setPosition(Vec2(winSize.width*0.8 + 20,beilvBk->getPositionY())); //右移20	
	this->addChild(beilvBk2,2);
	CtratebeilvInput = EditBox::create(cocos2d::Size(80,32), "blank.png");
	float CtratebeilvInputMidX = beilvBk2->getPositionX();
	CtratebeilvInput->setPosition(Vec2(CtratebeilvInputMidX, beilvBk->getPositionY()));
	CtratebeilvInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	CtratebeilvInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	CtratebeilvInput->setAnchorPoint(Vec2(0,0.5f));
	CtratebeilvInput->setFontColor(ccc3(0,0,0));
	CtratebeilvInput->setFontName("");CtratebeilvInput->setFontSize(fontSize);
	CtratebeilvInput->setColor(ccc3(112,112,112));
	CtratebeilvInput->setDelegate(this);
	CtratebeilvInput->setText("1");
	CtratebeilvInput->setInputMode(ui::EditBox::InputMode::NUMERIC);
	CtratebeilvInput->setMaxLength(20);
	this->addChild(CtratebeilvInput, 2);
	/*********************************/
    m_TableNode = Node::create();
    m_TableNode->setPosition(1000,1000);
    this->addChild(m_TableNode, 3);

	//白色背景 带边框
	tableCellWidth = 185;//216 //减少30
    fandianTableBk = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
    fandianTableBk->setAnchorPoint(Vec2(0,0));
    fandianTableBk->setPosition(Vec2::ZERO);
	fandianTableBk->setScaleX(tableCellWidth / fandianTableBk->getContentSize().width);
    m_TableNode->addChild(fandianTableBk,3);

    tableViewSize.width = tableCellWidth;
    tableViewSize.height = fandianTableBk->getContentSize().height;
    
	//追号列表
	fandianTable = TableView::create(this, tableViewSize);
    fandianTable->setDelegate(this);
    fandianTable->setAnchorPoint(Vec2(0.5, 0.5));
    fandianTable->setPosition(Vec2::ZERO);
    fandianTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    fandianTable->setDirection(ScrollView::Direction::VERTICAL);
    fandianTable->retain();
	fandianTable->setTag(1111);
    m_TableNode->addChild(fandianTable, 3);

	//是否倍投
	LabelTTF*	BeiNumberLabel = LabelTTF::create(String::createWithFormat("%s",ConfigMgr::instance()->text("display_DuangDong.xml","t173"))->getCString(), "",fontSize);
	BeiNumberLabel->setColor(ccc3(0,0,0));
	BeiNumberLabel->setPosition(Vec2(winSize.width*0.1,winSize.height*0.75 + upMove));
	this->addChild(BeiNumberLabel, 2);
	//倍投开关
	Sprite *winningRecordSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("beting16.png"));
	Sprite *contiRecordNormalImage = Sprite::createWithSpriteFrame(spriteFrame("beting16.png"));
	betRecordItem = MenuItemSprite::create(winningRecordSelectedImage,contiRecordNormalImage,CC_CALLBACK_1(ChaseNumBerLayer::IsEndeBeiTou,this));
	recordMenu = Menu::create(betRecordItem,NULL);
	recordMenu->setPosition(Vec2(winSize.width*0.3 - 20,winSize.height*0.75 + upMove));	
	this->addChild(recordMenu,2);
	//中奖后停止追号
	LabelTTF*	BeiNumberLabe2 = LabelTTF::create(String::createWithFormat("%s",ConfigMgr::instance()->text("display_DuangDong.xml","t174"))->getCString(), "",fontSize);
	BeiNumberLabe2->setColor(ccc3(0,0,0));
	BeiNumberLabe2->setPosition(Vec2(winSize.width*0.62 + 35,winSize.height*0.75 + upMove));//右移40
	this->addChild(BeiNumberLabe2,2);
	//追号开关
	Sprite *betRecordNormalImage = Sprite::createWithSpriteFrame(spriteFrame("beting15.png"));
	Sprite *betRecordSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("beting15.png"));
	GetRecordItem = MenuItemSprite::create(betRecordNormalImage,betRecordSelectedImage,CC_CALLBACK_1(ChaseNumBerLayer::IsEndeZhuiHao,this));
	recordMenu2 = Menu::create(GetRecordItem,NULL);
	recordMenu2->setPosition(Vec2(winSize.width*0.9,winSize.height*0.75 + upMove));
	this->addChild(recordMenu2,2);

	//表格头
	Sprite *gapTagSprite = Sprite::createWithSpriteFrame(spriteFrame("beting19.png"));
	gapTagSprite->setAnchorPoint(Vec2(0.5f,0.5f));
	//gapTagSprite->setScaleX(0.98f);
	gapTagSprite->setPosition(Vec2(SCREEN_WIDTH / 2,winSize.height*0.65f + 40 + upMove));//上移40
	this->addChild(gapTagSprite,1);

	//线
	//Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	//gap1->setAnchorPoint(Vec2(0, 1));
	//gap1->setPosition(Vec2(0, gapTagSprite->getPositionY() - gapTagSprite->getContentSize().height - 15));
	//this->addChild(gap1, 1);

	//增加外框
	Scale9Sprite *sptTableBack = Scale9Sprite::createWithSpriteFrame(spriteFrame("beting03.png"));
	sptTableBack->setAnchorPoint(Vec2(0.5f, 0.5f));
	sptTableBack->setPosition(Vec2(SCREEN_WIDTH / 2, (gapTagSprite->getPositionY() + 120 + gapTagSprite->getContentSize().height) / 2)); //设置位置
	sptTableBack->setScaleX((gapTagSprite->getContentSize().width + 20) / sptTableBack->getContentSize().width);
	sptTableBack->setScaleY((gapTagSprite->getPositionY() - 120 + 20) / sptTableBack->getContentSize().height);
	this->addChild(sptTableBack, 0);

	//追号表格
	cocos2d::Size tableViewSize;
	tableViewSize.width = gapTagSprite->getContentSize().width + 10;
	tableViewSize.height = gapTagSprite->getPositionY() - 50 - 120 - 4 - gapTagSprite->getContentSize().height;//tableViewSize.height = winSize.height*0.65 - 170;;
	touZhuTable = TableView::create(this, tableViewSize);  //设置可显区域
	touZhuTable->setDelegate(this);
	touZhuTable->setAnchorPoint(Vec2(0, 0));
	touZhuTable->setPosition(Vec2(12, 120 + upMove + 50)); //设置位置
	//touZhuTable->setPosition(sptTableBack->getPosition());//校正位置
	touZhuTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	touZhuTable->setDirection(ScrollView::Direction::VERTICAL);
	touZhuTable->retain();
	this->addChild(touZhuTable, 1);

	//touZhuTable->setVisible(false);

#ifndef VER_369
	//底部背景
	Sprite *bottomBk = Sprite::createWithSpriteFrame(spriteFrame("room_12_2.png"));
	bottomBk->setPosition(Vec2(SCREEN_WIDTH/2, 60));
	bottomBk->setScaleX(SCREEN_WIDTH / bottomBk->getContentSize().width);
	bottomBk->setScaleY(120/bottomBk->getContentSize().height);
	//bottomBk->setColor(ccc3(50,50,50));
	this->addChild(bottomBk,2);
#endif

	//确定 取消
	Sprite *StramNormalImage_1 = Sprite::createWithSpriteFrame(spriteFrame("beting22.png"));
	Sprite *StramNormalImage_2 = Sprite::createWithSpriteFrame(spriteFrame("beting23.png"));
	Sprite *CloseNormalImage_1 = Sprite::createWithSpriteFrame(spriteFrame("beting24.png"));
	Sprite *CloseNormalImage_2 = Sprite::createWithSpriteFrame(spriteFrame("beting25.png"));
	StramItem = MenuItemSprite::create(StramNormalImage_1,StramNormalImage_2,CC_CALLBACK_1(ChaseNumBerLayer::betRecord,this));
	CloseItem = MenuItemSprite::create(CloseNormalImage_1,CloseNormalImage_2,CC_CALLBACK_1(ChaseNumBerLayer::ZhuiHaoCloseButton,this));
	StramMenu2 = Menu::create(CloseItem,StramItem,NULL);
	StramMenu2->setPosition(Vec2(winSize.width*0.5,60));
	StramMenu2->alignItemsHorizontallyWithPadding(150);
	this->addChild(StramMenu2,2);
}

//top menu
void ChaseNumBerLayer::setting(Object* obj)
{
	playButtonSound();

	LotterySettingView *layer = LotterySettingView::create();

	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}

void ChaseNumBerLayer::back(Object *obj)
{
	playButtonSound();
	this->setPositionY(2000);
	NotificationCenter::getInstance()->postNotification("ChaseNumBerItem");
}
//transaction menu
void ChaseNumBerLayer::recharge(Object* obj)
{

}

void ChaseNumBerLayer::withdraw(Object *obj)
{

}
void ChaseNumBerLayer::ZhuiHaoCloseButton(Object *obj)
{
	this->setPositionY(2000);
	NotificationCenter::getInstance()->postNotification("ChaseNumBerItem");
}

//record menu
void ChaseNumBerLayer::changeKind(Object *obj)
{
	String *GDselectQiShuString = (String *)obj;
	GDselectedItemTagNumber = atoi(GDselectQiShuString->getCString());
	CCLOG("%d",GDselectedItemTagNumber);
//	touZhuTable->reloadData();
}
void ChaseNumBerLayer::ShowZhuiHaoMen()
{
	pCloseButton->setEnabled(true);
	CtratebeilvInput->setEnabled(true);
	recordMenu->setEnabled(true);
	recordMenu2->setEnabled(true);
	StramMenu2->setEnabled(true);
	//tempItemInpu->setEnabled(true);
	middleMenu->setEnabled(true);
}
void ChaseNumBerLayer::dissZhuiHaoMen()
{
	pCloseButton->setEnabled(false);
	CtratebeilvInput->setEnabled(false);
	recordMenu->setEnabled(false);
	recordMenu2->setEnabled(false);
	StramMenu2->setEnabled(false);
	//tempItemInpu->setEnabled(false);
	middleMenu->setEnabled(false);
}

void ChaseNumBerLayer::TouZhuZhuiHaoButton(Object *obj)
{
	if(MyBetNumber::getInstance()->size() == 0) return;

	nMoshiNumber = 0.0f;
	for(int i = 0;i < MyBetNumber::getInstance()->size();i ++)
		nMoshiNumber += MyBetNumber::getInstance()->getItem(i).getBetMoney();

	float ZhangHuYuENumber = EntityMgr::instance()->getDispatch()->f_yue;	

	char szBuf2[100] = {0};  
	sprintf(szBuf2, "%.3f ",ZhangHuYuENumber);
	String *TouZhuYuEString = String::createWithFormat("%s %s%s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t3"),szBuf2,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));	

	string nMoshiString = CtratebeilvInput->getText();
	int nBeiShu = atoi(nMoshiString.c_str());

	//总共金额
	float temp=0;
	for(int index = 0 ; index < zhuiHaoQiShu ; index++)
	{
		if( chaseStatus[index]==1){
			if(BeiTouBool ==0){
				chaseInputStatus[index] = nBeiShu;
			}else{
				chaseInputStatus[index+1]= chaseInputStatus[index]*2;
				if(chaseInputStatus[index+1]>MAX_ZHUIHAO_BEISHU)
				{
					chaseInputStatus[index+1] = MAX_ZHUIHAO_BEISHU;
				}}
			chaseStatus[index]=1;		

			chaseInputMonreny[index] = nMoshiNumber*chaseInputStatus[index];
			temp += chaseInputMonreny[index];
		}}
	char szBuf[100] = {0};  
	sprintf(szBuf, "%.3f ",temp);
	String *TouZhuJinECherString = String::createWithFormat("%s %s %s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t1"),szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
	TouZhuJinEChaseNumBerLabel->setString(TouZhuJinECherString->getCString());
	touZhuTable->reloadData();
}
void ChaseNumBerLayer::betRecord(Object *obj)
{
	playButtonSound();

	if (MyBetNumber::getInstance()->size() == 0) return;

	
	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if(!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t167"),
		Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else{
		//判断是否在封单
		NotificationCenter::getInstance()->postNotification("IsFendanHour");
	}
		
}

void ChaseNumBerLayer::IsFendanHourRet(Object *obj)
{
	Bool* isFendan = (Bool *)obj;
	//封单		
	if(isFendan->getValue())
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t8"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);		
		return;
	}

	float TouZhuJinE =0;
	char qiShu[MAX_QIHAO][20] = {0};
	int beiShu[MAX_QIHAO]={0};
	int totalQiShu = 0;
	string nMoshiString = CtratebeilvInput->getText();
	int nbeiShu = atoi(nMoshiString.c_str());

	if(ZhuiHao != 2) 
	{
		MyBetNumber::addRandSeed();
		ZhuiHao = rand() % 6000 + 4;
	}

	MyBetNumber::getInstance()->betReady();
	//追号投注开始
	for(int id = 0;id < MyBetNumber::getInstance()->size();id ++)
	{
		Bet_Info tempInfo = MyBetNumber::getInstance()->getItem(id);

		for(int i = 0 ; i < zhuiHaoQiShu ; i++){
			if(chaseStatus[i] == 1){
				totalQiShu++;
				beiShu[i] = chaseInputStatus[i] * tempInfo.m_betPlus->getValue(); 
				sprintf(qiShu[i], "%lld",BetLimit::getRealQihao(GDselectedItemTagNumber+i, tempInfo.m_gameKind->getValue(),GDselectedItemTagNumber));
				TouZhuJinE+=chaseInputMonreny[i];
			}
		}

		if(TouZhuJinE == 0) return;

		EntityMgr::instance()->getDispatch()->SendPacketWithTouZhuCQSSC_ZhioHao(zhuiHaoQiShu, qiShu, tempInfo.m_gameKind->getValue(), BetLimit::GetWanfaIdFromId(tempInfo.m_wanFaKind->getValue(),1),
			tempInfo.m_betNumber->getCString(), tempInfo.m_betCount->getValue(), beiShu, tempInfo.m_mode->getValue(), nbeiShu, 0, ZhuiHao, 0);

	}
	
	this->setPositionY(2000);
	NotificationCenter::getInstance()->postNotification("ZhuiHaoback");
	NotificationCenter::getInstance()->postNotification("ZhuiHaoTouZhuJinENumber",Float::create(TouZhuJinE));
}

void ChaseNumBerLayer::touZhuContinueRet(Object *obj)
{
	Array *data = (Array *)obj;
	Integer *lResult = (Integer *)data->objectAtIndex(0);
	String *resultDesc = (String *)data->objectAtIndex(1);
	Integer *nSign = (Integer *)data->objectAtIndex(2);

	//计数
	MyBetNumber::getInstance()->setRetType(lResult->getValue());
	if(!MyBetNumber::getInstance()->isEndBet()) return;
	int returnType = MyBetNumber::getInstance()->getRetType();

	playButtonSound();
	if(returnType == 0){
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t180"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else if(returnType > 0 && returnType <= 18)
	{
		int nTempType=returnType;
		//from T200 to T208
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%ld", 200+ nTempType)->getCString()),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}
	else if(returnType == 111)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t717"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else if(returnType > 18){
		int temp = 1900 + lResult->getValue() * 2;
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(String::createWithFormat("%s%d%s",
			ConfigMgr::instance()->text("display_text.xml", "t718"),
			temp,
			ConfigMgr::instance()->text("display_text.xml", "t719"))->getCString(),
			ccp(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t182"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}
	this->setPositionY(2000);
	NotificationCenter::getInstance()->postNotification("RemoveCurrentLayer");
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
}
void ChaseNumBerLayer::ZhuiHaoQiShuRecord(Object *obj)
{
}

void ChaseNumBerLayer::contiRecord(Object *obj)
{
}

void ChaseNumBerLayer::onEnter()
{
	Layer::onEnter();
}

void ChaseNumBerLayer::onExit()
{
	Layer::onExit();
}

void ChaseNumBerLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	//if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	//{
	//	back(NULL);
	//	event->stopPropagation();
	//}
}

cocos2d::Size ChaseNumBerLayer::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){size.height = 50;;}
	else{size.height = 50;}
	return cocos2d::Size(winSize.width,size.height);
}

ssize_t ChaseNumBerLayer::numberOfCellsInTableView(TableView *table)
{
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){return 5;}
	else{return zhuiHaoQiShu;}

}
TableViewCell *ChaseNumBerLayer::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	//没有值
	if(MyBetNumber::getInstance()->size() == 0) return cell;

	ZhuiHao11Xuan5KIsShowQiShu = false;	
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){
		cell->setTag(index);
		int value = 5.0;
		value+=5*index;
		char str[4] = {0};
		sprintf(str, "%d", value);
		LabelTTF *label = LabelTTF::create(str, "", 22);
		label->setAnchorPoint(Vec2(0,0.5f));
		label->setPosition(Vec2(10, cellHeight*2/3));
		label->setColor(ccc3(0,0,0));
		cell->addChild(label, 3);
		return cell;  
	}else {
		float gap = 15;
		float textWidth = 600;
		float fontSize = 27;
		string nMoshiString = CtratebeilvInput->getText();
		int nBeiShu = atoi(nMoshiString.c_str());
		QuanJunBeiShu =nBeiShu;
		ccColor3B redColor = ccc3(61,8,40);
		ccColor3B blackColor = ccc3(0,0,0);
		LabelTTF *normalttf;
		CCMenuItem *betemRecordItem;
		Sprite *betRecordNormalImage_1;
		Sprite *betRecordNormalImage_2;
		auto tempItemInpu1 = EditBox::create(cocos2d::Size(80, 32), "beting26.png");
		tempItemInpu1->setPosition(Vec2(gap,cellHeight/2));
		tempItemInpu1->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		tempItemInpu1->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		tempItemInpu1->setAnchorPoint(Vec2(0,0.5f));
		tempItemInpu1->setFontColor(ccc3(0,0,0));
		tempItemInpu1->setFontName("");tempItemInpu1->setFontSize(fontSize);
		tempItemInpu1->setColor(ccc3(112,112,112));
		tempItemInpu1->setMaxLength(30);
		tempItemInpu1->setTag(index);
		tempItemInpu1->setInputMode(ui::EditBox::InputMode::NUMERIC); 
		tempItemInpu1->setText(String::createWithFormat("%d", chaseInputStatus[index])->getCString());
		//tempItemInpu1->setDelegate(this);
		cell->addChild(tempItemInpu1, 2);

		tempItemInpu1->addClickEventListener([=](Ref *){tempItemInpu = tempItemInpu1;});
		//期号
		normalttf = LabelTTF::create(String::createWithFormat("%lld",BetLimit::getRealQihao(GDselectedItemTagNumber+index,MyBetNumber::getInstance()->getItem(0).m_gameKind->getValue(),GDselectedItemTagNumber))->getCString(),"",fontSize);
		normalttf->setColor(blackColor);
		normalttf->setAnchorPoint(Vec2(1,0.5f));
		normalttf->setPosition(Vec2(320, cellHeight/2));//280
		cell->addChild(normalttf, 2);
		//金额
		tempIttf = LabelTTF::create(String::createWithFormat("%.3f",chaseInputMonreny[index])->getCString(),"",fontSize);
		tempIttf->setColor(blackColor);
		tempIttf->setAnchorPoint(Vec2(1,0.5f));
		tempIttf->setPosition(Vec2(470,cellHeight/2));
		cell->addChild(tempIttf, 2);
		//选择期数
		Sprite *XuanZeQiShuNormalImage;
		Sprite *XuanZeQiShuSelectedImage;
		if(chaseStatus[index] == 0){
			XuanZeQiShuNormalImage = Sprite::createWithSpriteFrame(spriteFrame("beting27.png"));
			XuanZeQiShuSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("beting27.png"));
		}else{
			XuanZeQiShuNormalImage = Sprite::createWithSpriteFrame(spriteFrame("beting28.png"));
			XuanZeQiShuSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("beting28.png"));
		}
		XuanZeQiShuItem = MenuItemSprite::create(XuanZeQiShuNormalImage,XuanZeQiShuSelectedImage,CC_CALLBACK_1(ChaseNumBerLayer::XuanZeQiShu,this));
		XuanZeQiShuItem->setTag(index);
		middleMenu = Menu::create(XuanZeQiShuItem,NULL);
		middleMenu->setPosition(Vec2(640,cellHeight/2));	//600
		cell->addChild(middleMenu,2);	

		//较正位置
#ifdef VER_369
		tempItemInpu1->setPositionY(tempItemInpu1->getPositionY() + 10);
		normalttf->setPositionY(normalttf->getPositionY() + 10);
		tempIttf->setPositionY(tempIttf->getPositionY() + 10);
		middleMenu->setPositionY(middleMenu->getPositionY() + 10);		
#endif

		//增加背景色
		if (index % 2 == 0)
		{
			Sprite *cellBack = Sprite::createWithSpriteFrame(spriteFrame("room_12_2.png"));	
			cellBack->setAnchorPoint(Vec2(0,0.5));
			cellBack->setPosition(Vec2(10,cellHeight/2));
			cellBack->setOpacity(100);
			cellBack->setScaleX(SCREEN_WIDTH / cellBack->getContentSize().width);
			cellBack->setScaleY(50 / cellBack->getContentSize().height);
			cell->addChild(cellBack,1);

#ifdef VER_369
			cellBack->setVisible(false);
			//新的背景
			Sprite *cellBack1 = Sprite::createWithSpriteFrame(spriteFrame("beting19_3.png"));	
			cellBack1->setAnchorPoint(Vec2(0,0.5));
			cellBack1->setPosition(Vec2(10,cellHeight/2 + 10));				
			cellBack1->setScaleY(45 / cellBack1->getContentSize().height);
			cell->addChild(cellBack1,1);
#endif
		} else
		{
#ifdef VER_369
			Sprite *cellBack = Sprite::createWithSpriteFrame(spriteFrame("beting19_2.png"));	
			cellBack->setAnchorPoint(Vec2(0,0.5));
			cellBack->setPosition(Vec2(10,cellHeight/2 + 10));				
			cellBack->setScaleY(45 / cellBack->getContentSize().height);
			cell->addChild(cellBack,1);
#endif
		}
		return cell;  
	}
}

void ChaseNumBerLayer::tableCellTouched(TableView *table, TableViewCell *cell)
{
	if(table->getTag() != 1111) return;

	//点击
	XuanZeZhuiHaoQiShu(nullptr);

	int cellTag = cell->getTag();
	int value = 5.0;
	if(cellTag <0)
	{
		string beilvInputString = beilvInput->getText();
	    value = atoi(beilvInputString.c_str());
	}else{
    value+=5*cellTag;
	}
    char valueStr[10] = {0};
    sprintf(valueStr, "%d", value);
    beilvInput->setText(valueStr);
    m_TableNode->setPosition(Vec2(1000,1000));
	this->ShowZhuiHaoMen();
	zhuiHaoQiShu = value;
	string nMoshiString = CtratebeilvInput->getText();
	int nBeiShu = atoi(nMoshiString.c_str());
	float temp =0;
	for(int index =0;index<zhuiHaoQiShu;index++)
	{
		chaseInputStatus[index]=nBeiShu;
		chaseInputMonreny[index] =chaseInputStatus[index]*nMoshiNumber*betZhuiHaoZhuShu;
		temp += chaseInputMonreny[index];
		chaseStatus[index] =1;
		char szBuf[100] = {0};  
	    sprintf(szBuf, "%.3f ",temp);
	    String *TouZhuJinECherString = String::createWithFormat("%s %s %s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t1"),szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
	    TouZhuJinEChaseNumBerLabel->setString(TouZhuJinECherString->getCString());
	}
	if(BeiTouBool ==1)
	{
	    float tempBUn =0;
		if(nBeiShu !=1)
		{
			chaseInputStatus[0] = nBeiShu;
		}
		for(int i = 0 ; i < zhuiHaoQiShu ; i++)
		{
			if(nBeiShu !=1)
			{
				chaseInputStatus[i+1]= chaseInputStatus[i]*2;
			}else
			{
				chaseInputStatus[i+1]= chaseInputStatus[i]*2;
				CCLOG("%d",chaseInputStatus[i+1]);
			}
			if(chaseInputStatus[i+1]>MAX_ZHUIHAO_BEISHU)
			{
				chaseInputStatus[i+1] = MAX_ZHUIHAO_BEISHU;
			}
			chaseStatus[i]=1;
			chaseInputMonreny[i] = chaseInputStatus[i]*nMoshiNumber*betZhuiHaoZhuShu;
			CCLOG("MonENy == %d",chaseInputMonreny[i]);
			tempIttf ->setString(String::createWithFormat("%.3f",chaseInputMonreny[i])->getCString());
			temp += chaseInputMonreny[i];
					char szBuf[100] = {0};  
	    sprintf(szBuf, "%.3f ",temp);
	    String *TouZhuJinECherString = String::createWithFormat("%s %s %s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t1"),szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
	    TouZhuJinEChaseNumBerLabel->setString(TouZhuJinECherString->getCString());
		}
	}
	ZhuiHaoQiShuBool = false;
	touZhuTable->reloadData();
}

void ChaseNumBerLayer::scrollViewDidScroll(ScrollView* view)
{

}
void ChaseNumBerLayer::scrollViewDidZoom(ScrollView* view)
{

}
void ChaseNumBerLayer::XuanZeQiShu(Object *obj)
{
	playButtonSound();

	MenuItemSprite *XuanZeQiShuItem = (MenuItemSprite *)obj;
	int itemTag = XuanZeQiShuItem->getTag();
	if(chaseStatus[itemTag] == 0)
	{
		chaseStatus[itemTag] = 1;
		Sprite *XuanZeQiShuNormalImage = Sprite::createWithSpriteFrame(spriteFrame("beting28.png"));
		Sprite *XuanZeQiShuSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("beting28.png"));
		XuanZeQiShuItem->setNormalImage(XuanZeQiShuNormalImage);
		XuanZeQiShuItem->setSelectedImage(XuanZeQiShuSelectedImage);
	}else
	{
		chaseStatus[itemTag] = 0;
		Sprite *XuanZeQiShuNormalImage = Sprite::createWithSpriteFrame(spriteFrame("beting27.png"));
		Sprite *XuanZeQiShuSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("beting27.png"));
		XuanZeQiShuItem->setNormalImage(XuanZeQiShuNormalImage);
		XuanZeQiShuItem->setSelectedImage(XuanZeQiShuSelectedImage);
	}
	float temp=0;
	int temp2 =0;
	for(int i =0;i<zhuiHaoQiShu;i++)
	{	
		if(chaseStatus[i]==1)
		{
			chaseInputMonreny[i] =chaseInputStatus[i]*nMoshiNumber*betZhuiHaoZhuShu;
			temp += chaseInputMonreny[i];
			char szBuf[100] = {0};  
			sprintf(szBuf, "%.3f ",temp);
			String *TouZhuJinECherString = String::createWithFormat("%s %s %s",
				ConfigMgr::instance()->text("display_DuangDong.xml", "t1"),szBuf,
				ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
			TouZhuJinEChaseNumBerLabel->setString(TouZhuJinECherString->getCString());
		}
		temp2 +=chaseStatus[i];
		if(temp2 ==0)
		{
			temp =0;
			char szBuf[100] = {0};  
			sprintf(szBuf, "%.3f ",temp);
			String *TouZhuJinECherString = String::createWithFormat("%s %s %s",
				ConfigMgr::instance()->text("display_DuangDong.xml", "t1"),szBuf,
				ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
			TouZhuJinEChaseNumBerLabel->setString(TouZhuJinECherString->getCString());

		}
	}


}
void ChaseNumBerLayer::XuanZeZhuiHaoQiShu(Object *obj)
{
	playButtonSound();

	//MenuItemSprite *ButtonCodeRecordItem = (MenuItemSprite *)obj;
    if (fandianStatus != 0) 
	{
        fandianStatus = 0;
       // ButtonCodeRecordItem->unselected();
		m_TableNode->setPosition(Vec2(1000,1000));
		this->ShowZhuiHaoMen();
		betRecordItem->setEnabled(true);
		touZhuTable->setTouchEnabled(true);
        return;
    }
   // ButtonCodeRecordItem->selected();
    fandianStatus = 2;
    m_TableNode->setPosition(Vec2(winSize.width*0.2+2,winSize.height*0.6-3));
	this->dissZhuiHaoMen();
	betRecordItem->setEnabled(false);
	touZhuTable->setTouchEnabled(false);
	fandianTable->reloadData();
}

void ChaseNumBerLayer::IsEndeBeiTou(Object *obj)
{
	playButtonSound();

	MenuItemSprite *betRecordItem = (MenuItemSprite *)obj;
	string nMoshiString = CtratebeilvInput->getText();
	int nBeiShu = atoi(nMoshiString.c_str());
	float temp = 0;
	if(BeiTouBool==0)
	{
		BeiTouBool = 1;
		Sprite *StramNormalImage_1 = Sprite::createWithSpriteFrame(spriteFrame("beting15.png"));
		Sprite *StramNormalImage_2 = Sprite::createWithSpriteFrame(spriteFrame("beting15.png"));
		betRecordItem->setNormalImage(StramNormalImage_1);
		betRecordItem->setSelectedImage(StramNormalImage_2);
		if(nBeiShu !=1)
		{
			chaseInputStatus[0] = nBeiShu;
		}
		for(int i = 0 ; i < zhuiHaoQiShu ; i++)
		{
			if(nBeiShu !=1)
			{
				chaseInputStatus[i+1]= chaseInputStatus[i]*2;
			}else
			{
				chaseInputStatus[i+1]= chaseInputStatus[i]*2;
			}
			if(chaseInputStatus[i+1]>MAX_ZHUIHAO_BEISHU)
			{
				chaseInputStatus[i+1] = MAX_ZHUIHAO_BEISHU;
			}
			if(chaseStatus[i]==1){

			chaseInputMonreny[i] = chaseInputStatus[i]*nMoshiNumber*betZhuiHaoZhuShu;
			tempIttf ->setString(String::createWithFormat("%.3f",chaseInputMonreny[i])->getCString());
			temp += chaseInputMonreny[i];
			}
		}
		char szBuf[100] = {0};  
	    sprintf(szBuf, "%.3f ",temp);
	    String *TouZhuJinECherString = String::createWithFormat("%s %s %s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t1"),szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
	    TouZhuJinEChaseNumBerLabel->setString(TouZhuJinECherString->getCString());
		touZhuTable->reloadData();
	}else
	{
		BeiTouBool = 0;
		Sprite *StramNormalImage_1 = Sprite::createWithSpriteFrame(spriteFrame("beting16.png"));
		Sprite *StramNormalImage_2 = Sprite::createWithSpriteFrame(spriteFrame("beting16.png"));
		betRecordItem->setNormalImage(StramNormalImage_1);
		betRecordItem->setSelectedImage(StramNormalImage_2);
		for(int i = 0 ; i < zhuiHaoQiShu ; i++)
		{
			if(nBeiShu !=1)
			{
				chaseInputStatus[i]= nBeiShu;
			}else
				{chaseInputStatus[i+1]= 1;
			}
			if(chaseStatus[i]==1){
			chaseInputMonreny[i] = chaseInputStatus[i]*nMoshiNumber*betZhuiHaoZhuShu;
			tempIttf ->setString(String::createWithFormat("%.3f",chaseInputMonreny[i])->getCString());
			temp += chaseInputMonreny[i];
			}
		}
		char szBuf[100] = {0};  
	    sprintf(szBuf, "%.3f ",temp);
	    String *TouZhuJinECherString = String::createWithFormat("%s %s %s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t1"),szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
		TouZhuJinEChaseNumBerLabel->setString(TouZhuJinECherString->getCString());
		touZhuTable->reloadData();
	}
}
void ChaseNumBerLayer::IsEndeZhuiHao(Object *obj)
{
	playButtonSound();

	MenuItemSprite *GetRecordItem = (MenuItemSprite *)obj;
	if(ZhuiHaoBool == 0)
	{
		ZhuiHaoBool = 1;
		ZhuiHao =2;
		Sprite *CloseNormalImage_1 = Sprite::createWithSpriteFrame(spriteFrame("beting16.png"));
		Sprite *CloseNormalImage_2 = Sprite::createWithSpriteFrame(spriteFrame("beting16.png"));
		GetRecordItem->setNormalImage(CloseNormalImage_1);
		GetRecordItem->setSelectedImage(CloseNormalImage_2);
	}else
	{
		ZhuiHaoBool = 0;
		ZhuiHao = 4;
		Sprite *CloseNormalImage_1 = Sprite::createWithSpriteFrame(spriteFrame("beting15.png"));
		Sprite *CloseNormalImage_2 = Sprite::createWithSpriteFrame(spriteFrame("beting15.png"));
		GetRecordItem->setNormalImage(CloseNormalImage_1);
		GetRecordItem->setSelectedImage(CloseNormalImage_2);
	}
}

void ChaseNumBerLayer::editBoxEditingDidEnd(EditBox* editBox)
{
	string nMoshiString = CtratebeilvInput->getText();
	int nBeiShu = atoi(nMoshiString.c_str());

	if(nBeiShu < 1)
	{
		nBeiShu = 1;
		CtratebeilvInput->setText("1");
	}

	int boxTag = editBox->getTag();
	string beilvInputString = beilvInput->getText();
	int nbeilvIn = atoi(beilvInputString.c_str());
	zhuiHaoQiShu =nbeilvIn;
	
	if(zhuiHaoQiShu > MAX_QIHAO)
	{
		String* displayStr = String::createWithFormat("%s%d%s",ConfigMgr::instance()->text("display_text.xml", "t994"),MAX_QIHAO,ConfigMgr::instance()->text("display_text.xml", "t71"));
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(displayStr->getCString(),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);	
		beilvInput->setText("50");
		zhuiHaoQiShu = MAX_QIHAO;
	}

	if(nBeiShu > MAX_ZHUIHAO_BEISHU)
	{
		nBeiShu = MAX_ZHUIHAO_BEISHU;
		CtratebeilvInput->setText("999");
	}
	
	if(QuanJunBeiShu !=nBeiShu)
	{
		if(nBeiShu !=1)
		{
			chaseInputStatus[0] = nBeiShu;
		}
		for(int i =0;i<zhuiHaoQiShu;i++)
	    {
			if(BeiTouBool ==0)
			{
				chaseInputStatus[i] = nBeiShu;
			}else{
			chaseInputStatus[i+1]= chaseInputStatus[i]*2;
			if(chaseInputStatus[i+1]>MAX_ZHUIHAO_BEISHU)
			{
				chaseInputStatus[i+1] = MAX_ZHUIHAO_BEISHU;
			}
			}
			chaseStatus[i]=1;
		}
	}else if(boxTag < 0&&BeiTouBool==0)
	{
		for(int index =0;index<zhuiHaoQiShu;index++){
		chaseStatus[index]=1;
		chaseInputStatus[index]=nBeiShu;
		}
	}
	else{
		chaseInputStatus[boxTag] = (atoi(editBox->getText()));
		if(chaseInputStatus[boxTag] <=0){
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t169"),
		Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		chaseInputStatus[boxTag] = 1;
		return;
	}else if(chaseInputStatus[boxTag]>MAX_ZHUIHAO_BEISHU){
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
			String::createWithFormat("%s%d%s",ConfigMgr::instance()->text("display_DuangDong.xml", "t170"),MAX_ZHUIHAO_BEISHU,ConfigMgr::instance()->text("display_DuangDong.xml", "t199"))->getCString(),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		chaseInputStatus[boxTag] = 1;
		editBox->setText("1");		
		return;
		}
	}
	float temp=0;
	for(int i =0;i<zhuiHaoQiShu;i++)
	{	
		if(chaseStatus[i]==1){

		chaseInputMonreny[i] =chaseInputStatus[i]*nMoshiNumber*betZhuiHaoZhuShu;
		tempIttf ->setString(String::createWithFormat("%.3f",chaseInputMonreny[i])->getCString());
		temp += chaseInputMonreny[i];
	}
	}
	char szBuf[100] = {0};  
	sprintf(szBuf, "%.3f ",temp);
	String *TouZhuJinECherString = String::createWithFormat("%s %s %s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t1"),szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
	TouZhuJinEChaseNumBerLabel->setString(TouZhuJinECherString->getCString());
	if(BaoCunQiShu != nbeilvIn&&BeiTouBool==1)
	{
		BaoCunQiShu = nbeilvIn;
	    zhuiHaoQiShu = nbeilvIn;
	    float tempBUn =0;
		if(nBeiShu !=1)
		{
			chaseInputStatus[0] = nBeiShu;
		}
		for(int i = 0 ; i < zhuiHaoQiShu ; i++)
		{
			if(nBeiShu !=1)
			{
				chaseInputStatus[i+1]= chaseInputStatus[i]*2;
			}else
			{
				chaseInputStatus[i+1]= chaseInputStatus[i]*2;
				CCLOG("%d",chaseInputStatus[i+1]);
			}
			if(chaseInputStatus[i+1]>MAX_ZHUIHAO_BEISHU)
			{
				chaseInputStatus[i+1] = MAX_ZHUIHAO_BEISHU;
			}
			chaseStatus[i]=1;
			chaseInputMonreny[i] = chaseInputStatus[i]*nMoshiNumber*betZhuiHaoZhuShu;
			CCLOG("MonENy == %d",chaseInputMonreny[i]);
			tempIttf ->setString(String::createWithFormat("%.3f",chaseInputMonreny[i])->getCString());
			temp += chaseInputMonreny[i];
					char szBuf[100] = {0};  
	    sprintf(szBuf, "%.3f ",temp);
	    String *TouZhuJinECherString = String::createWithFormat("%s %s %s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t1"),szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
	    TouZhuJinEChaseNumBerLabel->setString(TouZhuJinECherString->getCString());
		}
	}
	touZhuTable->reloadData();
}

