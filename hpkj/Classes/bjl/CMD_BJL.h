#ifndef CMD_BJL_HEAD_FILE
#define CMD_BJL_HEAD_FILE
#include "Define.h"

#pragma pack(1)

#define CREATE_FUNC_MSG(__TYPE__) \
static __TYPE__* create(int v) \
{ \
	__TYPE__ *pRet = new __TYPE__(v); \
	pRet->autorelease(); \
	return pRet; \
}

enum Game_State
{
	Game_null=1,				//没开始
	Game_Start,					//明牌开始
	Game_landLord,			//抢地主
	Game_Run,					//打牌
	Game_Over,				//游戏结束
};

enum
{
	msgToView_time,	
	msgToView_TimeCallBack,						//--时间回调  Time 控制
	msgToView_OnLine,								//--玩家在线提示
	msgToView_Reconnect,						//--断线重连
	msgToView_socketoffConnect,				//--网络断开自动重连
	msgToView_socketoff,							//--网络断开，返回大厅
	msgToView_VoiceAndFace,					//--表情和语音
	msgToView_huafeidian,							//--花费点
	msgToView_Close,								//--强制关闭
	msgToView_RoomLimit,						//--房间限制
};

//////////////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						122									//游戏 I D
#define GAME_PLAYER					MAX_CHAIR							//游戏人数
#define GAME_NAME					TEXT("新百家乐")						//游戏名字

//组件属性
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

//状态定义
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_BET				GAME_STATUS_PLAY					//下注状态
#define	GAME_SCENE_END				GAME_STATUS_PLAY+1					//结束状态
//数目定义
#define MAX_COUNT					20									//最大数目
#define FULL_COUNT					54									//全牌数目

//逻辑数目
#define NORMAL_COUNT				17									//常规数目
#define DISPATCH_COUNT				51									//派发数目
#define GOOD_CARD_COUTN				38									//好牌数目

//数值掩码
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//逻辑类型
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对牌类型
#define CT_THREE					3									//三条类型
#define CT_SINGLE_LINE				4									//单连类型
#define CT_DOUBLE_LINE				5									//对连类型
#define CT_THREE_LINE				6									//三连类型
#define CT_THREE_TAKE_ONE			7									//三带一单
#define CT_THREE_TAKE_TWO			8									//三带一对
#define CT_FOUR_TAKE_ONE			9									//四带两单
#define CT_FOUR_TAKE_TWO			10									//四带两对
#define CT_BOMB_CARD				11									//炸弹类型
#define CT_MISSILE_CARD				12									//火箭类型

//////////////////////////////////////////////////////////////////////////////////
//状态定义

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_CALL				GAME_STATUS_PLAY					//叫分状态
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+1					//游戏进行

#define AREA_MAX					8									//最大区域
//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率
#define SERVER_LEN					32									//房间长度


//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码


//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_AMDIN_COMMAND			4									//管理员命令
//游戏空闲
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//剩余时间
};

//叫分状态
struct CMD_S_StatusCall
{
	//时间信息
	BYTE							cbTimeOutCard;						//出牌时间
	BYTE							cbTimeCallScore;					//叫分时间
	BYTE							cbTimeStartGame;					//开始时间
	BYTE							cbTimeHeadOutCard;					//首出时间

	//游戏信息
	LONG							lCellScore;							//单元积分
	WORD							wCurrentUser;						//当前玩家
	BYTE							cbBankerScore;						//庄家叫分
	BYTE							cbScoreInfo[GAME_PLAYER];			//叫分信息
	BYTE							cbHandCardData[NORMAL_COUNT];		//手上扑克

	//历史积分
	SCORE							lTurnScore[GAME_PLAYER];			//积分信息
	SCORE							lCollectScore[GAME_PLAYER];			//积分信息
};
//用户下注
struct CMD_S_PlaceBet
{
	WORD							wChairID;							//用户位置
	BYTE							cbBetArea;							//筹码区域
	LONGLONG						lBetScore;							//加注数目
	BYTE							cbAndroidUser;						//机器标识
};

