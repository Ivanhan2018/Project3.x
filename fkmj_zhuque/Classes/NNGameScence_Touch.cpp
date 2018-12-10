#include "NNGameScence.h"
#include "NNPlayer.h"
#include "GameLib.h"
#include "CMD_Ox.h"
#include "NNPlayer.h"

class NNTouchScence
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
	CREATE_FUNC(NNTouchScence);
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
		return NNGameScence::Instance().ccTouchBegan(pTouch->getLocation());
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
		return NNGameScence::Instance().ccTouchMoved(pTouch->getDelta());
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
		return NNGameScence::Instance().ccTouchEnded(pTouch->getLocation());
	}
};

void NNGameScence::initTouch()
{
	NNTouchScence* pTouchScence = new NNTouchScence;
	pTouchScence->init();
	addChild(pTouchScence);
}

bool NNGameScence::ccTouchBegan(cocos2d::Vec2 kPos)
{
	if (!IsInGame())
	{
		return false;
	}
	m_pTouchCardNode = m_pLocal->getTouchCardNode(kPos);
	if (m_pTouchCardNode)
	{
		return true;
	}
	
	return false;
}
void NNGameScence::ccTouchMoved(cocos2d::Vec2 kPos)
{
}
void NNGameScence::ccTouchEnded(cocos2d::Vec2 kPos)
{
	if (m_pTouchCardNode)
	{
		updateUpCardList(m_pTouchCardNode);
		updateBoxNum();
	}
}

void NNGameScence::updateUpCardList(cocos2d::Node* pCard)
{
	ASSERT(pCard);
	int nPosY = pCard->getPositionY();
	int nLenth = m_kUpCardList.size();

	if ( nPosY== NNPlayer::POSY_DOWN && nLenth<3 )
	{
		NNPlayer::clickCard(m_pTouchCardNode);
		m_kUpCardList.push_back(pCard);
	}
	else if (nPosY== NNPlayer::POSY_UP)
	{
		NNPlayer::clickCard(m_pTouchCardNode);
		utility::removeInVector(m_kUpCardList,pCard);
	}
}
void NNGameScence::updateBoxNum()
{
	for (int i=0;i<4;i++)
	{
		WidgetFun::setText(this,utility::toString("BoxNum",i),"");
	}
	int nCount = 0;
	for (int i=0;i<(int)m_kUpCardList.size();i++)
	{
		int nCardNum = utility::parseInt(WidgetFun::getWidgetUserInfo(m_kUpCardList[i],"CardNum"));
		if (nCardNum>10)
		{
			nCardNum = 10;
		}
		WidgetFun::setText(this,utility::toString("BoxNum",i),nCardNum);
		nCount += nCardNum;
	}
	WidgetFun::setText(this,utility::toString("BoxNum",3),nCount);
	if (nCount%10 == 0 && (int)m_kUpCardList.size() == 3)
	{
		WidgetFun::setText(this,"ReroultText",ScriptData<std::string>("NNResoultYou").Value());
		WidgetFun::setButtonEnabled(this,"NNButton_YouNiu",true);
		WidgetFun::setButtonEnabled(this,"NNButton_MeiYou",false);
	}
	else
	{
		WidgetFun::setText(this,"ReroultText",ScriptData<std::string>("NNResoultWu").Value());
		WidgetFun::setButtonEnabled(this,"NNButton_YouNiu",false);
		WidgetFun::setButtonEnabled(this,"NNButton_MeiYou",true);
	}

}