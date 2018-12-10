#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#include "datastream.h"

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#pragma pack(8)
//////////////////////////////////////////////////////////////////////////////////

#define MDM_CM_SYSTEM				10//1000								//系统命令

#define SUB_CM_SYSTEM_MESSAGE		100//1									//系统消息
#define SUB_GR_LOGON_AFFICHE        105                                 //公告信息   彭文添加
#define SUB_CM_ACTION_MESSAGE		2									//动作消息
#define SUB_CM_DOWN_LOAD_MODULE		3									//下载消息

//类型掩码
#define SMT_CHAT					0x0001								//聊天消息
#define SMT_EJECT					0x0002								//弹出消息
#define SMT_GLOBAL					0x0004								//全局消息
#define SMT_PROMPT					0x0008								//提示消息
#define SMT_TABLE_ROLL				0x0010								//滚动消息

//控制掩码
#define SMT_CLOSE_ROOM				0x0040//0x0100								//关闭房间
#define SMT_CLOSE_GAME				0x0010//0x0200								//关闭游戏
#define SMT_CLOSE_LINK				0x0020//0x0400								//中断连接

//系统消息
struct CMD_CM_SystemMessage
{
	word							wType;								//消息类型
	word							wLength;							//消息长度
	char							szString[1024];						//消息内容
};
//////////////////////////////////////////////////////////////////////////////////

#define MDM_GR_LOGON				1									//登录信息

//登录模式
#define SUB_GR_LOGON_USERID			2//1									//I D 登录
#define SUB_GR_LOGON_MOBILE			2									//手机登录
#define SUB_GR_LOGON_ACCOUNTS		3									//帐户登录

//登录结果
#define SUB_GR_LOGON_SUCCESS		100									//登录成功
#define SUB_GR_LOGON_FAILURE		101									//登录失败
#define SUB_GR_LOGON_FINISH			102									//登录完成

//升级提示
#define SUB_GR_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////
//
//升级提示
struct CMD_GR_UpdateNotify
{
	//升级标志
	byte							cbMustUpdatePlaza;					//强行升级
	byte							cbMustUpdateClient;					//强行升级
	byte							cbAdviceUpdateClient;				//建议升级

	//当前版本
	dword							dwCurrentPlazaVersion;				//当前版本
	dword							dwCurrentFrameVersion;				//当前版本
	dword							dwCurrentClientVersion;				//当前版本
	char							szDownLoadHttp[128];				//描述消息
};
//房间 ID 登录
struct CMD_GR_LogonUserID
{
	dword							dwPlazaVersion;						//广场版本
	///dword							dwFrameVersion;						//框架版本
	dword							dwProcessVersion;					//进程版本

	//登录信息
	dword							dwUserID;							//用户 I D
	char							szPassword[LEN_MD5];				//登录密码
	///char							szMachineID[LEN_MACHINE_ID];		//机器序列
	dword							wKindID;							//类型索引
};

//登录成功消息
struct CMD_GR_LogonSuccess
{
	unsigned int					dwUserID;							//用户 I D
	///dword							dwUserRight;						//用户权限
	///dword							dwMasterRight;						//管理权限
};

//登录失败
struct CMD_GR_LogonError
{
	unsigned int					lErrorCode;							//错误代码
	char							szErrorDescribe[128];				//错误消息
};

//////////////////////////////////////////////////////////////////////////////////
//配置命令

#define MDM_GR_CONFIG				3//2									//配置信息

#define SUB_GR_CONFIG_COLUMN		103//100									//列表配置
#define SUB_GR_CONFIG_SERVER		100//101									//房间配置
#define SUB_GR_CONFIG_PROPERTY		102									//道具配置
#define SUB_GR_CONFIG_FINISH		104//103									//配置完成
#define SUB_GR_CONFIG_USER_RIGHT	104									//玩家权限
//////////////////////////////////////////////////////////////////////////////////

