#ifndef __SGLY_POPUP_SUPPLY_CHIP_H__
#define __SGLY_POPUP_SUPPLY_CHIP_H__

#include "cocos2d.h"
#include "BaseDialog.h"
#include "ui/CocosGUI.h"
#include "Define.h"

USING_NS_CC;
using namespace ui;
using namespace std;
/*
**	@brief	水果乐园——时钟
*/
class SGLYPopupSupplyChip: public BaseDialog
{
public:
	SGLYPopupSupplyChip();
	~SGLYPopupSupplyChip();
	CREATE_FUNC(SGLYPopupSupplyChip);

	//	设置是否显示弹窗
	void setPopupVisible(bool bIsVisible);
private:
	bool init();
	void initUI();
	void initData();
	void update(float dt);

	//	显示兑换提示
	void showExchangeTips(const LONGLONG llGoldValue);
	
	//	点击筹码100按钮事件
	void onClickBtnChip100Event(Ref* pSender);

	//	点击筹码1000按钮事件
	void onClickBtnChip1000Event(Ref* pSender);

	//	点击筹码10000按钮事件
	void onClickBtnChip10000Event(Ref* pSender);

	//	点击筹码最大按钮事件
	void onClickBtnChipMaxEvent(Ref* pSender);

	//	点击清空按钮事件
	void onClickBtnClearEvent(Ref* pSender);

	//	点击确定按钮事件
	void onClickBtnOKEvent(Ref* pSender);

	//	点击取消按钮事件
	void onClickBtnNOEvent(Ref* pSender);

	//	背景
	Sprite* m_pBG;

	//	筹码按钮——100
	Button* m_pBtnChip100;

	//	筹码按钮——1000
	Button* m_pBtnChip1000;

	//	筹码按钮——10000
	Button* m_pBtnChip10000;

	//	筹码按钮——最大
	Button* m_pBtnChipMax;

	//	按钮——清空
	Button* m_pBtnClear;

	//	按钮——确定
	Button* m_pBtnOK;

	//	按钮——取消
	Button* m_pBtnNO;

	//	文本——声明
	Label* m_pLabDeclaration;

	//	文本——筹码汇率
	Label* m_pLabExchangeRate;

	//	文本——兑换筹码提示
	Label* m_pLabTipsExchangeChip;

	//	文本——剩余金币
	Label* m_pLabSurplusGold;

	//	字符串缓存——声明
	string m_sDeclaration;

	//	字符串缓存——兑换比率格式
	string m_sExchangeRateFormat;

	//	字符串缓存——兑换筹码提示格式
	string m_sTipsExchangeChipFormat;

	//	字符串缓存——剩余金币提示格式
	string m_sTipsSurplusGoldFormat;

	//	玩家筹码数
	LONGLONG m_llPlayerChipValue;

	//	玩家入场时金币数
	LONGLONG m_llPlayerGold;
};


#endif  // __SGLY_POPUP_SUPPLY_CHIP_H__