#include "HelpLayer.h"
#include "RoomLayer.h"
#include "ChongQingGroupSingle.h"
#include "VersionControl.h"

using namespace cocos2d;

int ballStatus[5][10] = {
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0}
};
int menuStatus[5][6] = {
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0}
};
ChongQingGroupSingle::ChongQingGroupSingle()
{

}

ChongQingGroupSingle::~ChongQingGroupSingle()
{

}

Scene* ChongQingGroupSingle::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		ChongQingGroupSingle *layer = ChongQingGroupSingle::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool ChongQingGroupSingle::init()
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
		listener->onTouchBegan = CC_CALLBACK_2(ChongQingGroupSingle::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(ChongQingGroupSingle::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(ChongQingGroupSingle::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(ChongQingGroupSingle::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		m_node = Node::create();
		m_node->setPosition(Vec2(0,0));
		this->addChild(m_node,2);
		pt = Vec2(SCREEN_WIDTH/2, 620);

		areaTitleXDelta = -5;
		fontSize = 27;
		initWArea();
		initQArea();
		initBArea();
		initSArea();
		initGWArea();

		pt.y -= 100;
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

//万位
void ChongQingGroupSingle::initWArea()
{
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaW = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaW->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaW,0);

	this->initMenu(Vec2(areaMenuPosX, areaW->getPositionY()), W_AREA_MENU_TAG);

	LabelTTF *areaWTitle = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t90"), "", fontSize);
	areaWTitle->setPosition(Vec2(areaW->getPositionX()+areaTitleXDelta, areaW->getPositionY()));
	areaWTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaWTitle, 0);

#ifdef VER_QIANBO
	areaWTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaWTitle->setColor(ccc3(50, 62, 123));
#endif

	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapW = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapW->setAnchorPoint(Vec2(0,0.5));
	gapW->setPosition(Vec2(0, pt.y));gapW->setScaleX(SCREEN_WIDTH / gapW->getContentSize().width);
	m_node->addChild(gapW,0);

	this->initBalls(Vec2(ballStartX, areaW->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2), W_AREA_BALL_TAG);
}

//千位
void ChongQingGroupSingle::initQArea()
{
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaQ = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaQ->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaQ,0);

	this->initMenu(Vec2(areaMenuPosX, areaQ->getPositionY()), Q_AREA_MENU_TAG);

	LabelTTF *areaQTitle = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t91"), "", fontSize);
	areaQTitle->setPosition(Vec2(areaQ->getPositionX()+areaTitleXDelta, areaQ->getPositionY()));
	areaQTitle->setColor(ccc3(167,60,60));;
	m_node->addChild(areaQTitle, 0);

#ifdef VER_QIANBO
	areaQTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaQTitle->setColor(ccc3(50, 62, 123));
#endif

	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapQ = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapQ->setAnchorPoint(Vec2(0,0.5));
	gapQ->setPosition(Vec2(0, pt.y));gapQ->setScaleX(SCREEN_WIDTH / gapQ->getContentSize().width);
	m_node->addChild(gapQ,0);

	this->initBalls(Vec2(ballStartX, areaQ->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2), Q_AREA_BALL_TAG);
}

//百位
void ChongQingGroupSingle::initBArea()
{
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaB = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaB->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaB,0);

	this->initMenu(Vec2(areaMenuPosX, areaB->getPositionY()), B_AREA_MENU_TAG);

	LabelTTF *areaBTitle = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t92"), "", fontSize);
	areaBTitle->setPosition(Vec2(areaB->getPositionX()+areaTitleXDelta, areaB->getPositionY()));
	areaBTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaBTitle, 0);

#ifdef VER_QIANBO
	areaBTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaBTitle->setColor(ccc3(50, 62, 123));
#endif

	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapB = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapB->setAnchorPoint(Vec2(0,0.5));
	gapB->setPosition(Vec2(0, pt.y));
	m_node->addChild(gapB,0);

	this->initBalls(Vec2(ballStartX, areaB->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2), B_AREA_BALL_TAG);
}

//十位
void ChongQingGroupSingle::initSArea()
{
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaS = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaS->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaS,0);

	this->initMenu(Vec2(areaMenuPosX, areaS->getPositionY()), S_AREA_MENU_TAG);

	LabelTTF *areaSTitle = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t93"), "", fontSize);
	areaSTitle->setPosition(Vec2(areaS->getPositionX()+areaTitleXDelta, areaS->getPositionY()));
	areaSTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaSTitle, 0);