//列表配置
struct CMD_GR_ConfigColumn
{
	byte							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItem[MAX_COLUMN];				//列表描述
};

//房间配置
struct CMD_GR_ConfigServer
{
#if 1
	//房间属性
	word							wKindID;							//类型 I D
	word							wTableCount;						//桌子数目
	word							wChairCount;						//椅子数目
	dword							dwServerRule;						//视频地址

	//扩展配置
	word							wServerType;						//游戏类型
	byte							cbHideUserInfo;						//隐藏信息
#else
	//房间属性
	word							wTableCount;						//桌子数目
	word							wChairCount;						//椅子数目

	//房间配置
	word							wServerType;						//房间类型
	dword							dwServerRule;						//房间规则
#endif
};

//道具配置
struct CMD_GR_ConfigProperty
{
	byte							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//道具描述
};

//玩家权限
struct CMD_GR_ConfigUserRight
{
	dword							dwUserRight;						//玩家权限
};
//////////////////////////////////////////////////////////////////////////////////

#define MDM_GR_USER					2//3									//用户信息

//用户动作
#define SUB_GR_USER_RULE			1									//用户规则
#define SUB_GR_USER_LOOKON			2									//旁观请求
#define SUB_GR_USER_SITDOWN			3									//坐下请求
#define SUB_GR_USER_STANDUP			4									//起立请求
#define SUB_GR_USER_INVITE			5									//用户邀请
#define SUB_GR_USER_INVITE_REQ		6									//邀请请求
#define SUB_GR_USER_REPULSE_SIT  	7									//拒绝玩家坐下
#define SUB_GR_USER_KICK_USER       8                                   //踢出用户
#define SUB_GR_USER_INFO_REQ        9                                   //请求用户信息
#define SUB_GR_USER_CHAIR_REQ       10                                  //请求更换位置
#define SUB_GR_USER_CHAIR_INFO_REQ  11                                  //请求椅子用户信息

//起立请求
struct CMD_GR_UserStandUp
{
	word							wTableID;							//桌子位置
	word							wChairID;							//椅子位置
	byte							cbForceLeave;						//强行离开
};

//用户状态
#define SUB_GR_USER_ENTER			100									//用户进入
#define SUB_GR_USER_SCORE			102//101									//用户分数
#define SUB_GR_USER_STATUS			101//102									//用户状态
#define SUB_GR_SIT_FAILED			103									//请求失败

//用户分数
struct CMD_GR_UserScore
{
	dword							dwUserID;							//用户标识
	tagUserScore					UserScore;							//积分信息
};


#define SUB_GR_AUTO_QUEUE			106									//自动排队      王辰添加  
#define SUB_GR_QUEUE_COUNT          107                                 //队列人数      彭文添加
#define SUB_GR_AUTO_QUEUE_FAIL      108                                 //排队失败      彭文添加
#define SUB_GR_AUTO_QUEUE_SUCC		109 								//排队成功      彭文添加  
#define SUB_GR_AUTO_QUEUE_CANCEL    110                                 //取消排队      彭文添加
#define SUB_GR_AUTO_QUEUE_CANCEL_FALL 111                               //取消失败      彭文添加
#define SUB_GR_AUTO_QUEUE_CANCEL_SUCC  112                               //取消成功      彭文添加
#define SUB_GR_AUTO_QUEUE_ALREADY      113                               //已经排队了    彭文添加

//自动排队
struct CMD_GR_AutoQueue
{
	int								iAutoQueueNum;						//队伍人数
};

#define SUB_GR_NO_GOLD              150                                 //金币不足              彭文添加  2011-9-30前为宝石 SUB_GR_NO_GEMS
#define SUB_GR_LOW_GRADE            152                                 //等级不足 
#define SUB_GR_GO_HALL              151                                 //返回大厅              彭文添加


