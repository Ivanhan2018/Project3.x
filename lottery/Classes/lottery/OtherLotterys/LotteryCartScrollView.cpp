#include "RoomLayer.h"
#include "LotteryCartScrollView.h"
#include "LotteryGambleLayer.h"
using namespace cocos2d;

LotteryCartScrollView::LotteryCartScrollView()
{

}
LotteryCartScrollView::~LotteryCartScrollView()
{
	if(lotteryKindData!=NULL){
		lotteryKindData->release();	
		lotteryKindData=NULL;
	}
	if(gameKindData!=NULL){
		gameKindData->release();
		gameKindData=NULL;
	}
}
Scene* LotteryCartScrollView::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		LotteryCartScrollView *layer = LotteryCartScrollView::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}
bool LotteryCartScrollView::init()
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
		listener->onTouchBegan = CC_CALLBACK_2(LotteryCartScrollView::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(LotteryCartScrollView::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(LotteryCartScrollView::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(LotteryCartScrollView::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		selectedSpriteTag = INVALID_TAG;
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		m_node = Node::create();
		m_node->setPosition(Vec2(0,0));
		this->addChild(m_node,2);
		Vec2 pt = Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT);

		lotteryKindData = Array::create();
		lotteryKindData->retain();
		for(int i = 1 ; i <= 16 ; i++)
		{
			lotteryKindData->addObject(Integer::create(i));
		}
		gameKindData = Array::create();
		gameKindData->retain();
		for(int i = 1 ; i <= 16 ; i++)
		{
			gameKindData->addObject(Integer::create(i));
		}
		lotteryCount = lotteryKindData->count();
		gameCount = gameKindData->count();

		float fontSize = 27;
		//热门彩种
		LabelTTF *lotteryTitle = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t11"), "", fontSize);
		lotteryTitle->setAnchorPoint(Vec2(0,0.5));
		lotteryTitle->setPosition(Vec2(20, pt.y-40));
		lotteryTitle->setColor(ccc3(0,0,0));
		m_node->addChild(lotteryTitle, 0);
		pt.y -= 40;

		Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
		gap1->setAnchorPoint(Vec2(0,0.5));
		gap1->setPosition(Vec2(0, pt.y-20));
		gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
		m_node->addChild(gap1,0);
		pt.y -= 20;

		char filename[30];

		float menuItemWidth = 107.0;
		float menuItemGap = (SCREEN_WIDTH - menuItemWidth * 4) / 5.0f;
		float menuItemPosX = menuItemGap+menuItemWidth/2;
		float menuItemPosY = pt.y - 70;
		for(int i = 0 ; i < lotteryCount ; )
		{
			memset(filename, 0, sizeof(filename));
			int kindId = ((Integer *)lotteryKindData->objectAtIndex(i))->getValue();
			if(kindId<10){
				sprintf(filename, "lottery_kind_0%d.png", kindId);
			}else{
				sprintf(filename, "lottery_kind_%d.png", kindId);
			}
			Sprite *kindSprite = Sprite::createWithSpriteFrame(spriteFrame(filename));
			kindSprite->setPosition(Vec2(menuItemPosX,menuItemPosY));
			kindSprite->setTag(LOTTERY_KIND_SPRITE_TAG+kindId);
			m_node->addChild(kindSprite);
			i++;
			menuItemPosX+=menuItemWidth+menuItemGap;
			if(i%4 == 0&&i < lotteryCount){
				menuItemPosY -= 120;
				menuItemPosX = menuItemGap+menuItemWidth/2;
			}
		}
		pt.y = menuItemPosY;

		fontSize = 27;
		LabelTTF *gameTitle = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t12"), "", fontSize);
		gameTitle->setAnchorPoint(Vec2(0,0.5));
		gameTitle->setPosition(Vec2(20, pt.y-100));
		gameTitle->setColor(ccc3(0,0,0));
		m_node->addChild(gameTitle, 0);
		pt.y -= 100;

		Sprite *gap3 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
		gap3->setAnchorPoint(Vec2(0,0.5));
		gap3->setPosition(Vec2(0, pt.y+30));
		//gap3->setColor(ccc3(100,100,100));
		m_node->addChild(gap3,0);

		Sprite *gap2 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
		gap2->setAnchorPoint(Vec2(0,0.5));
		gap2->setPosition(Vec2(0, pt.y-20));
		//gap2->setColor(ccc3(100,100,100));
		m_node->addChild(gap2,0);
		pt.y -= 20;

		if(gameCount == 0){
			fontSize = 27;
			LabelTTF *noGameHint = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t13"), "", fontSize);
			//noGameHint->setAnchorPoint(Vec2(0,0.5));
			noGameHint->setPosition(Vec2(SCREEN_WIDTH/2, pt.y - 50));
			noGameHint->setColor(ccc3(0,0,0));
			m_node->addChild(noGameHint, 0);
			pt.y -= 100;
		}else{
			menuItemPosX = menuItemGap+menuItemWidth/2;
			menuItemPosY = pt.y - 70;
			for(int i = 0 ; i < gameCount ; )
			{
				memset(filename, 0, sizeof(filename));
				int kindId = ((Integer *)gameKindData->objectAtIndex(i))->getValue();
				if(kindId<10){
					sprintf(filename, "lottery_kind_0%d.png", kindId);
				}else{
					sprintf(filename, "lottery_kind_%d.png", kindId);
				}
				Sprite *kindSprite = Sprite::createWithSpriteFrame(spriteFrame(filename));
				kindSprite->setPosition(Vec2(menuItemPosX,menuItemPosY));
				kindSprite->setTag(GAME_KIND_SPRITE_TAG+kindId);
				m_node->addChild(kindSprite);
				i++;
				menuItemPosX+=menuItemWidth+menuItemGap;
				if(i%4 == 0&&i < lotteryCount){
					menuItemPosY -= 120;
					menuItemPosX = menuItemGap+menuItemWidth/2;
				}
			}
			pt.y = menuItemPosY;
			pt.y -= 100;
		}

		pt.y -= 180;
		if(pt.y>0)
		{
			m_endPos = Vec2(pt.x,0);
		}else{
			m_endPos=Vec2(pt.x,-pt.y);
		}

		//this->setTouchEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void LotteryCartScrollView::pressKindButton(int tag)
{
	playButtonSound();
	LotteryGambleLayer *layer = LotteryGambleLayer::create();

	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}


void LotteryCartScrollView::onEnter()
{
	Layer::onEnter();
	//
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this,-128);	
}

void LotteryCartScrollView::onExit()
{
	//
	Layer::onExit();
}

bool LotteryCartScrollView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	touchPoint = m_node->convertToNodeSpace(touchPoint);
	m_start = touchPoint.y;
	startPoint = touchPoint;

	for(int i = 0 ; i < lotteryCount ; i++)
	{
		int kindId = ((Integer *)lotteryKindData->objectAtIndex(i))->getValue();
		Sprite *kindSprite = (Sprite *)m_node->getChildByTag(LOTTERY_KIND_SPRITE_TAG+kindId);
		Rect rect = kindSprite->boundingBox();
		if(rect.containsPoint(startPoint)){
			selectedSpriteTag = LOTTERY_KIND_SPRITE_TAG+kindId;
			kindSprite->setScale(0.9);
		}
	}

	for(int i = 0 ; i < gameCount ; i++)
	{
		int kindId = ((Integer *)gameKindData->objectAtIndex(i))->getValue();
		Sprite *kindSprite = (Sprite *)m_node->getChildByTag(GAME_KIND_SPRITE_TAG+kindId);
		Rect rect = kindSprite->boundingBox();
		if(rect.containsPoint(startPoint)){
			selectedSpriteTag = GAME_KIND_SPRITE_TAG+kindId;
			kindSprite->setScale(0.9);
		}
	}
	return true;
}
void LotteryCartScrollView::onTouchMoved(Touch *pTouch, Event *pEvent)
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

void LotteryCartScrollView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	touchPoint = m_node->convertToNodeSpace(touchPoint);
	endPoint = touchPoint;

	if(selectedSpriteTag == INVALID_TAG)
	{
		return;
	}
	Sprite *kindSprite = (Sprite *)m_node->getChildByTag(selectedSpriteTag);
	kindSprite->setScale(1.0);

	if (ChongQingcartAbs(startPoint.x - endPoint.x) > DELTA ||   
		ChongQingcartAbs(startPoint.y - endPoint.y) > DELTA)   
	{ 
		selectedSpriteTag = INVALID_TAG;
		return;  
	}

	Rect rect = kindSprite->boundingBox();
	if(rect.containsPoint(endPoint))
	{
		this->pressKindButton(selectedSpriteTag);
	}
	selectedSpriteTag = INVALID_TAG;
}

void LotteryCartScrollView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}