#include "HelpLayer.h"
#include "RoomLayer.h"
#include "bjpk10WuFen.h"
#include "BetLimit.h"
#include "MovingLabelLayer.h"
#include "VersionControl.h"

using namespace cocos2d;

bjpk10WuFen::bjpk10WuFen()
{

}

bjpk10WuFen::~bjpk10WuFen()
{
	m_vecPot.clear();
}

Scene* bjpk10WuFen::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		bjpk10WuFen *layer = bjpk10WuFen::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool bjpk10WuFen::init()
{
	bool bRet = false;
	do 
	{
		if (!Layer::init())
		{
			return false;
		}
		m_node = Node::create();
		m_node->setPosition(Vec2(0,0));
		this->addChild(m_node,2);

		areaTitleXDelta = -5;
		fontSize = 27;
		for(int i = 0 ; i < MAX_NUMBER ; i++){
			memset(ballStatus[i], 0, sizeof(ballStatus[i]));
			memset(menuStatus[i], 0, sizeof(menuStatus[i]));
		}
		memset(areaStatus, 0, sizeof(areaStatus));
		 //打开重力感应
		this->setAccelerometerEnabled(true);
		bRet = true;

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(bjpk10WuFen::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(bjpk10WuFen::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(bjpk10WuFen::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(bjpk10WuFen::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
		
	} while (0);

	return bRet;
}

void bjpk10WuFen::onAcceleration(Acceleration* acc, Event* unused_event) 
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
void bjpk10WuFen::setBetKind(PK10GameKind kind)
{
	pt = Vec2(240, SCREEN_HEIGHT - 310);//280
	m_betKind = kind;
	//的玩法
	switch (m_betKind)
	{
	case WF_GuanJun:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t111"));
			break;
		}
	case WF_GuanYaJun:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t111"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t112"));
			break;
		}
	case WF_QianSan:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t111"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t112"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t113"));			
			break;
		}
	case WF_QianSi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t111"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t112"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t113"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t114"));;
			break;
		}
	case WF_QianWu:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t111"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t112"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t113"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t114"));;
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t115"));
			break;
		}
	case WF_DWD:		//1-5
		{
			//initWArea(ConfigMgr::instance()->text("display_text.xml", "t111"));
			//initQArea(ConfigMgr::instance()->text("display_text.xml", "t112"));
			//initBArea(ConfigMgr::instance()->text("display_text.xml", "t113"));
			//initSArea(ConfigMgr::instance()->text("display_text.xml", "t114"));;
			//initGWArea(ConfigMgr::instance()->text("display_text.xml", "t115"));
			initFushiArea(10);
			break;
		}
	case WF_DXDS:		//1-5
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t111"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t112"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t113"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t114"));;
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t115"));
			break;
		}
	case WF_GYHZ:		//冠亚和值
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t121"));
			break;
		}
	
	default:
		//设置单式格式
		setDanShiCount();
		//设置单式区域
		initDanshiArea();
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

void bjpk10WuFen::editBoxEditingDidBegin(EditBox* editBox)
{
	LabelTTF* resultBox = (LabelTTF*)this->getChildByTag(9528);
	if(resultBox == nullptr) return;
	editBox->setFont("",27);
	resultBox->setVisible(false);

	if(m_inputString.size() > 0) editBox->setText(m_inputString.c_str());
}

void bjpk10WuFen::editBoxEditingDidEnd(EditBox* editBox)
{
	LabelTTF* resultBox = (LabelTTF* )this->getChildByTag(9528);
	if(resultBox == nullptr) return;
    
    //先清除
    m_betString.clear();
    m_betString.append(m_inputString);
    BetLimit::SpanStringByLen(m_betString);
    resultBox->setString(m_betString.substr(0,1000));

  editBox->setText("");   

	//计算单式
	m_betString = BetLimit::GetDanShiStr(m_betString.c_str(), m_num,m_repeat,10,2,true);
	//计算注数
	betZhuShu = m_betString.size() / m_num / 3;

	if (m_betString.size() < 2)
	{
		int tempInt = atoi(m_betString.c_str());
		if(tempInt >= 0 && tempInt < 4)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
				ConfigMgr::instance()->text("display_text.xml",String::createWithFormat("t%d", tempInt + 4200)->getCString()),
				Vec2(SCREEN_WIDTH/2, 427));		
			this->addChild(layer,255);
			resultBox->setString("");
           
		}		
	} 

	NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");
	
	resultBox->setVisible(true);
	editBox->setFont("",0);
}

void bjpk10WuFen::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
	m_inputString.clear();
	m_inputString.append(text);
	/* LabelTTF* valueTTF = (LabelTTF*)this->getChildByTag(9528);
	if(valueTTF == nullptr) return;
	//先清除
	m_betString.clear();
	m_betString.append(text);
	BetLimit::SpanStringByLen(m_betString);
	valueTTF->setString(m_betString);
     */
}


void bjpk10WuFen::initDXDS(Vec2 startPos, int startTag)
{
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_011.png");
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	Vec2 pos = startPos;
	for (int i = 0; i < 4;){
		Sprite *ballSprite = Sprite::createWithSpriteFrame(
			SpriteFrame::createWithTexture(ballTexture2D, Rect(i * ballRadius,0,ballRadius,ballRadius)));
		ballSprite->setPosition(Vec2(pos.x + (ballRadius + ballGapX)*(i ), pos.y));
		ballSprite->setTag(startTag + i);
		m_node->addChild(ballSprite);
		i++;
	}
}



void bjpk10WuFen::initHeZhiBalls(Vec2 startPos, int startTag)
{
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_00.png");
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	Vec2 pos = startPos;
	for (int i = 3; i < 20;){
		int tempInt = i - 3;
		Sprite *ballSprite = Sprite::createWithSpriteFrame(
			SpriteFrame::createWithTexture(ballTexture2D, Rect(i * ballRadius,0,ballRadius,ballRadius)));
		ballSprite->setPosition(Vec2(pos.x + (ballRadius + ballGapX)*(tempInt % 7), pos.y));
		ballSprite->setTag(startTag + i);
		m_node->addChild(ballSprite);
		i++;
		if ((tempInt + 1) % 7 == 0){
			pos.x = startPos.x;
			pos.y -= (ballRadius + areaGapY);
		}
	}
}

void bjpk10WuFen::initDanshiArea()
{	
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	cocos2d::Size dtBoxSize = cocos2d::Size(SCREEN_WIDTH * 3.0f / 4.0f,SCREEN_HEIGHT / 3.0f);
	//标题
	LabelTTF* ltfText1 = LabelTTF::create();
	ltfText1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	ltfText1->setPosition(Vec2(SCREEN_WIDTH / 8, pt.y));
	ltfText1->setFontSize(27);
	ltfText1->setString(ConfigMgr::instance()->text("display_text.xml", "t1100"));
	ltfText1->setFontFillColor(Color3B::GRAY);
	this->addChild(ltfText1, 1);
	//
	pt.y = pt.y - 40;

	//修改内容
	EditBox* valueBox = EditBox::create(dtBoxSize,"blank.png");
	valueBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	valueBox->setPosition(Vec2(SCREEN_WIDTH / 2, pt.y));
	valueBox->setFontSize(2);
	valueBox->setFontName("");
	valueBox->setMaxLength(2000000000);//
	valueBox->setFontColor(Color3B::BLACK);
	valueBox->setInputFlag(EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
	valueBox->setInputMode(EditBox::InputMode::ANY);
	valueBox->setReturnType(EditBox::KeyboardReturnType::DONE);
	valueBox->setTag(9527);//设置tag8z
	valueBox->setColor(ccc3(50,50,50));
	valueBox->setDelegate(this);
	this->addChild(valueBox, 1);

	Sprite* sptValue = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
	sptValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	sptValue->setPosition(valueBox->getPosition());
	sptValue->setColor(Color3B(245,245,245));
	sptValue->setScaleX(dtBoxSize.width / sptValue->getContentSize().width);
	sptValue->setScaleY(dtBoxSize.height / sptValue->getContentSize().height);	
	this->addChild(sptValue, 2);

	LabelTTF *valueTTF = LabelTTF::create(
		"",
		"",
		27,
		dtBoxSize,
		TextHAlignment::LEFT, 
		TextVAlignment::TOP);
	valueTTF->setContentSize(dtBoxSize);
	valueTTF->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	valueTTF->setColor(Color3B::BLACK);	
	valueTTF->setTag(9528);
	valueTTF->setPosition(valueBox->getPosition());
	this->addChild(valueTTF, 2);
	valueTTF->setVisible(false);

	pt.y = pt.y - dtBoxSize.height - 40;

	LabelTTF* ltfText2 = LabelTTF::create();
	ltfText2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	ltfText2->setPosition(Vec2(SCREEN_WIDTH / 8, pt.y));
	ltfText2->setFontSize(27);
	ltfText2->setString(ConfigMgr::instance()->text("display_text.xml", "t1101"));
	ltfText2->setFontFillColor(Color3B::GRAY);
	this->addChild(ltfText2, 1);
	ltfText2->setVisible(false);

	pt.y = pt.y - 40;

	Sprite* sptValue1 = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
	sptValue1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	sptValue1->setPosition(Vec2(SCREEN_WIDTH / 2, pt.y));
	sptValue1->setColor(Color3B(245,245,245));
	sptValue1->setScaleX(dtBoxSize.width / sptValue1->getContentSize().width);
	sptValue1->setScaleY(dtBoxSize.height / sptValue1->getContentSize().height);	
	this->addChild(sptValue1, 1);
	sptValue1->setVisible(false);

	LabelTTF *resultTTF = LabelTTF::create(
		"",
		"",
		27,
		dtBoxSize,
		TextHAlignment::LEFT, 
		TextVAlignment::TOP);
	resultTTF->setContentSize(dtBoxSize);
	resultTTF->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	resultTTF->setColor(Color3B::BLACK);	
	resultTTF->setTag(9529);
	resultTTF->setPosition(Vec2(SCREEN_WIDTH / 2, pt.y));
	this->addChild(resultTTF, 1);

	valueTTF->setVisible(true);
	//sptValue->setVisible(false);
	resultTTF->setVisible(false);
	sptValue1->setVisible(false);
	ltfText2->setVisible(false);
}

void bjpk10WuFen::initFushiArea(const int max_count)
{
	if(max_count > MAX_NUMBER) return;

	for(int i = 0;i < max_count;i ++)
	{
		areaStatus[i] = 1;
		pt.y = pt.y-areaGapY-areaBkHeight/2;
		//标志
		Sprite *areaW = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
		areaW->setPosition(Vec2(areaTitlePosX, pt.y));
		m_node->addChild(areaW,0);

		this->initMenu(Vec2(areaMenuPosX, areaW->getPositionY()), AREA_MENU_TAG1 + 20 * i);

		LabelTTF *areaWTitle = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml",String::createWithFormat("t%d", 111 + i)->getCString()), "", fontSize);
		areaWTitle->setPosition(Vec2(areaW->getPositionX()+areaTitleXDelta, areaW->getPositionY()));
		areaWTitle->setColor(ccc3(167,60,60));
		m_node->addChild(areaWTitle, 0);

#ifdef VER_QIANBO
	areaWTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaWTitle->setColor(ccc3(50, 62, 123));
#endif

		//那条线
		pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
		Sprite *gapW = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
		gapW->setAnchorPoint(Vec2(0,0.5));
		gapW->setPosition(Vec2(0, pt.y));gapW->setScaleX(SCREEN_WIDTH / gapW->getContentSize().width);		
		m_node->addChild(gapW,0);

		if (m_betKind == WF_GYHZ)
		{
			this->initHeZhiBalls(Vec2(ballStartX, areaW->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG1 + 20 * i);
			gapW->setPositionY(pt.y - areaHeight-areaGapY-areaBkHeight);
		}
		else if (m_betKind == WF_DXDS)
			this->initDXDS(Vec2(ballStartX, areaW->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG1 + 20 * i);	
		else
			this->initBalls(Vec2(ballStartX, areaW->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2), AREA_BALL_TAG1 + 20 * i);
	}
}

void bjpk10WuFen::initWArea(const char *titleXmlIndex)
{
	areaStatus[0] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	//标志
	Sprite *areaW = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaW->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaW,0);

	if(m_betKind != WF_DXDS)
	this->initMenu(Vec2(areaMenuPosX, areaW->getPositionY()), AREA_MENU_TAG1);

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

	//那条线
	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapW = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapW->setAnchorPoint(Vec2(0,0.5));
	gapW->setPosition(Vec2(0, pt.y));gapW->setScaleX(SCREEN_WIDTH / gapW->getContentSize().width);
	m_node->addChild(gapW,0);

	if (m_betKind == WF_GYHZ)
	{
		this->initHeZhiBalls(Vec2(ballStartX, areaW->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG1);
		gapW->setPositionY(pt.y - areaHeight-areaGapY-areaBkHeight);
	}
	else if (m_betKind == WF_DXDS)
		this->initDXDS(Vec2(ballStartX, areaW->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG1);	
	else
		this->initBalls(Vec2(ballStartX, areaW->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2), AREA_BALL_TAG1);
}

void bjpk10WuFen::initQArea(const char *titleXmlIndex)
{
	areaStatus[1] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaQ = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaQ->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaQ,0);

	if(m_betKind != WF_DXDS)
	this->initMenu(Vec2(areaMenuPosX, areaQ->getPositionY()), AREA_MENU_TAG2);

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

	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapQ = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapQ->setAnchorPoint(Vec2(0,0.5));
	gapQ->setPosition(Vec2(0, pt.y));gapQ->setScaleX(SCREEN_WIDTH / gapQ->getContentSize().width);
	m_node->addChild(gapQ,0);

	if (m_betKind == WF_GYHZ)
		this->initHeZhiBalls(Vec2(ballStartX, areaQ->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG2);
	else if (m_betKind == WF_DXDS)
		this->initDXDS(Vec2(ballStartX, areaQ->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG2);
	else
		this->initBalls(Vec2(ballStartX, areaQ->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG2);
}

void bjpk10WuFen::initBArea(const char *titleXmlIndex)
{
	areaStatus[2] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaB = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaB->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaB,0);

	if(m_betKind != WF_DXDS)
	this->initMenu(Vec2(areaMenuPosX, areaB->getPositionY()), AREA_MENU_TAG3);

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

	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapB = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapB->setAnchorPoint(Vec2(0,0.5));
	gapB->setPosition(Vec2(0, pt.y));
	gapB->setScaleX(SCREEN_WIDTH / gapB->getContentSize().width);
	m_node->addChild(gapB,0);

	if (m_betKind == WF_GYHZ)
		this->initHeZhiBalls(Vec2(ballStartX, areaB->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG2);
	else if(m_betKind == WF_DXDS)
		this->initDXDS(Vec2(ballStartX, areaB->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG3);
	else
		this->initBalls(Vec2(ballStartX, areaB->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG3);
}

void bjpk10WuFen::initSArea(const char *titleXmlIndex)
{
	areaStatus[3] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaS = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaS->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaS,0);

	if(m_betKind != WF_DXDS)
	this->initMenu(Vec2(areaMenuPosX, areaS->getPositionY()), AREA_MENU_TAG4);

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

	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapS = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapS->setAnchorPoint(Vec2(0,0.5));
	gapS->setPosition(Vec2(0, pt.y));gapS->setScaleX(SCREEN_WIDTH / gapS->getContentSize().width);
	m_node->addChild(gapS,0);

	if (m_betKind == WF_GYHZ)
		this->initHeZhiBalls(Vec2(ballStartX, areaS->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG4);
	else if (m_betKind == WF_DXDS)
		this->initDXDS(Vec2(ballStartX, areaS->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG4);
	else
		this->initBalls(Vec2(ballStartX, areaS->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG4);
}

void bjpk10WuFen::initGWArea(const char *titleXmlIndex)
{
	areaStatus[4] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaG = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaG->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaG,0);

	if(m_betKind != WF_DXDS)
	this->initMenu(Vec2(areaMenuPosX, areaG->getPositionY()), AREA_MENU_TAG5);

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

	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapG = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapG->setAnchorPoint(Vec2(0,0.5));
	gapG->setPosition(Vec2(0, pt.y));gapG->setScaleX(SCREEN_WIDTH / gapG->getContentSize().width);
	m_node->addChild(gapG,0);

	if (m_betKind == WF_GYHZ)
		this->initHeZhiBalls(Vec2(ballStartX, areaG->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG5);
	else if (m_betKind == WF_DXDS)
		this->initDXDS(Vec2(ballStartX, areaG->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG5);
	else
		this->initBalls(Vec2(ballStartX, areaG->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG5);
}

void bjpk10WuFen::initBalls(Vec2 startPos, int startTag)
{
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_02.png");
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

void bjpk10WuFen::initMenu(Vec2 menuPos, int startTag)
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
	MenuItemSprite *bigItem = MenuItemSprite::create(bigNormalImage,bigSelectedImage,CC_CALLBACK_1(bjpk10WuFen::pressBig,this));
	MenuItemSprite *littleItem = MenuItemSprite::create(littleNormalImage,littleSelectedImage,CC_CALLBACK_1(bjpk10WuFen::pressLittle,this));
	MenuItemSprite *allItem = MenuItemSprite::create(allNormalImage,allSelectedImage,CC_CALLBACK_1(bjpk10WuFen::pressAll,this));
	MenuItemSprite *singleItem = MenuItemSprite::create(singleNormalImage,singleSelectedImage,CC_CALLBACK_1(bjpk10WuFen::pressSingle,this));
	MenuItemSprite *doubleItem = MenuItemSprite::create(doubleNormalImage,doubleSelectedImage,CC_CALLBACK_1(bjpk10WuFen::pressDouble,this));
	MenuItemSprite *clearItem = MenuItemSprite::create(clearNormalImage,clearSelectedImage,CC_CALLBACK_1(bjpk10WuFen::pressClear,this));
	bigItem->setTag(startTag);
	littleItem->setTag(startTag+1);
	allItem->setTag(startTag+2);
	singleItem->setTag(startTag+3);
	doubleItem->setTag(startTag+4);
	clearItem->setTag(startTag+5);
	Menu *menu = Menu::create(bigItem,littleItem,allItem,singleItem,doubleItem,clearItem,NULL);
	menu->setTag(startTag);
	menu->setPosition(menuPos);	
	menu->setPosition(Vec2(menuPos.x + 200,menuPos.y));	//后移200px
	menu->alignItemsHorizontallyWithPadding(10);//
	m_node->addChild(menu,0);
}

void bjpk10WuFen::pressKindButton(int tag)
{
	CCLOG("fuck");
}

void bjpk10WuFen::onEnter()
{
	Layer::onEnter();
	//
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this,-128);	
}

void bjpk10WuFen::onExit()
{
	//
	Layer::onExit();
}

bool bjpk10WuFen::onTouchBegan(Touch *pTouch, Event *pEvent)
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
void bjpk10WuFen::onTouchMoved(Touch *pTouch, Event *pEvent)
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

//手指触摸事件
void bjpk10WuFen::onTouchEnded(Touch *pTouch, Event *pEvent)
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

	if(this->getChildByTag(9528) != nullptr) return;	

	int ballNum = 0;
	for(int j = 0 ; j < MAX_NUMBER ; j++)
	{
		if (m_betKind == WF_DXDS)		//大小单双
			ballNum = 4;
		else if (m_betKind == WF_GYHZ)	//冠亚和值
			ballNum = 20;
		else
			ballNum = 10;				//1-10
		for (int i = 0; i < ballNum; i++)
		{
			if(areaStatus[j] == 0)
			{
				continue;
			}
			Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+j*AREA_GAP+i);
			if (ballSprite == NULL)
				continue;

			Rect rect = ballSprite->boundingBox();
			if(rect.containsPoint(endPoint)){
				ballStatus[j][i] = 1-ballStatus[j][i];

				SpriteFrame *ballSpriteFrame = NULL;
				if (m_betKind == WF_DXDS)		//大小单双
					ballSpriteFrame = spriteFrame("gamble_011.png");
				else if (m_betKind == WF_GYHZ)
					ballSpriteFrame = spriteFrame("gamble_00.png");
				else
					ballSpriteFrame = spriteFrame("gamble_02.png");
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
					Menu *menu = (Menu *)m_node->getChildByTag(AREA_MENU_TAG1+j*AREA_GAP);
					MenuItemSprite *tempItem = (MenuItemSprite *)menu->getChildByTag(AREA_MENU_TAG1+j*AREA_GAP+5);
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

void bjpk10WuFen::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

String *bjpk10WuFen::getBetNumber()
{
	switch(m_betKind)
	{
	case WF_GuanJun:						//冠军
		{
			return this->getGuanJunBetNumber();
			break;
		}
	case WF_GuanYaJun:						//冠亚军
		{
			return this->getGuanYaJunBetNumber();
			break;
		}
	case WF_QianSan:						//前三
		{
			return this->getQianSanBetNumber();
			break;
		}
	case WF_QianSi:							//前四
		{
			return this->getQianSiBetNumber();
			break;
		}
	case WF_QianWu:							//前五
		{
			return this->getQianWuBetNumber();
			break;
		}
	case WF_DWD:							//定位胆
		{
			return this->getDingWeiDanBetNumber();
			break;
		}
	case WF_DXDS:							//大小单双
		{
			return this->getDaXiaoDanShuangBetNumber();
			break;
		}
	case WF_GYHZ:							//冠亚和值
		{
			return this->getGuanYaHeZhiBetNumber();
			break;
		}
	//////////////////////////////////////////////////////////////
	case QianEr_ZuXuan:				//前二组选选号
		{
			return this->getQianEr_ZuXuanBetNumber();
			break;
		}
	case QianSan_ZuXuan:       //前三组选选号
		{
			return this->getQianSan_ZuXuanBetNumber();
			break;
		}
	default:
		return this->getDanShiBetNumber();
		break;
	}
	return String::createWithFormat("");
}

//冠军
String* bjpk10WuFen::getGuanJunBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[11][3] = {{"01"},{"02"},{"03"},{"04"},{"05"},{"06"},{"07"},{"08"},{"09"},{"10"},{"11"}};

	int ballNum = 0;
	int oidfjgvbiodf = ballStatus[0][2];
	for(int i = 0 ; i < 10 ; i++)
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

	if(ballNum <1)
	{
		result = "";
		betZhuShu = 0;
		String *resultStr = String::createWithFormat("%s", result.c_str());
		return resultStr;
	}
	else{
		betZhuShu = ballNum;
	}
	if(betZhuShu<1){
		betZhuShu = 0;
	}
	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//冠亚
String* bjpk10WuFen::getGuanYaJunBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[11][3] = { { "01" }, { "02" }, { "03" }, { "04" }, { "05" }, { "06" }, { "07" }, { "08" }, { "09" }, { "10" }, { "11" } };
	
	int ballNum1 = 0;
	int ballNum2 = 0;
	int repeatNum = 0;

	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[0][i] == 1)
		{
			result.append(numberStr[i]);
			ballNum1++;
		}
	}
	//注数
	betZhuShu *= ballNum1;
	//分隔符
	result.append(",");
	//
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[1][i] == 1)
		{
			result.append(numberStr[i]);
			ballNum2++;
			//在相同的位置
			if(ballStatus[0][i] == 1) repeatNum ++;
		}
	}
	betZhuShu = betZhuShu * ballNum2 - repeatNum;
	
	if (betZhuShu<1){
		betZhuShu = 0;
	}
	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}
//前三
String* bjpk10WuFen::getQianSanBetNumber()
{
	betZhuShu = 0;
	string result = "";
	char numberStr[11][3] = { { "01" }, { "02" }, { "03" }, { "04" }, { "05" }, { "06" }, { "07" }, { "08" }, { "09" }, { "10" }, { "11" } };

	//第一位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[0][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}
	result.append(",");
	//第二位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[1][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}
	result.append(",");
	//第三位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[2][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}	
	//计算注数
	for(int i = 0;i < 10;i ++)
	{
		if(ballStatus[0][i] == 0) continue;
		for(int j = 0;j < 10;j ++)
		{
			if(ballStatus[1][j] == 0) continue;
			if(j == i) continue;
			for(int k = 0;k < 10;k ++)
			{
				if(ballStatus[2][k] == 0) continue;
				if(k == i || k == j) continue;

				betZhuShu ++;
			}
		}
	}
	

	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//前四
String* bjpk10WuFen::getQianSiBetNumber()
{
	betZhuShu = 0;
	string result = "";
	char numberStr[11][3] = { { "01" }, { "02" }, { "03" }, { "04" }, { "05" }, { "06" }, { "07" }, { "08" }, { "09" }, { "10" }, { "11" } };

	//第一位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[0][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}
	result.append(",");
	//第二位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[1][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}
	result.append(",");
	//第三位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[2][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}	
	result.append(",");
	//第四位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[3][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}
	//计算注数
	for(int i = 0;i < 10;i ++)
	{
		if(ballStatus[0][i] == 0) continue;
		for(int j = 0;j < 10;j ++)
		{
			if(ballStatus[1][j] == 0) continue;
			if(j == i) continue;
			for(int k = 0;k < 10;k ++)
			{
				if(ballStatus[2][k] == 0) continue;
				if(k == i || k == j) continue;
				for(int m = 0;m < 10;m ++)
				{
					if(ballStatus[3][m] == 0) continue;
					if(m == i || m == j || m == k) continue;
					betZhuShu ++;
				}
			}
		}
	}


	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//前五
String* bjpk10WuFen::getQianWuBetNumber()
{
	betZhuShu = 0;
	string result = "";
	char numberStr[11][3] = { { "01" }, { "02" }, { "03" }, { "04" }, { "05" }, { "06" }, { "07" }, { "08" }, { "09" }, { "10" }, { "11" } };

	//第一位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[0][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}
	result.append(",");
	//第二位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[1][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}
	result.append(",");
	//第三位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[2][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}	
	result.append(",");
	//第四位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[3][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}
	result.append(",");
	//第五位
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[4][i] == 1)
		{
			result.append(numberStr[i]);			
		}	
	}
	//计算注数
	for(int i = 0;i < 11;i ++)
	{
		if(ballStatus[0][i] == 0) continue;
		for(int j = 0;j < 11;j ++)
		{
			if(ballStatus[1][j] == 0) continue;
			if(j == i) continue;
			for(int k = 0;k < 10;k ++)
			{
				if(ballStatus[2][k] == 0) continue;
				if(k == i || k == j) continue;
				for(int m = 0;m < 10;m ++)
				{
					if(ballStatus[3][m] == 0) continue;
					if(m == i || m == j || m == k) continue;
					for(int n = 0;n < 10;n ++)
					{
						if(ballStatus[4][n] == 0) continue;
						if(n == i || n == j || n == k || n == m) continue;
						betZhuShu ++;
					}					
				}
			}
		}
	}

	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//定位胆
String* bjpk10WuFen::getDingWeiDanBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[11][3] = {{"01"},{"02"},{"03"},{"04"},{"05"},{"06"},{"07"},{"08"},{"09"},{"10"},{"11"}};

	int ballNum = 0;

	for (int j = 0; j < MAX_NUMBER; j++){
		for (int i = 0; i < 10; i++)
		{
			if (ballStatus[j][i] == 1)
			{
				result += numberStr[i];
				ballNum++;
			}
		}
		if (j != 9){
			result += ",";
		}
	}

	if (ballNum >= 1){
		betZhuShu *= ballNum;
	}
	else{
		result = "";
		betZhuShu = 0;
	}

	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//大小单双
String* bjpk10WuFen::getDaXiaoDanShuangBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[4][3] = { { "9" }, { "1" }, { "3" }, { "2" } };

	int ballNum = 0;

	for (int j = 0; j < 5; j++){
		for (int i = 0; i < 4; i++)
		{
			if (ballStatus[j][i] == 1)
			{
				result += numberStr[i];
				ballNum++;
			}
		}
		if (j != 4){
			result += ",";
		}
	}

	if (ballNum >= 1){
		betZhuShu *= ballNum;
	}
	else{
		result = "";
		betZhuShu = 0;
	}

	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//冠亚和值
String* bjpk10WuFen::getGuanYaHeZhiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[20][3] = { {"0"},{ "01" }, { "02" }, { "03" }, { "04" }, { "05" }, { "06" }, { "07" }, { "08" }, { "09" }, { "10" }, { "11" }, { "12" }, { "13" }, { "14" }, { "15" }, { "16" }, { "17" }, { "18" }, { "19" } };

	int ballNum = 0;
	int oidfjgvbiodf = ballStatus[0][2];
	for(int i = 0 ; i < 20 ; i++)
	{
		if(ballStatus[0][i] == 1)
		{
			if(ballNum>0){
				result+=",";
			}
			result+=numberStr[i];
			for (int m = 1; m < 11; m++)
			{
				for (int n = m; n < 11; n++)
				{
					if (m == n)
						continue;
					if ((m + n ) == atoi(numberStr[i]))
					{
						ballNum++;
					}
				}
			}
		}
	}

	if(ballNum < 1){
		result = "";
		betZhuShu = 0;
	}else{
		betZhuShu = ballNum;
	}
	if(betZhuShu<1){
		betZhuShu = 0;
	}
	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//暂时未用
////////////////////////////////////////////////////////////////////////////////
String* bjpk10WuFen::getQianEr_ZuXuanBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[11][3] = {{"01"},{"02"},{"03"},{"04"},{"05"},{"06"},{"07"},{"08"},{"09"},{"10"},{"11"}};

	int ballNum = 0;
	int oidfjgvbiodf = ballStatus[0][2];
	for(int i = 0 ; i < 10 ; i++)
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
	if(ballNum < 2){
		result = "ERZU";
		betZhuShu = 0;	
		String *resultStr = String::createWithFormat("%s", result.c_str());
	    return resultStr;
	}else{
		betZhuShu = this->combo(ballNum, 2)*2;
	}
	if(betZhuShu<1){
		betZhuShu = 0;
	}
	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String* bjpk10WuFen::getQianSan_ZuXuanBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[11][3] = {{"01"},{"02"},{"03"},{"04"},{"05"},{"06"},{"07"},{"08"},{"09"},{"10"},{"11"}};

	int ballNum = 0;
	int oidfjgvbiodf = ballStatus[0][2];
	for(int i = 0 ; i < 10 ; i++)
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

	if(ballNum < 3){
		result = "SANZU";
		betZhuShu = 0;
		String *resultStr = String::createWithFormat("%s", result.c_str());
	    return resultStr;
	}else{
		betZhuShu = this->combo(ballNum, 3);
	}
	if(betZhuShu<1){
		betZhuShu = 0;
	}
	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String* bjpk10WuFen::getDanShiBetNumber()
{	
	//创建字符串
	String* result = String::create(m_betString);

	if(m_betString.size() < 2) result = String::create("");

	return result;
}

void bjpk10WuFen::setDanShiCount()
{
	switch (m_betKind)
	{
	case    WF_GuanYaJun_DanShi://冠亚军单式
		m_num = 2;
		m_repeat = 2;
		break;		
	case    WF_QianSan_DanShi:
		m_num = 3;
		m_repeat = 2;
		break;			
	case    WF_QianSi_DanShi:
		m_num = 4;
		m_repeat = 2;
		break;
	case    WF_QianWu_DanShi:
		m_num = 5;
		m_repeat = 2;
		break;
	default:
		m_num = 0;
		m_repeat = 0;
		break;
	}
}

void bjpk10WuFen::createDanShiRandomNumber()
{
	this->clearNumbers(0);

	betZhuShu = 0;
}

void bjpk10WuFen::createRandomNumbers()
{
	CCLOG("bjpk10WuFen::createRandomNumbers");
	CCLOG("AAAAAAAA == %d", m_betKind);

	if(this->getChildByTag(9528) != nullptr)
	{
		createDanShiRandomNumber();
		return;
	}

	this->clearNumbers();
	SpriteFrame *ballSpriteFrame = NULL;
	
	int nBallNum = 0;
	if (m_betKind == WF_DXDS)
	{
		nBallNum = 4;
		ballSpriteFrame = spriteFrame("gamble_011.png");
	}
	else if (m_betKind == WF_GYHZ)
	{
		ballSpriteFrame = spriteFrame("gamble_00.png");
		nBallNum = 20;
	}
	else
	{
		ballSpriteFrame = spriteFrame("gamble_02.png");
		nBallNum = 10;
	}
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	for(int j = 0 ; j < MAX_NUMBER; j++)
	{
		if(areaStatus[j] == 0){
			continue;
		}
		int num = rand() % nBallNum;
		ballStatus[j][num] = 1-ballStatus[j][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+j*AREA_GAP+num);
		if (ballSprite == NULL)
			continue;
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}
void bjpk10WuFen::createRenXuanRandomNumbers()
{
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_02.png");
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	for(int j = 0 ; j < MAX_NUMBER ; j++){
		if(areaStatus[j] == 0){
			continue;
		}
		for(int i = 0 ; i < m_betKind+1 ; i++){
			int num = rand()%BALL_NUM;
			while(ballStatus[j][num] == 1){
				num = rand()%BALL_NUM;
			}
			ballStatus[j][num] = 1-ballStatus[j][num];
			Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+j*AREA_GAP+num);
			Rect rect = ballSprite->boundingBox();
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
		}
	}
}
void bjpk10WuFen::createQianEr_ZuXuanRandomNumbers()
{
	this->clearNumbers();
	for(int j = 0 ; j < 2 ; j++){
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
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}
void bjpk10WuFen::createQianSan_ZuXuanRandomNumbers()
{
	this->clearNumbers();
	for(int j = 0 ; j < 3 ; j++){
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
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}

void bjpk10WuFen::clearNumbers()
{
	SpriteFrame *ballSpriteFrame = NULL;
	int nBallNum = 0;
	if (m_betKind == WF_DXDS)
	{
		nBallNum = 4;
		ballSpriteFrame = spriteFrame("gamble_011.png");
	}
	else if (m_betKind == WF_GYHZ)
	{
		ballSpriteFrame = spriteFrame("gamble_00.png");
		nBallNum = 20;
	}
	else
	{
		ballSpriteFrame = spriteFrame("gamble_02.png");
		nBallNum = 10;
	}
	for(int j = 0 ; j < MAX_NUMBER ; j++)
	{
		for (int i = 0; i < nBallNum; i++)
		{
			if(areaStatus[j] == 0){
				continue;
			}
			Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+j*AREA_GAP+i);
			if (ballSprite == NULL)
				continue;
			Rect rect = ballSprite->boundingBox();
			
			Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
			if(ballStatus[j][i] == 1){
				ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
					Rect(i*ballRadius,0,ballRadius,ballRadius)));
				ballStatus[j][i] = 1-ballStatus[j][i];
			}
		}
	}

	m_betString = "";
	betZhuShu = 0;
	m_inputString  = "";

	EditBox* box1 = (EditBox *)this->getChildByTag(9527);
	if(box1 != nullptr)	box1->setText("");
	LabelTTF* ltf1 = (LabelTTF *)this->getChildByTag(9528);
	if(ltf1 != nullptr) ltf1->setString("");
	LabelTTF* ltf2 = (LabelTTF *)this->getChildByTag(9529);
	if(ltf2 != nullptr) ltf2->setString("");
}
void bjpk10WuFen::cancelAllMenuSelect()
{
	for(int i = AREA_MENU_TAG1 ; i <= AREA_MENU_TAG5 ; i+=AREA_GAP)
	{
		this->cancelMenuSelect(i);
	}
}
void bjpk10WuFen::clearNumbers(int area)
{
	SpriteFrame *ballSpriteFrame = NULL;
	int nBallNum = 0;
	if (m_betKind == WF_DXDS)
	{
		nBallNum = 4;
		ballSpriteFrame = spriteFrame("gamble_011.png");
	}
	else if (m_betKind == WF_GYHZ)
	{
		ballSpriteFrame = spriteFrame("gamble_00.png");
		nBallNum = 20;
	}
	else
	{
		ballSpriteFrame = spriteFrame("gamble_02.png");
		nBallNum = 10;
	}

	for (int i = 0; i < nBallNum; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+area*AREA_GAP+i);
		if (ballSprite == NULL)	continue;
		Rect rect = ballSprite->boundingBox();
		
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[area][i] == 1){
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
				Rect(i*ballRadius,0,ballRadius,ballRadius)));
			ballStatus[area][i] = 1-ballStatus[area][i];
		}
	}

	m_betString = "";
	betZhuShu = 0;
	m_inputString  = "";

	EditBox* box1 = (EditBox *)this->getChildByTag(9527);
	if(box1 != nullptr)	box1->setText("");
	LabelTTF* ltf1 = (LabelTTF *)this->getChildByTag(9528);
	if(ltf1 != nullptr) ltf1->setString("");
	LabelTTF* ltf2 = (LabelTTF *)this->getChildByTag(9529);
	if(ltf2 != nullptr) ltf2->setString("");
}

//大
void bjpk10WuFen::pressBig(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == WF_DXDS || m_betKind == WF_GYHZ)
		return;

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-AREA_MENU_TAG1)/AREA_GAP;
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
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+itemAreaIndex*AREA_GAP+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_02.png");
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
void bjpk10WuFen::pressLittle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == WF_DXDS || m_betKind == WF_GYHZ)
		return;

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-AREA_MENU_TAG1)/AREA_GAP;
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
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+itemAreaIndex*AREA_GAP+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_02.png");
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

void bjpk10WuFen::pressAll(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == WF_DXDS || m_betKind == WF_GYHZ)
		return;

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-AREA_MENU_TAG1)/AREA_GAP;
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
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+itemAreaIndex*AREA_GAP+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_02.png");
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
void bjpk10WuFen::pressSingle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == WF_DXDS || m_betKind == WF_GYHZ)
		return;

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-AREA_MENU_TAG1)/AREA_GAP;
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
	for(int i = 0 ; i < 10 ; i+=2)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+itemAreaIndex*AREA_GAP+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_02.png");
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
void bjpk10WuFen::pressDouble(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == WF_DXDS || m_betKind == WF_GYHZ)
		return;

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-AREA_MENU_TAG1)/AREA_GAP;
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
	for(int i = 1 ; i < 10 ; i+=2)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+itemAreaIndex*AREA_GAP+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_02.png");
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
void bjpk10WuFen::pressClear(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-AREA_MENU_TAG1)/AREA_GAP;

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

void bjpk10WuFen::cancelMenuSelect(int itemTag)
{
	int itemAreaIndex = (itemTag-AREA_MENU_TAG1)/AREA_GAP;
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