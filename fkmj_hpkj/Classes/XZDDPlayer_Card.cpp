#include "XZDDPlayer.h"
#include "XZDDGameLogic.h"
#include "XZDDSoundFun.h"


void XZDDPlayer::setHandCard(BYTE* pCardData,int iCout)
{
	m_kHandCardCout = 0;
	if (iCout <= 0 || iCout > MAX_COUNT)
	{
		return;
	}
	memcpy(m_kHandCardData,pCardData,sizeof(BYTE)*iCout);
	m_kHandCardCout = iCout;
}
void XZDDPlayer::setWeaveItem(XZDD_CMD_WeaveItem* pWeave,int iCout)
{
	m_kWeaveCount = 0;
	if (iCout <= 0 || iCout > MAX_WEAVE)
	{
		return;
	}
	memcpy(m_kWeaveItemArray,pWeave,sizeof(XZDD_CMD_WeaveItem)*iCout);
	m_kWeaveCount = iCout;
}
void XZDDPlayer::setOperateResoult(XZDD_CMD_S_OperateResult* pNetInfo)
{
	bool cbPublicCard = false;
	word wOperateUser=pNetInfo->wOperateUser;
	BYTE cbOperateCard=pNetInfo->cbOperateCard;

	if (pNetInfo->cbOperateCode == WIK_PENG)
	{
		runAniPeng();
	}
	if ((pNetInfo->cbOperateCode&WIK_GANG)!=0)
	{
		//组合扑克
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_kWeaveCount;i++)
		{
			BYTE cbWeaveKind=m_kWeaveItemArray[i].cbWeaveKind;
			BYTE cbCenterCard=m_kWeaveItemArray[i].cbCenterCard;
			if ((cbCenterCard==pNetInfo->cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				cbPublicCard = true;
				m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=cbPublicCard;
				m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
				m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;
				break;
			}
		}

		//组合扑克
		if (cbWeaveIndex==0xFF)
		{
			//暗杠判断
			cbPublicCard=(pNetInfo->wProvideUser==wOperateUser)?FALSE:TRUE;

			//设置扑克
			cbWeaveIndex=m_kWeaveCount++;
			m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_kWeaveItemArray[cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
			m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;
		}

		//扑克设置
		if (m_iIdex == 0 || m_bGameRecord|| m_bMingPai )
		{
			 XZDD::CGameLogic::Instance().RemoveValueCardAll(m_kHandCardData,m_kHandCardCout,pNetInfo->cbOperateCard);
		}
		m_kHandCardCout = MAX_COUNT - m_kWeaveCount*3 - 1;

		if ( !cbPublicCard)
		{
			runAniAnGang();
		}
		else
		{
			runAniMingGang();
		}
	}
	else if (pNetInfo->cbOperateCode!=WIK_NULL)
	{
		//设置组合
		BYTE cbWeaveIndex=m_kWeaveCount++;
		m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=true;
		m_kWeaveItemArray[cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
		m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;

		//组合界面
		//删除扑克
		if (m_iIdex == 0 || m_bGameRecord || m_bMingPai)
		{
			BYTE cbWeaveCard[4]={cbOperateCard,cbOperateCard,cbOperateCard,cbOperateCard};
			XZDD::CGameLogic::Instance().RemoveValueCard(m_kHandCardData,m_kHandCardCout,cbWeaveCard,2);
		}
		m_kHandCardCout = MAX_COUNT - m_kWeaveCount*3;
	}
	showHandCard();
}
void XZDDPlayer::addNewInCard(int iCard)
{
	m_kHandCardCout++;
	m_kHandCardData[MAX_COUNT-1-m_kWeaveCount*3] = iCard;

	showHandCard();
}
void XZDDPlayer::sendOutCard(int iCard)
{
	m_pHandOutCard = addHandOutCard(iCard);
	m_pHandOutCard->setVisible(false);
	WidgetFun::runWidgetAction(m_pHandOutCard,"OutAni");//等待2秒显示

	setActOutCard(iCard);
}

void XZDDPlayer::showMJPoint()
{
	if (m_pHandOutCard)
	{
		WidgetFun::setPos(m_pSeatNode,"MJPoint",m_pHandOutCard->getPosition());
		WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","Start");
	}
}

void XZDDPlayer::setActOutCard(int iCard)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"BigOutNode");
	pNode->setVisible(iCard >= 0);

	if (iCard < 0)
	{
		WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","End");
		return;
	}
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	setCardImagic(pNode,iCard,kImagic);

	if (m_pHandOutCard != NULL)
	{
		//出牌动画
		cocos2d::Vec2 kEndPos = m_pHandOutCard->getPosition();
		pNode->setAnchorPoint(m_pHandOutCard->getAnchorPoint());
		WidgetFun::setWidgetUserInfo(pNode,"OutHandPos",utility::toString(kEndPos));
		WidgetFun::runWidgetAction(pNode,"Start");
		showMJPoint();
	}

	XZDDSoundFun::playCardByGender(GetGender(),iCard);

	if (m_iIdex == 0 || m_bGameRecord || m_bMingPai)
	{
		XZDD::CGameLogic::Instance().RemoveValueCardOne(m_kHandCardData,m_kHandCardCout,iCard);
		m_kHandCardCout --;
		XZDD::CGameLogic::Instance().SortCardList(m_kHandCardData,m_kHandCardCout);
	}
	else
	{
		m_kHandCardCout -- ;
	}

	showHandCard();

}
cocos2d::Node* XZDDPlayer::addHandOutCard(int iCard)
{
	int iIdex = m_iIdex;
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode2");
	cocos2d::Vec2 kStartPos = WidgetFun::getChildWidget(m_pSeatNode,"OutPosNode")->getPosition();
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"OutPosNode","AddOder"));
	cocos2d::Vec2 kOutAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"OutPosNode","OutAddPos"));
	cocos2d::Vec2 kNextAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"OutPosNode","NextAddPos"));
	cocos2d::Vec2 kPos;
	int nLength = m_kOutCardList.size();
	if (nLength >= 10)
	{
		kPos = kOutAddPos*(nLength-10)+kStartPos;
		kPos += kNextAddPos;
	}
	else
	{
		kPos = kOutAddPos*nLength+kStartPos;
	}
	std::string kHandSkin = utility::toString("XZDD_OUT_",iIdex);

	cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
	pNode->setLocalZOrder(iAddOder*nLength);
	pNode->setPosition(kPos);
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	setCardImagic(pNode,iCard,kImagic);

	m_pHandOutCard = pNode;
	m_kOutCardList.push_back(OutCardInfo(pNode,iCard));
	return pNode;
}
void XZDDPlayer::showHandCard()
{
	showHandCard(m_kWeaveItemArray,m_kWeaveCount,m_kHandCardData,m_kHandCardCout);
}
void XZDDPlayer::showHandCard(XZDD_CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout)
{
	bool bHaveNewIn = (iWeaveCout*3+iHandCout) == MAX_COUNT;
	int iIdex = m_iIdex;
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	pCardNode->removeAllChildren();
	cocos2d::Vec2 kStartPos = WidgetFun::getChildWidget(m_pSeatNode,"HandPosNode")->getPosition();
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","AddOder"));
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","HandAddPos"));
	cocos2d::Vec2 kTempWeaveAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","WeaveAddPos"));
	cocos2d::Vec2 kNewInAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","NewInAddPos"));
	std::string kPengSkin = utility::toString("XZDD_PENG_",iIdex);
	std::string kGangSkin = utility::toString("XZDD_GANG_",iIdex);
	std::string kHandSkin = utility::toString("XZDD_HAND_",iIdex);
	int iOder = 0;

	if (m_iIdex == 1)
	{
		kStartPos.y += (4-m_kWeaveCount)*6;
	}
	if (m_iIdex == 3)
	{
		kStartPos.y -= (4-m_kWeaveCount)*6;
	}
	cocos2d::Vec2 kWeaveAddPos =  kTempWeaveAddPos*3;

	for (int i = 0;i<iWeaveCout;i++)
	{
		XZDD_CMD_WeaveItem* pTemp = pWeave+i;
		int iCardValue = pTemp->cbCenterCard;
		if (pTemp->cbPublicCard == 0)
		{
			iCardValue = -1;
		}

		if (pTemp->cbWeaveKind == WIK_PENG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_GANG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kGangSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card3"),pTemp->cbCenterCard,kImagic);
		}
		kStartPos += kWeaveAddPos;
		iOder+=iAddOder;
	}
	for (int i = 0;i<iHandCout;i++)
	{
		BYTE* pTemp = pHandCard+i;
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setTag(1);
		pNode->setLocalZOrder(iOder);
		pNode->setPosition(kStartPos);
		if (bHaveNewIn && i == (iHandCout - 2))
		{
			kStartPos += kNewInAddPos;
		}
		else
		{
			kStartPos += kHandAddPos;
		}
		int iCardValue = *pTemp;
		if (m_iIdex == 0)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");

			setCardImagic(pNode,iCardValue,kImagic);
		}
		else if (m_bMingPai|| m_bGameRecord)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"MingImagic");
			float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pNode,"HuScale"));
			pNode->setScale(fHuScale);
			setCardImagic(pNode,iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}
}
void XZDDPlayer::setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront)
{
	if (kValue > 0)
	{
		BYTE cbValue=((BYTE)kValue&MASK_VALUE);
		BYTE cbColor=(((BYTE)kValue&MASK_COLOR)>>4)+1;
		WidgetFun::setImagic(pNode,utility::toString(kImagicFront,(int)cbColor,(int)cbValue,".png"),false);
	}
	else
	{
		WidgetFun::setImagic(pNode,utility::toString(kImagicFront,0,0,".png"),false);
	}
}
cocos2d::Node* XZDDPlayer::getTouchCardNode(cocos2d::Vec2 kTouchPos)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		if (utility::IsSpriteInTouch(kTouchPos,pSprite))
		{
			return pSprite;
		}
	}
	return NULL;
}

