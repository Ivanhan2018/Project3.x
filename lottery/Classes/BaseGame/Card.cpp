#include "Card.h"

Card::Card() :
	m_pCardObverse(nullptr),
	m_pCardReverse(nullptr)
{
}

Card::~Card()
{
}

//	加载卡牌正面纹理
void Card::loadCardObverse(const string& sFileName)
{
	m_pCardObverse->initWithSpriteFrameName(sFileName);
	setContentSize(m_pCardObverse->getContentSize());
	m_sFileNameObverse = sFileName;
}

//	加载卡牌背面纹理
void Card::loadCardReverse(const string& sFileName)
{
	m_pCardReverse->initWithSpriteFrameName(sFileName);
	setContentSize(m_pCardReverse->getContentSize());
	m_sFileNameReverse = sFileName;
}

void Card::showCardObverse(bool isDoAni)
{
	if (isDoAni)
	{
		doAniOfObverse();
	}
	else
	{
		m_pCardObverse->setVisible(true);
		m_pCardReverse->setVisible(false);
	}
}

void Card::doAniOfObverse()
{
	auto pAnimObverse = Sequence::create(
		DelayTime::create(DZPK_CARD_ANI_TURN_DURATION *0.5),
		Show::create(),
		OrbitCamera::create(DZPK_CARD_ANI_TURN_DURATION *0.5, 1, 0, DZPK_CARD_OBVERSE_ANGLE_Z, DZPK_CARD_OBVERSE_DELTA_Z, 0, 0),
// 		OrbitCamera::create(DZPK_CARD_ANI_TURN_DURATION *0.5, 1, 0, 180, 0, 0, 0),
		NULL);

	auto pAnimReverse = Sequence::create(
		OrbitCamera::create(DZPK_CARD_ANI_TURN_DURATION *0.5, 1, 0, DZPK_CARD_REVERSE_ANGLE_Z, DZPK_CARD_REVERSE_DELTA_Z, 0, 0),
// 		OrbitCamera::create(DZPK_CARD_ANI_TURN_DURATION *0.5, 1, 0, 0, -180, 0, 0),
		Hide::create(),
		DelayTime::create(DZPK_CARD_ANI_TURN_DURATION * 0.5),
		NULL);
	m_pCardObverse->runAction(pAnimObverse);
	m_pCardReverse->runAction(pAnimReverse);
}

//	卡牌重置状态
void Card::reset()
{
	m_pCardObverse->setVisible(false);
	m_pCardReverse->setVisible(true);
// 	m_pCardReverse->setRotationSkewX(0);

	if (m_sFileNameReverse != "")
	{
		m_pCardReverse->removeFromParent();
		m_pCardReverse = Sprite::createWithSpriteFrameName(m_sFileNameReverse);
		addChild(m_pCardReverse);
	}
}

bool Card::init()
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

void Card::initUI()
{
	m_pCardObverse = Sprite::create();
	addChild(m_pCardObverse);
	m_pCardObverse->setVisible(false);

	m_pCardReverse = Sprite::create();
	addChild(m_pCardReverse);
}
