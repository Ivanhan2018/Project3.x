#ifndef __BRNN_DEFINE_H__
#define __BRNN_DEFINE_H__

/***********************百人牛牛自定义宏	begin*************************************************/

#define BRNN_POKER_CARD_PLIST "common_poker_card.plist"							//	扑克牌plist
#define BRNN_RES_PLIST "brnn_res.plist"											//	资源图片plist

#define BRNN_POKER_CARD_FORMAT "poker_card_%d.png"								//	扑克牌文件名格式（0为背面）
#define BRNN_POKER_CARD_COUNT 54												//	扑克牌总数

#define BRNN_HANDS_CARD_COUNT 5													//	百人牛牛——手牌数
#define BRNN_CARD_SCALE_BIG 1.1f												//	百人牛牛——手牌缩放——大

#define BRNN_AREA_CHIP_SCALE_SMALL 0.5f											//	百人牛牛——区域块筹码缩放比例

#define BRNN_CLOCK_NUMBER_FNT "brnn_fnt_time_num.fnt"							//	百人牛牛——时钟数字字体文件

#define BRNN_APPLY_LIST_VIEW_SIZE cocos2d::Size(590.0f, 81.0f * 3.0f)			//	百人牛牛——上庄列表视图尺寸
#define BRNN_TRAVEL_VIEW_SIZE cocos2d::Size(66.0f * 11.0f, 59.0f * 4.0f)		//	百人牛牛——路单视图尺寸
#define BRNN_TRAVEL_COLUMN_SIZE cocos2d::Size(66.0f, 59.0f * 4.0f)				//	百人牛牛——路单一列尺寸

#define BRNN_MAX_SCORE_HISTORY 16												//	百人牛牛——路单历史个数

//	XML相关

#define BRNN_XML_FILE_NAME "brnn_strings.xml"									//	百人牛牛——XML文件
#define BRNN_XML_KEY_QUIT "tips_quit"											//	百人牛牛——key——退出提示语
#define BRNN_XML_KEY_SYSTEM_BANKER "system_banker"								//	百人牛牛——key——系统坐庄时显示的庄家名
#define BRNN_XML_KEY_BET_FAILED "bet_failed"									//	百人牛牛——key——下注失败

//	玩家本地数据

#define BRNN_DATA_KEY_AUDIO_VOLUME_BGM "volume_bgm"								//	百人牛牛——key——背景音乐音量
#define BRNN_DATA_KEY_AUDIO_VOLUME_SOUND_EFFECT "volume_sound_effect"			//	百人牛牛——key——音效音量
#define BRNN_DATA_KEY_AUDIO_ON_BGM "on_bgm"										//	百人牛牛——key——打开背景音乐
#define BRNN_DATA_KEY_AUDIO_ON_SOUND_EFFECT "on_sound_effect"					//	百人牛牛——key——打开音效


/***********************百人牛牛自定义宏	end*************************************************/



/***********************百人牛牛枚举	begin*************************************************/



/*
**	@brief	百人牛牛——牌型列举
*/
enum E_BRNN_CardType
{
	E_BRNN_CardType_Error,			//	错误类型
	E_BRNN_CardType_Point,			//	点数类型
	E_BRNN_CardType_Niu1,			//	牛1
	E_BRNN_CardType_Niu2,			//	牛2
	E_BRNN_CardType_Niu3,			//	牛3
	E_BRNN_CardType_Niu4,			//	牛4
	E_BRNN_CardType_Niu5,			//	牛5
	E_BRNN_CardType_Niu6,			//	牛6
	E_BRNN_CardType_Niu7,			//	牛7
	E_BRNN_CardType_Niu8,			//	牛8
	E_BRNN_CardType_Niu9,			//	牛9
	E_BRNN_CardType_NiuNiu,			//	牛牛
	E_BRNN_CardType_SmallNiu,		//	小王牛牛
	E_BRNN_CardType_BigNiu,			//	大王牛牛
	E_BRNN_CardType_SilverNiu,		//	银牛（四花）
	E_BRNN_CardType_GoldNiu,		//	金牛（五花）
	E_BRNN_CardType_Bomb			//	炸弹
};

/*
**	@brief	百人牛牛——筹码类型
*/
enum E_BRNNChipType
{
	E_BRNNChip_1 = 0,	//	筹码面值——1
	E_BRNNChip_5,		//	筹码面值——5
	E_BRNNChip_10,		//	筹码面值——10
	E_BRNNChip_50,		//	筹码面值——50
	E_BRNNChip_100,		//	筹码面值——100
	E_BRNNChip_200,		//	筹码面值——200
	E_BRNNChip_500,		//	筹码面值——500
	E_BRNNChip_1k,		//	筹码面值——1k
	E_BRNNChip_NULL		//	筹码面值——无
};

/*
**	@brief	百人牛牛——筹码类型
*/
enum E_BRNNBetAreaType
{
	E_BRNNBetArea_E = 0,	//	下注区域——东
	E_BRNNBetArea_S,		//	下注区域——南
	E_BRNNBetArea_W,		//	下注区域——西
	E_BRNNBetArea_N,		//	下注区域——北
	E_BRNNBetArea_NULL		//	下注区域——无
};

/*
**	@brief	百人牛牛——扑克牌位置
*/
enum E_BRNN_PokerPos
{
	E_BRNN_PokerPos_B = 0,	//	扑克牌位置——庄
	E_BRNN_PokerPos_E,		//	扑克牌位置——东
	E_BRNN_PokerPos_S,		//	扑克牌位置——南
	E_BRNN_PokerPos_W,		//	扑克牌位置——西
	E_BRNN_PokerPos_N,		//	扑克牌位置——北
	E_BRNN_PokerPos_NULL	//	扑克牌位置——无
};

/*
**	@brief	百人牛牛——计时器类型
*/
enum E_BRNNProcessType
{
	E_BRNN_Idle = 0,	//	空闲时间
	E_BRNN_Bet,			//	下注时间	
	E_BRNN_OpenCard,	//	开牌时间
	E_BRNN_Null			//	无
};

/*
**	@brief	百人牛牛——申请上庄状态枚举
*/
enum E_BRNNApplyState
{
	E_BRNN_EPPC_Apply_State_Apply = 0,	//	申请上庄
	E_BRNN_EPPC_Apply_State_Banker,		//	已经坐庄
	E_BRNN_Apply_State_Null,			//	无
};

/*
**	@brief	百人牛牛——音频文件枚举
*/
enum E_BRNNAudio
{
	E_BRNNAudio_BGMusic = 0,	//	背景音乐
	E_BRNNAudio_Change_Banker,	//	音效——切换庄家
	E_BRNNAudio_Countdown,		//	音效——倒计时
	E_BRNNAudio_Game_Start,		//	音效——游戏开始
	E_BRNNAudio_Bet,			//	音效——下注
	E_BRNNAudio_Lose,			//	音效——输
	E_BRNNAudio_Win,			//	音效——赢
	E_BRNNAudio_Null			//	无
};



/***********************百人牛牛枚举	end*************************************************/

#endif	//	__BRNN_DEFINE_H__