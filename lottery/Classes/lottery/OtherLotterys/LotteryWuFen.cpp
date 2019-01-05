#include "HelpLayer.h"
#include "RoomLayer.h"
#include "LotteryWuFen.h"
#include "BetLimit.h"
#include "MovingLabelLayer.h"
#include "VersionControl.h"

using namespace cocos2d;

LotteryWuFen::LotteryWuFen()
{

}

LotteryWuFen::~LotteryWuFen()
{
	m_vecPot.clear();
}

Scene* LotteryWuFen::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		LotteryWuFen *layer = LotteryWuFen::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool LotteryWuFen::init()
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
		listener->onTouchBegan = CC_CALLBACK_2(LotteryWuFen::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(LotteryWuFen::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(LotteryWuFen::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(LotteryWuFen::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		m_node = Node::create();
		m_node->setPosition(Vec2(0,0));
		this->addChild(m_node,2);

		areaTitleXDelta = -5;
		fontSize = 27;
		m_num = 0;
		m_repeat = 0;	

		for(int i = 0 ; i < 6 ; i++){
			memset(ballStatus[i], 0, sizeof(ballStatus[i]));
			memset(menuStatus[i], 0, sizeof(menuStatus[i]));
		}
		memset(areaStatus, 0, sizeof(areaStatus));

		this->setAccelerometerEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void LotteryWuFen::onAcceleration(Acceleration* acc, Event* unused_event) 
{   
	static Acceleration lastAcc = *acc;
	const double dCurTimeStamp = acc->timestamp;  
	const double dLentthSq = (acc->x - lastAcc.x)*(acc->x - lastAcc.x)+
		(acc->y - lastAcc.y)*(acc->y - lastAcc.y)+
		(acc->z - lastAcc.z)*(acc->z - lastAcc.z);

	if( dLentthSq > 5 ){  
		this->createRandomNumbers();
	}

	lastAcc = *acc;  
} 

void LotteryWuFen::setBetKind(QiXingCaiKind kind)
{
	pt = Vec2(240, SCREEN_HEIGHT - 310);//280
	m_betKind = kind;
	switch (m_betKind)
	{
	case QiXingCai_LiangDing:
	case QiXingCai_SanDing:
	case QiXingCai_SiDing:
	case QiXingCai_YiDing:
		initWArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
		initQArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
		initBArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
		initSArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
		break;
	case QiXingCai_LiangZiXian:
	case QiXingCai_SanZiXian:
		initWArea(ConfigMgr::instance()->text("display_text.xml", "t127"));
		break;
	default:
		break;
	}

	pt.y -= 100 + 200;
	if(pt.y>0)
	{
		m_endPos = Vec2(pt.x,0);
	}else{
		m_endPos=Vec2(pt.x,-pt.y);
	}
}

void LotteryWuFen::initWArea(const char *titleXmlIndex)
{
	areaStatus[0] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称背景
	Sprite *areaW = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaW->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaW,0);

	//菜单 大小单双
	this->initMenu(Vec2(areaMenuPosX, areaW->getPositionY()), W_AREA_MENU_TAG);

	//分组名称文字
	LabelTTF *areaWTitle = LabelTTF::create(titleXmlIndex, "", fontSize);
	areaWTitle->setPosition(Vec2(areaW->getPositionX()+areaTitleXDelta, areaW->getPositionY()));
	areaWTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaWTitle, 0);

#ifdef VER_QIANBO
	areaWTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaWTitle->setColor(ccc3(50, 62, 123));
#endif

	//下划线
	pt.y -= (280 - areaGapY - areaBkHeight / 2);//(areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapW = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapW->setAnchorPoint(Vec2(0,0.5));
	gapW->setPosition(Vec2(0, pt.y));gapW->setScaleX(SCREEN_WIDTH / gapW->getContentSize().width);
	m_node->addChild(gapW,0);

	this->initBalls(Vec2(ballStartX, areaW->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2 - 8), W_AREA_BALL_TAG);
}

void LotteryWuFen::initQArea(const char *titleXmlIndex)
{
	areaStatus[1] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称
	Sprite *areaQ = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaQ->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaQ,0);

	//菜单 大小单双
	this->initMenu(Vec2(areaMenuPosX, areaQ->getPositionY()), Q_AREA_MENU_TAG);

	//分组名称文字
	LabelTTF *areaQTitle = LabelTTF::create(titleXmlIndex, "", fontSize);
	areaQTitle->setPosition(Vec2(areaQ->getPositionX()+areaTitleXDelta, areaQ->getPositionY()));
	areaQTitle->setColor(ccc3(167,60,60));;
	m_node->addChild(areaQTitle, 0);

#ifdef VER_QIANBO
	areaQTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaQTitle->setColor(ccc3(50, 62, 123));
#endif

	//下划线
	pt.y -= (280 - areaGapY - areaBkHeight / 2);//(areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapQ = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapQ->setAnchorPoint(Vec2(0,0.5));
	gapQ->setPosition(Vec2(0, pt.y));gapQ->setScaleX(SCREEN_WIDTH / gapQ->getContentSize().width);
	m_node->addChild(gapQ,0);

	this->initBalls(Vec2(ballStartX, areaQ->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), Q_AREA_BALL_TAG);
}

void LotteryWuFen::initBArea(const char *titleXmlIndex)
{
	areaStatus[2] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称
	Sprite *areaB = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaB->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaB,0);

	//菜单 大小单双
	this->initMenu(Vec2(areaMenuPosX, areaB->getPositionY()), B_AREA_MENU_TAG);

	//分组名称文字
	LabelTTF *areaBTitle = LabelTTF::create(titleXmlIndex, "", fontSize);
	areaBTitle->setPosition(Vec2(areaB->getPositionX()+areaTitleXDelta, areaB->getPositionY()));
	areaBTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaBTitle, 0);

