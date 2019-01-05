#include "HelpLayer.h"
#include "RoomLayer.h"
#include "FC3DPL3WuFen.h"
#include "BetLimit.h"
#include "MovingLabelLayer.h"
#include "VersionControl.h"

using namespace cocos2d;

FC3DPL3WuFen::FC3DPL3WuFen()
{

}

FC3DPL3WuFen::~FC3DPL3WuFen()
{
	m_vecPot.clear();
}

Scene* FC3DPL3WuFen::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		FC3DPL3WuFen *layer = FC3DPL3WuFen::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool FC3DPL3WuFen::init()
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
		listener->onTouchBegan = CC_CALLBACK_2(FC3DPL3WuFen::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(FC3DPL3WuFen::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(FC3DPL3WuFen::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(FC3DPL3WuFen::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		m_node = Node::create();
		m_node->setPosition(Vec2(0,0));
		this->addChild(m_node,2);

		areaTitleXDelta = -5;
		fontSize = 27;
		for(int i = 0 ; i < 3 ; i++){
			memset(ballStatus[i], 0, sizeof(ballStatus[i]));
			memset(menuStatus[i], 0, sizeof(menuStatus[i]));
		}
		memset(areaStatus, 0, sizeof(areaStatus));
		 //打开重力感应
		this->setAccelerometerEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void FC3DPL3WuFen::onAcceleration(Acceleration* acc, Event* unused_event) 
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
void FC3DPL3WuFen::setBetKind(ThreeNumbersKind kind)
{
	pt = Vec2(240, SCREEN_HEIGHT - 310);//280
	m_betKind = kind;
	//的玩法
	switch (m_betKind)
	{
	case enWF_Budingwei:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t127"));

			break;
		}
	case enWF_QiansanzuliuFushi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t80"));

			break;
		}
	case enWF_QiansanzusanFushi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t80"));

			break;
		}
	case enWF_ZhixuanFushi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t94"));

			break;
		}
	case enWF_QianyizhixuanFushi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t92"));

			break;
		}
	case enWF_QianerzhixuanFushi:
	{
		initWArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
		initQArea(ConfigMgr::instance()->text("display_text.xml", "t93"));

		break;
	}
	case enWF_HouyizhixuanFushi:
	{
		initBArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
		break;
	}
	case enWF_HouerzhixuanFushi:
	{
		initQArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
		initBArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
		break;
	}
	case enWF_DingWeiDan:		//1-5
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t94"));

			break;
		}
	case enWF_DaxiaoDanshuang:		//1-5
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t94"));

			break;
		}
	case enWF_ZhixuanHezhi:		//冠亚和值
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t100"));
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
	//this->setTouchEnabled(true);
}

void FC3DPL3WuFen::editBoxEditingDidBegin(EditBox* editBox)
{
	LabelTTF* resultBox = (LabelTTF*)this->getChildByTag(9528);
	if(resultBox == nullptr) return;
	editBox->setFont("",27);
	resultBox->setVisible(false);

	if(m_inputString.size() > 0) editBox->setText(m_inputString.c_str());
}

void FC3DPL3WuFen::editBoxEditingDidEnd(EditBox* editBox)
{


    LabelTTF* resultBox = (LabelTTF* )this->getChildByTag(9528);
	if(resultBox == nullptr) return;
    m_betString.clear();
    m_betString.append(m_inputString);
    BetLimit::SpanStringByLen(m_betString);
	//清空
	editBox->setText("");    
    resultBox->setString(m_betString.substr(0,1000));
    
	//计算单式
	m_betString = BetLimit::GetDanShiStr(m_betString.c_str(), m_num,m_repeat,9);
	//计算注数
	betZhuShu = m_betString.size() / m_num / 2;	

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
}

void FC3DPL3WuFen::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
	m_inputString.clear();
	m_inputString.append(text);

	/*LabelTTF* valueTTF = (LabelTTF*)this->getChildByTag(9528);
	if(valueTTF == nullptr) return;
	//先清除
	m_betString.clear();
	m_betString.append(text);
	BetLimit::SpanStringByLen(m_betString);
	valueTTF->setString(m_betString);
	*/
}


