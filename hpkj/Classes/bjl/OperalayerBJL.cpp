#include "OperalayerBJL.h"
#include "SimpleAudioEngine.h"
#include "CMD_BJL.h"
#include "msgDispatch/MsgDispatch.h"
#include "CocosUnits.h"
#include "BJLRes.h"
#include "MyConfig.h"
#define BTN_ANIM_TIME						0.25f								//按钮动画时间
#define BTN_PRESSED							  100								//按钮被按下
#define BTN_NOT_PRESSED						    0								//按钮没被按下

// 左下角坐标x，左下角坐标y，宽w，高h
Rect chipPosRect1[8] = {
	Rect(0,274,456,200),//0闲
	Rect(465,274,350,200),//1平
	Rect(822,274,450,200),//2庄
	Rect(160,145,290,125),//3闲天王
	Rect(830,145,290,125),//4庄天王
	Rect(460,145,360,125),//5同点平
	Rect(0,466,474,80),//6闲对子
	Rect(802,466,474,80)//7庄对子
};
float chipWidth1 = 0;
float chipHeight1 = 0;

int addRandSeedStaticPoint(Vec2 point)
{
	int  MaxAddRandSeed ;	
	for(int i = 0 ; i < 8 ; i++)
	{
		Rect rect = chipPosRect1[i];
		rect.size.width-=chipWidth1;
		rect.size.height-=chipHeight1;
		if(rect.containsPoint(point))
		{
			MaxAddRandSeed = i;
			return MaxAddRandSeed;
		}
	}
	return -1;
}
OperaLayer::OperaLayer()
{
	currentChipSprite = NULL;
	chipOfMine = Array::create();
	chipOfMine->retain();
	indexScore0 = 0;
	indexScore1 = 0;
	indexScore2 = 0;
	indexScore3 = 0;
	indexScore4 = 0;
	indexScore5 = 0;
	indexScore6 = 0;
	indexScore7 = 0;
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(OperaLayer::PlacelinitUnClickTouch), "UpdatePlacelinitUnClickTouch", NULL);
}

OperaLayer::~OperaLayer()
{
	if (chipOfMine != NULL)
	{
		CC_SAFE_DELETE(chipOfMine);
		chipOfMine = NULL;
	}
	NotificationCenter::getInstance()->removeObserver(this, "UpdatePlacelinitUnClickTouch");
}

OperaLayer * OperaLayer::create()
{
	OperaLayer *operaLayer = new OperaLayer();
	if (operaLayer&&operaLayer->initOperaLayer())
	{
		operaLayer->autorelease();
		return operaLayer;
	}	
	CC_SAFE_DELETE(operaLayer);
	return NULL;
}

