#include "XingYunGambleLayer.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include <sstream>
#include "LotteryHelpView.h"
#include "BetLimit.h"
#include "comm.h"
#include "LotterySceneControl.h"
#include "LotteryGameResultDetail.h"
#include "VersionControl.h"

#define CHOUMA_BOX 8321
#define CHOUMA_BUTTON  9321

using namespace CocosDenshion;
using namespace extension;

Scene* XingYunGambleLayer::scene()
{
	Scene *scene = Scene::create();
	XingYunGambleLayer *layer = XingYunGambleLayer::create();
	scene->addChild(layer);
	return scene;
}

XingYunGambleLayer::XingYunGambleLayer()
{
	MyBetNumber::addRandSeed();
	wKindID = NULL;
	wPeriod = NULL;
	szLotNum = NULL;
	szShijian = NULL;
	gameRule = NULL;
	gameKind = CZ_LUCKY_28;
	betZhuShuTouZhuJinE =0;
	TouZhuJinE =0.00;
	beiShu = 0;
	ZhangHuYuE = EntityMgr::instance()->getDispatch()->f_yue;

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(XingYunGambleLayer::GetLastYueRet), "GetLastYueRet", NULL);  ///余额查询	
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(XingYunGambleLayer::touZhuRet), "CQSSCRet", NULL);

	//跨年期号处理
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(XingYunGambleLayer::kuaNianExpect), "KuaNian", NULL);
}

void XingYunGambleLayer::getSystemTime(float dt)
{
	EntityMgr::instance()->getDispatch()->SendPacketWithGetKuaNianQiHao(gameKind);
}

XingYunGambleLayer::~XingYunGambleLayer()
{
	if(wKindID != NULL)
	{
		wKindID->release();
		wKindID = NULL;
	}
	if(wPeriod != NULL)
	{
		wPeriod->release();
		wPeriod = NULL;
	}
	if(szLotNum != NULL)
	{
		szLotNum->release();
		szLotNum = NULL;
	}
	if(szShijian != NULL)
	{
		szShijian->release();
		szShijian = NULL;
	}

	NotificationCenter::getInstance()->removeObserver(this, "GetLastYueRet");
	NotificationCenter::getInstance()->removeObserver(this, "CQSSCRet");	
	this->unschedule(schedule_selector(XingYunGambleLayer::myUpdate));
	this->unschedule(schedule_selector(XingYunGambleLayer::getSystemTime));

	NotificationCenter::getInstance()->removeObserver(this, "KuaNian");

	removeAllChildrenWithCleanup(true);
	TextureCache::getInstance()->removeAllTextures(); //释放到目前为止所有加载的图片
	TextureCache::getInstance()->removeUnusedTextures(); 
}

void XingYunGambleLayer::setGameKind(CaiZhong kind)
{

}

void XingYunGambleLayer::updatePeriod(float dt)
{
	
}

void XingYunGambleLayer::refreshLayer()
{
	//更新余额
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();		
}

void XingYunGambleLayer::setWanFaKind(XingYun28Kind kind)
{
	wanFaKind = kind;	
}

bool XingYunGambleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	//跨年处理
#if 1
	EntityMgr::instance()->getDispatch()->SendPacketWithGetKuaNianQiHao(gameKind);
	this->schedule(schedule_selector(XingYunGambleLayer::getSystemTime), 3);
