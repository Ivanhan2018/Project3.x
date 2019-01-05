#ifndef __SGLY_DEFINE_H__
#define __SGLY_DEFINE_H__

#include "VersionControl.h"

/***********************水果乐园自定义宏	begin*************************************************/
#ifdef VER_9YI
#define SGLY_APPLY_LIST_VIEW_SIZE cocos2d::Size(615.0f, 75.0f * 3.0f)							//	水果乐园——上庄列表视图尺寸
#endif // VER_9YI

#ifdef VER_QIANBO
#define SGLY_APPLY_LIST_VIEW_SIZE cocos2d::Size(593.0f, 49.0f * 4.0f)							//	水果乐园——上庄列表视图尺寸
#endif // VER_QIANBO

#ifdef VER_369
#define SGLY_APPLY_LIST_VIEW_SIZE cocos2d::Size(593.0f, 49.0f * 4.0f)							//	水果乐园——上庄列表视图尺寸
#endif // VER_369

#define SGLY_RES_PLIST "sgly_res.plist"															//	水果乐园——plist资源
#define SGLY_LOTTERY_FORMAT "sgly_lottery_%d.png"												//	水果乐园——抽奖项纹理文件名通用格式
#define SGLY_HISTORY_ITEM_FORMAT "sgly_history_item_%d.png"										//	水果乐园——历史记录子项纹理文件名通用格式

#define SGLY_LOTTERY_COUNT 24																	//	水果乐园——奖项总数
#define SGLY_HISTORY_COUNT_MAX 50																//	水果乐园——历史记录总个数
#define SGLY_HISTORY_LIST_VIEW_SIZE cocos2d::Size(67.0f * 8.0f, 70.0f)							//	水果乐园——历史记录列表视图尺寸
//	XML相关

#define SGLY_XML_FILE_NAME "sgly_strings.xml"													//	水果乐园——XML文件
#define SGLY_XML_KEY_QUIT "tips_quit"															//	水果乐园——key——退出提示语
#define SGLY_XML_KEY_SYSTEM_BANKER "system_banker"												//	水果乐园——key——系统坐庄时显示的庄家名
#define SGLY_XML_KEY_BET_FAILED "bet_failed"													//	水果乐园——key——下注失败

#define SGLY_XML_KEY_SUPPLY_DECLARATION "supply_declaration"									//	水果乐园——key——补充筹码窗口——声明
#define SGLY_XML_KEY_SUPPLY_EXCHANGE_RATE_FORMAT "supply_exchange_rate_format"					//	水果乐园——key——补充筹码窗口——兑换比率格式
#define SGLY_XML_KEY_SUPPLY_TIPS_EXCHANGE_CHIP_FORMAT "supply_tips_exchange_format"				//	水果乐园——key——补充筹码窗口——兑换筹码提示语格式
#define SGLY_XML_KEY_SUPPLY_TIPS_SURPLUS_GOLD_FORMAT "supply_tips_surplus_gold_format"			//	水果乐园——key——补充筹码窗口——剩余金币提示语格式

//	玩家本地数据

#define SGLY_DATA_KEY_AUDIO_VOLUME_BGM "sgly_volume_bgm"										//	水果乐园——key——背景音乐音量
#define SGLY_DATA_KEY_AUDIO_VOLUME_SOUND_EFFECT "sgly_volume_sound_effect"						//	水果乐园——key——音效音量
#define SGLY_DATA_KEY_AUDIO_ON_BGM "sgly_on_bgm"												//	水果乐园——key——打开背景音乐
#define SGLY_DATA_KEY_AUDIO_ON_SOUND_EFFECT "sgly_on_sound_effect"								//	水果乐园——key——打开音效

/***********************水果乐园自定义宏	end*************************************************/



/***********************水果乐园枚举	begin*************************************************/


/*
**	@brief	水果乐园——奖项类型枚举
*/
enum E_SGLY_Lottery
{
	E_SGLY_Lottery_Bar = 0,		//	BAR
	E_SGLY_Lottery_Seven,		//	77
	E_SGLY_Lottery_Star,		//	星星
	E_SGLY_Lottery_Watermelon,	//	西瓜
	E_SGLY_Lottery_Bell,		//	铃铛
	E_SGLY_Lottery_Pear,		//	梨
	E_SGLY_Lottery_Orange,		//	橙
	E_SGLY_Lottery_Apple,		//	苹果
	E_SGLY_Lottery_Null			//	无
};

