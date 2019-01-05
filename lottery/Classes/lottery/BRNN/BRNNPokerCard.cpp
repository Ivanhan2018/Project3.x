#include "BRNNPokerCard.h"
#include "BRNNRes.h"
#include "ToolKit.h"


/****************************BRNNPokerCard	begin********************************************/

BRNNPokerCard::BRNNPokerCard()
{
}

BRNNPokerCard::~BRNNPokerCard()
{
}

void BRNNPokerCard::initCard(const int nCardIdx)
{
	string sObverseFileName = getCardFileName(nCardIdx);
	string sReverseFileName = getCardFileName(0);
	loadCardObverse(sObverseFileName);
	loadCardReverse(sReverseFileName);
	m_pCardObverse->setScale(BRNN_CARD_SCALE_BIG);
	m_pCardReverse->setScale(BRNN_CARD_SCALE_BIG);
}

bool BRNNPokerCard::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Card::init());

		bRet = true;
	} while (0);

	return bRet;
}

string BRNNPokerCard::getCardFileName(const int nCardIdx)
{
	if (nCardIdx < 0 && nCardIdx > BRNN_POKER_CARD_COUNT)
	{
		return "";
	}
	char cBuf[128];
	sprintf(cBuf, BRNN_POKER_CARD_FORMAT, nCardIdx);

	return cBuf;
}

/****************************BRNNPokerCard	end********************************************/


/****************************BRNNHandsCard	begin********************************************/


BRNNHandsCard::BRNNHandsCard():
	m_eCardType(E_BRNN_CardType_Error)
{
	memset(m_cbCardData, 0, sizeof(m_cbCardData));
}

BRNNHandsCard::~BRNNHandsCard()
{
}

void BRNNHandsCard::resetHandsCard()
{
	for (int i = 0; i < BRNN_HANDS_CARD_COUNT; i++)
	{
		m_pPokerCard[i]->reset();
		m_pPokerCard[i]->setVisible(false);
	}
	m_pCardTypeFloor->setVisible(false);
	m_pPokerCard[0]->setPositionY(0);
	m_pPokerCard[1]->setPositionY(0);
	memset(m_cbCardData, 0, sizeof(m_cbCardData));
	m_eCardType = E_BRNN_CardType_Error;
}

//	初始化单张手牌
void BRNNHandsCard::initHandsCard(int nIdx, const BYTE sCardData)
{
	m_pPokerCard[nIdx]->initCard(sCardData);
	m_cbCardData[nIdx] = sCardData;
}

//	初始化整副手牌
void BRNNHandsCard::initHandsCard(const BYTE sCardData[])
{
	for (int i = 0; i < BRNN_HANDS_CARD_COUNT; i++)
	{
		m_pPokerCard[i]->initCard(sCardData[i]);
	}
	memcpy(m_cbCardData, sCardData, sizeof(sCardData));
}

//	展现手牌类型
void BRNNHandsCard::showCardType(E_BRNN_CardType eCardType)
{
	switch (eCardType)
	{
	case E_BRNN_CardType_Error:
		return;
	case E_BRNN_CardType_Point:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_meiniu);
		break;
	case E_BRNN_CardType_Niu1:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niu1);
		break;
	case E_BRNN_CardType_Niu2:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niu2);
		break;
	case E_BRNN_CardType_Niu3:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niu3);
		break;
	case E_BRNN_CardType_Niu4:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niu4);
		break;
	case E_BRNN_CardType_Niu5:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niu5);
		break;
	case E_BRNN_CardType_Niu6:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niu6);
		break;
	case E_BRNN_CardType_Niu7:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niu7);
		break;
	case E_BRNN_CardType_Niu8:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niu8);
		break;
	case E_BRNN_CardType_Niu9:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niu9);
		break;
	case E_BRNN_CardType_NiuNiu:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niuniu);
		break;
	case E_BRNN_CardType_SmallNiu:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niuniu);
		break;
	case E_BRNN_CardType_BigNiu:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_niuniu);
		break;
	case E_BRNN_CardType_SilverNiu:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_silverniu);
		break;
	case E_BRNN_CardType_GoldNiu:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_goldniu);
		break;
	case E_BRNN_CardType_Bomb:
		m_pCardType->initWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_bomb);
		break;
	default:
		return;
	}
	m_pCardTypeFloor->setVisible(true);
}

//	醒目展示手牌
void BRNNHandsCard::showCardMarkedness()
{
	m_pPokerCard[0]->setPositionY(40);
	m_pPokerCard[1]->setPositionY(40);
}

bool BRNNHandsCard::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void BRNNHandsCard::initUI()
{
	for (int i = 0, j = 2; i < BRNN_HANDS_CARD_COUNT; i++)
	{
		m_pPokerCard[i] = BRNNPokerCard::create();
		addChild(m_pPokerCard[i]);
		m_pPokerCard[i]->initCard(0);
		m_pPokerCard[i]->setPosition(Vec2(- (j-- * 40), 0));
		m_pPokerCard[i]->setVisible(false);
	}
	setContentSize(m_pPokerCard[0]->getContentSize());
	m_pCardTypeFloor = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_card_type_floor);
	addChild(m_pCardTypeFloor);
	m_pCardTypeFloor->setPosition(Vec2(0, m_pPokerCard[0]->getContentSize().height * -0.35f));
	m_pCardTypeFloor->setVisible(false);

	m_pCardType = Sprite::create();
	m_pCardTypeFloor->addChild(m_pCardType);
	m_pCardType->setPosition(Vec2(m_pCardTypeFloor->getContentSize() / 2));
}


/****************************BRNNHandsCard	end********************************************/
