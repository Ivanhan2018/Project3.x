#ifndef DDZ_CMD_GAMECLIENT_HEAD_FILE
#define DDZ_CMD_GAMECLIENT_HEAD_FILE
#include "Define.h"
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//公共宏定义

#define DDZKIND_ID					200									//游戏 I D
#define DDZGAME_PLAYER				3							        //游戏人数
#define GAME_NAME					TEXT("斗地主")						//游戏名字

//组件属性
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

//状态定义
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY					//游戏进行
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
/*
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_CALL				GAME_STATUS_PLAY					//叫分状态
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+1					//游戏进行
*/

//游戏状态   斗地主新加参数
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_CALL				GAME_STATUS_PLAY					//叫地主状态
#define GAME_SCENE_ROD				GAME_STATUS_PLAY+1					//抢地主状态
//#define GAME_SCENE_ADD				GAME_STATUS_PLAY+2					//加倍状态
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+2					//游戏进行


#define AREA_MAX					8									//最大区域
//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率
#define SERVER_LEN					32									//房间长度

#if 0
//#define KIND_ID						ID_LAND						        //游戏 I D
#define DDZGAME_PLAYER					3									//游戏人数
//#define GAME_NAME					TEXT("斗地主游戏")					//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//游戏类型
#define MAX_COUNT					20
#define GAME_WIDTH				1024
#define GAME_HEIGHT				738

//聊天组件的位置
#define CHAT_DLG_X					740                     
#define CHAT_DLG_Y					650
#endif

//游戏空闲
struct DDZCMD_S_GameFree
{
	//游戏属性
	LONG							lCellScore;							//基础积分

	//时间信息
	BYTE							cbTimeCallBanker;					//叫地主时间
	BYTE							cbTimeRodBanker;					//抢地主时间
	BYTE							cbTimeOutCard;						//出牌时间
	//BYTE							cbTimeCallScore;					//叫分时间
	BYTE							cbTimeStartGame;					//开始时间
	BYTE							cbTimeHeadOutCard;					//首出时间

	//历史积分
	SCORE							lTurnScore[DDZGAME_PLAYER];			//积分信息
	SCORE							lCollectScore[DDZGAME_PLAYER];			//积分信息
};

//叫地主状态
struct DDZCMD_S_StatusCall
{
	//时间信息
	BYTE							cbTimeCallBanker;					//叫地主时间
	BYTE							cbTimeRodBanker;					//抢地主时间
	//BYTE							cbTimeAddDouble;					//加倍时间
	BYTE							cbTimeStartGame;					//开始时间
	BYTE							cbTimeHeadOutCard;					//首出时间
	BYTE							cbTimeOutCard;						//出牌时间

	//游戏信息
	LONG							lCellScore;							//单元积分
	WORD							wFirstUser;							//首叫用户
	//WORD							wFirstValidUser;					//首叫明牌
	WORD							wCurrentUser;						//当前玩家
	WORD							wStartTime;							//初始倍数
	//WORD							wValidCardTime;						//明牌倍数
	//bool							bValidCardInfo[GAME_PLAYER];		//明牌信息
	BYTE							cbCallBankerInfo[DDZGAME_PLAYER];		//叫地主信息	
	BYTE							cbHandCardData[DDZGAME_PLAYER][NORMAL_COUNT];		//手上扑克

	//历史积分
	SCORE							lTurnScore[DDZGAME_PLAYER];			//积分信息
	SCORE							lCollectScore[DDZGAME_PLAYER];			//积分信息
};

//抢地主状态
struct DDZCMD_S_StatusRod
{
	//时间信息
	BYTE							cbTimeCallBanker;					//叫地主时间
	BYTE							cbTimeRodBanker;					//抢地主时间
	//BYTE							cbTimeAddDouble;					//加倍时间
	BYTE							cbTimeStartGame;					//开始时间
	BYTE							cbTimeHeadOutCard;					//首出时间
	BYTE							cbTimeOutCard;						//出牌时间

	//游戏信息
	LONG							lCellScore;							//单元积分
	WORD							wFirstUser;							//首叫用户
	//WORD							wFirstValidUser;					//首叫明牌
	WORD							wBankerUser;						//庄家用户
	WORD							wCurrentUser;						//当前用户

	WORD							wStartTime;							//初始倍数
	//WORD							wValidCardTime;						//明牌倍数
	WORD							wRodBankerTime;						//抢地主倍数

