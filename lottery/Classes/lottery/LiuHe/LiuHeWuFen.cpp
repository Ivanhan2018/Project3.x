#include "HelpLayer.h"
#include "RoomLayer.h"
#include "LiuHeWuFen.h"
#include "BetLimit.h"
#include "MovingLabelLayer.h"
#include "comm.h"
#include "xDate.h"

using namespace cocos2d;

LiuHeWuFen::LiuHeWuFen()
{

}

LiuHeWuFen::~LiuHeWuFen()
{
	m_vecPot.clear();
}

Scene* LiuHeWuFen::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		LiuHeWuFen *layer = LiuHeWuFen::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool LiuHeWuFen::init()
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
		listener->onTouchBegan = CC_CALLBACK_2(LiuHeWuFen::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(LiuHeWuFen::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(LiuHeWuFen::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(LiuHeWuFen::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		m_node = Node::create();
		m_node->setPosition(Vec2(0,0));
		this->addChild(m_node,2);

		fontSize = 27;
		pt = Vec2(240, SCREEN_HEIGHT - 162 - 70);	

		memset(ballStatus,0,sizeof(ballStatus));		

		bRet = true;
	} while (0);

	return bRet;
}

void LiuHeWuFen::initBetArea()
{
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//gamble_02_3.png
	float posXA1 = 20 + ballRadius / 2;
	float deltaA1 = 225;	
	float deltaA2 = 105;
	float posXA2 = 34 + ballRadius + 8;
	int fontSize = 27;
	int backHeight = 80;

	//顶部背景
	Sprite* sptBackground = Sprite::createWithSpriteFrame(spriteFrame("blank.png"));
	sptBackground->setPosition(Vec2(SCREEN_WIDTH / 2, pt.y));
	sptBackground->setScaleX(SCREEN_WIDTH / sptBackground->getContentSize().width);
	sptBackground->setColor(ccc3(225,225,225));
	sptBackground->setScaleY(backHeight / sptBackground->getContentSize().height);  //高度;
	m_node->addChild(sptBackground, 0);

	//初始化头
	for(int j = 0;j < 3;j ++)
	{
		//显示号码字样
		LabelTTF* ltfHaoMa = LabelTTF::create();
		ltfHaoMa->setPosition(Vec2(posXA1 + (j % 3) * deltaA1, pt.y));
		ltfHaoMa->setFontName("");
		ltfHaoMa->setFontSize(fontSize);
		ltfHaoMa->setFontFillColor(ccColor3B::BLACK);		
		ltfHaoMa->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t18"));
		m_node->addChild(ltfHaoMa);

		//显示金额字样
		LabelTTF* lftMoney = LabelTTF::create();
		lftMoney->setPosition(Vec2(posXA2 + (j % 3) * deltaA1 + deltaA2  / 2, pt.y));
		lftMoney->setFontName("");
		lftMoney->setFontSize(fontSize);
		lftMoney->setFontFillColor(ccColor3B::BLACK);			
		lftMoney->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t19"));		
		m_node->addChild(lftMoney);
	}
	pt.y -= backHeight;
	//初始化投注号码表
	Texture2D* tdBall = spriteFrame("gamble_02_3.png")->getTexture();
	//投注本体
	for(int i = 0;i < 49;i ++)
	{
		float myDeltaX = i % 3;
		int myDeltaY = i / 3;

		//球显示
		Sprite* ballSprite = Sprite::createWithTexture(tdBall,cocos2d::Rect((i % 10)*ballRadius, (i / 10)* ballRadius, ballRadius, ballRadius));
		ballSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		ballSprite->setPosition(Vec2(posXA1 + myDeltaX * deltaA1, pt.y - (ballRadius + 4) * myDeltaY));
		m_node->addChild(ballSprite, 1);

		//金额显示  这个背景可以直接加在editbox里面
		//Sprite* sptBack = Sprite::createWithSpriteFrame(spriteFrame("setting_08.png"));
		//sptBack->setPosition(Vec2(posXA2 + myDeltaX * deltaA1, ballRadius * myDeltaY));
		//m_node->addChild(sptBack, 2);

		//金额输入框
		EditBox* dtxInput = EditBox::create(cocos2d::Size(deltaA2,60),"setting_08.png");
		dtxInput->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		dtxInput->setPosition(Vec2(posXA2 + myDeltaX * deltaA1, pt.y - (ballRadius + 4) * myDeltaY));
		dtxInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		dtxInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		dtxInput->setInputMode(ui::EditBox::InputMode::NUMERIC);
		dtxInput->setAnchorPoint(Vec2(0,0.5f));
		dtxInput->setFontColor(ccColor3B::BLACK);
		dtxInput->setFontName("");	dtxInput->setFontSize(fontSize);		
		dtxInput->setText("");
		dtxInput->setMaxLength(10);
		dtxInput->setDelegate(this);
		dtxInput->setTag(TAG_EDITBOX + i);
		m_node->addChild(dtxInput, 2);
	}	

	String* strPeiLv = String::create(ConfigMgr::instance()->text("liuhecai_config.xml", "t40"));
	//更新主界面赔率
	NotificationCenter::getInstance()->postNotification("UpdatePeiLv", strPeiLv);

	pt.y -= 1600;
	if(pt.y>0)
	{
		m_endPos = Vec2(pt.x,0);
	}else{
		m_endPos=Vec2(pt.x,-pt.y);
	}
}

void LiuHeWuFen::initDXDSBetArea()
{
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//gamble_02_3.png
	float posXA1 = 34 + ballRadius / 2;
	float deltaA1 = 100;	
	float deltaA2 = 105;	
	int fontSize = 27;
	int backHeight = 80;

	//顶部背景
	Sprite* sptBackground = Sprite::createWithSpriteFrame(spriteFrame("blank.png"));
	sptBackground->setPosition(Vec2(SCREEN_WIDTH / 2, pt.y));
	sptBackground->setScaleX(SCREEN_WIDTH / sptBackground->getContentSize().width);
	sptBackground->setColor(ccc3(225,225,225));
	sptBackground->setScaleY(backHeight / sptBackground->getContentSize().height);  //高度;
	m_node->addChild(sptBackground, 0);

	//显示类型
	LabelTTF* ltfHaoMa = LabelTTF::create();
	ltfHaoMa->setPosition(Vec2(posXA1, pt.y));
	ltfHaoMa->setFontName("");
	ltfHaoMa->setFontSize(fontSize);
	ltfHaoMa->setFontFillColor(ccColor3B::BLACK);
	ltfHaoMa->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t21"));
	m_node->addChild(ltfHaoMa, 1);

	//显示赔率
	LabelTTF* ltfPei = LabelTTF::create();
	ltfPei->setPosition(Vec2(posXA1 + deltaA1, pt.y));
	ltfPei->setFontName("");
	ltfPei->setFontSize(fontSize);
	ltfPei->setFontFillColor(ccColor3B::BLACK);
	ltfPei->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t20"));
	m_node->addChild(ltfPei, 1);

	//显示金额字样
	LabelTTF* lftMoney = LabelTTF::create();
	lftMoney->setPosition(Vec2(posXA1 + 2 * deltaA1 + deltaA2  / 2, pt.y));
	lftMoney->setFontName("");
	lftMoney->setFontSize(fontSize);
	lftMoney->setFontFillColor(ccColor3B::BLACK);
	lftMoney->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t19"));
	m_node->addChild(lftMoney, 1);
	
	pt.y -= backHeight;
	//投注本体
	for(int i = 0;i < 4;i ++)
	{
		//玩法显示
		Sprite* sptWanfa = Sprite::createWithSpriteFrame(spriteFrame("setting_09.png"));
		sptWanfa->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptWanfa->setPosition(Vec2(posXA1, pt.y - (ballRadius + 4) * i));
		m_node->addChild(sptWanfa, 1);

		LabelTTF* ltfWanfa = LabelTTF::create();
		ltfWanfa->setPosition(Vec2(posXA1, pt.y - (ballRadius + 4) * i));
		ltfWanfa->setFontName("");
		ltfWanfa->setFontSize(fontSize);
		ltfWanfa->setFontFillColor(ccColor3B::BLACK);
		ltfWanfa->setString(ConfigMgr::instance()->text("display_text.xml",String::createWithFormat("t%d",4408 + i)->getCString()));
		m_node->addChild(ltfWanfa,2);

		LabelTTF* ltfPei = LabelTTF::create();
		ltfPei->setPosition(Vec2(posXA1 + deltaA1, pt.y - (ballRadius + 4) * i));
		ltfPei->setFontName("");
		ltfPei->setFontSize(fontSize);
		ltfPei->setFontFillColor(ccColor3B::BLACK);
		ltfPei->setString(ConfigMgr::instance()->text("liuhecai_config.xml", "t43"));
		m_node->addChild(ltfPei);

		//金额输入框
		EditBox* dtxInput = EditBox::create(cocos2d::Size(deltaA2,60),"setting_08.png");
		dtxInput->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		dtxInput->setPosition(Vec2(posXA1 + 2 * deltaA1, pt.y - (ballRadius + 4) * i));
		dtxInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		dtxInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		dtxInput->setInputMode(ui::EditBox::InputMode::NUMERIC);
		dtxInput->setAnchorPoint(Vec2(0,0.5f));
		dtxInput->setFontColor(ccColor3B::BLACK);
		dtxInput->setFontName(""); dtxInput->setFontSize(fontSize); 	
		dtxInput->setText("");
		dtxInput->setMaxLength(10);
		dtxInput->setDelegate(this);
		dtxInput->setTag(TAG_EDITBOX + i);
		m_node->addChild(dtxInput, 2);
	}	

	//更新赔率
	NotificationCenter::getInstance()->postNotification("UpdatePeiLv");

	pt.y -= 320;
	if(pt.y>0)
	{
		m_endPos = Vec2(pt.x,0);
	}else{
		m_endPos=Vec2(pt.x,-pt.y);
	}
}

void LiuHeWuFen::initSXBetArea()
{
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	//gamble_02_3.png
	float posXA1 = 20 + ballRadius / 2;
	float deltaA1 = 70;	
	float deltaA2 = 105;	
	int fontSize = 27;
	int backHeight = 80;

	//顶部背景
	Sprite* sptBackground = Sprite::createWithSpriteFrame(spriteFrame("blank.png"));
	sptBackground->setPosition(Vec2(SCREEN_WIDTH / 2, pt.y));
	sptBackground->setScaleX(SCREEN_WIDTH / sptBackground->getContentSize().width);
	sptBackground->setColor(ccc3(225,225,225));
	sptBackground->setScaleY(backHeight / sptBackground->getContentSize().height);  //高度;
	m_node->addChild(sptBackground, 0);

	//显示类型
	LabelTTF* ltfType = LabelTTF::create();
	ltfType->setPosition(Vec2(posXA1, pt.y));
	ltfType->setFontName("");
	ltfType->setFontSize(fontSize);
	ltfType->setFontFillColor(ccColor3B::BLACK);
	ltfType->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t22"));
	m_node->addChild(ltfType, 1);

	//显示赔率
	LabelTTF* ltfPei = LabelTTF::create();
	ltfPei->setPosition(Vec2(posXA1 + deltaA1, pt.y));
	ltfPei->setFontName("");
	ltfPei->setFontSize(fontSize);
	ltfPei->setFontFillColor(ccColor3B::BLACK);
	ltfPei->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t20"));
	m_node->addChild(ltfPei, 1);

	//显示金额字样
	LabelTTF* lftMoney = LabelTTF::create();
	lftMoney->setPosition(Vec2(posXA1 + 2 * deltaA1 + deltaA2  / 2, pt.y));
	lftMoney->setFontName("");
	lftMoney->setFontSize(fontSize);
	lftMoney->setFontFillColor(ccColor3B::BLACK);
	lftMoney->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t19"));
	m_node->addChild(lftMoney, 1);

	//显示号码字样
	LabelTTF* ltfHaoMa = LabelTTF::create();
	ltfHaoMa->setPosition(Vec2((SCREEN_WIDTH  + lftMoney->getPositionX()) / 2, pt.y));  // 
	ltfHaoMa->setFontName("");
	ltfHaoMa->setFontSize(fontSize);
	ltfHaoMa->setFontFillColor(ccColor3B::BLACK);
	ltfHaoMa->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t18"));
	m_node->addChild(ltfHaoMa, 1);

	pt.y -= backHeight;
	//初始化投注号码表
	Texture2D* tdBall = spriteFrame("gamble_02_3.png")->getTexture();	

	int nChunJie = 0;
	xDate	xD;
	int iLunarYear=0;
	int iLunarMon=0;
	int iLunarDay=0;
	time_t  ct_now;
	EntityMgr::instance()->getDispatch()->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);
	tm	tm_SpringFest;
	memset(&tm_SpringFest, 0, sizeof(tm_SpringFest));
	tm_SpringFest.tm_year = tmLocal->tm_year;
	tm_SpringFest.tm_mon = 0;
	tm_SpringFest.tm_mday = 1;
	//寻找农历新年
	time_t ct_fest_year = mktime(&tm_SpringFest);
	
	xD.GetLunarDate(1900+tmLocal->tm_year, tmLocal->tm_mon+1, tmLocal->tm_mday, iLunarYear, iLunarMon, iLunarDay);
	for(int n=1; n<100; n++)
	{
		xD.GetLunarDate(1900+tm_SpringFest.tm_year, tm_SpringFest.tm_mon+1, tm_SpringFest.tm_mday, iLunarYear, iLunarMon, iLunarDay);
		if(iLunarMon == 1 && iLunarDay == 1)
			break;
		ct_fest_year += 1440 * 60;						//oneday
		tm* pTimeNextDay = localtime(&ct_fest_year);
		tm_SpringFest = *pTimeNextDay;
	}
	if(ct_now > ct_fest_year)
		nChunJie = 1 + (iLunarYear - 2017);
	else
		nChunJie =  (iLunarYear - 2017);
	//投注本体
	for(int i = 0;i < 12;i ++)
	{
		int nowPos = i;
		for(int k=0; k<nChunJie; k++)
		{
			nowPos = (nowPos + 11) % 12;		//;(nowPos + 11) % 12;
		}
		//玩法显示
		Sprite* sptWanfa = Sprite::createWithSpriteFrame(spriteFrame("setting_09.png"));
		sptWanfa->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptWanfa->setPosition(Vec2(posXA1, pt.y - (ballRadius + 4) * i));
		m_node->addChild(sptWanfa, 1);

		LabelTTF* ltfWanfa = LabelTTF::create();
		ltfWanfa->setPosition(Vec2(posXA1, pt.y - (ballRadius + 4) * i));
		ltfWanfa->setFontName("");
		ltfWanfa->setFontSize(fontSize);
		ltfWanfa->setFontFillColor(ccColor3B::BLACK);
		ltfWanfa->setString(ConfigMgr::instance()->text("liuhecai_config.xml",String::createWithFormat("t%d",14 + i)->getCString()));
		m_node->addChild(ltfWanfa,2);

		//赔率
		LabelTTF* ltfPei = LabelTTF::create();
		ltfPei->setPosition(Vec2(posXA1 + deltaA1, pt.y - (ballRadius + 4) * i));
		ltfPei->setFontName("");
		ltfPei->setFontSize(fontSize);
		ltfPei->setFontFillColor(ccColor3B::BLACK);
		ltfPei->setString(ConfigMgr::instance()->text("liuhecai_config.xml", String::createWithFormat("t%d",27 + nowPos)->getCString()));
		m_node->addChild(ltfPei);

		//金额输入框
		EditBox* dtxInput = EditBox::create(cocos2d::Size(deltaA2,60),"setting_08.png");
		dtxInput->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		dtxInput->setPosition(Vec2(posXA1 + 2 * deltaA1, pt.y - (ballRadius + 4) * i));
		dtxInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		dtxInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		dtxInput->setInputMode(ui::EditBox::InputMode::NUMERIC);
		dtxInput->setAnchorPoint(Vec2(0,0.5f));
		dtxInput->setFontColor(ccColor3B::BLACK);
		dtxInput->setFontName(""); dtxInput->setFontSize(fontSize); 	
		dtxInput->setText("");
		dtxInput->setMaxLength(10);
		dtxInput->setDelegate(this);
		dtxInput->setTag(TAG_EDITBOX + i);
		m_node->addChild(dtxInput, 2);

		//该下面的球
		const char* cStr = ConfigMgr::instance()->text("liuhecai_config.xml",String::createWithFormat("t%d",1 + nowPos)->getCString());		
		int iJiZhi = atoi(cStr);
		int posCount = 2;
		for(int num = iJiZhi - 1;num < 50;num +=12)
		{				
			//球显示
			Sprite* ballSprite = Sprite::createWithTexture(tdBall,cocos2d::Rect((num % 10)*ballRadius, (num / 10)* ballRadius, ballRadius, ballRadius));
			ballSprite->setPosition(Vec2(posXA1 + 2 * deltaA1 + posCount * ballRadius, pt.y - (ballRadius + 4) * i));
			m_node->addChild(ballSprite, 2);
			posCount ++;
		}
	}

	//更新赔率
	NotificationCenter::getInstance()->postNotification("UpdatePeiLv");

	pt.y -= 1200;
	if(pt.y>0)
	{
		m_endPos = Vec2(pt.x,0);
	}else{
		m_endPos=Vec2(pt.x,-pt.y);
	}
}

void LiuHeWuFen::initBSBetArea()
{
	pt.y = pt.y-areaGapY-areaBkHeight/2;

	float posXA1 = 34 + ballRadius / 2;
	float deltaA1 = 100;	
	float deltaA2 = 105;	
	int fontSize = 27;
	int backHeight = 80;

	//顶部背景
	Sprite* sptBackground = Sprite::createWithSpriteFrame(spriteFrame("blank.png"));
	sptBackground->setPosition(Vec2(SCREEN_WIDTH / 2, pt.y));
	sptBackground->setScaleX(SCREEN_WIDTH / sptBackground->getContentSize().width);
	sptBackground->setColor(ccc3(225,225,225));
	sptBackground->setScaleY(backHeight / sptBackground->getContentSize().height);  //高度;
	m_node->addChild(sptBackground, 0);

	//显示类型 波色
	LabelTTF* ltfHaoMa = LabelTTF::create();
	ltfHaoMa->setPosition(Vec2(posXA1, pt.y));
	ltfHaoMa->setFontName("");
	ltfHaoMa->setFontSize(fontSize);
	ltfHaoMa->setFontFillColor(ccColor3B::BLACK);
	ltfHaoMa->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t23"));
	m_node->addChild(ltfHaoMa, 1);

	//显示赔率
	LabelTTF* ltfPei = LabelTTF::create();
	ltfPei->setPosition(Vec2(posXA1 + deltaA1, pt.y));
	ltfPei->setFontName("");
	ltfPei->setFontSize(fontSize);
	ltfPei->setFontFillColor(ccColor3B::BLACK);
	ltfPei->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t20"));
	m_node->addChild(ltfPei, 1);

	//显示金额字样
	LabelTTF* lftMoney = LabelTTF::create();
	lftMoney->setPosition(Vec2(posXA1 + 2 * deltaA1 + deltaA2  / 2, pt.y));
	lftMoney->setFontName("");
	lftMoney->setFontSize(fontSize);
	lftMoney->setFontFillColor(ccColor3B::BLACK);
	lftMoney->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t19"));
	m_node->addChild(lftMoney, 1);

	pt.y -= backHeight;
	//初始化投注号码表
	Texture2D* tdBall = spriteFrame("gamble_02_3.png")->getTexture();	
	//投注本体
	for(int i = 0;i < 3;i ++)
	{
		//玩法显示
		Sprite* sptWanfa = Sprite::createWithSpriteFrame(spriteFrame("setting_09.png"));
		sptWanfa->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptWanfa->setPosition(Vec2(posXA1, pt.y));
		m_node->addChild(sptWanfa, 1);

		LabelTTF* ltfWanfa = LabelTTF::create();
		ltfWanfa->setPosition(Vec2(posXA1, pt.y));
		ltfWanfa->setFontName("");
		ltfWanfa->setFontSize(fontSize);
		ltfWanfa->setFontFillColor(ccc3(i == 0 ? 255 :0,i == 2 ? 255 : 0, i == 1 ? 255 : 0));
		ltfWanfa->setString(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d",30 + i)->getCString()));
		m_node->addChild(ltfWanfa,2);

		//波色赔率
		LabelTTF* ltfPei = LabelTTF::create();  //
		ltfPei->setPosition(Vec2(posXA1 + deltaA1, pt.y));
		ltfPei->setFontName("");
		ltfPei->setFontSize(fontSize);
		ltfPei->setFontFillColor(ccColor3B::BLACK);
		ltfPei->setString(ConfigMgr::instance()->text("liuhecai_config.xml", String::createWithFormat("t%d",44 + i)->getCString()));
		m_node->addChild(ltfPei);

		//金额输入框
		EditBox* dtxInput = EditBox::create(cocos2d::Size(deltaA2,60),"setting_08.png");
		dtxInput->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		dtxInput->setPosition(Vec2(posXA1 + 2 * deltaA1, pt.y));
		dtxInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		dtxInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		dtxInput->setInputMode(ui::EditBox::InputMode::NUMERIC);
		dtxInput->setAnchorPoint(Vec2(0,0.5f));
		dtxInput->setFontColor(ccColor3B::BLACK);
		dtxInput->setFontName(""); dtxInput->setFontSize(fontSize); 	
		dtxInput->setText("");
		dtxInput->setMaxLength(10);
		dtxInput->setDelegate(this);
		dtxInput->setTag(TAG_EDITBOX + i);
		m_node->addChild(dtxInput, 2);

		//内循环获得纵坐标
		const char* cStr = ConfigMgr::instance()->text("liuhecai_config.xml",String::createWithFormat("t%d",50 + i * 20)->getCString());
		int numCount = atoi(cStr);
		for(int j = 0;j < numCount;j ++)
		{
			const char* tempChar = ConfigMgr::instance()->text("liuhecai_config.xml",String::createWithFormat("t%d",51 + i * 20 + j)->getCString());
			int num = atoi(tempChar) - 1;
			if(j % 8 == 0)
			{
				pt.y -= 82; //换行
			}
			//球
			Sprite* ballSprite = Sprite::createWithTexture(tdBall,cocos2d::Rect((num % 10)*ballRadius, (num / 10)* ballRadius, ballRadius, ballRadius));
			ballSprite->setPosition(Vec2(posXA1 + (j % 8) * (ballRadius + 4), pt.y));
			m_node->addChild(ballSprite, 2);
		}

		pt.y -= 70;
		//底下的线
		Sprite* sptLine = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
		sptLine->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptLine->setPosition(Vec2(SCREEN_WIDTH / 2, pt.y));
		sptLine->setScaleX(SCREEN_WIDTH / sptLine->getContentSize().width);
		m_node->addChild(sptLine, 1);

		pt.y -= 40;
	}

	//更新赔率
	NotificationCenter::getInstance()->postNotification("UpdatePeiLv");

	pt.y -= 200;
	if(pt.y>0)
	{
		m_endPos = Vec2(pt.x,0);
	}else{
		m_endPos=Vec2(pt.x,-pt.y);
	}
}

void LiuHeWuFen::setBetKind(LiuHeCaiKind kind)
{
	pt = Vec2(240, SCREEN_HEIGHT - 310);//280
	m_betKind = kind;
	switch (m_betKind)
	{
	case LiuHeCai_Zm1t:
	case LiuHeCai_Zm2t:
	case LiuHeCai_Zm3t:
	case LiuHeCai_Zm4t:
	case LiuHeCai_Zm5t:
	case LiuHeCai_Zm6t:
		initBetArea();
		break;
	case LiuHeCai_Tmdm: //特码单买
		initBetArea();
		break;
	case LiuHeCai_Tmds:	//特码单双
	case LiuHeCai_Tmdx:	//特码大小	
		initDXDSBetArea();	
		break;
	case LiuHeCai_Tmsx:	//特码生肖
		initSXBetArea();
		break;
	case LiuHeCai_Tmbs:	//特码波色
		initBSBetArea();
		break;
	case LiuHeCai_Pmdm:	//平码单买
		initBetArea();
		break;	
	default:	
		break;
	}
}

void LiuHeWuFen::onEnter()
{
	Layer::onEnter();	
}

void LiuHeWuFen::onExit()
{
	//
	Layer::onExit();
}

bool LiuHeWuFen::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (pTouch->getLocation().y < 280 || pTouch->getLocation().y > 1080)  //1280-162-30
	{		
		return false;
	}
		
	//清空数组
	m_vecPot.clear();
	m_bDrag = false;

	return true;
}
void LiuHeWuFen::onTouchMoved(Touch *pTouch, Event *pEvent)
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
void LiuHeWuFen::onTouchEnded(Touch *pTouch, Event *pEvent)
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
}