bool OperaLayer::initOperaLayer()
{
	bool bRet = false;
	do 
	{
		isControl = false;
		IsXIazhuNumber = false;
		stasteat = 0;
		nodeScale = 1;
		winSize = WINSIZE;

		auto listener = EventListenerTouchOneByOne::create();

		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(OperaLayer::onTouchBegan,this);
		listener->onTouchEnded = CC_CALLBACK_2(OperaLayer::onTouchEnded,this);
		listener->onTouchMoved = CC_CALLBACK_2(OperaLayer::onTouchMoved,this);
		listener->onTouchCancelled = CC_CALLBACK_2(OperaLayer::onTouchCancelled,this);

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		anode = Node::create();	
		anode->setAnchorPoint(Vec2::ZERO);
		anode->setPosition(ccp(0,0));
		this->addChild(anode, 3);
		m_pCKPX_100Sprite_1 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pCKPX_100EnumItem)));
		m_pCKPX_100Sprite_2 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pCKPX_100EnumItem)));
		m_pCKPX_1000Sprite_1 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pCKPX_1000EnumItem)));
		m_pCKPX_1000Sprite_2 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pCKPX_1000EnumItem)));
		m_pGRYZ_1WSprite_1 =Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pGRYZ_1WEnumItem)));
		m_pGRYZ_1WSprite_2 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pGRYZ_1WEnumItem)));
		m_pGRYZ_10WSprite_1 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pGRYZ_10WEnumItem)));
		m_pGRYZ_10WSprite_2 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pGRYZ_10WEnumItem)));
		m_pCKPX_100WSprite_1 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pGRYZ_100WEnumItem)));
		m_pCKPX_100WSprite_2 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pGRYZ_100WEnumItem)));
		m_pGRYZ_500WSprite_1 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pGRYZ_500WEnumItem)));
		m_pGRYZ_500WSprite_2 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pGRYZ_500WEnumItem)));
		m_pGRYZ_1000WSprite_1 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pGRYZ_1000WEnumItem)));
		m_pGRYZ_1000WSprite_2 = Sprite::createWithSpriteFrame(spriteFrame(BJL_diss_x_png(m_pGRYZ_1000WEnumItem)));

		MyConfig& myConfig=MyConfig::Instance();

		m_pCKPX_100Image = MenuItemSprite::create(m_pCKPX_100Sprite_1,m_pCKPX_100Sprite_2,CC_CALLBACK_1(OperaLayer::menuCloseCallbackboy, this));
		m_pCKPX_100Image->setTag(m_pCKPX_100EnumItem);
		m_pCKPX_100Image->setScale(myConfig.m_ChipMenuScale);
	    m_pCKPX_1000Image = MenuItemSprite::create(m_pCKPX_1000Sprite_1,m_pCKPX_1000Sprite_2,CC_CALLBACK_1(OperaLayer::menuCloseCallbackboy, this));
		m_pCKPX_1000Image->setTag(m_pCKPX_1000EnumItem);
		m_pCKPX_1000Image->setScale(myConfig.m_ChipMenuScale);
		m_pGRYZ_1WImage = MenuItemSprite::create(m_pGRYZ_1WSprite_1,m_pGRYZ_1WSprite_2,CC_CALLBACK_1(OperaLayer::menuCloseCallbackboy, this));
		m_pGRYZ_1WImage->setTag(m_pGRYZ_1WEnumItem);  
		m_pGRYZ_1WImage->setScale(myConfig.m_ChipMenuScale);
		m_pGRYZ_10WImage = MenuItemSprite::create(m_pGRYZ_10WSprite_1,m_pGRYZ_10WSprite_2,CC_CALLBACK_1(OperaLayer::menuCloseCallbackboy, this));
		m_pGRYZ_10WImage->setTag(m_pGRYZ_10WEnumItem);
		m_pGRYZ_10WImage->setScale(myConfig.m_ChipMenuScale);
		m_pGRYZ_100WImage = MenuItemSprite::create(m_pCKPX_100WSprite_1,m_pCKPX_100WSprite_2,CC_CALLBACK_1(OperaLayer::menuCloseCallbackboy, this));
		m_pGRYZ_100WImage->setTag(m_pGRYZ_100WEnumItem);
		m_pGRYZ_100WImage->setScale(myConfig.m_ChipMenuScale);
		m_pGRYZ_500WImage = MenuItemSprite::create(m_pGRYZ_500WSprite_1,m_pGRYZ_500WSprite_2,CC_CALLBACK_1(OperaLayer::menuCloseCallbackboy, this));
		m_pGRYZ_500WImage->setTag(m_pGRYZ_500WEnumItem);
		m_pGRYZ_500WImage->setScale(myConfig.m_ChipMenuScale);
		m_pGRYZ_1000WImage = MenuItemSprite::create(m_pGRYZ_1000WSprite_1,m_pGRYZ_1000WSprite_2,CC_CALLBACK_1(OperaLayer::menuCloseCallbackboy, this));
		m_pGRYZ_1000WImage->setTag(m_pGRYZ_1000WEnumItem);
		m_pGRYZ_1000WImage->setScale(myConfig.m_ChipMenuScale);

		pCloseMenu_100 = Menu::create(m_pCKPX_100Image,NULL);
	    pCloseMenu_100->setAnchorPoint(ccp(0,0));
		pCloseMenu_100->setEnabled(false);
		pCloseMenu_100->setPosition(ccp(myConfig.m_ChipMenuPos[0][0],myConfig.m_ChipMenuPos[0][1]));
		this->addChild(pCloseMenu_100,3);
		pCloseMenu_1000 = Menu::create(m_pCKPX_1000Image,NULL);
		pCloseMenu_1000->setAnchorPoint(ccp(0,0));
		pCloseMenu_1000->setEnabled(false);
		pCloseMenu_1000->setPosition(ccp(myConfig.m_ChipMenuPos[1][0],myConfig.m_ChipMenuPos[1][1]));
		this->addChild(pCloseMenu_1000,3);	
		pCloseMenu_1W = Menu::create(m_pGRYZ_1WImage,NULL);
		pCloseMenu_1W->setAnchorPoint(ccp(0,0));
		pCloseMenu_1W->setEnabled(false);
		pCloseMenu_1W->setPosition(ccp(myConfig.m_ChipMenuPos[2][0],myConfig.m_ChipMenuPos[2][1]));
		this->addChild(pCloseMenu_1W,3);	
		pCloseMenu_10W = Menu::create(m_pGRYZ_10WImage,NULL);
		pCloseMenu_10W->setAnchorPoint(ccp(0,0));
		pCloseMenu_10W->setEnabled(false);
		pCloseMenu_10W->setPosition(ccp(myConfig.m_ChipMenuPos[3][0],myConfig.m_ChipMenuPos[3][1]));
		this->addChild(pCloseMenu_10W,3);	
		pCloseMenu_100W = Menu::create(m_pGRYZ_100WImage,NULL);
		pCloseMenu_100W->setAnchorPoint(ccp(0,0));
		pCloseMenu_100W->setEnabled(false);
		pCloseMenu_100W->setPosition(ccp(myConfig.m_ChipMenuPos[4][0],myConfig.m_ChipMenuPos[4][1]));
		this->addChild(pCloseMenu_100W,3);	
		pCloseMenu_500W= Menu::create(m_pGRYZ_500WImage,NULL);
		pCloseMenu_500W->setAnchorPoint(ccp(0,0));
		pCloseMenu_500W->setEnabled(false);
		pCloseMenu_500W->setPosition(ccp(myConfig.m_ChipMenuPos[5][0],myConfig.m_ChipMenuPos[5][1]));
		this->addChild(pCloseMenu_500W,3);		
		pCloseMenu_1000W= Menu::create(m_pGRYZ_1000WImage,NULL);
		pCloseMenu_1000W->setAnchorPoint(ccp(0,0));
		pCloseMenu_1000W->setEnabled(false);
		pCloseMenu_1000W->setPosition(ccp(myConfig.m_ChipMenuPos[6][0],myConfig.m_ChipMenuPos[6][1]));
		this->addChild(pCloseMenu_1000W,3);	

		m_pLight= Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_Light));
		m_pLight->setPosition(ccp(myConfig.m_ChipMenuPos[0][0],myConfig.m_ChipMenuPos[0][1]));
		m_pLight->setVisible(false);
		this->addChild(m_pLight,2);

		initOpenRaChipTTf();
		setTouchEnabled(true);
		bRet = true;
	}while (0);
	return bRet;
}