	//bool							bValidCardInfo[GAME_PLAYER];		//明牌信息
	BYTE							cbCallBankerInfo[DDZGAME_PLAYER];		//叫地主信息	
	BYTE							cbRodBankerInfo[DDZGAME_PLAYER];		//抢地主信息
	//BYTE							cbValidCardInfo[DDZGAME_PLAYER];		//明牌信息
	BYTE							cbHandCardData[DDZGAME_PLAYER][NORMAL_COUNT];//手上扑克

	//历史积分
	SCORE							lTurnScore[DDZGAME_PLAYER];			//积分信息
	SCORE							lCollectScore[DDZGAME_PLAYER];			//积分信息
};

/*
//叫分状态
struct DDZCMD_S_StatusCall
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
	BYTE							cbScoreInfo[DDZGAME_PLAYER];			//叫分信息
	BYTE							cbHandCardData[NORMAL_COUNT];		//手上扑克
	//历史积分
	SCORE							lTurnScore[DDZGAME_PLAYER];			//积分信息
	SCORE							lCollectScore[DDZGAME_PLAYER];			//积分信息
};
*/
//用户下注
struct DDZCMD_S_PlaceBet
{
	WORD							wChairID;							//用户位置
	BYTE							cbBetArea;							//筹码区域
	LONGLONG						lBetScore;							//加注数目
	BYTE							cbAndroidUser;						//机器标识
};

//游戏状态
struct DDZ_CMD_S_StatusPlay
{
	//时间信息
	BYTE							cbTimeCallBanker;					//叫地主时间
	BYTE							cbTimeRodBanker;					//抢地主时间
	BYTE							cbTimeAddDouble;					//加倍时间
	BYTE							cbTimeStartGame;					//开始时间
	BYTE							cbTimeHeadOutCard;					//首出时间
	BYTE							cbTimeOutCard;						//出牌时间

	//游戏变量
	LONG							lCellScore;							//单元积分
	WORD							wBankerUser;						//庄家用户
	WORD							wCurrentUser;						//当前玩家

	WORD							wStartTime;							//初始倍数
	//WORD							wValidCardTime;						//明牌倍数
	WORD							wRodBankerTime;						//抢地主倍数
	WORD							wBackCardTime;						//底牌倍数
	WORD							wBombTime;							//炸弹倍数
	WORD							wCardTypeTime;						//牌型倍数

	BYTE							cbBombCount;						//炸弹次数
	BYTE							cbCallBankerInfo[DDZGAME_PLAYER];		//叫地主信息	
	BYTE							cbRodBankerInfo[DDZGAME_PLAYER];		//抢地主信息
	BYTE							cbAddDoubleInfo[DDZGAME_PLAYER];		//加倍信息	
	//bool							bValidCardInfo[GAME_PLAYER];		//明牌信息

	//出牌信息
	WORD							wTurnWiner;							//胜利玩家
	BYTE							cbTurnCardCount;					//出牌数目
	BYTE							cbTurnCardData[MAX_COUNT];			//出牌数据

	//扑克信息
	BYTE							cbBankerCard[3];					//游戏底牌
	BYTE							cbHandCardCount[DDZGAME_PLAYER];	//扑克数目
	BYTE							cbHandCardData[DDZGAME_PLAYER][MAX_COUNT];		//手上扑克
	//BYTE							cbHandCardData[MAX_COUNT];			//手上扑克

	//历史积分
	SCORE							lTurnScore[DDZGAME_PLAYER];			//积分信息
	SCORE							lCollectScore[DDZGAME_PLAYER];		//积分信息
};

//游戏结束<by hxh 20160822>
struct DDZCMD_S_GameEnd
{
	
	//积分变量
	LONG							lCellScore;							//单元积分
	SCORE							lGameScore[DDZGAME_PLAYER];			//游戏积分

	//春天标志
	BYTE							bChunTian;							//春天标志
	BYTE							bFanChunTian;						//春天标志

	//炸弹信息
	BYTE							cbBombCount;						//炸弹个数
	BYTE							cbEachBombCount[DDZGAME_PLAYER];	//炸弹个数

	//游戏信息
	//BYTE							cbBankerScore;						//叫分数目
	BYTE							cbCardCount[DDZGAME_PLAYER];		//扑克数目
	BYTE							cbHandCardData[FULL_COUNT];			//扑克列表
	
};

