#include "SceneTableInterfaceLayer.h"

SceneTableInterfaceLayer::SceneTableInterfaceLayer()
{

}

SceneTableInterfaceLayer::~SceneTableInterfaceLayer()
{

}

cocos2d::Scene* SceneTableInterfaceLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create(); 
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		SceneTableInterfaceLayer *layer = SceneTableInterfaceLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool SceneTableInterfaceLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	cocos2d::Size WinSize = Director::getInstance()->getWinSize();

	this->setKeypadEnabled(true);
	LayerColor* colorLayer = LayerColor::create(ccc4(166, 166, 166, 100), WinSize.width, WinSize.height);
	this->addChild(colorLayer);
	Sprite *loadingTableInter = Sprite::create("background/bg2.png");
	loadingTableInter->setPosition(ccp(WinSize.width/2,WinSize.height/2));
	this->addChild(loadingTableInter);
	return true;
}

void SceneTableInterfaceLayer::claosre( Object *obj )
{

}

void SceneTableInterfaceLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{

}