void FC3DPL3WuFen::initDXDS(Vec2 startPos, int startTag)
{
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_011.png");
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	Vec2 pos = startPos;
	for (int i = 0; i < 4;){
		Sprite *ballSprite = Sprite::createWithSpriteFrame(
			SpriteFrame::createWithTexture(ballTexture2D, Rect(i * ballRadius,0,ballRadius,ballRadius)));
		ballSprite->setPosition(Vec2(pos.x + (ballRadius + ballGapX)*(i), pos.y));
		ballSprite->setTag(startTag + i);
		m_node->addChild(ballSprite);
		i++;
	}
}



void FC3DPL3WuFen::initHeZhiBalls(Vec2 startPos, int startTag)
{
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_00.png");
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	Vec2 pos = startPos;
	for (int i = 0; i < 28;){
		Sprite *ballSprite = Sprite::createWithSpriteFrame(
			SpriteFrame::createWithTexture(ballTexture2D, Rect(i * ballRadius,0,ballRadius,ballRadius)));
		ballSprite->setPosition(Vec2(pos.x + (ballRadius + ballGapX)*(i % 7), pos.y));
		ballSprite->setTag(startTag + i);
		m_node->addChild(ballSprite);
		i++;
		if (i % 7 == 0){
			pos.x = startPos.x;
			pos.y -= (ballRadius + areaGapY);
		}
	}
}

void FC3DPL3WuFen::initDanshiArea()
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

	pt.y = pt.y - dtBoxSize.height - 40;

	LabelTTF* ltfText2 = LabelTTF::create();
	ltfText2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	ltfText2->setPosition(Vec2(SCREEN_WIDTH / 8, pt.y));
	ltfText2->setFontSize(27);
	ltfText2->setString(ConfigMgr::instance()->text("display_text.xml", "t1101"));
	ltfText2->setFontFillColor(Color3B::GRAY);
	this->addChild(ltfText2, 1);

	pt.y = pt.y - 40;

	Sprite* sptValue1 = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
	sptValue1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	sptValue1->setPosition(Vec2(SCREEN_WIDTH / 2, pt.y));
	sptValue1->setColor(Color3B(245,245,245));
	sptValue1->setScaleX(dtBoxSize.width / sptValue1->getContentSize().width);
	sptValue1->setScaleY(dtBoxSize.height / sptValue1->getContentSize().height);	
	this->addChild(sptValue1, 1);

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

void FC3DPL3WuFen::initWArea(const char *titleXmlIndex)
{
	areaStatus[0] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	//标志
	Sprite *areaW = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaW->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaW,0);

	if(m_betKind != enWF_DaxiaoDanshuang)
	this->initMenu(Vec2(areaMenuPosX, areaW->getPositionY()), AREA_MENU_TAG1);

	LabelTTF *areaWTitle = LabelTTF::create(titleXmlIndex, "", fontSize);
	areaWTitle->setPosition(Vec2(areaW->getPositionX()+areaTitleXDelta, areaW->getPositionY()));
	areaWTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaWTitle, 0);
	//那条线
	pt.y -= (areaHeight-areaGapY-areaBkHeight/2);
	Sprite *gapW = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapW->setAnchorPoint(Vec2(0,0.5));
	gapW->setPosition(Vec2(0, pt.y));gapW->setScaleX(SCREEN_WIDTH / gapW->getContentSize().width);
	m_node->addChild(gapW,0);

#ifdef VER_QIANBO
	areaWTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaWTitle->setColor(ccc3(50, 62, 123));
#endif

	if (m_betKind == enWF_ZhixuanHezhi)
	{
		this->initHeZhiBalls(Vec2(ballStartX, areaW->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG1);
		gapW->setPositionY(pt.y - areaHeight-areaGapY-areaBkHeight);
	}
	else if (m_betKind == enWF_DaxiaoDanshuang)
		this->initDXDS(Vec2(ballStartX, areaW->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG1);
	else
		this->initBalls(Vec2(ballStartX, areaW->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2), AREA_BALL_TAG1);
}

void FC3DPL3WuFen::initQArea(const char *titleXmlIndex)
{
	areaStatus[1] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaQ = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaQ->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaQ,0);

	if(m_betKind != enWF_DaxiaoDanshuang)
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

	if (m_betKind == enWF_ZhixuanHezhi)
		this->initHeZhiBalls(Vec2(ballStartX, areaQ->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG2);
	else if (m_betKind == enWF_DaxiaoDanshuang)
		this->initDXDS(Vec2(ballStartX, areaQ->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG2);
	else
		this->initBalls(Vec2(ballStartX, areaQ->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG2);
}

void FC3DPL3WuFen::initBArea(const char *titleXmlIndex)
{
	areaStatus[2] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaB = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaB->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaB,0);

	if(m_betKind != enWF_DaxiaoDanshuang)
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
	m_node->addChild(gapB,0);

	if (m_betKind == enWF_ZhixuanHezhi)
		this->initHeZhiBalls(Vec2(ballStartX, areaB->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG2);
	else if (m_betKind == enWF_DaxiaoDanshuang)
		this->initDXDS(Vec2(ballStartX, areaB->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG3);
	else
		this->initBalls(Vec2(ballStartX, areaB->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG3);
}

void FC3DPL3WuFen::initSArea(const char *titleXmlIndex)
{
	areaStatus[3] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaS = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaS->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaS,0);

	if(m_betKind != enWF_DaxiaoDanshuang)
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

	if (m_betKind == enWF_ZhixuanHezhi)
		this->initHeZhiBalls(Vec2(ballStartX, areaS->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG4);
	else if (m_betKind == enWF_DaxiaoDanshuang)
		this->initDXDS(Vec2(ballStartX, areaS->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG4);
	else
		this->initBalls(Vec2(ballStartX, areaS->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG4);
}

void FC3DPL3WuFen::initGWArea(const char *titleXmlIndex)
{
	areaStatus[4] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;
	Sprite *areaG = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaG->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaG,0);

	if(m_betKind != enWF_DaxiaoDanshuang)
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

	if (m_betKind == enWF_ZhixuanHezhi)
		this->initHeZhiBalls(Vec2(ballStartX, areaG->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG5);
	else if (m_betKind == enWF_DaxiaoDanshuang)
		this->initDXDS(Vec2(ballStartX, areaG->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG5);
	else
		this->initBalls(Vec2(ballStartX, areaG->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2), AREA_BALL_TAG5);
}

void FC3DPL3WuFen::initBalls(Vec2 startPos, int startTag)
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

void FC3DPL3WuFen::initMenu(Vec2 menuPos, int startTag)
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
	MenuItemSprite *bigItem = MenuItemSprite::create(bigNormalImage,bigSelectedImage,CC_CALLBACK_1(FC3DPL3WuFen::pressBig,this));
	MenuItemSprite *littleItem = MenuItemSprite::create(littleNormalImage,littleSelectedImage,CC_CALLBACK_1(FC3DPL3WuFen::pressLittle,this));
	MenuItemSprite *allItem = MenuItemSprite::create(allNormalImage,allSelectedImage,CC_CALLBACK_1(FC3DPL3WuFen::pressAll,this));
	MenuItemSprite *singleItem = MenuItemSprite::create(singleNormalImage,singleSelectedImage,CC_CALLBACK_1(FC3DPL3WuFen::pressSingle,this));
	MenuItemSprite *doubleItem = MenuItemSprite::create(doubleNormalImage,doubleSelectedImage,CC_CALLBACK_1(FC3DPL3WuFen::pressDouble,this));
	MenuItemSprite *clearItem = MenuItemSprite::create(clearNormalImage,clearSelectedImage,CC_CALLBACK_1(FC3DPL3WuFen::pressClear,this));
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

void FC3DPL3WuFen::pressKindButton(int tag)
{
	CCLOG("fuck");
}

void FC3DPL3WuFen::onEnter()
{
	Layer::onEnter();
	//
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this,-128);	
}

void FC3DPL3WuFen::onExit()
{
	//
	Layer::onExit();
}

bool FC3DPL3WuFen::onTouchBegan(Touch *pTouch, Event *pEvent)
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
void FC3DPL3WuFen::onTouchMoved(Touch *pTouch, Event *pEvent)
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
void FC3DPL3WuFen::onTouchEnded(Touch *pTouch, Event *pEvent)
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
	for(int j = 0 ; j < 6 ; j++)
	{
		if (m_betKind == enWF_DaxiaoDanshuang)		//大小单双
			ballNum = 4;
		else if (m_betKind == enWF_ZhixuanHezhi)	//三星和值
			ballNum = 28;
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
				if (m_betKind == enWF_DaxiaoDanshuang)		//大小单双
					ballSpriteFrame = spriteFrame("gamble_011.png");
				else if (m_betKind == enWF_ZhixuanHezhi)
					ballSpriteFrame = spriteFrame("gamble_00.png");
				else
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

void FC3DPL3WuFen::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

String *FC3DPL3WuFen::getBetNumber()
{
	switch(m_betKind)
	{
	case enWF_Budingwei:			//不定位
		return this->getBuDingWeiBetNumber();
	case enWF_QianyizhixuanFushi:	//冠军
		{
			return this->getGuanJunBetNumber();
			break;
		}
	case enWF_QianerzhixuanFushi:						//冠亚军
		{
			return this->getGuanYaJunBetNumber();
			break;
		}
	case enWF_ZhixuanFushi:						//前三
		{
			return this->getQianSanBetNumber();
			break;
		}
	case enWF_HouyizhixuanFushi:							//后1
		{
			return this->getHouYiBetNumber();
			break;
		}
	case enWF_HouerzhixuanFushi:							//后2
		{
			return this->getHouErBetNumber();
			break;
		}
	case enWF_DingWeiDan:							//定位胆
		{
			return this->getDingWeiDanBetNumber();
			break;
		}
	case enWF_DaxiaoDanshuang:							//大小单双
		{
			return this->getDaXiaoDanShuangBetNumber();
			break;
		}
	case enWF_ZhixuanHezhi:							//和值
		{
			return this->getGuanYaHeZhiBetNumber();
			break;
		}
	//////////////////////////////////////////////////////////////

	case enWF_QiansanzuliuFushi:       //前三组选6选号
	{
		return this->getQianSan_ZuXuanBetNumber(6);
		break;
	}
	case enWF_QiansanzusanFushi:       //前三组选3选号
		{
			return this->getQianSan_ZuXuanBetNumber(3);
			break;
		}
	default:
		return this->getDanShiBetNumber();
		break;
	}
	return String::createWithFormat("");
}

//冠军
String* FC3DPL3WuFen::getGuanJunBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = { { "0" }, { "1" }, { "2" }, { "3" }, { "4" }, { "5" }, { "6" }, { "7" }, { "8" }, { "9" } };

	int ballNum = 0;
	int oidfjgvbiodf = ballStatus[0][2];
	for(int i = 0 ; i < 10 ; i++)
	{
		if(ballStatus[0][i] == 1)
		{			
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

String* FC3DPL3WuFen::getBuDingWeiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = { { "0" }, { "1" }, { "2" }, { "3" }, { "4" }, { "5" }, { "6" }, { "7" }, { "8" }, { "9" } };

	int ballNum = 0;
	int oidfjgvbiodf = ballStatus[0][2];
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[0][i] == 1)
		{
			if (ballNum>0){
				result += ",";
			}
			result += numberStr[i];
			ballNum++;
		}
	}

	if (ballNum <1)
	{
		result = "//";
		betZhuShu = 0;
		String *resultStr = String::createWithFormat("%s", result.c_str());
		return resultStr;
	}
	else{
		betZhuShu = ballNum;
	}
	if (betZhuShu<1){
		betZhuShu = 0;
	}
	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}
//冠亚
String* FC3DPL3WuFen::getGuanYaJunBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = { { "0" }, { "1" }, { "2" }, { "3" }, { "4" }, { "5" }, { "6" }, { "7" }, { "8" }, { "9" } };

	int ballNum1 = 0;
	int ballNum2 = 0;
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[0][i] == 1)
		{
			result += numberStr[i];
			ballNum1++;
		}
	}

	if (ballNum1 < 1){
		result = "ERZHI";
		betZhuShu = 1;
		String *resultStr = String::createWithFormat("%s", result.c_str());
		return resultStr;
	}
	else{
		betZhuShu *= ballNum1;
		result += ",";
		for (int i = 0; i < 10; i++)
		{
			if (ballStatus[1][i] == 1)
			{
				result += numberStr[i];
				ballNum2++;
			}
		}
		if (ballNum2<1)
		{
			betZhuShu = 0;
			result = "ERZHI";
			String *resultStr = String::createWithFormat("%s", result.c_str());
			return resultStr;
		}
		else{ betZhuShu *= ballNum2; }
	}
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
String* FC3DPL3WuFen::getQianSanBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = { { "0" }, { "1" }, { "2" }, { "3" }, { "4" }, { "5" }, { "6" }, { "7" }, { "8" }, { "9" } };

	
	int ballNum1 = 0;
	int ballNum2 = 0;
	int ballNum3 = 0;
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[0][i] == 1)
		{
			result += numberStr[i];
			ballNum1++;
		}
	}

	if (ballNum1 < 1)
	{
		result = "SAN";
		betZhuShu = 0;
		String *resultStr = String::createWithFormat("%s", result.c_str());
		return resultStr;
	}

	betZhuShu *= ballNum1;
	result += ",";
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[1][i] == 1)
		{
			result += numberStr[i];
			ballNum2++;
		}
	}

	if (ballNum2 < 1)
	{
		result = "SAN";
		betZhuShu = 0;
		String *resultStr = String::createWithFormat("%s", result.c_str());
		return resultStr;
	}

	betZhuShu *= ballNum2;
	result += ",";
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[2][i] == 1)
		{
			result += numberStr[i];
			ballNum3++;
		}
	}

	if (ballNum3<1){
		betZhuShu = 0;
		result = "SAN";
		String *resultStr = String::createWithFormat("%s", result.c_str());
		return resultStr;
	}
	else{ betZhuShu *= ballNum3; }

	if (betZhuShu<1){
		betZhuShu = 0;
	}
	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//后1
