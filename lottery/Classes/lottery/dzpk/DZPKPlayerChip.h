#ifndef __DZPK_PLAYER_CHIP_H__
#define __DZPK_PLAYER_CHIP_H__

#include "cocos2d.h"
#include "DZPKChip.h"

USING_NS_CC;

class DZPKPlayerChip: public DZPKChip
{
public:
	DZPKPlayerChip();
	~DZPKPlayerChip();
	CREATE_FUNC(DZPKPlayerChip);
	bool init();

	/*
	**	@brief	清除筹码
	*/
	void clearChip();

	/*
	**	@brief	设置筹码位置
	**	@vBetPos	下注时筹码所在位置
	**	@vPlayerPos	玩家筹码初始位置
	**	@vChipPoolPos	筹码池位置
	*/
	void setChipPosition(Vec2 vBetPos, Vec2 vPlayerPos, Vec2 vChipPoolPos);

	/*
	**	@brief	下注
	*/
	void bet(LONGLONG llValue);

	/*
	**	@brief	汇总
	*/
	void collect();

	/*
	**	@brief	分发
	*/
	void distribute(LONGLONG llValue);

	/*
	**	@brief	下注动画
	*/
	void aniBet();

	/*
	**	@brief	汇总动画
	*/
	void aniCollect();

	/*
	**	@brief	分发动画
	*/
	void aniDistribute();

private:

	/*
	**	@brief	筹码数值
	*/
	LONGLONG m_llChipValue;

	/*
	**	@brief	下注时筹码所在位置
	*/
	Vec2 m_vBetPos;

	/*
	**	@brief	玩家筹码初始位置
	*/
	Vec2 m_vPlayerPos;

	/*
	**	@brief	筹码池位置
	*/
	Vec2 m_vChipPotPos;

};



#endif  // __DZPK_PLAYER_CHIP_H__