//聊天命令
#define SUB_GR_USER_CHAT			201									//聊天消息
#define SUB_GR_USER_EXPRESSION		202									//表情消息
#define SUB_GR_WISPER_CHAT			203									//私聊消息
#define SUB_GR_WISPER_EXPRESSION	204									//私聊表情
#define SUB_GR_COLLOQUY_CHAT		205									//会话消息
#define SUB_GR_COLLOQUY_EXPRESSION	206									//会话表情

//道具命令
#define SUB_GR_PROPERTY_BUY			300									//购买道具
#define SUB_GR_PROPERTY_SUCCESS		301									//道具成功
#define SUB_GR_PROPERTY_FAILURE		302									//道具失败
#define SUB_GR_PROPERTY_MESSAGE     303                                 //道具消息
#define SUB_GR_PROPERTY_EFFECT      304                                 //道具效应
#define SUB_GR_PROPERTY_TRUMPET		305                                 //喇叭消息

#define SUB_GR_GLAD_MESSAGE			400                                 //喜报消息

//用户语音聊天
struct CMD_GR_C_TableTalk
{
	enum TALK_TYPE
	{
		TYPE_FILE,        //语音
		TYPE_WORD,
		TYPE_DEFINE,		//系统语音
		TYPE_BIAOQING,
	};
	CMD_GR_C_TableTalk()
	{
		cbType = 0;
		strTalkSize = 0;
	}
	byte							cbType;								//类型
	byte							cbChairID;							//座位
	char							strString[128];						//自定义
	int								strTalkSize;
	char							strTalkData[20000];					//自定义
};

//请求坐下
struct CMD_GR_UserSitDown
{
	word							wTableID;							//桌子位置
	word							wChairID;							//椅子位置
	char							szTablePass[PASS_LEN];				//桌子密码
};

//用户状态
struct CMD_GR_UserStatus
{
	dword							dwUserID;							//用户标识
	tagUserStatus					UserStatus;							//用户状态
};

struct IPC_GF_UserInfo
{
	byte							cbCompanion;						//用户关系
	tagUserInfoHead					UserInfoHead;						//用户信息
};

//请求失败
struct CMD_GR_RequestFailure
{
	unsigned int					lErrorCode;							//错误代码
	char							szDescribeString[256];				//描述信息
};

//////////////////////////////////////////////////////////////////////////////////
//状态命令

#define MDM_GR_STATUS				4									//状态信息

#define SUB_GR_TABLE_INFO			100									//桌子信息
#define SUB_GR_TABLE_STATUS			101									//桌子状态

//////////////////////////////////////////////////////////////////////////////////

//桌子信息
struct CMD_GR_TableInfo
{
	word							wTableCount;						//桌子数目
	tagTableStatus					TableStatusArray[512];				//桌子状态
};

//桌子状态
struct CMD_GR_TableStatus
{
	word							wTableID;							//桌子号码
	tagTableStatus					TableStatus;						//桌子状态
};


//////////////////////////////////////////////////////////////////////////////////
//比赛命令

#define MDM_GR_MATCH				9									//比赛命令

#define SUB_GR_MATCH_FEE			400									//报名费用
#define SUB_GR_MATCH_NUM			401									//等待人数
#define SUB_GR_LEAVE_MATCH			402									//退出比赛
#define SUB_GR_MATCH_INFO			403									//比赛信息
#define SUB_GR_MATCH_WAIT_TIP		404									//等待提示
#define SUB_GR_MATCH_RESULT			405									//比赛结果
#define SUB_GR_MATCH_STATUS			406									//比赛状态
#define SUB_GR_MATCH_GOLDUPDATE		409									//金币更新
#define SUB_GR_MATCH_ELIMINATE		410									//比赛淘汰
#define SUB_GR_MATCH_JOIN_RESOULT	411									//加入结果



//费用提醒
struct CMD_GR_Match_Fee
{
	SCORE							lMatchFee;							//报名费用
	char							szNotifyContent[128];				//提示内容
};


//费用提醒
struct CMD_GR_Match_JoinResoult
{
	word							wSucess;
};