BYTE XZDDPlayer::getTouchCardVlaue(cocos2d::Node* pNode)
{
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		if (pSprite == pNode)
		{
			return m_kHandCardData[iIdex];
		}
		iIdex++;
	}
	return NULL;
}
void XZDDPlayer::showHuPai(bool bGaizhu)
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pSprite,"HuScale"));
		pSprite->setScale(fHuScale);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pSprite,"MingImagic");
		
		if(bGaizhu)
		{
			setCardImagic(pSprite,0,kImagic);
		}
	}
}

void XZDDPlayer::sortHandCard()
{
	if (m_iIdex != 0 )return;
	XZDD::CGameLogic::Instance().SortCardList(m_kHandCardData,m_kHandCardCout);
}

void XZDDPlayer::showHuanPai(int nGameType /*= GAME_TYPE_CHENGDU*/)
{
	if (m_iIdex != 0 )return;

	sortHandCard();
	showHandCard();

	std::vector<XZDD_Card_Info> kCardInfoList;
	XZDD::CGameLogic::Instance().GetCardInfoList(m_kHandCardData,MAX_COUNT-1,kCardInfoList);

	std::vector<BYTE> kCardDataList;
	if (nGameType == GAME_TYPE_SICHUAN)
	{
		kCardDataList = setHuanPaiSiChuan(kCardInfoList);
	}
	else if (nGameType == GAME_TYPE_CHENGDU)
	{
		kCardDataList = setHuanPaiChengDu(kCardInfoList);
		ASSERT((int)kCardDataList.size() == HUAN_CARD_NUM);
		int nColor = XZDD::CGameLogic::Instance().GetCardColor(kCardDataList[0]);
		blackHandCardByColor(nColor);
	}

	m_pHuanCardNodeList.clear();
	getCardNodeByCard(kCardDataList,m_pHuanCardNodeList);

	for (int i=0;i<(int)m_pHuanCardNodeList.size();i++)
	{
		cocos2d::Node* pNode = m_pHuanCardNodeList[i];
		pNode->setPositionY(CARD_UP_POSY);
	}
}

