#include "BRNNClock.h"
#include "ToolKit.h"
#include "BRNNRes.h"
#include "BRNNDefine.h"
#include "BRNNManageAudio.h"
#include "DDZTimeCheckControl.h"

BRNNClock::BRNNClock()
{
}

BRNNClock::~BRNNClock()
{
}

void BRNNClock::playCountdownTimer(int nValue)
{
	m_nTime = nValue;
	setVisible(true);

	m_pBMFNum->setString(Value(m_nTime--).asString());
	unschedule(schedule_selector(BRNNClock::onCountdown));
	schedule(schedule_selector(BRNNClock::onCountdown), 1.0f);
}

void BRNNClock::setTimerType(E_BRNNProcessType eType)
{
	switch (eType)
	{
	case E_BRNN_Idle:
		m_pTips->setSpriteFrame(BRNNTextureName::s_brnn_tips_idle_time);
		break;
	case E_BRNN_Bet:
		m_pTips->setSpriteFrame(BRNNTextureName::s_brnn_tips_bet_time);
		break;
	case E_BRNN_OpenCard:
		m_pTips->setSpriteFrame(BRNNTextureName::s_brnn_tips_open_card_time);
		break;
	case E_BRNN_Null:
		break;
	default:
		return;
	}

	m_eType = eType;
}

bool BRNNClock::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		initUI();
		initData();

		bRet = true;
	} while (0);

	return bRet;
}

void BRNNClock::initUI()
{
	m_pBG = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_clock_bg);
	addChild(m_pBG);

	cocos2d::Size sBGSize = m_pBG->getContentSize();
	m_pTips = Sprite::create();
	m_pBG->addChild(m_pTips);
	m_pTips->setPosition(Vec2(sBGSize.width * 2, sBGSize.height * 0.19));

	m_pBMFNum = Label::createWithBMFont(BRNN_CLOCK_NUMBER_FNT, "");
	m_pBG->addChild(m_pBMFNum);
	m_pBMFNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pBMFNum->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.4));

}

void BRNNClock::initData()
{
	m_nTime = 0;
	m_eType = E_BRNN_Null;
}

void BRNNClock::onCountdown(float dt)
{
	int nPauseTime = DDZTimeCheckControl::getInstance()->getPauseTime();
	m_nTime -= nPauseTime;
	m_pBMFNum->setString(Value(m_nTime).asString());
	if (m_nTime <= 5)
	{
		BRNNManageAudio::getInstance()->playSoundEffect(E_BRNNAudio_Countdown);
	}
	if (m_nTime <= 0)
	{
		setVisible(false);
		unschedule(schedule_selector(BRNNClock::onCountdown));
	}

	m_nTime --;
}
