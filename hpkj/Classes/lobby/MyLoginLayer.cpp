#include "MyLoginLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UIHelper.h"
#include "ui/UIButton.h"
#include "SysFunc.h"
#include "RoomLayer.h"
#include "MyRoomLayer.h"
#include "ConfigMgr.h"
#include "GroupSprite.h"
#include "RegistLayer.h"
#include "ModifyPassWord.h"
#include "MovingLabelLayer.h"
#include "RoomOption.h"
#include "PromptBox.h"
//#include "LotterySceneControl.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
using namespace cocos2d::ui;

USING_NS_CC;

using namespace cocostudio::timeline;
MyLoginLayer* MyLoginLayer::LOGINSCENE = NULL;
Scene* MyLoginLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MyLoginLayer::create();
	LOGINSCENE = layer;
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MyLoginLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Widget* Root = static_cast<Widget*>(CSLoader::createNode("Login.csb"));
	addChild(Root);

	MyLoginLayerRoot = static_cast<Widget*>(Root->getChildByName("ditu"));

	Size size = WINSIZE;
	Root->setScale(size.width / MyLoginLayerRoot->getContentSize().width, size.height / MyLoginLayerRoot->getContentSize().height);
	
	CCSprite* GonggaoLayer = static_cast<CCSprite*>(MyLoginLayerRoot->getChildByName("gonggao"));
	GonggaoLayer->setVisible(false);
	cocos2d::ui::Button* pButton_2 = static_cast<cocos2d::ui::Button*>(GonggaoLayer->getChildByName("Button_2"));
	cocos2d::ui::Button* pButton_3 = static_cast<cocos2d::ui::Button*>(GonggaoLayer->getChildByName("Button_3"));
	pButton_2->addTouchEventListener(CC_CALLBACK_2(MyLoginLayer::OnCloseBulletinBoard, this));
	pButton_3->addTouchEventListener(CC_CALLBACK_2(MyLoginLayer::OnCloseBulletinBoard, this));

	cocos2d::Node* pImgNode = GonggaoLayer->getChildByName("Image_2");
	cocos2d::ui::Text* pText = static_cast<cocos2d::ui::Text*>(pImgNode->getChildByName("Text_1"));
	pText->setString("");

	Button* btn = static_cast<Button*>(MyLoginLayerRoot->getChildByName("Button_1"));

	btn->addTouchEventListener(CC_CALLBACK_2(MyLoginLayer::MyLoginLayerGame, this));
	MyLoginLayerRoot->getChildByName("Loading")->setVisible(false);

	TextField* text = (TextField*)(MyLoginLayerRoot->getChildByName("TextField_1"));
	text->setScale(1.5,1.5);
	text->setPosition(Vec2(text->getPositionX(), text->getPositionY() - 10));
	text->setVisible(true);

	return true;
}

void MyLoginLayer::MyLoginLayerGame(Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{

	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		MyLoginLayerRoot->getChildByName("Loading")->setVisible(true);
		TextField* text = (TextField*)(MyLoginLayerRoot->getChildByName("TextField_1"));
		String str = text->getString().c_str();
		CCLOG("////...............................%s", text->getString().c_str());
		CCLOG("%d", str.length());

		if (str.length() <= 0)
		{
			MyLoginLayerRoot->getChildByName("Loading")->setVisible(false);
			return;
		}


		MyLoginLayerRoot->getChildByName("Loading")->setVisible(true);

		onAnsLoginFinish(nullptr);
	}

}

void MyLoginLayer::OnCloseBulletinBoard(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CCSprite* GonggaoLayer = static_cast<CCSprite*>(MyLoginLayerRoot->getChildByName("gonggao"));
	GonggaoLayer->setVisible(false);
}

void MyLoginLayer::OnGetBullet(const std::string& strBullet)
{

}

void MyLoginLayer::onAnsLoginFinish(Ref* obj)
{
	MyRoomLayer* layer = MyRoomLayer::create();
	Scene* scene = Scene::create();
	scene->addChild(layer);
	///Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}
