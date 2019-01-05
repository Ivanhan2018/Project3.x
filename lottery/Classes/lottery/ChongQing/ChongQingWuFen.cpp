#include "HelpLayer.h"
#include "RoomLayer.h"
#include "ChongQingWuFen.h"
#include "BetLimit.h"
#include "MovingLabelLayer.h"
#include "VersionControl.h"

using namespace cocos2d;

ChongQingWuFen::ChongQingWuFen()
{

}

ChongQingWuFen::~ChongQingWuFen()
{
	m_vecPot.clear();
}

Scene* ChongQingWuFen::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		ChongQingWuFen *layer = ChongQingWuFen::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool ChongQingWuFen::init()
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
		listener->onTouchBegan = CC_CALLBACK_2(ChongQingWuFen::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(ChongQingWuFen::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(ChongQingWuFen::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(ChongQingWuFen::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		m_node = Node::create();
		m_node->setPosition(Vec2(0,0));
		this->addChild(m_node,2);

		areaTitleXDelta = -5;
		fontSize = 27;
		m_num = 0;
		m_repeat = 0;

		isWanWeiSelected = 1;
		isQianWeiSelected = 1;
		isBaiWeiSelected = 1;
		isShiWeiSelected = 1;
		isGeWeiSelected = 1;

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

void ChongQingWuFen::onAcceleration(Acceleration* acc, Event* unused_event) 
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

void ChongQingWuFen::setBetKind(SSCWanFaKind kind)
{
	pt = Vec2(240, SCREEN_HEIGHT - 310);//280
	m_betKind = kind;
	switch (m_betKind)
	{
	case WuxingZhixuan:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t90"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
			break;
		}
	case Zuxuan120:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case Zuxuan60:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t97"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t96"));
			break;
		}
	case Zuxuan30:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t97"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t96"));
			break;
		}
	case Zuxuan20:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t98"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t96"));
			break;
		}
	case Zuxuan10:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t98"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t97"));
			break;
		}
	case Zuxuan5:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t99"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t96"));
			break;
		}
	case WuxingQianSi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t90"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			break;
		}
	case QSZuXuan24:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case QSZuXuan12:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t97"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t96"));
			break;
		}
	case QSZuXuan6:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t97"));			
			break;
		}
	case QSZuXuan4:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t98"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t96"));
			break;
		}
	case WuxingHousi:
		{
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
			break;
		}
	case HSZuXuan24:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case HSZuXuan12:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t97"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t96"));
			break;
		}
	case HSZuXuan6:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t97"));		
			break;
		}
	case HSZuXuan4:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t98"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t96"));
			break;
		}
	case QianSanZhiXuan_FuShi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t90"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			break;
		}	
	case QianSanZuXuan_ZuSanFuShi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case QianSanZuXuan_ZuLiuFuShi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case QianSanZhiXuanHeZhi:
		{
			initHeZhiArea(ConfigMgr::instance()->text("display_text.xml", "t100"));
			break;
		}
	case WuxingZhongsan:
		{
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			break;
		}
	case ZhongsanZusan:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case ZhongsanZuliu:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case ZhongSanZhiXuanHeZhi:
		{
			initHeZhiArea(ConfigMgr::instance()->text("display_text.xml", "t100"));
			break;
		}
	case HouSanZhiXuan_FuShi:
		{
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
			break;
		}
	case HouSanZuXuan_ZuSanFuShi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case HouSanZuXuan_ZuLiuFuShi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case HouSanZhiXuanHeZhi:
		{
			initHeZhiArea(ConfigMgr::instance()->text("display_text.xml", "t100"));
			break;
		}
	case WuXingHouEr_FuShi:
		{
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
			break;
		}
	case WuXingQianEr_FuShi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t90"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
			break;
		}
	case HouErZuXuan_FuShi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case QianErZuXuan_FuShi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case DingWeiDan:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t90"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
			break;
		}
	case DaXiaoDanShuang_DW:
	{
		initWArea(ConfigMgr::instance()->text("display_text.xml", "t90"));
		initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
		initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
		initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
		initGWArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
		break;
	}
	case BuDingWei_HouSan:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case BuDingWei_QianSan:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case BuDingWei_HouEr:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case BuDingWei_QianEr:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case QW_YiFanFengShun:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t95"));
			break;
		}
	case QW_HaoShiChengShuang:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t97"));
			break;
		}
	case QW_SanXingBaoXi:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t98"));
			break;
		}
	case QW_SiJiFaCai:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t99"));
			break;
		}
	case SSC_LongHu:
		{
			initLongHuHe();
			break;
		}
	case RenXuan4:		//任选4
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t90"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
			break;
		}
	case RenXuan3:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t90"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
			break;
		}
	case Ren3Zu3_Fushi:
		{
			initHeadArea(ConfigMgr::instance()->text("display_text.xml", "t4620"));
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t4621"));

			m_num = 3;
			break;
		}
	case Ren3Zu6_Fushi:
		{
			initHeadArea(ConfigMgr::instance()->text("display_text.xml", "t4620"));
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t4621"));

			m_num = 3;
			break;
		}
	case RenXuan2:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t90"));
			initQArea(ConfigMgr::instance()->text("display_text.xml", "t91"));
			initBArea(ConfigMgr::instance()->text("display_text.xml", "t92"));
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
			break;
		}
	case Ren2Zuxuan_Fushi:
		{
			initHeadArea(ConfigMgr::instance()->text("display_text.xml", "t4620"));
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t4621"));

			m_num = 2;
			break;
		}
	case Zonghe_DXDSLh:
		{
			initWArea(ConfigMgr::instance()->text("display_text.xml", "t100"));
			break;
		}
	case DaXiaoDanShuang:
		{
			initSArea(ConfigMgr::instance()->text("display_text.xml", "t93"));
			initGWArea(ConfigMgr::instance()->text("display_text.xml", "t94"));
			break;
		}
	case RenXuan4_DanShi:
	case RenXuan3_DanShi:
	case RenXuan2_DanShi:
	case Ren3Zu3_DanShi:
	case Ren3Zu6_DanShi:
	case Ren3Hunhe_Zuxuan:
	case Ren2Zuxuan_Danshi:
		//设置头 万千百十个
		initHeadArea(ConfigMgr::instance()->text("display_text.xml", "t4620"));
		//设置单式格式
		setDanShiCount();
		//设置单式区域
		initDanshiArea();
		break;		
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

	updateRenXuanHeadTips();
}

void ChongQingWuFen::editBoxEditingDidBegin(EditBox* editBox)
{
	LabelTTF* resultBox = (LabelTTF*)this->getChildByTag(9528);
	if(resultBox == nullptr) return;
	editBox->setFont("",27);
	resultBox->setVisible(false);

	if(m_inputString.size() > 0) editBox->setText(m_inputString.c_str());
}

void ChongQingWuFen::editBoxEditingDidEnd(EditBox* editBox)
{
    LabelTTF* resultBox = (LabelTTF* )this->getChildByTag(9528);
	if(resultBox == nullptr) return;
	resultBox->setVisible(true);

    m_betString.clear();
    m_betString.append(m_inputString);
    BetLimit::SpanStringByLen(m_betString);		
	
	//清空
	editBox->setText("");
    
    resultBox->setString(m_betString.substr(0,1000));
	
	//计算注数和号码
	countTouZhuParams();

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
}

void ChongQingWuFen::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
	LabelTTF* resultBox = (LabelTTF*)this->getChildByTag(9528);
	if(resultBox == nullptr) return;
	if(!resultBox->isVisible())
	{
		m_inputString.clear();
		m_inputString.append(text);
	}
}

void ChongQingWuFen::initLongHuHe()
{
	areaStatus[0] = 1;
	pt.y = pt.y- (200 - areaGapY - areaBkHeight / 2) / 2;

	for(int irow = 0;irow < 10;irow ++)
	{	
		//分组名称背景
		Sprite *sptBack = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
		sptBack->setPosition(Vec2(areaTitlePosX, pt.y));
		m_node->addChild(sptBack,0);
		
		//分组名称文字
		LabelTTF *title = LabelTTF::create(
			ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d",970 + irow)->getCString()), 
			"", 
			fontSize);
		title->setPosition(Vec2(sptBack->getPositionX()+areaTitleXDelta, sptBack->getPositionY()));
		title->setColor(ccc3(167,60,60));
		m_node->addChild(title, 0);

#ifdef VER_QIANBO
		title->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
		title->setColor(ccc3(50, 62, 123));
#endif

		//init球
		SpriteFrame *ballSpriteFrame = spriteFrame("gamble_02_1.png");
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		
		for(int i = 0 ; i < 3 ; i ++){
			Sprite *ballSprite = Sprite::createWithSpriteFrame(
				SpriteFrame::createWithTexture(ballTexture2D, Rect(i*ballRadius,0,ballRadius,ballRadius)));
			ballSprite->setPosition(Vec2(ballStartX+(ballRadius+ballGapX)*(i % 3 + 1) * 1.2f,pt.y));
			ballSprite->setTag(W_AREA_BALL_TAG + 3 * irow + i);//设置tag
			m_node->addChild(ballSprite);			
		}

		//下划线	

		Sprite *gap = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
		gap->setAnchorPoint(Vec2(0,0.5));
		gap->setPosition(Vec2(0, pt.y - 60));
		m_node->addChild(gap,0);
		
		pt.y -= 120;
	}
}

void ChongQingWuFen::initDanshiArea()
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
	this->addChild(sptValue,2);

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

	//隐藏
	valueTTF->setVisible(true);
	//sptValue->setVisible(false);
	resultTTF->setVisible(false);
	sptValue1->setVisible(false);
	ltfText2->setVisible(false);
}

void ChongQingWuFen::initWArea(const char *titleXmlIndex)
{
	areaStatus[0] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称背景
	Sprite *areaW = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaW->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaW,0);

	bool isDXDS = m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang;

	//菜单 大小单双
	if(!isDXDS)
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

	if (isDXDS)
		this->initDXDS(Vec2(ballStartX, areaW->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), W_AREA_BALL_TAG);
	else
		this->initBalls(Vec2(ballStartX, areaW->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2 - 8), W_AREA_BALL_TAG);
}