std::vector<BYTE> XZDDPlayer::setHuanPaiSiChuan(std::vector<XZDD_Card_Info> kCardInfoList)
{
	std::vector<BYTE> kCardDataList;

	ASSERT(kCardInfoList.size() == CARD_COLOR_TIAO);  //花色数判断
	if (kCardInfoList.size() != CARD_COLOR_TIAO)return kCardDataList;

	int nLen1 = kCardInfoList[0].nLen;			//最少花色
	int nLen2 = kCardInfoList[1].nLen;			//中间花色
	int nLen3 = kCardInfoList[2].nLen;			//最多花色

	ASSERT(nLen1 + nLen2 + nLen3 == MAX_COUNT-1 );
	if (nLen1 + nLen2 + nLen3 != MAX_COUNT-1 ) return kCardDataList;
 
	m_nMastKindColor = kCardInfoList[2].nColor;

	zeromemory(m_pHuanCards,sizeof(m_pHuanCards));

	int iIdex = 0;
	for (int i = 0;i<nLen1 && iIdex < 3;i++)
	{
		m_pHuanCards[iIdex]=kCardInfoList[0].kCardList[i];
		iIdex++;
	}
	for (int i = 0;i<nLen2 && iIdex < 3;i++)
	{
		m_pHuanCards[iIdex]=kCardInfoList[1].kCardList[i];
		iIdex++;
	}
	for (int i = 0;i<nLen3 && iIdex < 3;i++)
	{
		m_pHuanCards[iIdex]=kCardInfoList[2].kCardList[i];
		iIdex++;
	}

	kCardDataList.push_back(m_pHuanCards[0]);
	kCardDataList.push_back(m_pHuanCards[1]);
	kCardDataList.push_back(m_pHuanCards[2]);

	return kCardDataList;
}
std::vector<BYTE> XZDDPlayer::setHuanPaiChengDu(std::vector<XZDD_Card_Info> kCardInfoList)
{
	std::vector<BYTE> kCardDataList;

	ASSERT(kCardInfoList.size() == CARD_COLOR_TIAO);  //花色数判断
	if (kCardInfoList.size() != CARD_COLOR_TIAO)return kCardDataList;

	int nLen1 = kCardInfoList[0].nLen;			//最少花色
	int nLen2 = kCardInfoList[1].nLen;			//中间花色
	int nLen3 = kCardInfoList[2].nLen;			//最多花色

	ASSERT(nLen1 + nLen2 + nLen3 == MAX_COUNT-1 );
	if (nLen1 + nLen2 + nLen3 != MAX_COUNT-1 ) return kCardDataList;
  
	m_nMastKindColor = kCardInfoList[2].nColor;

	zeromemory(m_pHuanCards,sizeof(m_pHuanCards));

	if (nLen1>=3)
	{
		m_pHuanCards[0]=kCardInfoList[0].kCardList[0];
		m_pHuanCards[1]=kCardInfoList[0].kCardList[1];
		m_pHuanCards[2]=kCardInfoList[0].kCardList[2];
	}
	else if (nLen2>=3)
	{
		m_pHuanCards[0]=kCardInfoList[1].kCardList[0];
		m_pHuanCards[1]=kCardInfoList[1].kCardList[1];
		m_pHuanCards[2]=kCardInfoList[1].kCardList[2];
	}
	else if (nLen3>=3)
	{
		m_pHuanCards[0]=kCardInfoList[2].kCardList[0];
		m_pHuanCards[1]=kCardInfoList[2].kCardList[1];
		m_pHuanCards[2]=kCardInfoList[2].kCardList[2];
	}

	kCardDataList.push_back(m_pHuanCards[0]);
	kCardDataList.push_back(m_pHuanCards[1]);
	kCardDataList.push_back(m_pHuanCards[2]);

	return kCardDataList;
}

