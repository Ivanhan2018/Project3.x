#include "SGLYClock.h"
#include "ToolKit.h"
#include "DDZTimeCheckControl.h"
#include "SGLYRes.h"
#include "SGLYManageAudio.h"

SGLYClock::SGLYClock()
{
}

SGLYClock::~SGLYClock()
{
}

//	中断
void SGLYClock::reset()
{
	unscheduleAllCallbacks();
	setVisible(false);
	m_eType = E_SGLY_Process_Null;
}

void SGLYClock::playCountdownTimer(int nValue)
{
	m_nTime = nValue;
	if (m_eType >= E_SGLY_Process_Lottery)
	{
		m_pLabNum->setString(Value(0).asString());
	}
	else
	{
		m_pLabNum->setString(Value(m_nTime--).asString());
	}
	setVisible(true);
// 	unschedule(schedule_selector(SGLYClock::onCountdown));
	unscheduleAllCallbacks();
	schedule(schedule_selector(SGLYClock::onCountdown), 1.0f);
}

void SGLYClock::setTimerType(E_SGLY_Process eType)
{
	switch (eType)
	{
	case E_SGLY_Process_Idle:
		m_pTips->initWithSpriteFrameName(SGLYTextureName::sSGLY_TipsIdleTime);
		break;
	case E_SGLY_Process_Bet:
		m_pTips->initWithSpriteFrameName(SGLYTextureName::sSGLY_TipsBetTime);
		break;
	case E_SGLY_Process_Lottery:
		m_pTips->initWithSpriteFrameName(SGLYTextureName::sSGLY_TipsLotteryTime);
		break;
	case E_SGLY_Process_Guess:
		m_pTips->initWithSpriteFrameName(SGLYTextureName::sSGLY_TipsGuessTime);
		break;
	case E_SGLY_Process_Null:
		break;
	default:
		break;
	}
	m_eType = eType;
}

//	显示猜大小动画
void SGLYClock::showGuessAni()
{
	setVisible(true);
	setTimerType(E_SGLY_Process_Guess);
	unscheduleAllCallbacks();
	schedule(schedule_selector(SGLYClock::showRandomNum), 0.25f);
}

//	设置显示的数字
void SGLYClock::setLabNum(int nValue)
{
	m_pLabNum->setString(Value(nValue).asString());
}

E_SGLY_Process SGLYClock::getProcess() const
{
	return m_eType;
}

bool SGLYClock::init()
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

void SGLYClock::initUI()
{
	m_pBG = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_ClockBG);
	addChild(m_pBG);
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pTipsBG = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_TipsBG);
	m_pBG->addChild(m_pTipsBG);
	m_pTipsBG->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 2));

	m_pTips = Sprite::create();
	m_pTipsBG->addChild(m_pTips);
	m_pTips->setPosition(Vec2(m_pTipsBG->getContentSize() / 2));

	m_pLabNum = Label::create();
	m_pBG->addChild(m_pLabNum);
	m_pLabNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pLabNum->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.5));
	m_pLabNum->setSystemFontSize(35);
	m_pLabNum->setColor(Color3B::WHITE);
}

void SGLYClock::initData()
{
	m_nTime = 0;
	m_nRandomNum = 0;
	m_eType = E_SGLY_Process_Null;
}

void SGLYClock::onCountdown(float dt)
{
	int nPauseTime = DDZTimeCheckControl::getInstance()->getPauseTime();
	m_nTime -= nPauseTime;
	if (m_eType < E_SGLY_Process_Lottery)
	{
		setLabNum(m_nTime);
	}
	if (m_nTime <= 5)
	{
		if (m_eType < E_SGLY_Process_Lottery)
		{
			SGLYManageAudio::getInstance()->playSoundEffect(E_SGLY_Audio_TimeWarning);
		}
	}
	if (m_nTime <= 0)
	{
		setVisible(false);
// 		unschedule(schedule_selector(SGLYClock::onCountdown));
		unscheduleAllCallbacks();
	}

	m_nTime --;
}

void SGLYClock::showRandomNum(float dt)
{
	do 
	{
		int nNum = random(1, 14);
		if (nNum != m_nRandomNum)
		{
			m_nRandomNum = nNum;
			break;
		}
	} while (1);
	setLabNum(m_nRandomNum);
}
