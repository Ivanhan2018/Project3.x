#ifndef __SGLY_GAME_LAYER_H__
#define __SGLY_GAME_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SGLYDefine.h"

USING_NS_CC;
using namespace ui;
using namespace std;

/*
**	@brief	水果乐园——游戏表现层
*/
class SGLYGameLayer: public Layer
{
public:
	SGLYGameLayer();
	~SGLYGameLayer();
	CREATE_FUNC(SGLYGameLayer);

	//	重置
	void reset();
	
	//	设置坐庄状态
	void setApplyState(E_SGLY_ApplyState eState);

	//	获取申请状态
	E_SGLY_ApplyState getApplyState() const;

	/*
	**	@brief	设置某个奖项高亮
	**	@nLotteryID		奖项ID
	**	@bIsHighLight	是否高亮
	*/	
	void setLotteryHighLight(int nLotteryID, bool bIsHighLight);

	/*
	**	@brief	设置仅有1个奖项高亮
	**	@nLotteryID		奖项ID
	**	@bIsHighLight	是否高亮
	*/	
	void setOnlyLotteryHighLight(int nLotteryID, bool bIsHighLight);

	/*
	**	@brief	关闭所有奖项高亮
	*/	
	void closedLotteryHighLight();

	//	切换庄家动画
	void showAniChangeBanker(int nChairID);

	//	展现奖项类型表现
	void showLotteryType(int nLotteryID);

	//	展现GoodLuck类型表现
	void showGoodLuckType(E_SGLY_GoodLuckType eType);

	//	展现猜大小结果
	void showGuessReslt(bool bIsWin);

	//	获取奖项对象
	Sprite* getLottery(int nLotteryID) const;

	//	处理游戏结束阶段逻辑
	void dealGameEnd();

	//	当前庄家椅子ID
	CC_SYNTHESIZE(int, m_wBankerChairID, BankerChairID);

	//	特殊高亮罩容器
	vector<Node*> m_vSpecialMask;
private:
	bool init();

	void initUI();

	//	更变倍数模式
	void changeMultipleMode(E_SGLY_BettingMode eType);

	//	点击下注按钮事件
	void onClickBtnBetEvent(Ref* pSender);

	//	点击退出按钮事件
	void onClickBtnQuitEvent(Ref* pSender);

	//	点击设置按钮事件
	void onClickBtnSettingEvent(Ref* pSender);

	//	点击补充筹码按钮事件
	void onClickBtnSupplyEvent(Ref* pSender);

	//	点击续押按钮事件
	void onClickBtnBetContinueEvent(Ref* pSender);

	//	点击押小（1-7）按钮事件
	void onClickBtnBetSmallEvent(Ref* pSender);

	//	点击押大（8-14）按钮事件
	void onClickBtnBetBigEvent(Ref* pSender);

	//	点击倍数按钮事件
	void onClickBtnMultipleEvent(Ref* pSender);

	//	点击上庄列表按钮事件
	void onClickBtnApplyListEvent(Ref* pSender);

	//	点击申请上庄按钮事件
	void onClickBtnApplyEvent(Ref* pSender);

	//	点击取消申请按钮事件
	void onClickBtnCancelEvent(Ref* pSender);

	//	点击我要下庄按钮事件
	void onClickBtnDownEvent(Ref* pSender);

	//	背景
	Sprite* m_pBG;

	//	奖项图标
	Sprite* m_pIconLottery[SGLY_LOTTERY_COUNT];

	//	奖项高亮
	Sprite* m_pLotteryHighLight[SGLY_LOTTERY_COUNT];

	//	倍数模式
	Button* m_pBtnMultiple[E_SGLY_BettingMode_Null];

	//	退出按钮
	Button* m_pBtnQuit;

	//	设置按钮
	Button* m_pBtnSetting;

	//	上庄列表
	Button* m_pBtnApplyList;

	//	按钮——补充筹码
	Button* m_pBtnSupplyChip;

	//	按钮——续押
	Button* m_pBtnBetContinue;

	//	按钮——申请上庄
	Button* m_pBtnApply;

	//	按钮——取消申请
	Button* m_pBtnCancel;

	//	按钮——我要下庄
	Button* m_pBtnDown;

	//	 奖项提示
	Sprite* m_pLotteryTips;

	//	 GoodLuck提示
	Sprite* m_pGoodLuckTips;

	//	猜大小结果提示
	Sprite* m_pGuessResultTips;

	//	申请状态
	E_SGLY_ApplyState m_eApplyState;

	//	当前押注倍数
	int m_nCurrBetMultiple;
};


#endif  // __SGLY_GAME_LAYER_H__