//手指触摸取消
void LiuHeWuFen::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

void LiuHeWuFen::editBoxEditingDidEnd(EditBox* editBox)
{
	//当前是哪个box
	int tagBox = editBox->getTag();
	if(editBox->getText() == nullptr)
	{
		ballStatus[tagBox - TAG_EDITBOX] = 0;
	} else
	{
		ballStatus[tagBox - TAG_EDITBOX] = atoi(editBox->getText());
	}

	NotificationCenter::getInstance()->postNotification("UpdateTouZhu");
}

void LiuHeWuFen::getBetNumber()
{
	betZhuShu = 0;  //投注个数
	betBeiShu = 0;  //投注倍数

	for(int i = 0;i < 50;i ++)
	{
		if(ballStatus[i])
		{		
			betZhuShu ++;
			betBeiShu += ballStatus[i];								
		}
	}

	return;
}

String* LiuHeWuFen::getBetNumberAt(const int index)
{
	if(ballStatus[index] == 0) return NULL;

	int areaDXDS[4] = {9, 1, 3, 2};	

	switch (m_betKind)
	{
	case LiuHeCai_Zm1t:
	case LiuHeCai_Zm2t:
	case LiuHeCai_Zm3t:
	case LiuHeCai_Zm4t:
	case LiuHeCai_Zm5t:
	case LiuHeCai_Zm6t:
	case LiuHeCai_Tmdm: //特码单买
	case LiuHeCai_Pmdm:	//平码单买
		return String::createWithFormat("%02d|", index + 1);
		break;
	case LiuHeCai_Tmds:	//特码单双
	case LiuHeCai_Tmdx:	//特码大小	
	
		return String::createWithFormat("%d|",areaDXDS[index]);
		break;
	case LiuHeCai_Tmsx:	//特码生肖			
		return String::createWithFormat("%02d|",atoi(ConfigMgr::instance()->text("liuhecai_config.xml",String::createWithFormat("t%d",1 + ((index + 11) % 12))->getCString())));
		break;
	case LiuHeCai_Tmbs:	//特码波色
		return String::createWithFormat("%02d|", index + 1);
		break;
	default:	
		break;
	}
}

int LiuHeWuFen::getBetBeiShuAt(const int index)
{
	return ballStatus[index];
}

void LiuHeWuFen::clearNumbers()
{
	for(int i = 0;i < 50;i ++)
	{
		//球的状态
		ballStatus[i] = 0;
		EditBox* dtxInput = (EditBox *)m_node->getChildByTag(TAG_EDITBOX + i);
		if(dtxInput != nullptr)
		{
			dtxInput->setText("");	
		}
	}	

	//NotificationCenter::getInstance()->postNotification("UpdateTouZhu");
}