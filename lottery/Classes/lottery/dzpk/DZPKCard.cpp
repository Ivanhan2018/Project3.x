#include "DZPKCard.h"

/**************************DZPKCard	Begin**********************************************/

DZPKCard::DZPKCard()
{
	m_pCardObverse = nullptr;
	m_pCardReverse = nullptr;
	m_cCardData = 0;
}

DZPKCard::~DZPKCard()
{
}

bool DZPKCard::init()
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

void DZPKCard::initUI()
{
	m_pCardObverse = Sprite::create();
	addChild(m_pCardObverse);
	m_pCardObverse->setVisible(false);

	m_pCardReverse = getCard(0);
	addChild(m_pCardReverse);

	m_pCardHighLight = Sprite::createWithSpriteFrameName(texture_name::s_room_card_frame);
	m_pCardObverse->addChild(m_pCardHighLight);
	m_pCardHighLight->setVisible(false);
}

void DZPKCard::setCardObverse(unsigned char cCardData)
{
	Sprite* pTmp = getCard(cCardData);
// 	m_pCardObverse->initWithTexture(pTmp->getTexture(), pTmp->getTextureRect());
	m_pCardObverse->setTexture(pTmp->getTexture());
	m_pCardObverse->setTextureRect(pTmp->getTextureRect());

	m_cCardData = cCardData;
}

Sprite* DZPKCard::getCard(unsigned char cCardData)
{
	Sprite* pCard = NULL;
// 	pCard->setTexture("Card_All.png");
	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("Card_All.png");

	if (cCardData == 0x00)  
	{
// 		pCard->initWithTexture(pCard->getTexture(), CCRectMake(0, 528, 96, 132));
		pCard = Sprite::createWithTexture(texture, CCRectMake(0, 528, 96, 132));
	}
	else
	{
		int row = cCardData%16;
		int col = cCardData/16+1;
// 		pCard->initWithTexture(pCard->getTexture(), CCRectMake((row-1) * 96+ 1, 132 * (col-1), 96,132));
		pCard = Sprite::createWithTexture(texture, CCRectMake((row-1) * 96+ 1, 132 * (col-1), 96,132));
	}

	return pCard;
}

void DZPKCard::animationTurnCard()
{
	auto pAnimObverse = Sequence::create(
		DelayTime::create(DZPK_CARD_ANI_TURN_DURATION *0.5),
		Show::create(),
		OrbitCamera::create(DZPK_CARD_ANI_TURN_DURATION *0.5, 1, 0, DZPK_CARD_OBVERSE_ANGLE_Z, DZPK_CARD_OBVERSE_DELTA_Z, 0, 0),NULL);

	auto pAnimReverse = Sequence::create(
		OrbitCamera::create(DZPK_CARD_ANI_TURN_DURATION *0.5, 1, 0, DZPK_CARD_REVERSE_ANGLE_Z, DZPK_CARD_REVERSE_DELTA_Z, 0, 0),
		Hide::create(),
		DelayTime::create(DZPK_CARD_ANI_TURN_DURATION * 0.5),NULL);

	m_pCardObverse->runAction(pAnimObverse);
	m_pCardReverse->runAction(pAnimReverse);
}

//	卡牌变灰暗
void DZPKCard::showCardDark()
{
	m_pCardObverse->setColor(Color3B::GRAY);
}

//	卡牌变高亮
void DZPKCard::showCardHighLight()
{
	Color3B cNorColor = m_pCardReverse->getColor();
	m_pCardObverse->setColor(cNorColor);
	m_pCardHighLight->setPosition(Vec2(m_pCardObverse->getContentSize() / 2));
	m_pCardHighLight->setVisible(true);
	Vec2 vOffset = Vec2(0, 15);
	this->setPosition(Vec2(m_vNorPosition + vOffset));
}

//	卡牌重置状态
void DZPKCard::resetCard()
{
	Color3B cNorColor = m_pCardReverse->getColor();
	m_pCardObverse->setColor(cNorColor);
	m_pCardHighLight->setVisible(false);
	m_cCardData = 0;
	m_pCardObverse->setVisible(false);
	m_pCardReverse->removeFromParent();
	m_pCardReverse = getCard(0);
	addChild(m_pCardReverse);
	this->setPosition(m_vNorPosition);
}

char DZPKCard::getCardData() const
{
	return m_cCardData;
}

void DZPKCard::setNorPos(Vec2 vPosition)
{
	m_vNorPosition = vPosition;
}
/**************************DZPKCard	End**********************************************/



/**************************DZPKHandCard	Begin**********************************************/

DZPKHandCard::DZPKHandCard()
{
}

DZPKHandCard::~DZPKHandCard()
{
}

bool DZPKHandCard::init()
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

void DZPKHandCard::initUI()
{
	m_pImgHandCard[EDZPKCardID_1] = DZPKCard::create();
	addChild(m_pImgHandCard[EDZPKCardID_1]);
	m_pImgHandCard[EDZPKCardID_1]->setPosition(Vec2(-10, 0));
	m_pImgHandCard[EDZPKCardID_1]->setVisible(false);
	m_pImgHandCard[EDZPKCardID_1]->setNorPos(Vec2(-10, 0));


	m_pImgHandCard[EDZPKCardID_2] = DZPKCard::create();
	addChild(m_pImgHandCard[EDZPKCardID_2]);
	m_pImgHandCard[EDZPKCardID_2]->setPosition(Vec2(10, 0));
	m_pImgHandCard[EDZPKCardID_2]->setVisible(false);
	m_pImgHandCard[EDZPKCardID_2]->setNorPos(Vec2(10, 0));
}

