#include "SGLYInfoCenter.h"
#include "SGLYGameScene.h"
#include "ToolKit.h"
#include "MyNSString.h"
#include "EntityMgr.h"
#include "SGLYTool.h"

SGLYInfoCenter::SGLYInfoCenter()
{
}

SGLYInfoCenter::~SGLYInfoCenter()
{
}

void SGLYInfoCenter::reset()
{
	for (int i = E_SGLY_Lottery_Bar; i < E_SGLY_Lottery_Null; i++)
	{
		updateSelfBet((E_SGLY_Lottery)i, 0);
		updateTotalBet((E_SGLY_Lottery)i, 0);
	}
}

void SGLYInfoCenter::updateBankerInfo(WORD wBankerUser, TCHART szBankerNickName[], LONGLONG lBankerChip, LONGLONG lBankerWinScore, WORD cbBankerTime, bool bSystemBanker)
{
	string sBankerName = "";
	if (wBankerUser == INVALID_CHAIR)
	{
		sBankerName = m_sNoneBankerName;
		lBankerChip = 10000;
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

	lBankerChip = SGLYTool::exchangeForChip(lBankerChip);
	lBankerWinScore = SGLYTool::exchangeForChip(lBankerWinScore);

	//	庄家昵称
	m_pLabBankerNickname->setString(sBankerName);

	//	庄家财富
	updateInfoBar(E_SGLY_InfoBar_BankerChip, lBankerChip);

	//	庄家成绩
	updateInfoBar(E_SGLY_InfoBar_BankerWinScore, lBankerWinScore);

	//	庄家连庄
	updateInfoBar(E_SGLY_InfoBar_BankerTime, cbBankerTime);
}

//	更新个注
void SGLYInfoCenter::updateSelfBet(E_SGLY_Lottery eType, const LONGLONG sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%lld", sValue);
	m_pLabSelfBet[eType]->setString(cBuf);
	setValueSelfBet(eType, sValue);
}

//	更新总注
void SGLYInfoCenter::updateTotalBet(E_SGLY_Lottery eType, const LONGLONG sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%lld", sValue);
	m_pLabTotalBet[eType]->setString(cBuf);
	setValueTotalBet(eType, sValue);
}

//	更新信息栏
void SGLYInfoCenter::updateInfoBar(E_SGLY_InfoBar eType, const LONGLONG sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%lld", sValue);
	m_pLabInfoBar[eType]->setString(cBuf);
	setValueInfoBar(eType, sValue);
}

//	设置信息栏数值
void SGLYInfoCenter::setValueInfoBar(E_SGLY_InfoBar eType, const LONGLONG sValue)
{
	m_sValueInfoBar[eType] = sValue;
}

//	获得信息栏数值
LONGLONG SGLYInfoCenter::getValueInfoBar(E_SGLY_InfoBar eType) const
{
	return m_sValueInfoBar[eType];
}

//	设置个注数值
void SGLYInfoCenter::setValueSelfBet(E_SGLY_Lottery eType, const LONGLONG sValue)
{
	m_sValueSelfBet[eType] = sValue;
}

//	获得个注数值
LONGLONG SGLYInfoCenter::getValueSelfBet(E_SGLY_Lottery eType) const
{
	return m_sValueSelfBet[eType];
}

//	设置总注数值
void SGLYInfoCenter::setValueTotalBet(E_SGLY_Lottery eType, const LONGLONG sValue)
{
	m_sValueTotalBet[eType] = sValue;
}

//	获得总注数值
LONGLONG SGLYInfoCenter::getValueTotalBet(E_SGLY_Lottery eType) const
{
	return m_sValueTotalBet[eType];
}

//	设置续押数值
void SGLYInfoCenter::setValueContinueBet(E_SGLY_Lottery eType, const LONGLONG sValue)
{
	m_sValueContinueBet[eType] = sValue;
}

//	获得续押数值
LONGLONG SGLYInfoCenter::getValueContinueBet(E_SGLY_Lottery eType) const
{
	return m_sValueContinueBet[eType];
}

bool SGLYInfoCenter::init()
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

void SGLYInfoCenter::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	int nBetFontSize = 30;
	Color3B cBetFontColor3B = Color3B::YELLOW;
	for (int i = E_SGLY_Lottery_Bar; i < E_SGLY_Lottery_Null; i++)
	{
		m_pLabSelfBet[i] = Label::create();
		addChild(m_pLabSelfBet[i]);
		m_pLabSelfBet[i]->setPosition(Vec2(sVSize.width * (0.06 + i * 0.125), sVSize.height * 0.13));
		m_pLabSelfBet[i]->setSystemFontSize(nBetFontSize);
		m_pLabSelfBet[i]->setColor(cBetFontColor3B);

		m_pLabTotalBet[i] = Label::create();
		addChild(m_pLabTotalBet[i]);
		m_pLabTotalBet[i]->setPosition(Vec2(sVSize.width * (0.06 + i * 0.125), sVSize.height * 0.155));
		m_pLabTotalBet[i]->setSystemFontSize(nBetFontSize);
		m_pLabTotalBet[i]->setColor(cBetFontColor3B);
	}

	Vec2 vPosInfoBar[E_SGLY_InfoBar_Null] =
	{
		Vec2(sVSize.width * 0.88, sVSize.height * 0.28),
		Vec2(sVSize.width * 0.48, sVSize.height * 0.28),
		Vec2(sVSize.width * 0.51, sVSize.height * 0.95),
		Vec2(sVSize.width * 0.51, sVSize.height * 0.985),
		Vec2(sVSize.width * 0.21, sVSize.height * 0.95),
	};
	for (int i = E_SGLY_InfoBar_PlayerChip; i < E_SGLY_InfoBar_Null; i++)
	{
		m_pLabInfoBar[i] = Label::create();
		addChild(m_pLabInfoBar[i]);
		m_pLabInfoBar[i]->setPosition(vPosInfoBar[i]);
		m_pLabInfoBar[i]->setSystemFontSize(nBetFontSize);
		m_pLabInfoBar[i]->setColor(cBetFontColor3B);

		if (i == E_SGLY_InfoBar_PlayerChip || i == E_SGLY_InfoBar_PlayerScore)
		{
			m_pLabInfoBar[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			m_pLabInfoBar[i]->setColor(Color3B::RED);
		}
		else
		{
			m_pLabInfoBar[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		}
	}

	m_pLabBankerNickname = Label::create();
	addChild(m_pLabBankerNickname);
	m_pLabBankerNickname->setPosition(Vec2(sVSize.width * 0.21, sVSize.height * 0.985));
	m_pLabBankerNickname->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabBankerNickname->setSystemFontSize(nBetFontSize);
	m_pLabBankerNickname->setColor(cBetFontColor3B);
}

void SGLYInfoCenter::initData()
{
	for (int i = E_SGLY_Lottery_Bar; i < E_SGLY_Lottery_Null; i++)
	{
		updateSelfBet((E_SGLY_Lottery)i, 0);
		updateTotalBet((E_SGLY_Lottery)i, 0);
		setValueContinueBet((E_SGLY_Lottery)i, 0);
	}

	for (int i = E_SGLY_InfoBar_PlayerChip; i < E_SGLY_InfoBar_Null; i++)
	{
		updateInfoBar((E_SGLY_InfoBar)i, 0);
	}
	setOverwrite(false);
	

	Dictionary * pDictionary = Dictionary::createWithContentsOfFile(SGLY_XML_FILE_NAME);
	m_sNoneBankerName = ((CCString*)pDictionary->objectForKey(SGLY_XML_KEY_SYSTEM_BANKER))->getCString();
	m_sNoneBankerName = ToolKit::generateNickname();
	LONGLONG llApplyBankerCondition = SGLYTool::m_llApplyBankerCondition;
	m_llNoneBankerChip = random(llApplyBankerCondition, llApplyBankerCondition * 2);
}