#ifdef VER_QIANBO
	areaBTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaBTitle->setColor(ccc3(50, 62, 123));
#endif

	//下划线
	pt.y -= (280 - areaGapY - areaBkHeight / 2);//(areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapB = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapB->setAnchorPoint(Vec2(0,0.5));
	gapB->setPosition(Vec2(0, pt.y));
	m_node->addChild(gapB,0);

	this->initBalls(Vec2(ballStartX, areaB->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), B_AREA_BALL_TAG);
}

void LotteryWuFen::initSArea(const char *titleXmlIndex)
{
	areaStatus[3] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称
	Sprite *areaS = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaS->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaS,0);

	//菜单 大小单双
	this->initMenu(Vec2(areaMenuPosX, areaS->getPositionY()), S_AREA_MENU_TAG);

	//分组名称文字
	LabelTTF *areaSTitle = LabelTTF::create(titleXmlIndex, "", fontSize);
	areaSTitle->setPosition(Vec2(areaS->getPositionX()+areaTitleXDelta, areaS->getPositionY()));
	areaSTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaSTitle, 0);

#ifdef VER_QIANBO
	areaSTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaSTitle->setColor(ccc3(50, 62, 123));
#endif
	//下划线
	pt.y -= (280 - areaGapY - areaBkHeight / 2);//(areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapS = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapS->setAnchorPoint(Vec2(0,0.5));
	gapS->setPosition(Vec2(0, pt.y));gapS->setScaleX(SCREEN_WIDTH / gapS->getContentSize().width);
	m_node->addChild(gapS,0);

	this->initBalls(Vec2(ballStartX, areaS->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), S_AREA_BALL_TAG);
}

void LotteryWuFen::initGWArea(const char *titleXmlIndex)
{
	areaStatus[4] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称
	Sprite *areaG = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaG->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaG,0);

	//菜单 大小单双
	this->initMenu(Vec2(areaMenuPosX, areaG->getPositionY()), G_AREA_MENU_TAG);

	//分组名称文字
	LabelTTF *areaGTitle = LabelTTF::create(titleXmlIndex, "", fontSize);
	areaGTitle->setPosition(Vec2(areaG->getPositionX()+areaTitleXDelta, areaG->getPositionY()));
	areaGTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaGTitle, 0);

