#include "BRNNSettlement.h"
#include "BRNNRes.h"

BRNNSettlement::BRNNSettlement()
{
}

BRNNSettlement::~BRNNSettlement()
{
}

//	更新玩家得分
void BRNNSettlement::updatePlayerScore(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pLabPlayerScore->setString(cBuf);
	if (sValue < 1000)
	{
		m_pLabPlayerScore->setSystemFontSize(30);
	}
	else if (sValue >= 1000 && sValue < 100000)
	{
		m_pLabPlayerScore->setSystemFontSize(25);
	}
	else if (sValue >= 100000)
	{
		m_pLabPlayerScore->setSystemFontSize(20);
	}
}

//	更新玩家返回本金
void BRNNSettlement::updatePlayerCapital(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pLabPlayerCapital->setString(cBuf);

	if (sValue < 1000)
	{
		m_pLabPlayerCapital->setSystemFontSize(30);
	}
	else if (sValue >= 1000 && sValue < 100000)
	{
		m_pLabPlayerCapital->setSystemFontSize(25);
	}
	else if (sValue >= 100000)
	{
		m_pLabPlayerCapital->setSystemFontSize(20);
	}
}

//	更新庄家得分
void BRNNSettlement::updateBankerScore(SCORE sValue)
{
	char cBuf[128];
	sprintf(cBuf, "%.2f", sValue);
	m_pLabBankerScore->setString(cBuf);

	if (sValue < 1000)
	{
		m_pLabBankerScore->setSystemFontSize(30);
	}
	else if (sValue >= 1000 && sValue < 100000)
	{
		m_pLabBankerScore->setSystemFontSize(25);
	}
	else if (sValue >= 100000)
	{
		m_pLabBankerScore->setSystemFontSize(20);
	}

}

bool BRNNSettlement::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		initUI();

		bRet = true;
	} while (0);

	return bRet;

}

void BRNNSettlement::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	m_pBG = Sprite::create(BRNNTextureName::s_brnn_settlement_bg);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(sVSize / 2));

	cocos2d::Size sBGSize = m_pBG->getContentSize();
	int nFontSize = 30;

	m_pLabPlayerScore = Label::create();
	m_pBG->addChild(m_pLabPlayerScore);
	m_pLabPlayerScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabPlayerScore->setPosition(Vec2(sBGSize.width * 0.38f, sBGSize.height * 0.6f));
	m_pLabPlayerScore->setSystemFontSize(nFontSize);

	m_pLabPlayerCapital = Label::create();
	m_pBG->addChild(m_pLabPlayerCapital);
	m_pLabPlayerCapital->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabPlayerCapital->setPosition(Vec2(sBGSize.width * 0.62f, sBGSize.height * 0.6f));
	m_pLabPlayerCapital->setSystemFontSize(nFontSize);

	m_pLabBankerScore = Label::create();
	m_pBG->addChild(m_pLabBankerScore);
	m_pLabBankerScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabBankerScore->setPosition(Vec2(sBGSize.width * 0.38f, sBGSize.height *0.32f));
	m_pLabBankerScore->setSystemFontSize(nFontSize);
}