void XZDDPlayer::standUpCard(BYTE cbCard,bool nOnly /*= true*/)
{
	cocos2d::Node* pCard = getCardNodeByCard(cbCard);
	pCard->setPositionY(CARD_UP_POSY);
}
void XZDDPlayer::seatDownCard()
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		pSprite->setPositionY(CARD_DWON_POSY);
	}
}

cocos2d::Node* XZDDPlayer::getCardNodeByCard(BYTE cbCard)
{
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}

		if (cbCard == m_kHandCardData[iIdex])
		{
			return pSprite;
		}
		iIdex++;
	}
	return NULL;
}

void XZDDPlayer::getCardNodeByCard(std::vector<BYTE> CardDataList,std::vector<cocos2d::Node*>& kCardList)
{
	int nSize = CardDataList.size();

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");

	for (int n=0;n<nSize;n++)
	{
		int iIdex = 0;
		for (int i = 0;i<pRootNode->getChildrenCount();i++)
		{
			cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
			if (pSprite == NULL || pSprite->getTag() != 1)
			{
				continue;
			}
			if (CardDataList[n] == m_kHandCardData[iIdex]&& ! isHave<cocos2d::Node>(kCardList,pSprite))
			{
				kCardList.push_back(pSprite);
				break;
			}
			iIdex++;
		}
	}
}