//申请庄家
struct DDZCMD_S_ApplyBanker
{
	WORD							wApplyUser;							//申请玩家
};
//取消申请
struct DDZCMD_S_CancelBanker
{
	WORD							wCancelUser;						//取消玩家
};
//切换庄家
struct DDZCMD_S_ChangeBanker
{
	WORD							wBankerUser;						//当庄玩家
	LONGLONG						lBankerScore;						//庄家分数
};

////记录信息
//struct tagServerGameRecord
//{
//	BYTE							cbKingWinner;						//天王赢家
//	bool							bPlayerTwoPair;						//对子标识
//	bool							bBankerTwoPair;						//对子标识
//	BYTE							cbPlayerCount;						//闲家点数
//	BYTE							cbBankerCount;						//庄家点数
//};
//下注失败
struct DDZCMD_S_PlaceBetFail
{
	WORD							wPlaceUser;							//下注玩家
	BYTE							lBetArea;							//下注区域
	LONGLONG						lPlaceScore;						//当前下注
};
//请求回复
struct DDZCMD_S_CommandResult
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
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_FREE				99									//游戏空闲
#if 0
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_PLACE_JETTON			101									//用户下注
#define SUB_S_GAME_END				102									//游戏结束
#define SUB_S_APPLY_BANKER			103									//申请庄家
#define SUB_S_CHANGE_BANKER			104									//切换庄家
#define SUB_S_CHANGE_USER_SCORE		105									//更新积分
#define SUB_S_SEND_RECORD			106									//游戏记录
#define SUB_S_PLACE_JETTON_FAIL		107									//下注失败
#else
#define DDZ_SUB_S_GAME_START			100									//游戏开始
#define DDZ_SUB_S_CALL_BANKER			101									//叫地主
#define DDZ_SUB_S_BANKER_INFO			102									//庄家信息
#define DDZ_SUB_S_ROD_BANKER			103									//抢地主
#define DDZ_SUB_S_OUT_CARD				106									//用户出牌
#define DDZ_SUB_S_PASS_CARD				107									//用户放弃
#define DDZ_SUB_S_OUT_START_START		108									//开始出牌
#define DDZ_SUB_S_GAME_CONCLUDE			109									//游戏结束
#define DDZ_SUB_S_SET_BASESCORE			110									//设置基数
#define DDZ_SUB_S_CHEAT_CARD			111									//作弊扑克

#endif
#define SUB_S_CANCEL_BANKER			108									//取消申请
#define SUB_S_AMDIN_COMMAND			109									//管理员命令

#define HLDDZ_SUB_S_GAME_START              100									//游戏开始
#define HLDDZ_SUB_S_CALL_SCORE              101									//用户叫分
#define HLDDZ_SUB_S_CALL_BANKER             101									//叫地主
#define HLDDZ_SUB_S_BANKER_INFO             102									//庄家信息
#define HLDDZ_SUB_S_ROD_BANKER              103									//抢地主
#define HLDDZ_SUB_S_DOUBLE                  104									//加倍信息
#define HLDDZ_SUB_S_VALID_CARD              105									//用户明牌
#define HLDDZ_SUB_S_OUT_CARD				106                                 //用户出牌
#define HLDDZ_SUB_S_PASS_CARD				107                                 //用户放弃
#define HLDDZ_SUB_S_OUT_START_START         108									//开始出牌
#define HLDDZ_SUB_S_GAME_CONCLUDE			109                                 //游戏结束
#define HLDDZ_SUB_S_SET_BASESCORE			110                                 //设置基数
#define HLDDZ_SUB_S_TRUSTEE                 111									//用户托管
#define HLDDZ_SUB_S_NEWBANK                 112                                 //新的地主
#if 0
#define REC_SUB_S_SEND_CARD				100									//发牌命令
#define REC_SUB_S_LAND_SCORE			101									//叫分命令
#define REC_SUB_S_GAME_START			102									//游戏开始
#define REC_SUB_S_OUT_CARD				103									//用户出牌
#define REC_SUB_S_PASS_CARD				104									//放弃出牌
#define REC_SUB_S_GAME_END				105									//游戏结束
#define REC_SUB_S_SPRING                106                                 //春天消息
#define REC_SUB_S_OUTCARD_TIMES         108                                 //出牌次数    
#define REC_SUB_S_LOOK_TRUSTEE          109                                 //托管查询
#define REC_SUB_S_WARN_INFO             111                                 //报警消息
#define REC_SUB_S_OFFLINE_CARD          125                                 //断线出牌
#define REC_SUB_S_USER_OFFLINE          150                                 //玩家断线
#define REC_SUB_S_DAOFAN                151                                 //用户倒反
#define REC_SUB_S_OPENCARD			152									//用户明牌
#define REC_SUB_S_TIMETOCLOSE			154								//解散用户
#define GS_WK_SCORE                    50                                   //叫分状态  
#define SUB_S_GAME_READY             5001                //通知客户端发送准备请求
#define SUB_C_GAME_READY             5002                //通知客户端发送准备请求
#define SUB_S_ALL_READY              5003                //通知客户端已经准备好的人
#define SUB_C_ALL_READY              5004                //通知服务端已经准备好的人
#define SUB_C_ANDROCHAIR             5005                //机器人椅子号
//#define SUB_C_MATCH_TYPE             5006                //比赛类型
#define SUB_SC_READYTIMEOUT					72				//准备超时
#define SUB_S_GAMESTART					5008				//游戏开始
#endif

