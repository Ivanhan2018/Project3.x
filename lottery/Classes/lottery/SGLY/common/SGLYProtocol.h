#ifndef __SGLY_PROTOCOL_H__
#define __SGLY_PROTOCOL_H__

#include "Define.h"
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						32								//游戏 I D
#define GAME_PLAYER					100								//游戏人数

#define VERSION_SERVER				PROCESS_VERSION(6,0,3)			//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)			//程序版本

#define GAME_NAME					TEXT("水果乐园")				//游戏名字
#define CLIENT_MODULE_NAME			TEXT("SGDZ_CLIENT.exe")			//客户端模块名称
#define	SERVER_MODULE_NAME			TEXT("SGDZ_SERVER.DLL")			//服务端模块名称

//状态定义
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY				//下注状态
#define	GS_GAME_END					GAME_STATUS_PLAY+1				//结束状态

//玩家索引
#define ID_BAR					0									//BAR索引
#define ID_DOUBLE_SEVEN			1									//双七索引
#define ID_STAR					2									//星星索引
#define ID_WATERMELON			3									//西瓜索引
#define ID_BELL					4									//铃铛索引
#define ID_PAWPAW				5									//木瓜索引
#define ID_ORANGE				6									//橙子索引
#define ID_APPLE				7									//苹果索引

#define	PATH_STEP_NUMBER		24									//转轴格子数
#define JETTON_AREA_COUNT		8									//下注区域数量

//记录信息
struct SGLY_tagServerGameRecord
{
	DWORD	 	dwGameTimes;										//第几局
	BYTE		cbRecord;											//范围在ID_BAR到ID_APPLE
	bool		bGoodLuck;
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SGLY_SUB_S_GAME_FREE			99										//游戏空闲
#define SGLY_SUB_S_GAME_START			100										//游戏开始
#define SGLY_SUB_S_PLACE_JETTON			101										//用户下注
#define SGLY_SUB_S_GAME_END				102										//游戏结束
#define SGLY_SUB_S_APPLY_BANKER			103										//申请庄家
#define SGLY_SUB_S_CHANGE_BANKER		104										//切换庄家
#define SGLY_SUB_S_CHANGE_USER_SCORE	105										//更新积分
#define SGLY_SUB_S_SEND_RECORD			106										//游戏记录
#define SGLY_SUB_S_PLACE_JETTON_FAIL	107										//下注失败
#define SGLY_SUB_S_CANCEL_BANKER		108										//取消申请
#define SGLY_SUB_S_BIG_SMALL			109										//猜大小
#define SGLY_SUB_S_CHIP_UPDATE			110										//兑换筹码
#define SGLY_SUB_S_ADMIN_COMMDN			111										//特殊账号控制

//	更新筹码
struct SGLY_CMD_S_ChipUpdate
{
	WORD								wChairID;								//兑换筹码的玩家
	LONGLONG							lTotalScore;							//此玩家的总筹码数所对应的分数
};

//猜大小
struct SGLY_CMD_S_BigSmall
{
	bool								bWin;									//猜大小成功
	BYTE								cbBigSmall;								//大小的实际数值
	LONGLONG							lUserScore;								//玩家成绩
	LONGLONG							lBankerScore;							//庄家成绩
};

//用户下注
struct SGLY_CMD_S_PlaceJetton
{
	WORD								wChairID;								//用户位置
	BYTE								cbJettonArea;							//筹码区域
	LONGLONG							lJettonScore;							//加注数目
	LONGLONG							lTotalJettonScore[JETTON_AREA_COUNT];	//	所有下注额度
	bool								cbAndroid;
};

//失败结构
struct CMD_S_PlaceJettonFail
{
	BYTE								cbJettonArea;							//下注区域
	LONGLONG							lJettonScore;							//当前下注
};

//上庄列表玩家
struct SGLY_tagBankerListPlayer
{
	SGLY_tagBankerListPlayer(){memset(this, 0, sizeof(*this));}
	WORD wChairID;
	DWORD dwUserID;
	DOUBLE lScore;
	BYTE cbGender;
	TCHART szNickName[32];
};

//上庄列表
struct SGLY_CMD_S_ApplyBankerList
{
	SGLY_CMD_S_ApplyBankerList(){memset(this, 0, sizeof(*this));}
	BYTE cbCount;//	上庄列表玩家个数
	inline SGLY_tagBankerListPlayer & operator [](unsigned int);
	int getLength()const{return sizeof(*this) + cbCount * sizeof(SGLY_tagBankerListPlayer);}
};

struct SGLY_CMD_S_ApplyBankerListMax:public SGLY_CMD_S_ApplyBankerList
{
	SGLY_tagBankerListPlayer wApplyList[GAME_PLAYER];
};

inline SGLY_tagBankerListPlayer & SGLY_CMD_S_ApplyBankerList::operator [](unsigned int index)
{
	return ((SGLY_tagBankerListPlayer *)((unsigned char *)this + sizeof(*this)))[index];
}

//申请庄家
struct SGLY_CMD_S_ApplyBanker
{
	WORD								wApplyUser;								//申请玩家
};

//取消申请
struct SGLY_CMD_S_CancelBanker
{
	TCHART								szCancelUser[32];						//取消玩家
	WORD								wChairID;								//取消玩家chairID
};

//切换庄家
struct SGLY_CMD_S_ChangeBanker
{
	WORD								wBankerUser;							//当庄玩家
	TCHART								szBankerNickName[32];					//	庄家昵称
	LONGLONG							lBankerScore;							//庄家金币
	LONGLONG							lBankerChip;							//	庄家当前持有筹码
};

//游戏状态
struct SGLY_CMD_S_StatusFree
{
	LONGLONG							lCurrentJetton[GAME_PLAYER];			//数据量较大，同步每个玩家的筹码量
	DWORD								dwChipRate;								//筹码比率
	//全局信息
	BYTE								cbTimeLeave;							//剩余时间
	//玩家信息
	LONGLONG							lUserMaxScore;							//玩家金币
	//庄家信息
	WORD								wBankerUser;							//当前庄家
	TCHART								szBankerNickName[32];					//	庄家昵称
	WORD								wBankerTime;							//庄家局数
	LONGLONG							lBankerWinScore;						//庄家成绩
	LONGLONG							lBankerScore;							//庄家分数
	bool								bEnableSysBanker;						//系统做庄
	//控制信息
	LONGLONG							lApplyBankerCondition;					//申请条件
	LONGLONG							lAreaLimitScore[JETTON_AREA_COUNT];		//区域限制

	//房间名称
	TCHART								szRoomName[32];							//房间名称
};

//游戏状态
struct SGLY_CMD_S_StatusPlay
{
	LONGLONG							lCurrentJetton[GAME_PLAYER];			//数据量较大，同步每个玩家的筹码量
	DWORD								dwChipRate;								//筹码比率
	//全局下注
	LONGLONG							lTotalJettonScore[JETTON_AREA_COUNT];
	//玩家下注
	LONGLONG							lUserAreaScore[JETTON_AREA_COUNT];
	//各区域限注
	LONGLONG							lAreaScoreLimit[JETTON_AREA_COUNT];		

	//扑克信息
	BYTE							cbWinArea;							//本次停止的位置
	BYTE							cbGoodLuckType;						//开中GoodLuck
	BYTE							cbPaoHuoCheCount;
	BYTE							cbPaoHuoCheArea[7];

	//玩家积分
	LONGLONG							lUserMaxScore;							//最大下注							
	//控制信息
	LONGLONG							lApplyBankerCondition;					//申请条件
	//扑克信息
	BYTE								cbCardCount[2];							//扑克数目
	BYTE								cbTableCardArray[2][3];					//桌面扑克
	//庄家信息
	WORD								wBankerUser;							//当前庄家
	TCHART								szBankerNickName[32];					//	庄家昵称
	WORD								cbBankerTime;							//庄家局数
	LONGLONG							lBankerWinScore;						//庄家赢分
	LONGLONG							lBankerScore;							//庄家分数
	bool								bEnableSysBanker;						//系统做庄
	//结束信息
	LONGLONG							lEndBankerScore;						//庄家成绩
	LONGLONG							lEndUserScore;							//玩家成绩
	LONGLONG							lEndUserReturnScore;					//返回积分
	LONGLONG							lEndRevenue;							//游戏税收
	//全局信息
	BYTE								cbTimeLeave;							//剩余时间
	BYTE								cbGameStatus;							//游戏状态

	//房间名称
	TCHART								szRoomName[32];							//房间名称
};

//游戏空闲
struct SGLY_CMD_S_GameFree
{
	BYTE								cbTimeLeave;							//剩余时间
	BYTE								cbGameRecordCount;
	SGLY_tagServerGameRecord			cbGameRecord[8];
	//BYTE								cbGameRecord;							//本次开出的结果
	//LONGLONG							lGameTimes;								//当前是游戏启动以来的第几局
	LONGLONG							lApplyBankerCondition;
	DWORD								wApplyUserCount;
	LONGLONG							lBankerTotallScore;						//庄家成绩
	int									iBankerTime;							//做庄次数
};

//游戏开始
struct SGLY_CMD_S_GameStart
{
	WORD								wBankerUser;							//庄家位置
	LONGLONG							lBankerScore;							//庄家金币
	LONGLONG							lUserMaxScore;							//我的金币
	BYTE								cbTimeLeave;							//剩余时间
	LONGLONG							lAreaLimitScore[JETTON_AREA_COUNT];		//各区域可下分
};

//游戏结束
struct SGLY_CMD_S_GameEnd
{
	//下局信息
	BYTE								cbTimeLeave;							//剩余时间
	//扑克信息
	BYTE								cbWinArea;								//本次停止的位置
	BYTE								cbGoodLuckType;							//开中GoodLuck
	BYTE								cbPaoHuoCheCount;
	BYTE								cbPaoHuoCheArea[7];
	//庄家信息
	LONGLONG							lBankerScore;							//庄家成绩
	LONGLONG							lBankerTotallScore;						//庄家成绩
	int									iBankerTime;							//做庄次数
	//玩家成绩
	LONGLONG							lUserScore;								//玩家成绩
	LONGLONG							lUserReturnScore;						//返回积分
	//全局信息
	LONGLONG							lRevenue;								//游戏税收
	LONGLONG							lApplyBankerCondition;
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SGLY_SUB_C_PLACE_JETTON			1										//用户下注
#define SGLY_SUB_C_APPLY_BANKER			2										//申请庄家
#define SGLY_SUB_C_CANCEL_BANKER		3										//取消申请
#define SGLY_SUB_C_BIG_SMALL			4										//猜大小
#define SGLY_SUB_C_EXCHANGE				5										//兑换筹码
#define SGLY_SUB_C_ADMIN_COMMDN			6										//特殊账号控制
#define SGLY_SUB_C_GET_PLAYLIST			7										//上庄列表

//兑换筹码
struct SGLY_CMD_C_Exchange
{
	LONGLONG							lExchangeScore;							//兑换成筹码的总分数（不是本次要兑换的分数，而是总的分数）
};

//猜大小	（小：0；大：1）
struct SGLY_CMD_C_BigSmall
{
	BYTE								cbBigSmall;
};

//用户下注
struct SGLY_CMD_C_PlaceJetton
{
	BYTE								cbJettonArea;							//筹码区域
	LONGLONG							lJettonScore;							//加注数目
};

//操作结果
enum SGLY_enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};

//记录信息
struct SGLY_tagClientGameRecord
{
	SGLY_enOperateResult				enOperateFlags;							//操作标识
	BYTE								cbPlayerCount;							//闲家点数
	BYTE								cbBankerCount;							//庄家点数
	BYTE								cbKingWinner;							//天王赢家
	bool								bPlayerTwoPair;							//对子标识
	bool								bBankerTwoPair;							//对子标识
};

//历史记录
#define MAX_SCORE_HISTORY			1000								//历史个数
#define MAX_SHOW_HISTORY			10									//一次最多显示的历史记录个数

//同时显示申请上庄玩家数量
#define MAX_SCORE_BANKER			4
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//控制区域信息
struct SGLY_tagControlInfo
{
	BYTE cbControlArea;						//控制区域
};

//服务器控制返回
#define	 S_CR_FAILURE				0		//失败
#define  S_CR_UPDATE_SUCCES			1		//更新成功
#define	 S_CR_SET_SUCCESS			2		//设置成功
#define  S_CR_CANCEL_SUCCESS		3		//取消成功

struct SGLY_CMD_S_ControlReturns
{
	BYTE cbReturnsType;				//回复类型
	BYTE cbControlArea;				//控制区域
	BYTE cbControlTimes;			//控制次数
};

//客户端控制申请
#define  C_CA_UPDATE				1		//更新
#define	 C_CA_SET					2		//设置
#define  C_CA_CANCELS				3		//取消
struct SGLY_CMD_C_ControlApplication
{
	BYTE cbControlAppType;			//申请类型
	BYTE cbControlArea;				//控制区域
	BYTE cbControlTimes;			//控制次数
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif	//	__SGLY_PROTOCOL_H__
