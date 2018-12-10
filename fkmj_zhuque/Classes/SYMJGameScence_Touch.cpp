#include "SYMJGameScence.h"
#include "SYMJPlayer.h"
#include "GameLib.h"
#include "CMD_SYMJ.h"
#include "SYMJPlayer.h"

class SYMJTouchScence
	:public cocos2d::Layer
{
public:
	virtual bool init()
	{
		if ( !cocos2d::Layer::init() )
		{
			return false;
		}
		setTouchEnabled(true);
		setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
		return true;
	}
public:
	CREATE_FUNC(SYMJTouchScence);
public:
	virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return false;
		}
		if (pTouch->getID() != 0)
		{
			return false;
		}
		return SYMJGameScence::Instance().ccTouchBegan(pTouch->getLocation());
	}
	virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return;
		}
		if (pTouch->getID() != 0)
		{
			return;
		}
		return SYMJGameScence::Instance().ccTouchMoved(pTouch->getDelta());
	}
	virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return;
		}
		if (pTouch->getID() != 0)
		{
			return;
		}
		return SYMJGameScence::Instance().ccTouchEnded(pTouch->getLocation());
	}
};

void SYMJGameScence::initTouch()
{
	SYMJTouchScence* pTouchScence = new SYMJTouchScence;
	pTouchScence->init();
	addChild(pTouchScence);
}

bool SYMJGameScence::ccTouchBegan(cocos2d::Vec2 kPos)
{
	if (!IsInGame()|| WidgetFun::isWidgetVisble(this,"SelfActionNode") || WidgetFun::isWidgetVisble(this,"TuoGuanStateNode"))
	{
		return false;
	}
	if (m_pLocal->GetChairID() == m_iCurrentUser && getGameState() == HNMJ_STATE_PLAYING)
	{
		m_pTouchCardNode = m_pLocal->getTouchCardNode(kPos);
		if (!m_pTouchCardNode)
		{
			m_pLocal->seatDownCard();
			return false;
		}
		cocos2d::Color3B kColor = m_pTouchCardNode->getDisplayedColor();
		if ( kColor == cocos2d::Color3B::GRAY)
		{
			return false;
		}
		m_kTouchSrcPos = m_pTouchCardNode->getPosition();
		if (m_kTouchSrcPos.y == SYMJPlayer::CARD_UP_POSY)
		{
			SendOutCard(m_pTouchCardNode);
			return false;
		}
		else
		{
			m_pLocal->seatDownCard();
			m_pLocal->ClickCard(m_pTouchCardNode);
			return true;
		}
	}
	
	return false;
}
void SYMJGameScence::ccTouchMoved(cocos2d::Vec2 kPos)
{
	cocos2d::Vec2 kTempPos = m_pTouchCardNode->getPosition();
	m_pTouchCardNode->setPosition(kTempPos+kPos);
}
void SYMJGameScence::ccTouchEnded(cocos2d::Vec2 kPos)
{
	cocos2d::Vec2 kTempPos = m_pTouchCardNode->getPosition();
	if (kTempPos.y != SYMJPlayer::CARD_UP_POSY || kTempPos.x !=  m_kTouchSrcPos.x)
	{
		m_pTouchCardNode->setPosition(m_kTouchSrcPos);
	}
	if (kTempPos.y > SYMJPlayer::CARD_UP_POSY+20)
	{
		SendOutCard(m_pTouchCardNode);
	}
}

void SYMJGameScence::SendOutCard(cocos2d::Node* pCard)
{
	ASSERT(pCard && m_nGameState == HNMJ_STATE_PLAYING);
	if (pCard == NULL || m_nGameState != HNMJ_STATE_PLAYING ) return;

	CMD_C_OutCard OutCard;
	OutCard.cbCardData = m_pLocal->getTouchCardVlaue(pCard);
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
	remmoveAutoAction();
}