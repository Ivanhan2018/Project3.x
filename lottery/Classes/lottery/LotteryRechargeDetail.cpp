#include "LotteryRechargeDetail.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "comm.h"
#include "EntityMgr.h"
#include "MyNSString.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryRechargeDetail::scene()
{
	Scene *scene = Scene::create();
	LotteryRechargeDetail *layer = LotteryRechargeDetail::create();
	scene->addChild(layer);
	return scene;
}

LotteryRechargeDetail::LotteryRechargeDetail()
{
	infoData = NULL;
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRechargeDetail::getChongzhiTypeRet), "GetChongzhiTypeRet", NULL);	
}
LotteryRechargeDetail::~LotteryRechargeDetail()
{
	if(infoData != NULL){
		infoData->release();
	}
	NotificationCenter::getInstance()->removeObserver(this, "GetChongzhiTypeRet");
}

void LotteryRechargeDetail::getChongzhiTypeRet(Object *obj)
{

}

bool LotteryRechargeDetail::init()
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

void LotteryRechargeDetail::initData()
{
	winSize = Director::getInstance()->getWinSize();
	blackColor = ccc3(0,0,0);
	redColor = ccc3(241,91,90);
	posX1 = 280;
	posX2 = 280;
	gapY = 80;
}

void LotteryRechargeDetail::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(245, 245, 245, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
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
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryRechargeDetail::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t602"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 27;
	LabelTTF *title1 = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t595"), "", fontSize);
	title1->setAnchorPoint(Vec2(0,0.5f));
	title1->setPosition(Vec2(20, SCREEN_HEIGHT - 180));
	title1->setColor(ccc3(171,71,71));
	this->addChild(title1, 1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, title1->getPositionY()-40));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	this->addChild(gap1,1);

	startY = gap1->getPositionY()-80;

	fontSize = 27;
	for(int i = 0 ; i < 4 ; i++){
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 603+i)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(posX1, startY-gapY*i));
		label->setColor(blackColor);
		this->addChild(label, 1);
	}

	for(int i = 0 ; i < 4 ; i++){
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 608+i)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(0,0.5f));
		label->setPosition(Vec2(posX2, startY-gapY*i));
		label->setColor(redColor);
		label->setHorizontalAlignment(kCCTextAlignmentLeft);
		label->setTag(DETAIL_INFO_LABEL_TAG+i);
		this->addChild(label, 1);
	}

	LabelTTF *hintLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t607"), "", fontSize);
	hintLabel->setAnchorPoint(Vec2(0,0.5f));
	hintLabel->setPosition(Vec2(10, startY-gapY*7));
	hintLabel->setColor(ccc3(171,71,71));
	hintLabel->setDimensions(cocos2d::Size(SCREEN_WIDTH, 400));
	hintLabel->setTag(DETAIL_INFO_LABEL_TAG + 4);
	hintLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(hintLabel, 1);


	Sprite *normalButton = Sprite::createWithSpriteFrame(spriteFrame("recharge_09.png"));
	Sprite *selectButton = Sprite::createWithSpriteFrame(spriteFrame("recharge_10.png"));
	MenuItemSprite *itemSprite = MenuItemSprite::create
		(normalButton,selectButton,CC_CALLBACK_1(LotteryRechargeDetail::openUrl,this));
	Menu *rechargeButton = Menu::create(itemSprite,NULL);
	rechargeButton->setPosition(Vec2(winSize.width / 2, hintLabel->getPositionY() - 300));
	this->addChild(rechargeButton,1);
}

void LotteryRechargeDetail::openUrl(Object *obj)
{
	playButtonSound();

	if(infoData == NULL) return;

	String *url=NULL;	
	Integer *ntypeid = (Integer*)infoData->objectForKey("n_t_typeid");

	//增加了参数：userid, ntypeid
	url = String::createWithFormat("%s?userid=%ld&tid=%ld", 
		((String *)infoData->objectForKey("s_t_web"))->getCString(),
		EntityMgr::instance()->getDispatch()->m_dwUserID,
		(ntypeid==NULL)? 0 : ntypeid->getValue());
	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	TCHART w_url[255] = {0};
	MyNSString::mbs2wc(url->getCString(), strlen(url->getCString()), w_url);
	ShellExecute(NULL, L"open", w_url, NULL, NULL, SW_SHOWNORMAL);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	Application::getInstance()->openURL(url->getCString());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	Application::getInstance()->openURL(url->getCString());
#endif
}

//返回上一级
void LotteryRechargeDetail::back(Object *obj)
{
	playButtonSound();
	Director::getInstance()->popScene();
}

//进入
void LotteryRechargeDetail::onEnter()
{
	Layer::onEnter();
}

//退出
void LotteryRechargeDetail::onExit()
{
	Layer::onExit();
}

void LotteryRechargeDetail::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryRechargeDetail::setBank(int id)
{
	bankID = id;
}

void LotteryRechargeDetail::setInfoData(Dictionary *dic)
{
	if(dic == NULL || dic->count() == 0){
		return;
	}
	infoData = Dictionary::createWithDictionary(dic);
	infoData->retain();
	
	for(int i = 0 ; i < 5 ; i++){
		LabelTTF *label = (LabelTTF *)this->getChildByTag(DETAIL_INFO_LABEL_TAG+i);
		if(i == 0){
			String *str = (String *)infoData->objectForKey("s_t_zhanghao");
			label->setString(str->getCString());
		}else if(i == 1){
			String *str = (String *)infoData->objectForKey("s_t_kaihuren");
			label->setString(str->getCString());
		}else if(i == 2){
			String *str = (String *)infoData->objectForKey("s_t_yinhang");
			label->setString(str->getCString());
		}else if(i == 3){
			String *str = String::createWithFormat("%ld",EntityMgr::instance()->getDispatch()->m_dwUserID);
			label->setString(str->getCString());
		}else if(i == 4)
		{
			String *str = (String *)infoData->objectForKey("s_t_desc");
			label->setString(str->getCString());
		}
		else if(i == 5)
		{
			Integer *ntypeid = (Integer*)infoData->objectForKey("n_t_typeid");
			String *StrType = String::createWithFormat("%d", ntypeid->getValue());
//			label->setString(StrType->getCString());
		}
	}
}