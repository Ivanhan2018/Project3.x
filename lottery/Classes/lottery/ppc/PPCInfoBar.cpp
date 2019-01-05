#include "PPCInfoBar.h"
#include "PPCRes.h"
#include "EntityMgr.h"
#include "ToolKit.h"
#include "PPCTool.h"
#include "MyNSString.h"
#include "MyConfig.h"//by hxh

PPCInfoBar::PPCInfoBar()
{
}

PPCInfoBar::~PPCInfoBar()
{
}

void PPCInfoBar::resetPPCInfoBar()
{
	updatePlayerBetSum(0);
}

void PPCInfoBar::updateBankerInfo(WORD wBankerUser, TCHART szBankerNickName[], SCORE lBankerScore,SCORE lBankerWinScore, WORD cbBankerTime, bool bSystemBanker)
{
	string sBankerName = "";
	if (wBankerUser == INVALID_CHAIR)
	{
		Dictionary * pDictionary = Dictionary::createWithContentsOfFile(PPC_XML_FILE_NAME);
		string sText = ((CCString*)pDictionary->objectForKey(PPC_XML_KEY_SYSTEM_BANKER))->getCString();
		sBankerName = sText;
		lBankerScore = 100000000;
		lBankerWinScore = 0;
	}
	else
	{
		char cNikeName[256] = {0};
		MyNSString::wc2mbs(szBankerNickName, 32, cNikeName);
		sBankerName = cNikeName;

		tagUserData* pSelfData = EntityMgr::instance()->roomFrame()->getMeUserDate();
		if (wBankerUser != pSelfData->wChairID)
		{
			sBankerName = ToolKit::dealStringOfPrivacy(sBankerName);
		}
	}

	//	庄家昵称
	m_pBankerName->setString(sBankerName);
	//	庄家财富
	updateBankerGold(lBankerScore);
	//	庄家成绩
	updateBankerTotal(lBankerWinScore);
	//	庄家连庄
	updateBankerTime(cbBankerTime);
}

void PPCInfoBar::updatePlayerGold(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pPlayerGold->setString(cBuf);
	setPlayerGold(sValue);
}

void PPCInfoBar::updatePlayerTotal(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pPlayerTotal->setString(cBuf);
	setPlayerTotal(sValue);
}

void PPCInfoBar::updatePlayerBetSum(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pPlayerBetSum->setString(cBuf);
	setPlayerBetSum(sValue);
}

//	更新庄家金币
void PPCInfoBar::updateBankerGold(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pBankerGold->setString(cBuf);
}

//	更新庄家成绩
void PPCInfoBar::updateBankerTotal(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pBankerTotal->setString(cBuf);
}

//	更新庄家连庄数
void PPCInfoBar::updateBankerTime(WORD wValue)
{
	char cBuf[128];
	sprintf(cBuf, "%d", wValue);
	m_pBankerTime->setString(cBuf);
}

bool PPCInfoBar::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		initUI();
		initData();

		bRet = true;
	} while (0);

	return bRet;
}

void PPCInfoBar::initUI()
{
	MyConfig& myConfig=MyConfig::Instance();
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	
	int nFontSize = 20;

	m_pFace = Sprite::create();
	addChild(m_pFace);
	m_pFace->setPosition(Vec2(sVSize.width * myConfig.m_FacePos[0], sVSize.height * myConfig.m_FacePos[1]));

	m_pPlayerName = Label::create();
	addChild(m_pPlayerName);
	m_pPlayerName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pPlayerName->setPosition(Vec2(sVSize.width * 0.149+myConfig.m_InfoBarOffset[0][0], sVSize.height * 0.145+myConfig.m_InfoBarOffset[0][1]));
	m_pPlayerName->setSystemFontSize(nFontSize);
	m_pPlayerName->setColor(Color3B::WHITE);

	m_pPlayerGold = Label::create();
	addChild(m_pPlayerGold);
	m_pPlayerGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pPlayerGold->setPosition(Vec2(sVSize.width * 0.149+myConfig.m_InfoBarOffset[0][0], sVSize.height * 0.105+myConfig.m_InfoBarOffset[0][1]));
	m_pPlayerGold->setSystemFontSize(nFontSize);
	m_pPlayerGold->setColor(Color3B::WHITE);

	m_pPlayerTotal = Label::create();
	addChild(m_pPlayerTotal);
	m_pPlayerTotal->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pPlayerTotal->setPosition(Vec2(sVSize.width * 0.149+myConfig.m_InfoBarOffset[0][0], sVSize.height * 0.065+myConfig.m_InfoBarOffset[0][1]));
	m_pPlayerTotal->setSystemFontSize(nFontSize);
	m_pPlayerTotal->setColor(Color3B::WHITE);

	m_pPlayerBetSum = Label::create();
	addChild(m_pPlayerBetSum);
	m_pPlayerBetSum->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pPlayerBetSum->setPosition(Vec2(sVSize.width * 0.149+myConfig.m_InfoBarOffset[0][0], sVSize.height * 0.025+myConfig.m_InfoBarOffset[0][1]));
	m_pPlayerBetSum->setSystemFontSize(nFontSize);
	m_pPlayerBetSum->setColor(Color3B::WHITE);


	m_pBankerName = Label::create();
	addChild(m_pBankerName);
	m_pBankerName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pBankerName->setPosition(Vec2(sVSize.width * 0.81+myConfig.m_InfoBarOffset[1][0], sVSize.height * 0.14+myConfig.m_InfoBarOffset[1][1]));
	m_pBankerName->setSystemFontSize(nFontSize);
	m_pBankerName->setColor(Color3B::WHITE);

	m_pBankerGold = Label::create();
	addChild(m_pBankerGold);
	m_pBankerGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pBankerGold->setPosition(Vec2(sVSize.width * 0.81+myConfig.m_InfoBarOffset[1][0], sVSize.height * 0.101+myConfig.m_InfoBarOffset[1][1]));
	m_pBankerGold->setSystemFontSize(nFontSize);
	m_pBankerGold->setColor(Color3B::WHITE);

	m_pBankerTotal = Label::create();
	addChild(m_pBankerTotal);
	m_pBankerTotal->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pBankerTotal->setPosition(Vec2(sVSize.width * 0.81+myConfig.m_InfoBarOffset[1][0], sVSize.height * 0.062+myConfig.m_InfoBarOffset[1][1]));
	m_pBankerTotal->setSystemFontSize(nFontSize);
	m_pBankerTotal->setColor(Color3B::WHITE);

	m_pBankerTime = Label::create();
	addChild(m_pBankerTime);
	m_pBankerTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pBankerTime->setPosition(Vec2(sVSize.width * 0.81+myConfig.m_InfoBarOffset[1][0], sVSize.height * 0.023+myConfig.m_InfoBarOffset[1][1]));
	m_pBankerTime->setSystemFontSize(nFontSize);
	m_pBankerTime->setColor(Color3B::WHITE);
}

void PPCInfoBar::initData()
{
	m_pBankerTotal->setString("0");
	m_pBankerTime->setString("0");
	updatePlayerGold(0);
	updatePlayerTotal(0);
	updatePlayerBetSum(0);
}
