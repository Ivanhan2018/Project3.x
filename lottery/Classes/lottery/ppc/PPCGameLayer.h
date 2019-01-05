#ifndef __PPC_GAME_LAYER_H__
#define __PPC_GAME_LAYER_H__

#include "cocos2d.h"
#include "PPCCarLogo.h"
#include "PPCProtocol.h"
#include "PPCTravel.h"
#include "ui/CocosGUI.h"
#include "PPCPopupApplyList.h"

using namespace ui;

//	游戏主场景层
class PPCGameLayer: public Layer
{
public:
	PPCGameLayer();
	~PPCGameLayer();
	CREATE_FUNC(PPCGameLayer);

	//	重置
	void resetGameLayer();

	//	重置汽车logo
	void resetCarLogo();

	//	设置坐庄状态
	void setApplyState(EPPCApplyState eState);

	//	获取当前坐庄状态
	EPPCApplyState getApplyState();

	/*
	**	@brief		只显示单个汽车logo高亮；
					可以设置是否同时显示汽车，默认高亮时不显示汽车；
	**	@nIdx		高亮logo的索引
	**	@bShowCar	true：显示汽车；false：隐藏汽车；默认为false
	*/
	void showSingleCarLogoHighLight(int nIdx, bool bShowCar = false);

	//	切换庄家动画
	void showAniChangeBanker(int nChairID);

	//	汽车logo列表
	map<int, PPCCarLogo*> m_mCarLogoList;

	//	路单
	PPCTravel* m_pTravel;

public:
	//添加游戏记录
	void addGameRecord(EPPCCarLogoType eType);

private:
	bool init();

	void initUI();

	//	初始化汽车logo
	void initCarLogo();

	//	点击退出事件
	void onClickBtnQuit();

	//	点击设置按钮
	void onClickBtnSetting();

	//	点击上庄列表事件
	void onClickBtnApplyList();

	//	点击申请上庄事件
	void onClickBtnApply();

	//	点击取消申请事件
	void onClickBtnCancelApply();

	//	点击我要下庄事件
	void onClickBtnDown();

	//	按钮——退出
	Button* m_pBtnQuit;

	//	按钮——上庄列表
	Button* m_pBtnApplyList;

	//	按钮——申请上庄
	Button* m_pBtnApply;

	//	按钮——取消申请
	Button* m_pBtnCancelApply;

	//	按钮——我要下庄
	Button* m_pBtnDown;

	//	碰碰车背景
	Sprite* m_pPPCBG;

	//	申请上庄状态
	EPPCApplyState m_eApplyState;

};

#endif  // __PPC_GAME_LAYER_H__