#ifdef VER_QIANBO
	areaSTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaSTitle->setColor(ccc3(50, 62, 123));
#endif

	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapS = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapS->setAnchorPoint(Vec2(0,0.5));
	gapS->setPosition(Vec2(0, pt.y));gapS->setScaleX(SCREEN_WIDTH / gapS->getContentSize().width);
	m_node->addChild(gapS,0);

	this->initBalls(Vec2(ballStartX, areaS->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2), S_AREA_BALL_TAG);
}

//个位
void ChongQingGroupSingle::initGWArea()
{
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaG = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaG->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaG,0);

	this->initMenu(Vec2(areaMenuPosX, areaG->getPositionY()), G_AREA_MENU_TAG);

	LabelTTF *areaGTitle = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t94"), "", fontSize);
	areaGTitle->setPosition(Vec2(areaG->getPositionX()+areaTitleXDelta, areaG->getPositionY()));
	areaGTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaGTitle, 0);

#ifdef VER_QIANBO
	areaGTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaGTitle->setColor(ccc3(50, 62, 123));
#endif

	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapG = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapG->setAnchorPoint(Vec2(0,0.5));
	gapG->setPosition(Vec2(0, pt.y));gapG->setScaleX(SCREEN_WIDTH / gapG->getContentSize().width);
	m_node->addChild(gapG,0);

	this->initBalls(Vec2(ballStartX, areaG->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2), G_AREA_BALL_TAG);
}

//初始化UI:大小单双
void ChongQingGroupSingle::initDXDS(Vec2 startPos, int startTag)		
{
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_011.png");
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	Vec2 pos = startPos;
	for (int i = 0; i < 4;){
		Sprite *ballSprite = Sprite::createWithSpriteFrame(
			SpriteFrame::createWithTexture(ballTexture2D, Rect(i * ballRadius,0,ballRadius,ballRadius)));
		ballSprite->setPosition(Vec2(pos.x + (ballRadius + ballGapX)*(i % 7), pos.y));
		ballSprite->setTag(startTag + i);
		m_node->addChild(ballSprite);
		i++;
	}

}

//初始化UI:数字0-9
void ChongQingGroupSingle::initBalls(Vec2 startPos, int startTag)
{
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	Vec2 pos = startPos;
	for(int i = 0 ; i < 10 ; ){
		Sprite *ballSprite = Sprite::createWithSpriteFrame(
			SpriteFrame::createWithTexture(ballTexture2D, Rect(i*ballRadius,0,ballRadius,ballRadius)));
		ballSprite->setPosition(Vec2(pos.x+(ballRadius+ballGapX)*(i%7),pos.y));
		ballSprite->setTag(startTag+i);
		m_node->addChild(ballSprite);
		i++;
		if(i%7==0){
			pos.x = startPos.x;
			pos.y -= (ballRadius + areaGapY);
		}
	}
}

void ChongQingGroupSingle::initMenu(Vec2 menuPos, int startTag)
{
	Sprite *bigNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_28.png"));
	Sprite *bigSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_28.png"));
	Sprite *littleNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_30.png"));
	Sprite *littleSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_30.png"));
	Sprite *allNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_32.png"));
	Sprite *allSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_32.png"));
	Sprite *singleNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_34.png"));
	Sprite *singleSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_34.png"));
	Sprite *doubleNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_36.png"));
	Sprite *doubleSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_36.png"));
	Sprite *clearNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_38.png"));
	Sprite *clearSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_38.png"));
	MenuItemSprite *bigItem = MenuItemSprite::create(bigNormalImage,bigSelectedImage,CC_CALLBACK_1(ChongQingGroupSingle::pressBig,this));
	MenuItemSprite *littleItem = MenuItemSprite::create(littleNormalImage,littleSelectedImage,CC_CALLBACK_1(ChongQingGroupSingle::pressLittle,this));
	MenuItemSprite *allItem = MenuItemSprite::create(allNormalImage,allSelectedImage,CC_CALLBACK_1(ChongQingGroupSingle::pressAll,this));
	MenuItemSprite *singleItem = MenuItemSprite::create(singleNormalImage,singleSelectedImage,CC_CALLBACK_1(ChongQingGroupSingle::pressSingle,this));
	MenuItemSprite *doubleItem = MenuItemSprite::create(doubleNormalImage,doubleSelectedImage,CC_CALLBACK_1(ChongQingGroupSingle::pressDouble,this));
	MenuItemSprite *clearItem = MenuItemSprite::create(clearNormalImage,clearSelectedImage,CC_CALLBACK_1(ChongQingGroupSingle::pressClear,this));
	bigItem->setTag(startTag);
	littleItem->setTag(startTag+1);
	allItem->setTag(startTag+2);
	singleItem->setTag(startTag+3);
	doubleItem->setTag(startTag+4);
	clearItem->setTag(startTag+5);
	Menu *menu = Menu::create(bigItem,littleItem,allItem,singleItem,doubleItem,clearItem,NULL);
	menu->setTag(startTag);
	menu->setPosition(menuPos);	
	menu->alignItemsHorizontallyWithPadding(10);//�������з�ʽ��������20
	m_node->addChild(menu,0);
}