void ChongQingWuFen::initQArea(const char *titleXmlIndex)
{
	areaStatus[1] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称
	Sprite *areaQ = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaQ->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaQ,0);

	bool isDXDS = m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang;

	//菜单 大小单双
	if(!isDXDS)
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

	if (isDXDS)
		this->initDXDS(Vec2(ballStartX, areaQ->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), Q_AREA_BALL_TAG);
	else
		this->initBalls(Vec2(ballStartX, areaQ->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), Q_AREA_BALL_TAG);
}

void ChongQingWuFen::initBArea(const char *titleXmlIndex)
{
	areaStatus[2] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称
	Sprite *areaB = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaB->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaB,0);

	bool isDXDS = m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang;

	//菜单 大小单双
	if(!isDXDS)
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

	if (isDXDS)
		this->initDXDS(Vec2(ballStartX, areaB->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), B_AREA_BALL_TAG);
	else
		this->initBalls(Vec2(ballStartX, areaB->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), B_AREA_BALL_TAG);
}

void ChongQingWuFen::initSArea(const char *titleXmlIndex)
{
	areaStatus[3] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称
	Sprite *areaS = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaS->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaS,0);

	bool isDXDS = m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang;

	//菜单 大小单双
	if(!isDXDS)
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

	if (isDXDS) 
		this->initDXDS(Vec2(ballStartX, areaS->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), S_AREA_BALL_TAG);
	else
		this->initBalls(Vec2(ballStartX, areaS->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), S_AREA_BALL_TAG);
}

void ChongQingWuFen::initGWArea(const char *titleXmlIndex)
{
	areaStatus[4] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称
	Sprite *areaG = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaG->setPosition(Vec2(areaTitlePosX, pt.y));
	m_node->addChild(areaG,0);

	bool isDXDS = m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang;

	//菜单 大小单双
	if(!isDXDS)
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

	if (isDXDS)
		this->initDXDS(Vec2(ballStartX, areaG->getPositionY() - areaGapY*2 - ballRadius / 2 - areaBkHeight / 2 - 8), G_AREA_BALL_TAG);
	else
		this->initBalls(Vec2(ballStartX, areaG->getPositionY() - areaGapY - ballRadius / 2 - areaBkHeight / 2 - 8), G_AREA_BALL_TAG);
}

void ChongQingWuFen::initHeadArea(const char *titleXmlIndex)
{
	areaStatus[0] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2 - 5;

	//分组名称文字
	LabelTTF *areaWTitle = LabelTTF::create(titleXmlIndex, "", fontSize);
	areaWTitle->setPosition(Vec2(areaTitlePosX + areaTitleXDelta * 1.5f + 30, pt.y));
	areaWTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaWTitle, 0);

	//提示文字
	LabelTTF *ltfHelp = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t94"), "", fontSize);
	ltfHelp->setPosition(Vec2(areaWTitle->getPositionX() - areaWTitle->getContentSize().width / 2, pt.y - 60));	
	ltfHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	ltfHelp->setTag(8527);
	ltfHelp->setColor(ccc3(120,120,120));
	m_node->addChild(ltfHelp, 0);

	//按钮位置
	for(int i = 0;i < 5;i ++)
	{
		auto btWanWei = ui::Button::create(String::createWithFormat("bt_tz_bs_jian_normal1%d.png",i)->getCString());	
		btWanWei->setPosition(Vec2(areaTitlePosX + 37 + (i + 1) * 110, pt.y));		
		m_node->addChild(btWanWei, 1);	

		auto sptWanWei = Sprite::createWithSpriteFrame(spriteFrame(String::createWithFormat("bt_tz_bs_jian_pressed1%d.png", i)->getCString()));
		sptWanWei->setPosition(Vec2(areaTitlePosX + 37 + (i + 1) * 110, pt.y));
		sptWanWei->setTag(8000 + i);
		sptWanWei->setVisible(true);
		m_node->addChild(sptWanWei, 2);

		btWanWei->addClickEventListener([=](Ref*){
			sptWanWei->setVisible(!sptWanWei->isVisible());

			switch (sptWanWei->getTag())
			{
			case 8000://万位
				isWanWeiSelected = sptWanWei->isVisible() ? 1 : 0;
				break;
			case 8001://千位
				isQianWeiSelected = sptWanWei->isVisible() ? 1 : 0;
				break;
			case 8002://百位
				isBaiWeiSelected = sptWanWei->isVisible() ? 1 : 0;
				break;
			case 8003://十位
				isShiWeiSelected = sptWanWei->isVisible() ? 1 : 0;
				break;
			case 8004://个位
				isGeWeiSelected = sptWanWei->isVisible() ? 1 : 0;
				break;
			default:
				break;
			}
		
			countTouZhuParams();					

			updateRenXuanHeadTips();

			NotificationCenter::getInstance()->postNotification("UpdateTouZhuJine");			
		}
		);
	}	

	//下划线
	pt.y -= 100;
	Sprite *gapW = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapW->setAnchorPoint(Vec2(0,0.5));
	gapW->setPosition(Vec2(0, pt.y));gapW->setScaleX(SCREEN_WIDTH / gapW->getContentSize().width);
	m_node->addChild(gapW,0);
}

void ChongQingWuFen::initHeZhiArea(const char *titleXmlIndex)
{
	areaStatus[5] = 1;
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//分组名称
	Sprite *areaG = Sprite::createWithSpriteFrame(spriteFrame("gamble_05.png"));
	areaG->setPosition(Vec2(areaTitlePosX+20, pt.y));
	m_node->addChild(areaG,0);

	//菜单 大小单双
	this->initMenu(Vec2(areaMenuPosX, areaG->getPositionY()), HEZHI_AREA_MENU_TAG);

	//分组名称文字
	LabelTTF *areaGTitle = LabelTTF::create(titleXmlIndex, "", fontSize);
	areaGTitle->setPosition(Vec2(areaG->getPositionX()+areaTitleXDelta, areaG->getPositionY()));
	areaGTitle->setColor(ccc3(167,60,60));
	m_node->addChild(areaGTitle, 0);
	areaG->setScaleX(areaGTitle->getContentSize().width/areaG->getContentSize().width+0.2);

#ifdef VER_QIANBO
	areaGTitle->setColor(ccc3(255,255,255));
#endif // VER_QIANBO

#ifdef VER_369
	areaGTitle->setColor(ccc3(50, 62, 123));
#endif

	//下划线
	pt.y -= areaHeight*2;
	Sprite *gapG = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gapG->setAnchorPoint(Vec2(0,0.5));
	gapG->setPosition(Vec2(0, pt.y));gapG->setScaleX(SCREEN_WIDTH / gapG->getContentSize().width);
	m_node->addChild(gapG,0);

	this->initHeZhiBalls(Vec2(ballStartX, areaG->getPositionY()-areaGapY-ballRadius/2-areaBkHeight/2 - 8), HEZHI_AREA_BALL_TAG);
}

/***
大小单双
***/
void ChongQingWuFen::initDXDS(Vec2 startPos, int startTag)
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

/***
数字0-9
***/
void ChongQingWuFen::initBalls(Vec2 startPos, int startTag)
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
数字0-80
***/
void ChongQingWuFen::initHeZhiBalls(Vec2 startPos, int startTag)
{
	SpriteFrame *ballSpriteFrame = spriteFrame("gamble_00.png");
	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	Vec2 pos = startPos;
	for(int i = 0 ; i < 28 ; ){
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

/***
大小单双 菜单按钮
***/
void ChongQingWuFen::initMenu(Vec2 menuPos, int startTag)
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
	MenuItemSprite *bigItem = MenuItemSprite::create(bigNormalImage,bigSelectedImage,CC_CALLBACK_1(ChongQingWuFen::pressBig,this));
	MenuItemSprite *littleItem = MenuItemSprite::create(littleNormalImage,littleSelectedImage,CC_CALLBACK_1(ChongQingWuFen::pressLittle,this));
	MenuItemSprite *allItem = MenuItemSprite::create(allNormalImage,allSelectedImage,CC_CALLBACK_1(ChongQingWuFen::pressAll,this));
	MenuItemSprite *singleItem = MenuItemSprite::create(singleNormalImage,singleSelectedImage,CC_CALLBACK_1(ChongQingWuFen::pressSingle,this));
	MenuItemSprite *doubleItem = MenuItemSprite::create(doubleNormalImage,doubleSelectedImage,CC_CALLBACK_1(ChongQingWuFen::pressDouble,this));
	MenuItemSprite *clearItem = MenuItemSprite::create(clearNormalImage,clearSelectedImage,CC_CALLBACK_1(ChongQingWuFen::pressClear,this));
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

void ChongQingWuFen::pressKindButton(int tag)
{
	CCLOG("fuck");
}

void ChongQingWuFen::onEnter()
{
	Layer::onEnter();
	//
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this,-128);	
}

void ChongQingWuFen::onExit()
{
	//
	Layer::onExit();
}

bool ChongQingWuFen::onTouchBegan(Touch *pTouch, Event *pEvent)
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
void ChongQingWuFen::onTouchMoved(Touch *pTouch, Event *pEvent)
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
void ChongQingWuFen::onTouchEnded(Touch *pTouch, Event *pEvent)
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

	int ballNum;
	for(int j = 0 ; j < 6 ; j++)
	{
		ballNum = 10;

		//大小单双定位
		if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
			ballNum = 4;
		else if (m_betKind == QianSanZhiXuanHeZhi || m_betKind == HouSanZhiXuanHeZhi || m_betKind == ZhongSanZhiXuanHeZhi)
			ballNum = 28;
		if (m_betKind == SSC_LongHu)
			ballNum = 30;

		for(int i = 0 ; i < ballNum ; i++)
		{
			if(areaStatus[j] == 0){
				continue;
			}
			Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+i);
			if(ballSprite == NULL) continue;
			Rect rect = ballSprite->boundingBox();
			if(rect.containsPoint(endPoint)){	

				if(m_betKind == SSC_LongHu) this->clearNumbers();

				ballStatus[j][i] = 1-ballStatus[j][i];

				SpriteFrame *ballSpriteFrame;
				if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
					ballSpriteFrame = spriteFrame("gamble_011.png");
				else if (m_betKind == QianSanZhiXuanHeZhi || m_betKind == HouSanZhiXuanHeZhi || m_betKind == ZhongSanZhiXuanHeZhi)
					ballSpriteFrame = spriteFrame("gamble_00.png");
				else if (m_betKind == SSC_LongHu)
					ballSpriteFrame = spriteFrame("gamble_02_1.png");
				else
					ballSpriteFrame = spriteFrame("gamble_01.png");

				Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
				if(ballStatus[j][i] == 0){
					if (m_betKind == SSC_LongHu){
						ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
							Rect((i % 3)*ballRadius,0,ballRadius,ballRadius)));
					} else
					{
						ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
							Rect(i*ballRadius,0,ballRadius,ballRadius)));
					}					
				}else{
					if (m_betKind == SSC_LongHu){
						ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
							Rect((i%3)*ballRadius,ballRadius,ballRadius,ballRadius)));
					} else
					{
						ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
						Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));
					}
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
void ChongQingWuFen::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

