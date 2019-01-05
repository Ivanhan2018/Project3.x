#ifndef __PPC_PROTOCOL_H__
#define __PPC_PROTOCOL_H__

#include "Define.h"
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////


//公共宏定义

#define KIND_ID						108									//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("碰碰车")						//游戏名字

//版本信息
#define VERSION_SERVER			    PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

//状态定义
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY						//下注状态
#define	GS_GAME_END					GAME_STATUS_PLAY+1						//结束状态
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+2						//结束状态

//区域索引
#define ID_TIAN_MEN					1									//顺门
#define ID_DI_MEN					2									//左边角
#define ID_XUAN_MEN					3									//桥
#define ID_HUANG_MEN				4									//对门

//玩家索引
#define BANKER_INDEX				0									//庄家索引
#define SHUN_MEN_INDEX				1									//顺门索引
#define DUI_MEN_INDEX				2									//对门索引
#define DAO_MEN_INDEX				3									//倒门索引
#define HUAN_MEN_INDEX				4									//倒门索引

#define AREA_COUNT					8									//区域数目


//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率

//机器人信息
struct ppcTagRobotInfo
{
	int nChip[6];														//筹码定义
	int nAreaChance[AREA_COUNT];										//区域几率
	TCHART szCfgFileName[260];										//配置文件
	int	nMaxTime;														//最大赔率