void OperaLayer::setChipButtonAnimation(int idx)
{
	m_pLight->setVisible(false);
	ScaleTo *pChangeSmall=ScaleTo::create(0.5f,0.8f);
	ScaleTo *pChangeBig=ScaleTo::create(0.5f,1.0f);
	CCFiniteTimeAction *pAction=Sequence::create(pChangeBig,pChangeSmall,NULL);
	RepeatForever *pRepeatForever=RepeatForever::create((CCActionInterval *)pAction);

	MyConfig& myConfig=MyConfig::Instance();
	m_pLight->stopAllActions();
	m_pLight->setVisible(true);
	m_pLight->setPosition(ccp(myConfig.m_ChipMenuPos[idx][0],myConfig.m_ChipMenuPos[idx][1]));
	m_pLight->runAction(pRepeatForever);
}

void OperaLayer::initOpenRaChipTTf()
{
	MyConfig& myConfig=MyConfig::Instance();
	for (int i = 0;i<8;i++)
	{
		m_pTotalChipValueTTf[i] = CCLabelAtlas::create("",texture_name::s_TIME_NUMBERScore,13,17,'0');//自己下注和总注用不同的数字
		m_pTotalChipValueTTf[i]->setAnchorPoint(Vec2::ZERO);
		m_pTotalChipValueTTf[i]->setColor(ccc3(255,204,0));
		m_pMySpriteNunber[i] = Sprite::createWithSpriteFrame(spriteFrame("ME_SCORE_BACK.png"));
		m_pMySpriteNunber[i]->setAnchorPoint(Vec2::ZERO);
		m_pTotalChipValueTTf[i]->setPosition(ccp(myConfig.m_OperaLayerMyNunberPos[i][0]+80,myConfig.m_OperaLayerMyNunberPos[i][1]));
		m_pMySpriteNunber[i]->setPosition(ccp(myConfig.m_OperaLayerMyNunberPos[i][0],myConfig.m_OperaLayerMyNunberPos[i][1]));
		//CCLOG("Pos%d=\"%f,%f\" ",i,m_pMySpriteNunber[i]->getPositionX(),m_pMySpriteNunber[i]->getPositionY());
		// 玩家自己在下注区域的下注额
		m_pTotalChipValueTTf[i]->setVisible(false);
		m_pMySpriteNunber[i]->setVisible(false);
		this->addChild(m_pMySpriteNunber[i],1);
       this->addChild(m_pTotalChipValueTTf[i],2);
	}
}

void OperaLayer::showOperMenut()
{
	pCloseMenu_100->setVisible(true);
	pCloseMenu_1000->setVisible(true);
	pCloseMenu_1W->setVisible(true);
	pCloseMenu_10W->setVisible(true);
	pCloseMenu_100W->setVisible(true);
	pCloseMenu_500W->setVisible(true);
	pCloseMenu_1000W->setVisible(true);
}

void OperaLayer::remaggerdisflase()
{

}

bool OperaLayer::onTouchBegan(Touch *pTouch,Event *pEvent)
{	
	return true;
}
void OperaLayer::onTouchMoved(Touch* touch,Event* event)
{
	//2.在onTouchMoved中，若布尔型的全局变量为true，说明在onTouchBegan中触点已经控制主角，并根据触摸点的横纵坐标设置主角位置
	if (isControl)
	{
		Vec2 location = touch->getLocationInView();
		location = Director::getInstance()->convertToGL(location);
		location = anode->convertTouchToNodeSpace(touch);
		currentChipSprite->setPosition(location);
		return;
		if(stasteat == 1)
		{
			pCKPX_100->setPosition(ccp(location.x, location.y));
		}else if(stasteat == 2)
		{
			pCKPX_1000->setPosition(ccp(location.x, location.y));
		}else if(stasteat == 3)
		{
			pCKPX_1W->setPosition(ccp(location.x, location.y));
		}else if(stasteat == 4)
		{
			pCKPX_10W->setPosition(ccp(location.x, location.y));
		}else if(stasteat == 5)
		{
			pCKPX_100W->setPosition(ccp(location.x, location.y));
		}else if(stasteat == 6)
		{
			pCKPX_500W->setPosition(ccp(location.x, location.y));
		}
		else if (stasteat == 7)
		{
			pCKPX_1000W->setPosition(ccp(location.x, location.y));
		}
	}
}

void OperaLayer::onTouchCancelled(Touch* touch, Event* event)
{
    NotificationCenter::getInstance()->postNotification("PrintTest2", String::createWithFormat("OperaLayer::onTouchCancelled"));
}

void OperaLayer::onTouchEnded(Touch *pTouch,Event *pEvent)
{
	//变量定义
	BYTE cbBuffer[sizeof(CMD_C_PlaceBet)];
	memset(cbBuffer, 0 , sizeof(CMD_C_PlaceBet));
	CMD_C_PlaceBet *PlaceBet = (CMD_C_PlaceBet *)cbBuffer;
	Vec2 location = pTouch->getLocationInView();
	location = Director::getInstance()->convertToGL(location);
	bool bRet = false;

	PlaceBet->cbBetArea = addRandSeedStaticPoint(location);
	if(PlaceBet->cbBetArea>=0&&PlaceBet->cbBetArea<8){
	    bRet = true;
		if (isControl)
		{
			chipOfMine->addObject(currentChipSprite);
		}
		NotificationCenter::getInstance()->postNotification("PrintTest", String::createWithFormat("x=%f,y=%f,cbBetArea=%d",location.x,location.y,PlaceBet->cbBetArea));
	}

	if (!bRet) //判断是否成功下注
	{
		if (currentChipSprite && isControl)
		{
			currentChipSprite->setVisible(false);
			chipOfMine->addObject(currentChipSprite);
		}
		NotificationCenter::getInstance()->postNotification("PrintTest", String::createWithFormat("x=%f,y=%f",location.x,location.y));
	}
	PlaceBet->lBetScore = 0;
	if (bRet == true&&isControl == true)
	{
		if (PlaceBet->cbBetArea == 0)
		{
			indexScore0+=XIazNumberScore;
			PlaceBet->lBetScore = XIazNumberScore;
			m_pTotalChipValueTTf[PlaceBet->cbBetArea]->setString(String::createWithFormat("%d",indexScore0)->getCString());
		}else if (PlaceBet->cbBetArea == 1)
		{
			indexScore1+=XIazNumberScore;
			PlaceBet->lBetScore = XIazNumberScore;
			m_pTotalChipValueTTf[PlaceBet->cbBetArea]->setString(String::createWithFormat("%d",indexScore1)->getCString());
		}else if (PlaceBet->cbBetArea == 2)
		{
			indexScore2+=XIazNumberScore;
			PlaceBet->lBetScore = XIazNumberScore;
			m_pTotalChipValueTTf[PlaceBet->cbBetArea]->setString(String::createWithFormat("%d",indexScore2)->getCString());
		}else if (PlaceBet->cbBetArea == 3)
		{
			indexScore3+=XIazNumberScore;
			PlaceBet->lBetScore = XIazNumberScore;
			m_pTotalChipValueTTf[PlaceBet->cbBetArea]->setString(String::createWithFormat("%d",indexScore3)->getCString());
		}else if (PlaceBet->cbBetArea == 4)
		{
			indexScore4+=XIazNumberScore;
			PlaceBet->lBetScore = XIazNumberScore;
			m_pTotalChipValueTTf[PlaceBet->cbBetArea]->setString(String::createWithFormat("%d",indexScore4)->getCString());
		}else if (PlaceBet->cbBetArea == 5)
		{
			indexScore5+=XIazNumberScore;
			PlaceBet->lBetScore = XIazNumberScore;
			m_pTotalChipValueTTf[PlaceBet->cbBetArea]->setString(String::createWithFormat("%d",indexScore5)->getCString());
		}else if (PlaceBet->cbBetArea == 6)
		{
			indexScore6+=XIazNumberScore;
			PlaceBet->lBetScore = XIazNumberScore;
			m_pTotalChipValueTTf[PlaceBet->cbBetArea]->setString(String::createWithFormat("%d",indexScore6)->getCString());
		}else if (PlaceBet->cbBetArea == 7)
		{
			indexScore7+=XIazNumberScore;
			PlaceBet->lBetScore = XIazNumberScore;
			m_pTotalChipValueTTf[PlaceBet->cbBetArea]->setString(String::createWithFormat("%d",indexScore7)->getCString());
		}	

		m_pTotalChipValueTTf[PlaceBet->cbBetArea]->setVisible(true);
		m_pMySpriteNunber[PlaceBet->cbBetArea]->setVisible(true);
		NotificationCenter::getInstance()->postNotification("UpdatePlacelBankScore", Integer::create(PlaceBet->lBetScore));

		pMsgDispatch->sendSocketDate(SUB_C_PLACE_JETTON,PlaceBet,sizeof(CMD_C_PlaceBet));
		CCLOG("OperaLayer::onTouchEnded cbBetArea=%d[%s],lBetScore=%d",PlaceBet->cbBetArea,BET_AREA_NAME(PlaceBet->cbBetArea),PlaceBet->lBetScore);
	}
}
void OperaLayer::initClickTouch()
{
   ManNumberScore = g_GlobalUnits.getGolbalUserData()->lScore;
   schedule(schedule_selector(OperaLayer::update),0.1f);
}

