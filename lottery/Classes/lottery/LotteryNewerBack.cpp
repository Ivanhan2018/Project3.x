#include "LotteryNewerBack.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "LotteryActiveCenter.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryNewerBack::scene()
{
	Scene *scene = Scene::create();
	LotteryNewerBack *layer = LotteryNewerBack::create();
	scene->addChild(layer);
	return scene;
}

LotteryNewerBack::LotteryNewerBack()
{
	//用的签到拼音，但这里并不是签到，是新人充值送
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryNewerBack::qianDaoRet), "QianDaoRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryNewerBack::qianDaoInfoRet), "QianDaoInfoRet", NULL);
}
LotteryNewerBack::~LotteryNewerBack()
{
	NotificationCenter::getInstance()->removeObserver(this,"QianDaoRet");
	NotificationCenter::getInstance()->removeObserver(this,"QianDaoInfoRet");
}

bool LotteryNewerBack::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);

	EntityMgr::instance()->getDispatch()->SendPacketWithDailiSignUpInfo();

	return true;
}

void LotteryNewerBack::initData()
{
	winSize = Director::getInstance()->getWinSize();
}

void LotteryNewerBack::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryNewerBack::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	//新人充值送
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_config.xml", "t5030"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	//
	//背景图
	Sprite* sptBody = Sprite::createWithSpriteFrame(spriteFrame("hd_04.png"));
	sptBody->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	sptBody->setPosition(Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 162));
	this->addChild(sptBody);

	fontSize = 16;
	float deltaHeight = 155;
	float deltaWordHeight = 10;
	float deltaWordWidth = 4;

	sptBody->setPositionY(sptBody->getPositionY() + 35);

	//
	Texture2D* tdStatus = TextureCache::getInstance()->addImage("hd_11.png");
	Texture2D* tdGold = TextureCache::getInstance()->addImage("hd_10.png");
	for(int i = 0;i < 9;i ++)
	{
		//每一项的背景
		ui::Button* btnBack = ui::Button::create("hd_09.png","hd_09.png","hd_09.png");
		btnBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		btnBack->setTag(9527 + i);
		btnBack->setPosition(Vec2(SCREEN_WIDTH / 4 + 8 + (i % 2)*(SCREEN_WIDTH  / 2 - 15),SCREEN_HEIGHT / 2 + 20 + btnBack->getContentSize().height / 2 - (btnBack->getContentSize().height + 12) * (i / 2) + deltaHeight));
		this->addChild(btnBack, 1);
		btnBack->setEnabled(false);

		//奖金
		Sprite* sptBack = Sprite::createWithTexture(tdGold,cocos2d::Rect(101 * i,0,101,82));
		sptBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptBack->setPosition(Vec2(btnBack->getPositionX() + 110,btnBack->getPositionY() + 20));
		this->addChild(sptBack, 2);

		//新注册用户 以它为基准
		LabelTTF* ltfUser = LabelTTF::create("", "", fontSize);
		ltfUser->setPosition(Vec2(90 + deltaWordWidth, btnBack->getContentSize().height / 2 + 22 - deltaWordHeight));
		ltfUser->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		ltfUser->setColor(ccc3(255,255,255));
		ltfUser->setTag(8527 + i);
		ltfUser->setHorizontalAlignment(TextHAlignment::LEFT);
		btnBack->addChild(ltfUser, 1);
		//投注量
		LabelTTF* ltfTouzhu = LabelTTF::create("", "", fontSize);
		ltfTouzhu->setPosition(Vec2(ltfUser->getPositionX() - 20 + deltaWordWidth, btnBack->getContentSize().height / 2 - deltaWordHeight));
		ltfTouzhu->setColor(ccc3(255,255,255));
		ltfTouzhu->setTag(7527 + i);
		ltfTouzhu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		ltfTouzhu->setHorizontalAlignment(TextHAlignment::LEFT);
		btnBack->addChild(ltfTouzhu, 1);
		//充值量
		LabelTTF* ltfRecharge = LabelTTF::create("", "", fontSize);
		ltfRecharge->setPosition(Vec2(ltfUser->getPositionX() - 20 + deltaWordWidth, btnBack->getContentSize().height / 2 - 22 - deltaWordHeight));
		ltfRecharge->setColor(ccc3(255,255,255));
		ltfRecharge->setTag(6527 + i);
		ltfRecharge->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		ltfRecharge->setHorizontalAlignment(TextHAlignment::LEFT);
		btnBack->addChild(ltfRecharge, 1);
		//状态栏
		Sprite* sptStatus = Sprite::createWithTexture(tdStatus,cocos2d::Rect(76,0,76,23));
		sptStatus->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptStatus->setTag(5527 + i);
		sptStatus->setPosition(Vec2(btnBack->getContentSize().width - 50 - 5,22));
		btnBack->addChild(sptStatus, 2);
	}
}

void LotteryNewerBack::onEnter()
{
	Layer::onEnter();
}

void LotteryNewerBack::onExit()
{
	Layer::onExit();
}

void LotteryNewerBack::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryNewerBack::back(Object *obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	LotteryActiveCenter *layer = LotteryActiveCenter::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void LotteryNewerBack::qianDaoRet(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;

	Integer* I_Result = (Integer *)data->objectForKey("n_t_state");
	Double* D_Jine = (Double *)data->objectForKey("f_t_jine");
	if(I_Result == nullptr) return;

	String* Str_Display = NULL;
	if(I_Result->getValue() == 0)
	{
		Str_Display = String::createWithFormat("%s %.02f %s",
			ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d", 1080 + I_Result->getValue())->getCString()),
			D_Jine->getValue(),
			ConfigMgr::instance()->text("display_text.xml","t4571")
			); 
	}else
	{
		Str_Display = String::createWithFormat("%s",ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d", 1080 + I_Result->getValue())->getCString())); 
	}
	//显示信息
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(Str_Display->getCString(),
		Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
	Director::getInstance()->getRunningScene()->addChild(layer, 255);

	//重发消息
	EntityMgr::instance()->getDispatch()->SendPacketWithDailiSignUpInfo();
}

void LotteryNewerBack::qianDaoInfoRet(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;

	Integer* I_Reg = (Integer *)data->objectForKey("cbNewRegUser");
	Double* D_Touzhu = (Double *)data->objectForKey("f_t_touzhu");
	Double* D_Chongzhi = (Double *)data->objectForKey("f_t_chongzhi");	

	const float tzArray[] = {488.0f,988.0f,1988.0f,5800.0f,8800.0f,9888.0f,14888.0f,16888.0f,22888.0f};
	const float max_count = 100.0f;
	//
	int i_PosLingjiang = -1; 
	for(int max_i = 8;max_i > -1;max_i --)
	{
		if(tzArray[max_i] < D_Touzhu->getValue()) i_PosLingjiang = max_i;
	}
	//刷新界面1-9级
	for(int i = 0;i < 9;i ++)
	{
		int status = 0;
		
		if(i == i_PosLingjiang)
		{
			status = 1;//签到
		}else
		{
			status = 0; //未达标
		}
		//获得节点
		ui::Button* btnBack = (ui::Button *)this->getChildByTag(9527 + i);
		if(btnBack == nullptr) continue;
		//注册用户
		LabelTTF* ltfReg = (LabelTTF *)btnBack->getChildByTag(8527 + i);
		if(ltfReg)
		{
			ltfReg->setString(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d", 936 + I_Reg->getValue() % 2)->getCString()));
		}
		//投注量
		LabelTTF* ltfTouzhu = (LabelTTF *)btnBack->getChildByTag(7527 + i);
		if(ltfTouzhu)
		{
			ltfTouzhu->setString(String::createWithFormat("%.02f/%.02f",D_Touzhu->getValue(),tzArray[i])->getCString());
		}
		//充值量
		LabelTTF* ltfChongzhi = (LabelTTF *)btnBack->getChildByTag(6527 + i);
		if(ltfChongzhi)
		{
			ltfChongzhi->setString(String::createWithFormat("%.02f/%.02f",D_Chongzhi->getValue(),max_count)->getCString());
		}
		//可以签到		
		if(status == 1) 
		{
			btnBack->setEnabled(true);
			btnBack->addClickEventListener([=](Ref *){
				//发送签到消息
				EntityMgr::instance()->getDispatch()->SendPacketWithDailiSignUp();
			});
		}
		Sprite* sptBackBottom = (Sprite *)btnBack->getChildByTag(5527 + i);
		if (sptBackBottom == nullptr) continue;

		//主要是修改下sprite		
		sptBackBottom->setTextureRect(cocos2d::Rect(76 * status, 0, 76, 23));
	}
}

void LotteryNewerBack::chongZhiSongGetRet(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;

	Integer* I_Result = (Integer *)data->objectForKey("n_t_state");
	Double* D_Jine = (Double *)data->objectForKey("f_t_jine");
	if(I_Result == nullptr) return;

	Sprite* sptBackBottom = (Sprite *)this->getChildByTag(9527);
	if (sptBackBottom == nullptr) return;

	Texture2D* tdGet = TextureCache::getInstance()->addImage("hd_12.png");

	if(I_Result->getValue() == 0)
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [=](Touch *touch, Event *event){

			auto sptButton = (Sprite *)this->getChildByTag(9527);
			if (sptButton == nullptr) return true;
			if(sptButton->getBoundingBox().containsPoint(touch->getLocation()))
			{
				EntityMgr::instance()->getDispatch()->SendPacketWithChongZhiSongSet();
			}
			return true;					
		};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		//换图
		sptBackBottom->setTextureRect(cocos2d::Rect(213,0,213,61));
	}else
	if(I_Result->getValue() == 2)
	{
		//主要是修改下sprite		
		sptBackBottom->setTextureRect(cocos2d::Rect(213 * 2,0,213,61));
	}else
	{
		sptBackBottom->setTextureRect(cocos2d::Rect(0,0,213,61));
	}
}

void LotteryNewerBack::chongZhiSongSetRet(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;

	Integer* I_Result = (Integer *)data->objectForKey("n_t_state");
	Double* D_Jine = (Double *)data->objectForKey("f_t_jine");	

	String* Str_Display = NULL;
	if(I_Result->getValue() == 4)
	{
		Str_Display = String::createWithFormat("%s %.02f %s",
			ConfigMgr::instance()->text("display_DuangDong.xml","t1563"),
			D_Jine->getValue(),
			ConfigMgr::instance()->text("display_text.xml","t4571")
			); 
	}else
	{
		Str_Display = String::createWithFormat("%s",ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d", 1559 + I_Result->getValue())->getCString())); 
	}
	//显示信息
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(Str_Display->getCString(),	Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
	Director::getInstance()->getRunningScene()->addChild(layer, 255);
}