	ppcTagRobotInfo()
	{
		int nTmpChip[6] = {1,5,10,100,500,1000};
		int nTmpAreaChance[AREA_COUNT] = {1, 2, 2, 5, 10, 10, 10, 10};
// 		TCHART szTmpCfgFileName[MAX_PATH] = _T("BumperCarBattle.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
// 		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//记录信息
struct ppcTagServerGameRecord
{
	BYTE							bWinMen[AREA_COUNT+1];						//顺门胜利
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_TO_ANDROID			98									//庄家取钱
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
#define SUB_S_CHECK_IMAGE			109									//取消申请
#define SUB_S_ADMIN_COMMDN			110									//系统控制


//失败结构
struct ppcCMD_S_PlaceJettonFail
{
	WORD						wPlaceUser;							//下注玩家
	BYTE						lJettonArea;						//下注区域
	SCORE						lPlaceScore;						//当前下注
};

//更新积分
struct ppcCMD_S_ChangeUserScore
{
	WORD						wChairID;							//椅子号码
	DOUBLE						lScore;								//玩家积分

	//庄家信息
	WORD						wCurrentBankerChairID;				//当前庄家
	BYTE						cbBankerTime;						//庄家局数
	DOUBLE						lCurrentBankerScore;				//庄家分数
};

//上庄列表玩家
struct ppcTagBankerListPlayer
{
	ppcTagBankerListPlayer(){memset(this, 0, sizeof(*this));}
	WORD wChairID;				//	椅子ID
	DWORD dwUserID;				//	用户ID
	DOUBLE lScore;				//	金币数
	BYTE cbGender;				//	性别
	TCHART szNickName[32];		//	昵称
};

//上庄列表
struct ppcCMD_S_ApplyBankerList
{
	ppcCMD_S_ApplyBankerList(){memset(this, 0, sizeof(*this));}
	BYTE cbCount;//	上庄列表玩家个数
	inline ppcTagBankerListPlayer & operator [](unsigned int);
	int getLength()const{return sizeof(*this) + cbCount * sizeof(ppcTagBankerListPlayer);}
};

inline ppcTagBankerListPlayer & ppcCMD_S_ApplyBankerList::operator [](unsigned int index)
{
	return ((ppcTagBankerListPlayer *)((unsigned char *)this + sizeof(*this)))[index];
}

//申请庄家
struct ppcCMD_S_ApplyBanker
{
	WORD						wApplyUser;							//申请玩家（椅子ID）
};

//取消申请
struct ppcCMD_S_CancelBanker
{
	TCHART					    szCancelUser[32];					//取消玩家
	WORD						wChairID;							//椅子号码
};

//切换庄家
struct ppcCMD_S_ChangeBanker
{
	WORD						wBankerUser;						//当庄玩家
	TCHART						szBankerNickName[32];				//	庄家昵称
	SCORE						lBankerScore;						//庄家金币
};

//空闲状态
struct ppcCMD_S_StatusFree
{
	//全局信息
	BYTE						cbTimeLeave;						//剩余时间

	//玩家信息
	SCORE						lUserMaxScore;						//玩家金币

	//庄家信息
	WORD						wBankerUser;						//当前庄家
	TCHART						szBankerNickName[32];				//	庄家昵称
	WORD						cbBankerTime;						//庄家局数
	SCORE						lBankerWinScore;					//庄家成绩
	SCORE						lBankerScore;						//庄家分数
	bool						bEnableSysBanker;					//系统做庄

	//控制信息
	SCORE						lApplyBankerCondition;				//申请条件
	SCORE						lAreaLimitScore;					//区域限制
	int							CheckImage;

	TCHART						szGameRoomName[32];					//房间名称 
};

//游戏状态
struct ppcCMD_S_StatusPlay
{
	//全局下注
	SCORE						lAllJettonScore[AREA_COUNT+1];		//全体总注

	//玩家下注
	SCORE						lUserJettonScore[AREA_COUNT+1];		//个人总注

	//玩家积分
	SCORE						lUserMaxScore;						//最大下注							

	//控制信息
	SCORE						lApplyBankerCondition;				//申请条件
	SCORE						lAreaLimitScore;					//区域限制

	//扑克信息
	BYTE						cbTableCardArray[1][1];				//桌面扑克

	//庄家信息
	WORD						wBankerUser;						//当前庄家
	TCHART						szBankerNickName[32];				//	庄家昵称
	WORD						cbBankerTime;						//庄家局数
	SCORE						lBankerWinScore;					//庄家赢分
	SCORE						lBankerScore;						//庄家分数
	bool						bEnableSysBanker;					//系统做庄

	//结束信息
	SCORE						lEndBankerScore;					//庄家成绩
	SCORE						lEndUserScore;						//玩家成绩
	SCORE						lEndUserReturnScore;				//返回积分
	SCORE						lEndRevenue;						//游戏税收

	//全局信息
	BYTE						cbTimeLeave;						//剩余时间
	BYTE						cbGameStatus;						//游戏状态
	int							CheckImage;
	TCHART						szGameRoomName[32];					//房间名称 
};

//游戏空闲
struct ppcCMD_S_GameFree
{
	BYTE						cbTimeLeave;						//剩余时间
};

//游戏开始
struct ppcCMD_S_GameStart
{
	WORD						wBankerUser;						//当前庄家
	SCORE						lBankerScore;						//庄家金币
	SCORE						lUserMaxScore;						//我的金币
	BYTE						cbTimeLeave;						//剩余时间	
	bool						bContiueCard;						//继续发牌
	int							nChipRobotCount;					//人数上限 (下注机器人)
};

//用户下注
struct ppcCMD_S_PlaceJetton
{
	WORD						wChairID;							//用户位置
	BYTE						cbJettonArea;						//筹码区域
	SCORE						lJettonScore;						//加注数目
	BYTE						cbAndroid;							//机器人
};

//游戏结束
struct ppcCMD_S_GameEnd
{
	//下局信息
	BYTE						cbTimeLeave;						//剩余时间

	//扑克信息
	BYTE						cbTableCardArray[1][1];				//桌面扑克
	BYTE						cbLeftCardCount;					//扑克数目

	BYTE						bcFirstCard;
 
	//庄家信息
	SCORE						lBankerScore;						//庄家成绩
	SCORE						lBankerTotallScore;					//庄家成绩
	int							nBankerTime;						//做庄次数
								
	//玩家成绩
	SCORE						lUserScore;							//玩家成绩
	SCORE						lUserReturnScore;					//返回积分

	//全局信息
	SCORE						lRevenue;							//游戏税收
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_CONTINUE_CARD			4									//继续发牌
#define SUB_C_CHECK_IMAGE			5									//继续发牌
#define SUB_C_ADMIN_COMMDN			6									//系统控制
#define SUB_C_GET_ACCOUNT			7									//获取帐号
#define SUB_C_CHECK_ACCOUNT			8									//获取帐号
#define SUB_S_SCORE_RESULT			9									//积分结果
#define SUB_S_ACCOUNT_RESULT		10									//帐号结果
#define SUB_C_GET_PLAYLIST			11									//上庄列表

//用户请求下注
struct ppcCMD_C_PlaceJetton
{
	BYTE						cbJettonArea;						//筹码区域
	SCORE						lJettonScore;						//加注数目
};

struct ppcCMD_C_CheckImage
{
	int Index;
};

//////////////////////////////////////////////////////////////////////////

#define IDM_ADMIN_COMMDN	WM_USER+1000
#define IDM_GET_ACCOUNT		WM_USER+1001
#define IDM_CHEAK_ACCOUNT	WM_USER+1002

//控制区域信息
struct ppcTagControlInfo
{
	BYTE cbControlArea;						//控制区域
};

//服务器控制返回
#define	 S_CR_FAILURE				0		//失败
#define  S_CR_UPDATE_SUCCES			1		//更新成功
#define	 S_CR_SET_SUCCESS			2		//设置成功
#define  S_CR_CANCEL_SUCCESS		3		//取消成功
struct ppcCMD_S_ControlReturns
{
	BYTE cbReturnsType;				//回复类型
	BYTE cbControlArea;				//控制区域
	BYTE cbControlTimes;			//控制次数
};


//客户端控制申请
#define  C_CA_UPDATE				1		//更新
#define	 C_CA_SET					2		//设置
#define  C_CA_CANCELS				3		//取消
struct ppcCMD_C_ControlApplication
{
	BYTE cbControlAppType;			//申请类型
	BYTE cbControlArea;				//控制区域
	BYTE cbControlTimes;			//控制次数
};

struct ppcCMD_C_CheakAccount
{
	TCHART szUserAccount[32];
};

struct ppcCMD_S_ScoreResult
{
	SCORE lUserJettonScore[AREA_COUNT+1];	//个人总注
};

struct ppcCMD_S_AccountResult
{
	TCHART szAccount[100][32];					//帐号昵称
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif	//	__PPC_PROTOCOL_H__
