#ifndef __DZPK_CHIP_POT_H__
#define __DZPK_CHIP_POT_H__

#include "cocos2d.h"
#include "DZPKChip.h"

USING_NS_CC;

//	德州扑克——筹码池
class DZPKChipPot: public Node
{
public:
	DZPKChipPot();
	~DZPKChipPot();
	CREATE_FUNC(DZPKChipPot);
	bool init();
	
	//	重置筹码池
	void restPot();

	//	更新筹码池
	void updatePot(LONGLONG llValue);

private:
	void initData();
	void initUI();

	//	设置筹码池文本
	void setLabChipPot(LONGLONG llValue);

	//	筹码池背景
	Sprite* m_pPotBG;

	//	筹码
	DZPKChip* m_pChip;

	//	筹码池数值
	LONGLONG m_llChipPotValue;

	//	筹码池文本
	Label* m_pLabChipPot;

	

};


#endif	//	__DZPK_CHIP_POT_H__