#include "DZPKChipPot.h"
#include "DDZRes.h"
#include "DZPKTools.h"

DZPKChipPot::DZPKChipPot()
{
}

DZPKChipPot::~DZPKChipPot()
{
}

bool DZPKChipPot::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		initData();
		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void DZPKChipPot::initUI()
{
	m_pPotBG = Sprite::createWithSpriteFrameName(texture_name::s_desk_num_frame);
	addChild(m_pPotBG);

	setContentSize(m_pPotBG->getContentSize());
	cocos2d::Size sPotSize = getContentSize();

	m_pLabChipPot = Label::create();
	m_pPotBG->addChild(m_pLabChipPot);
	m_pLabChipPot->setPosition(Vec2(0, sPotSize.height * 1.3));
	m_pLabChipPot->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabChipPot->setColor(Color3B::WHITE);
	m_pLabChipPot->setSystemFontSize(15);
	setLabChipPot(m_llChipPotValue);

	m_pChip = DZPKChip::create();
	m_pPotBG->addChild(m_pChip);
	m_pChip->setPosition(Vec2(sPotSize.width * 0.2, sPotSize.height * 0.4));
}

void DZPKChipPot::initData()
{
	m_llChipPotValue = 0;
}

void DZPKChipPot::setLabChipPot(LONGLONG llValue)
{
	char cBuf[64];
	sprintf(cBuf, DZPK_loadText("t9"), llValue);
	m_pLabChipPot->setString(cBuf);
}

//	重置筹码池
void DZPKChipPot::restPot()
{
	m_llChipPotValue = 0;
	m_pChip->changeChip(m_llChipPotValue);
	setLabChipPot(m_llChipPotValue);
	m_pChip->setVisible(false);
}

//	更新筹码池
void DZPKChipPot::updatePot(LONGLONG llValue)
{
	m_llChipPotValue = llValue;
	if (m_llChipPotValue <= 0)
	{
		return;
	}
	setLabChipPot(llValue);
	m_pChip->setVisible(true);
	m_pChip->changeChip(m_llChipPotValue);
}