//	设置手牌
void DZPKHandCard::setHandCardImg(int nCardID, unsigned char cCardData)
{
	if (nCardID < 0 || nCardID > 1)
	{
		//	玩家手牌ID只有0和1
		return;
	}
	m_pImgHandCard[nCardID]->setCardObverse(cCardData);

}

void DZPKHandCard::aniTurnCard(int nCardID)
{
	int nAngle = nCardID == EDZPKCardID_1 ? -15 : 15;
	m_pImgHandCard[nCardID]->setVisible(true);
	m_pImgHandCard[nCardID]->animationTurnCard();
	m_pImgHandCard[nCardID]->setRotation(nAngle);
}

void DZPKHandCard::setCardVisible(int nCardID, bool bIsVisible)
{
	m_pImgHandCard[nCardID]->setVisible(bIsVisible);
}

char DZPKHandCard::getHandCardData(int nCardID) const
{
	return m_pImgHandCard[nCardID]->getCardData();
}

void DZPKHandCard::resetHandCard()
{
	setCardVisible(EDZPKCardID_1, false);
	m_pImgHandCard[EDZPKCardID_1]->setRotation(0);
	m_pImgHandCard[EDZPKCardID_1]->resetCard();

	setCardVisible(EDZPKCardID_2, false);
	m_pImgHandCard[EDZPKCardID_2]->setRotation(0);
	m_pImgHandCard[EDZPKCardID_2]->resetCard();
}

/**************************DZPKHandCard	End**********************************************/


/**************************DZPKCommunityCards	Begin**********************************************/


DZPKCommunityCards::DZPKCommunityCards()
{
}

DZPKCommunityCards::~DZPKCommunityCards()
{
}

bool DZPKCommunityCards::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		initUI();
		resetCommunityCards();

		bRet = true;
	} while (0);

	return bRet;
}

void DZPKCommunityCards::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	for (int i = 0, j = 2; i < MAX_CENTERCOUNT; i++)
	{
		m_pImgComunityCard[i] = DZPKCard::create();
		addChild(m_pImgComunityCard[i]);
		m_pImgComunityCard[i]->setPosition(Vec2(- (j-- * 100), 0));
		m_pImgComunityCard[i]->setNorPos(m_pImgComunityCard[i]->getPosition());
	}
}

//	设置公共牌
void DZPKCommunityCards::setCommunityCardImg(int nCardID, unsigned char cCardData)
{
	if (nCardID < 0 || nCardID > 5)
	{
		//	玩家公共牌ID只有0至4
		return;
	}
	m_cComunityCardData[nCardID] = cCardData;
	m_pImgComunityCard[nCardID]->setCardObverse(cCardData);
}

void DZPKCommunityCards::resetCommunityCards()
{
	memset(m_cComunityCardData, 0, sizeof(m_cComunityCardData));
	for (int i = 0, j = 2; i < MAX_CENTERCOUNT; i++)
	{
		m_pImgComunityCard[i]->setVisible(false);
		m_pImgComunityCard[i]->resetCard();
	}
}

char* DZPKCommunityCards::getComunityCardData()
{
	return m_cComunityCardData;
}

//	获取公共牌图像
DZPKCard* DZPKCommunityCards::getCommunityCardImg(int nCardID) const
{
	return m_pImgComunityCard[nCardID];
}

//	显示前三张公共牌
void DZPKCommunityCards::showFirstThreeCard(char cCardData[])
{
	for (int i = 0; i < EDZPKCardID_4; i++)
	{
		m_cComunityCardData[i] = cCardData[i];
		setCommunityCardImg(i, cCardData[i]);
		m_pImgComunityCard[i]->setVisible(true);
		m_pImgComunityCard[i]->animationTurnCard();
	}
}

//	显示第四张公共牌
void DZPKCommunityCards::showFourthCard(char cCardData)
{
	m_cComunityCardData[EDZPKCardID_4] = cCardData;
	setCommunityCardImg(EDZPKCardID_4, cCardData);
	m_pImgComunityCard[EDZPKCardID_4]->setVisible(true);
	m_pImgComunityCard[EDZPKCardID_4]->animationTurnCard();
}

//	显示第五张公共牌
void DZPKCommunityCards::showFifthCard(char cCardData)
{
	m_cComunityCardData[EDZPKCardID_5] = cCardData;
	setCommunityCardImg(EDZPKCardID_5, cCardData);
	m_pImgComunityCard[EDZPKCardID_5]->setVisible(true);
	m_pImgComunityCard[EDZPKCardID_5]->animationTurnCard();
}

void DZPKCommunityCards::showCommunityCard(char cCardData[])
{
	for (int i = 0; i < MAX_CENTERCOUNT; i++)
	{
		if (m_cComunityCardData[i] == cCardData[i])
		{
			continue;
		}

		setCommunityCardImg(i, cCardData[i]);
		m_pImgComunityCard[i]->setVisible(true);
		m_pImgComunityCard[i]->animationTurnCard();
	}
}

/**************************DZPKCommunityCards	End**********************************************/