#ifdef VER_QIANBO
	areaGTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaGTitle->setColor(ccc3(50, 62, 123));
#endif

	//下划线
	pt.y -= (280 - areaGapY - areaBkHeight / 2);//(areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapG = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapG->setAnchorPoint(Vec2(0,0.5));
	gapG->setPosition(Vec2(0, pt.y));gapG->setScaleX(SCREEN_WIDTH / gapG->getContentSize().width);
	m_node->addChild(gapG,0);

	this->initBalls(Vec2(ballStartX, areaG->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), G_AREA_BALL_TAG);
}

/***
数字0-9
***/
void LotteryWuFen::initBalls(Vec2 startPos, int startTag)
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
			pos.y -= (ballRadius + areaGapY + 2);
		}
	}
}

/***
大小单双 菜单按钮
***/
void LotteryWuFen::initMenu(Vec2 menuPos, int startTag)
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
	MenuItemSprite *bigItem = MenuItemSprite::create(bigNormalImage,bigSelectedImage,CC_CALLBACK_1(LotteryWuFen::pressBig,this));
	MenuItemSprite *littleItem = MenuItemSprite::create(littleNormalImage,littleSelectedImage,CC_CALLBACK_1(LotteryWuFen::pressLittle,this));
	MenuItemSprite *allItem = MenuItemSprite::create(allNormalImage,allSelectedImage,CC_CALLBACK_1(LotteryWuFen::pressAll,this));
	MenuItemSprite *singleItem = MenuItemSprite::create(singleNormalImage,singleSelectedImage,CC_CALLBACK_1(LotteryWuFen::pressSingle,this));
	MenuItemSprite *doubleItem = MenuItemSprite::create(doubleNormalImage,doubleSelectedImage,CC_CALLBACK_1(LotteryWuFen::pressDouble,this));
	MenuItemSprite *clearItem = MenuItemSprite::create(clearNormalImage,clearSelectedImage,CC_CALLBACK_1(LotteryWuFen::pressClear,this));
	bigItem->setTag(startTag);
	littleItem->setTag(startTag+1);
	allItem->setTag(startTag+2);
	singleItem->setTag(startTag+3);
	doubleItem->setTag(startTag+4);
	clearItem->setTag(startTag+5);
	Menu *menu = Menu::create(bigItem,littleItem,allItem,singleItem,doubleItem,clearItem,NULL);
	menu->setTag(startTag);
	menu->setAnchorPoint(Vec2(0,0));
	menu->setPosition(Vec2(menuPos.x + 200,menuPos.y));	//后移200px
	menu->alignItemsHorizontallyWithPadding(10);
	m_node->addChild(menu,0);
}

void LotteryWuFen::pressKindButton(int tag)
{
	CCLOG("fuck");
}

void LotteryWuFen::onEnter()
{
	Layer::onEnter();
	//
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this,-128);	
}

void LotteryWuFen::onExit()
{
	//
	Layer::onExit();
}

bool LotteryWuFen::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (pTouch->getLocation().y < 280 || pTouch->getLocation().y > 970)
	{
		pEvent->stopPropagation();
		return false;
	}
		
	//清空数组
	m_vecPot.clear();
	m_bDrag = false;

	return true;
}
void LotteryWuFen::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = pTouch->getLocation();	
	//重写加速滑动
	m_bDrag = true; 

	m_end = m_node->getPosition().y + pTouch->getDelta().y;

	if(m_end<0){
		m_node->setPositionY(0);
		m_end = 0;
	} else
		if (m_end>m_endPos.y)
		{
			m_node->setPositionY(m_endPos.y);
			m_end = m_endPos.y;
		} else
		{
			m_node->setPositionY(m_end);	
			//加入点	
			m_vecPot.push_back(pTouch->getLocation()); 			
		}
}

