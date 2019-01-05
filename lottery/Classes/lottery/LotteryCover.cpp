#include "LotteryCover.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "LotteryActiveCenter.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryCover::scene()
{
	Scene *scene = Scene::create();
	LotteryCover *layer = LotteryCover::create();
	scene->addChild(layer);
	return scene;
}

LotteryCover::LotteryCover()
{
	
}
LotteryCover::~LotteryCover()
{
	
}

bool LotteryCover::init()
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

void LotteryCover::initData()
{

}

void LotteryCover::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 125), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	this->addChild(layer);

	//添加触摸监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *touch, Event *pEvent){
		auto sptTop = (Sprite *)layer->getChildByTag(9527);
		if(sptTop == nullptr) return true;
		if(sptTop->getBoundingBox().containsPoint(touch->getLocation()))
		{
			this->removeFromParentAndCleanup(true);
		}
		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layer);
	//添加键盘监听
	auto listenerKeyBoard = EventListenerKeyboard::create();
	listenerKeyBoard->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){
		if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
		{			
			//禁掉返回键
			event->stopPropagation();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeyBoard, layer);

	//我知道了
	Sprite* sptTop = Sprite::createWithSpriteFrame(spriteFrame("t1_02A.png"));
	sptTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	sptTop->setPosition(Vec2(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 3));
	sptTop->setTag(9527);
	layer->addChild(sptTop);

	//此处可弹开看历史开奖
	Sprite* sptLeft = Sprite::createWithSpriteFrame(spriteFrame("t1_02B.png"));
	sptLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	sptLeft->setPosition(Vec2(30, SCREEN_HEIGHT * 5 / 6 - 90));
	layer->addChild(sptLeft);

	//背景图
	Sprite* sptRight = Sprite::createWithSpriteFrame(spriteFrame("t1_02C.png"));
	sptRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	sptRight->setPosition(Vec2(SCREEN_WIDTH / 2 + 75, SCREEN_HEIGHT * 5 / 6 - 130));
	layer->addChild(sptRight);

	//背景图
	Sprite* sptDown = Sprite::createWithSpriteFrame(spriteFrame("t1_02D.png"));
	sptDown->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	sptDown->setPosition(Vec2(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 6 - 76));
	layer->addChild(sptDown);

#ifdef VER_369
	sptDown->setPosition(sptDown->getPosition() + Vec2(18,5));
#endif
}

void LotteryCover::onEnter()
{
	Layer::onEnter();
}

void LotteryCover::onExit()
{
	Layer::onExit();
}

void LotteryCover::showModal(LayerColor* parent)
{
	bool isShowed = UserDefault::getInstance()->getBoolForKey("SHOWTIPS",false);
	if(isShowed) return;
	UserDefault::getInstance()->setBoolForKey("SHOWTIPS",true);

	LotteryCover* layer = LotteryCover::create();
	parent->addChild(layer, 255);
}

//滚动
MyScrollLayer::MyScrollLayer()
{

}

MyScrollLayer::~MyScrollLayer()
{

}

bool MyScrollLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	return true;
}

void MyScrollLayer::initData()
{

}

void MyScrollLayer::initView()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->onTouchBegan = CC_CALLBACK_2(MyScrollLayer::onTouchBegan, this);	
	listener->onTouchEnded = CC_CALLBACK_2(MyScrollLayer::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(MyScrollLayer::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(MyScrollLayer::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void MyScrollLayer::setEndPos(Vec2 pos)
{
	this->m_endPos.x = pos.x;
	this->m_endPos.y = pos.y;
}

bool MyScrollLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	//清空数组
	m_vecPot.clear();
	m_bDrag = false;

	return true;
}

void MyScrollLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	Vec2 touchPoint = touch->getLocation();	
	//重写加速滑动
	m_bDrag = true; 

	auto m_end = this->getPosition().y + touch->getDelta().y;

	if(m_end<0){
		this->setPositionY(0);
		m_end = 0;
	} else
		if (m_end>m_endPos.y)
		{
			this->setPositionY(m_endPos.y);
			m_end = m_endPos.y;
		} else
		{
			this->setPositionY(m_end);	
			//加入点	
			m_vecPot.push_back(touch->getLocation()); 			
		} 	
}

void MyScrollLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if(m_bDrag && m_vecPot.size() > 5)
	{
		m_bDrag = false;
		int iVecSize = m_vecPot.size(); 
		if(iVecSize < 1) return;
		float fYspeed = m_vecPot[iVecSize - 1].y;

		if(iVecSize < 2) return;

		fYspeed = fYspeed - m_vecPot[iVecSize - 2].y; 	

		while (abs(fYspeed) > 0.1)  
		{  
			Vec2 potNew = this->getPosition() + Vec2(0, fYspeed);  
			auto moveBy = MoveBy::create(0.5f, Vec2(0, fYspeed * 20)); 

			//检测滚动的位置
			this->runAction(Sequence::create(moveBy,CallFunc::create([=](){
				if(this->getPositionY() < 0)
				{
					this->setPositionY(0);					
				} else
					if (this->getPositionY() > m_endPos.y)
					{
						this->setPositionY(m_endPos.y);						
					}

			}),nullptr));				

			fYspeed *= 0.2;  //设置垂直摩擦系数
		}  

		return;
	}
}