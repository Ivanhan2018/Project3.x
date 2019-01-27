#ifndef __USER_DZPK_H__
#define __USER_DZPK_H__

#include "cocos2d.h"
#include "BJLRes.h"
#include "GlobalDef.h"
#include "DZPKCard.h"
#include "DZPKPlayerChip.h"

USING_NS_CC;

// 测试文本控件的索引<add by hxh 20161027>
#define LABTEST2_IDX (1)

#define DZPK_HAND_CARD_PUBLIC_SCALE 1.0f	//	德州扑克——手牌公开缩放比值
#define DZPK_HAND_CARD_PRIVATE_SCALE 0.5f	//	德州扑克——手牌私有缩放比值

//	德州扑克——玩家视图ID枚举
enum EDZPKPlayerViewID
{
	EDZPKPlayerViewID_0 = 0,	//	视觉座椅0
	EDZPKPlayerViewID_1,		//	视觉座椅1
	EDZPKPlayerViewID_2,		//	视觉座椅2
	EDZPKPlayerViewID_3,		//	视觉座椅3	（自身玩家位置）
	EDZPKPlayerViewID_4,		//	视觉座椅4
	EDZPKPlayerViewID_5,		//	视觉座椅5
	EDZPKPlayerViewID_6,		//	视觉座椅6
	EDZPKPlayerViewID_7			//	视觉座椅7
};

//	德州扑克——玩家行为系列枚举
enum EDZPKPlayerBehaviorType
{
	EDZPKPlayerBehaviorType_Check = 1,	//	过
	EDZPKPlayerBehaviorType_Call,		//	跟注
	EDZPKPlayerBehaviorType_Raise,		//	加注
	EDZPKPlayerBehaviorType_Bet,		//	下注
	EDZPKPlayerBehaviorType_Fold,		//	弃牌
	EDZPKPlayerBehaviorType_AllIn,		//	全下
	EDZPKPlayerBehaviorType_Null,		//	无
};

//	德州扑克——卡牌组合类型枚举
enum EDZPKCardGroupType
{
	EDZPKCardGroupType_HighCard = 1,	//	高牌
	EDZPKCardGroupType_OnePair,			//	一对
	EDZPKCardGroupType_TwoPairs,		//	两对
	EDZPKCardGroupType_ThreeOfAKind,	//	三条
	EDZPKCardGroupType_Straight,		//	顺子
	EDZPKCardGroupType_Flush,			//	同花
	EDZPKCardGroupType_FullHouse,		//	葫芦
	EDZPKCardGroupType_FourOfAKind,		//	四条
	EDZPKCardGroupType_StraightFlush,	//	同花顺
	EDZPKCardGroupType_RoyalFlush,		//	皇家同花顺
};

//	德州扑克——盲注类型枚举
enum EDZPKBlindType
{
	EDZPKBlindType_Big = 0,		//	大盲注
	EDZPKBlindType_small		//	小盲注
};

//	玩家信息物件ZOrder
enum EDZPKUserZOrder
{
	EDZPKUserZOrder_Normal = 100,	//	普通层
	EDZPKUserZOrder_Card,		//	手牌层
	EDZPKUserZOrder_Mask		//	遮罩层
};

//	德州扑克——玩家类
class UserDZPK : public Node
{
public:
	UserDZPK();
	~UserDZPK();
	virtual bool init();
	CREATE_FUNC(UserDZPK);

	//	初始化UI
	void initUI();

	//	重置
	void resetPlayer();

	//	物件自动定位
	void autoPositioning();

	//	设置玩家信息
	void setPlayerInfor(tagUserData sUserData);

	//  设置玩家金币<by hxh>
	void setPlayerGold(LONGLONG lScore);

	//  获取玩家金币<by hxh>
	LONGLONG getPlayerGold() const;

	//  调试信息<by hxh>
	void updateTest(const char* szName,int idx);

	//	处理玩家状态
	void dealPlayerState();

	//	设置视图ID
	void setViewID(EDZPKPlayerViewID eViewID);

	//	获取视图ID
	EDZPKPlayerViewID getViewID() const;

	//	重置手牌
	void resetHandCard();

	//	设置手牌
	void setHandCard(int nCardID, unsigned char cCardData);

	//	玩家行为表现
	void onBehavior(EDZPKPlayerBehaviorType eType);

	//	展示盲注
	void showBlind(EDZPKBlindType eType);

	//	隐藏盲注
	void hideBlind();

	//	展示牌型标识
	void showCardGroupType(BYTE bCardType);

	//	展示等待标识
	void showWaiting();

	//	展示D玩家标识
	void showDPlayer(bool bIsDPlayer);

	//	展示正常状态
	void showNormal();

	//	展示掉线状态
	void showOffline();

	//	处理准备阶段表现
	void dealReady();

	//	获取用户信息
	tagUserData getPlayerData() const;

	//	执行倒计时
	void showCountdown();

	//	取消倒计时
	void cancelCountdown();

	//	筹码
	DZPKPlayerChip* m_pPlayerChip;

	//	玩家手牌
	DZPKHandCard* m_pHandCard;

	//	筹码池的位置
	Vec2 m_vChipPotPosition;

	//	玩家行为状态
	CC_SYNTHESIZE(EDZPKPlayerBehaviorType, m_eBehaviorState, BehaviorState);
private:

	//	显示玩家行为提示
	void showBehaviorTips(EDZPKPlayerBehaviorType eType);

	//	播放玩家行为音效
	void playBehaviorMusicEffect(EDZPKPlayerBehaviorType eType, char cGender);

	//	弃牌表现
	void onFold();

	//	弃牌动画
	void aniFold();

	//	背景——遮罩
	Sprite* m_pBGMask;

	//	背景——玩家信息
	Sprite* m_pBGPlayerData;

	//	图标——玩家头像
	Sprite* m_pIconFace;

	//	图标——玩家行为
	Sprite* m_pIconPBehavior;

	//	图标——VIP
	Sprite* m_pIconVIP;

	//	图标——等待下一局
	Sprite* m_pIconWait;

	//	图标——盲注
	Sprite* m_pIconBlind;

	//	图标——网络状态
	Sprite* m_pIconNetwork;

	//	图标——准备
	Sprite* m_pIconReady;

	//	图标——庄家（D玩家）
	Sprite* m_pIconBanker;

	//	图标——牌型
	Sprite* m_pIconCardGroupType;

	//	文本——名称
	Label* m_pLabName;

	//	文本——玩家持有金币
	Label* m_pLabGold;

	//	文本——测试消息
	Label* m_pLabTest;
	Label* m_pLabTest2;

	//	倒计时
	ProgressTimer* m_pCountdown;

	//	玩家信息
	tagUserData m_sPlayerData;

	//	数值——玩家持有金币数
	LONGLONG m_llCurrGold;

	//	是否掉线
	bool m_bOffline;

	//	是否显示遮罩
	bool m_bShowMask;

	//	玩家视图ID
	EDZPKPlayerViewID m_eViewID;
};

#endif	//	__USER_DZPK_H__