// 0闲,1平,2庄,3闲天王,4庄天王,5同点平,6闲对子,7庄对子
inline const char * BET_AREA_NAME(BYTE cbBetArea){
	const char * s_szArr[8]={"0Xian","1Ping","2Zhuang","3XianTianWang","4ZhuangTianWang","5TongDianPing","6XianDuiZi","7ZhuangDuiZi"};
	const char * s=(cbBetArea>=0&&cbBetArea<8?s_szArr[cbBetArea]:"Unknown");
	return s;
}

//游戏空闲状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	//玩家信息
	LONGLONG						lPlayFreeSocre;						//玩家自由金币

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	TCHART							szBankerNickName[32];				//	庄家昵称
	LONGLONG						lBankerScore;						//庄家分数
	LONGLONG						lBankerWinScore;					//庄家赢分
	WORD							wBankerTime;						//庄家局数

	//是否系统坐庄
	bool							bEnableSysBanker;					//系统做庄

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制

	//房间信息
	TCHART							szGameRoomName[SERVER_LEN];			//房间名称

	WORD                            wFaceID;
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间
	BYTE							cbGameStatus;						//游戏状态

	//下注数
	LONGLONG						lAllBet[AREA_MAX];					//总下注
	LONGLONG						lPlayBet[AREA_MAX];					//玩家下注

	//玩家积分
	LONGLONG						lPlayBetScore;						//玩家最大下注
	LONGLONG						lPlayFreeSocre;						//玩家自由金币

	//玩家输赢
	LONGLONG						lPlayScore[AREA_MAX];				//玩家输赢
	LONGLONG						lPlayAllScore;						//玩家成绩

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	TCHART							szBankerNickName[32];				//	庄家昵称
	LONGLONG						lBankerScore;						//庄家分数
	LONGLONG						lBankerWinScore;					//庄家赢分
	WORD							wBankerTime;						//庄家局数

	//是否系统坐庄
	bool							bEnableSysBanker;					//系统做庄

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制

	//扑克信息
	BYTE							cbCardCount[2];						//扑克数目
	BYTE							cbTableCardArray[2][3];				//桌面扑克

	//房间信息
	TCHART							szGameRoomName[SERVER_LEN];			//房间名称

	WORD                            wFaceID;
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	//扑克信息
	BYTE							cbCardCount[2];						//扑克数目
	BYTE							cbTableCardArray[2][3];				//桌面扑克

	//庄家信息
	LONGLONG						lBankerScore;						//庄家成绩
	LONGLONG						lBankerTotallScore;					//庄家成绩
	int								nBankerTime;						//做庄次数

	//玩家成绩
	LONGLONG						lPlayScore[AREA_MAX];				//玩家成绩
	DOUBLE						    lPlayAllScore;						    //玩家成绩

	//全局信息
	LONGLONG						lRevenue;							//游戏税收
};
//申请庄家
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//申请玩家
};
//取消申请
struct CMD_S_CancelBanker
{
	WORD							wCancelUser;						//取消玩家
};
//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//当庄玩家
	TCHART							szBankerNickName[32];				//	庄家昵称
	LONGLONG						lBankerScore;						//庄家分数
	WORD							wFaceID;
};

//用户下注
struct CMD_C_PlaceBet
{
	BYTE							cbBetArea;						//筹码区域
	LONGLONG						lBetScore;						//加注数目
};


//下注失败
struct CMD_S_PlaceBetFail
{
	WORD							wPlaceUser;							//下注玩家
	BYTE							lBetArea;							//下注区域
	LONGLONG						lPlaceScore;						//当前下注
};
//请求回复
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//回复类型
#define ACK_SET_WIN_AREA  1
#define ACK_PRINT_SYN     2
#define ACK_RESET_CONTROL 3
	BYTE cbResult;
#define CR_ACCEPT  2			//接受
#define CR_REFUSAL 3			//拒绝
	BYTE cbExtendData[20];			//附加数据
};