String* FC3DPL3WuFen::getHouYiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = { { "0" }, { "1" }, { "2" }, { "3" }, { "4" }, { "5" }, { "6" }, { "7" }, { "8" }, { "9" } };

	int ballNum3 = 0;

	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[2][i] == 1)
		{
			result += numberStr[i];
			ballNum3++;
		}
	}

	if (ballNum3<1){
		betZhuShu = 0;
		result = "";
		String *resultStr = String::createWithFormat("%s", result.c_str());
		return resultStr;
	}
	else{ betZhuShu *= ballNum3; }

	if (betZhuShu<1){
		betZhuShu = 0;
	}
	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//后2
String* FC3DPL3WuFen::getHouErBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = { { "0" }, { "1" }, { "2" }, { "3" }, { "4" }, { "5" }, { "6" }, { "7" }, { "8" }, { "9" } };

	int ballNum2 = 0;
	int ballNum3 = 0;
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[1][i] == 1)
		{
			result += numberStr[i];
			ballNum2++;
		}
	}

	if (ballNum2 < 1)
	{
		result = "SAN";
		betZhuShu = 0;
		String *resultStr = String::createWithFormat("%s", result.c_str());
		return resultStr;
	}

	betZhuShu *= ballNum2;
	result += ",";
	for (int i = 0; i < 10; i++)
	{
		if (ballStatus[2][i] == 1)
		{
			result += numberStr[i];
			ballNum3++;
		}
	}

	if (ballNum3<1){
		betZhuShu = 0;
		result = "SAN";
		String *resultStr = String::createWithFormat("%s", result.c_str());
		return resultStr;
	}
	else{ betZhuShu *= ballNum3; }

	if (betZhuShu<1){
		betZhuShu = 0;
	}
	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//定位胆
String* FC3DPL3WuFen::getDingWeiDanBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = { { "0" }, { "1" }, { "2" }, { "3" }, { "4" }, { "5" }, { "6" }, { "7" }, { "8" }, { "9" } };

	int ballNum = 0;

	for (int j = 0; j < 3; j++){
		for (int i = 0; i < 10; i++)
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

//大小单双
String* FC3DPL3WuFen::getDaXiaoDanShuangBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[4][3] = { { "9" }, { "1" }, { "3" }, { "2" } };

	int ballNum = 0;

	for (int j = 0; j < 3; j++){
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

//修改为前三和值。。。
String* FC3DPL3WuFen::getGuanYaHeZhiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[28][3] = { { "0" }, { "1" }, { "2" }, { "3" }, { "4" }, { "5" }, { "6" }, { "7" }, { "8" }, { "9" }, \
	{ "10" }, { "11" }, { "12" }, { "13" }, { "14" }, { "15" }, { "16" }, { "17" }, { "18" }, { "19" },  \
	{ "20" }, { "21" }, { "22" }, { "23" }, { "24" }, { "25" }, { "26" }, { "27" } };
	
	int ballNum = 0;
	int oidfjgvbiodf = ballStatus[0][2];
	for(int i = 0 ; i < 28 ; i++)
	{
		if(ballStatus[0][i] == 1)
		{
			if(ballNum>0){
				result+=",";
			}
			result+=numberStr[i];
			//三星和值:求注数
			for (int m = 0; m < 10; m++)
			{
				for (int n = 0; n < 10; n++)
				{
					for (int o = 0; o < 10; o++)
					{
						if ((m + n + o) == atoi(numberStr[i]) )
						{
							ballNum++;
						}
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

////////////////////////////////////////////////////////////////////////////////
String* FC3DPL3WuFen::getQianEr_ZuXuanBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = { { "0" }, { "1" }, { "2" }, { "3" }, { "4" }, { "5" }, { "6" }, { "7" }, { "8" }, { "9" } };
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

String* FC3DPL3WuFen::getQianSan_ZuXuanBetNumber(int nAmount)
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = { { "0" }, { "1" }, { "2" }, { "3" }, { "4" }, { "5" }, { "6" }, { "7" }, { "8" }, { "9" } };

	int ballNum = 0;
	int oidfjgvbiodf = ballStatus[0][2];
	for(int i = 0 ; i < 10 ; i++)
	{ 
		if(ballStatus[0][i] == 1)
		{
			if((ballNum>0 )&&( nAmount != 3 )&& (nAmount != 6)){
				result+=",";
			}
			result+=numberStr[i];
			ballNum++;
		}
	}
	int nLessNum = 0;
	if (nAmount == 6)
		nLessNum = 3;		//组6 至少3个数字
	if (nAmount == 3)		
		nLessNum = 2;		//组3 至少2个数字
	if (ballNum < nLessNum){
		if (nAmount == 6)
			result = "SANZU";
		else
			result = "ZU3";
		betZhuShu = 0;
		String *resultStr = String::createWithFormat("%s", result.c_str());
	    return resultStr;
	}else{
		if (nAmount == 6)
			betZhuShu = this->combo(ballNum, 3);		//组6
		else
			betZhuShu = this->combo(ballNum, 2)*2;		//组3
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

String* FC3DPL3WuFen::getDanShiBetNumber()
{	
	//创建字符串
	String* result = String::create(m_betString);

	if(m_betString.size() < 2) result = String::create("");

	return result;
}

void FC3DPL3WuFen::setDanShiCount()
{
	switch (m_betKind)
	{	
			
	case 	enWF_QianerzhixuanDanshi:			//前二直选单式
		m_num = 2;
		m_repeat = 0;
		break;				
	case 	enWF_ZhixuanDanshi:					//三星直选单式
		m_num = 3;
		m_repeat = 0;
		break;				
	case 	enWF_QiansanzusanDanshi:
		m_num = 3;
		m_repeat = 1;
		break;				
	case 	enWF_QiansanzuliuDanshi:	
		m_num = 3;
		m_repeat = 2;
		break;				
	case 	enWF_HouerzhixuanDanshi:
		m_num = 2;
		m_repeat = 0;
		break;				
	default:
		m_num = 0;
		m_repeat = 0;
		break;
	}
}

void FC3DPL3WuFen::createDanShiRandomNumber()
{
	this->clearNumbers(0);

	betZhuShu = 0;
}

void FC3DPL3WuFen::createRandomNumbers()
{
	CCLOG("FC3DPL3WuFen::createRandomNumbers");
	CCLOG("AAAAAAAA == %d", m_betKind);

	if(this->getChildByTag(9528) != nullptr)
	{
		createDanShiRandomNumber();
		return;
	}

	this->clearNumbers();
	SpriteFrame *ballSpriteFrame = NULL;
	
	int nBallNum = 0;
	if (m_betKind == enWF_DaxiaoDanshuang)
	{
		nBallNum = 4;
		ballSpriteFrame = spriteFrame("gamble_011.png");
	}
	else if (m_betKind == enWF_ZhixuanHezhi)
	{
		ballSpriteFrame = spriteFrame("gamble_00.png");
		nBallNum = 28;
	}
	else
	{
		ballSpriteFrame = spriteFrame("gamble_01.png");
		nBallNum = 10;
	}
	if (m_betKind == enWF_QiansanzuliuFushi)
		return createZuXuanRandomNumbers(0, 3);
	if (m_betKind == enWF_QiansanzusanFushi)
		return createZuXuanRandomNumbers(0, 2);

	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	for(int j = 0 ; j < 5 ; j++)
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
void FC3DPL3WuFen::createRenXuanRandomNumbers()
{
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	for(int j = 0 ; j < 5 ; j++){
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
void FC3DPL3WuFen::createQianEr_ZuXuanRandomNumbers()
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
void FC3DPL3WuFen::createQianSan_ZuXuanRandomNumbers()
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

void FC3DPL3WuFen::clearNumbers()
{
	SpriteFrame *ballSpriteFrame = NULL;
	int nBallNum = 0;
	if (m_betKind == enWF_DaxiaoDanshuang)
	{
		nBallNum = 4;
		ballSpriteFrame = spriteFrame("gamble_011.png");
	}
	else if (m_betKind == enWF_ZhixuanHezhi)
	{
		ballSpriteFrame = spriteFrame("gamble_00.png");
		nBallNum = 28;
	}
	else
	{
		ballSpriteFrame = spriteFrame("gamble_01.png");
		nBallNum = 10;
	}
	for(int j = 0 ; j < 5 ; j++)
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
void FC3DPL3WuFen::cancelAllMenuSelect()
{
	for(int i = AREA_MENU_TAG1 ; i <= AREA_MENU_TAG5 ; i+=AREA_GAP)
	{
		this->cancelMenuSelect(i);
	}
}
void FC3DPL3WuFen::clearNumbers(int area)
{
	SpriteFrame *ballSpriteFrame = NULL;
	int nBallNum = 0;
	if (m_betKind == enWF_DaxiaoDanshuang)
	{
		nBallNum = 4;
		ballSpriteFrame = spriteFrame("gamble_011.png");
	}
	else if (m_betKind == enWF_ZhixuanHezhi)
	{
		ballSpriteFrame = spriteFrame("gamble_00.png");
		nBallNum = 28;
	}
	else
	{
		ballSpriteFrame = spriteFrame("gamble_01.png");
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
void FC3DPL3WuFen::pressBig(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == enWF_DaxiaoDanshuang)
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
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
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
void FC3DPL3WuFen::pressLittle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == enWF_DaxiaoDanshuang)
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
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
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

void FC3DPL3WuFen::pressAll(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == enWF_DaxiaoDanshuang)
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
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
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
void FC3DPL3WuFen::pressDouble(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == enWF_DaxiaoDanshuang)
		return;

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-AREA_MENU_TAG1)/AREA_GAP;
	//if(menuStatus[itemAreaIndex][3] == 1){	
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
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+itemAreaIndex*AREA_GAP+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
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
void FC3DPL3WuFen::pressSingle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == enWF_DaxiaoDanshuang)
		return;

	int itemTag = menuItem->getTag();
	int itemAreaIndex = (itemTag-AREA_MENU_TAG1)/AREA_GAP;
	//if(menuStatus[itemAreaIndex][4] == 1){	
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
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(AREA_BALL_TAG1+itemAreaIndex*AREA_GAP+i);
		Rect rect = ballSprite->boundingBox();
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_01.png");
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
void FC3DPL3WuFen::pressClear(Object *obj)
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

void FC3DPL3WuFen::cancelMenuSelect(int itemTag)
{
	int itemAreaIndex = (itemTag-AREA_MENU_TAG1)/AREA_GAP;
	Menu *menu = (Menu *)m_node->getChildByTag(itemTag);
	if (menu == NULL)
		return;
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


//组选随机
void FC3DPL3WuFen::createZuXuanRandomNumbers(int area, int num)
{
	this->clearNumbers(area);
	for (int j = 0; j < num; j++){
		SpriteFrame *ballSpriteFrame;
		Texture2D *ballTexture2D;
		int ballNum = 10;
		if (area == 5){
			ballNum = 28;
			ballSpriteFrame = spriteFrame("gamble_00.png");
		}
		else{
			ballSpriteFrame = spriteFrame("gamble_01.png");
		}
		ballTexture2D = ballSpriteFrame->getTexture();
		int num = rand() % ballNum;
		while (ballStatus[area][num] == 1){
			num = rand() % ballNum;
		}
		ballStatus[area][num] = 1 - ballStatus[area][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag( area* 10 + num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num * ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}