////游戏开始
//struct DDZCMD_S_GameStart
//{
//	BYTE							cbTimeLeave;						//剩余时间
//
//	WORD							wBankerUser;						//庄家位置
//	LONGLONG						lBankerScore;						//庄家金币
//
//	LONGLONG						lPlayBetScore;						//玩家最大下注	
//	LONGLONG						lPlayFreeSocre;						//玩家自由金币
//
//	int								nChipRobotCount;					//人数上限 (下注机器人) 
//	__int64                         nListUserCount;						//列表人数
//	int								nAndriodCount;						//机器人列表人数
//};

//发送扑克<add by hxh 20160822>
struct DDZCMD_S_GameStart
{
	WORD							wStartUser;							//开始玩家
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wStartTime;							//初始倍数
	BYTE							cbCardData[NORMAL_COUNT];			//扑克列表
};

#if 1
//发送扑克
struct HLDDZ_CMD_S_GameStart
{
	WORD							wStartUser;							//开始玩家
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							cbValidCardData;					//明牌扑克
	BYTE							cbValidCardIndex;					//明牌位置
	BYTE							cbCardData[NORMAL_COUNT];			//扑克列表
};
#else
//发送扑克
struct DDZCMD_S_SendAllCard
{
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bCardData[GAME_PLAYER][MAX_COUNT];			//扑克列
	BYTE                            bMingPai[MAX_COUNT];                           //明牌
	BYTE							bBackCardData[GAME_PLAYER];					//底牌扑克
	LONG							lBaseScore;							//基础积分
	bool							bDog;								//gou
};
//开始信息
struct HLDDZ_CMD_S_Game_Start
{
	WORD				 			wBankerUser;						//庄家玩家
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							cbCardData[MAX_COUNT];				//扑克数据
    
};
#endif

//机器人扑克
struct DDZCMD_S_AndroidCard
{
	BYTE							cbHandCard[DDZGAME_PLAYER][NORMAL_COUNT];//手上扑克
	WORD							wCurrentUser ;						//当前玩家
#if 1
	//梦成添加
	BYTE							cbBankerCard[3];					  //庄家扑克
	WORD							wAndroidUserID;						  //好牌玩家
#endif
};

//作弊扑克
struct DDZCMD_S_CheatCard
{
	WORD							wCardUser[DDZGAME_PLAYER ];				//作弊玩家
	BYTE							cbUserCount;							//作弊数量
	BYTE							cbCardData[DDZGAME_PLAYER ][MAX_COUNT];	//扑克列表
	BYTE							cbCardCount[DDZGAME_PLAYER ];				//扑克数量

};
//用户叫分
struct DDZCMD_S_CallScore
{
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wCallScoreUser;						//叫分玩家
	BYTE							cbCurrentScore;						//当前叫分
	BYTE							cbUserCallScore;					//上次叫分
};


//用户叫地主
struct CMD_S_CallBanker
{
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wLastUser;							//叫地主玩家
	BYTE							cbCallInfo;							//叫地主
};

//用户抢地主
struct CMD_S_RodBanker
{
	WORD							wRodBankerTime;						//抢地主倍数

	WORD				 			wBankerUser;						//庄家玩家
	WORD				 			wCurrentUser;						//当前玩家	
	WORD							wLastUser;							//抢地主玩家
	BYTE							cbRodlInfo;							//抢地主
};

