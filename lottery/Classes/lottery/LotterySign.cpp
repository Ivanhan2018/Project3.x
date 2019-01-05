#include "LotterySign.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "VersionControl.h"
#include "LotteryActiveCenter.h"
#include "ui/UIImageView.h"

using namespace CocosDenshion;

Scene* LotterySign::scene()
{
	Scene *scene = Scene::create();
	LotterySign *layer = LotterySign::create();
	scene->addChild(layer);
	return scene;
}

LotterySign::LotterySign()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotterySign::signUpRet), "SignUpRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotterySign::huanLeSongRet), "HuanLeSongRet", NULL);
}

LotterySign::~LotterySign()
{
	NotificationCenter::getInstance()->removeObserver(this,"SignUpRet");
	NotificationCenter::getInstance()->removeObserver(this,"HuanLeSongRet");
}

bool LotterySign::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	//发消息
	EntityMgr::instance()->getDispatch()->SendPacketWithGetSignUpInfo();
	//EntityMgr::instance()->getDispatch()->SendPacketWithGetUserInfo();
	return true;
}

void LotterySign::initData()
{
	winSize = Director::getInstance()->getWinSize();
}

void LotterySign::initView()
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
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotterySign::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	//天天签到
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t50"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	//背景图
	Sprite* sptBody = Sprite::createWithSpriteFrame(spriteFrame("hd_01.png"));
	sptBody->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	sptBody->setPosition(Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 162));
	this->addChild(sptBody);

	float delta_content = 0;
	float delta_money = 0;
	float delta_content_item = 0;
	float delta_money_x1 = 0;
	float delta_money_x2 = 0;
	float delta_content_x = 0;

#ifdef VER_QIANBO
	delta_content = 240;
	delta_money = 190;
	delta_money_x1 = + SCREEN_WIDTH / 2 - 85;
	delta_money_x2 =  - SCREEN_WIDTH / 2 - 5;
	delta_content_item = 10;
	delta_content_x = 30;
#endif

#ifdef VER_369
	delta_content = 130;
	delta_money = 296;
	delta_money_x1 = 45;
	delta_money_x2 = 0;
	delta_content_item = 12;
	delta_content_x = 0;
#endif

	//当天消费额
	LabelTTF *ltfTodayFeed = LabelTTF::create("0", "", 24);
	ltfTodayFeed->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	ltfTodayFeed->setPosition(Vec2(SCREEN_WIDTH / 4 + delta_money_x1, SCREEN_HEIGHT / 4  + 127 + delta_money));
	ltfTodayFeed->setColor(ccc3(255,255,255));
	ltfTodayFeed->setTag(1000);
	ltfTodayFeed->setFontFillColor(ccColor3B::RED);
	ltfTodayFeed->setHorizontalAlignment(TextHAlignment::LEFT);
	this->addChild(ltfTodayFeed, 1);

	//总签到额
	LabelTTF *ltfTotalFeed = LabelTTF::create("0", "", 24);
	ltfTotalFeed->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	ltfTotalFeed->setPosition(Vec2(SCREEN_WIDTH * 3 / 4 - 15 + delta_money_x2, ltfTodayFeed->getPositionY()));
	ltfTotalFeed->setColor(ccc3(255,255,255));
	ltfTotalFeed->setTag(1001);
	ltfTotalFeed->setFontFillColor(ccColor3B::RED);
	ltfTotalFeed->setHorizontalAlignment(TextHAlignment::LEFT);
	this->addChild(ltfTotalFeed, 1);
	
	Texture2D *tdStatus = TextureCache::getInstance()->addImage("hd_08.png");

#ifdef VER_QIANBO
	Texture2D *tdDays = TextureCache::getInstance()->addImage("hd_07.png");
	//签到按钮
	for(int i = 0;i < 7;i ++)
	{
		//位置
		Vec2 pos = Vec2(delta_content_x + (34 - delta_content_item +103 / 2)*(i % 5 + 1) + 50 * (i % 5) - 50, SCREEN_HEIGHT / 2 + 30 + (100 / 2 + 25) * (i < 5 ? 1 : -1) - delta_content + 32);
		//上
		Sprite* sptBackTop = Sprite::createWithTexture(tdDays,cocos2d::Rect(i * 101,0,101,91));
		sptBackTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptBackTop->setPosition(Vec2(pos.x + 103 / 2, pos.y));
		this->addChild(sptBackTop);
		//下 
		Sprite* sptBackBottom = Sprite::createWithTexture(tdStatus, cocos2d::Rect(0, 0, 90, 29));
		sptBackBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptBackBottom->setPosition(Vec2(pos.x + 103 / 2, pos.y - 70));
		sptBackBottom->setTag(9527 + i); //可以按的
		this->addChild(sptBackBottom);
	}
#endif

#ifdef VER_9YI
	Texture2D *tdDays = TextureCache::getInstance()->addImage("hd_07.png");

	//签到按钮
	for(int i = 0;i < 7;i ++)
	{
		std::string strBtnName = "hd_06.png";
		if(i % 2 == 0)
		{	
			strBtnName.clear();
			strBtnName.append("hd_05.png");
		}
		//按钮
		ui::Button* btnBack = ui::Button::create(strBtnName,strBtnName,strBtnName);
		btnBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		btnBack->setPosition(Vec2(delta_content_x + (34 - delta_content_item + btnBack->getContentSize().width  / 2)*(i % 5 + 1) + 50 * (i % 5), SCREEN_HEIGHT / 2 + 30 + (btnBack->getContentSize().height / 2 + 20) * (i < 5 ? 1 : -1) - delta_content));
		btnBack->setTag(9527 + i);
		this->addChild(btnBack);
		btnBack->setEnabled(false);
		//上
		Sprite* sptBackTop = Sprite::createWithTexture(tdDays,cocos2d::Rect(i * 46,0,46,20));
		sptBackTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptBackTop->setPosition(Vec2(btnBack->getContentSize().width / 2, btnBack->getContentSize().height - 10));
		sptBackTop->setTag(8527 + i);
		btnBack->addChild(sptBackTop);
		//下 
		Sprite* sptBackBottom = Sprite::createWithTexture(tdStatus, cocos2d::Rect(0, 0, 83, 20));
		sptBackBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptBackBottom->setPosition(Vec2(btnBack->getContentSize().width / 2, 35));
		sptBackBottom->setTag(7527 + i);
		btnBack->addChild(sptBackBottom);
	}	
#endif

#ifdef VER_369
	Texture2D *tdDays = TextureCache::getInstance()->addImage("hd_05.png");	

	//签到按钮
	for(int i = 0;i < 7;i ++)
	{		
		//按钮
		//ui::ImageView* btnBack = ui::ImageView::create("active.png");	
		ui::Button* btnBack = ui::Button::create("active.png", "active.png", "active.png");
		btnBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		btnBack->setPosition(Vec2(delta_content_x + (34 - delta_content_item + btnBack->getContentSize().width  / 2)*(i % 5 + 1) + 50 * (i % 5), SCREEN_HEIGHT / 2 + 30 + (btnBack->getContentSize().height / 2 + 40) * (i < 5 ? 1 : -1) - delta_content));
		btnBack->setTag(9527 + i);
		this->addChild(btnBack);
		btnBack->setEnabled(false);		
		//上
		Sprite* sptBackTop = Sprite::createWithTexture(tdDays,cocos2d::Rect(126*i,0,126,92));
		sptBackTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptBackTop->setPosition(Vec2(btnBack->getContentSize().width / 2, btnBack->getContentSize().height / 2));
		sptBackTop->setTag(8527 + i);
		btnBack->addChild(sptBackTop);
		//下 
		Sprite* sptBackBottom = Sprite::createWithTexture(tdStatus, cocos2d::Rect(0, 0, 125, 42));
		sptBackBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptBackBottom->setPosition(Vec2(btnBack->getContentSize().width / 2, -sptBackBottom->getContentSize().height / 2));
		sptBackBottom->setTag(7527 + i);
		btnBack->addChild(sptBackBottom);
	}

#endif
}

void LotterySign::onEnter()
{
	Layer::onEnter();
}

void LotterySign::onExit()
{
	Layer::onExit();
}

void LotterySign::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotterySign::back(Object *obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	LotteryActiveCenter *layer = LotteryActiveCenter::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void LotterySign::signUpRet(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;

	Integer* I_Result = (Integer *)data->objectForKey("n_t_res");
	if(I_Result == nullptr) return;

	String* Str_Display = String::createWithFormat("t%d", 1050 + I_Result->getValue());
	//显示信息
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml",Str_Display->getCString()),
		Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
	Director::getInstance()->getRunningScene()->addChild(layer, 255);

	//重发消息
	EntityMgr::instance()->getDispatch()->SendPacketWithGetSignUpInfo();
}

void LotterySign::huanLeSongRet(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;

	Integer* I_Res = (Integer *)data->objectForKey("nRes");
	Double* D_XiaoFei = (Double *)data->objectForKey("f_t_xiaofei");
	Double* D_Prize = (Double *)data->objectForKey("f_t_prize");
	Integer* I_Count = (Integer *)data->objectForKey("n_t_count");
	Integer* I_XXCount = (Integer *)data->objectForKey("n_t_XXcount");
	//刷新界面
	LabelTTF* ltfTodayCost = (LabelTTF *)this->getChildByTag(1000);
	if(ltfTodayCost)
	{
		ltfTodayCost->setString(String::createWithFormat("%.03f",D_XiaoFei->getValue())->getCString());
	}

	LabelTTF* ltfTotalCost = (LabelTTF *)this->getChildByTag(1001);
	if(ltfTotalCost)
	{
		ltfTotalCost->setString(String::createWithFormat("%.03f",D_Prize->getValue())->getCString());
	}

#ifdef VER_9YI
	//刷新界面1-7天
	for(int i = 0;i < 7;i ++)
	{
		int status = 0;
		if(i < I_Count->getValue())
		{
			status = 2;//已签到
		}else
		if(i == I_Count->getValue() && I_Res->getValue() == 0)
		{
			status = 1;//签到  
		}else
		{
			status = 0; //未达标
		}
		//获得节点
		ui::Button* btnBack = (ui::Button *)this->getChildByTag(9527 + i);
		if(btnBack == nullptr) continue;
		//可以签到		
		if(status == 1) 
		{
			btnBack->setEnabled(true);
			btnBack->addClickEventListener([=](Ref *){
				//发送签到消息
				EntityMgr::instance()->getDispatch()->SendPacketWithSignUp();
			});
		}
		Sprite* sptBackBottom = (Sprite *)btnBack->getChildByTag(7527 + i);
		if (sptBackBottom == nullptr) continue;
		
		//主要是修改下sprite		
		sptBackBottom->setTextureRect(cocos2d::Rect(83 * status, 0, 83, 20));
	}
#endif

#ifdef VER_369
	//刷新界面1-7天
	for(int i = 0;i < 7;i ++)
	{
		int status = 0;
		if(i < I_Count->getValue())
		{
			status = 2;//已签到
		}else
			if(i == I_Count->getValue() && I_Res->getValue() == 0)
			{
				status = 1;//签到  
			}else
			{
				status = 0; //未达标
			}
			//获得节点
			ui::ImageView* btnBack = (ui::ImageView *)this->getChildByTag(9527 + i);
			if(btnBack == nullptr) continue;
			//可以签到		
			if(status == 1) 
			{
				btnBack->setEnabled(true);
				btnBack->addClickEventListener([=](Ref *){
					//发送签到消息
					EntityMgr::instance()->getDispatch()->SendPacketWithSignUp();
				});
			}
			Sprite* sptBackBottom = (Sprite *)btnBack->getChildByTag(7527 + i);
			if (sptBackBottom == nullptr) continue;

			//主要是修改下sprite		
			sptBackBottom->setTextureRect(cocos2d::Rect(125 * status, 0, 125, 42));
	}
#endif
}