String *ChongQingWuFen::getBetNumber()
{
	switch (m_betKind)
	{
	case WuxingZhixuan:
		{
			return this->getWuxingZhixuanBetNumber();
			
		}
	case Zuxuan120:
		{
			return this->getZuxuan120BetNumber();
			
		}
	case Zuxuan60:
		{
			return this->getZuxuan60BetNumber();
			
		}
	case Zuxuan30:
		{
			return this->getZuxuan30BetNumber();
			
		}
	case Zuxuan20:
		{
			return this->getZuxuan20BetNumber();
			
		}
	case Zuxuan10:
		{
			return this->getZuxuan10BetNumber();
			
		}
	case Zuxuan5:
		{
			return this->getZuxuan5BetNumber();
			
		}
	case WuxingQianSi:
		{
			return this->getWuxingQianSiBetNumber();
			
		}
	case QSZuXuan24:
		{
			return this->getZuxuanQS24BetNumber();
		}
	case QSZuXuan12:
		{
			return this->getZuxuanQS12BetNumber();
		}
	case QSZuXuan6:
		{
			return this->getZuxuanQS6BetNumber();
		}
	case QSZuXuan4:
		{
			return this->getZuxuanQS4BetNumber();
		}	
	case HSZuXuan24:
		{
			return this->getZuxuanHS24BetNumber();
		}
	case HSZuXuan12:
		{
			return this->getZuxuanHS12BetNumber();
		}
	case HSZuXuan6:
		{
			return this->getZuxuanHS6BetNumber();
		}
	case HSZuXuan4:
		{
			return this->getZuxuanHS4BetNumber();
		}
	case WuxingHousi:
		{
			return this->getWuxingHousiBetNumber();
			
		}
	case QianSanZhiXuan_FuShi:
		{
			return this->getQianSanZhiXuan_FuShiBetNumber();
			
		}
	case QianSanZuXuan_ZuSanFuShi:
		{
			return this->getQianSanZuXuan_ZuSanFuShiBetNumber();
			
		}
	case QianSanZuXuan_ZuLiuFuShi:
		{
			return this->getQianSanZuXuan_ZuLiuFuShiBetNumber();
			
		}
	case QianSanZhiXuanHeZhi:
		{
			return this->getQianSanZhiXuanHeZhiBetNumber();
			
		}
	case WuxingZhongsan:
		{
			return this->getZhongSanZhiXuan_FuShiBetNumber();
			
		}
	case ZhongsanZusan:
		{
			return this->getZhongSanZuXuan_ZuSanFuShiBetNumber();
			
		}
	case ZhongsanZuliu:
		{
			return this->getZhongSanZuXuan_ZuLiuFuShiBetNumber();
			
		}
	case ZhongSanZhiXuanHeZhi:
		{
			return this->getZhongSanZhiXuanHeZhiBetNumber();
			
		}
	case HouSanZhiXuan_FuShi:
		{
			return this->getHouSanZhiXuan_FuShiBetNumber();
			
		}
	case HouSanZuXuan_ZuSanFuShi:
		{
			return this->getHouSanZuXuan_ZuSanFuShiBetNumber();
			
		}
	case HouSanZuXuan_ZuLiuFuShi:
		{
			return this->getHouSanZuXuan_ZuLiuFuShiBetNumber();
			
		}
	case HouSanZhiXuanHeZhi:
		{
			return this->getHouSanZhiXuanHeZhiBetNumber();
			
		}
	case WuXingHouEr_FuShi:
		{
			return this->getWuXingHouEr_FuShiBetNumber();
			
		}
	case WuXingQianEr_FuShi:
		{
			return this->getWuXingQianEr_FuShiBetNumber();
			
		}
	case HouErZuXuan_FuShi:
		{
			return this->getHouErZuXuan_FuShiBetNumber();
			
		}
	case QianErZuXuan_FuShi:
		{
			return this->getQianErZuXuan_FuShiBetNumber();
			
		}
	case DingWeiDan:
		{
			return this->getDingWeiDanBetNumber();
			
		}
	case DaXiaoDanShuang_DW:
	{
		return this->GetDaXiaoDanShuang_DW();
		
	}
	case DaXiaoDanShuang:
		{
			return this->GetDaXiaoDanShuang_DW(true);

		}
	case Zonghe_DXDSLh:
		{
			return this->GetDaXiaoDanShuang_DW(false,1);

		}
	case BuDingWei_HouSan:
		{
			return this->getBuDingWei_HouSanBetNumber();
			
		}
	case BuDingWei_QianSan:
		{
			return this->getBuDingWei_QianSanBetNumber();
			
		}
	case BuDingWei_HouEr:
		{
			return this->getBuDingWei_HouErBetNumber();
			
		}
	case BuDingWei_QianEr:
		{
			return this->getBuDingWei_QianErBetNumber();
			
		}
	case QW_YiFanFengShun:
		{
			return this->getQW_YiFanFengShunBetNumber();
			
		}
	case QW_HaoShiChengShuang:
		{
			return this->getQW_HaoShiChengShuangBetNumber();
			
		}
	case QW_SanXingBaoXi:
		{
			return this->getQW_SanXingBaoXiBetNumber();
			
		}
	case QW_SiJiFaCai:
		{
			return this->getQW_SiJiFaCaiBetNumber();
			
		}
	case SSC_LongHu:
		{
			return this->getSSCLongHuBetNumber();
			
		}
	case RenXuan4:
		{
			return this->getRen4BetNumber();
			
		}
	case RenXuan3:
		{
			return this->getRen3BetNumber();
			
		}
	case Ren3Zu3_Fushi:
		{
			return this->getRen3Zu3BetNumber();
		}
	case Ren3Zu6_Fushi:
		{
			return this->getRen3Zu6BetNumber();
		}
	case RenXuan2:
		{
			return this->getRen2BetNumber();
		}
	case Ren2Zuxuan_Fushi:
		{
			return this->getRen2ZuXuanBetNumber();
		}
	default:
		return this->getDanShiBetNumber();
		
	}
	return String::create("");
}

String *ChongQingWuFen::getZhixuanBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};
	bool flag;
	int beiLv;
	bool signSwitch = false;
	for(int j = 0 ; j < 5 ; j++){
		if(areaStatus[j] == 0){
			continue;
		}
		flag = false;
		beiLv = 0;
		if(signSwitch == true){
			result+=",";
		}else{
			signSwitch = true;
		}
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[j][i] == 1)
			{
				result+=numberStr[i];
				beiLv++;
				flag = true;
			}
		}
		betZhuShu *= beiLv;
		if(flag == false){
			result = "";
			betZhuShu = 1;
			break;
		}
	}
	if(betZhuShu<1){
		betZhuShu = 1;
	}
	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}


String *ChongQingWuFen::getWuxingZhixuanBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};
	bool flag;
	int beiLv;
	for(int j = 0 ; j < 5 ; j++){
		flag = false;
		beiLv = 0;
		if(j!=0){
			result+=",";
		}
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[j][i] == 1)
			{
				result+=numberStr[i];
				beiLv++;
				flag = true;
			}
		}
		betZhuShu *= beiLv;
		if(flag == false){
			result = "";
			betZhuShu = 0;
			break;
		}
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