#if 0
//用户叫分（服务端）
struct DDZCMD_S_LandScore
{
	bool								bCanLand;							//有无叫分
	WORD							bLandUser;							//叫分玩家
	WORD							wLanddata;							//叫分玩家标识--用于标记抢地主和叫地主
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wCurrentData;						//当前玩家标识
	//BYTE							bLandScore;							//上次叫分
	//BYTE							bCurrentScore;						//当前叫分
	//bool                            bGameStart;                         //游戏开始标志
};
//用户叫地主
struct HLDDZ_CMD_S_CallBanker
{
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wLastUser;							//叫地主玩家
	BYTE							cbCallInfo;							//叫地主
};
#endif

//庄家信息
struct DDZCMD_S_BankerInfo
{

	WORD				 			wBankerUser;						//庄家玩家
	WORD				 			wCurrentUser;						//当前玩家
	//WORD							wBackCardTime;						//底牌倍数
	BYTE							cbBankerCard[3];					//庄家扑克
	//BYTE							cbBackCardType;						//底牌类型
	//bool							bDoubleInfo[3];						//能否加倍
};

#if 1
//庄家信息
struct HLDDZ_CMD_S_BankerInfo
{
	WORD				 			wBankerUser;						//庄家玩家
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							cbBankerScore;						//庄家叫分
	BYTE							cbBankerCard[3];					//庄家扑克
};
#else
//庄家信息
struct HLDDZ_CMD_S_BankerInfo
{
	WORD				 			wBankerUser;						//庄家玩家
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wBackCardTime;						//底牌倍数
	BYTE							cbBankerCard[3];					//庄家扑克
	BYTE							cbBackCardType;						//底牌类型
	bool							bDoubleInfo[3];						//能否加倍
};
#endif

//用户出牌
struct DDZCMD_S_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wOutCardUser;						//出牌玩家
	BYTE							cbCardData[MAX_COUNT];				//扑克列表
};

//用户出牌
struct HLDDZ_CMD_S_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wOutCardUser;						//出牌玩家
	BYTE							cbCardData[MAX_COUNT];				//扑克列表
};

//放弃出牌
struct DDZCMD_S_PassCard
{
	BYTE							cbTurnOver;							//一轮结束
	WORD				 			wCurrentUser;						//当前玩家
	WORD				 			wPassCardUser;						//放弃玩家
};

//放弃出牌
struct HLDDZ_CMD_S_PassCard
{
	BYTE							cbTurnOver;							//一轮结束
	WORD				 			wCurrentUser;						//当前玩家
	WORD				 			wPassCardUser;						//放弃玩家
};

//游戏结束
struct DDZCMD_S_GameConclude
{
	//积分变量
	SCORE							lCellScore;							//单元积分<by hxh>
	SCORE							lGameScore[DDZGAME_PLAYER];			//游戏积分

	//春天标志
	BYTE							bChunTian;							//春天标志
	BYTE							bFanChunTian;						//春天标志

	//炸弹信息
	BYTE							cbBombCount;						//炸弹个数
	BYTE							cbEachBombCount[DDZGAME_PLAYER];		//炸弹个数

	//游戏信息
	BYTE							cbBankerScore;						//叫分数目
	BYTE							cbCardCount[DDZGAME_PLAYER];			//扑克数目
	BYTE							cbHandCardData[FULL_COUNT];			//扑克列表
};

//游戏结束
struct HLDDZ_CMD_S_GameConclude
{
	//积分变量
	SCORE							lCellScore;							//单元积分
	SCORE							lGameScore[DDZGAME_PLAYER];			//游戏积分
    
	//春天标志
	BYTE							bChunTian;							//春天标志
	BYTE							bFanChunTian;						//春天标志
    
	//炸弹信息
	BYTE							cbBombCount;						//炸弹个数
	BYTE							cbEachBombCount[DDZGAME_PLAYER];		//炸弹个数
    
	//游戏信息
	BYTE							cbBankerScore;						//叫分数目
	BYTE							cbCardCount[DDZGAME_PLAYER];			//扑克数目
	BYTE							cbHandCardData[DDZGAME_PLAYER];			//扑克列表
};

//////////////////////////////////////////////////////////////////////////////////
//命令定义

