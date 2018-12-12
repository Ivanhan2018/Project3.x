#include "DialogLayer.h"
#include "CMD_BJL.h"
#include "BJLSceneControl.h"
#include "SceneView.h"

DialogLayer::DialogLayer(dialog_type type)
{
	m_type = type;
}

DialogLayer::~DialogLayer()
{
}

DialogLayer * DialogLayer::create(dialog_type type)
{
	DialogLayer * pRet = new DialogLayer(type);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool DialogLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(DialogLayer::onTouchBegan,this);
	listener->onTouchEnded = CC_CALLBACK_2(DialogLayer::onTouchEnded,this);
	listener->onTouchMoved = CC_CALLBACK_2(DialogLayer::onTouchMoved,this);
	listener->onTouchCancelled = CC_CALLBACK_2(DialogLayer::onTouchCancelled,this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	switch(m_type)
	{
	case dialog_hints:
		initTable("socketofboxstring",1);
		break;
	case dialog_back:
		initTable("backtodt",2);
		break;
	case dialog_poChan:
		initTable("pochan",1);
		break;
	case dialog_poChanSongCoin:
		initTable("pochansongcoin",1);
		break;
	case dialog_dismissTable:
		initTable("dismisstable",1);
		break;
	case dialog_roomLimitMax:
		initTable("roomLimitMax",1);
		break;
	case dialog_roomLimitMin:
		initTable("roomLimitMin",2);
		break;
	}
	return true;
}

bool DialogLayer::onTouchBegan(Touch *pTouch,Event *pEvent)
{
	return true;
}

void DialogLayer::menuOkCallBack(Object * pSender)
{
	switch(m_type)
	{
	case dialog_back:
	case dialog_poChan:
	case dialog_hints:
	case dialog_dismissTable:
		{
			Director::getInstance()->replaceScene(BJLSceneControl::sharedSceneControl()->getScene(SCENE_start,false));
		}
		break;
	case dialog_roomLimitMax:
		{
			BJLSceneControl::sharedSceneControl()->getScene(SCENE_endAgain,false);					//--直接排队
		}
		break;
	case dialog_roomLimitMin:
		{
			NotificationCenter::getInstance()->postNotification(MESSAGE_JNICONTROL,Integer::create(MSG_JNI_Recharge));//充值
			BJLSceneControl::sharedSceneControl()->getScene(SCENE_endAgain,false);					//--直接排队
		}
		break;
	case dialog_poChanSongCoin:
		{
			Director::getInstance()->replaceScene(BJLSceneControl::sharedSceneControl()->getScene(SCENE_start,false));		//--返回大厅
		}
		break;
	}
	getParent()->removeChild(this);
}

void DialogLayer::menuCancleCallBack(Object * pSender)
{
	switch(m_type)
	{
	case dialog_back:			//--不做处理
		break;
	case dialog_roomLimitMin:
		{
			BJLSceneControl::sharedSceneControl()->getScene(SCENE_endAgain,false);					//--直接排队
		}
		break;
	}
	getParent()->removeChild(this);
}

void DialogLayer::onEnter()
{
	Layer::onEnter();
}

void DialogLayer::onExit()
{
	Layer::onExit();
}

void DialogLayer::initTable( char* Name,int type)
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	LayerColor * colorLayer = LayerColor::create(ccc4(0,0,0,120));
	Sprite * bj = Sprite::createWithSpriteFrameName(texture_name::s_setup_bg);
	bj->setPosition(ccp(size.width/2,size.height/2));

	CCDictionary * strings = CCDictionary::createWithContentsOfFile("bjl/xml/strings.xml");
	LabelTTF *lableTTF = LabelTTF::create(((String*)strings->objectForKey(Name))->getCString(),
		"Helvetica-Bold",
		30,
		cocos2d::Size(480,90),
		kCCTextAlignmentCenter,
		kCCVerticalTextAlignmentTop);
	lableTTF->setPosition(ccp(size.width/2,size.height/2+50));
	lableTTF->setColor(ccc3(118,78,0));
	if (type==1)
	{
		Sprite * confirmNormalItem = Sprite::createWithSpriteFrameName(texture_name::s_confirm);
		Sprite * confirmSelectedItem = Sprite::createWithSpriteFrameName(texture_name::s_confirm1);
		confirmSelectedItem->setColor(ccc3(120,120,120));
		MenuItemSprite * itemOkSprite = MenuItemSprite::create(confirmNormalItem,confirmSelectedItem,CC_CALLBACK_1(DialogLayer::menuOkCallBack,this));
		itemOkSprite->setPosition(ccp(0,-80.0f));

		Menu * menu = Menu::create(itemOkSprite,NULL);
		colorLayer->addChild(bj);
		colorLayer->addChild(lableTTF);
		colorLayer->addChild(menu);
	}
	else if(type==2)
	{
			Sprite * confirmNormalItem = Sprite::createWithSpriteFrameName(texture_name::s_confirm);
			Sprite * confirmSelectedItem = Sprite::createWithSpriteFrameName(texture_name::s_confirm1);
			confirmSelectedItem->setColor(ccc3(120,120,120));
			MenuItemSprite * itemSprite = MenuItemSprite::create(confirmNormalItem,confirmSelectedItem,CC_CALLBACK_1(DialogLayer::menuOkCallBack,this));
			itemSprite->setPosition(ccp(-100.0f,-80.0f));

			Sprite * cancleNormalItem = Sprite::createWithSpriteFrameName(texture_name::s_btn_cancle);
			Sprite * cancleSelectedItem = Sprite::createWithSpriteFrameName(texture_name::s_btn_cancle1);
			cancleSelectedItem->setColor(ccc3(120,120,120));
			MenuItemSprite * itemCancleSprite = MenuItemSprite::create(cancleNormalItem,cancleSelectedItem,CC_CALLBACK_1(DialogLayer::menuCancleCallBack,this));
			itemCancleSprite->setPosition(ccp(100.0f,-80.0f));

			Menu * menu = Menu::create(itemSprite,itemCancleSprite,NULL);
			colorLayer->addChild(bj);
			colorLayer->addChild(lableTTF);
			colorLayer->addChild(menu);
	}
	this->addChild(colorLayer);
}