//比赛人数
struct CMD_GR_Match_Num
{
	dword							dwWaitting;							//等待人数
	dword							dwTotal;							//开赛人数
};

//赛事信息
struct CMD_GR_Match_Info
{
	char							szTitle[4][64];						//信息标题
	word							wGameCount;							//游戏局数
	word							wRank;								//当前名次
};

//提示信息
struct CMD_GR_Match_Wait_Tip
{
	SCORE							lScore;								//当前积分
	word							wRank;								//当前名次
	word							wCurTableRank;						//本桌名次
	word							wUserCount;							//当前人数
	word							wCurGameCount;						//当前局数
	word							wGameCount;							//总共局数
	word							wPlayingTable;						//游戏桌数
	char							szMatchName[LEN_SERVER];			//比赛名称
};

//比赛结果
struct CMD_GR_MatchResult
{	
	SCORE							lGold;								//金币奖励
	dword							dwIngot;							//元宝奖励
	dword							dwExperience;						//经验奖励
	char							szDescribe[256];					//得奖描述
};
							//最多描述

//金币更新
struct CMD_GR_MatchGoldUpdate
{
	SCORE							lCurrGold;							//当前金币
	SCORE							lCurrIngot;							//当前元宝
	dword							dwCurrExprience;					//当前经验
};

//////////////////////////////////////////////////////////////////////////////////
//私人场命令

#define MDM_GR_PRIVATE					10										//比赛命令

#define SUB_GR_PRIVATE_INFO				401									//私人场信息
#define SUB_GR_CREATE_PRIVATE			402									//创建私人场
#define SUB_GR_CREATE_PRIVATE_SUCESS	403									//创建私人场成功
#define SUB_GR_JOIN_PRIVATE				404									//加入私人场
#define SUB_GF_PRIVATE_ROOM_INFO		405									//私人场房间信息
#define SUB_GR_PRIVATE_DISMISS			406									//私人场请求解散
#define SUB_GF_PRIVATE_END				407									//私人场结算
#define SUB_GR_RIVATE_AGAIN				408									//创建私人场
#define SUB_GR_EXIT_SAVE				409									//离开但保存

//私人场信息
struct CMD_GR_Private_Info
{	
	word							wKindID;
	SCORE							lCostGold;
	byte							bPlayCout[4];							//玩家局数
	SCORE							lPlayCost[4];							//消耗点数
};

enum RoomType
{
	Type_Private,
	Type_Public,
};

//创建房间
struct CMD_GR_Create_Private
{	
	byte							cbGameType;								//游戏类型
	byte							bPlayCoutIdex;							//游戏局数
	byte							bGameTypeIdex;							//游戏类型
	dword							bGameRuleIdex;							//游戏规则
	char							stHttpChannel[LEN_NICKNAME];			//http获取
};

//创建房间
struct CMD_GR_Create_Private_Sucess
{	
	SCORE							lCurSocre;								//当前剩余
	dword							dwRoomNum;								//房间ID
};

//创建房间
struct CMD_GR_Join_Private
{	
	dword							dwRoomNum;								//房间ID
};

//私人场房间信息
struct CMD_GF_Private_Room_Info
{	
	byte			bPlayCoutIdex;		//玩家局数0 1，  8 或者16局
	byte			bGameTypeIdex;		//游戏类型
	dword		bGameRuleIdex;		//游戏规则

	byte			bStartGame;
	dword			dwPlayCout;			//游戏局数
	dword			dwRoomNum;
	dword			dwCreateUserID;
	dword			dwPlayTotal;		//总局数

	byte			cbRoomType;

	std::vector<int>	kWinLoseScore;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(bPlayCoutIdex);
		Stream_VALUE(bGameTypeIdex);
		Stream_VALUE(bGameRuleIdex);
		Stream_VALUE(bStartGame);
		Stream_VALUE(dwPlayCout);
		Stream_VALUE(dwRoomNum);
		Stream_VALUE(dwCreateUserID);
		Stream_VALUE(dwPlayTotal);
		Stream_VECTOR(kWinLoseScore);
		Stream_VALUE(cbRoomType);
	}
};

