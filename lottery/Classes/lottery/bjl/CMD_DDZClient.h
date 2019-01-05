#ifndef CMD_GAMECLIENT_HEAD_FILE
#define CMD_GAMECLIENT_HEAD_FILE
#include "Define.h"

#pragma pack(1)//<by hxh>
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
//#define GAME_SCENE_PLAY				GAME_STATUS_PLAY					//游戏进行
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

////逻辑类型
//#define CT_ERROR					0									//错误类型
//#define CT_SINGLE					1									//单牌类型
//#define CT_DOUBLE					2									//对牌类型
//#define CT_THREE					3									//三条类型
//#define CT_SINGLE_LINE				4									//单连类型
//#define CT_DOUBLE_LINE				5									//对连类型
//#define CT_THREE_LINE				6									//三连类型
//#define CT_THREE_TAKE_ONE			7									//三带一单
//#define CT_THREE_TAKE_TWO			8									//三带一对
//#define CT_FOUR_TAKE_ONE			9									//四带两单
//#define CT_FOUR_TAKE_TWO			10									//四带两对
//#define CT_BOMB_CARD				11									//炸弹类型
//#define CT_MISSILE_CARD				12									//火箭类型

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

// by hxh 20160707
inline const char * BET_AREA_NAME(BYTE cbBetArea){
	const char * s_szArr[8]={"0Xian","1Ping","2Zhuang","3XianTianWang","4ZhuangTianWang","5TongDianPing","6XianDuiZi","7ZhuangDuiZi"};
	const char * s=(cbBetArea>=0&&cbBetArea<8?s_szArr[cbBetArea]:"Unknown");
	return s;
}

// 测试用<by hxh>
inline const char * GetCardName(unsigned char cbCardData)
{
	//获取花色={0方块,16梅花,32红桃,48黑桃,64王}
	BYTE cbCardColor=cbCardData&LOGIC_MASK_COLOR;
	//获取数值
	BYTE cbCardValue=cbCardData&LOGIC_MASK_VALUE;

	//有效判断
	bool bLegal=false;
	if ((cbCardData==0x4E)||(cbCardData==0x4F))
		bLegal=true;
	if ((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D)) 
		bLegal=true;

	if(bLegal)
	{
		static const char * szCardName[5][15]={
			{"方块A","方块2","方块3","方块4","方块5","方块6","方块7","方块8","方块9","方块10","方块J","方块Q","方块K","",""},
			{"梅花A","梅花2","梅花3","梅花4","梅花5","梅花6","梅花7","梅花8","梅花9","梅花10","梅花J","梅花Q","梅花K","",""},
			{"红桃A","红桃2","红桃3","红桃4","红桃5","红桃6","红桃7","红桃8","红桃9","红桃10","红桃J","红桃Q","红桃K","",""},
			{"黑桃A","黑桃2","黑桃3","黑桃4","黑桃5","黑桃6","黑桃7","黑桃8","黑桃9","黑桃10","黑桃J","黑桃Q","黑桃K","",""},
			{"","","","","","","","","","","","","","小王","大王"},
		};
		unsigned char nColorIndex=(cbCardData & 0xf0)>>4;
		unsigned char nValueIndex=(cbCardData & 0x0f)-1;
		return szCardName[nColorIndex][nValueIndex];
	}
	else
		return "";
}
//struct	CMD_C_HumanArea
//{
//	LONGLONG lHumanArea[AREA_MAX];			//各区域真人加注
//	//BYTE	cbGameOver;						//游戏是否结束
//};


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

	// add by hxh 20160627
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

	// add by hxh 20160627
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
	DOUBLE						lPlayAllScore;						    //玩家成绩<by hxh>

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
//切换庄家<by hxh>
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

//提示消息<by hxh>
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
#define SUB_S_TIP_INFO			    110									//提示消息<by hxh>
#define SUB_S_CANCEL_JETTON			111									//取消下注<by hxh>

//游戏开始
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;						//剩余时间

	WORD							wBankerUser;						//庄家位置
	LONGLONG						lBankerScore;						//庄家金币

	LONGLONG						lPlayBetScore;						//玩家最大下注
	LONGLONG						lPlayFreeSocre;						//玩家自由金币

	int								nChipRobotCount;					//人数上限 (下注机器人)
	// by hxh 20160627
#if 0
	LONGLONG						nListUserCount;						//列表人数
	int								nAndriodCount;						//机器人列表人数
#endif
};

#pragma pack()//<by hxh>

#endif