#endif
	this->schedule(schedule_selector(XingYunGambleLayer::myUpdate), 1);

	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();

	//监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(XingYunGambleLayer::onTouchBegan, this);
	listener->onTouchCancelled = CC_CALLBACK_2(XingYunGambleLayer::onTouchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(XingYunGambleLayer::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(XingYunGambleLayer::onTouchMoved, this) ;

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void XingYunGambleLayer::myUpdate(float dt)
{
	string value6 = gameRule->GetNextExpect();
	string value3 = gameRule->GetKjShjDiffDesc();

	stringstream strValue;
	strValue << value6.c_str();
	strValue>>currentLotteryId;

	currentTime = value3;

	this->updateLotteryId();
	this->updateCurrentTime();
	EntityMgr::instance()->getDispatch()->TimeIncrease();
}

//初始化默认数据
//确定进入以后默认玩法
void XingYunGambleLayer::initData()
{

	n_t_kindid=NULL;			//玩法ID
	f_t_bonus = NULL;			//最小奖金数字
	f_t_bonusPercent = NULL;	//间距
	f_t_fandian = NULL;			//最大百分比

	selectedItemTag = 1;
	wanFaKind = XingYun28Kind::enXY28_Funny;

	currentLotteryId = 0;
	currentTime = string("19:30");
	winSize = Director::getInstance()->getWinSize();

	//初始化
	if(gameRule == NULL)
	{
		gameRule = new CKuaiLe8Rule();
	}
}

//初始化界面
void XingYunGambleLayer::initView()
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
	this->addChild(bk1,2);

	//顶部返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(XingYunGambleLayer::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setAnchorPoint(Vec2(0,1));
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,2);

	//顶部功能按钮
	Sprite *setupItemNormalImage = Sprite::createWithSpriteFrame(spriteFrame("room_03.png"));
	Sprite *setupItemSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("room_03.png"));
	MenuItemSprite *setupItem = MenuItemSprite::create(setupItemNormalImage,setupItemSelectedImage,CC_CALLBACK_1(XingYunGambleLayer::setting,this));
	setupItem->setPosition(Vec2(SCREEN_WIDTH - 17 - setupItemNormalImage->getContentSize().width / 2, pCloseButton->getPositionY()));	
	LabelTTF* moreItemLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t4700"), "", 30);
	MenuItemLabel *lbItem = MenuItemLabel::create(moreItemLabel,CC_CALLBACK_1(XingYunGambleLayer::jumpResult,this));
	lbItem->setPosition(Vec2(setupItem->getPositionX() - setupItem->getContentSize().width - 15, setupItem->getPositionY() - 10));
	Menu *m_pSettingMenu = Menu::create(setupItem, lbItem, NULL);
	m_pSettingMenu->setPosition(Vec2::ZERO);
	this->addChild(m_pSettingMenu,2);	

	//彩种名称
	float fontSize = 38;
	gameTitle = LabelTTF::create(ConfigMgr::instance()->text("display_config.xml", "t37"), "", fontSize);
	gameTitle->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	gameTitle->setColor(ccc3(255,255,255));
	this->addChild(gameTitle, 2);

	//第N期
	fontSize = 27;
	String *lotteryIdString;
	lotteryIdString = String::createWithFormat("%s%s",
		ConfigMgr::instance()->text("display_text.xml", "t70"),
		ConfigMgr::instance()->text("display_text.xml", "t71"));
	lotteryIdLabel = LabelTTF::create(lotteryIdString->getCString(), "", fontSize);
	lotteryIdLabel->setAnchorPoint(Vec2(0,0.5f));
	lotteryIdLabel->setPosition(Vec2(32, SCREEN_HEIGHT - 162 - 35));
	lotteryIdLabel->setColor(ccc3(0,0,0));
	this->addChild(lotteryIdLabel, 2);

	//当前时间倒计时
	String *currentTimeString = String::create(ConfigMgr::instance()->text("display_text.xml", "t72"));
	currentTimeLabel = LabelTTF::create(currentTimeString->getCString(), "", fontSize);
	currentTimeLabel->setAnchorPoint(Vec2(0,0.5f));
	currentTimeLabel->setPosition(Vec2(SCREEN_WIDTH/2, lotteryIdLabel->getPositionY()));
	currentTimeLabel->setColor(ccc3(0,0,0));
	this->addChild(currentTimeLabel, 2);	

	//下划线
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, lotteryIdLabel->getPositionY() - 24)); //
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	this->addChild(gap1,2);

	static char* arrayInt[]={"50","100","500","1000","5000"};
	//筹码
	for(int k = 0;k < 5;k ++)
	{
		Sprite* sptChouMa = Sprite::createWithSpriteFrame(spriteFrame(String::createWithFormat("dt_tz_bs_jian_%02d.png",6 + k)->getCString()));
		sptChouMa->setPosition(Vec2(SCREEN_WIDTH / 2 - (2 - (k % 5)) * (sptChouMa->getContentSize().width + 50), 150 + sptChouMa->getContentSize().height / 2 + 10));
		sptChouMa->setScale(1.4f);
		sptChouMa->setTag(CHOUMA_BUTTON + k);
		this->addChild(sptChouMa);
#ifdef VER_QIANBO
		sptChouMa->setScale(1.2f);
		sptChouMa->setPositionY(150 + sptChouMa->getContentSize().height / 2);
#endif // VER_QIANBO

#ifdef VER_369
		sptChouMa->setScale(1.2f);
		sptChouMa->setPositionY(150 + sptChouMa->getContentSize().height / 2);
#endif // VER_QIANBO

		//编辑
		EditBox* dtChouMa = EditBox::create(cocos2d::Size(80, 20), "blank.png");
		dtChouMa->setPosition(sptChouMa->getPosition());
		dtChouMa->setText(UserDefault::getInstance()->getStringForKey(String::createWithFormat("CHOUMA_%d", k)->getCString(),arrayInt[k]).c_str());
		dtChouMa->setEnabled(false);
		dtChouMa->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		dtChouMa->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		dtChouMa->setInputMode(ui::EditBox::InputMode::NUMERIC);
		dtChouMa->setFontName("");
		dtChouMa->setFontSize(20);
		dtChouMa->setTag(CHOUMA_BOX + k);
		this->addChild(dtChouMa, 3);
		//dtChouMa->setDelegate(this);
	}	

	//底部背景不透明
	Sprite *bottomBk = Sprite::createWithSpriteFrame(spriteFrame("room_12_2.png"));
	bottomBk->setPosition(Vec2(SCREEN_WIDTH/2, 65));
	bottomBk->setScaleX(SCREEN_WIDTH / bottomBk->getContentSize().width);
	bottomBk->setScaleY(150/bottomBk->getContentSize().height);
	//bottomBk->setColor(ccc3(50,50,50));
	this->addChild(bottomBk,2);

	int tempH = 30;

	//文字 投注多少，彩票余额多少
	TouZhuJinELabel = LabelTTF::create("", "", fontSize);
	TouZhuJinELabel->setPosition(Vec2(34,bottomBk->getPositionY() - tempH));//winSize.width*0.03
	TouZhuJinELabel->setAnchorPoint(Vec2(0, 0.5f));
	TouZhuJinELabel->setFontFillColor(ccc3(0,0,0));
	this->addChild(TouZhuJinELabel, 2);

	updateMoneyLabel();

	//新增
	LabelTTF *beilvBkTTf  = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t1"), "", fontSize);
	beilvBkTTf->setAnchorPoint(Vec2(0,0.5f));
	beilvBkTTf->setPosition(Vec2(30, bottomBk->getPositionY() + tempH));
	beilvBkTTf->setFontFillColor(ccc3(0,0,0));
	this->addChild(beilvBkTTf, 3);
	//后边框 多少倍
	Sprite *beilvBk = Sprite::createWithSpriteFrame(spriteFrame("dt_tz_bs_03.png"));
	beilvBk->setAnchorPoint(Vec2(0,0.5f));
	beilvBk->setPosition(Vec2(170, beilvBkTTf->getPositionY()));
	this->addChild(beilvBk,3);

	//输入框
	fontSize = 30;
	beilvInput = EditBox::create(cocos2d::Size(110, 32), "blank.png");
	beilvInputMidX = beilvBk->getPositionX();
	beilvInput->setPosition(Vec2(beilvInputMidX, beilvBk->getPositionY()));
	beilvInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	beilvInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	beilvInput->setInputMode(ui::EditBox::InputMode::NUMERIC);
	beilvInput->setAnchorPoint(Vec2(0,0.5f));
	beilvInput->setFontColor(ccc3(0,0,0));
	beilvInput->setFontName("");
	beilvInput->setFontSize(fontSize);
	beilvInput->setColor(ccc3(112,112,112));
	beilvInput->setText("0");
	beilvInput->setMaxLength(20);
	this->addChild(beilvInput, 3);
	beilvInput->setDelegate(this);

	//底部按钮 确定投注和选定
	Sprite *cartNormalImage = Sprite::createWithSpriteFrame(spriteFrame("bt_tz_bs_jian_normal61.png"));//自定义
	Sprite *cartSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("bt_tz_bs_jian_pressed61.png"));
	Sprite *confirmNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_11.png"));//确认投注
	Sprite *confirmSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_12.png"));
	Sprite *modifyNormalImage = Sprite::createWithSpriteFrame(spriteFrame("bt_tz_bs_jian_normal62.png"));//保存自定义
	Sprite *modifySelectedImage = Sprite::createWithSpriteFrame(spriteFrame("bt_tz_bs_jian_pressed62.png"));
	Sprite *cancelNormalImage = Sprite::createWithSpriteFrame(spriteFrame("beting24.png"));//取消
	Sprite *cancelSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("beting25.png"));
	MenuItemSprite *cartItem = MenuItemSprite::create(cartNormalImage,cartSelectedImage,CC_CALLBACK_1(XingYunGambleLayer::toCustom,this));
	MenuItemSprite *confirmItem = MenuItemSprite::create(confirmNormalImage,confirmSelectedImage,CC_CALLBACK_1(XingYunGambleLayer::toGamble,this));
	MenuItemSprite *modifyItem = MenuItemSprite::create(modifyNormalImage, modifySelectedImage, CC_CALLBACK_1(XingYunGambleLayer::toConfirm, this));
	MenuItemSprite *cancelItem = MenuItemSprite::create(cancelNormalImage, cancelSelectedImage, CC_CALLBACK_1(XingYunGambleLayer::toCancel, this));
	cartItem->setPosition(Vec2(SCREEN_WIDTH - 32 * 2 - cartNormalImage->getContentSize().width - confirmNormalImage->getContentSize().width / 2, bottomBk->getPositionY() + tempH));
	confirmItem->setPosition(Vec2(SCREEN_WIDTH - 32 - cartNormalImage->getContentSize().width / 2, bottomBk->getPositionY() + tempH));
	modifyItem->setPosition(confirmItem->getPosition());
	cancelItem->setPosition(cartItem->getPosition());
	cartItem->setTag(9528);
	confirmItem->setTag(9529);
	modifyItem->setTag(9530);
	cancelItem->setTag(9531);
	Menu *bottomMenu = Menu::create(cartItem,confirmItem,modifyItem,cancelItem,NULL);
	//Menu *bottomMenu = Menu::create(deleteItem,confirmItem,NULL);
	bottomMenu->setPosition(Vec2::ZERO);
	bottomMenu->setTag(9527);
	this->addChild(bottomMenu,2);

