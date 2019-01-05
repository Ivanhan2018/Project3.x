#ifndef __BRNN_GAME_LOGIC_H__
#define __BRNN_GAME_LOGIC_H__

#include "cocos2d.h"
#include "BRNNDefine.h"
#include "Define.h"

//////////////////////////////////////////////////////////////////////////
		
//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//排序类型
#define	ST_VALUE					1									//数值排序
#define	ST_NEW					    3									//数值排序
#define	ST_LOGIC					2									//逻辑排序

//扑克数目
#define CARD_COUNT					54									//扑克数目

//////////////////////////////////////////////////////////////////////////


/*
**	@brief	百人牛牛——游戏逻辑
*/
class BRNNGameLogic
{
	//变量定义
public:

	//函数定义
public:
	//构造函数
	BRNNGameLogic();
	//析构函数
	virtual ~BRNNGameLogic();

	//类型函数
public:
	//获取数值
	static BYTE GetCardValue(BYTE cbCardData) 
	{ 
		return cbCardData&LOGIC_MASK_VALUE; 
	}

	//获取花色
	static BYTE GetCardColor(BYTE cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//控制函数
public:
	//排列扑克
	static void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//	转换对应牌型
	static E_BRNN_CardType RetType(int itype);

	//逻辑函数
public:
	//获取牌点
	static BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

	//获取牌型
	static E_BRNN_CardType GetCardType(const BYTE cbCardData[], BYTE cbCardCount,BYTE bcOutCadData[] = NULL);

	//大小比较
	static int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,BYTE &Multiple);

	//逻辑大小
	static BYTE GetCardLogicValue(BYTE cbCardData);

	static BYTE GetCardNewValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif	//	__BRNN_GAME_LOGIC_H__