String *ChongQingWuFen::getZuxuan120BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;
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

	if(ballNum < 5){
		result = "";
		betZhuShu = 0;
	}else{
		betZhuShu = this->combo(ballNum, 5);
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

String *ChongQingWuFen::getZuxuan60BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum1 = 0;

	for(int i = 0 ; i < 10 ; i++)
	{
		if(ballStatus[0][i] == 1)
		{
			if(ballNum1 != 0) result+=",";
			result+=numberStr[i];			
			ballNum1++;
		}
	}

	if(ballNum1>0){		
		result+=";";
		int ballNum2 = 0;
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[1][i] == 1)  //计算总数和result
			{
				if(ballNum2 != 0) result+=",";
				result+=numberStr[i];				
				ballNum2++;
			}
		}
		if(ballNum2<3){
			result = "";
			betZhuShu = 0;
		}else{	
			int nRepeat = 0;
			betZhuShu = 0;
			for(int i = 0;i < 10;i ++)
			{		
				nRepeat = 0;
				if(ballStatus[0][i] == 1) 
				{
					if (ballStatus[1][i] == 1)
					nRepeat ++;
					//为1才算
					betZhuShu += combo(ballNum2 - nRepeat, 3);
				}				
			}						
		}
	}else{	
		result = "";
		betZhuShu = 0;
	}

	//if(result!=""){
	//	result+="|";
	//}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getZuxuan30BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum1 = 0;

	for(int i = 0 ; i < 10 ; i++)
	{
		if(ballStatus[0][i] == 1)
		{
			if(ballNum1 != 0) result+=",";
			result+=numberStr[i];
			ballNum1++;
		}
	}

	if(ballNum1>=2){
		result+=";";
		int ballNum2 = 0;
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[1][i] == 1)
			{
				if(ballNum2 != 0) result+=",";
				result+=numberStr[i];
				ballNum2++;
			}
		}
		if(ballNum2<=0){
			result = "";
			betZhuShu = 0;
		}else{	
			int nRepeat = 0;
			betZhuShu = 0;
			for(int i = 0;i < 10;i ++)
			{		
				nRepeat = 0;
				if(ballStatus[1][i] == 1) 
				{
					if (ballStatus[0][i] == 1)
						nRepeat ++;
					//为1才算
					betZhuShu += combo(ballNum1 - nRepeat, 2);
				}				
			}			
		}

	}else{	
		result = "";
		betZhuShu = 0;
	}

	//if(result!=""){
	//	result+="|";
	//}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getZuxuan20BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum1 = 0;

	for(int i = 0 ; i < 10 ; i++)
	{
		if(ballStatus[0][i] == 1)
		{
			if(ballNum1 != 0) result+=",";
			result+=numberStr[i];
			ballNum1++;
		}
	}

	if(ballNum1>0){
		betZhuShu = 0;
		result+=";";
		int ballNum2 = 0;
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[1][i] == 1)
			{
				if(ballNum2 != 0) result+=",";
				result+=numberStr[i];
				ballNum2++;
			}
		}
		if(ballNum2<2){
			result = "";
			betZhuShu = 0;
		}else{	
			int nRepeat = 0;
			betZhuShu = 0;
			for(int i = 0;i < 10;i ++)
			{		
				nRepeat = 0;
				if(ballStatus[0][i] == 1) 
				{
					if (ballStatus[1][i] == 1)
						nRepeat ++;
					//为1才算
					betZhuShu += combo(ballNum2 - nRepeat, 2);
				}				
			}			
		}
	}else{	
		result = "";
		betZhuShu = 0;
	}

	//if(result!=""){
	//	result+="|";
	//}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getZuxuan10BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum1 = 0;

	for(int i = 0 ; i < 10 ; i++)
	{
		if(ballStatus[0][i] == 1)
		{
			if(ballNum1 != 0) result+=",";
			result+=numberStr[i];
			ballNum1++;
		}
	}

	if(ballNum1>0){
		betZhuShu*=ballNum1;
		result+=";";
		int ballNum2 = 0;
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[1][i] == 1)
			{
				if(ballNum2 != 0) result+=",";
				result+=numberStr[i];
				ballNum2++;
			}
		}
		if(ballNum2 < 1){
			result = "";
			betZhuShu = 0;
		}else{	
			int nRepeat = 0;
			betZhuShu = 0;
			for(int i = 0;i < 10;i ++)
			{		
				nRepeat = 0;
				if(ballStatus[0][i] == 1) 
				{
					if (ballStatus[1][i] == 1) 
					{
						nRepeat ++;
					}
					//为1才算
					betZhuShu += ballNum2 - nRepeat;
				}				
			}
		}
	}else{	
		result = "";
		betZhuShu = 0;
	}

	//if(result!=""){
	//	result+="|";
	//}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getZuxuan5BetNumber()
{
	return this->getZuxuan10BetNumber();
}