//手指触摸离开
void LotteryWuFen::onTouchEnded(Touch *pTouch, Event *pEvent)
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
			Vec2 potNew = m_node->getPosition() + Vec2(0, fYspeed);  
			auto moveBy = MoveBy::create(0.5f, Vec2(0, fYspeed * 20)); 

			//检测滚动的位置
			m_node->runAction(Sequence::create(moveBy,CallFunc::create([=](){
				if(m_node->getPositionY() < 0)
				{
					m_node->setPositionY(0);					
				} else
					if (m_node->getPositionY() > m_endPos.y)
					{
						m_node->setPositionY(m_endPos.y);						
					}

			}),nullptr));				

			fYspeed *= 0.2;  //设置垂直摩擦系数
		}  

		return;
	}

	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	touchPoint = m_node->convertToNodeSpace(touchPoint);
	endPoint = touchPoint;

	int ballNum;
	for(int j = 0 ; j < 6 ; j++)
	{
		ballNum = 10;		

		for(int i = 0 ; i < ballNum ; i++)
		{
			if(areaStatus[j] == 0){
				continue;
			}
			Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+i);
			if(ballSprite == NULL) continue;
			Rect rect = ballSprite->boundingBox();
			if(rect.containsPoint(endPoint)){					

				ballStatus[j][i] = 1-ballStatus[j][i];

				SpriteFrame *ballSpriteFrame;
				ballSpriteFrame = spriteFrame("gamble_01.png");

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
				NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
			} 				
		}
	}
}

//手指触摸取消
void LotteryWuFen::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

String *LotteryWuFen::getBetNumber()
{
	String* result = NULL;
	switch (m_betKind)
	{
	case QiXingCai_LiangDing:
		result = this->getZhixuanBetNumber(2);
		break;
	case QiXingCai_SanDing:
		result = this->getZhixuanBetNumber(3);
		break;
	case QiXingCai_SiDing:
		result = this->getZhixuanBetNumber(4);
		break;
	case QiXingCai_YiDing:
		result = this->getZhixuanBetNumber(1);
		break;
	case QiXingCai_LiangZiXian:
		result = this->getBuDingWeiBetNumber(2);
		break;
	case QiXingCai_SanZiXian:
		result = this->getBuDingWeiBetNumber(3);
		break;
	default:
		result = String::create("");
		break;
	}

	return result;
}

String *LotteryWuFen::getZhixuanBetNumber(int num)
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

	int selRowCount = 0; //已经选择的行数

	for(int j = 0 ; j < 4 ; j++){  //4行
		int itemcount = 0;

		for(int i = 0 ; i < 10 ; i++)//10列
		{
			if(ballStatus[j][i] == 1)
			{
				result+=numberStr[i];
				ballNum++;
				itemcount ++;
			}
		}
		if(itemcount != 0)
		{
			selRowCount ++; //有选中，则行数加1
			betZhuShu *= itemcount;			
		}
		if(j != 3){
			result+=",";
		}
	}

	if(ballNum < 1 || selRowCount != num){
		result = "";
		betZhuShu = 0;
	}

	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *LotteryWuFen::getBuDingWeiBetNumber(int num)
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};
	int ballNum = 0;
	int oidfjgvbiodf = ballStatus[0][2];
	for(int i = 0 ; i < 11 ; i++)
	{
		if(ballStatus[0][i] == 1)
		{
			if(ballNum>0){
				result+=",";
			}
			result+=numberStr[i];
			ballNum++;
		}
	}
	if(ballNum == num){
		betZhuShu = 1;
	}else{	
		result = "";
		betZhuShu = 0;
	}

	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

void LotteryWuFen::createRandomNumbers()
{
	CCLOG("LotteryWuFen::createRandomNumbers");

	switch (m_betKind)
	{
	case QiXingCai_LiangDing:
		this->createRenxuanRandomNumber(2);
		break;
	case QiXingCai_SanDing:
		this->createRenxuanRandomNumber(3);
		break;
	case QiXingCai_SiDing:
		this->createRenxuanRandomNumber(4);
		break;
	case QiXingCai_YiDing:
		this->createRenxuanRandomNumber(1);
		break;
	case QiXingCai_LiangZiXian:
		this->createZuxuanRandomNumbers(2);
		break;
	case QiXingCai_SanZiXian:
		this->createZuxuanRandomNumbers(3);
		break;
	default:
		break;
	}
}

void LotteryWuFen::createZhixuanRandomNumbers(int num)
{
	this->clearNumbers();
	for(int j = 0 ; j < 6 ; j++){
		if(areaStatus[j] == 0){
			continue;
		}
		SpriteFrame *ballSpriteFrame; 
		Texture2D *ballTexture2D; 
		int ballNum = 10;		
		ballSpriteFrame = spriteFrame("gamble_01.png");		
		ballTexture2D = ballSpriteFrame->getTexture();
		int num = rand()%ballNum;
		ballStatus[j][num] = 1-ballStatus[j][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}

void LotteryWuFen::createRenxuanRandomNumber(int num)
{
	if(num > 4) return;
	int myCount[4] = {1,1,1,1};
	this->clearNumbers();

	//任选多少个数
	for(int i = 0;i < 4 - num;i ++)
	{
		int tempInt = rand() % 4;
		while (myCount[tempInt] == 0)
		{
			tempInt = (tempInt + 1) % 4;			
		}
		myCount[tempInt] = 0;
	}	

	for(int j = 0 ; j < 6 ; j++){
		if(areaStatus[j] == 0 || (j < 5 && myCount[j] == 0)){
			continue;
		}
		SpriteFrame *ballSpriteFrame; 
		Texture2D *ballTexture2D; 
		int ballNum = 10;		
		ballSpriteFrame = spriteFrame("gamble_01.png");		
		ballTexture2D = ballSpriteFrame->getTexture();
		int num = rand()%ballNum;
		ballStatus[j][num] = 1-ballStatus[j][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
};

void LotteryWuFen::createZuxuanRandomNumbers(int num)
{
	this->clearNumbers(0);
	for(int j = 0 ; j < num ; j++){
		SpriteFrame *ballSpriteFrame; 
		Texture2D *ballTexture2D; 
		int ballNum = 10;		
		ballSpriteFrame = spriteFrame("gamble_01.png");		
		ballTexture2D = ballSpriteFrame->getTexture();
		int num = rand()%ballNum;
		while(ballStatus[0][num] == 1){
			num = rand()%ballNum;
		}
		ballStatus[0][num] = 1-ballStatus[0][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}

void LotteryWuFen::clearNumbers()
{
	int ballNum;
	for(int j = 0 ; j < 6 ; j++){
		if(j == 5){
			ballNum = 28;
		}else{
			ballNum = 10;
		}
		
		for(int i = 0 ; i < ballNum ; i++)
		{
			if(areaStatus[j] == 0){
				continue;
			}
			Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+i);			
			if(ballSprite == NULL) continue;
			Rect rect = ballSprite->boundingBox();
			SpriteFrame *ballSpriteFrame;
			if(j == 5){
				ballSpriteFrame = spriteFrame("gamble_00.png");
			}else{
				ballSpriteFrame = spriteFrame("gamble_01.png");
			}
			
			Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
			if(ballStatus[j][i] == 1){				
				ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
					Rect(i*ballRadius,0,ballRadius,ballRadius)));
				ballStatus[j][i] = 1-ballStatus[j][i];				
			}
		}
	}

	betZhuShu = 0;
}

void LotteryWuFen::clearNumbers(int area)
{
	int ballNum = 10;
	if(area == 5)
	{
		ballNum = 28;
	}

	for(int i = 0 ; i < ballNum ; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+area*10+i);
		if(ballSprite == nullptr) continue;
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame;
		if(area == 5){
			ballSpriteFrame = spriteFrame("gamble_00.png");
		}else{
			ballSpriteFrame = spriteFrame("gamble_01.png");
		}
		
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[area][i] == 1){			
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,0,ballRadius,ballRadius)));
			ballStatus[area][i] = 1-ballStatus[area][i];			
		}
	}
	betZhuShu = 0;
}

