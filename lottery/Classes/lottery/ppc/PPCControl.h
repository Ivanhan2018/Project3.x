#ifndef __PPC_CONTROL_H__
#define __PPC_CONTROL_H__

#include "cocos2d.h"
#include "PPCRes.h"
#include "PPCTableArea.h"
#include "Define.h"
#include "ui/CocosGUI.h"
#include "Define.h"

USING_NS_CC;
using namespace ui;

//	碰碰车——控制台
class PPCControl: public Layer
{
public:
	PPCControl();
	~PPCControl();
	CREATE_FUNC(PPCControl);
	
	//	重置
	void resetPPCControl();

	/*
	**	@brief		下注
	**	@eAreaType	手机端定义的区域类型
	**	@sValue		筹码面值
	*/
	void bet(EPPCCarLogoType eAreaType, SCORE sValue);

	//	下注动画
	void showAniBet(EPPCCarLogoType eType);

	//	显示区域高亮
	void showAreaHighLight(EPPCCarLogoType eCarLogoType);

	/*
	**	@brief		获取区域块
	**	@eType		手机端定义的区域类型
	**	@return		返回指定区域块指针
	*/
	PPCTableArea* getTableArea(EPPCCarLogoType eType);

	/*
	当前区域已经下注总数，分成每个面值的筹码进行展示
	*/
	void betAll(EPPCCarLogoType eAreaType, SCORE sValue);

	//	设置筹码按钮是否启用使能
	void setBtnChipEnable(EPPCChipType eType, bool bEnabled);

	//	设置所有筹码按钮是否启用使能
	void setAllBtnChipEnable(bool bEnabled);

	//	检出筹码按钮使能
	void checkoutEnabledBtnChip(SCORE sValue);

	//	是否中奖
	bool isWinTheLotterys(EPPCCarLogoType eLotteryType);

private:
	bool init();
	void initData();
	void initUI();

	//	点击筹码事件
	void onClickChip(Ref* pSender);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	//	桌面区域块
	PPCTableArea* m_pTableArea[EPPCCarLogo_Null];

	//	筹码按钮高亮光环
	Sprite* m_pChipHighLight[EPPCChip_Null];

	//	筹码
	Button* m_pBtnChip[EPPCChip_Null];

	//	当前选中筹码类型
	EPPCChipType m_eCurrChipType;

};


#endif  // __PPC_CONTROL_H__