#define SUB_C_CALL_SCORE			1									//用户叫分
#define SUB_C_CALL_BANKER			2									//用户叫地主
#define SUB_C_ROD_BANKER			3									//用户抢地主
#define SUB_C_DOUBLE				4									//用户加倍
#define SUB_C_OUT_CARD				5									//用户出牌
#define SUB_C_PASS_CARD				6									//用户放弃



//========================================================================================
//斗地主命令值定义  （发送叫，抢操作时，相应动作值）
//叫地主
#define CB_NOT_CALL					0									//没叫地主
#define CB_CALL_BENKER				1									//叫地主
#define CB_NO_CALL_BENKER			2									//不叫地主

//抢地主
#define CB_NOT_ROD					0									//没抢地主
#define CB_ROD_BANKER				1									//抢地主
#define CB_NO_ROD_BANKER			2									//不抢地主
#define CB_CAN_NO_ROD				3									//不能抢

//////////////////////////////////////////////////////////////////////////////////


#if 0
#define SUB_C_CALL_SCORE			1									//用户叫分
#define SUB_C_DOUBLE_SCORE			2									//用户加倍
#define SUB_C_OUT_CARD				3									//用户出牌
#define SUB_C_PASS_CARD				4									//用户放弃
#define SUB_C_TRUSTEE               5                                   //用户托管

#define HLDDZ_SUB_C_VALID_CARD			1									//用户明牌
#define HLDDZ_SUB_C_CALL_SCORE			2									//用户叫分
#define HLDDZ_SUB_C_CALL_BANKER			2									//用户叫地主
#define HLDDZ_SUB_C_ROD_BANKER			3									//用户抢地主
#define HLDDZ_SUB_C_DOUBLE				4									//用户加倍
#define HLDDZ_SUB_C_OUT_CARD				5									//用户出牌
#define HLDDZ_SUB_C_PASS_CARD				6									//用户放弃
#define HLDDZ_SUB_C_TRUSTEE				7									//用户托管
#define HLDDZ_SUB_C_BRIGHT_START			8									//明牌开始
#endif
#if 0
#define SUB_C_CLIENT_READY           4001                //客户端准备好了
#define REC_SUB_C_LAND_SCORE			1									//用户叫分
#define REC_SUB_C_OUT_CART				2									//用户出牌
#define REC_SUB_C_PASS_CARD				3									//放弃出牌
#define REC_SUB_C_TRUSTEE				4									//托管消息
#define REC_SUB_C_GAME_END              5                                   //游戏结束
#define REC_SUB_C_LOOKTRUSTEE           6                                   //查询托管
#define REC_SUB_C_WARN_INFO             12									//报警消息
#define REC_SUB_C_TURN_COUNT            13									//出牌轮数
#define REC_SUB_C_CONTINUE              14                                  //按键继续
#define	REC_SUB_S_GOLDEGG               16                                  //送金蛋
#define REC_SUB_S_RESED_CARD            17                                   //重新发牌（用于纠错）
#define REC_SUB_S_BEILV						20								//--倍率变化
#define REC_SUB_S_ONLINE					22								//--玩家在线标志
#define REC_SUB_C_OVER						24								//--客户端主动结束，用于测试
#endif
#define REC_SUB_C_DAOFAN                15                                  //用户倒反
#define REC_SUB_C_OPNE_CARD				18								//--玩家明牌

//用户叫分
struct DDZCMD_C_CallScore
{
	BYTE							cbCallScore;						//叫分数目
};

//用户叫地主
struct CMD_C_CallBanker
{				
	BYTE							cbCallInfo;							//叫地主
};

//用户抢地主
struct CMD_C_RodBanker
{				
	BYTE							cbRodInfo;							//抢地主
};



#if 1
//用户叫分
struct HLDDZ_CMD_C_CallScore
{
	BYTE							cbCallScore;						//叫分数目
};
#else
//用户叫分(客户端)
struct DDZCMD_C_LandScore  
{
	bool								  bLandScore;							//地主分数
    WORD                            bLandUser;                          //叫分用户
};
#endif

#if 0
//用户叫地主
struct HLDDZ_CMD_C_CallBanker
{
	BYTE							cbCallInfo;							//叫地主
};

//用户抢地主
struct HLDDZ_CMD_C_RodBanker
{
	BYTE							cbRodInfo;							//抢地主
};

//用户加倍
struct HLDDZ_CMD_C_Double
{
	BYTE							cbDoubleInfo;						//加倍信息
};