void XZDDPlayer::setHuanPai(BYTE* nHuan)
{
	utility::log(utility::toString("XZDDPlayer::setHuanPai",(int)nHuan[0]," ",(int)nHuan[1]," ",(int)nHuan[2]).c_str());
	XZDD::CGameLogic::Instance().RemoveValueCard(m_kHandCardData,MAX_COUNT-1,m_pHuanCards,HUAN_CARD_NUM);
	XZDD::CGameLogic::Instance().ReplaceCardData(m_kHandCardData,MAX_COUNT-1,nHuan,HUAN_CARD_NUM);
	sortHandCard(); //重新设置牌
	showHandCard();

	std::vector<BYTE> kCardDataList;
	kCardDataList.push_back(nHuan[0]);
	kCardDataList.push_back(nHuan[1]);
	kCardDataList.push_back(nHuan[2]);

	std::vector<cocos2d::Node*> kCardList;
	getCardNodeByCard(kCardDataList,kCardList);
 	for (int i=0;i<(int)kCardList.size();i++)
 	{
 		cocos2d::Node* pNode = kCardList[i];
 		assert(pNode);
 
 		cocos2d::Vec2 posEnd = pNode->getPosition();
 		WidgetFun::setWidgetUserInfo(pNode,"CardPos",utility::toString((int)posEnd.x," ",(int)posEnd.y));
 		WidgetFun::runWidgetAction(pNode,"HuanPaiAni");
 	}
}

void XZDDPlayer::saveRemoveHuanPai(BYTE* nHuan)
{
	memcpy(m_pHuanCards,nHuan,sizeof(m_pHuanCards));
	std::vector<BYTE> kCardDataList;
	for (int i = 0;i<HUAN_CARD_NUM;i++)
	{
		kCardDataList.push_back(nHuan[i]);
	}
	seatDownCard();
	m_pHuanCardNodeList.clear();
	getCardNodeByCard(kCardDataList,m_pHuanCardNodeList);

	for (int i=0;i<(int)m_pHuanCardNodeList.size();i++)
	{
		cocos2d::Node* pNode = m_pHuanCardNodeList[i];
		pNode->setPositionY(CARD_UP_POSY);
	}
}
void XZDDPlayer::saveHuanPai(BYTE* nHuan)
{
	memcpy(m_pSaveHuanPai,nHuan,sizeof(m_pSaveHuanPai));
}

void XZDDPlayer::setHuanPai_ChengDu()
{
	setHuanPai(m_pSaveHuanPai);
}
void XZDDPlayer::blackAllHandCard(bool bBlack)
{
	if (m_iIdex !=0)
	{
		return;
	}
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		if (bBlack)
		{
			pSprite->setColor(cocos2d::Color3B::GRAY);
		}
		else
		{
			pSprite->setColor(cocos2d::Color3B::WHITE);
		}
		iIdex++;
	}
}

void XZDDPlayer::blackHandCardByQueColor()
{
	bool bHaveQue = false;
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		assert(iIdex<MAX_COUNT);
		BYTE nColor = XZDD::CGameLogic::Instance().GetCardColor(m_kHandCardData[iIdex]);

		if (nColor == m_nQueColor)
		{
			pSprite->setColor(cocos2d::Color3B::WHITE);
			bHaveQue = true;
		}
		else
		{
			pSprite->setColor(cocos2d::Color3B::GRAY);
		}
		iIdex++;
	}

	if (!bHaveQue)
	{
		blackAllHandCard(false);
	}
}
void XZDDPlayer::blackHandCardByColor(int nColorWhite)
{
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		BYTE nColor = XZDD::CGameLogic::Instance().GetCardColor(m_kHandCardData[iIdex]);

		if (nColor == nColorWhite)
		{
			pSprite->setColor(cocos2d::Color3B::WHITE);
		}
		else
		{
			pSprite->setColor(cocos2d::Color3B::GRAY);
		}
		iIdex++;
	}
}

