#ifndef __BRNN_GAME_LAYER_H__
#define __BRNN_GAME_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "BRNNBetArea.h"
#include "BRNNDefine.h"
#include "BRNNRes.h"
#include "Define.h"
#include "BRNNPokerCard.h"
#include "BRNNPopupApplyList.h"

USING_NS_CC;
using namespace std;
using namespace ui;

/*
**	@brief	百人牛牛——游戏表现层
*/
class BRNNGameLayer: public Layer
{
public:
	BRNNGameLayer();
	~BRNNGameLayer();
	CREATE_FUNC(BRNNGameLayer);

	//	重置
	void resetGameLayer();
	
	/*
	**	@brief			发牌动画
	**	@nIdx			发牌起始索引
	**	@sCardData		卡牌数据
	**	@return			返回发牌动画总时间
	*/
	float doAniSendCard(const int nIdx, const BYTE sCardData[][5]);

	/*
	**	@brief		下注
	**	@eAreaType	手机端定义的区域类型
	**	@sValue		筹码面值
	*/
	void bet(E_BRNNBetAreaType eAreaType, SCORE sValue);

	/*
	**	@brief		获取区域块
	**	@eType	手机端定义的区域类型
	**	@return		返回指定区域块指针
	*/
	BRNNBetArea* getTableArea(E_BRNNBetAreaType eType);

	//	分配筹码
	void allocationChip(E_BRNNBetAreaType eAreaType, SCORE sValue);

	//	切换庄家动画
	void showAniChangeBanker(int nChairID);

	//	设置筹码按钮是否启用使能
	void setBtnChipEnable(E_BRNNChipType eType, bool bEnabled);

	//	设置所有筹码按钮是否启用使能
	void setAllBtnChipEnable(bool bEnabled);

	//	检出筹码按钮使能
	void checkoutEnabledBtnChip(SCORE sValue);

	//	设置坐庄状态
	void setApplyState(E_BRNNApplyState eState);

	//	获取当前坐庄状态
	E_BRNNApplyState getApplyState() const;

	//	手牌
	BRNNHandsCard* m_pHandCard[E_BRNN_PokerPos_NULL];

	//	下注区域块
	BRNNBetArea* m_pBetArea[E_BRNNBetArea_NULL];

private:
	bool init();
	void initUI();

	//	单点触摸事件——开始
	bool onTouchBegan(Touch *touch, Event *unused_event);

	//	单点触摸事件——移动
	void onTouchMoved(Touch *touch, Event *unused_event);

	//	单点触摸事件——取消
	void onTouchCancelled(Touch *touch, Event *unused_event);

	//	单点触摸事件——结束
	void onTouchEnded(Touch *touch, Event *unused_event);

	//	点击筹码事件
	void onClickBTNChipEvent(Ref* pSender);

	//	点击退出游戏事件
	void onClickBTNQuitEvent();

	//	点击设置按钮
	void onClickBTNSettingEvent();

	//	点击路单按钮
	void onClickBTNTravelEvent();

	//	点击上庄列表事件
	void onClickBTNApplyListEvent();

	//	点击申请上庄事件
	void onClickBTNApplyEvent();

	//	点击取消申请事件
	void onClickBTNCancelApplyEvent();

	//	点击我要下庄事件
	void onClickBTNDownEvent();

	//	游戏场景背景
	Sprite* m_pBG;

	//	按钮——退出游戏
	Button* m_pBTNQuit;

	//	按钮——设置
	Button* m_pBTNSetting;

	//	按钮——路单
	Button* m_pBTNTravel;

	//	按钮——上庄列表
	Button* m_pBTNApplyList;

	//	按钮——申请上庄
	Button* m_pBTNApply;

	//	按钮——取消申请
	Button* m_pBTNCancelApply;

	//	按钮——我要下庄
	Button* m_pBTNDown;

	//	筹码按钮
	Button* m_pBTNChip[E_BRNNChip_NULL];

	//	筹码高亮罩子
	Sprite* m_pChipHighLight[E_BRNNChip_NULL];

	//	当前选中筹码的类型
	E_BRNNChipType m_eCurrChipType;

	//	申请状态
	E_BRNNApplyState m_eApplyState;
};


#endif  // __BRNN_GAME_LAYER_H__