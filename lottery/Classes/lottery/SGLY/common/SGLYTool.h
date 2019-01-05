#ifndef __SGLY_TOOL_H__
#define __SGLY_TOOL_H__

#include "cocos2d.h"
#include "Define.h"

USING_NS_CC;
using namespace std;

/*
**	@brief	水果乐园——工具
*/
class SGLYTool
{
public:
	SGLYTool();
	~SGLYTool();

	/*
	**	@brief		筹码兑换成金币
	**	@llValue	筹码数额
	**	@return		金币
	*/
	static LONGLONG exchangeForGold(LONGLONG llValue);

	/*
	**	@brief		金币兑换成筹码
	**	@llValue	金币数额
	**	@return		筹码
	*/
	static LONGLONG exchangeForChip(LONGLONG llValue);

	static DWORD m_dwExchangeRate;
	static LONGLONG m_llApplyBankerCondition;
private:
	
};


#endif  // __SGLY_TOOL_H__