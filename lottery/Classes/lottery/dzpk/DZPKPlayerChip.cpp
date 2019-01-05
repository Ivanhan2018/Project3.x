#include "DZPKPlayerChip.h"

#define CHIP_MOVE_SPEED 500			//³ïÂëÒÆ¶¯ËÙ¶È

DZPKPlayerChip::DZPKPlayerChip()
{
}

DZPKPlayerChip::~DZPKPlayerChip()
{
}

bool DZPKPlayerChip::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!DZPKChip::init());

		clearChip();

		bRet = true;
	} while (0);

	return bRet;
}

void DZPKPlayerChip::clearChip()
{
	m_llChipValue = 0;
	changeChip(m_llChipValue);
	this->setVisible(false);
}

void DZPKPlayerChip::setChipPosition(Vec2 vBetPos, Vec2 vPlayerPos, Vec2 vChipPoolPos)
{
	m_vBetPos = vBetPos;
	m_vPlayerPos = vPlayerPos;
	m_vChipPotPos = vChipPoolPos;
}

void DZPKPlayerChip::bet(LONGLONG llValue)
{
	m_llChipValue += llValue;
	if (m_llChipValue <= 0)
	{
		return;
	}
	changeChip(m_llChipValue);
	aniBet();
}

void DZPKPlayerChip::collect()
{
	if (m_llChipValue <= 0)
	{
		return;
	}
	aniCollect();
}

void DZPKPlayerChip::distribute(LONGLONG llValue)
{
	m_llChipValue = llValue;
	if (m_llChipValue <= 0)
	{
		return;
	}
	changeChip(m_llChipValue);
	aniDistribute();
}

void DZPKPlayerChip::aniBet()
{
	float fDistance = m_vPlayerPos.distance(m_vBetPos);
	float fTime = fDistance / CHIP_MOVE_SPEED;
	this->setPosition(m_vPlayerPos);
	this->setVisible(true);
	m_pChipNum->setVisible(false);
	auto pMove = EaseExponentialOut::create(MoveTo::create(fTime, m_vBetPos));
	auto pShowChipValue = CallFunc::create([=](){
		m_pChipNum->setVisible(true);
	});
	this->runAction(Sequence::create(pMove,
		pShowChipValue,
		nullptr));
}

void DZPKPlayerChip::aniCollect()
{
	float fDistance = m_vBetPos.distance(m_vChipPotPos);
	float fTime = fDistance / CHIP_MOVE_SPEED;
	this->setPosition(m_vBetPos);
	this->setVisible(true);
	m_pChipNum->setVisible(false);

	auto pMove = EaseExponentialOut::create(MoveTo::create(fTime, m_vChipPotPos));
	auto pClear = CallFunc::create(CC_CALLBACK_0(DZPKPlayerChip::clearChip, this));
	this->runAction(Sequence::create(
		pMove,
		pClear,
		nullptr));
}

void DZPKPlayerChip::aniDistribute()
{
	float fDistance = m_vChipPotPos.distance(m_vPlayerPos);
	float fTime = fDistance / CHIP_MOVE_SPEED;
	this->setPosition(m_vChipPotPos);
	this->setVisible(true);
	m_pChipNum->setVisible(false);

	auto pMove = EaseExponentialOut::create(MoveTo::create(fTime, m_vPlayerPos));
	auto pClear = CallFunc::create(CC_CALLBACK_0(DZPKPlayerChip::clearChip, this));
	this->runAction(Sequence::create(
		pMove,
		pClear,
		nullptr));
}