//提示消息
struct CMD_S_TipInfo
{
	BYTE						cbTimeLeave;					//剩余时间
	TCHART						szTipInfo[256];						//提示消息
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_FREE				99									//游戏空闲
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_PLACE_JETTON			101									//用户下注
#define SUB_S_GAME_END				102									//游戏结束
#define SUB_S_APPLY_BANKER			103									//申请庄家
#define SUB_S_CHANGE_BANKER			104									//切换庄家
#define SUB_S_CHANGE_USER_SCORE		105									//更新积分
#define SUB_S_SEND_RECORD			106									//游戏记录
#define SUB_S_PLACE_JETTON_FAIL		107									//下注失败
#define SUB_S_CANCEL_BANKER			108									//取消申请
#define SUB_S_AMDIN_COMMAND			109									//管理员命令
#define SUB_S_TIP_INFO			    110									//提示消息
#define SUB_S_CANCEL_JETTON			111									//取消下注

//游戏开始
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;						//剩余时间

	WORD							wBankerUser;						//庄家位置
	LONGLONG						lBankerScore;						//庄家金币

	LONGLONG						lPlayBetScore;						//玩家最大下注
	LONGLONG						lPlayFreeSocre;						//玩家自由金币

	int								nChipRobotCount;					//人数上限 (下注机器人)
};


//机器人扑克
struct CMD_S_AndroidCard
{
	BYTE							cbHandCard[GAME_PLAYER][NORMAL_COUNT];//手上扑克
	WORD							wCurrentUser ;						//当前玩家
};

//作弊扑克
struct CMD_S_CheatCard
{
	WORD							wCardUser[GAME_PLAYER ];				//作弊玩家
	BYTE							cbUserCount;							//作弊数量
	BYTE							cbCardData[GAME_PLAYER ][MAX_COUNT];	//扑克列表
	BYTE							cbCardCount[GAME_PLAYER ];				//扑克数量

};
//用户叫分
struct CMD_S_CallScore
{
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wCallScoreUser;						//叫分玩家
	BYTE							cbCurrentScore;						//当前叫分
	BYTE							cbUserCallScore;					//上次叫分
};

//庄家信息
struct CMD_S_BankerInfo
{
	WORD				 			wBankerUser;						//庄家玩家
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							cbBankerScore;						//庄家叫分
	BYTE							cbBankerCard[3];					//庄家扑克
};

//用户出牌
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wOutCardUser;						//出牌玩家
	BYTE							cbCardData[MAX_COUNT];				//扑克列表
};

//放弃出牌
struct CMD_S_PassCard
{
	BYTE							cbTurnOver;							//一轮结束
	WORD				 			wCurrentUser;						//当前玩家
	WORD				 			wPassCardUser;						//放弃玩家
};

//游戏结束
struct CMD_S_GameConclude
{
	//积分变量
	LONG							lCellScore;							//单元积分
	SCORE							lGameScore[GAME_PLAYER];			//游戏积分

	//春天标志
	BYTE							bChunTian;							//春天标志
	BYTE							bFanChunTian;						//春天标志

	//炸弹信息
	BYTE							cbBombCount;						//炸弹个数
	BYTE							cbEachBombCount[GAME_PLAYER];		//炸弹个数

	//游戏信息
	BYTE							cbBankerScore;						//叫分数目
	BYTE							cbCardCount[GAME_PLAYER];			//扑克数目
	BYTE							cbHandCardData[FULL_COUNT];			//扑克列表
};

//////////////////////////////////////////////////////////////////////////////////
//命令定义

#define SUB_C_CALL_SCORE			1									//用户叫分
#define SUB_C_OUT_CARD				2									//用户出牌
#define SUB_C_PASS_CARD				3									//用户放弃

//用户叫分
struct CMD_C_CallScore
{
	BYTE							cbCallScore;						//叫分数目
};

//用户出牌
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	BYTE							cbCardData[MAX_COUNT];				//扑克数据
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif
