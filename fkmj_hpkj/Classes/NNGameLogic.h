#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "CMD_Ox.h"
#include "GameDefine.h"
#include "FvSingleton.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//扑克类型
#define OX_VALUE0					0									//混合牌型
#define OX_FOUR_SAME				103									//四条牌型
#define OX_FIVEKING					104									//五花牛
#define OX_FIVESMALL				105									//五小牛

//////////////////////////////////////////////////////////////////////////

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//四张数目
	BYTE 							cbThreeCount;						//三张数目
	BYTE 							cbDoubleCount;						//两张数目
	BYTE							cbSignedCount;						//单张数目
	BYTE 							cbFourLogicVolue[1];				//四张列表
	BYTE 							cbThreeLogicVolue[1];				//三张列表
	BYTE 							cbDoubleLogicVolue[2];				//两张列表
	BYTE 							cbSignedLogicVolue[5];				//单张列表
	BYTE							cbFourCardData[MAX_COUNT];			//四张列表
	BYTE							cbThreeCardData[MAX_COUNT];			//三张列表
	BYTE							cbDoubleCardData[MAX_COUNT];		//两张列表
	BYTE							cbSignedCardData[MAX_COUNT];		//单张数目
};


//游戏逻辑类

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class NNGameLogic
	: public FvSingleton<NNGameLogic>
{
	//变量定义
private:
	static BYTE						m_cbCardListData[52];				//扑克定义

	//函数定义
public:
	//构造函数
	NNGameLogic();
	//析构函数
	virtual ~NNGameLogic();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return (cbCardData&LOGIC_MASK_COLOR)>>4; }
	//获取倍数
	BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);
	//获取牛牛
	bool GetOxCard(BYTE cbCardData[], BYTE cbCardCount);
	//获取整数
	bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//功能函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,bool FirstOX,bool NextOX);
};

//////////////////////////////////////////////////////////////////////////

#endif