void XZDDPlayer::ClickCard(cocos2d::Node* pCard)
{
	if (pCard == NULL )return;
	
	int nPosY = pCard->getPositionY();
	if (nPosY == CARD_DWON_POSY)
	{
		pCard->setPositionY(CARD_UP_POSY);
	}
	else
	{
		pCard->setPositionY(CARD_DWON_POSY);
	}
}

int XZDDPlayer::getUpCardList(std::vector<BYTE>& kCardDataList)
{
	if (m_iIdex != 0)return 0;

	kCardDataList.clear();
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		
		int nPoy = pSprite->getPositionY();
		if (nPoy == CARD_UP_POSY)
		{
			kCardDataList.push_back(m_kHandCardData[iIdex]);
		}
		iIdex++;
	}
	if ((int)kCardDataList.size()>0)
	{
		return XZDD::CGameLogic::Instance().GetCardColor(kCardDataList[0]);
	}
	else
	{
		return CARD_COLOR_NULL;
	}
}



bool XZDDPlayer::getHuanCards(BYTE nHuanCard[HUAN_CARD_NUM])
{
	std::vector<BYTE> kCardDataList;
	getUpCardList(kCardDataList);

	if (kCardDataList.size() != 3)
	{
		NoticeMsg::Instance().ShowTopMsgByScript("HuanPaiError0");
		return false;
	}

	for (int n=0;n<HUAN_CARD_NUM;n++)
	{
		nHuanCard[n]=kCardDataList[n];
	}
	memcpy(m_pHuanCards,nHuanCard,sizeof(m_pHuanCards));
	return true;
}


int XZDDPlayer::getDefuaultHuanValue(int nIndex)
{
	ASSERT( nIndex>=0 && nIndex<HUAN_CARD_NUM);

	for (;nIndex<(int)m_pHuanCardNodeList.size();nIndex++)
	{
		if (m_pHuanCardNodeList[nIndex]->getPositionY() == CARD_DWON_POSY)
		{
			return getTouchCardVlaue(m_pHuanCardNodeList[nIndex]);
		}
	}
	ASSERT(false);
	return 0;
}
void XZDDPlayer::showCard()
{
	showCard(m_kHandCardData,m_kHandCardCout);
}
void XZDDPlayer::showCard(BYTE* cbCardData,BYTE cbCardCount)
{
	BYTE cbIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pSprite,"HuScale"));
		pSprite->setScale(fHuScale);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pSprite,"MingImagic");
		if (cbIdex < cbCardCount)
		{
			setCardImagic(pSprite,cbCardData[cbIdex],kImagic);
		}
		else
		{
			setCardImagic(pSprite,0,kImagic);
		}
		cbIdex++;
	}
	blackAllHandCard(false);
}

BYTE XZDDPlayer::getGangCard( BYTE currentCard )
{
	ASSERT(m_iIdex == 0);

	BYTE cardIndex[MAX_INDEX]={0};
	XZDD::CGameLogic::Instance().SwitchToCardIndex(m_kHandCardData,m_kHandCardCout,cardIndex);
	if (XZDD::CGameLogic::Instance().IsValidCard(currentCard))
	{
		bool bValue = (bool)XZDD::CGameLogic::Instance().EstimateGangCard(cardIndex,currentCard);
		if (bValue)
		{
			return currentCard;
		}
	}
	XZDD::tagGangCardResult kRes;
	XZDD::CGameLogic::Instance().AnalyseGangCard(cardIndex,m_kWeaveItemArray,m_kWeaveCount,kRes);

	if (kRes.cbCardCount>0)
	{
		return kRes.cbCardData[kRes.cbCardCount-1];
	}
	ASSERT(false);
	return 0;
}

