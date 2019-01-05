#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE
#include"CMD_DZPK.h"

// 德州扑克牌逻辑
class DZPKGameLogic
{
	//函数定义
public:
	//构造函数
	DZPKGameLogic();
	//析构函数
	virtual ~DZPKGameLogic();

	//变量定义
private:
	static BYTE m_cbCardData[DZPK_FULL_COUNT];				//扑克定义

	//控制函数
public:
	//排列扑克
	static void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//混乱扑克
	static void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//类型函数
public:
	//获取类型
	static BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//获取数值
	static BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	static BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
	//查找扑克
	static BYTE GetSameCard(const BYTE bCardData[],const BYTE bMaxCard[],BYTE bCardCount,BYTE bMaxCardCount,BYTE bResultData[]);

	//功能函数
public:
	//逻辑数值
	static BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	static BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
	//分析扑克
	static void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult_dzpk & AnalyseResult);
	//7返5
	static BYTE FiveFromSeven(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbCenterCardData[],BYTE cbCenterCardCount,BYTE cbLastCardData[],BYTE cbLastCardCount);
	//查找最大
	static bool SelectMaxUser(BYTE bCardData[DZPKGAME_PLAYER][MAX_CENTERCOUNT],UserWinList &EndResult,const LONGLONG lAddScore[]);
};

//////////////////////////////////////////////////////////////////////////

#endif
