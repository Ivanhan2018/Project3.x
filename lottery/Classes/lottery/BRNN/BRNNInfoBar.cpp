#include "BRNNInfoBar.h"
#include "BRNNDefine.h"
#include "EntityMgr.h"
#include "ToolKit.h"
#include "MyNSString.h"
#include "MyConfig.h"
#include "BRNNRes.h"

BRNNInfoBar::BRNNInfoBar()
{
}

BRNNInfoBar::~BRNNInfoBar()
{
}

void BRNNInfoBar::resetInfoBar()
{
	updatePlayerBetSum(0);
}

void BRNNInfoBar::updateBankerInfo(WORD wBankerUser, TCHART szBankerNickName[], BYTE cbBankerGender, SCORE lBankerScore, SCORE lBankerWinScore, WORD cbBankerTime, bool bSystemBanker)
{	
	string sBankerName = "";
	string sBankerFace;
	if (wBankerUser == INVALID_CHAIR)
	{
		sBankerName = m_sNoneBankerName;
		lBankerScore = 1000000;
		lBankerWinScore = 0;
		sBankerFace = BRNNTextureName::s_brnn_face_null;
	}
	else
	{
		char cNikeName[256]={0};
		MyNSString::wc2mbs(szBankerNickName, 32, cNikeName);
		sBankerName = cNikeName;

 		tagUserData* pSelfData = EntityMgr::instance()->roomFrame()->getMeUserDate();
 		if (wBankerUser != pSelfData->wChairID)
 		{
 			sBankerName = ToolKit::dealStringOfPrivacy(sBankerName);
 		}

		if (cbBankerGender == 1)	//	男
		{
			sBankerFace = BRNNTextureName::s_brnn_face_man;
		}
		else
		{
			sBankerFace = BRNNTextureName::s_brnn_face_woman;
		}
	}

	//	庄家昵称
	m_pBankerName->setString(sBankerName);

	//	庄家头像
	m_pBankerFace->initWithSpriteFrameName(sBankerFace);

	//	庄家财富
	updateBankerGold(lBankerScore);

	//	庄家成绩
	updateBankerTotal(lBankerWinScore);

	//	庄家连庄
	updateBankerTime(cbBankerTime);
}

void BRNNInfoBar::updatePlayerGold(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pPlayerGold->setString(cBuf);
	setPlayerGold(sValue);
}

void BRNNInfoBar::updatePlayerTotal(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pPlayerTotal->setString(cBuf);
	setPlayerTotal(sValue);
}

void BRNNInfoBar::updatePlayerBetSum(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pPlayerBetSum->setString(cBuf);
	setPlayerBetSum(sValue);
}

//	更新庄家金币
void BRNNInfoBar::updateBankerGold(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pBankerGold->setString(cBuf);
}

//	更新庄家成绩
void BRNNInfoBar::updateBankerTotal(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pBankerTotal->setString(cBuf);
}

//	更新庄家连庄数
void BRNNInfoBar::updateBankerTime(WORD wValue)
{
	char cBuf[128];
	sprintf(cBuf, "%d", wValue);
	m_pBankerTime->setString(cBuf);
}

bool BRNNInfoBar::init()
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

void BRNNInfoBar::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	
	int nFontSize = 20;

	m_pPlayerFace = Sprite::create();
	addChild(m_pPlayerFace);
	m_pPlayerFace->setPosition(Vec2(sVSize.width * 0.042745, sVSize.height * 0.089217));

	m_pPlayerName = Label::create();
	addChild(m_pPlayerName);
	m_pPlayerName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pPlayerName->setPosition(Vec2(sVSize.width * 0.12, sVSize.height * 0.138));
	m_pPlayerName->setSystemFontSize(nFontSize);
	m_pPlayerName->setColor(Color3B::WHITE);

	m_pPlayerGold = Label::create();
	addChild(m_pPlayerGold);
	m_pPlayerGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pPlayerGold->setPosition(Vec2(sVSize.width * 0.12, sVSize.height * 0.103));
	m_pPlayerGold->setSystemFontSize(nFontSize);
	m_pPlayerGold->setColor(Color3B::WHITE);

	m_pPlayerTotal = Label::create();
	addChild(m_pPlayerTotal);
	m_pPlayerTotal->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pPlayerTotal->setPosition(Vec2(sVSize.width * 0.12, sVSize.height * 0.07));
	m_pPlayerTotal->setSystemFontSize(nFontSize);
	m_pPlayerTotal->setColor(Color3B::WHITE);

	m_pPlayerBetSum = Label::create();
	addChild(m_pPlayerBetSum);
	m_pPlayerBetSum->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pPlayerBetSum->setPosition(Vec2(sVSize.width * 0.12, sVSize.height * 0.037));
	m_pPlayerBetSum->setSystemFontSize(nFontSize);
	m_pPlayerBetSum->setColor(Color3B::WHITE);

	m_pBankerFace = Sprite::create();
	addChild(m_pBankerFace);
	m_pBankerFace->setPosition(Vec2(sVSize.width * 0.043721, sVSize.height * 0.917716));

	m_pBankerName = Label::create();
	addChild(m_pBankerName);
	m_pBankerName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pBankerName->setPosition(Vec2(sVSize.width * 0.12, sVSize.height * 0.965));
	m_pBankerName->setSystemFontSize(nFontSize);
	m_pBankerName->setColor(Color3B::WHITE);

	m_pBankerGold = Label::create();
	addChild(m_pBankerGold);
	m_pBankerGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pBankerGold->setPosition(Vec2(sVSize.width * 0.12, sVSize.height * 0.93));
	m_pBankerGold->setSystemFontSize(nFontSize);
	m_pBankerGold->setColor(Color3B::WHITE);

	m_pBankerTotal = Label::create();
	addChild(m_pBankerTotal);
	m_pBankerTotal->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pBankerTotal->setPosition(Vec2(sVSize.width * 0.12, sVSize.height * 0.899));
	m_pBankerTotal->setSystemFontSize(nFontSize);
	m_pBankerTotal->setColor(Color3B::WHITE);

	m_pBankerTime = Label::create();
	addChild(m_pBankerTime);
	m_pBankerTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pBankerTime->setPosition(Vec2(sVSize.width * 0.12, sVSize.height * 0.865));
	m_pBankerTime->setSystemFontSize(nFontSize);
	m_pBankerTime->setColor(Color3B::WHITE);
}

void BRNNInfoBar::initData()
{
	updatePlayerGold(0);
	updatePlayerTotal(0);
	updatePlayerBetSum(0);
	updateBankerTime(0);
	updateBankerTotal(0);
	updateBankerGold(0);

	Dictionary * pDictionary = Dictionary::createWithContentsOfFile(BRNN_XML_FILE_NAME);
	m_sNoneBankerName = ((CCString*)pDictionary->objectForKey(BRNN_XML_KEY_SYSTEM_BANKER))->getCString();
}