void ChongQingGroupSingle::pressKindButton(int tag)
{
	CCLOG("fuck");
}

void ChongQingGroupSingle::onEnter()
{
	Layer::onEnter();
	//
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this,-128);	
}

void ChongQingGroupSingle::onExit()
{
	//
	Layer::onExit();
}

bool ChongQingGroupSingle::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	touchPoint = m_node->convertToNodeSpace(touchPoint);
	m_start = touchPoint.y;
	startPoint = touchPoint;


	return true;
}
void ChongQingGroupSingle::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	touchPoint = m_node->convertToNodeSpace(touchPoint);
	m_end = touchPoint.y;
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

void ChongQingGroupSingle::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	touchPoint = m_node->convertToNodeSpace(touchPoint);
	endPoint = touchPoint;

	for(int j = 0 ; j < 5 ; j++){
		for(int i = 0 ; i < 10 ; i++)
		{
			Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+i);
			if(ballSprite == NULL) continue;
			Rect rect = ballSprite->boundingBox();
			if(rect.containsPoint(startPoint)){
				ballStatus[j][i] = 1-ballStatus[j][i];

				SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
				Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
				if(ballStatus[j][i] == 0){
					ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
						Rect(i*ballRadius,0,ballRadius,ballRadius)));
				}else{
					ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
						Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
				}
				if(menuStatus[j][5] == 1){
					menuStatus[j][5] = 0;
					Menu *menu = (Menu *)m_node->getChildByTag(W_AREA_MENU_TAG+j*10);
					MenuItemSprite *tempItem = (MenuItemSprite *)menu->getChildByTag(W_AREA_MENU_TAG+j*10+5);
					Sprite *tempNormal = Sprite::createWithSpriteFrame(spriteFrame("gamble_38.png"));
					Sprite *tempSelected = Sprite::createWithSpriteFrame(spriteFrame("gamble_38.png"));
					tempItem->setNormalImage(tempNormal);
					tempItem->setSelectedImage(tempSelected);
				}
			}
		}
	}
}

void ChongQingGroupSingle::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

String *ChongQingGroupSingle::getBetNumber()
{
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};
	bool flag;
	for(int j = 0 ; j < 5 ; j++){
		flag = false;
		if(j!=0){
			result+="|";
		}
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[j][i] == 1)
			{
				result+=numberStr[i];
				flag = true;
			}
		}
		if(flag == false){
			result = "";
			break;
		}
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

void ChongQingGroupSingle::createRandomNumbers()
{
	CCLOG("ChongQingGroupSingle::createRandomNumbers");
	this->clearNumbers();
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	for(int j = 0 ; j < 5 ; j++){
		int num = rand()%10;
		ballStatus[j][num] = 1-ballStatus[j][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}

void ChongQingGroupSingle::clearNumbers()
{
	for(int j = 0 ; j < 5 ; j++){
		for(int i = 0 ; i < 10 ; i++)
		{
			Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+i);
			if(ballSprite == NULL) continue;
			Rect rect = ballSprite->boundingBox();
			SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
			Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
			if(ballStatus[j][i] == 1){
				ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
					Rect(i*ballRadius,0,ballRadius,ballRadius)));
				ballStatus[j][i] = 1-ballStatus[j][i];
			}
		}
	}
}

void ChongQingGroupSingle::clearNumbers(int area)
{
	for(int i = 0 ; i < 10 ; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+area*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[area][i] == 1){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,0,ballRadius,ballRadius)));
			ballStatus[area][i] = 1-ballStatus[area][i];
		}
	}
}

void ChongQingGroupSingle::pressBig(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-50)/10;
	//if(menuStatus[itemAreaIndex][0] == 1){	
	//	return;
	//}
	this->cancelMenuSelect(itemTag);
	Sprite *normalImage;
	Sprite *selectedImage;
	normalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_29.png"));
	selectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_29.png"));
	menuItem->setNormalImage(normalImage);
	menuItem->setSelectedImage(selectedImage);
	menuStatus[itemAreaIndex][0] = 1;

	//update balls
	this->clearNumbers(itemAreaIndex);
	for(int i = 5 ; i < 10 ; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[itemAreaIndex][i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[itemAreaIndex][i] = 1;
		}
	}
}