//大
void LotteryWuFen::pressBig(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;
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
	int startIndex = 5;
	int endIndex = 9;
	if(itemAreaIndex == 5){
		startIndex = 14;
		endIndex = 27;
	}
	for(int i = startIndex ; i <= endIndex ; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame;
		if(itemAreaIndex == 5){
			ballSpriteFrame = spriteFrame("gamble_00.png");
		}else{
			ballSpriteFrame = spriteFrame("gamble_01.png");
		}
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[itemAreaIndex][i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[itemAreaIndex][i] = 1;
		}
	}
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//小
void LotteryWuFen::pressLittle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;
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
	int startIndex = 0;
	int endIndex = 4;
	if(itemAreaIndex == 5){
		endIndex = 13;
	}
	for(int i = startIndex ; i <= endIndex ; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame;
		if(itemAreaIndex == 5){
			ballSpriteFrame = spriteFrame("gamble_00.png");
		}else{
			ballSpriteFrame = spriteFrame("gamble_01.png");
		}
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[itemAreaIndex][i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[itemAreaIndex][i] = 1;
		}
	}
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//全
void LotteryWuFen::pressAll(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;
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
	int ballNum = 10;
	if(itemAreaIndex == 5){
		ballNum = 28;
	}
	for(int i = 0 ; i < ballNum ; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame;
		if(itemAreaIndex == 5){
			ballSpriteFrame = spriteFrame("gamble_00.png");
		}else{
			ballSpriteFrame = spriteFrame("gamble_01.png");
		}
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[itemAreaIndex][i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[itemAreaIndex][i] = 1;
		}
	}
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//单
void LotteryWuFen::pressSingle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;
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
	int ballNum = 10;
	if(itemAreaIndex == 5){
		ballNum = 28;
	}
	for(int i = 1 ; i < ballNum ; i+=2)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame;
		if(itemAreaIndex == 5){
			ballSpriteFrame = spriteFrame("gamble_00.png");
		}else{
			ballSpriteFrame = spriteFrame("gamble_01.png");
		}
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[itemAreaIndex][i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[itemAreaIndex][i] = 1;
		}
	}
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//双
void LotteryWuFen::pressDouble(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;
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
	int ballNum = 10;
	if(itemAreaIndex == 5){
		ballNum = 28;
	}
	for(int i = 0 ; i < ballNum ; i+=2)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame;
		if(itemAreaIndex == 5){
			ballSpriteFrame = spriteFrame("gamble_00.png");
		}else{
			ballSpriteFrame = spriteFrame("gamble_01.png");
		}
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[itemAreaIndex][i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[itemAreaIndex][i] = 1;
		}
	}
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");

}

//清
void LotteryWuFen::pressClear(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;

	this->cancelMenuSelect(itemTag-5);
	Sprite *normalImage;
	Sprite *selectedImage;
	normalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_39.png"));
	selectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_39.png"));
	menuItem->setNormalImage(normalImage);
	menuItem->setSelectedImage(selectedImage);
	menuStatus[itemAreaIndex][5] = 1;
	
	this->clearNumbers(itemAreaIndex);
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//取消
void LotteryWuFen::cancelMenuSelect(int itemTag)
{
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;
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

void LotteryWuFen::cancelAllMenuSelect()
{
	for(int i = W_AREA_MENU_TAG ; i <= G_AREA_MENU_TAG ; i+=(Q_AREA_MENU_TAG-W_AREA_MENU_TAG))
	{
		this->cancelMenuSelect(i);
	}
}