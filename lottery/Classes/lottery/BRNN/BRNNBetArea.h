#ifndef __BRNN_BET_AREA_H__
#define __BRNN_BET_AREA_H__

#include "cocos2d.h"
#include "BRNNDefine.h"
#include "Define.h"

USING_NS_CC;
using namespace std;

/*
**	@brief	百人牛牛——下注区域块
*/
class BRNNBetArea: public Node
{
public:
	BRNNBetArea();
	~BRNNBetArea();
	CREATE_FUNC(BRNNBetArea);

	//	重置下注区域块
	void resetTableArea();

	//	初始化区域块
	void initArea(E_BRNNBetAreaType eArea);

	//	判断一点是否在区域内
	bool isContainsPoint(Vec2 vPos);

	//	添加筹码
	Sprite* addTableChip(E_BRNNChipType eType);

	//	设置区域是否高亮
	void setAreaHighLight(bool isHighLight);

	//	设置筹码总注文本数值
	void setLabChipValueSum(LONGLONG llValue);

	//	设置筹码个注文本数值
	void setLabChipValueSelf(LONGLONG llValue);

private:
	bool init();

	//	区域背景
	Sprite* m_pBG;

	//	区域高亮
	Sprite* m_pLight;

	//	筹码个注文本底纹
	Sprite* m_pChipValueSelfFloor;

	//	筹码总注文本
	Label* m_pLabChipValueSum;

	//	筹码个注文本
	Label* m_pLabChipValueSelf;

	//	区域矩阵
	cocos2d::Rect m_rAreaRect;

	//	区域类型
	E_BRNNBetAreaType m_eAreaType;

	//	桌面筹码容器
	Node* m_pVesselChip;

	//	筹码总注数
	CC_SYNTHESIZE(LONGLONG, m_llChipValueSum, ChipValueSum);

	//	筹码个注数
	CC_SYNTHESIZE(LONGLONG, m_llChipValueSelf, ChipValueSelf);
};


#endif  // __BRNN_BET_AREA_H__