#ifdef VER_369
	cartItem->setPositionX(cartItem->getPositionX() + 20);
	cancelItem->setPositionX(cartItem->getPositionX());
	TouZhuJinELabel->setFontFillColor(ccc3(255,255,255));
	beilvInput->setFontColor(ccc3(255,255,255));
	beilvBkTTf->setFontFillColor(ccc3(255,255,255));

	cancelNormalImage->setSpriteFrame(cancelSelectedImage->getDisplayFrame());
	cancelSelectedImage->setSpriteFrame(spriteFrame("beting24.png"));//取消
#endif

	modifyItem->setVisible(false);
	cancelItem->setVisible(false);

	//分组选择 彩票选择模块
	betArea = XingYunWuFen::create();
	this->addChild(betArea,1);

	//顶部掩体，向上滑动时到此为止就被遮住
	LayerColor* maskLayer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, 190);
	maskLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	maskLayer->setPosition(Vec2(winSize.width*0.5, SCREEN_HEIGHT)); 
	this->addChild(maskLayer, 1);
}

void XingYunGambleLayer::updateLotteryId()
{
	static bool isRun = false;
	if(isRun) return;
	isRun = true;

	char tempStr[128] = {0};
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	sprintf(tempStr, "%I64d", currentLotteryId);
#else
	sprintf(tempStr, "%lld", currentLotteryId);
#endif
	if(wPeriod != NULL)
	{
		wPeriod->release();
		wPeriod = NULL;
	}
	wPeriod = String::create(tempStr);
	//CCLOG("nnnnnnnnnnnnnnnnn now period: %s", wPeriod->getCString());
	wPeriod->retain();
	String *lotteryIdString;
	lotteryIdString = String::createWithFormat("%s%s%s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t15"), tempStr,
		ConfigMgr::instance()->text("display_text.xml", "t71"));
	lotteryIdLabel->setString(lotteryIdString->getCString());

	NotificationCenter::getInstance()->postNotification("GDselectedItemTagNumberCartFailed",String::create(tempStr));

	isRun = false;
}

void XingYunGambleLayer::updateCurrentTime()
{
	String *headStr = NULL;
	if(currentLotteryId == 0)
	{
		headStr = String::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t1550"));
	} else
	{
		if(gameRule->IsFenDanDuration())//正在封单
		{
			headStr = String::create(ConfigMgr::instance()->text("display_text.xml", "t74"));		
		} else
		{
			headStr = String::create(ConfigMgr::instance()->text("display_text.xml", "t72"));
		}
	}

	currentTimeLabel->setString(String::createWithFormat("%s%s",headStr->getCString(), currentTime.c_str())->getCString());	
}

void XingYunGambleLayer::kuaNianExpect(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;
	Integer* I_KindId = (Integer *)data->objectForKey("n_t_kindid");
	Integer* I_Ifts = (Integer *)data->objectForKey("c_t_ifts");
	String* S_Expect = (String *)data->objectForKey("s_t_expect");
	String* S_Time = (String *)data->objectForKey("s_t_shijian");

	//有可能是上一个返回的
	if(I_KindId->getValue() != gameKind) return;
	gameRule->setIsStopSell(I_Ifts->getValue());
	gameRule->SetStartQiHao(atoi(S_Expect->getCString()));
	gameRule->SetStartTime(S_Time->getCString());
}

void XingYunGambleLayer::touZhuRet(Object *obj)
{
	MyBetNumber::getInstance()->betOneEnd();
	
	this->setKeypadEnabled(true);
	Array *data = (Array *)obj;
	Integer *lResult = (Integer *)data->objectAtIndex(0);
	String *resultDesc = (String *)data->objectAtIndex(1);
	Integer *nSign = (Integer *)data->objectAtIndex(2);

	//计数
	MyBetNumber::getInstance()->setRetType(lResult->getValue());
	if(!MyBetNumber::getInstance()->isEndBet()) return;
	int returnType = MyBetNumber::getInstance()->getRetType();

	if(returnType == 0){		
		EntityMgr::instance()->getDispatch()->f_yue-= TouZhuJinE;  

		TouZhuJinE =0;
		betZhuShuTouZhuJinE =0;
		beiShu = 0;
		beilvInput->setText("0");	

		MovingLabelLayer* layer =MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t180"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}
	else if(returnType > 0 && returnType < 18)
	{
		int nTempType=returnType;
		//from T200 to T208
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%ld", 200+ nTempType)->getCString()),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}
	else if(lResult->getValue() == 112)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t4600"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else if(lResult->getValue() == 111)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t717"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else if(lResult->getValue() > 18){
		int temp = 1900 + lResult->getValue() * 2;
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(String::createWithFormat("%s%d%s",
			ConfigMgr::instance()->text("display_text.xml", "t718"),
			temp,
			ConfigMgr::instance()->text("display_text.xml", "t719"))->getCString(),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t182"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}

	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
}

//top menu
void XingYunGambleLayer::setting(Object* obj)
{
	playButtonSound();
	LotteryHelpView *layer = LotteryHelpView::create();
	layer->setHelpId(gameKind);
	
	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}

void XingYunGambleLayer::jumpResult(Object* obj)
{	
	playButtonSound();	

	//跳转到开奖
	LotteryGameResultDetail *layer = LotteryGameResultDetail::create();
	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);	

	EntityMgr::instance()->getDispatch()->SendPacketQueryMoreGameResult(gameKind);
}

void XingYunGambleLayer::toGamble(Object* obj)
{
	playButtonSound();

	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if(!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t167"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else{
		if(betArea == NULL){
			return;
		}
		//封单		
		if(gameRule->IsFenDanDuration())
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t8"),
				Vec2(winSize.width * 0.5,winSize.height * 0.5));
			this->addChild(layer, 255);
			MyBetNumber::getInstance()->clear();
			betArea->clearNumbers();
			betArea->cancelAllMenuSelect();
			return;
		}
		String *betNumber = betArea->getBetNumber();
		int betZhuShu = betArea->getBetZhuShu(); //实际上是玩法ID
		wanFaKind = (XingYun28Kind)betZhuShu;
		betZhuShu = 1;//重置注数
		CCLOG("%s", betNumber->getCString());
		
		if(strcmp(betNumber->getCString(), "") == 0){
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t198"),
				Vec2(winSize.width * 0.5, winSize.height * 0.5));
			this->addChild(layer, 255);
			return;
		}
		if(beiShu < 1)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t1005"),
				Vec2(winSize.width * 0.5, winSize.height * 0.5));
			this->addChild(layer, 255);
			return;
		}

		if(beiShu > ZhangHuYuE)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t1006"),
				Vec2(winSize.width * 0.5, winSize.height * 0.5));
			this->addChild(layer, 255);
			return;
		}

		if(wPeriod == NULL)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t439"),
				Vec2(winSize.width * 0.5,winSize.height * 0.5));
			this->addChild(layer, 255);
			return;
		}	

		//判断是否还在投注
		if(!MyBetNumber::getInstance()->isEndBet())
		{
			return;
		}

		MyBetNumber::getInstance()->betReady();

		char tempStr[20] = {0};
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		sprintf(tempStr, "%I64d", currentLotteryId);
#else
		sprintf(tempStr, "%lld", currentLotteryId);