//游戏叫分
struct DDZCMD_S_StatusScore
{
	BYTE							bLandScore;							//地主分数
	LONG							lBaseScore;							//基础积分
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bScoreInfo[DDZGAME_PLAYER];			//叫分信息
	BYTE							bCardData[DDZGAME_PLAYER][20];			//手上扑克
	bool							bUserTrustee[DDZGAME_PLAYER];			//玩家托管
	BYTE							bBackCardData[DDZGAME_PLAYER];			//底牌扑克
	WORD						wOpenCard[DDZGAME_PLAYER];				//明牌  倍率相乘
	//历史积分
	long long						lTurnScore[DDZGAME_PLAYER];			//积分信息
	long long						lCollectScore[DDZGAME_PLAYER];			//积分信息
};
//送金蛋
struct DDZCMD_S_GoldEgg
{
	WORD                            bCurrentUser;                       //当前玩家
	BYTE                            bGoldEgg;                           //金蛋数目
};
//重新发牌（用于纠错）
struct DDZCMD_S_ReSedCrad
{
	WORD                            bTurnUser;                          //上一轮出牌用户
	BYTE							bTurnCount;							//上一轮出牌牌数目
	BYTE							bTurnData[MAX_COUNT];						//扑克列表
	BYTE							bCardCount;							//牌数目
	BYTE							bCardData[MAX_COUNT];						//扑克列表
};

//出牌轮数
struct DDZCMD_S_TurnCount
{
	WORD                            wTurnChairID;                       //玩家ID
	WORD                            wTurnCount;                         //轮数
};

//用户报警
struct DDZCMD_S_WarnInfo
{
	WORD                           wWarnUser;                           //报警玩家
	bool                           bWarn;                               //报警信息
	WORD                           wWarnNum;                            //报警时牌数目
};
//托管结构
struct DDZCMD_C_UserTrustee {
	WORD							wUserChairID;						//玩家椅子
	bool							bTrustee;							//托管标识
};

//出牌提示
struct DDZCMD_S_OutCardNotify
{
	BYTE							bOutCard;							//出牌标志
	WORD							wCurrentUser;						//当前用户
};
#endif

//用户出牌
struct DDZCMD_C_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	BYTE							cbCardData[MAX_COUNT];				//扑克数据
};

//用户出牌
struct HLDDZ_CMD_C_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	BYTE							cbCardData[MAX_COUNT];				//扑克数据
};
//用户出牌（服务端）
//struct CMD_S_OutCard
//{
//	BYTE							bCardCount;							//出牌数目
//	WORD				 			wCurrentUser;						//当前玩家
//	WORD							wOutCardUser;						//出牌玩家
//	BYTE							bCardData[MAX_COUNT];						//扑克列表
//	bool                            bIsNoGameEnd;                       //是否结束
//};

//出牌（客户端）
//struct CMD_C_OutCard
//{
//	BYTE							bCardCount;							//出牌数目
//	BYTE							bCardData[MAX_COUNT];						//扑克列表
//	WORD                            wOutCardUser;                       //出牌玩家
//};


//放弃出牌
//struct CMD_S_PassCard
//{
//	bool							bNewTurn;							//一轮开始
//	WORD				 			wPassUser;							//放弃玩家
//	WORD				 			wCurrentUser;						//当前玩家
//};

//--玩家断线重连标志
struct DDZCMD_S_UserOffLine
{
	WORD    m_userID;
	bool        m_isOnline;
};

//用户明牌
struct DDZCMD_S_OPENCARD
{
	WORD	bOpenUser;
};

//用户明牌
struct DDZCMD_C_OPENCARD
{
	WORD	bOpenUser;
	WORD		bNum;
};

//用户倒反
struct DDZCMD_C_DAOFAN
{
	WORD                            m_bUserDaofanID;
};
//用户准备好了
struct DDZCMD_C_START
{
	WORD							bCurrentUser;                     //开始用户 
	bool								bOpenCard;							//明牌开始
};
struct DDZCMD_S_READY
{
	WORD							bCurrentUser;                     //开始用户 
	bool								bOpenCard;							//明牌开始
};
struct DDZCMD_S_DAOFAN
{
	WORD                            m_bUserDaofanID;								//当前用户
};

//倍率变化
struct DDZCMD_S_BEILV
{
	WORD beillu[DDZGAME_PLAYER];							//倍率
};
//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif