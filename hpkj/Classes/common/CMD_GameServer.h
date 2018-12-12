#pragma once
#include "packet.h"
#pragma pack(1)

#define MDM_CM_SYSTEM				1000								//系统命令
///////////////////////////////////////////////////////////////////////////
// 登录命令 游戏房间
#define MDM_GR_LOGON				1									// 登录信息

#pragma mark -
#pragma mark 登录模式
#define SUB_GR_LOGON_USERID			1									// id 登录
#define SUB_GR_LOGON_MOBILE			2									// 手机登录
#define SUB_GR_LOGON_ACCOUNTS		3									// 账户登录

#define SUB_GR_LOGON_SUCCESS		100
#define SUB_GR_LOGON_ERROR		    101
#define SUB_GR_LOGON_FINISH		    102

//#pragma mark -
//#pragma mark 登录结果
#define SUB_GR_LOGON_FAILURE		101									// 登录失败
#define	SUB_GR_LOGON_NOTIFY			104                                 // 登陆提示TIPS     mojunxin

//#pragma mark -
//#pragma mark 升级提示
#define SUB_GR_UPDATE_NOTIFY		200									// 升级提示

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//#pragma mark -
//#pragma mark 设备类型
#define DEVICE_TYPE_ITOUCH			0x20								// iTouch
#define DEVICE_TYPE_IPHONE			0x40								// iPhone
#define DEVICE_TYPE_IPAD			0x80								// iPad
#define DEVICE_TYPE_ADNROID			0x13								// android


//#pragma mark -
//#pragma mark 视图模式
#define	VIEW_MODE_ALL				0x0001								// 全部可视
#define	VIEW_MODE_PART				0x0002								// 部分可视

//#pragma mark -
//#pragma mark 信息模式
#define VIEW_INFO_LEVEL_1			0x0010								// 部分信息
#define VIEW_INFO_LEVEL_2			0x0020								// 部分信息
#define VIEW_INFO_LEVEL_3			0x0040								// 部分信息
#define VIEW_INFO_LEVEL_4			0x0080								// 部分信息

//#pragma mark -
//#pragma mark 其他配置
#define RECVICE_GAME_CHAT			0x0100								// 接收聊天
#define RECVICE_ROOM_CHAT			0x0200								// 接收聊天
#define RECVICE_ROOM_WHISPER		0x0400								// 接收私聊

//#pragma mark -
//#pragma mark 行为标识
#define BEHAVIOR_LOGON_NORMAL       0x0000                              // 普通登录
#define BEHAVIOR_LOGON_IMMEDIATELY  0x1000                              // 立即登录


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
typedef int                BOOL;//<>
#endif


//id 登录
typedef struct
{
	// 版本信息
	DWORD							dwPlazaVersion;						// 广场版本
	DWORD							dwFrameVersion;						// 框架版本
	DWORD							dwProcessVersion;					// 进程版本
	
	// 登录信息
	DWORD							dwUserID;							// 用户id
	TCHART							szPassword[LEN_MD5];				// 登录密码
	TCHART							szMachineID[LEN_MACHINE_ID];		// 机器序列
	
} CMD_GR_LogonUserID;


// 手机登录
typedef struct
{
	// 版本信息
	WORD							wGameID;							// 游戏标识
	DWORD							dwProcessVersion;					// 进程版本
	
	WORD                            wVisibleMode;                     // 行为标识<视图模式>
	WORD                            wVisibleTable;                    // 分页桌数<可视数目>
	
	// 登录信息
	DWORD							dwUserID;							// 用户 I D
	TCHART							szPassword[LEN_MD5];				// 登录密码
	TCHART							szMachineID[LEN_MACHINE_ID];		// 机器标识
	
}CMD_GR_LogonMobile;



// 帐号登录
typedef struct
{
	// 版本信息
	DWORD							dwPlazaVersion;						// 广场版本
	DWORD							dwFrameVersion;						// 框架版本
	DWORD							dwProcessVersion;					// 进程版本
	
	// 登录信息
	TCHART							szAccounts[LEN_ACCOUNTS];			// 登录帐号
	TCHART							szPassword[LEN_MD5];				// 登录密码
	TCHART							szMachineID[LEN_MACHINE_ID];		// 机器序列

} CMD_GR_LogonAccounts;

// 登录成功
typedef struct
{
	DWORD							dwUserRight;						// 用户权限
	DWORD							dwMasterRight;						// 管理权限                            
} CMD_GR_LogonSuccess;

// 登录失败
typedef struct
{
	int							lErrorCode;							// 错误代码
	TCHART							szDescribeString[128];				// 错误描述
} CMD_GR_LogonFailure;


// 登陆提示TIPS
typedef struct
{
	DWORD                           dwTick;
	TCHART							szDescribeString[64];
} CMD_GP_LogonNotify;

// 升级提示
typedef struct
{	
	// 升级标志
	BYTE							cbMustUpdatePlaza;					// 强行升级
	BYTE							cbMustUpdateClient;					// 强行升级
	BYTE							cbAdviceUpdateClient;				// 建议升级
	
	// 当前版本
	DWORD							dwCurrentPlazaVersion;				// 当前版本
	DWORD							dwCurrentFrameVersion;				// 当前版本
	DWORD							dwCurrentClientVersion;				// 当前版本
} CMD_GR_UpdateNotify;

//////////////////////////////////////////////////////////////////////////
struct CMD_GR_LogonByAccounts
{
	DWORD							dwPlazaVersion;
	DWORD							dwProcessVersion;
	unsigned char					szAccounts[NAME_LEN];
	unsigned char					szPassWord[PASS_LEN];
};

struct CMD_GR_LogonByUserID
{
	DWORD							dwPlazaVersion;
	DWORD							dwProcessVersion;
	DWORD							dwUserID;
	unsigned char					szPassWord[PASS_LEN];
};

struct CMD_GR_LogonError
{
	LONG							lErrorCode;
	char							szErrorDescribe[128];
};

enum enEnterRoomType
{
	FIRST_LOGON  =0,
	RECONNECT_LOGON =1,
};

//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//#pragma mark -
//#pragma mark 配置命令

#define	MDM_GR_CONFIG				2									// 配置信息

#define	SUB_GR_CONFIG_COLUMN		100									// 列表配置
#define SUB_GR_CONFIG_SERVER		101									// 房间配置
#define SUB_GR_CONFIG_PROPERTY		102									// 道具配置
#define SUB_GR_CONFIG_FINISH		103									// 配置完成




/////////////////////////////////////////////////////////////////////////////////

// 列表配置
typedef struct
{
	BYTE							cbColumnCount;						// 列表数目
	tagColumnItem					ColumnItem[MAX_COLUMN];				// 列表描述
} CMD_GR_ConfigColumn;

// 房间配置
typedef struct
{
	//房间属性
	WORD							wTableCount;						// 桌子数目
	WORD							wChairCount;						// 椅子数目
	
	//房间配置
	BOOL                            bAllowDistribute;                   //允许分组 
	WORD							wServerType;						//房间类型	
	DWORD							dwServerRule;						//房间规则
} CMD_GR_ConfigServer;

// 道具配置
typedef struct
{
	BYTE							cbPropertyCount;					// 道具数目
} CMD_GR_ConfigProperty;


/////////////////////////////////////////////////////////////////////////////////
//#pragma mark -
//#pragma mark 用户命令

#define MDM_GR_USER					3									// 用户信息

// 用户动作
#define	SUB_GR_USER_LOOKON			1									// 旁观请求
#define	SUB_GR_USER_SITDOWN			2									// 坐下请求
#define	SUB_GR_USER_STANDUP			3									// 起立请求
#define SUB_GR_USER_RULE			4									// 用户规则
#define SUB_GR_USER_INVITE			5									// 用户邀请
#define SUB_GR_USER_INVITE_REQ		6									// 邀请请求
#define SUB_GR_USER_REPULSE_SIT  	7									// 拒绝玩家坐下
#define SUB_GR_USER_KICK_USER       8                                   // 踢出用户     踢人 消息
#define SUB_GR_USER_INFO_REQ        9                                   // 请求用户信息
#define SUB_GR_USER_CHAIR_REQ       10                                  // 请求更换位置
#define SUB_GR_USER_CHAIR_INFO_REQ	11									// 请求椅子用户信息
#define SUB_GR_USER_WAIT_DISTRIBUTE 12                                  // 等待分配