String *ChongQingWuFen::getZuxuanQS24BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;
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

	if(ballNum < 4){
		result = "";
		betZhuShu = 0;
	}else{
		betZhuShu = this->combo(ballNum, 4);
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

String *ChongQingWuFen::getZuxuanQS12BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum1 = 0;

	for(int i = 0 ; i < 10 ; i++)
	{
		if(ballStatus[0][i] == 1)
		{
			if(ballNum1 != 0) result+=",";
			result+=numberStr[i];			
			ballNum1++;
		}
	}

	if(ballNum1>0){		
		result+=";";
		int ballNum2 = 0;
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[1][i] == 1)  //计算总数和result
			{
				if(ballNum2 != 0) result+=",";
				result+=numberStr[i];				
				ballNum2++;
			}
		}
		if(ballNum2<2){
			result = "";
			betZhuShu = 0;
		}else{	
			int nRepeat = 0;
			betZhuShu = 0;
			for(int i = 0;i < 10;i ++)
			{		
				nRepeat = 0;
				if(ballStatus[0][i] == 1) 
				{
					if (ballStatus[1][i] == 1)
						nRepeat ++;
					//为1才算
					betZhuShu += combo(ballNum2 - nRepeat, 2);
				}				
			}						
		}
	}else{	
		result = "";
		betZhuShu = 0;
	}

	//if(result!=""){
	//	result+="|";
	//}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getZuxuanQS6BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;
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
		result = "";
		betZhuShu = 0;
	}else{
		betZhuShu = this->combo(ballNum, 2);
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

String *ChongQingWuFen::getZuxuanQS4BetNumber()
{
	return getZuxuan5BetNumber();
}
//后四
String *ChongQingWuFen::getZuxuanHS24BetNumber()
{
	return getZuxuanQS24BetNumber();
}

String *ChongQingWuFen::getZuxuanHS12BetNumber()
{
	return getZuxuanQS12BetNumber();
}

String *ChongQingWuFen::getZuxuanHS6BetNumber()
{
	return getZuxuanQS6BetNumber();
}

String *ChongQingWuFen::getZuxuanHS4BetNumber()
{
	return getZuxuanQS4BetNumber();
}

String *ChongQingWuFen::getWuxingQianSiBetNumber()
{
	return this->getZhixuanBetNumber();
}

String *ChongQingWuFen::getWuxingHousiBetNumber()
{
	return this->getZhixuanBetNumber();
}

String *ChongQingWuFen::getQianSanZhiXuan_FuShiBetNumber()
{
	return this->getZhixuanBetNumber();
}

//String *ChongQingWuFen::getQianSanZhiXuan_DanShiBetNumber()
//{
//	return String::create("");
//}

String *ChongQingWuFen::getQianSanZuXuan_ZuSanFuShiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

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

	if(ballNum>=2){
		betZhuShu*=this->combo(ballNum,2)*2;
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

String *ChongQingWuFen::getQianSanZuXuan_ZuLiuFuShiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

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

	if(ballNum>=3){
		betZhuShu*=this->combo(ballNum,3);
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

String *ChongQingWuFen::getQianSanZhiXuanHeZhiBetNumber()
{
	betZhuShu = 0;
	char numberStr[30][3] = { {"00"},{"01"}, { "02" }, { "03" }, { "04" }, { "05" }, { "06" }, { "07" }, { "08" }, { "09" },
	{"10"},{"11"},{"12"},{"13"},{"14"},{"15"},{"16"},{"17"},{"18"},{"19"},
	{"20"},{"21"},{"22"},{"23"},{"24"},{"25"},{"26"},{"27"},{"28"},{"29"}};
	string result = "";
	int ballNum = 0;
	for(int i = 0 ; i < 28 ; i++)
	{
		if(ballStatus[5][i] == 1)
		{
			if(ballNum>0){
				result+=",";
			}
			result+=numberStr[i];
			betZhuShu += this->heZhiZhuShu(i);
			ballNum++;
		}
	}

	if(ballNum<1){
		result = "";
		betZhuShu = 0;
	}
	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getZhongSanZhiXuan_FuShiBetNumber()
{
	return this->getZhixuanBetNumber();
}

String *ChongQingWuFen::getZhongSanZuXuan_ZuSanFuShiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

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

	if(ballNum>=2){
		betZhuShu*=this->combo(ballNum,2)*2;
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

String *ChongQingWuFen::getZhongSanZuXuan_ZuLiuFuShiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

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

	if(ballNum>=3){
		betZhuShu*=this->combo(ballNum,3);
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

String *ChongQingWuFen::getZhongSanZhiXuanHeZhiBetNumber()
{
	betZhuShu = 0;
	char numberStr[30][3] = { {"00"},{"01"}, { "02" }, { "03" }, { "04" }, { "05" }, { "06" }, { "07" }, { "08" }, { "09" },
	{"10"},{"11"},{"12"},{"13"},{"14"},{"15"},{"16"},{"17"},{"18"},{"19"},
	{"20"},{"21"},{"22"},{"23"},{"24"},{"25"},{"26"},{"27"},{"28"},{"29"}};
	string result = "";
	int ballNum = 0;
	for(int i = 0 ; i < 28 ; i++)
	{
		if(ballStatus[5][i] == 1)
		{
			if(ballNum>0){
				result+=",";
			}
			result+=numberStr[i];
			betZhuShu += this->heZhiZhuShu(i);
			ballNum++;
		}
	}
	if(ballNum<1){
		result = "";
		betZhuShu = 0;
	}
	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getHouSanZhiXuan_FuShiBetNumber()
{
	return this->getZhixuanBetNumber();
}

String *ChongQingWuFen::getHouSanZuXuan_ZuSanFuShiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

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

	if(ballNum>=2){
		betZhuShu*=this->combo(ballNum,2)*2;
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

String *ChongQingWuFen::getHouSanZuXuan_ZuLiuFuShiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

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

	if(ballNum>=3){
		betZhuShu*=this->combo(ballNum,3);
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

String *ChongQingWuFen::getHouSanZhiXuanHeZhiBetNumber()
{
	betZhuShu = 0;
	char numberStr[30][3] = {{"00"},{"01"},{"02"},{"03"},{"04"},{"05"},{"06"},{"07"},{"08"},{"09"},
	{"10"},{"11"},{"12"},{"13"},{"14"},{"15"},{"16"},{"17"},{"18"},{"19"},
	{"20"},{"21"},{"22"},{"23"},{"24"},{"25"},{"26"},{"27"},{"28"},{"29"}};
	string result = "";
	int ballNum = 0;
	for(int i = 0 ; i < 28 ; i++)
	{
		if(ballStatus[5][i] == 1)
		{
			if(ballNum>0){
				result+=",";
			}
			result+=numberStr[i];
			betZhuShu += this->heZhiZhuShu(i);
			ballNum++;
		}
	}
	if(ballNum<1){
		result = "";
		betZhuShu = 0;
	}
	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getWuXingHouEr_FuShiBetNumber()
{
	return this->getZhixuanBetNumber();
}

String *ChongQingWuFen::getWuXingQianEr_FuShiBetNumber()
{
	return this->getZhixuanBetNumber();
}

String *ChongQingWuFen::getHouErZuXuan_FuShiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

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

	if(ballNum>=2){
		betZhuShu*=this->combo(ballNum,2);
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

String *ChongQingWuFen::getQianErZuXuan_FuShiBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

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

	if(ballNum>=2){
		betZhuShu*=this->combo(ballNum,2);
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
//定位 大小单双
String *ChongQingWuFen::GetDaXiaoDanShuang_DW(bool isRepeat,int num)
{
	betZhuShu = 1;
	string result = "";
	char numberStr[4][3] = { { "9" }, { "1" }, { "3" }, { "2" } };

	int ballNum = 0;

	int rowCount = 0;
	int resultCount = 1;

	for (int j = 0; j < 5; j++){
		rowCount = 0;
		//跳过
		if(isRepeat && j < 3) continue;
		if(num == 1 && j > 0) continue;

		for (int i = 0; i < 4; i++)
		{
			if (ballStatus[j][i] == 1)
			{
				result += numberStr[i];
				ballNum++;
				rowCount ++;
				if(!isRepeat && rowCount > 1)
				{
					result = "";
					betZhuShu = 0;
					return String::create("");
				}
			}
		}
		
		resultCount *= rowCount;
		
		if (j != 4 && num != 1){
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

	if(isRepeat)
	{
		betZhuShu = resultCount;
	}

	if (result != ""){
		result += "|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

//定位胆
String *ChongQingWuFen::getDingWeiDanBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

	for(int j = 0 ; j < 5 ; j++){
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[j][i] == 1)
			{
				result+=numberStr[i];
				ballNum++;
			}
		}
		if(j != 4){
			result+=",";
		}
	}

	if(ballNum>=1){
		betZhuShu*=ballNum;
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

String *ChongQingWuFen::getBuDingWeiBetNumber()
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
	if(ballNum>=1){
		betZhuShu*=ballNum;
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

String *ChongQingWuFen::getBuDingWei_HouSanBetNumber()
{
	return this->getBuDingWeiBetNumber();
}

String *ChongQingWuFen::getBuDingWei_QianSanBetNumber()
{
	return this->getBuDingWeiBetNumber();
}

String *ChongQingWuFen::getBuDingWei_HouErBetNumber()
{
	return this->getBuDingWeiBetNumber();
}

String *ChongQingWuFen::getBuDingWei_QianErBetNumber()
{
	return this->getBuDingWeiBetNumber();
}

String *ChongQingWuFen::getQW_YiFanFengShunBetNumber()
{
	return this->getBuDingWeiBetNumber();
}

String *ChongQingWuFen::getQW_HaoShiChengShuangBetNumber()
{
	return this->getBuDingWeiBetNumber();
}

String *ChongQingWuFen::getQW_SanXingBaoXiBetNumber()
{
	return this->getBuDingWeiBetNumber();
}

String *ChongQingWuFen::getQW_SiJiFaCaiBetNumber()
{
	return this->getBuDingWeiBetNumber();
}

String *ChongQingWuFen::getSSCLongHuBetNumber()
{
	betZhuShu = 1;
	string result = "";
	string numberStr[10] = {"11000","10100","10010","10001","01100","01010","01001","00110","00101","00011"};
	string longhuStr[3] = {"6","4","5"};
	int ballNum = 0;

	for(int i = 0 ; i < 30 ; i++)
	{
		if(ballStatus[0][i] == 1)
		{
			if(ballNum>0){
				result+="|";
			}
			result+= longhuStr[i % 3] + "," + numberStr[i / 3];
			ballNum++;
		}
	}

	betZhuShu=ballNum;	

	if(result!=""){
		result+="|";
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String* ChongQingWuFen::getDanShiBetNumber()
{	
	//创建字符串
	String* result = String::create(m_betString);

	if(m_betString.size() < 2) result = String::create("");

	return result;
}

void ChongQingWuFen::setDanShiCount()
{
	switch (m_betKind)
	{
	case 	WuxingZhixuan_DanShi:
		m_num = 5;
		m_repeat = 0;
		break;
	case 	WuxingQianSi_DanShi:
		m_num = 4;
		m_repeat = 0;
		break;
	case 	WuxingHousi_DanShi:
		m_num = 4;
		m_repeat = 0;
		break;
	case 	QianSanZhiXuan_DanShi:
		m_num = 3;
		m_repeat = 0;
		break;
	case 	QianSanZuXuan_ZuLiuDanShi:
		m_num = 3;
		m_repeat = 2;
		break;
	case 	QianSanZuXuan_ZuSanDanShi:
		m_num = 3;
		m_repeat = 1;
		break;
	case 	QianSanHunhe:
		m_num = 3;
		m_repeat = 3;
		break;
	case 	WuxingZhongsan_DanShi:
		m_num = 3;
		m_repeat = 0;
		break;
	case 	ZhongsanZuliu_DanShi:
		m_num = 3;
		m_repeat = 2;
		break;
	case 	ZhongsanZusan_DanShi:
		m_num = 3;
		m_repeat = 1;
		break;
	case 	ZhongSanHunhe:
		m_num = 3;
		m_repeat = 3;
		break;
	case 	HouSanZhiXuan_DanShi:
		m_num = 3;
		m_repeat = 0;
		break;
	case 	HouSanZuXuan_ZuLiuDanShi:
		m_num = 3;
		m_repeat = 2;
		break;
	case 	HouSanZuXuan_ZuSanDanShi:
		m_num = 3;
		m_repeat = 1;
		break;
	case 	HouSanHunhe:
		m_num = 3;
		m_repeat = 3;
		break;
	case 	WuXingQianEr_DanShi:
		m_num = 2;
		m_repeat = 0;
		break;
	case 	QianErZuXuan_DanShi:
		m_num = 2;
		m_repeat = 2;
		break;
	case 	WuXingHouEr_DanShi:
		m_num = 2;
		m_repeat = 0;
		break;
	case 	HouErZuXuan_DanShi:
		m_num = 2;
		m_repeat = 2;
		break;
	case RenXuan4_DanShi:  //任四单式
		m_num = 4;
		m_repeat = 0;  //0是没有要求 1是必须重复而且只重复一次，2是必须不重复 3是不能全相同
		break;
	case RenXuan3_DanShi:
		m_num = 3;
		m_repeat = 0;
		break;
	case RenXuan2_DanShi:
		m_num = 2;
		m_repeat = 0;
		break;
	case Ren2Zuxuan_Danshi:
		m_num = 2;
		m_repeat = 2;
		break;
	case Ren3Zu3_DanShi:
		m_num = 3;
		m_repeat = 1;
		break;
	case Ren3Zu6_DanShi:
		m_num = 3;
		m_repeat = 2;
		break;
	case Ren3Hunhe_Zuxuan:
		m_num = 3;
		m_repeat = 0;
		break;
	default:
		m_num = 0;
		m_repeat = 0;
		break;
	}
}

void ChongQingWuFen::createRandomNumbers()
{
	CCLOG("ChongQingWuFen::createRandomNumbers");

	if(this->getChildByTag(9528) != nullptr)
	{
		createDanShiRandomNumber();
		return;
	}

	switch (m_betKind)
	{
	case WuxingZhixuan:
		{
			this->createWuxingZhixuanRandomNumbers();
			break;
		}
	case Zuxuan120:
		{
			this->createZuxuan120RandomNumbers();
			break;
		}
	case Zuxuan60:
		{
			this->createZuxuan60RandomNumbers();
			break;
		}
	case Zuxuan30:
		{
			this->createZuxuan30RandomNumbers();
			break;
		}
	case Zuxuan20:
		{
			this->createZuxuan20RandomNumbers();
			break;
		}
	case Zuxuan10:
		{
			this->createZuxuan10RandomNumbers();
			break;
		}
	case Zuxuan5:
		{
			this->createZuxuan5RandomNumbers();
			break;
		}
	case WuxingQianSi:
		{
			this->createWuxingQianSiRandomNumbers();
			break;
		}
	case QSZuXuan24:
		{
			this->createZuxuanQS24RandomNumber();
			break;
		}
	case QSZuXuan12:
		{
			this->createZuxuanQS12RandomNumber();
			break;
		}
	case QSZuXuan6:
		{
			this->createZuxuanQS6RandomNumber();
			break;
		}
	case QSZuXuan4:
		{
			this->createZuxuanQS4RandomNumber();
			break;
		}	
	case HSZuXuan24:
		{
			this->createZuxuanHS24RandomNumber();
			break;
		}
	case HSZuXuan12:
		{
			this->createZuxuanHS12RandomNumber();
			break;
		}
	case HSZuXuan6:
		{
			this->createZuxuanHS6RandomNumber();
			break;
		}
	case HSZuXuan4:
		{
			this->createZuxuanHS4RandomNumber();
			break;
		}
	case WuxingHousi:
		{
			this->createWuxingHousiRandomNumbers();
			break;
		}
	case QianSanZhiXuan_FuShi:
		{
			this->createQianSanZhiXuan_FuShiRandomNumbers();
			break;
		}
	case QianSanZuXuan_ZuSanFuShi:
		{
			this->createQianSanZuXuan_ZuSanFuShiRandomNumbers();
			break;
		}
	case QianSanZuXuan_ZuLiuFuShi:
		{
			this->createQianSanZuXuan_ZuLiuFuShiRandomNumbers();
			break;
		}
	case QianSanZhiXuanHeZhi:
		{
			this->createQianSanZhiXuanHeZhiRandomNumbers();
			break;
		}
	case WuxingZhongsan:
		{
			this->createZhongSanZhiXuan_FuShiRandomNumbers();
			break;
		}
	case ZhongsanZusan:
		{
			this->createZhongSanZuXuan_ZuSanFuShiRandomNumbers();
			break;
		}
	case ZhongsanZuliu:
		{
			this->createZhongSanZuXuan_ZuLiuFuShiRandomNumbers();
			break;
		}
	case ZhongSanZhiXuanHeZhi:
		{
			this->createZhongSanZhiXuanHeZhiRandomNumbers();
			break;
		}
	case HouSanZhiXuan_FuShi:
		{
			this->createHouSanZhiXuan_FuShiRandomNumbers();
			break;
		}
	case HouSanZuXuan_ZuSanFuShi:
		{
			this->createHouSanZuXuan_ZuSanFuShiRandomNumbers();
			break;
		}
	case HouSanZuXuan_ZuLiuFuShi:
		{
			this->createHouSanZuXuan_ZuLiuFuShiRandomNumbers();
			break;
		}
	case HouSanZhiXuanHeZhi:
		{
			this->createHouSanZhiXuanHeZhiRandomNumbers();
			break;
		}
	case WuXingHouEr_FuShi:
		{
			this->createWuXingHouEr_FuShiRandomNumbers();
			break;
		}
	case WuXingQianEr_FuShi:
		{
			this->createWuXingQianEr_FuShiRandomNumbers();
			break;
		}
	case HouErZuXuan_FuShi:
		{
			this->createHouErZuXuan_FuShiRandomNumbers();
			break;
		}
	case QianErZuXuan_FuShi:
		{
			this->createQianErZuXuan_FuShiRandomNumbers();
			break;
		}
	case DingWeiDan:
		{
			this->createDingWeiDanRandomNumbers();
			break;
		}
	case DaXiaoDanShuang_DW:
	{		
		this->createDXDSRandomNumbers(5);		//大小单双机选
		break;
	}
	case DaXiaoDanShuang:
		{
			this->createDXDSRandomNumbers(7);		//后二大小单双机选
			break;
		}
	case Zonghe_DXDSLh:
		{
			this->createDXDSRandomNumbers(1);		//总和大小单双机选
			break;
		}
	case BuDingWei_HouSan:
		{
			this->createBuDingWei_HouSanRandomNumbers();
			break;
		}
	case BuDingWei_QianSan:
		{
			this->createBuDingWei_QianSanRandomNumbers();
			break;
		}
	case BuDingWei_HouEr:
		{
			this->createBuDingWei_HouErRandomNumbers();
			break;
		}
	case BuDingWei_QianEr:
		{
			this->createBuDingWei_QianErRandomNumbers();
			break;
		}
	case QW_YiFanFengShun:
		{
			this->createQW_YiFanFengShunRandomNumbers();
			break;
		}
	case QW_HaoShiChengShuang:
		{
			this->createQW_HaoShiChengShuangRandomNumbers();
			break;
		}
	case QW_SanXingBaoXi:
		{
			this->createQW_SanXingBaoXiRandomNumbers();
			break;
		}
	case QW_SiJiFaCai:
		{
			this->createQW_SiJiFaCaiRandomNumbers();
			break;
		}
	case SSC_LongHu:
		{
			this->createLongHuHeRandomNumbers();
			break;
		}
	case RenXuan4:		//任选4
		{
			this->createRen4RandomNumbers();
			break;
		}
	case RenXuan3:
		{
			this->createRen3RandomNumbers();
			break;
		}
	case Ren3Zu3_Fushi:
		{
			this->createRen3Zu3RandomNumbers();
			break;
		}
	case Ren3Zu6_Fushi:
		{
			this->createRen3Zu6RandomNumbers();
			break;
		}
	case RenXuan2:
		{
			this->createRen2RandomNumbers();
			break;
		}
	case Ren2Zuxuan_Fushi:
		{
			this->createRen2ZuXuanRandomNumbers();
			break;
		}

	default:
		{
			this->clearNumbers();
			for(int j = 0 ; j < 6 ; j++){
				if(areaStatus[j] == 0){
					continue;
				}
				SpriteFrame *ballSpriteFrame; 
				Texture2D *ballTexture2D; 
				int ballNum = 10;
				if(j == 5){
					ballNum = 28;
					ballSpriteFrame = spriteFrame("gamble_00.png");
				}else{
					ballSpriteFrame = spriteFrame("gamble_01.png");
				}
				ballTexture2D = ballSpriteFrame->getTexture();
				int num = rand()%ballNum;
				ballStatus[j][num] = 1-ballStatus[j][num];
				Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+num);
				Rect rect = ballSprite->boundingBox();
				ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
					Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
			}
			break;
		}
	}
}

void ChongQingWuFen::createWuxingZhixuanRandomNumbers()
{
	this->clearNumbers();
	for(int j = 0 ; j < 6 ; j++){
		if(areaStatus[j] == 0){
			continue;
		}
		SpriteFrame *ballSpriteFrame; 
		Texture2D *ballTexture2D; 
		int ballNum = 10;
		if(j == 5){
			ballNum = 28;
			ballSpriteFrame = spriteFrame("gamble_00.png");
		}else{
			ballSpriteFrame = spriteFrame("gamble_01.png");
		}
		ballTexture2D = ballSpriteFrame->getTexture();
		int num = rand()%ballNum;
		ballStatus[j][num] = 1-ballStatus[j][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}

void ChongQingWuFen::createRenxuanRandomNumber(int num)
{
	if(num > 5) return;
	int myCount[5] = {1,1,1,1,1};
	this->clearNumbers();

	//任选多少个数
	for(int i = 0;i < 5 - num;i ++)
	{
		int tempInt = rand() % 5;
		while (myCount[tempInt] == 0)
		{
			tempInt = (tempInt + 1) % 5;			
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
		if(j == 5){
			ballNum = 28;
			ballSpriteFrame = spriteFrame("gamble_00.png");
		}else{
			ballSpriteFrame = spriteFrame("gamble_01.png");
		}
		ballTexture2D = ballSpriteFrame->getTexture();
		int num = rand()%ballNum;
		ballStatus[j][num] = 1-ballStatus[j][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+j*10+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
};

void ChongQingWuFen::createZuXuanRandomNumbers(int area, int num)
{
	this->clearNumbers(area);
	for(int j = 0 ; j < num ; j++){
		SpriteFrame *ballSpriteFrame; 
		Texture2D *ballTexture2D; 
		int ballNum = 10;
		if(area == 5){
			ballNum = 28;
			ballSpriteFrame = spriteFrame("gamble_00.png");
		}else{
			ballSpriteFrame = spriteFrame("gamble_01.png");
		}
		ballTexture2D = ballSpriteFrame->getTexture();
		int num = rand()%ballNum;
		while(ballStatus[area][num] == 1){
			num = rand()%ballNum;
		}
		ballStatus[area][num] = 1-ballStatus[area][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+area*AREA_BALL_GAP+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}

void ChongQingWuFen::createZuxuan120RandomNumbers()
{
	this->clearNumbers();
	for(int j = 0 ; j < 5 ; j++){
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
void ChongQingWuFen::createZuxuan60RandomNumbers()
{
	this->clearNumbers();
	SpriteFrame *ballSpriteFrame; 
	Texture2D *ballTexture2D; 
	int ballNum = 10;
	ballSpriteFrame = spriteFrame("gamble_01.png");
	ballTexture2D = ballSpriteFrame->getTexture();

	int num = rand()%ballNum;
	ballStatus[0][num] = 1-ballStatus[0][num];
	Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+num);
	Rect rect = ballSprite->boundingBox();
	ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
		Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));

	for(int j = 0 ; j < 3 ; j++){
		num = rand()%ballNum;
		while(ballStatus[1][num] == 1){
			num = rand()%ballNum;
		}
		ballStatus[1][num] = 1-ballStatus[1][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+10+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}
void ChongQingWuFen::createZuxuan30RandomNumbers()
{
	this->clearNumbers();
	SpriteFrame *ballSpriteFrame; 
	Texture2D *ballTexture2D; 
	int ballNum = 10;
	ballSpriteFrame = spriteFrame("gamble_01.png");
	ballTexture2D = ballSpriteFrame->getTexture();

	int num;
	for(int j = 0 ; j < 2 ; j++){
		num = rand()%ballNum;
		while(ballStatus[1][num] == 1){
			num = rand()%ballNum;
		}
		ballStatus[0][num] = 1-ballStatus[0][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}

	num = rand()%ballNum;
	ballStatus[1][num] = 1-ballStatus[1][num];
	Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+10+num);
	Rect rect = ballSprite->boundingBox();
	ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
		Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
}

void ChongQingWuFen::createZuxuan20RandomNumbers()
{
	this->clearNumbers();
	SpriteFrame *ballSpriteFrame; 
	Texture2D *ballTexture2D; 
	int ballNum = 10;
	ballSpriteFrame = spriteFrame("gamble_01.png");
	ballTexture2D = ballSpriteFrame->getTexture();

	int num;
	num = rand()%ballNum;
	ballStatus[0][num] = 1-ballStatus[0][num];
	Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+num);
	Rect rect = ballSprite->boundingBox();
	ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
		Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));

	for(int j = 0 ; j < 2 ; j++){
		num = rand()%ballNum;
		while(ballStatus[1][num] == 1){
			num = rand()%ballNum;
		}
		ballStatus[1][num] = 1-ballStatus[1][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+10+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}

void ChongQingWuFen::createZuxuan10RandomNumbers()
{
	this->clearNumbers();
	SpriteFrame *ballSpriteFrame; 
	Texture2D *ballTexture2D; 
	int ballNum = 10;
	ballSpriteFrame = spriteFrame("gamble_01.png");
	ballTexture2D = ballSpriteFrame->getTexture();

	int num;
	num = rand()%ballNum;
	ballStatus[0][num] = 1-ballStatus[0][num];
	Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+num);
	Rect rect = ballSprite->boundingBox();
	ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
		Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));

	num = rand()%ballNum;
	ballStatus[1][num] = 1-ballStatus[1][num];
	Sprite *ballSprite1 = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+10+num);
	ballSprite1->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
		Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
}

//大小单双
void ChongQingWuFen::createDXDSRandomNumbers(int nArea)		//大小单双机选
{
	//此函数最多随机一个数字出来 最多5个数字，大于5则是从后面开始数  比如7就是3,4号位置
	//位置信息
	int iAreaPos = 0;
	if(nArea > 5)
	{
		iAreaPos = nArea % 5;
		iAreaPos = rand() % iAreaPos  + (5 - iAreaPos);
	}else
	{
		iAreaPos = rand() % nArea;
	}
	
	for (int j = 0; j < 5; j++)
	{
		this->clearNumbers(j);
	}

	SpriteFrame *ballSpriteFrame;
	Texture2D *ballTexture2D;
	int ballNum = 4;
	ballSpriteFrame = spriteFrame("gamble_011.png");

	ballTexture2D = ballSpriteFrame->getTexture();
	int num = rand() % ballNum;
	while (ballStatus[iAreaPos][num] == 1){
		num = rand() % ballNum;
	}
	ballStatus[iAreaPos][num] = 1 - ballStatus[iAreaPos][num];
	Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG + iAreaPos*AREA_BALL_GAP + num);
	Rect rect = ballSprite->boundingBox();
	ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D, Rect(num * ballRadius,ballRadius,ballRadius,ballRadius)));
}

void ChongQingWuFen::createZuxuan5RandomNumbers()
{
	this->createZuxuan10RandomNumbers();
}

//前四
void ChongQingWuFen::createZuxuanQS24RandomNumber()
{
	this->clearNumbers();
	for(int j = 0 ; j < 4 ; j++){
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

void ChongQingWuFen::createZuxuanQS12RandomNumber()
{
	this->createZuxuan20RandomNumbers();
}

void ChongQingWuFen::createZuxuanQS6RandomNumber()
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
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect(num*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}

void ChongQingWuFen::createZuxuanQS4RandomNumber()
{
	this->createZuxuan5RandomNumbers();
}

//后四
void ChongQingWuFen::createZuxuanHS24RandomNumber()
{
	this->createZuxuanQS24RandomNumber();
}

void ChongQingWuFen::createZuxuanHS12RandomNumber()
{
	this->createZuxuanQS12RandomNumber();
}

void ChongQingWuFen::createZuxuanHS6RandomNumber()
{
	this->createZuxuanQS6RandomNumber();
}

void ChongQingWuFen::createZuxuanHS4RandomNumber()
{
	this->createZuxuanQS4RandomNumber();
}

void ChongQingWuFen::createWuxingQianSiRandomNumbers()
{
	this->createWuxingZhixuanRandomNumbers();
}
void ChongQingWuFen::createWuxingHousiRandomNumbers()
{
	this->createWuxingZhixuanRandomNumbers();
}
void ChongQingWuFen::createQianSanZhiXuan_FuShiRandomNumbers()
{
	this->createWuxingZhixuanRandomNumbers();
}
void ChongQingWuFen::createQianSanZuXuan_ZuSanFuShiRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,2);
}
void ChongQingWuFen::createQianSanZuXuan_ZuLiuFuShiRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,3);
}
void ChongQingWuFen::createQianSanZhiXuanHeZhiRandomNumbers()
{
	this->createZuXuanRandomNumbers(5,1);
}
void ChongQingWuFen::createZhongSanZhiXuan_FuShiRandomNumbers()
{
	this->createWuxingZhixuanRandomNumbers();
}
void ChongQingWuFen::createZhongSanZuXuan_ZuSanFuShiRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,2);
}
void ChongQingWuFen::createZhongSanZuXuan_ZuLiuFuShiRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,3);
}
void ChongQingWuFen::createZhongSanZhiXuanHeZhiRandomNumbers()
{
	this->createZuXuanRandomNumbers(5,1);
}
void ChongQingWuFen::createHouSanZhiXuan_FuShiRandomNumbers()
{
	this->createWuxingZhixuanRandomNumbers();
}
void ChongQingWuFen::createHouSanZuXuan_ZuSanFuShiRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,2);
}
void ChongQingWuFen::createHouSanZuXuan_ZuLiuFuShiRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,3);
}
void ChongQingWuFen::createHouSanZhiXuanHeZhiRandomNumbers()
{
	this->createZuXuanRandomNumbers(5,1);
}
void ChongQingWuFen::createWuXingHouEr_FuShiRandomNumbers()
{
	this->createWuxingZhixuanRandomNumbers();
}
void ChongQingWuFen::createWuXingQianEr_FuShiRandomNumbers()
{
	this->createWuxingZhixuanRandomNumbers();
}
void ChongQingWuFen::createHouErZuXuan_FuShiRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,2);
}
void ChongQingWuFen::createQianErZuXuan_FuShiRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,2);
}
void ChongQingWuFen::createDingWeiDanRandomNumbers()
{
	this->clearNumbers();
	int area = rand()%5;
	this->createZuXuanRandomNumbers(area,1);
}
void ChongQingWuFen::createBuDingWei_HouSanRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,1);
}
void ChongQingWuFen::createBuDingWei_QianSanRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,1);
}
void ChongQingWuFen::createBuDingWei_HouErRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,1);
}
void ChongQingWuFen::createBuDingWei_QianErRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,1);
}
void ChongQingWuFen::createQW_YiFanFengShunRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,1);
}
void ChongQingWuFen::createQW_HaoShiChengShuangRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,1);
}
void ChongQingWuFen::createQW_SanXingBaoXiRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,1);
}
void ChongQingWuFen::createQW_SiJiFaCaiRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,1);
}

void ChongQingWuFen::createLongHuHeRandomNumbers()
{
	this->clearNumbers(0);
	for(int j = 0 ; j < 1 ; j++){
		SpriteFrame *ballSpriteFrame; 
		Texture2D *ballTexture2D; 
		int ballNum = 30;
		
		ballSpriteFrame = spriteFrame("gamble_02_1.png");
		
		ballTexture2D = ballSpriteFrame->getTexture();
		int num = rand()%ballNum;
		while(ballStatus[0][num] == 1){
			num = rand()%ballNum;
		}
		ballStatus[0][num] = 1-ballStatus[0][num];
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+num);
		Rect rect = ballSprite->boundingBox();
		ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
			Rect((num % 3)*ballRadius,ballRadius,ballRadius,ballRadius)));
	}
}

void ChongQingWuFen::createDanShiRandomNumber()
{
	this->clearNumbers(0);

	betZhuShu = 0;
}

void ChongQingWuFen::clearNumbers()
{
	int ballNum;
	for(int j = 0 ; j < 6 ; j++){
		if(j == 5){
			ballNum = 28;
		}else{
			ballNum = 10;
		}
		if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
			ballNum = 4;
		if(m_betKind == SSC_LongHu) ballNum = 30;
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
			if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
				ballSpriteFrame = spriteFrame("gamble_011.png");
			if(m_betKind == SSC_LongHu)
				ballSpriteFrame = spriteFrame("gamble_02_1.png");
			Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
			if(ballStatus[j][i] == 1){

				if(m_betKind == SSC_LongHu)
				{
					ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
						Rect((i%3)*ballRadius,0,ballRadius,ballRadius)));
					ballStatus[j][i] = 1-ballStatus[j][i];
				} else
				{			
					ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
						Rect(i*ballRadius,0,ballRadius,ballRadius)));
					ballStatus[j][i] = 1-ballStatus[j][i];
				}
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