void XZDDPlayer::removeHandOutCard( BYTE cbCardData )
{
	int nSize = m_kOutCardList.size();
	if (nSize <=0 )
	{
		return;
	}
	int i = nSize -1;
	if (m_kOutCardList[i].pCardNode != NULL && m_kOutCardList[i].nCardDarta == cbCardData)
	{
		m_kOutCardList[i].pCardNode->removeFromParent();
		m_kOutCardList[i].pCardNode = NULL;
		m_kOutCardList.pop_back();
	}

}
void XZDDPlayer::showJieSuanCard(cocos2d::Node* pCardNode,XZDD_CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout)
{
	ASSERT(pCardNode);
	pCardNode->removeAllChildren();

	int iIdex = 0;

	cocos2d::Vec2 kStartPos = pCardNode->getPosition();
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(pCardNode,"AddOder"));
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"JieSuanAddPos"));
	cocos2d::Vec2 kJieSuanWeaveGap = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"JieSuanWeaveGap"));

	std::string kPengSkin = utility::toString("XZDD_PENG_",iIdex);
	std::string kGangSkin = utility::toString("XZDD_GANG_",iIdex);
	std::string kHandSkin = utility::toString("XZDD_HAND_",iIdex);
	int iOder = 0;

	for (int i = 0;i<iWeaveCout;i++)
	{
		XZDD_CMD_WeaveItem* pTemp = pWeave+i;
		int iCardValue = pTemp->cbCenterCard;

		if (pTemp->cbWeaveKind == WIK_PENG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3 + kJieSuanWeaveGap;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_GANG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kGangSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3+kJieSuanWeaveGap;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card3"),iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}

	BYTE cbChiHuCard = 0;

	for (int i = 0;i<iHandCout;i++)
	{
		BYTE* pTemp = pHandCard+i;
		int iCardValue = *pTemp;
		if (iCardValue == getChiHuCard())
		{
			cbChiHuCard = getChiHuCard();
			setChiHuCard(0);
			continue;
		}

		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setTag(1);
		pNode->setLocalZOrder(iOder);
		pNode->setPosition(kStartPos);
		{
			kStartPos += kHandAddPos;
		}
		if (iIdex == 0)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"MingImagic");
			setCardImagic(pNode,iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}

	if (cbChiHuCard != 0)
	{
		cocos2d::Vec2 kChiHuPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"JieSuanChiHuPos"));
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setLocalZOrder(iOder);
		pNode->setPosition(kStartPos+kChiHuPos);

		std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"MingImagic");
		setCardImagic(pNode,cbChiHuCard,kImagic);
	}
}
void XZDDPlayer::setChiHuCard(BYTE cbCard)
{
	m_cbChiHuCard = cbCard;
	if (cbCard>0)
	{
		m_kHuCardList.push_back(cbCard);
		showChiHuCard();
	}
}

void XZDDPlayer::clearChiHuCard()
{
	cocos2d::Node* pChiHuNode=	WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHu");
	pChiHuNode->removeAllChildren();
	m_kHuCardList.clear();
}
BYTE XZDDPlayer::getChiHuCard()
{
	return m_cbChiHuCard;
}

void XZDDPlayer::showChiHuCard()
{
	cocos2d::Node* pChiHuNode=	WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHu");
	pChiHuNode->removeAllChildren();

	cocos2d::Vec2 kStartPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pChiHuNode,"StartPos"));
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(pChiHuNode,"AddOder"));
	cocos2d::Vec2 kOutAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pChiHuNode,"OutAddPos"));
	cocos2d::Color3B kColor = utility::parseColour(WidgetFun::getWidgetUserInfo(pChiHuNode,"CardColor"));
	std::string kHuCardSkin = utility::toString("XZDD_OUT_",m_iIdex);

	int iOder = 0;
	for (int i=0;i<(int)m_kHuCardList.size();i++)
	{
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHuCardSkin,pChiHuNode);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");

		setCardImagic(pNode,m_kHuCardList[i],kImagic);
		pNode->setLocalZOrder(iOder);
		pNode->setPosition(kStartPos);
		pNode->setColor(kColor);

		kStartPos += kOutAddPos;
		iOder+=iAddOder;
	}
}

void XZDDPlayer::removeHandCard(BYTE cbCard)
{
	if (m_iIdex == 0)
	{
		XZDD::CGameLogic::Instance().RemoveValueCardOne(m_kHandCardData,m_kHandCardCout,cbCard);
		m_kHandCardCout --;
		XZDD::CGameLogic::Instance().SortCardList(m_kHandCardData,m_kHandCardCout);
	}
	else
	{
		m_kHandCardCout -- ;
	}
	showHandCard();
}