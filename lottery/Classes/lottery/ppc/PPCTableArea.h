#ifndef __PPC_TABLE_AREA_H__
#define __PPC_TABLE_AREA_H__

#include "cocos2d.h"
#include "common/PPCEnum.h"
#include "common/PPCRes.h"
#include "Define.h"

USING_NS_CC;
using namespace std;

//	桌面区域块
class PPCTableArea: public Node
{
public:
	PPCTableArea();
	~PPCTableArea();
	CREATE_FUNC(PPCTableArea);

	//	重置桌面区域块
	void resetTableArea();

	//	初始化区域块
	void initArea(EPPCCarLogoType eArea);

	//	判断一点是否在区域内
	bool isContainsPoint(Vec2 vPos);

	//	添加筹码
	Sprite* addTableChip(EPPCChipType eType);

	//	设置区域是否高亮
	void setAreaHighLight(bool isHighLight);

	//	设置筹码总注文本数值
	void setLabChipValueSum(LONGLONG llValue);

	//	设置筹码个注文本数值
	void setLabChipValueSelf(LONGLONG llValue);

private:
	bool init();
	void initData();

	//	区域图标
	Sprite* m_pIcon;

	//	筹码总注文本
	Label* m_pLabChipValueSum;

	//	筹码个注文本
	Label* m_pLabChipValueSelf;

	//	区域高亮
	Sprite* m_pLight;

	//	区域矩阵
	cocos2d::Rect m_rAreaRect;

	//	区域类型
	EPPCCarLogoType m_eAreaType;

	//	桌面筹码容器
	vector<Node*> m_vVesselChip;

	//	筹码总注数
	CC_SYNTHESIZE(LONGLONG, m_llChipValueSum, ChipValueSum);

	//	筹码个注数
	CC_SYNTHESIZE(LONGLONG, m_llChipValueSelf, ChipValueSelf);
};


#endif  // __PPC_TABLE_AREA_H__