void ChongQingWuFen::clearNumbers(int area)
{
	int ballNum = 10;
	if(area == 5)
	{
		ballNum = 28;
	}
	if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
		ballNum = 4;
	if(m_betKind == SSC_LongHu) ballNum = 30;
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
		if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
			ballSpriteFrame = spriteFrame("gamble_011.png");
		if(m_betKind == SSC_LongHu)
			ballSpriteFrame = spriteFrame("gamble_02_1.png");
		Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
		if(ballStatus[area][i] == 1){
			if(m_betKind == SSC_LongHu)
			{
				ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
					Rect((i%3)*ballRadius,0,ballRadius,ballRadius)));
				ballStatus[area][i] = 1-ballStatus[area][i];
			} else
			{			
				ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
					Rect(i*ballRadius,0,ballRadius,ballRadius)));
				ballStatus[area][i] = 1-ballStatus[area][i];
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

//大
void ChongQingWuFen::pressBig(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
		return;
	
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
void ChongQingWuFen::pressLittle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();
	if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
		return;
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
void ChongQingWuFen::pressAll(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();
	if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
		return;
	
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
void ChongQingWuFen::pressSingle(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
		return;
	
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
void ChongQingWuFen::pressDouble(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
		return;
	
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
void ChongQingWuFen::pressClear(Object *obj)
{
	MenuItemSprite *menuItem = (MenuItemSprite *)obj;
	Vec2 pos = menuItem->getPosition(); pos = menuItem->convertToWorldSpace(pos);if(pos.y < 280) return;

	playButtonSound();

	if (m_betKind == DaXiaoDanShuang_DW || m_betKind == Zonghe_DXDSLh || m_betKind == DaXiaoDanShuang)
		return;

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
void ChongQingWuFen::cancelMenuSelect(int itemTag)
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

void ChongQingWuFen::cancelAllMenuSelect()
{
	for(int i = W_AREA_MENU_TAG ; i <= G_AREA_MENU_TAG ; i+=(Q_AREA_MENU_TAG-W_AREA_MENU_TAG))
	{
		this->cancelMenuSelect(i);
	}
}

void ChongQingWuFen::createRen4RandomNumbers()
{
	this->createRenxuanRandomNumber(4);
}

void ChongQingWuFen::createRen3RandomNumbers()
{
	this->createRenxuanRandomNumber(3);
}

void ChongQingWuFen::createRen3Zu3RandomNumbers()
{
	this->createZuXuanRandomNumbers(0,2);
}

void ChongQingWuFen::createRen3Zu6RandomNumbers()
{
	this->createZuXuanRandomNumbers(0,3);
}

void ChongQingWuFen::createRen2RandomNumbers()
{
	this->createRenxuanRandomNumber(2);
}

void ChongQingWuFen::createRen2ZuXuanRandomNumbers()
{
	this->createZuXuanRandomNumbers(0,2);
}

String *ChongQingWuFen::getRen4BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;
	int maxRowCount = 0;
	int selRowCount = 0; //已经选择的行数

	for(int j = 0 ; j < 5 ; j++){  //5行
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
			if(itemcount > 9) maxRowCount ++; //大于9个数的情况次数
		}
		if(j != 4){
			result+=",";
		}
	}	

	//if(ballNum < 1 || selRowCount != 4){  //改成能全选，则不用判断==4
	if(ballNum < 1 || selRowCount < 4){
		result = "";
		betZhuShu = 0;
	}else
	if(selRowCount > 4)	//如果大于4则需要拆单
	{
		betZhuShu = BetLimit::GetStrFromSplit(result, 4,8000);

		if(betZhuShu == 0) //最大
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
				ConfigMgr::instance()->text("display_text.xml","t4800"),
				Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));		
			this->addChild(layer,255);
			result.clear();		
		}
	}else
	{
		result+="|";
		int maxCount = 8000;
		if(betZhuShu > maxCount)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
				ConfigMgr::instance()->text("display_text.xml","t4800"),
				Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));		
			this->addChild(layer,255);
			result.clear();	
			betZhuShu = 0;
		}
	}

	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getRen3BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;
	int maxRowCount = 0;
	int selRowCount = 0; //已经选择的行数

	for(int j = 0 ; j < 5 ; j++){  //5行
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
			if(itemcount > 9) maxRowCount ++;
		}
		if(j != 4){
			result+=",";			
		}
	}



	if(ballNum < 1 || selRowCount < 3){
		result = "";
		betZhuShu = 0;
	}else
	if(selRowCount > 3)	//如果大于4则需要拆单
	{
		betZhuShu = BetLimit::GetStrFromSplit(result, 3,800);	
		if(betZhuShu == 0) //最大
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
				ConfigMgr::instance()->text("display_text.xml","t4801"),
				Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));		
			this->addChild(layer,255);
			result.clear();		
		}
	}else
	{
		result+="|";
		int maxCount = 800;
		if(betZhuShu > maxCount)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
				ConfigMgr::instance()->text("display_text.xml","t4801"),
				Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));		
			this->addChild(layer,255);
			result.clear();	
			betZhuShu = 0;
		}
	}

	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getRen3Zu3BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

	for(int j = 0 ; j < 5 ; j++){	
		if(areaStatus[j] == 0){
			continue;
		}
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[j][i] == 1)
			{
				result+=numberStr[i];
				ballNum++;
			}
		}
		if(j != 4){
			result+=",";
		}
	}

	int tempSelected = isWanWeiSelected + isQianWeiSelected + isBaiWeiSelected + isShiWeiSelected + isGeWeiSelected;	

	if(ballNum>=2 && tempSelected >= 3){
		int nBei = combo(tempSelected, 3);
		betZhuShu *=combo(ballNum, 2) * nBei * 2;
	}else{	
		result = "";
		betZhuShu = 0;
	}

	if(result!=""){
		String* tempStr = String::createWithFormat("%d%d%d%d%d",isWanWeiSelected ,isQianWeiSelected ,isBaiWeiSelected , isShiWeiSelected , isGeWeiSelected);
		result.append(tempStr->getCString());
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getRen3Zu6BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

	for(int j = 0 ; j < 5 ; j++){	
		if(areaStatus[j] == 0){
			continue;
		}
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[j][i] == 1)
			{
				result+=numberStr[i];
				ballNum++;
			}
		}
		if(j != 4){
			result+=",";
		}
	}

	int tempSelected = isWanWeiSelected + isQianWeiSelected + isBaiWeiSelected + isShiWeiSelected + isGeWeiSelected;	

	if(ballNum>=3 && tempSelected >= 3){
		int nBei = combo(tempSelected, 3);
		betZhuShu *=combo(ballNum, 3) * nBei;
	}else{	
		result = "";
		betZhuShu = 0;
	}

	if(result!=""){
		String* tempStr = String::createWithFormat("%d%d%d%d%d",isWanWeiSelected ,isQianWeiSelected ,isBaiWeiSelected , isShiWeiSelected , isGeWeiSelected);
		result.append(tempStr->getCString());
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getRen2BetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;
	int maxRowCount = 0;
	int selRowCount = 0; //已经选择的行数 

	for(int j = 0 ; j < 5 ; j++){  //5行
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
			if(maxRowCount > 8) maxRowCount ++;
		}
		if(j != 4){
			result+=",";
		}
	}

	

	if(ballNum < 1 || selRowCount < 2){
		result = "";
		betZhuShu = 0;
	}else
	if(selRowCount > 2)	//如果大于4则需要拆单
	{
		betZhuShu = BetLimit::GetStrFromSplit(result, 2, 80);
		if(betZhuShu == 0) //最大
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
				ConfigMgr::instance()->text("display_text.xml","t4802"),
				Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));		
			this->addChild(layer,255);
			result.clear();		
		}
	}else
	{
		result+="|";
		int maxCount = 80;
		if(betZhuShu > maxCount)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
				ConfigMgr::instance()->text("display_text.xml","t4802"),
				Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));		
			this->addChild(layer,255);
			result.clear();	
			betZhuShu = 0;
		}
	}

	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

