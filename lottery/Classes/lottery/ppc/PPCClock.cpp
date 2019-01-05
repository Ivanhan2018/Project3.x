#include "PPCClock.h"
#include "PPCRes.h"
#include "PPCGameScene.h"
#include "ToolKit.h"
#include "PPCSound.h"

PPCClock::PPCClock()
{
}

PPCClock::~PPCClock()
{
}

void PPCClock::countdownTimer(int nValue)
{
	m_nTime = nValue;
	setVisible(true);

	m_pBMFNum->setString(Value(m_nTime--).asString());
	unschedule(schedule_selector(PPCClock::onCountdown));
	schedule(schedule_selector(PPCClock::onCountdown), 1.0f);
}

void PPCClock::setTimerType(EPPCProcessType eType)
{
	switch (eType)
	{
	case EPPC_Idle:
		m_pTips->setSpriteFrame(PPCTextureName::s_ppc_tips_idle_time);
		break;
	case EPPC_Bet:
		m_pTips->setSpriteFrame(PPCTextureName::s_ppc_tips_bet_time);
		break;
	case EPPC_Lottery:
		m_pTips->setSpriteFrame(PPCTextureName::s_ppc_tips_lottery_time);
		break;
	case EPPC_Null:
		break;
	default:
		return;
	}

	m_eType = eType;
}

bool PPCClock::init()
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

void PPCClock::initUI()
{
	m_pBG = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_clock_bg);
	addChild(m_pBG);

	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pTips = Sprite::create();
	m_pBG->addChild(m_pTips);
	m_pTips->setPosition(Vec2(sBGSize.width * 1.6, sBGSize.height * 0.25));

	m_pBMFNum = Label::createWithBMFont("ddz_fnt_time_num.fnt", "");
	m_pBG->addChild(m_pBMFNum);
	m_pBMFNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pBMFNum->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.4));

}

void PPCClock::initData()
{
	m_nTime = 0;
	m_eType = EPPC_Null;
}

void PPCClock::onCountdown(float dt)
{
	m_pBMFNum->setString(Value(m_nTime).asString());
	onTimeEvent();
	if (m_nTime <= 0)
	{
		setVisible(false);
		unschedule(schedule_selector(PPCClock::onCountdown));
	}

	m_nTime --;
}

void PPCClock::onTimeEvent()
{
	switch (m_eType)
	{
	case EPPC_Idle:
		break;
	case EPPC_Bet:
		{
			if (m_nTime <= 5)
			{
				PPCSound::getInstance()->playEffect(EFFECT_TYPE_TIME_WARIMG);
			}
		}
		break;
	case EPPC_Lottery:
		onLotteryTime();
		break;
	case EPPC_Null:
		break;
	default:
		break;
	}
}

void PPCClock::onLotteryTime()
{
	if (m_nTime > PPC_SHOW_ENDING_DURATION)
	{
		return;
	}

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return;
	}

	pGameScene->m_pPPCEnding->setVisible(true);

// 	//testcode , need add the win or lost info
// 	PPCSound::getInstance()->playEffectShowResult(true);
}