/*
**	@brief	水果乐园——申请上庄状态枚举
*/
enum E_SGLY_ApplyState
{
	E_SGLY_ApplyState_Apply = 0,	//	申请上庄
	E_SGLY_ApplyState_Banker,		//	已经坐庄
	E_SGLY_ApplyState_Null,			//	无
};

/*
**	@brief	水果乐园——时间流程
*/
enum E_SGLY_Process
{
	E_SGLY_Process_Idle = 0,	//	空闲时间
	E_SGLY_Process_Bet,			//	下注时间	
	E_SGLY_Process_Lottery,		//	开奖时间
	E_SGLY_Process_Guess,		//	猜大小时间
	E_SGLY_Process_Null			//	无
};

/*
**	@brief	水果乐园——押注模式
*/
enum E_SGLY_BettingMode
{
	E_SGLY_BettingMode_M1 = 0,	//	1倍
	E_SGLY_BettingMode_M10,		//	10倍	
	E_SGLY_BettingMode_M100,	//	100倍
	E_SGLY_BettingMode_Null		//	无
};

/*
**	@brief	水果乐园——界面标题数值类型
*/
enum E_SGLY_InfoBar
{
	E_SGLY_InfoBar_PlayerChip = 0,	//	玩家筹码
	E_SGLY_InfoBar_PlayerScore,		//	玩家成绩
	E_SGLY_InfoBar_BankerChip,		//	庄家筹码
	E_SGLY_InfoBar_BankerWinScore,	//	庄家成绩
	E_SGLY_InfoBar_BankerTime,		//	庄数
	E_SGLY_InfoBar_Null				//	无
};

/*
**	@brief	水果乐园——GoodLuck类型
*/
enum E_SGLY_GoodLuckType
{
	E_SGLY_GoodLuck_Swallow = 0,	//	被吃了
	E_SGLY_GoodLuck_DaSanYuan,		//	大三元
	E_SGLY_GoodLuck_DaSiXi,			//	大四喜
	E_SGLY_GoodLuck_XiaoSanYuan,	//	小三元
	E_SGLY_GoodLuck_PaoHuoChe,		//	跑火车
	E_SGLY_GoodLuck_SuiJiDaQiang,	//	随机打枪
	E_SGLY_GoodLuck_Null			//	无
};

/*
**	@brief	水果乐园——音频文件枚举
*/
enum E_SGLY_Audio
{
	//	背景音乐

	E_SGLY_Audio_BGM = 0,					//	背景音乐

	//	音效

	E_SGLY_Audio_BtnBet_Bar,				//	下注按钮——BAR
	E_SGLY_Audio_BtnBet_Seven,				//	下注按钮——77
	E_SGLY_Audio_BtnBet_Star,				//	下注按钮——星星
	E_SGLY_Audio_BtnBet_Watermelon,			//	下注按钮——西瓜
	E_SGLY_Audio_BtnBet_Bell,				//	下注按钮——铃铛
	E_SGLY_Audio_BtnBet_Pear,				//	下注按钮——梨
	E_SGLY_Audio_BtnBet_Orange,				//	下注按钮——橙
	E_SGLY_Audio_BtnBet_Apple,				//	下注按钮——苹果
	E_SGLY_Audio_Lottery_Bar,				//	奖项——BAR
	E_SGLY_Audio_Lottery_Seven,				//	奖项——77
	E_SGLY_Audio_Lottery_Star,				//	奖项——星星
	E_SGLY_Audio_Lottery_Watermelon,		//	奖项——西瓜
	E_SGLY_Audio_Lottery_Bell,				//	奖项——铃铛
	E_SGLY_Audio_Lottery_Pear,				//	奖项——梨
	E_SGLY_Audio_Lottery_Orange,			//	奖项——橙
	E_SGLY_Audio_Lottery_Apple,				//	奖项——苹果
	E_SGLY_Audio_Guess_Lose,				//	猜大小——失败
	E_SGLY_Audio_Guess_Win,					//	猜大小——成功
	E_SGLY_Audio_GoodLuck_Flash,			//	GoodLuck——轮转动画移动
	E_SGLY_Audio_GoodLuck_Hit,				//	GoodLuck——抵达目标位置
	E_SGLY_Audio_TimeWarning,				//	时间警告
	E_SGLY_Audio_TurnStart,					//	轮转动画开始
	E_SGLY_Audio_DispatchLottery,			//	默认轮转动画移动
	E_SGLY_Audio_GameStart,					//	游戏开始

	E_SGLY_Audio_Null						//	无
};


/***********************水果乐园枚举	end*************************************************/

#endif	//	__SGLY_DEFINE_H__