// 用户状态
#define	SUB_GR_USER_ENTER			100									// 用户进入
#define	SUB_GR_USER_SCORE			101									// 用户分数
#define SUB_GR_USER_STATUS			102									// 用户状态
#define SUB_GR_USER_SPREADINFO		103									// 用户信息
#define SUB_GR_REQUEST_FAILURE		104									// 请求失败

// 聊天命令
#define	SUB_GR_USER_CHAT			201									// 聊天信息
#define	SUB_GR_USER_EXPRESSION		202									// 表情消息
#define SUB_GR_WISPER_CHAT			203									// 私聊消息
#define	SUB_GR_WISPER_EXPRESSION	204									// 私聊表情
#define SUB_GR_COLLOQUY_CHAT		205									// 会话消息
#define	SUB_GR_COLLOQUY_ERPRESSION	206									// 会话表情

// 道具命令
#define SUB_GR_PROPERTY_BUY			300									// 购买道具
#define SUB_GR_PROPERTY_SUCCESS		301									// 道具成功
#define SUB_GR_PROPERTY_FAILURE		302									// 道具失败
#define SUB_GR_PROPERTY_MESSAGE     303                                 // 道具消息
#define SUB_GR_PROPERTY_EFFECT      304                                 // 道具效应
#define SUB_GR_PROPERTY_TRUMPET     305                                 // 用户喇叭

// 手机游戏：抢宝箱指令
#define	SUB_GR_CLICK_BONUS			403									// 每隔N分钟的豆豆获取，客户端发起
#define	SUB_GR_GET_BONUS			1404								// 服务器端返回结果，以及豆豆数
#define	SUB_GR_GET_ERROR			1405								// 点击宝箱返回错误消息描述

/////////////////////////////////////////////////////////////////////////////////

// 旁观请求
typedef struct
{
	WORD							wTableID;							// 桌子位置
	WORD							wChairID;							// 椅子位置
} CMD_GR_UserLookon;

// 坐下请求
typedef struct
{
	WORD							wTableID;							// 桌子位置
	WORD							wChairID;							// 椅子位置
	TCHART							szPassword[LEN_PASSWORD];			// 桌子密码
} CMD_GR_UserSitDown;

// 起立请求
typedef struct
{
	WORD							wTableID;							// 桌子位置
	WORD							wChairID;							// 椅子位置
	BYTE							cbForceLeave;						// 强行离开
} CMD_GR_UserStandUp;

//#pragma mark -
//#pragma mark 用户分数
typedef struct 
{
	DWORD							dwUserID;							// 用户标识
	tagMobileUserScore				UserScore;							// 积分信息
} CMD_GR_MobileUserScore;


// 用户状态
typedef struct
{
	DWORD							dwUserID;							// 用户标识
	tagUserStatus					UserStatus;							// 用户状态
} CMD_GR_UserStatus;

// 请求用户信息
typedef struct 
{
	DWORD                           dwUserIDReq;                        // 请求用户
	WORD							wTablePos;							// 桌子位置
} CMD_GR_UserInfoReq;


// 请求用户信息
typedef struct 
{
	WORD							wTableID;							// 桌子号码
	WORD							wChairID;							// 椅子号码
} CMD_GR_ChairUserInfoReq;


// 请求失败
typedef struct
{
	BYTE							cbFailureCode;						// 错误代码
	BYTE                            cbCommandCode;                      // 命令代码
	TCHART							szDescribeString[256];				// 描述信息
} CMD_GR_RequestFailure;

// 用户聊天
typedef struct
{
	WORD							wChatLength;						// 信息长度
	DWORD							dwChatColor;						// 信息颜色
	DWORD							dwTargerUserID;						// 目标用户
	TCHART							szChatString[LEN_USER_CHAT];		// 聊天信息
} CMD_GR_C_UserChat;

