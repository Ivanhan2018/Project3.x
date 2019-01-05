#include "HelpLayer.h"
#include "RoomLayer.h"
#include "BJK8WuFen.h"
#include "BetLimit.h"
#include "MovingLabelLayer.h"
#include "VersionControl.h"

using namespace cocos2d;

BJK8WuFen::BJK8WuFen()
{

}

BJK8WuFen::~BJK8WuFen()
{
	m_vecPot.clear();
}

Scene* BJK8WuFen::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		BJK8WuFen *layer = BJK8WuFen::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool BJK8WuFen::init()
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
		listener->onTouchBegan = CC_CALLBACK_2(BJK8WuFen::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(BJK8WuFen::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(BJK8WuFen::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(BJK8WuFen::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		m_node = Node::create();
		m_node->setPosition(Vec2(0,0));
		this->addChild(m_node,2);

		areaTitleXDelta = -5;
		fontSize = 27;
		m_num = 0;
		m_repeat = 0;	
	
		memset(ballStatus, 0, sizeof(ballStatus));
		memset(menuStatus, 0, sizeof(menuStatus));

		this->setAccelerometerEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void BJK8WuFen::onAcceleration(Acceleration* acc, Event* unused_event) 
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

void BJK8WuFen::setBetKind(BeiJingKuai8Kind kind)
{
	pt = Vec2(240, SCREEN_HEIGHT - 310);//280
	m_betKind = kind;
	switch (m_betKind)
	{
	case enBJK8_RX_1:
	case enBJK8_RX_2:
	case enBJK8_RX_3:
	case enBJK8_RX_4:
	case enBJK8_RX_5:
	case enBJK8_RX_6:
	case enBJK8_RX_7:
		initBetArea(ConfigMgr::instance()->text("display_DuangDong.xml", "t533"));		
		break;
	case enBJK8_ShangXia:
		initOtherBetArea(0, 3);
		break;
	case enBJK8_JiOu:
		initOtherBetArea(3, 3);
		break;
	case enBJK8_HeZhiDXDS:
		initOtherBetArea(6, 4);
		break;
	default:
		break;
	}

	pt.y -= 1600;
	if(pt.y>0)
	{
		m_endPos = Vec2(pt.x,0);
	}else{
		m_endPos=Vec2(pt.x,-pt.y);
	}
}

void BJK8WuFen::initBetArea(const char *titleXmlIndex)
{
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

	this->initBalls(Vec2(ballStartX, areaW->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2 - 8), W_AREA_BALL_TAG);
}

void BJK8WuFen::initOtherBetArea(const int start,const int count)
{
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//上中下奇偶和大小
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_02_2.png");	

	Vec2 pos = Vec2(ballStartX, pt.y-areaGapY-ballRadius/2-areaBkHeight/2 - 8);
	for(int i = 0 ; i < count ; i++){
		Sprite *ballSprite = nullptr;
		
		ballSprite = Sprite::createWithSpriteFrame(
			SpriteFrame::createWithTexture(ballSpriteFrame->getTexture(), Rect((i + start)*ballRadius,0,ballRadius,ballRadius)));
		
		//定位置
		ballSprite->setPosition(Vec2(pos.x+(ballRadius+ballGapX) * i,pos.y));
		ballSprite->setTag(W_AREA_BALL_TAG+i);
		m_node->addChild(ballSprite);				
	}
}

/***
数字0-80
***/
void BJK8WuFen::initBalls(Vec2 startPos, int startTag)
{
	//全部80个球
	SpriteFrame *ballSpriteFrame1 = spriteFrame("gamble_A1.png");
	SpriteFrame *ballSpriteFrame2 = spriteFrame("gamble_A2.png");
	SpriteFrame *ballSpriteFrame3 = spriteFrame("gamble_A3.png");
	SpriteFrame *ballSpriteFrame4 = spriteFrame("gamble_A4.png");

	Vec2 pos = startPos;
	for(int i = 0 ; i < 80 ; ){
		Sprite *ballSprite = nullptr;
		if(i < 20)
		{
			ballSprite = Sprite::createWithSpriteFrame(
				SpriteFrame::createWithTexture(ballSpriteFrame1->getTexture(), Rect(i*ballRadius,0,ballRadius,ballRadius)));
		}else
		if(i < 40)
		{
			ballSprite = Sprite::createWithSpriteFrame(
				SpriteFrame::createWithTexture(ballSpriteFrame2->getTexture(), Rect((i - 20)*ballRadius,0,ballRadius,ballRadius)));
		}else
		if(i < 60)
		{
			ballSprite = Sprite::createWithSpriteFrame(
				SpriteFrame::createWithTexture(ballSpriteFrame3->getTexture(), Rect((i - 40)*ballRadius,0,ballRadius,ballRadius)));
		}else
		if(i < 80)
		{
			ballSprite = Sprite::createWithSpriteFrame(
				SpriteFrame::createWithTexture(ballSpriteFrame4->getTexture(), Rect((i - 60)*ballRadius,0,ballRadius,ballRadius)));
		}
		//定位置
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
void BJK8WuFen::initMenu(Vec2 menuPos, int startTag)
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
	MenuItemSprite *bigItem = MenuItemSprite::create(bigNormalImage,bigSelectedImage,CC_CALLBACK_1(BJK8WuFen::pressBig,this));
	MenuItemSprite *littleItem = MenuItemSprite::create(littleNormalImage,littleSelectedImage,CC_CALLBACK_1(BJK8WuFen::pressLittle,this));
	MenuItemSprite *allItem = MenuItemSprite::create(allNormalImage,allSelectedImage,CC_CALLBACK_1(BJK8WuFen::pressAll,this));
	MenuItemSprite *singleItem = MenuItemSprite::create(singleNormalImage,singleSelectedImage,CC_CALLBACK_1(BJK8WuFen::pressSingle,this));
	MenuItemSprite *doubleItem = MenuItemSprite::create(doubleNormalImage,doubleSelectedImage,CC_CALLBACK_1(BJK8WuFen::pressDouble,this));
	MenuItemSprite *clearItem = MenuItemSprite::create(clearNormalImage,clearSelectedImage,CC_CALLBACK_1(BJK8WuFen::pressClear,this));
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

void BJK8WuFen::pressKindButton(int tag)
{
	CCLOG("fuck");
}

void BJK8WuFen::onEnter()
{
	Layer::onEnter();
	
}

void BJK8WuFen::onExit()
{
	//
	Layer::onExit();
}

bool BJK8WuFen::onTouchBegan(Touch *pTouch, Event *pEvent)
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
void BJK8WuFen::onTouchMoved(Touch *pTouch, Event *pEvent)
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
void BJK8WuFen::onTouchEnded(Touch *pTouch, Event *pEvent)
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

	int ballNum = 80;
	int start = 0;

	if(m_betKind == enBJK8_JiOu)
	{
		start = -3;ballNum = 3;
	} else
	if(m_betKind == enBJK8_ShangXia) 
	{
		start = 0;ballNum = 3;
	}else
	if(m_betKind == enBJK8_HeZhiDXDS)
	{
		start = -6;ballNum = 4;
	}

	for(int i = 0 ; i < ballNum ; i++)
	{			
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+i);
		if(ballSprite == NULL) continue;
		Rect rect = ballSprite->boundingBox();
		if(rect.containsPoint(endPoint)){					

			ballStatus[i] = 1-ballStatus[i];

			SpriteFrame *ballSpriteFrame;
			if(ballNum < 10)
			{
				ballSpriteFrame = spriteFrame("gamble_02_2.png");
			}else
			if(i < 20)
			{
				ballSpriteFrame = spriteFrame("gamble_A1.png");
				start = 0;
			}else
			if(i < 40)
			{
				ballSpriteFrame = spriteFrame("gamble_A2.png");
				start = 20;
			}else
			if(i < 60)
			{
				ballSpriteFrame = spriteFrame("gamble_A3.png");
				start = 40;
			}else
			if(i < 80)
			{
				ballSpriteFrame = spriteFrame("gamble_A4.png");
				start = 60;
			}			

			Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
			if(ballStatus[i] == 0){					
				ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
					Rect((i - start)*ballRadius,0,ballRadius,ballRadius)));				
			}else{					
				ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
					Rect((i - start)*ballRadius,ballRadius,ballRadius,ballRadius)));					
			}
			if(menuStatus[5] == 1){
				menuStatus[5] = 0;
				Menu *menu = (Menu *)m_node->getChildByTag(W_AREA_MENU_TAG);
				MenuItemSprite *tempItem = (MenuItemSprite *)menu->getChildByTag(W_AREA_MENU_TAG+5);
				Sprite *tempNormal = Sprite::createWithSpriteFrame(spriteFrame("gamble_38.png"));
				Sprite *tempSelected = Sprite::createWithSpriteFrame(spriteFrame("gamble_38.png"));
				tempItem->setNormalImage(tempNormal);
				tempItem->setSelectedImage(tempSelected);
			}
			NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
		} 		
	}
}