void OperaLayer::dissUnclickTouch()
{
	isControl = false;
	m_pLight->setVisible(false);
	pCloseMenu_100->setEnabled(false);
	pCloseMenu_1000->setEnabled(false);
	pCloseMenu_1W->setEnabled(false);
	pCloseMenu_10W->setEnabled(false);
	pCloseMenu_100W->setEnabled(false);
	pCloseMenu_500W->setEnabled(false);
	pCloseMenu_1000W->setEnabled(false);
	m_pCKPX_100Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pCKPX_100EnumItem)));
	m_pCKPX_1000Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pCKPX_1000EnumItem)));
	m_pGRYZ_1WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1WEnumItem)));
	m_pGRYZ_10WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_10WEnumItem)));
	m_pCKPX_100WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_100WEnumItem)));
	m_pGRYZ_500WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_500WEnumItem)));
	m_pGRYZ_1000WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1000WEnumItem)));
}

void OperaLayer::menuCloseCallbackboy( Object* obj )
{
	MyConfig& myConfig=MyConfig::Instance();
	IsXIazhuNumber = true;
	int tag = dynamic_cast<Node*>(obj)->getTag();
	isControl = true;
	//触屏检测
	setTouchEnabled(true);
	switch (tag)
	{
		case m_pCKPX_100EnumItem:
		{
			stasteat = 1;
			XIazNumberScore = CHIP_SCORE_1;
			currentChipSprite = Sprite::createWithSpriteFrame(spriteFrame(BJL_Cannger_x_png(m_pCKPX_100EnumItem)));;
			currentChipSprite->setScale(myConfig.m_ChipMenuScale);
			currentChipSprite->setTag(m_pGRYZ_ChipSpriteWEnumItem);
			currentChipSprite->setAnchorPoint(ccp(0, 0));
			currentChipSprite->setPosition(ccp(myConfig.m_ChipMenuPos[0][0],myConfig.m_ChipMenuPos[0][1]));
			chipOfMine->addObject(currentChipSprite);
			setChipButtonAnimation(tag);
			isControl = true;
			break;
		}
		case m_pCKPX_1000EnumItem:
		{
			stasteat = 2;
			XIazNumberScore = CHIP_SCORE_2;
			currentChipSprite = Sprite::createWithSpriteFrame(spriteFrame(BJL_Cannger_x_png(m_pCKPX_1000EnumItem)));
			Rect rect = currentChipSprite->boundingBox();   //获取矩形区域
			currentChipSprite->setScale(myConfig.m_ChipMenuScale);
			currentChipSprite->setAnchorPoint(ccp(0, 0));
			currentChipSprite->setPosition(ccp(myConfig.m_ChipMenuPos[1][0],myConfig.m_ChipMenuPos[1][1]));
			chipOfMine->addObject(currentChipSprite);
			currentChipSprite->setTag(m_pGRYZ_ChipSpriteWEnumItem);
			setChipButtonAnimation(tag);
			isControl = true;
			break;
		}
		case m_pGRYZ_1WEnumItem:
		{
			stasteat = 3;
			XIazNumberScore = CHIP_SCORE_3;
			currentChipSprite = Sprite::createWithSpriteFrame(spriteFrame(BJL_Cannger_x_png(m_pGRYZ_1WEnumItem)));
			Rect rect = currentChipSprite->boundingBox();   //获取矩形区域
			currentChipSprite->setScale(myConfig.m_ChipMenuScale);
			currentChipSprite->setAnchorPoint(ccp(0, 0));
			currentChipSprite->setPosition(ccp(myConfig.m_ChipMenuPos[2][0],myConfig.m_ChipMenuPos[2][1]));
			chipOfMine->addObject(currentChipSprite);
			currentChipSprite->setTag(m_pGRYZ_ChipSpriteWEnumItem);
			setChipButtonAnimation(tag);
			isControl = true;
			break;
		}
		case m_pGRYZ_10WEnumItem:
		{
			stasteat = 4;
			XIazNumberScore = CHIP_SCORE_4;
			currentChipSprite = Sprite::createWithSpriteFrame(spriteFrame(BJL_Cannger_x_png(m_pGRYZ_10WEnumItem)));
			Rect rect = currentChipSprite->boundingBox();   //获取矩形区域
			currentChipSprite->setScale(myConfig.m_ChipMenuScale);
			currentChipSprite->setTag(m_pGRYZ_ChipSpriteWEnumItem);
			currentChipSprite->setAnchorPoint(ccp(0, 0));
			currentChipSprite->setPosition(ccp(myConfig.m_ChipMenuPos[3][0],myConfig.m_ChipMenuPos[3][1]));
			chipOfMine->addObject(currentChipSprite);
			setChipButtonAnimation(tag);
			isControl = true;
			break;
		}
		case m_pGRYZ_100WEnumItem:
		{
			stasteat = 5;
			XIazNumberScore = CHIP_SCORE_5;
			currentChipSprite = Sprite::createWithSpriteFrame(spriteFrame(BJL_Cannger_x_png(m_pGRYZ_100WEnumItem)));
			Rect rect = currentChipSprite->boundingBox();   //获取矩形区域
			currentChipSprite->setScale(myConfig.m_ChipMenuScale);
			currentChipSprite->setTag(m_pGRYZ_ChipSpriteWEnumItem);
			currentChipSprite->setAnchorPoint(ccp(0, 0));
			currentChipSprite->setPosition(ccp(myConfig.m_ChipMenuPos[4][0],myConfig.m_ChipMenuPos[4][1]));
			chipOfMine->addObject(currentChipSprite);
			setChipButtonAnimation(tag);
			isControl = true;
			break;
		}
		case m_pGRYZ_500WEnumItem:
		{
			stasteat = 6;
			XIazNumberScore = CHIP_SCORE_6;
			currentChipSprite = Sprite::createWithSpriteFrame(spriteFrame(BJL_Cannger_x_png(m_pGRYZ_500WEnumItem)));
			Rect rect = currentChipSprite->boundingBox();   //获取矩形区域
			currentChipSprite->setScale(myConfig.m_ChipMenuScale);
			currentChipSprite->setTag(m_pGRYZ_ChipSpriteWEnumItem);
			currentChipSprite->setAnchorPoint(ccp(0, 0));
			currentChipSprite->setPosition(ccp(myConfig.m_ChipMenuPos[5][0],myConfig.m_ChipMenuPos[5][1]));
			chipOfMine->addObject(currentChipSprite);
			setChipButtonAnimation(tag);
			isControl = true;
			break;
		}
		case m_pGRYZ_1000WEnumItem:
		{
			stasteat = 7;
			XIazNumberScore = CHIP_SCORE_7;
			currentChipSprite = Sprite::createWithSpriteFrame(spriteFrame(BJL_Cannger_x_png(m_pGRYZ_1000WEnumItem)));
			Rect rect = currentChipSprite->boundingBox();   //获取矩形区域
			currentChipSprite->setScale(myConfig.m_ChipMenuScale);
			currentChipSprite->setTag(m_pGRYZ_ChipSpriteWEnumItem);
			currentChipSprite->setAnchorPoint(ccp(0, 0));
			currentChipSprite->setPosition(ccp(myConfig.m_ChipMenuPos[6][0],myConfig.m_ChipMenuPos[6][1]));
			chipOfMine->addObject(currentChipSprite);
			setChipButtonAnimation(tag);
			isControl = true;
			break;
		}
		default:
		{
			isControl = false;
			currentChipSprite = NULL;
			break;
		}
	}
}