String *ChongQingWuFen::getRen2ZuXuanBetNumber()
{
	betZhuShu = 1;
	string result = "";
	char numberStr[10][3] = {{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

	int ballNum = 0;

	for(int j = 0 ; j < 5 ; j++){	
		if(areaStatus[j] == 0){
			continue;
		}
		for(int i = 0 ; i < 10 ; i++)
		{
			if(ballStatus[j][i] == 1)
			{
				result+=numberStr[i];
				ballNum++;
			}
		}
		if(j != 4){
			result+=",";
		}
	}

	int tempSelected = isWanWeiSelected + isQianWeiSelected + isBaiWeiSelected + isShiWeiSelected + isGeWeiSelected;	

	if(ballNum>=2 && tempSelected >= 2){
		int nBei = combo(tempSelected, 2);
		betZhuShu *=combo(ballNum, 2) * nBei;
	}else{	
		result = "";
		betZhuShu = 0;
	}

	if(result!=""){
		String* tempStr = String::createWithFormat("%d%d%d%d%d",isWanWeiSelected ,isQianWeiSelected ,isBaiWeiSelected , isShiWeiSelected , isGeWeiSelected);
		result.append(tempStr->getCString());
	}
	String *resultStr = String::createWithFormat("%s", result.c_str());
	return resultStr;
}

void ChongQingWuFen::updateRenXuanHeadTips()
{
	LabelTTF* ltfHelp = (LabelTTF *)m_node->getChildByTag(8527);
	if(ltfHelp == nullptr) return;

	int count = isWanWeiSelected + isQianWeiSelected + isBaiWeiSelected + isShiWeiSelected + isGeWeiSelected;
	int beishu = combo(count, m_num);

	String* strHelp;

	if (beishu == 0)
	{
		strHelp = String::create(ConfigMgr::instance()->text("display_text.xml", "t4633"));
	} else
	{
		strHelp = String::createWithFormat("%s%d%s%d%s", 
			ConfigMgr::instance()->text("display_text.xml", "t4630"),
			count,
			ConfigMgr::instance()->text("display_text.xml", "t4631"),
			beishu,
			ConfigMgr::instance()->text("display_text.xml", "t4632")
			);
	}


	ltfHelp->setString(strHelp->getCString());
}

void ChongQingWuFen::countTouZhuParams()
{
	if( m_betKind == RenXuan4_DanShi ||  //比较特殊的，任选单式 直选
		m_betKind ==  RenXuan3_DanShi ||
		m_betKind == RenXuan2_DanShi
		)
	{
		//字符串
		String* tempStr = String::createWithFormat("%d%d%d%d%d",isWanWeiSelected ,isQianWeiSelected ,isBaiWeiSelected , isShiWeiSelected , isGeWeiSelected);
		int nBei = combo(isWanWeiSelected + isQianWeiSelected + isBaiWeiSelected +  isShiWeiSelected + isGeWeiSelected, m_num);	
		//单式字符串
		m_betString = BetLimit::GetRenXuanDanShiStr(m_inputString.c_str(),m_num,m_repeat,9,tempStr->getCString(),nBei,true);
		//计算注数
		betZhuShu = m_betString.size() / (m_num + 5);
		//注数限制
		if(BetLimit::GetMaxBetFromId(1,m_betKind)*nBei < betZhuShu)
		{
			betZhuShu = 0;
		}
	} else
		if(	m_betKind == Ren2Zuxuan_Danshi ||//比较特殊的，任选单式 组选		
			m_betKind == Ren3Zu3_DanShi ||
			m_betKind ==  Ren3Zu6_DanShi ||
			m_betKind == Ren3Hunhe_Zuxuan)
		{
			//字符串
			String* tempStr = String::createWithFormat("%d%d%d%d%d",isWanWeiSelected ,isQianWeiSelected ,isBaiWeiSelected , isShiWeiSelected , isGeWeiSelected);
			int nBei = combo(isWanWeiSelected + isQianWeiSelected + isBaiWeiSelected +  isShiWeiSelected + isGeWeiSelected, m_num);		
			//单式字符串
			m_betString = BetLimit::GetRenXuanDanShiStr(m_inputString.c_str(),m_num,m_repeat,9,tempStr->getCString(),nBei,false);
			//计算注数
			betZhuShu = m_betString.size() / (m_num + tempStr->length() + 2) * nBei;
			//注数限制
			if(m_betKind == Ren3Hunhe_Zuxuan)
			{
				if(180 *nBei < betZhuShu)
				{
					betZhuShu = 0;
				}
			}

		} else //其它单式
		{
			//计算单式
			m_betString = BetLimit::GetDanShiStr(m_inputString.c_str(), m_num,m_repeat,9);
			//计算注数
			betZhuShu = m_betString.size() / m_num / 2;	
			if(m_num == 5) betZhuShu = m_betString.size() / (m_num + 1);
		}
}