//手指触摸取消
void BJK8WuFen::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

String *BJK8WuFen::getBetNumber()
{
	String* result = NULL;

	switch (m_betKind)
	{
	case enBJK8_RX_1:
		result = this->getZhixuanBetNumber(1);
		break;
	case enBJK8_RX_2:
		result = this->getZhixuanBetNumber(2);
		break;
	case enBJK8_RX_3:
		result = this->getZhixuanBetNumber(3);
		break;
	case enBJK8_RX_4:
		result = this->getZhixuanBetNumber(4);
		break;
	case enBJK8_RX_5:
		result = this->getZhixuanBetNumber(5);
		break;
	case enBJK8_RX_6:
		result = this->getZhixuanBetNumber(6);
		break;
	case enBJK8_RX_7:
		result = this->getZhixuanBetNumber(7);
		break;
	case enBJK8_ShangXia:
		result = this->getZhixuanBetNumber(1);
		break;
	case enBJK8_JiOu:
		result = this->getZhixuanBetNumber(1);		
		break;
	case enBJK8_HeZhiDXDS:
		result = this->getZhixuanBetNumber(1);
		break;
	default:
		result = String::create("");
		break;
	}

	return result;
}

String *BJK8WuFen::getZhixuanBetNumber(int num)
{
	betZhuShu = 0; //注数
	string result = "";
	int ballNum = 0; //选择了多少个

	int tempInt[4] = {2,3,0,1};

	for(int i = 0;i < 80;i ++)
	{
		if(ballStatus[i] == 1)//已选择
		{
			if(ballNum != 0)
			{
				result.append(",");
			}
			ballNum ++;

			if(m_betKind == enBJK8_JiOu || m_betKind == enBJK8_ShangXia)
			{
				result.append(String::createWithFormat("%d", i)->getCString());
			}else
			if(m_betKind == enBJK8_HeZhiDXDS)
			{
				result.append(String::createWithFormat("%d", tempInt[i])->getCString());
			}else
			{
				result.append(String::createWithFormat("%02d", i + 1)->getCString());
			}			
		}
	}
	if(ballNum > 13)
	{
		result = "";
		betZhuShu = 0;
		MovingLabelLayer* layer =MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t1007"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}
	if(ballNum >= num && result.size() > 0)
	{
		result.append("|");
		betZhuShu = combo(ballNum,num);
	}else
	{
		result = "";
		betZhuShu = 0;
	}

	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

void BJK8WuFen::createRandomNumbers()
{
	CCLOG("BJK8WuFen::createRandomNumbers");

	switch (m_betKind)
	{
	case enBJK8_RX_1:
		createZhixuanRandomNumbers(1);
		break;
	case enBJK8_RX_2:
		createZhixuanRandomNumbers(2);
		break;
	case enBJK8_RX_3:
		createZhixuanRandomNumbers(3);
		break;
	case enBJK8_RX_4:
		createZhixuanRandomNumbers(4);
		break;
	case enBJK8_RX_5:
		createZhixuanRandomNumbers(5);
		break;
	case enBJK8_RX_6:
		createZhixuanRandomNumbers(6);
		break;
	case enBJK8_RX_7:
		createZhixuanRandomNumbers(7);
		break;
	case enBJK8_ShangXia:
		//createZhixuanRandomNumbers(1);
		break;
	case enBJK8_JiOu:
		//createZhixuanRandomNumbers(1);		
		break;
	case enBJK8_HeZhiDXDS:
		//createZhixuanRandomNumbers(1);
		break;
	default:		
		break;
	}
}

void BJK8WuFen::createZhixuanRandomNumbers(int num)
{
	this->clearNumbers();
	
	for(int j = 0 ; j < num ; j++){
		SpriteFrame *ballSpriteFrame; 	
		int ballNum = 80;		
		int ranNum = rand()%ballNum;
		while(ballStatus[ranNum] == 1){
			ranNum = rand()%ballNum;
		}
		ballStatus[ranNum] = 1;
		int start = 0;
		if(ranNum < 20)
		{
			ballSpriteFrame = spriteFrame("gamble_A1.png");	
			start = 0;
		}else
			if(ranNum < 40)
			{
				ballSpriteFrame = spriteFrame("gamble_A2.png");		
				start = 20;
			}else
				if(ranNum < 60)
				{
					ballSpriteFrame = spriteFrame("gamble_A3.png");
					start = 40;
				}else
					if(ranNum < 80)
					{
						ballSpriteFrame = spriteFrame("gamble_A4.png");	
						start = 60;
					}			
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+ranNum);
	
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballSpriteFrame->getTexture(),
			Rect((ranNum - start)*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
	
}

void BJK8WuFen::clearNumbers()
{
	int ballNum = 80;
	int start = 0;

	if(m_betKind == enBJK8_JiOu)
	{
		start = -3;ballNum = 3;
	} else
		if(m_betKind == enBJK8_ShangXia) 
		{
			start = 0;ballNum = 3;
		}else
			if(m_betKind == enBJK8_HeZhiDXDS)
			{
				start = -6;ballNum = 4;
			}

			for(int i = 0 ; i < ballNum ; i++)
			{			
				Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+i);
				if(ballSprite == NULL) continue;

				SpriteFrame *ballSpriteFrame;
				if(ballNum < 10)
				{
					ballSpriteFrame = spriteFrame("gamble_02_2.png");
				}else
					if(i < 20)
					{
						ballSpriteFrame = spriteFrame("gamble_A1.png");
						start = 0;
					}else
						if(i < 40)
						{
							ballSpriteFrame = spriteFrame("gamble_A2.png");
							start = 20;
						}else
							if(i < 60)
							{
								ballSpriteFrame = spriteFrame("gamble_A3.png");
								start = 40;
							}else
								if(i < 80)
								{
									ballSpriteFrame = spriteFrame("gamble_A4.png");
									start = 60;
								}			

								Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
								if(ballStatus[i] == 1){					
									ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
										Rect((i - start)*ballRadius,0,ballRadius,ballRadius)));				
								}
								ballStatus[i] = 0;
			}

	betZhuShu = 0;
}