void OperaLayer::resetManagerOper()
{
	for (int i = 0 ; i < chipOfMine->count() ; i++)
	{
		Node *node = (Node *)chipOfMine->objectAtIndex(i);
		node->removeFromParentAndCleanup(true);
	}
	CCLOG("ChipManagerArray has %d element", chipOfMine->count());
	chipOfMine->removeAllObjects();
	for (int i = 0;i<8;i++)
	{
		m_pTotalChipValueTTf[i]->setVisible(false);
		m_pMySpriteNunber[i]->setVisible(false);
	}
	indexScore0 = 0;
	indexScore1 = 0;
	indexScore2 = 0;
	indexScore3 = 0;
	indexScore4 = 0;
	indexScore5 = 0;
	indexScore6 = 0;
	indexScore7 = 0;
	IsXIazhuNumber = false;
}

void OperaLayer::update( float delta )
{
	unschedule(schedule_selector(OperaLayer::update));
	LONGLONG Score = ManNumberScore;
	CCLOG("wwwwwwwwwwwww = %d",Score);
	if (Score>=CHIP_SCORE_7)
	{
	   	pCloseMenu_100->setEnabled(true);
		pCloseMenu_1000->setEnabled(true);
		pCloseMenu_1W->setEnabled(true);
		pCloseMenu_10W->setEnabled(true);
		pCloseMenu_100W->setEnabled(true);
		pCloseMenu_500W->setEnabled(true);
		pCloseMenu_1000W->setEnabled(true);
		m_pCKPX_100Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_100EnumItem)));
		m_pCKPX_1000Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_1000EnumItem)));
		m_pGRYZ_1WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_1WEnumItem)));
		m_pGRYZ_10WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_10WEnumItem)));
		m_pCKPX_100WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_100WEnumItem)));
		m_pGRYZ_500WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_500WEnumItem)));
		m_pGRYZ_1000WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_1000WEnumItem)));
	}
	else if (Score>=CHIP_SCORE_6 &&Score<CHIP_SCORE_7)
	{
		pCloseMenu_100->setEnabled(true);
		pCloseMenu_1000->setEnabled(true);
		pCloseMenu_1W->setEnabled(true);
		pCloseMenu_10W->setEnabled(true);
		pCloseMenu_100W->setEnabled(true);
		pCloseMenu_500W->setEnabled(true);
		pCloseMenu_1000W->setEnabled(false);
		m_pCKPX_100Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_100EnumItem)));
		m_pCKPX_1000Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_1000EnumItem)));
		m_pGRYZ_1WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_1WEnumItem)));
		m_pGRYZ_10WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_10WEnumItem)));
		m_pCKPX_100WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_100WEnumItem)));
		m_pGRYZ_500WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_500WEnumItem)));
		m_pGRYZ_1000WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1000WEnumItem)));
	}else if (Score>=CHIP_SCORE_5 &&Score<CHIP_SCORE_6)
	{
		pCloseMenu_100->setEnabled(true);
		pCloseMenu_1000->setEnabled(true);
		pCloseMenu_1W->setEnabled(true);
		pCloseMenu_10W->setEnabled(true);
		pCloseMenu_100W->setEnabled(true);
		pCloseMenu_500W->setEnabled(false);
		pCloseMenu_1000W->setEnabled(false);
		m_pCKPX_100Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_100EnumItem)));
		m_pCKPX_1000Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_1000EnumItem)));
		m_pGRYZ_1WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_1WEnumItem)));
		m_pGRYZ_10WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_10WEnumItem)));
		m_pCKPX_100WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_100WEnumItem)));
		m_pGRYZ_500WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_500WEnumItem)));
		m_pGRYZ_1000WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1000WEnumItem)));
	}
	else if (Score>=CHIP_SCORE_4 &&Score<CHIP_SCORE_5)
	{
		pCloseMenu_100W->setEnabled(false);
		pCloseMenu_500W->setEnabled(false);
		pCloseMenu_1000W->setEnabled(false);
		pCloseMenu_100->setEnabled(true);
		pCloseMenu_1000->setEnabled(true);
		pCloseMenu_1W->setEnabled(true);
		pCloseMenu_10W->setEnabled(true);
		m_pCKPX_100Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_100EnumItem)));
		m_pCKPX_1000Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_1WEnumItem)));
		m_pGRYZ_1WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_10WEnumItem)));
		m_pGRYZ_10WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_100WEnumItem)));
		m_pCKPX_100WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_1000WEnumItem)));
		m_pCKPX_100WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_100WEnumItem)));
		m_pGRYZ_500WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_500WEnumItem)));
		m_pGRYZ_1000WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1000WEnumItem)));
	}
	else if (Score>=CHIP_SCORE_3 &&Score<CHIP_SCORE_4)
	{
		pCloseMenu_100->setEnabled(true);
		pCloseMenu_1000->setEnabled(true);
		pCloseMenu_1W->setEnabled(true);
		pCloseMenu_100W->setEnabled(false);
		pCloseMenu_500W->setEnabled(false);
		pCloseMenu_1000W->setEnabled(false);
		pCloseMenu_10W->setEnabled(false);
		m_pCKPX_100Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_100EnumItem)));
		m_pCKPX_1000Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_1000EnumItem)));
		m_pGRYZ_1WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pGRYZ_1WEnumItem)));
		m_pGRYZ_10WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_10WEnumItem)));
		m_pCKPX_100WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_100WEnumItem)));
		m_pGRYZ_500WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_500WEnumItem)));
		m_pGRYZ_1000WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1000WEnumItem)));
	}
	else if (Score>=CHIP_SCORE_2 &&Score<CHIP_SCORE_3)
	{
		pCloseMenu_100->setEnabled(true);
		pCloseMenu_1000->setEnabled(true);
		pCloseMenu_100W->setEnabled(false);
		pCloseMenu_500W->setEnabled(false);
		pCloseMenu_1000W->setEnabled(false);
		pCloseMenu_10W->setEnabled(false);
		pCloseMenu_1W->setEnabled(false);
		m_pCKPX_100Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_100EnumItem)));
		m_pCKPX_1000Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_1000EnumItem)));
		m_pGRYZ_1WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1WEnumItem)));
		m_pGRYZ_10WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_10WEnumItem)));
		m_pCKPX_100WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_100WEnumItem)));
		m_pGRYZ_500WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_500WEnumItem)));
		m_pGRYZ_1000WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1000WEnumItem)));
	}else if (Score>=CHIP_SCORE_1 &&Score<CHIP_SCORE_2)
	{
		pCloseMenu_100->setEnabled(true);
		pCloseMenu_1000->setEnabled(false);
		pCloseMenu_1W->setEnabled(false);
		pCloseMenu_10W->setEnabled(false);
		pCloseMenu_100W->setEnabled(false);
		pCloseMenu_500W->setEnabled(false);
		pCloseMenu_1000W->setEnabled(false);
		m_pCKPX_100Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_Cannger_x_png(m_pCKPX_100EnumItem)));
		m_pCKPX_1000Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pCKPX_1000EnumItem)));
		m_pGRYZ_1WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1WEnumItem)));
		m_pGRYZ_10WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_10WEnumItem)));
		m_pCKPX_100WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_100WEnumItem)));
		m_pGRYZ_500WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_500WEnumItem)));
		m_pGRYZ_1000WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1000WEnumItem)));
	}
	else if (Score>0 &&Score<CHIP_SCORE_1)
	{
		pCloseMenu_100->setEnabled(false);
		pCloseMenu_1000->setEnabled(false);
		pCloseMenu_1W->setEnabled(false);
		pCloseMenu_10W->setEnabled(false);
		pCloseMenu_100W->setEnabled(false);
		pCloseMenu_500W->setEnabled(false);
		pCloseMenu_1000W->setEnabled(false);
		m_pCKPX_100Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pCKPX_100EnumItem)));
		m_pCKPX_1000Sprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pCKPX_1000EnumItem)));
		m_pGRYZ_1WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1WEnumItem)));
		m_pGRYZ_10WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_10WEnumItem)));
		m_pCKPX_100WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_100WEnumItem)));
		m_pGRYZ_500WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_500WEnumItem)));
		m_pGRYZ_1000WSprite_1->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(BJL_diss_x_png(m_pGRYZ_1000WEnumItem)));
	}
	CCLOG("fccccc");

}

void OperaLayer::PlacelinitUnClickTouch( Object *obl )
{
	ManNumberScore = ((Integer *)obl)->getValue();
	schedule(schedule_selector(OperaLayer::update),0.1f);
}