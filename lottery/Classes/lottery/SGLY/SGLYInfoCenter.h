#ifndef __SGLY_INFO_CENTER_H__
#define __SGLY_INFO_CENTER_H__

#include "cocos2d.h"
#include "SGLYDefine.h"
#include "Define.h"

USING_NS_CC;

/*
**	@brief	水果乐园——信息栏
*/
class SGLYInfoCenter: public Layer
{
public:
	SGLYInfoCenter();
	~SGLYInfoCenter();
	CREATE_FUNC(SGLYInfoCenter);

	//	重置
	void reset();

	/*
	**	@brief				更新庄家信息
	**	@wBankerUser		当前庄家
	**	@szBankerNickName	庄家昵称
	**	@lBankerChip		庄家筹码
	**	@lBankerWinScore	庄家成绩
	**	@cbBankerTime		庄家局数
	**	@bSystemBanker		系统做庄
	*/
	void updateBankerInfo(WORD wBankerUser, TCHART szBankerNickName[], LONGLONG lBankerChip, LONGLONG lBankerWinScore, WORD cbBankerTime, bool bSystemBanker = true);

	//	更新个注
	void updateSelfBet(E_SGLY_Lottery eType, const LONGLONG sValue);

	//	更新总注
	void updateTotalBet(E_SGLY_Lottery eType, const LONGLONG sValue);

	//	更新信息栏
	void updateInfoBar(E_SGLY_InfoBar eType, const LONGLONG sValue);

	//	设置信息栏数值
	void setValueInfoBar(E_SGLY_InfoBar eType, const LONGLONG sValue);

	//	获得信息栏数值
	LONGLONG getValueInfoBar(E_SGLY_InfoBar eType) const;

	//	设置个注数值
	void setValueSelfBet(E_SGLY_Lottery eType, const LONGLONG sValue);

	//	获得个注数值
	LONGLONG getValueSelfBet(E_SGLY_Lottery eType) const;

	//	设置总注数值
	void setValueTotalBet(E_SGLY_Lottery eType, const LONGLONG sValue);

	//	获得总注数值
	LONGLONG getValueTotalBet(E_SGLY_Lottery eType) const;

	//	设置续押数值
	void setValueContinueBet(E_SGLY_Lottery eType, const LONGLONG sValue);

	//	获得续押数值
	LONGLONG getValueContinueBet(E_SGLY_Lottery eType) const;

	//	是否重写续押数值
	CC_SYNTHESIZE(bool, m_bOverwrite, Overwrite);
private:
	bool init();

	void initUI();

	void initData();

	//	数值——无人坐庄
	string m_sNoneBankerName;

	//	文本——庄家昵称
	Label* m_pLabBankerNickname;

	//	数值——信息栏
	LONGLONG m_sValueInfoBar[E_SGLY_InfoBar_Null];

	//	文本——信息栏
	Label* m_pLabInfoBar[E_SGLY_InfoBar_Null];

	//	数值——个注
	LONGLONG m_sValueSelfBet[E_SGLY_Lottery_Null];

	//	文本——个注
	Label* m_pLabSelfBet[E_SGLY_Lottery_Null];

	//	数值——总注
	LONGLONG m_sValueTotalBet[E_SGLY_Lottery_Null];

	//	文本——总注
	Label* m_pLabTotalBet[E_SGLY_Lottery_Null];

	//	数值——续押
	LONGLONG m_sValueContinueBet[E_SGLY_Lottery_Null];
	LONGLONG m_llNoneBankerChip;
};


#endif  // __SGLY_INFO_CENTER_H__