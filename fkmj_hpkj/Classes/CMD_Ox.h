#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#include "GameDefine.h"
#pragma pack(1)

#define GAME_RULE_SUIJIZHUANG		1
#define GAME_RULE_QIANGZHUANG		2
#define GAME_RULE_NOT_WAITE				3

#define GAME_PLAYER						5									//游戏人数
#define MAX_COUNT						5									//最大数目

//结束原因
#define GER_NO_PLAYER					0x10								//没有玩家


//游戏状态
#define GS_TK_FREE						GAME_STATUS_FREE					//等待开始
#define GS_TK_CALL						GAME_STATUS_PLAY					//叫庄状态
#define GS_TK_SCORE						GAME_STATUS_PLAY+1					//下注状态
#define GS_TK_PLAYING						GAME_STATUS_PLAY+2					//游戏进行


#define SERVER_LEN						32 

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START				100									//游戏开始
#define SUB_S_ADD_SCORE					101									//加注结果
#define SUB_S_PLAYER_EXIT				102									//用户强退
#define SUB_S_SEND_CARD					103									//发牌消息
#define SUB_S_GAME_END					104									//游戏结束
#define SUB_S_OPEN_CARD					105									//用户摊牌
#define SUB_S_CALL_BANKER				106									//用户叫庄
#define SUB_S_ALL_CARD					107									//发牌消息
#define SUB_S_AMDIN_COMMAND				108									//系统控制


#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#define myatoi      _wtoi
#define myatoi64	_wtoi64
#endif

//游戏状态
struct CMD_S_StatusFree
{
	LONGLONG							lCellScore;							//基础积分

	//历史积分
	LONGLONG							lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG							lCollectScore[GAME_PLAYER];			//积分信息
	TCHAR								szGameRoomName[SERVER_LEN];			//房间名称
};

//游戏状态
struct CMD_S_StatusCall
{
	WORD								wCallBanker;						//叫庄用户
	BYTE                                cbDynamicJoin;                      //动态加入 
	BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
	BYTE								bCallStatus[GAME_PLAYER];			//叫庄状态

	//扑克信息
	BYTE								cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克
	//历史积分
	LONGLONG							lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG							lCollectScore[GAME_PLAYER];			//积分信息
};

//游戏状态
struct CMD_S_StatusScore
{
	//下注信息
	BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
	BYTE                                cbDynamicJoin;                      //动态加入
	LONGLONG							lTableScore[GAME_PLAYER];			//下注数目
	WORD								wBankerUser;						//庄家用户

	//扑克信息
	BYTE								cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克
	//历史积分
	LONGLONG							lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG							lCollectScore[GAME_PLAYER];			//积分信息
	TCHAR								szGameRoomName[SERVER_LEN];			//房间名称
};

//游戏状态
struct CMD_S_StatusPlay
{
	//状态信息
	BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
	BYTE                                cbDynamicJoin;                      //动态加入
	LONGLONG							lTableScore[GAME_PLAYER];			//下注数目
	WORD								wBankerUser;						//庄家用户

	//扑克信息
	BYTE								cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克
	BYTE								bOxCard[GAME_PLAYER];				//牛牛数据

	//历史积分
	LONGLONG							lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG							lCollectScore[GAME_PLAYER];			//积分信息
	TCHAR								szGameRoomName[SERVER_LEN];			//房间名称
};

//用户叫庄
struct CMD_S_CallBanker
{
	WORD								wCallChairID;						//叫庄用户
	BYTE								cbCallDouble;						//倍数
};

//游戏开始
struct CMD_S_GameStart
{
	//下注信息
	WORD								wBankerUser;						//庄家用户
	BYTE								cbBankerMaxDouble;				//庄家倍数
	bool								bQiangPlayer[GAME_PLAYER];			//抢庄玩家
};

//用户下注
struct CMD_S_AddScore
{
	WORD								wAddScoreUser;						//加注用户
	LONGLONG							lAddScoreCount;						//加注数目
};

//游戏结束
struct CMD_S_GameEnd
{
	LONGLONG							lGameTax[GAME_PLAYER];				//游戏税收
	LONGLONG							lGameScore[GAME_PLAYER];			//游戏得分
	BYTE								cbCardData[GAME_PLAYER];			//用户扑克
};

//发牌数据包
struct CMD_S_SendCard
{
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//用户扑克
};

//发牌数据包
struct CMD_S_AllCard
{
	bool								bAICount[GAME_PLAYER];
	BYTE                            cbPlayStatus[GAME_PLAYER];          //用户状态
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//用户扑克
	bool                            bIsQiang;                         //是否是抢庄
};

//用户退出
struct CMD_S_PlayerExit
{
	WORD								wPlayerID;							//退出用户
};

//用户摊牌
struct CMD_S_Open_Card
{
	WORD								wPlayerID;							//摊牌用户
	BYTE								bOpen;								//摊牌标志
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_CALL_BANKER				1									//用户叫庄
#define SUB_C_ADD_SCORE					2									//用户加注
#define SUB_C_OPEN_CARD					3									//用户摊牌
#define SUB_C_SPECIAL_CLIENT_REPORT     4                                   //特殊终端
#define SUB_C_AMDIN_COMMAND				5									//系统控制

//用户叫庄
struct CMD_C_CallBanker
{
	BYTE								bBanker;							//做庄标志
};

//终端类型
struct CMD_C_SPECIAL_CLIENT_REPORT        
{
	WORD                                wUserChairID;                       //用户方位
};

//用户加注
struct CMD_C_AddScore
{
	LONGLONG							lScore;								//加注数目
};

//用户摊牌
struct CMD_C_OxCard
{
	BYTE								bOX;								//牛牛标志
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//控制定义
#define SUB_C_AMDIN_COMMAND			5									//管理员命令

#define RQ_OPTION_CANCLE		1		//取消
#define RQ_OPTION_QUERYING		2		//查询
#define RQ_OPTION_SETING		3		//设置

#define CHEAT_TYPE_LOST		0		//输
#define CHEAT_TYPE_WIN		1		//赢

struct CMD_C_AdminReq
{
	BYTE cbReqType;							//请求类型
	BYTE cbCheatCount;						//控制次数
	BYTE cbCheatType;						//控制类型
	DWORD dwGameID;							//玩家标识
};

#pragma pack()
#endif
