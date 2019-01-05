#include "DDZSceneTableInterfaceLayer.h"

DDZSceneTableInterfaceLayer::DDZSceneTableInterfaceLayer()
{

}

DDZSceneTableInterfaceLayer::~DDZSceneTableInterfaceLayer()
{

}

cocos2d::Scene* DDZSceneTableInterfaceLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create(); 
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		DDZSceneTableInterfaceLayer *layer = DDZSceneTableInterfaceLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool DDZSceneTableInterfaceLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	cocos2d::Size WinSize = Director::getInstance()->getWinSize();

	setKeypadEnabled(true);
	LayerColor* colorLayer = LayerColor::create(ccc4(166, 166, 166, 100), WinSize.width, WinSize.height);
	addChild(colorLayer);
	Sprite *loadingTableInter = Sprite::create("background/bg2.png");
	loadingTableInter->setPosition(ccp(WinSize.width/2,WinSize.height/2));
	addChild(loadingTableInter);
	return true;
}

void DDZSceneTableInterfaceLayer::claosre( Object *obj )
{

}

//void DDZSceneTableInterfaceLayer::keyBackClicked( void )
//{
//
//}