// 用户聊天
typedef struct
{
	WORD							wChatLength;						// 信息长度
	DWORD							dwChatColor;						// 信息颜色
	DWORD							dwSendUserID;						// 发送用户
	DWORD							dwTargetUserID;						// 目标用户
	TCHART							szChatString[LEN_USER_CHAT];		// 聊天信息
} CMD_GR_S_UserChat;

// 用户表情
typedef struct
{
	WORD							wItemIndex;							// 表情索引
	DWORD							dwTargetUserID;						// 目标用户
} CMD_GR_C_UserExpression;

//记录信息
struct tagServerGameRecord
{
	BYTE							cbKingWinner;						//天王赢家
	bool							bPlayerTwoPair;						//对子标识
	bool							bBankerTwoPair;						//对子标识
	BYTE							cbPlayerCount;						//闲家点数
	BYTE							cbBankerCount;						//庄家点数
};

// 用户表情
typedef struct
{
	WORD							wItemIndex;							// 表情索引
	DWORD							dwSendUserID;						// 发送用户
	DWORD							dwTargetUserID;						// 目标用户
} CMD_GR_S_UserExpression;

// 用户私聊
typedef struct
{
	WORD							wChatLength;						// 信息长度
	DWORD							dwChatColor;						// 信息颜色
	DWORD							dwTargetUserID;						// 目标用户
	TCHART							szChatString[LEN_USER_CHAT];		// 聊天信息
} CMD_GR_C_WisperChat;

// 用户私聊
typedef struct
{
	WORD							wChatLength;						// 信息长度
	DWORD							dwChatColor;						// 信息颜色
	DWORD							dwSendUserID;						// 发送用户
	DWORD							dwTargetUserID;						// 目标用户
	TCHART							szChatString[LEN_USER_CHAT];		// 聊天信息
} CMD_GR_S_WisperChat;

// 私聊表情
typedef struct
{
	WORD							wItemIndex;							// 表情索引
	DWORD							dwTargetUserID;						// 目标用户
} CMD_GR_C_WisperExpression;

// 私聊表情
typedef struct
{
	DWORD							wItemIndex;							// 表情索引
	DWORD							dwSendUserID;						// 发送用户
	DWORD							dwTargetUserID;						// 目标用户
} CMD_GR_S_WisperExpression;

// 用户会话
typedef struct
{
	WORD							wChatLength;						// 信息长度
	DWORD							dwChatColor;						// 信息颜色
	DWORD							dwSendUserID;						// 发送用户
	DWORD							dwConversationID;					// 会话标识
	DWORD							dwTargetUserID[16];					// 目标用户
	TCHART							szChatString[LEN_USER_CHAT];		// 聊天信息
} CMD_GR_GR_ColloquyChat;


// 邀请用户
typedef struct
{
	WORD							wTableID;							// 桌子号码
	DWORD							dwSendUserID;						// 发送用户
} CMD_GR_C_InviteUser;

// 邀请用户
typedef struct
{
	DWORD							dwTargetUserID;						// 目标用户
} CMD_GR_S_InviteUser;


typedef enum
{
    kPropTypeBrick  = 12,
    kPropTypeFlower = 13,

} Prop_type;

// 购买道具
typedef struct 
{
	BYTE                            cbRequestArea;						// 请求范围
	BYTE							cbConsumeScore;						// 积分消费
	WORD							wItemCount;							// 购买数目
	WORD							wPropertyIndex;						// 道具索引  // 鲜花:13，板砖:12
	DWORD							dwTargetUserID;						// 使用对象
} CMD_GR_C_PropertyBuy;


// 道具成功  道具消息成功包
typedef struct
{
	BYTE                            cbRequestArea;						// 使用环境
	WORD							wItemCount;							// 购买数目
	WORD							wPropertyIndex;						// 道具索引 // 鲜花:13，板砖:12
	DWORD							dwSourceUserID;						// 使用用户
	DWORD							dwTargetUserID;						// 目标对象
} CMD_GR_S_PropertySuccess;

// 道具失败 失败包
typedef struct
{
	WORD                            wRequestArea;                       // 请求区域
	LONG							lErrorCode;							// 错误代码
	TCHART							szDescribeString[256];				// 描述信息
} CMD_GR_PropertyFailure;


// 道具消息
typedef struct
{
    WORD                            wPropertyIndex;                     // 道具索引
    WORD                            wPropertyCount;                     // 道具数目
    DWORD                           dwSourceUserID;                     // 使用用户
    DWORD                           dwTargetUserID;                     // 使用对象
} CMD_GR_S_PropertyMessage;

// 踢出用户
typedef struct
{
	DWORD							dwTargetUserID;						// 目标用户
} CMD_GR_KickUser;

/////////////////////////////////////////////////////////////////////////////////
//#pragma mark -
//#pragma mark 状态命令

#define MDM_GR_STATUS				4									//状态信息

#define SUB_GR_TABLE_INFO			100									//桌子信息
#define	SUB_GR_TABLE_STATUS			101									//桌子状态

/////////////////////////////////////////////////////////////////////////////////
// 保险柜命令

#define MDM_GR_INSURE					5								// 用户信息

// 保险柜命令
#define SUB_GR_QUERY_INSURE_INFO		1								// 查询保险柜
#define SUB_GR_SAVE_SCORE_REQUEST		2								// 存入操作
#define SUB_GR_TAKE_SCORE_REQUEST		3								// 取出操作
#define SUB_GR_TRANSFER_SCORE_REQUEST	4								// 转出操作

#define SUB_GR_USER_INSURE_INFO			100								// 保险柜资料
#define SUB_GR_USER_INSURE_SUCCESS		101								// 保险柜成功
#define SUB_GR_USER_INSURE_FAILURE		102								// 保险柜失败

/////////////////////////////////////////////////////////////////////////////////
//管理命令

#define	MDM_GR_MANAGE				6									//管理命令

#define SUB_GR_SEND_WARNING			1									//发送警告
#define SUB_GR_SEND_MESSAGE			2									//发送消息
#define SUB_GR_LOOK_USER_IP			3									//查看地址
#define SUB_GR_KICK_USER			4									//踢出用户
#define SUB_GR_LIMIT_ACCOUNTS		5									//禁用账户
#define SUB_GR_SET_USER_RIGHT		6									//权限设置

//房间设置
#define	SUB_GR_QUERY_OPTION			7									//查询设置
#define SUB_GR_OPTION_SERVER		8									//房间设置
#define SUB_GR_OPTION_CURRENT		9									//当前设置

////////////////////////////////////////////////////////////////////////////////

//设置标志
#define OSF_ROOM_CHAT               1                                   //大厅聊天
#define OSF_GAME_CHAT               2                                   //游戏聊天
#define OSF_ROOM_WISPER             3                                   //大厅私聊
#define OSF_ENTER_TABLE             4                                   //进入游戏
#define OSF_ENTER_SERVER            5                                   //进入房间
#define OSF_SEND_BUGLE              12                                  //发送喇叭


///////////////////////////////////////////////////////////////////////////////
//框架命令

#define	MDM_GF_FRAME				100									// 框架命令

// 用户命令
#define SUB_GF_GAME_OPTION			1									// 游戏配置
#define SUB_GF_USER_READY			2									// 用户准备
#define SUB_GF_LOOKON_CONFIG		3									// 旁观配置

// 聊天命令
#define SUB_GF_USER_CHAT			10									// 用户聊天
#define SUB_GF_USER_EXPRESSION		11									// 用户表情

// 游戏信息
#define SUB_GF_GAME_STATUS			100									// 游戏状态
#define SUB_GF_GAME_SCENE			101									// 旁观场景
#define SUB_GF_LOOKON_STATUS		102									// 旁观状态

// 系统消息
#define SUB_GF_SYSTEM_MESSAGE		200									// 系统消息
#define SUB_GF_ACTION_MESSAGE		201									// 动作消息

/////////////////////////////////////////////////////////////////////////////////



//游戏配置
typedef struct
{
	BYTE							cbAllowLookon;						//旁观标志
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwClientVersion;					//游戏版本

}CMD_GF_GameOption;


/////////////////////////////////////////////////////////////////////////////////

//游戏命令
#define MDM_GF_GAME					200									//游戏命令

//其他信息
#define DTP_GR_TABLE_PASSWORD		1									//桌子密码


//用户属性
#define DTP_GR_NICK_NAME			1									//用户昵称
#define DTP_GR_USER_NOTE			2									//用户备注
#define DTP_GR_ADDR_DESCRIBE		3									//地址描述

/////////////////////////////////////////////////////////////////////////////////

//请求错误
#define REQUEST_FAILURE_NORMAL		0									//常规原因
#define REQUEST_FAILURE_NOGOLD		1									//豆豆不足
#define REQUEST_FAILURE_NOSCORE		2									//积分不足
#define REQUEST_FAILURE_PASSWORD	3									//密码错误

#define REQUEST_FAILURE_ACCOUNTS	7									//昵称错误

struct CMD_CM_SystemMessage
{
	WORD							wType;								//消息类型
	WORD							wLength;							//消息长度
	TCHART							szString[1024];						//消息内容
};

// 客户端发送，某个玩家触摸到宝箱
typedef struct
{
	WORD							wTableID;							// 桌子位置
	WORD							wChairID;							// 椅子位置
} CMD_GR_BoxTouch;

// 服务器端发送，决断宝箱由谁获得
typedef struct
{
	WORD							wWinChairID;						// 椅子位置
	TCHART							szDescription[128];					// 返回消息，应该在游戏画面滚动显示
} CMD_GR_BoxEnd;


// apple应用内购买完成，向服务器申请充值
typedef struct
{
	DWORD							dwUserID;							// 用户ID
	LONGLONG						lPayMoney;							// 充值金额 (为人民币，不是游戏豆豆)
    BYTE							cbPhoneMode;						// 系统  1：Android  2:iPhone
	TCHART							szPayDesc[128];						// 验证字符串
} CMD_GP_ApplyPayIP;

// 申请充值后，服务器返回的数据
typedef struct
{
	DWORD							dwUserID;							// 用户ID
	TCHART							szDesc[128];						// 返回客户端描述
} CMD_GP_PayDescIP;

//////////////////////////////////////////////////////////////////////////
#define MDM_GR_SYSTEM				10
#define SUB_GR_MESSAGE				100

#define SMT_INFO					0x0001
#define SMT_EJECT					0x0002
#define SMT_GLOBAL					0x0004

#define SMT_CONNECT_NEXT			0x0008
#define SMT_TABLE_ROLL				0x0010
#define SMT_TABLE_FIX				0x0020
#define SMT_TABLE_WINTIP			0x0040
#define SMT_TABLE_ANI				0x0080

#define SMT_CLOSE_ROOM				0x1000
#define SMT_INTERMIT_LINE			0x4000

struct CMD_GR_Message
{
	WORD							wMessageType;
	WORD							wMessageLength;
	char							szContent[1024];
};

#define MDM_GR_SERVER_INFO		    11
#define SUB_GR_ONLINE_COUNT_INFO	100

struct tagOnLineCountInfo
{
	WORD							wKindID;
	DWORD							dwOnLineCount;
};

#define SUB_GR_USER_SIT_REQ		1
#define SUB_GR_USER_LOOKON_REQ		2
#define SUB_GR_USER_STANDUP_REQ		3
#define SUB_GR_USER_LEFT_GAME_REQ	4

#define SUB_GR_USER_COME			100
#define SUB_GR_SIT_FAILED			103
#define SUB_GR_USER_RIGHT			104
#define SUB_GR_MEMBER_ORDER			105
#define SUB_GR_USER_SORT			106
#define SUB_GR_USER_MATCH_SCORE     107
#define SUB_GR_MATCH_USER_COM       108

#define SUB_GR_USER_WISPER			205
#define SUB_GR_JOIN_MATCH			400
#define SUB_GR_USER_START_QUEUE_REQ 401

#define SUB_GR_MODIFY_GOLD			402
#define SUB_GR_GAME_MATCH_INFO_REQ	403
#define SUB_GR_GAME_MATCH_INFO		404
#define SUB_GR_CLOSE_GAME_ROOM		405
#define SUB_GR_CLIENT_READY         406
#define SUB_GR_CLIENT_START_GAME    407
#define SUB_GR_SET_VIPROOM_RATE     408									
#define SUB_GR_OK_PASSWORD_REQ		409
#define SUB_GR_ERROR_PASSWORD		410
#define SUB_GR_START_GAME_INFO		411
#define SUB_GR_END_GAME_INFO		412								
#define SUB_GR_DRAW_VIPRATE		413
								
struct CMD_GR_MemberOrder
{
	DWORD							dwUserID;
	BYTE							cbMemberOrder;
};

struct CMD_GR_TaskSystem
{
	WORD							wKindID;
	WORD							wProblemID;
	WORD							wEggNumber;
	bool							bDrawAnimation;
};
				
struct tagUserTaskParameter
{
	DWORD	dwTaskID;
	DWORD	dwCondition;
	DWORD	dwRewardA;
	DWORD	dwRewardB;
	DWORD	dwTaskKind;
	DWORD	dwFinish;
	DWORD	dwDoingValue;

	char	szTaskName[128];
	char	szDescript[128];
	char	szRewardContent[128];
};

struct CMD_GR_UserRight
{
	DWORD							dwUserID;
	DWORD							dwUserRight;
};

struct CMD_GR_UserScore
{
	LONG							lWeekWinCount;						
	LONG							lWeekMingci;						
	DWORD							dwUserID;
	tagUserScore					UserScore;
};


struct CMD_GR_UserMatchScore
{
	DWORD                           dwUserID;
	LONG                            lMatchScore;
};


struct CMD_GR_UserSort
{
	DWORD								dwUserID;
	WORD								wSortID;
	EN_MatchStatus						enMatchStatus;
};

struct CMD_GR_AddBlood_Gold
{
	DWORD                               dwUserID;
	WORD                                wSubGold;
};

struct CMD_GR_UserSitReq
{
	WORD							wTableID;
	WORD							wChairID;
	BYTE							cbPassLen;
	char							szTablePass[PASS_LEN];
};

struct CMD_GR_UserInviteReq
{
	WORD							wTableID;
	DWORD							dwUserID;
};

struct CMD_GR_SitFailed
{
	char							szFailedDescribe[256];
};


struct CMD_GR_UserChat
{
	WORD							wChatLength;
	DWORD							dwSendUserID;
	DWORD							dwTargetUserID;
	char							szChatMessage[MAX_CHAT_LEN];
};

struct CMD_GR_Wisper
{
	WORD							wChatLength;
	DWORD							dwSendUserID;
	DWORD							dwTargetUserID;
	char							szChatMessage[MAX_CHAT_LEN];
};

struct CMD_GR_UserRule
{
	bool							bPassword;
	bool							bLimitWin;
	bool							bLimitFlee;
	bool							bLimitScore;
	bool							bCheckSameIP;
	WORD							wWinRate;
	WORD							wFleeRate;
	LONG							lMaxScore;
	LONG							lLessScore;
	char							szPassword[PASS_LEN];
};

struct CMD_GR_UserInvite
{
	WORD							wTableID;
	DWORD							dwUserID;	
};

struct CMD_GR_JoinMatch
{
	WORD							wJoin;
	char							szVIPPassWord[PASS_LEN];			
	LONG							lCellScore;							
};

struct CMD_GR_Draw_VIPRate
{
	LONG							nRate;	
};

struct CMD_GR_VIPRoom_PassWord
{
    char							szPassword[PASS_LEN];
    LONG							lCellScore;
};

struct CMD_GR_GameMatchInfo
{
	WORD							wSignUpCount;							
	WORD							wJoinCount;
	WORD							wPlayeCount;
	DWORD                           dwUserID;
	char							szMatchTime[24];					
	char							szMatchStatus[16];
};

#define MDM_GR_INFO					3

#define SUB_GR_SERVER_INFO			100
#define SUB_GR_ORDER_INFO			101
#define SUB_GR_MEMBER_INFO			102
#define SUB_GR_COLUMN_INFO			108

struct CMD_GR_ServerInfo
{
	WORD							wKindID;
	WORD							wTableCount;
	WORD							wChairCount;
	DWORD							dwVideoAddr;
	WORD							wGameGenre;
	BYTE							cbHideUserInfo;
	BYTE                            cbLimitDraw;                
};


struct CMD_GR_ScoreInfo
{
	WORD							wDescribeCount;
	WORD							wDataDescribe[16];
};


struct tagOrderItem
{
	LONG							lScore;
	WORD							wOrderDescribe[16];
};

struct CMD_GR_OrderInfo
{
	WORD							wOrderCount;
	tagOrderItem					OrderItem[128];
};

struct CMD_GR_ColumnInfo
{
	WORD						wColumnCount;
	tagColumnItem				ColumnItem[32];
};

#define MDM_GR_STATUS				4

#define SUB_GR_TABLE_INFO			100
#define SUB_GR_TABLE_STATUS			101

struct CMD_GR_TableInfo
{
	WORD						wTableCount;
	tagTableStatus				TableStatus[512];
};

#define MDM_GR_MANAGER				5									

#define SUB_GR_SEND_WARNING			1									
#define SUB_GR_SEND_MESSAGE			2									
#define SUB_GR_LOOK_USER_IP			3									
#define SUB_GR_KILL_USER			4									
#define SUB_GR_LIMIT_ACCOUNS		5								
#define SUB_GR_SET_USER_RIGHT		6								

struct CMD_GR_SendWarning
{
	WORD							wChatLength;
	DWORD							dwTargetUserID;
	char							szWarningMessage[MAX_CHAT_LEN];
};


struct CMD_GR_SendMessage
{
	BYTE							cbGame;
	BYTE							cbRoom;
	WORD							wChatLength;
	char							szSystemMessage[MAX_CHAT_LEN];
};

struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;
};


struct CMD_GR_KillUser
{
	DWORD							dwTargetUserID;
};

struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						
};

struct CMD_GR_SetUserRight
{
	DWORD							dwTargetUserID;	
	BYTE							cbGameRight;
	BYTE							cbAccountsRight;
	BYTE							cbLimitRoomChat;
	BYTE							cbLimitGameChat;					
	BYTE							cbLimitPlayGame;					
	BYTE							cbLimitSendWisper;					
	BYTE							cbLimitLookonGame;					
};

struct CMD_CS_MatchData
{
	DWORD								dwUserID;							
	tagUserGold							UserGold;							
};

#define OSF_ROOM_CHAT				1
#define OSF_GAME_CHAT				2
#define OSF_ROOM_WISPER				3
#define OSF_ENTER_GAME				4
#define OSF_ENTER_ROOM				5
#define OSF_SHALL_CLOSE				6


struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;
	BYTE							cbOptionValue;
};

struct tagVIPTableInfo
{
	WORD							wTableID;
	char							szPassWord[PASS_LEN];
	bool							bTablePassWord;
	LONG							lCellScore;
};

struct NTY_ControlEvent
{
	WORD							wControlID;
};

struct NTY_DataBaseEvent
{
	WORD							wRequestID;
	DWORD							dwContextID;
};

struct NTY_TCPSocketReadEvent
{
	WORD							wDataSize;
	WORD							wServiceID;
	CMD_Command						Command;
};


struct NTY_TCPSocketCloseEvent
{
	WORD							wServiceID;
	BYTE							cbShutReason;
};

struct NTY_TCPSocketConnectEvent
{
	int							    nErrorCode;
	WORD							wServiceID;
};

struct NTY_TCPNetworkAcceptEvent
{
	DWORD							dwSocketID;
	DWORD							dwClientIP;
};

struct NTY_TCPNetworkReadEvent
{
	WORD							wDataSize;
	DWORD							dwSocketID;
	CMD_Command						Command;
};

struct NTY_TCPNetworkCloseEvent
{
	DWORD							dwSocketID;
	DWORD							dwClientIP;
	DWORD							dwActiveTime;
};
//////////////////////////////////////////////////////////////////////////

#pragma pack()