//解散房间
struct CMD_GR_Dismiss_Private
{	
	byte			bDismiss;			//解散
};

//重新加入
struct CMD_GR_Again_Private
{	
	char							stHttpChannel[LEN_NICKNAME];
	CMD_GR_Again_Private()
	{
		zeromemory(stHttpChannel,sizeof(stHttpChannel));
	}
};


//私人场解散信息
struct CMD_GF_Private_Dismiss_Info
{	
	CMD_GF_Private_Dismiss_Info()
	{
		zeromemory(this,sizeof(CMD_GF_Private_Dismiss_Info));
	}
	dword			dwDissUserCout;
	dword			dwDissChairID[MAX_CHAIR-1];
	dword			dwValue1;
	dword			dwNotAgreeUserCout;
	dword			dwNotAgreeChairID[MAX_CHAIR-1];
	dword			dwValue2;
};


#define MAX_PRIVATE_ACTION 8
//私人场结算信息
struct CMD_GF_Private_End_Info
{	
	std::vector<SCORE> lPlayerWinLose;
	std::vector<byte> lPlayerAction;
	systemtime		kPlayTime;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(lPlayerWinLose);
		Stream_VECTOR(lPlayerAction);
		Stream_VALUE_SYSTEMTIME(kPlayTime);
	}
};



//////////////////////////////////////////////////////////////////////////////////
//框架命令

#define MDM_GF_FRAME				101//100									//框架命令

//////////////////////////////////////////////////////////////////////////////////
//框架命令

//用户命令
#define SUB_GF_GAME_OPTION			1									//游戏配置
#define SUB_GF_USER_READY			2									//用户准备
#define SUB_GF_LOOKON_CONFIG		3									//旁观配置

//游戏配置
struct CMD_GF_GameOption
{
	byte							cbAllowLookon;						//旁观标志
	dword							dwFrameVersion;						//框架版本
	dword							dwClientVersion;					//游戏版本
};

//聊天命令
#define SUB_GF_USER_CHAT			10									//用户聊天
#define SUB_GF_USER_EXPRESSION		11									//用户表情
#define SUB_GR_TABLE_TALK			12									//用户聊天

//游戏信息
#define SUB_GF_GAME_STATUS			100									//游戏状态
#define SUB_GF_GAME_SCENE			101									//游戏场景
#define SUB_GF_LOOKON_STATUS		102									//旁观状态

//游戏环境
struct CMD_GF_GameStatus
{
	byte							cbGameStatus;						//游戏状态
	byte							cbAllowLookon;						//旁观标志
};

//系统消息
#define SUB_GF_SYSTEM_MESSAGE		200									//系统消息
#define SUB_GF_ACTION_MESSAGE		201									//动作消息

//////////////////////////////////////////////////////////////////////////////////
//游戏命令

#define MDM_GF_GAME					100//200									//游戏命令

//////////////////////////////////////////////////////////////////////////////////
//携带信息

//其他信息
#define DTP_GR_TABLE_PASSWORD		1									//桌子密码

//用户属性
#define DTP_GR_NICK_NAME			10									//用户昵称
#define DTP_GR_GROUP_NAME			11									//社团名字
#define DTP_GR_UNDER_WRITE			12									//个性签名

//附加信息
#define DTP_GR_USER_NOTE			20									//用户备注
#define DTP_GR_CUSTOM_FACE			21									//自定头像

//////////////////////////////////////////////////////////////////////////////////

//请求错误
#define REQUEST_FAILURE_NORMAL		0									//常规原因
#define REQUEST_FAILURE_NOGOLD		1									//金币不足
#define REQUEST_FAILURE_NOSCORE		2									//积分不足
#define REQUEST_FAILURE_PASSWORD	3									//密码错误

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif