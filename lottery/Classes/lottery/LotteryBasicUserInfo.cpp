#include "LotteryBasicUserInfo.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "LotteryUserInfo.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryBasicUserInfo::scene()
{
	Scene *scene = Scene::create();
	LotteryBasicUserInfo *layer = LotteryBasicUserInfo::create();
	scene->addChild(layer);
	return scene;
}

LotteryBasicUserInfo::LotteryBasicUserInfo()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryBasicUserInfo::getUserInfoRet), "GetUserInfoRet", NULL);
}
LotteryBasicUserInfo::~LotteryBasicUserInfo()
{
	NotificationCenter::getInstance()->removeObserver(this, "GetUserInfoRet");
}

void LotteryBasicUserInfo::getUserInfoRet(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	String *s_t_qq = (String *)dic->objectForKey("s_t_qq");
	myQQLabel->setString(s_t_qq->getCString());
	String *s_t_sjqq = (String *)dic->objectForKey("s_t_sjqq");
	shangJiQQLabel->setString(s_t_sjqq->getCString());
	
	Double *f_t_yongjin = (Double *)dic->objectForKey("f_t_yongjin");
	Double *f_t_yongjinze = (Double *)dic->objectForKey("f_t_yongjinze");
	Double *f_t_kuisunyongjin = (Double *)dic->objectForKey("f_t_kuisunyongjin");
	Double *f_t_kuisunyongjinze = (Double *)dic->objectForKey("f_t_kuisunyongjinze");
	String *t_yj_time = (String *)dic->objectForKey("t_yj_time");
	String *t_ksyj_time = (String *)dic->objectForKey("t_ksyj_time");

	consumeMoney = f_t_yongjin->getValue();
	consumeTotalMoney = f_t_yongjinze->getValue();
	earnedMoney = f_t_kuisunyongjin->getValue();
	earnedTotalMoney = f_t_kuisunyongjinze->getValue();

	String *consumeStr = String::createWithFormat("%s %s %.3lf%s    %s %.3lf%s",
		t_yj_time->getCString(), ConfigMgr::instance()->text("display_text.xml","t696"), consumeMoney,
		ConfigMgr::instance()->text("display_text.xml","t162"), 
		ConfigMgr::instance()->text("display_text.xml","t697"), consumeTotalMoney,
		ConfigMgr::instance()->text("display_text.xml","t162"));
	if (consumeLabel)
	consumeLabel->setString(consumeStr->getCString());
		
	String *earnedStr = String::createWithFormat("%s %s %.3lf%s    %s %.3lf%s",
		t_ksyj_time->getCString(), ConfigMgr::instance()->text("display_text.xml","t698"), earnedMoney,
		ConfigMgr::instance()->text("display_text.xml","t162"), 
		ConfigMgr::instance()->text("display_text.xml","t699"), earnedTotalMoney,
		ConfigMgr::instance()->text("display_text.xml","t162"));
	if (consumeLabel)
	earnedLabel->setString(earnedStr->getCString());
}

bool LotteryBasicUserInfo::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	EntityMgr::instance()->getDispatch()->SendPacketWithGetUserInfo();
	return true;
}

void LotteryBasicUserInfo::refreshLayer()
{	
	EntityMgr::instance()->getDispatch()->SendPacketWithGetUserInfo();	
}

void LotteryBasicUserInfo::initData()
{
	winSize = Director::getInstance()->getWinSize();
	//username = g_GlobalUnits.getGolbalUserData()->szNickName;
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;
	posX1 = 150;
	posX2 = 180;
	gapY = 60;

	consumeMoney = 2;
	consumeTotalMoney = 16;
	earnedMoney = 8;
	earnedTotalMoney = 96;

	consumeLabel = NULL;
}

void LotteryBasicUserInfo::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//±≥æ∞
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryBasicUserInfo::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	//帐户基本信息
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t1111"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 27;
	ccColor3B color = ccc3(0,0,0);
	//帐户基本信息
	LabelTTF *subTitle = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t1111"), "", fontSize);
	subTitle->setAnchorPoint(Vec2(0,1));
	subTitle->setPosition(Vec2(20, SCREEN_HEIGHT - 170));
	subTitle->setColor(ccc3(171, 71, 71));
	subTitle->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(subTitle, 1);

	//分割线
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, subTitle->getPositionY()-60));
	this->addChild(gap1, 1);

	startY = gap1->getPositionY()-50;
	fontSize = 27;

	//帐户基本信息的文字
	for(int i = 0 ; i < 5 ; i++){

#ifdef VER_QIANBO
		if(i > 2) continue;
#endif

#ifdef VER_369
		if(i > 2) continue;
#endif
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 691+i)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(posX1, startY-gapY*i));
		label->setColor(blackColor);
		this->addChild(label, 1);
	}

	//增加的棋牌返点	
	LabelTTF *label = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t952"), "", fontSize);
	label->setAnchorPoint(Vec2(1,0.5f));
	label->setPosition(Vec2(posX1, startY-gapY*5));
	label->setColor(blackColor);
	this->addChild(label, 1);	
	
	//值的颜色
#ifdef VER_369
	blackColor = ccc3(50, 62, 123);
#endif

	//内容
	LabelTTF *userNameLabel = LabelTTF::create(username.c_str(), "", fontSize);
	userNameLabel->setAnchorPoint(Vec2(0,0.5f));
	userNameLabel->setPosition(Vec2(posX2, startY));
	userNameLabel->setColor(blackColor);
	this->addChild(userNameLabel, 1);

	String *userIDStr = String::createWithFormat("%ld", EntityMgr::instance()->getDispatch()->m_dwUserID);
	LabelTTF *userIDLabel = LabelTTF::create(userIDStr->getCString(), "", fontSize);
	userIDLabel->setAnchorPoint(Vec2(0,0.5f));
	userIDLabel->setPosition(Vec2(posX2, startY-gapY));
	userIDLabel->setColor(blackColor);
	this->addChild(userIDLabel, 1);
	DOUBLE fandian = (EntityMgr::instance()->getDispatch()->f_fandian)*100;
	char str[33];
	sprintf(str, "%.3lf", fandian);
	string userId1=string(str);
	string urlExtra2 = "%";
	string url;
	url = userId1+urlExtra2;
	LabelTTF *fanDianLabel = LabelTTF::create(url.c_str(), "", fontSize);
	fanDianLabel->setAnchorPoint(Vec2(0,0.5f));
	fanDianLabel->setPosition(Vec2(posX2, startY-gapY*2));
	fanDianLabel->setColor(blackColor);
	this->addChild(fanDianLabel, 1);

	Sprite *inputBk = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
	inputBk->setAnchorPoint(Vec2(0,0.5f));
	inputBk->setPosition(Vec2(posX2-5, startY-gapY*3));
	this->addChild(inputBk);
#ifdef VER_QIANBO
	inputBk->setVisible(false);
#endif

#ifdef VER_369
	inputBk->setVisible(false);
#endif

	myQQLabel = LabelTTF::create("123456789", "", fontSize);
	myQQLabel->setAnchorPoint(Vec2(0,0.5f));
	myQQLabel->setPosition(Vec2(posX2, startY-gapY*3));
	myQQLabel->setColor(ccc3(0,0,0));
	this->addChild(myQQLabel, 1);

	shangJiQQLabel = LabelTTF::create("454651", "", fontSize);
	shangJiQQLabel->setAnchorPoint(Vec2(0,0.5f));
	shangJiQQLabel->setPosition(Vec2(posX2, startY-gapY*4));
	shangJiQQLabel->setColor(blackColor);
	this->addChild(shangJiQQLabel, 1);

	char str1[33];
	sprintf(str1, "%.3lf", fandian * 20);
	string tss1=string(str1);
	string tss2 = "%";
	string tss;
	tss = tss1+tss2;
	LabelTTF *qipaifandianLabel = LabelTTF::create(tss.c_str(), "", fontSize);
	qipaifandianLabel->setAnchorPoint(Vec2(0,0.5f));
	qipaifandianLabel->setPosition(Vec2(posX2, startY-gapY*5));
	qipaifandianLabel->setColor(blackColor);
	this->addChild(qipaifandianLabel, 1);

#ifdef VER_QIANBO	
	qipaifandianLabel->setPositionY(myQQLabel->getPositionY());
	label->setPositionY(qipaifandianLabel->getPositionY());
	myQQLabel->setVisible(false);
	shangJiQQLabel->setVisible(false);
#endif // DEBUG

#ifdef VER_369	
	qipaifandianLabel->setPositionY(myQQLabel->getPositionY());
	label->setPositionY(qipaifandianLabel->getPositionY());
	myQQLabel->setVisible(false);
	shangJiQQLabel->setVisible(false);
#endif // DEBUG

	time_t t = time(0); 
	char timeStr[64] = {0}; 
	tm *tmLocal = localtime(&t);
	strftime(timeStr, sizeof(timeStr), "%Y-%m-%d",tmLocal); 
	//strftime(timeStr, sizeof(timeStr), "%Y-%m-%d",localtime(&t)); 
	fontSize = 22;

	//if (EntityMgr::instance()->getDispatch()->m_cbType > 0 && fandian > 2.5)
	if(false)
	{
		String *consumeStr = String::createWithFormat("%s %s %.3lf%s    %s %.3lf%s",
			timeStr, ConfigMgr::instance()->text("display_text.xml","t696"), consumeMoney,
			ConfigMgr::instance()->text("display_text.xml","t162"), 
			ConfigMgr::instance()->text("display_text.xml","t697"), consumeTotalMoney,
			ConfigMgr::instance()->text("display_text.xml","t162"));
		consumeLabel = LabelTTF::create(consumeStr->getCString(), "", fontSize);
		consumeLabel->setAnchorPoint(Vec2(0,0.5f));
		consumeLabel->setPosition(Vec2(20, startY-gapY*6));
		consumeLabel->setColor(ccc3(100,0,0));
		consumeLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
		this->addChild(consumeLabel, 1);

		String *earnedStr = String::createWithFormat("%s %s %.3lf%s    %s %.3lf%s",
			timeStr, ConfigMgr::instance()->text("display_text.xml","t698"), earnedMoney,
			ConfigMgr::instance()->text("display_text.xml","t162"), 
			ConfigMgr::instance()->text("display_text.xml","t699"), earnedTotalMoney,
			ConfigMgr::instance()->text("display_text.xml","t162"));
		earnedLabel = LabelTTF::create(earnedStr->getCString(), "", fontSize);
		earnedLabel->setAnchorPoint(Vec2(0,0.5f));
		earnedLabel->setPosition(Vec2(consumeLabel->getPositionX(), startY-gapY*7));
		earnedLabel->setColor(ccc3(100,0,0));
		earnedLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
		this->addChild(earnedLabel, 1);
	}
}

void LotteryBasicUserInfo::onEnter()
{
	Layer::onEnter();
}

void LotteryBasicUserInfo::onExit()
{
	Layer::onExit();
}

void LotteryBasicUserInfo::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryBasicUserInfo::back(Object *obj)
{
	playButtonSound();
	//Scene *scene = Scene::create();
	//LotteryUserInfo *layer = LotteryUserInfo::create();
	//scene->addChild(layer);
	//Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	Director::getInstance()->popScene();
}