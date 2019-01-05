#include "LotteryHelpView.h"
#include "RoomLayer.h"
#include "ChongQingGambleLayer.h"
#include "GuangDong11Xuan5Layer.h"
#include "LotterySceneControl.h"
#include "bjpk10Layer.h"
#include "FC3dpl3Layer.h"
#include "LotteryGambleLayer.h"
#include "XingYunGambleLayer.h"
#include "BJK8GambleLayer.h"
#include "LiuHeGambleLayer.h"
#include "VersionControl.h"

using namespace cocos2d;
LotteryHelpView::LotteryHelpView()
{

}

LotteryHelpView::~LotteryHelpView()
{

}

Scene* LotteryHelpView::scene()
{
	Scene * scene = NULL;
	do
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		LotteryHelpView *layer = LotteryHelpView::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool LotteryHelpView::init()
{
	bool bRet = false;
	do
	{
		if (!Layer::init())
		{
			return false;
		}

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(LotteryHelpView::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(LotteryHelpView::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(LotteryHelpView::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(LotteryHelpView::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		cocos2d::Size winSize = Director::getInstance()->getWinSize();

		m_node = Node::create();
		m_node->setPosition(Vec2(0,0));
		this->addChild(m_node, 1);		

		Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
		bk1->setAnchorPoint(Vec2(0,1));
		bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
		bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
		bk1->setScaleY(162 / bk1->getContentSize().height);
		this->addChild(bk1,1);

		//背景
		Sprite* otherbackground = Sprite::createWithSpriteFrame(spriteFrame("beark.png"));		
		otherbackground->setPosition(Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
		otherbackground->setScaleX(SCREEN_WIDTH / otherbackground->getContentSize().width);
		otherbackground->setScaleY(SCREEN_HEIGHT / otherbackground->getContentSize().height);
		this->addChild(otherbackground,0);	

		Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
		Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
		MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
			(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryHelpView::back,this));
		Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
		pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
		this->addChild(pCloseButton,1);

		float fontSize = 38;
		title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t140"), "", fontSize);
		title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
		title->setColor(ccc3(255,255,255));
		this->addChild(title, 1);		

		this->setTouchEnabled(true);
		this->setKeypadEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void LotteryHelpView::back(Object *obj)
{
	playButtonSound();
	
	if (helpId == CZChongQingSSC ||
		helpId == CZJiangXiSSC || helpId == CZXinJiangSSC ||
		helpId == CZ_FENFEN_CAI ||
		helpId == CZ_WUFEN_CAI || helpId == CZ_TIANJIN_SSC || 
		helpId == CZ_KOREA_FEN_CAI || helpId == CZ_JILI_2FEN_CAI ||
		helpId == CZ_TENCENT_FEN_CAI || helpId == CZ_QQ_FEN_CAI ||
		helpId == CZ_BEIJING_5FEN_CAI || helpId == CZ_CANADA_FEN_CAI)
		
	{
		ChongQingGambleLayer *layer = ChongQingGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind(helpId);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	} else
	if (helpId == CZGD11Xuan5 || helpId == CZSD11Xuan5 ||helpId == CZJX11Xuan5 || helpId == CZ_HLJ11Xuan5)
	{
		GuangDong11Xuan5Layer *layer = GuangDong11Xuan5Layer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind(helpId);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	} else
	if (helpId == CZ_BJPK10)
	{
		bjpk10Layer *layer = bjpk10Layer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind(helpId);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	} else
	if (helpId == CZ3D || helpId == CZPailie3)
	{
		FC3DPL3Layer *layer = FC3DPL3Layer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind(helpId);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	} else
	if (helpId == CZ_SEVENSTAR)
	{
		LotteryGambleLayer *layer = LotteryGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind(helpId);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	} else
	if( helpId == CZ_LUCKY_28)
	{
		XingYunGambleLayer *layer = XingYunGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);		
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	} else
		if( helpId == CZKUAILE8)
		{
			BJK8GambleLayer *layer = BJK8GambleLayer::create();
			Scene *scene = Scene::create();
			scene->addChild(layer);	
			layer->setGameKind(helpId);
			Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
		}
		else
			if( helpId == CZ_HK6HeCai)
			{
				LiuHeGambleLayer *layer = LiuHeGambleLayer::create();
				Scene *scene = Scene::create();
				scene->addChild(layer);	
				layer->setGameKind(helpId);
				Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			}
}

void LotteryHelpView::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

bool LotteryHelpView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	touchPoint = m_node->convertToNodeSpace(touchPoint);
	m_start = touchPoint.y;
	return true;
}
void LotteryHelpView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	touchPoint = m_node->convertToNodeSpace(touchPoint);
	m_end = touchPoint.y;
	if((m_end-m_start)<5&&(m_end-m_start)>-5){
		return;
	}
	m_node->setPosition(Vec2(m_node->getPositionX(),m_node->getPositionY()+m_end-m_start));
	if(m_node->getPositionY()<0){
		m_node->setPositionY(0);
	}
	if (m_node->getPositionY()>m_endPos.y)
	{
		m_node->setPositionY(m_endPos.y);
	}
	m_start = m_end;
}

void LotteryHelpView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	touchPoint = m_node->convertToNodeSpace(touchPoint);
}

void LotteryHelpView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

void LotteryHelpView::setHelpId(CaiZhong id)
{
	helpId = id;
	if (helpId == CZChongQingSSC || helpId == CZJiangXiSSC || helpId == CZXinJiangSSC || helpId == CZ_FENFEN_CAI || helpId == CZ_CANADA_FEN_CAI
		|| helpId == CZ_TENCENT_FEN_CAI || helpId == CZ_QQ_FEN_CAI
		|| helpId == CZ_WUFEN_CAI || helpId == CZ_TIANJIN_SSC || helpId == CZ_KOREA_FEN_CAI || helpId == CZ_JILI_2FEN_CAI || helpId == CZ_BEIJING_5FEN_CAI)
	{
		refreshContent("chongqin_help_%02d.png", 9, String::createWithFormat("chongqin_help_f%d.png",helpId)->getCString());
	}else
	if (helpId == CZGD11Xuan5 || helpId == CZSD11Xuan5 	|| helpId == CZJX11Xuan5 ||  helpId == CZ_HLJ11Xuan5)
	{
		refreshContent("11Xuan5_help_%02d.png", 4,"11Xuan5_help_01.png");
	}else
	if (helpId == CZ_BJPK10)			//北京快车
	{
		refreshContent("PK10_%02d.png", 4,"PK10_01.png");
	}else
	if (helpId == CZ3D || helpId == CZPailie3)
	{
		refreshContent("FC3D_%02d.png", 5,"FC3D_01.png");
	}else
	if(helpId == CZ_SEVENSTAR)
	{
		refreshContent("7x_help.png", 1,"7x_help.png");
	}else
	if(helpId == CZ_LUCKY_28)
	{
		refreshContent("xy28_%02d.png",6,"xy28_01.png");
	}else
	if(helpId == CZKUAILE8)
	{
		refreshContent("xy28_%02d.png",6,"xy28_01.png");
	}
	else
	if(helpId == CZ_HK6HeCai)
	{
		refreshContent("xy28_%02d.png",6,"xy28_01.png");
	}
}

void LotteryHelpView::refreshContent(const char* src, const int num,const char* firstSrc)
{
	Vec2 pt = Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150);
	if(num == 1)
	{	
		//底色
		auto sptBacks = Sprite::createWithSpriteFrame(spriteFrame("blank.png"));
		sptBacks->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptBacks->setPosition(Vec2(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2));
		sptBacks->setScaleX(SCREEN_WIDTH / sptBacks->getContentSize().width);
		sptBacks->setScaleY(SCREEN_HEIGHT / sptBacks->getContentSize().height);
		m_node->addChild(sptBacks, 0);
		
		background = Sprite::createWithSpriteFrame(spriteFrame(src));
		background->setAnchorPoint(Vec2(0,1));
		background->setPosition(Vec2(0, pt.y));
		m_node->addChild(background,1);				

		m_endPos = Vec2(pt.x,0);
	} else
	{
		for(int i = 1;i <= num;i ++)
		{
			Sprite* myBack = nullptr;
			if(i == 1)
			{
				myBack = Sprite::createWithSpriteFrame(spriteFrame(firstSrc));
			} else
			{
				myBack = Sprite::createWithSpriteFrame(spriteFrame(String::createWithFormat(src, i)->getCString()));
			}			
			myBack->setAnchorPoint(Vec2(0,1));
			myBack->setPosition(Vec2(0, pt.y));
			m_node->addChild(myBack,0);
			pt.y = pt.y - myBack->getContentSize().height;
		}
		m_endPos = Vec2(pt.x, -pt.y);
	}
}