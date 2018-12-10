#include "NCMJGameScence.h"
#include "NCMJPlayer.h"
#include "GameLib.h"
#include "CMD_NCMJ.h"
#include "NCMJPlayer.h"

class NCMJTouchScence
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
	CREATE_FUNC(NCMJTouchScence);
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
		return NCMJGameScence::Instance().ccTouchBegan(pTouch->getLocation());
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
		return NCMJGameScence::Instance().ccTouchMoved(pTouch->getDelta());
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
		return NCMJGameScence::Instance().ccTouchEnded(pTouch->getLocation());
	}
};

void NCMJGameScence::initTouch()
{
	NCMJTouchScence* pTouchScence = new NCMJTouchScence;
	pTouchScence->init();
	addChild(pTouchScence);
}

bool NCMJGameScence::ccTouchBegan(cocos2d::Vec2 kPos)
{
	if (!IsInGame())
	{
		return false;
	}
	if (m_pLocal->GetChairID() == m_iCurrentUser && getGameState() == NCMJ_STATE_PLAYING)
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
		if (m_kTouchSrcPos.y == NCMJPlayer::CARD_UP_POSY)
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
void NCMJGameScence::ccTouchMoved(cocos2d::Vec2 kPos)
{
	cocos2d::Vec2 kTempPos = m_pTouchCardNode->getPosition();
	m_pTouchCardNode->setPosition(kTempPos+kPos);
}
void NCMJGameScence::ccTouchEnded(cocos2d::Vec2 kPos)
{
	cocos2d::Vec2 kTempPos = m_pTouchCardNode->getPosition();
	if (kTempPos.y != NCMJPlayer::CARD_UP_POSY || kTempPos.x !=  m_kTouchSrcPos.x)
	{
		m_pTouchCardNode->setPosition(m_kTouchSrcPos);
	}
	if (kTempPos.y > NCMJPlayer::CARD_UP_POSY)
	{
		SendOutCard(m_pTouchCardNode);
	}
}

void NCMJGameScence::SendOutCard(cocos2d::Node* pCard)
{
	ASSERT(pCard && m_nGameState == NCMJ_STATE_PLAYING);
	if (pCard == NULL || m_nGameState != NCMJ_STATE_PLAYING ) return;

	CMD_C_OutCard OutCard;
	OutCard.cbCardData = m_pLocal->getTouchCardVlaue(pCard);
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
}