void ChongQingGroupSingle::pressLittle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-50)/10;
	//if(menuStatus[itemAreaIndex][1] == 1){	
	//	return;
	//}
	this->cancelMenuSelect(itemTag-1);
	Sprite *normalImage;
	Sprite *selectedImage;
	normalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_31.png"));
	selectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_31.png"));
	menuItem->setNormalImage(normalImage);
	menuItem->setSelectedImage(selectedImage);
	menuStatus[itemAreaIndex][1] = 1;

	//update balls
	this->clearNumbers(itemAreaIndex);
	for(int i = 0 ; i < 5 ; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[itemAreaIndex][i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[itemAreaIndex][i] = 1;
		}
	}
}

void ChongQingGroupSingle::pressAll(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-50)/10;
	//if(menuStatus[itemAreaIndex][2] == 1){	
	//	return;
	//}
	this->cancelMenuSelect(itemTag-2);
	Sprite *normalImage;
	Sprite *selectedImage;
	normalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_33.png"));
	selectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_33.png"));
	menuItem->setNormalImage(normalImage);
	menuItem->setSelectedImage(selectedImage);
	menuStatus[itemAreaIndex][2] = 1;

	//update balls
	this->clearNumbers(itemAreaIndex);
	for(int i = 0 ; i < 10 ; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[itemAreaIndex][i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[itemAreaIndex][i] = 1;
		}
	}
}

void ChongQingGroupSingle::pressSingle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-50)/10;
	//if(menuStatus[itemAreaIndex][3] == 1){	
	//	return;
	//}
	this->cancelMenuSelect(itemTag-3);
	Sprite *normalImage;
	Sprite *selectedImage;
	normalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_35.png"));
	selectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_35.png"));
	menuItem->setNormalImage(normalImage);
	menuItem->setSelectedImage(selectedImage);
	menuStatus[itemAreaIndex][3] = 1;

	//update balls
	this->clearNumbers(itemAreaIndex);
	for(int i = 1 ; i < 10 ; i+=2)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[itemAreaIndex][i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[itemAreaIndex][i] = 1;
		}
	}
}

void ChongQingGroupSingle::pressDouble(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-50)/10;
	//if(menuStatus[itemAreaIndex][4] == 1){	
	//	return;
	//}
	this->cancelMenuSelect(itemTag-4);
	Sprite *normalImage;
	Sprite *selectedImage;
	normalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_37.png"));
	selectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_37.png"));
	menuItem->setNormalImage(normalImage);
	menuItem->setSelectedImage(selectedImage);
	menuStatus[itemAreaIndex][4] = 1;

	//update balls
	this->clearNumbers(itemAreaIndex);
	for(int i = 0 ; i < 10 ; i+=2)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[itemAreaIndex][i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[itemAreaIndex][i] = 1;
		}
	}
}

void ChongQingGroupSingle::pressClear(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-50)/10;

	this->cancelMenuSelect(itemTag-5);
	Sprite *normalImage;
	Sprite *selectedImage;
	normalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_39.png"));
	selectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_39.png"));
	menuItem->setNormalImage(normalImage);
	menuItem->setSelectedImage(selectedImage);
	menuStatus[itemAreaIndex][5] = 1;

	//update balls	
	this->clearNumbers(itemAreaIndex);
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

void ChongQingGroupSingle::cancelMenuSelect(int itemTag)
{
	int itemAreaIndex = (itemTag-50)/10;
	Menu *menu = (Menu *)m_node->getChildByTag(itemTag);
	for(int i = 0 ; i < 6 ; i++){
		if(menuStatus[itemAreaIndex][i] == 1){
			MenuItemSprite *tempItem = (MenuItemSprite *)menu->getChildByTag(itemTag+i);
			Sprite *tempNormal = Sprite::createWithSpriteFrame(spriteFrame(
				String::createWithFormat("gamble_%d.png", 28+2*i)->getCString()));
			Sprite *tempSelected = Sprite::createWithSpriteFrame(spriteFrame(
				String::createWithFormat("gamble_%d.png", 28+2*i)->getCString()));
			tempItem->setNormalImage(tempNormal);
			tempItem->setSelectedImage(tempSelected);
			menuStatus[itemAreaIndex][i] = 0;
		}
	}
}