void BJK8WuFen::cancelAllMenuSelect()
{
	clearNumbers();
}

//大
void BJK8WuFen::pressBig(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();
	
	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;
	this->cancelMenuSelect(itemTag);
	Sprite *normalImage;
	Sprite *selectedImage;
	normalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_29.png"));
	selectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_29.png"));
	menuItem->setNormalImage(normalImage);
	menuItem->setSelectedImage(selectedImage);
	menuStatus[0] = 1;

	//update balls
	this->clearNumbers();

	for(int i = 0 ; i < 80 ; i++)
	{
		//大的
		if(i <= 40) continue;

		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);		
		SpriteFrame *ballSpriteFrame;

		int start = 0;
		if(i < 20)
		{
			ballSpriteFrame = spriteFrame("gamble_A1.png");	
			start = 0;
		}else
			if(i < 40)
			{
				ballSpriteFrame = spriteFrame("gamble_A2.png");		
				start = 20;
			}else
				if(i < 60)
				{
					ballSpriteFrame = spriteFrame("gamble_A3.png");
					start = 40;
				}else
					if(i < 80)
					{
						ballSpriteFrame = spriteFrame("gamble_A4.png");	
						start = 60;
					}
		
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[i] == 0){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect((i - start)*ballRadius,ballRadius,ballRadius,ballRadius)));
			ballStatus[i] = 1;
		}
	}
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//小
void BJK8WuFen::pressLittle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;
	this->cancelMenuSelect(itemTag-1);
	Sprite *normalImage;
	Sprite *selectedImage;
	normalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_31.png"));
	selectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_31.png"));
	menuItem->setNormalImage(normalImage);
	menuItem->setSelectedImage(selectedImage);
	menuStatus[1] = 1;

	//update balls
	this->clearNumbers();

	for(int i = 0 ; i < 80 ; i++)
	{
		//小的
		if(i > 40) continue;

		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);		
		SpriteFrame *ballSpriteFrame;

		int start = 0;
		if(i < 20)
		{
			ballSpriteFrame = spriteFrame("gamble_A1.png");	
			start = 0;
		}else
			if(i < 40)
			{
				ballSpriteFrame = spriteFrame("gamble_A2.png");		
				start = 20;
			}else
				if(i < 60)
				{
					ballSpriteFrame = spriteFrame("gamble_A3.png");
					start = 40;
				}else
					if(i < 80)
					{
						ballSpriteFrame = spriteFrame("gamble_A4.png");	
						start = 60;
					}

					Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
					if(ballStatus[i] == 0){
						ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
							Rect((i - start)*ballRadius,ballRadius,ballRadius,ballRadius)));
						ballStatus[i] = 1;
					}
	}
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//全
void BJK8WuFen::pressAll(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;
	this->cancelMenuSelect(itemTag-2);
	Sprite *normalImage;
	Sprite *selectedImage;
	normalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_33.png"));
	selectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_33.png"));
	menuItem->setNormalImage(normalImage);
	menuItem->setSelectedImage(selectedImage);
	menuStatus[2] = 1;

	//update balls
	this->clearNumbers();

	for(int i = 0 ; i < 80 ; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);		
		SpriteFrame *ballSpriteFrame;

		int start = 0;
		if(i < 20)
		{
			ballSpriteFrame = spriteFrame("gamble_A1.png");	
			start = 0;
		}else
			if(i < 40)
			{
				ballSpriteFrame = spriteFrame("gamble_A2.png");		
				start = 20;
			}else
				if(i < 60)
				{
					ballSpriteFrame = spriteFrame("gamble_A3.png");
					start = 40;
				}else
					if(i < 80)
					{
						ballSpriteFrame = spriteFrame("gamble_A4.png");	
						start = 60;
					}

					Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
					if(ballStatus[i] == 0){
						ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
							Rect((i - start)*ballRadius,ballRadius,ballRadius,ballRadius)));
						ballStatus[i] = 1;
					}
	}
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//单
void BJK8WuFen::pressSingle(Object *obj)
{
	playButtonSound();

	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
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
	menuStatus[3] = 1;

	//update balls
	this->clearNumbers();

	for(int i = 0 ; i < 80 ; i++)
	{
		//单的
		if(i % 2 != 0) continue;

		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);		
		SpriteFrame *ballSpriteFrame;

		int start = 0;
		if(i < 20)
		{
			ballSpriteFrame = spriteFrame("gamble_A1.png");	
			start = 0;
		}else
			if(i < 40)
			{
				ballSpriteFrame = spriteFrame("gamble_A2.png");		
				start = 20;
			}else
				if(i < 60)
				{
					ballSpriteFrame = spriteFrame("gamble_A3.png");
					start = 40;
				}else
					if(i < 80)
					{
						ballSpriteFrame = spriteFrame("gamble_A4.png");	
						start = 60;
					}

					Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
					if(ballStatus[i] == 0){
						ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
							Rect((i - start)*ballRadius,ballRadius,ballRadius,ballRadius)));
						ballStatus[i] = 1;
					}
	}
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//双
void BJK8WuFen::pressDouble(Object *obj)
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
	menuStatus[4] = 1;

	//update balls
	this->clearNumbers();

	for(int i = 0 ; i < 80 ; i++)
	{
		//双的
		if(i % 2 == 0) continue;

		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+itemAreaIndex*10+i);		
		SpriteFrame *ballSpriteFrame;

		int start = 0;
		if(i < 20)
		{
			ballSpriteFrame = spriteFrame("gamble_A1.png");	
			start = 0;
		}else
			if(i < 40)
			{
				ballSpriteFrame = spriteFrame("gamble_A2.png");		
				start = 20;
			}else
				if(i < 60)
				{
					ballSpriteFrame = spriteFrame("gamble_A3.png");
					start = 40;
				}else
					if(i < 80)
					{
						ballSpriteFrame = spriteFrame("gamble_A4.png");	
						start = 60;
					}

					Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
					if(ballStatus[i] == 0){
						ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
							Rect((i - start)*ballRadius,ballRadius,ballRadius,ballRadius)));
						ballStatus[i] = 1;
					}
	}
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//清
void BJK8WuFen::pressClear(Object *obj)
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
	menuStatus[5] = 1;
	
	this->clearNumbers();
	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
}

//取消
void BJK8WuFen::cancelMenuSelect(int itemTag)
{
	int itemAreaIndex = (itemTag-W_AREA_MENU_TAG)/10;
	Menu *menu = (Menu *)m_node->getChildByTag(itemTag);
	for(int i = 0 ; i < 6 ; i++){
		if(menuStatus[i] == 1){
			MenuItemSprite *tempItem = (MenuItemSprite *)menu->getChildByTag(itemTag+i);
			Sprite *tempNormal = Sprite::createWithSpriteFrame(spriteFrame(
				String::createWithFormat("gamble_%d.png", 28+2*i)->getCString()));
			Sprite *tempSelected = Sprite::createWithSpriteFrame(spriteFrame(
				String::createWithFormat("gamble_%d.png", 28+2*i)->getCString()));
			tempItem->setNormalImage(tempNormal);
			tempItem->setSelectedImage(tempSelected);
			menuStatus[i] = 0;
		}
	}
}