#include "LotteryUpdate.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "LotterySettingView.h"
#include "MyNSString.h"

LotteryUpdate::LotteryUpdate()
{
	
}
LotteryUpdate::~LotteryUpdate()
{
	
}

void LotteryUpdate::showModal(Node* nowLayer,std::string updateUrl)
{
	//初始化界面
	LayerColor* layer = LayerColor::create(ccc4(200, 200, 200, 200), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setOpacity(125);
	layer->setPosition(Vec2(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2));
	
	//禁掉点击事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){return true;};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layer);

	//禁掉返回键
	auto listener_back = EventListenerKeyboard::create();
	listener_back->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){
		if(keycode == EventKeyboard::KeyCode::KEY_BACK)
		{
			event->stopPropagation();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_back, layer);

	//背景图
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame("beting46.png"));
	bk1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bk1->setPosition(Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50));
	layer->addChild(bk1,1);

	//系统提示
	LabelTTF* ltfTitle = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1100"), "", 42);
	ltfTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ltfTitle->setPosition(Vec2(bk1->getContentSize().width / 2 + 25,bk1->getContentSize().height - 42));
	ltfTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfTitle->setFontFillColor(ccc3(255,255,255));
	bk1->addChild(ltfTitle);

	//前图
	Sprite *sptTips = Sprite::createWithSpriteFrame(spriteFrame("setting_07_2.png"));
	sptTips->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptTips->setPosition(Vec2(ltfTitle->getPositionX() - ltfTitle->getContentSize().width + 40, ltfTitle->getPositionY()));
	bk1->addChild(sptTips);

	//提示内容
	LabelTTF* ltfContent = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1101"), "", 30);
	ltfContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ltfContent->setFontFillColor(ccc3(100,100,100));
	ltfContent->setPosition(Vec2(bk1->getContentSize().width / 2,bk1->getContentSize().height / 2 + 20));
	ltfContent->setHorizontalAlignment(TextHAlignment::CENTER);
	bk1->addChild(ltfContent);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_24.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_24_2.png"));
	Sprite *pJumpNormalButton = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_23.png"));
	Sprite *pJumpSelectButton = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_23_2.png"));
	//继续
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create(pCloseNormalButton,pCloseSelectButton,[=](Ref* ){
		playButtonSound();
		layer->removeFromParent();
	});
	//跳转
	MenuItemSprite *pJumpItemSprite = MenuItemSprite::create(pJumpNormalButton, pJumpSelectButton, [=](Ref* ){
		playButtonSound();
		string url = ConfigMgr::instance()->text("display_config.xml", "t5020") + updateUrl;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		TCHART w_url[100] = {0};
		MyNSString::mbs2wc(url.c_str(), strlen(url.c_str()), w_url);
		ShellExecute(NULL, L"open", w_url, NULL, NULL, SW_SHOWNORMAL);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		Application::getInstance()->openURL(url.c_str());
		exit(0);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Application::getInstance()->openURL(url.c_str());
		exit(0);
#endif
	});

	Menu *pCloseButton = Menu::create(pCloseItemSprite,pJumpItemSprite,NULL);
	pCloseButton->alignItemsHorizontallyWithPadding(100);
	pCloseButton->setPosition(Vec2(SCREEN_WIDTH / 2 - 50, 70));
	bk1->addChild(pCloseButton,1);

	nowLayer->addChild(layer, 255);
}

void LotteryUpdate::showPopWndUpdate(Node* nowLayer,int idx)
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	float x = winSize.width;
	float y = winSize.height;
	CCLayer* layer=CCLayer::create();
	nowLayer->addChild(layer,2,20161202);

	//禁掉点击事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){return true;};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layer);

	//禁掉返回键
	auto listener_back = EventListenerKeyboard::create();
	listener_back->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){
		event->stopPropagation();
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_back, layer);

	//Back
	CCSprite* sprite_back = CCSprite::create("beting46.png");
	//sprite_back->setPosition(ccp(x/2,y/2));
	//sprite_back->setScaleX(x*0.7/480);
	//sprite_back->setScaleY(y*0.7/320);
	sprite_back->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sprite_back->setPosition(Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50));
	layer->addChild(sprite_back);

	//相对Back的位置
	float x_layer = sprite_back->getPositionX();
	float y_layer = sprite_back->getPositionY();

	//系统提示——资源更新提示
	LabelTTF* ltfTitle = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1100"), "", 42);
	ltfTitle->setPosition(Vec2(x_layer,y_layer+sprite_back->getContentSize().height/2-40));
	ltfTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfTitle->setFontFillColor(ccc3(128,128,0));
	layer->addChild(ltfTitle);

	//标签1
	CCLabelTTF* pLabel_1 = CCLabelTTF::create("No Found resource !","",x*20.f/480); 
	pLabel_1->setPosition(CCPoint(x_layer,y_layer+x*30/480));
	pLabel_1->setColor(ccc3(128,128,0));
	layer->addChild(pLabel_1,1);
	//标签2
	CCLabelTTF* pLabel_2 = CCLabelTTF::create("DownLoad ?","",x*20.f/480); 
	pLabel_2->setPosition(CCPoint(x_layer,y_layer));
	pLabel_2->setColor(ccc3(128,128,0));
	layer->addChild(pLabel_2,1);

	//马上更新（下载）
	MenuItemImage *pImagItem_1 = MenuItemImage::create("my_lottery_23.png","my_lottery_23_2.png",[=](Ref* ){
		playButtonSound();
		layer->removeFromParent();
		// 通知更新下载
		//NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATEDOWNLOAD,Integer::create(idx));
	});
	//下次更新（取消）
	MenuItemImage *pImagItem_2 = MenuItemImage::create("my_lottery_24.png","my_lottery_24_2.png",[=](Ref* ){
		playButtonSound();
		layer->removeFromParent();
	});
	CCMenu* pMenu_down= CCMenu::create(pImagItem_1,pImagItem_2,NULL);
	pMenu_down->alignItemsHorizontallyWithPadding(100);
	pMenu_down->setPosition(Vec2(x_layer,y_layer-sprite_back->getContentSize().height/2+70));
	layer->addChild(pMenu_down,1,1);
}