#endif
		string qishuStr = string(tempStr);
		EntityMgr::instance()->getDispatch()->SendPacketWithTouZhuCQSSC(qishuStr, gameKind, wanFaKind,
			betNumber->getCString(), betZhuShu, beiShu, 0, 1, 0, 0);
	}
	//清空数据
	MyBetNumber::getInstance()->clear();
	betArea->clearNumbers();
	betArea->cancelAllMenuSelect();
}

void XingYunGambleLayer::getSystemTimeObject(Object *obj)
{
	BaseLayer::getSystemTimeObject(nullptr);

	//Dictionary *data = (Dictionary *)obj;
	//wYear = ((Integer *)data->objectForKey("wYear"))->getValue();
	//wMonth = ((Integer *)data->objectForKey("wMonth"))->getValue();
	//wDay = ((Integer *)data->objectForKey("wDay"))->getValue();
	//wHour = ((Integer *)data->objectForKey("wHour"))->getValue();
	//wMinute = ((Integer *)data->objectForKey("wMinute"))->getValue();
	//wSecond = ((Integer *)data->objectForKey("wSecond"))->getValue();
}

void XingYunGambleLayer::toCustom(Object* obj)
{
	playButtonSound();

	Menu* tempMenu = (Menu *)this->getChildByTag(9527);
	if(tempMenu == nullptr) return;
	MenuItemSprite *tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9528);
	if(tempItem1) tempItem1->setVisible(false);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9529);
	if(tempItem1) tempItem1->setVisible(false);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9530);
	if(tempItem1) tempItem1->setVisible(true);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9531);
	if(tempItem1) tempItem1->setVisible(true);
	//放开
	for(int k = 0;k < 5;k ++)
	{		
		//编辑
		EditBox* dtChouMa = (EditBox* )this->getChildByTag(CHOUMA_BOX + k);
		if (dtChouMa)
		{
			dtChouMa->setEnabled(true);			
		}	
	}	
}

void XingYunGambleLayer::toConfirm(Object* obj)
{
	toCancel(nullptr);

	//编辑框
	for(int k = 0;k < 5;k ++)
	{		
		//编辑
		EditBox* dtChouMa = (EditBox* )this->getChildByTag(CHOUMA_BOX + k);
		if (dtChouMa)
		{
			std::string tempStr = dtChouMa->getText();
			UserDefault::getInstance()->setStringForKey(String::createWithFormat("CHOUMA_%d", k)->getCString(),tempStr);
		}	
	}	
}

void XingYunGambleLayer::toCancel(Object* obj)
{
	playButtonSound();

	Menu* tempMenu = (Menu *)this->getChildByTag(9527);
	if(tempMenu == nullptr) return;
	MenuItemSprite *tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9528);
	if(tempItem1) tempItem1->setVisible(true);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9529);
	if(tempItem1) tempItem1->setVisible(true);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9530);
	if(tempItem1) tempItem1->setVisible(false);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9531);
	if(tempItem1) tempItem1->setVisible(false);

	//放开
	for(int k = 0;k < 5;k ++)
	{		
		//编辑
		EditBox* dtChouMa = (EditBox* )this->getChildByTag(CHOUMA_BOX + k);
		if (dtChouMa)
		{
			dtChouMa->setEnabled(false);			
		}	
	}
}

void XingYunGambleLayer::calculateCQSSCQiShu()
{

}

void XingYunGambleLayer::onEnter()
{
	BaseLayer::onEnter();
}

void XingYunGambleLayer::onExit()
{
	BaseLayer::onExit();
}

void XingYunGambleLayer::back(Object *obj)
{
	playButtonSound();

	MyBetNumber::getInstance()->clear();

	RoomLayer* layer = RoomLayer::create();
	layer->automaticLogin(false);
	Scene* scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void XingYunGambleLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void XingYunGambleLayer::GetLastYueRet(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	Double *f_t_yue = (Double *)dic->objectForKey("f_t_yue");
	Double *f_t_dongjie = (Double *)dic->objectForKey("f_t_dongjie");
	Double *f_t_score = (Double *)dic->objectForKey("f_t_score");
	ZhangHuYuE =f_t_yue->getValue();

	updateMoneyLabel();
}

//------------------------------------------------新增----------------------------------------------------------------
void XingYunGambleLayer::editBoxEditingDidEnd(EditBox* editBox)
{
	string str = editBox->getText();
	beiShu = atoi(str.c_str());

	if(beiShu < 0)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t169"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		beilvInput->setText("0");
		beiShu = 0;
	}

	updateMoneyLabel();
}

/**触摸开始*/
bool XingYunGambleLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{	
	Vec2 pos = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	startPoint = pos;

	for(int k = 0;k < 5;k ++)
	{
		Sprite* sptChouMa = (Sprite *)this->getChildByTag(CHOUMA_BUTTON + k);
		if(sptChouMa)
		{
			if(sptChouMa->getBoundingBox().containsPoint(pos))
			{
				EditBox* tempBox = (EditBox *)this->getChildByTag(CHOUMA_BOX + k);
				if(tempBox)
				{
					if(tempBox->isEnabled()) break;//正在编辑则退出

					int tempInt = atoi(tempBox->getText());
					if(beilvInput)
					{
						tempInt += atoi(beilvInput->getText());
						beiShu = tempInt;
						beilvInput->setText(String::createWithFormat("%d",tempInt)->getCString());
						updateMoneyLabel();
						break;
					}
				}
			}
		}
	}	
	
	return true;
}

void XingYunGambleLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void XingYunGambleLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{	

}

void XingYunGambleLayer::updateMoneyLabel()
{	
	TouZhuJinE = beiShu;
	//显示投注金额，账户余额

	char szBuf1[100] = { 0 };
	char szBuf2[100] = { 0 };
	sprintf(szBuf1, "%.0f ", TouZhuJinE);
	sprintf(szBuf2, "%.3f ", ZhangHuYuE);

	String *TouZhuJinEString = String::createWithFormat("%s%s%s  %s %s%s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t14"), szBuf1,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"),
		ConfigMgr::instance()->text("display_DuangDong.xml", "t3"), szBuf2,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));

	TouZhuJinELabel->setString(TouZhuJinEString->getCString());	
}