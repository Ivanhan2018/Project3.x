#include "NNPlayer.h"
#include "NNSoundFun.h"
#include "NNGameLogic.h"


void NNPlayer::showBackCard()
 {
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode");
	for (int i = 0;i < 5;i++)
	{
		cocos2d::Node* pCardNode = WidgetFun::getChildWidget(pNode,utility::toString("Card",i));
		pCardNode->setVisible(true);
		WidgetFun::setVisible(pCardNode,"CardShow",false);
		WidgetFun::setVisible(pCardNode,"CardBack",true);
	}
}
void NNPlayer::showStartCard(BYTE* cbCardData)
{
	memcpy(m_cbCardData,cbCardData,sizeof(m_cbCardData));
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode");
	for (int i = 0;i < 3;i++)
	{
		cocos2d::Node* pCardNode = WidgetFun::getChildWidget(pNode,utility::toString("Card",i));
		pCardNode->setVisible(true);
		upCardTexture(cbCardData[i],pCardNode);
		WidgetFun::setVisible(pCardNode,"CardShow",true);
		WidgetFun::setVisible(pCardNode,"CardBack",false);
	}
	for (int i = 3;i < 5;i++)
	{
		cocos2d::Node* pCardNode = WidgetFun::getChildWidget(pNode,utility::toString("Card",i));
		pCardNode->setVisible(true);
		upCardTexture(cbCardData[i],pCardNode);
		WidgetFun::setVisible(pCardNode,"CardShow",false);
		WidgetFun::setVisible(pCardNode,"CardBack",true);
	}
}
void NNPlayer::setHandCard(BYTE* cbCardData,bool bShow)
{
	memcpy(m_cbCardData,cbCardData,sizeof(m_cbCardData));
	if (bShow)
	{
		showAllCard();
	}
	else
	{
		showBackCard();
	}
}
void NNPlayer::showAllCard()
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode");
	for (int i = 0;i < 5;i++)
	{
		cocos2d::Node* pCardNode = WidgetFun::getChildWidget(pNode,utility::toString("Card",i));
		pCardNode->setVisible(true);
		upCardTexture(m_cbCardData[i],pCardNode);
		WidgetFun::setVisible(pCardNode,"CardShow",true);
		WidgetFun::setVisible(pCardNode,"CardBack",false);
	}
}
void NNPlayer::showNiuCard(BYTE cbNiu)
{
	WidgetFun::setVisible(m_pSeatNode,"NN_CardType",true);
	if (cbNiu)
	{
		int iNum = NNGameLogic::Instance().GetCardType(m_cbCardData,MAX_COUNT);
		WidgetFun::setImagic(m_pSeatNode,"TypeCard",utility::toString("GameNN/niu_",iNum,".png"));
		NNSoundFun::playEffect(utility::toString("manbull",iNum,".mp3"));
	}
	else
	{
		WidgetFun::setImagic(m_pSeatNode,"TypeCard",utility::toString("GameNN/niu_",0,".png"));
		NNSoundFun::playEffect(utility::toString("manbull",0,".mp3"));
	}
	showAllCard();
}

void NNPlayer::upCardTexture(BYTE uCard,cocos2d::Node* pNode)
{

	int cbNum = NNGameLogic::Instance().GetCardValue(uCard);
	int cbType = NNGameLogic::Instance().GetCardColor(uCard);

	WidgetFun::setWidgetUserInfo(pNode,"CardNum",utility::toString(cbNum));
	WidgetFun::setVisible(pNode,"Joker",false);
	WidgetFun::setVisible(pNode,"ImagicSmall",true);
	WidgetFun::setVisible(pNode,"Number",true);

	if (cbType == 1 ||cbType == 3)
	{
		WidgetFun::setImagic(pNode,"Number",utility::toString("dalete/b_",cbNum,".png"),true);
	}
	else
	{
		WidgetFun::setImagic(pNode,"Number",utility::toString("dalete/r_",cbNum,".png"),true);
	}

	WidgetFun::setImagic(pNode,"ImagicSmall",utility::toString("dalete/TypeS",cbType+1,".png"),true);
	WidgetFun::setImagic(pNode,"ImagicBig",utility::toString("dalete/Type",cbType+1,".png"),true);

	WidgetFun::setVisible(pNode,"OtherImagicBig",false);
	WidgetFun::setVisible(pNode,"ImagicBig",true);
}
void NNPlayer::sendCardAni(int nValue)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode");
	for (int i = 0;i < 5;i++)
	{
		cocos2d::Node* pCardNode = WidgetFun::getChildWidget(pNode,utility::toString("Card",i));
		float fTime =utility::parseFloat( WidgetFun::getWidgetUserInfo(pCardNode,"SendWaitTime"));
		WidgetFun::setWidgetUserInfo(pCardNode,"SendWaitTimeTemp",utility::toString(fTime+nValue*0.4f));
		WidgetFun::runWidgetAction(pCardNode,"SendAni");
	}
}

cocos2d::Node* NNPlayer::getTouchCardNode(cocos2d::Vec2 kTouchPos)
{
	for (int i = 0;i<MAX_CARD;i++)
	{
		cocos2d::Node* pSprite =  WidgetFun::getChildWidget(m_pSeatNode,utility::toString("Card",i,":CardShow"));
		if (pSprite == NULL)
		{
			continue;
		}
		if (utility::IsSpriteInTouch(kTouchPos,(cocos2d::Sprite*)pSprite))
		{
			return pSprite->getParent();
		}
	}
	return NULL;
}