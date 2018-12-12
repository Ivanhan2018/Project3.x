#pragma once
#include "define.h"
#include "packet.h"

#pragma pack(1)

#define MDM_GP_LOGON					1								

#define SUB_GP_LOGON_ACCOUNTS			1								
#define SUB_GP_LOGON_USERID				2								
#define SUB_GP_REGISTER_ACCOUNTS		3								
#define SUB_GP_SYSTEM_VERSION			4								
#define SUB_GP_GET_PASSWORD				5								
#define SUB_GP_FIND_PASSWORD			6								
#define SUB_GP_GET_PZINFO				7                               //配置信息
#define SUB_GP_LOGON_VERSION			11								//游戏版本
#define SUB_GP_REGISTER_ACCOUNTS_APP	12								//APP注册账号
#define SUB_GP_VISITOR_TO_PALYER		13								//完善正常账号
#define SUB_GP_QUICK_GAMESERVER_ADR		14								//获取游戏服地址
#define SUB_GP_LOGON_SUCCESS			100								
#define SUB_GP_LOGON_ERROR				101								
#define SUB_GP_LOGON_FINISH				102								

#define SUB_GP_GET_PASSWORD_RESULT		105								
#define SUB_GP_FIND_PASSWORD_RESULT		110								
#define SUB_GP_GET_PASSWORD_GETALIST_RESULT		111						//获取密码,得到帐户列表

// 登陆命令
#define MDM_MB_LOGON							100							//广场登陆

//登陆模式
#define SUB_MB_LOGON_GAMEID						1							//ID登陆
#define SUB_MB_LOGON_ACCOUNTS					2							//账号登陆
#define SUB_MB_REGISITER_ACCOUNTS				3							//注册账号
#define SUB_MB_LOGON_OTHERPLATFORM				4							//其他登陆

//登陆结果
#define SUB_MB_LOGON_SUCCESS					100							//登陆成功
#define SUB_MB_LOGON_FAILURE					101							//登陆失败

//升级提示
#define SUB_MB_UPDATE_NOTIFY					200							//升级提示

//账号登陆
typedef struct 
{
	//系统信息
	WORD			wModuleID;												//模块标识
	DWORD			dwPlazaVersion;											//广场版本

	//登陆信息
	TCHART			szPassword[LEN_MD5];									//登陆密码
	TCHART			szAccounts[LEN_ACCOUNTS];								//登陆账号

	//连接信息
	TCHART			szMachineID[LEN_MACHINE_ID];							//机器标识
	TCHART			szMobilePhone[LEN_MOBILE_PHONE];						//电话号码
} CMD_MB_LogonAccounts;

//注册账号
typedef struct 
{
	// 系统消息
	WORD			wModuleID;												//模块标识
	DWORD			dwPlazaVersion;											//广场版本

	// 密码变量
	TCHART			szLogonPass[LEN_MD5];									//登陆密码
	TCHART			szInsurePass[LEN_MD5];									//保险柜密码

	// 注册信息
	WORD			wFaceID;												//头像标识
	BYTE			cbGender;												//用户性别
	TCHART			szAccounts[LEN_ACCOUNTS];								//登录账号
	TCHART			szNickName[LEN_NICKNAME];								//用户昵称

	// 连接信息
	TCHART			szMachineID[LEN_MACHINE_ID-2];							//机器标识
	DWORD			dwSpreadID;												//推荐人ID
	TCHART			szMobilePhone[LEN_MOBILE_PHONE];						//电话号码
} CMD_MB_RegisterAccounts;

//其他登陆
typedef struct 
{
	// 系统信息
	WORD			wModuleID;												// 模块标识
	DWORD			dwPlazaVersion;											// 广场版本
	BYTE			cbDeviceType;											// 设备类型

	// 注册信息
	BYTE			cbGender;												// 用户性别
	DWORD			dwUserUin;												// 登陆账号
	TCHART			szNickName[LEN_NICKNAME];								// 用户昵称
	TCHART			zsCompellation[LEN_COMPELLATION];						//真实姓名

	// 连接信息
	TCHART			szMachineID[LEN_MACHINE_ID];							// 机器标识
	TCHART			szMobilePhone[LEN_MOBILE_PHONE];						// 电话号码
} CMD_MB_LogonOtherPlatform;

//登录成功
struct CMD_MB_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwExperience;						//经验数值

	//用户成绩
	SCORE							lScore;								//用户积分
	SCORE							lInsure;							//用户银行
	SCORE                           lUserMedal;                         //用户奖牌 

	//用户信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHART							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHART							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHART                           szAddrDescribe[128];  //地址描述

	int								n_type;
	DOUBLE							f_fandian;
	DOUBLE							f_dongjie;
	DOUBLE							f_yue;

	BYTE							cbShowServerStatus;					//是否显示在线人数
};

//登陆成功
struct CMD_GP_LogonSuccess
{	
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwExperience;						//经验数值

	//用户成绩
	DOUBLE							lScore;								//用户积分
	SCORE							lInsure;							//用户银行
	SCORE                           lUserMedal;                         //用户奖牌 

	//用户信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHART							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHART							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHART                           szAddrDescribe[128];               //地址描述

	int								n_type;
	DOUBLE							f_fandian;
	DOUBLE							f_dongjie;
	DOUBLE							f_yue;

	BYTE							cbShowServerStatus;					//是否显示在线人数
};


// 登陆失败
typedef struct
{
	long			lErrorCode;												//错误代码
	TCHART			szDescribeString[128];									//错误消息
} CMD_MB_LogonFailure;

struct CMD_GP_LogonError
{
	LONG								lErrorCode;
	char								szErrorDescribe[128];
};

// 升级提示
typedef struct
{
	BYTE			cbMustUpdate;											//强行升级
	BYTE			cbAdviceUpdate;											//建议版本
	DWORD			dwCurrentVersion;										//当前版本
} CMD_MB_UpdateNotify;


/////////////////////////////////////////////////////////////////////
// 列表命令
#define MDM_MB_SERVER_LIST						101							//列表信息

#define SUB_MB_LIST_KIND						101							//种类列表
#define SUB_MB_LIST_SERVER						102							//房间列表
#define	SUB_MB_LIST_FINISH						103							//列表完成

#define MDM_GP_SERVER_LIST				2								

#define SUB_GP_LIST_TYPE				100								
#define SUB_GP_LIST_KIND				101								
#define SUB_GP_LIST_STATION				102								
#define SUB_GP_LIST_SERVER				103								
#define SUB_GP_LIST_FINISH				104								
#define SUB_GP_LIST_CONFIG				105
//////////////////////////////////////////////////////////////
//服务命令
#define MDM_MB_USER_SERVICE						102							//用户服务

////////////////////////////////////////////////////////////////////////////////
// 帐号服务
#define SUB_MB_MODIFY_ACCOUNTS					1							//修改账号
#define SUB_MB_MODIFY_LOGON_PASS				2							//修改密码
#define SUB_MB_MODIFY_INSURE_PASS				3							//修改密码
#define SUB_MB_MODIFY_INDIVIDUAL				4							//修改资料

// 查询命令
#define SUB_MB_QUERY_INDIVIDUAL					10							//查询信息
///////////////////////////////////////////////////////////////////////////////////

// 操作结果
#define SUB_MB_OPERATE_SUCCESS					100							//操作成功
#define SUB_MB_OPERATE_FAILURE					101							//操作失败

// 查询结果
#define SUB_MB_USER_INDIVIDUAL					200							//个人资料


//////////////////////////////////////////////////////////////////////////////////
// 大厅保险柜服务命令(登录服务器)

#define MDM_GP_USER_SERVICE						3							// 用户服务 大厅保险柜的主命令


// 账号服务
#define SUB_GP_MODIFY_MACHINE					100							//修改机器
#define SUB_GP_MODIFY_LOGON_PASS				101							//修改密码
#define SUB_GP_MODIFY_INSURE_PASS				102							//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE				103							//修改签名

// 修改头像
#define SUB_GP_USER_FACE_INFO					200							//头像信息
#define SUB_GP_SYSTEM_FACE_INFO					201							//系统头像
#define SUB_GP_CUSTOM_FACE_INFO					202							//自定头像

// 个人资料
#define SUB_GP_USER_INDIVIDUAL					301							//个人资料
#define SUB_GP_QUERY_INDIVIDUAL					302							//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL				303							//修改资料

// 保险柜服务
#define SUB_GP_USER_SAVE_SCORE					400							//存入操作
#define SUB_GP_USER_TAKE_SCORE					401							//取出操作
#define SUB_GP_USER_TRANSFER_SCORE					402							//转账操作
#define SUB_GP_USER_INSURE_INFO					403							//保险柜资料
#define SUB_GP_QUERY_INSURE_INFO				404							//查询保险柜
#define SUB_GP_USER_INSURE_SUCCESS				405							//保险柜成功
#define SUB_GP_USER_INSURE_FAILURE				406							//保险柜失败
#define SUB_GP_QUERY_USER_INFO_REQUEST			407							//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT			408							//用户信息
#define SUB_GP_QUERY_TRANSFER_LOG	409								
#define SUB_GP_QUERY_TRANSFER_LOG_RET	410								
#define SUB_GP_SEARCH_GOODS_INFO	411								
#define SUB_GP_SEARCH_GOODS_INFO_RET 412							
#define SUB_GP_SALE_GOODS_INFO		413								
#define SUB_GR_SALE_GOODS_INFO_RET	414								
#define SUB_GP_BUY_GOODS_INFO		415								
#define SUB_GR_BUY_GOODS_INFO_RET	416								
#define SUB_GR_GET_USER_PACKET		417								
#define SUB_GR_GET_USER_PACKET_RET	418								
#define SUB_GP_USER_INSURE_SUCCESS2	419								

///////////////////////猜硬币 大转盘
#define SUB_GP_GUESS_COIN			420									//掷硬币
#define SUB_GP_GUESS_COIN_RET		421									//掷硬币返回
#define SUB_GP_TURN_TABLE			422									//大转盘
#define SUB_GP_TURN_TALBE_RET		423									//大转盘返回
//游戏开奖查询
#define SUB_GP_QUERY_GAME_RESULT	500									//查询开奖结果
#define SUB_GP_QUERY_GAME_RET		501									//查询开奖结果
//查询系统时间
#define	SUB_GP_QUERY_TIME			502									//查询时间
#define	SUB_GP_QUERY_TIME_RET		503									//查询时间
//重庆时时彩下注
#define	SUB_GP_TOUZHU_CQSSC			504									//查询时间
#define	SUB_GP_TOUZHU_CQSSC_RET		505									//查询时间
#define	SUB_GP_TOUZHU_CQSSC_DAN		908									//单式下注
#define	SUB_GP_TOUZHU_CONTINUE_RET		407									//查询时间
#define	SUB_GP_TOUZHU_CQSSC_ZHUIHAO		408									//单式下注
#define	SUB_MB_TOUZHU_CQSSC_ZHUIHAO		636                                 //手机追号

//查询银行
#define	SUB_GP_QUERY_YINHANG			909									//查询银行
#define	SUB_GP_QUERY_YINHANG_RET		910									//查询银行
//操作结果
#define SUB_GP_OPERATE_SUCCESS		900									//操作成功
#define SUB_GP_OPERATE_FAILURE		901									//操作失败

//游戏开奖查询
#define SUB_GP_GET_CP_USER			902									//获取彩票玩家信息
#define SUB_GP_GET_CP_USER_RET		903									//获取彩票玩家信息结果

//查询返点
#define SUB_GP_GET_USER_FANDIAN			904									//获取彩票玩家返点
#define SUB_GP_GET_USER_FANDIAN_RET		905									//获取彩票玩家返点结果

//设置返点
#define SUB_GP_SET_USER_BONUS			906									//设置彩票玩家返点
#define SUB_GP_SET_USER_BONUS_RET		907									//设置彩票玩家返点结果

//获取MAPBONUS
#define SUB_GP_GET_MAP_BONUS			598									//获取MAPBONUS
#define SUB_GP_GET_MAP_BONUS_RET		599									//获取MAPBONUS结果

#define SUB_GP_GET_CHONGZHI_TISHI	508									//充值提示
#define SUB_GP_GET_CHONGZHI_TISHI_RET		509							//充值提示返回

#define SUB_GP_GET_TIXIAN_TISHI		512									//提现提示
#define SUB_GP_GET_TIXIAN_TISHI_RET		513							//提现提示返回

#define SUB_GP_GET_TOUZHU_TISHI		516									//投注提示
#define SUB_GP_GET_TOUZHU_TISHI_RET		517							//投注提示返回

//个人账户
#define SUB_GP_GET_TOUZHU_LOG_COUNT			506								//查询投注日志数
#define SUB_GR_GET_TOUZHU_LOG_COUNT_RET		507							//查询投注日志数返回
//////////////////////////////////////////////////////////////////////////////////
struct CMD_GR_TouzhuRes
{
	LONG							lResult;							//返回值
	TCHART							szDesc[126];						//返回描述
	int								nSign;								//标志

};

//个人账户
#define SUB_GP_GET_TOUZHU_LOG_COUNT			506								//查询投注日志数
#define SUB_GR_GET_TOUZHU_LOG_COUNT_RET		507							//查询投注日志数返回

//查询投注日志数
struct CMD_GP_GetTouzhuLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetTouzhuLogCountRet
{
	DWORD							dwCount;							//返回
};


#define SUB_GP_GET_TOUZHU_LOG			514								//查询投注日志
#define SUB_GP_GET_TOUZHU_LOG_RET		515								//返回

struct	CMD_GP_GetTouzhuLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetTouzhuLogRet
{
	int								n_t_id;
	int								n_t_type;
	int								n_t_kind;
	TCHART							szQishu[30];
	int								n_t_zhushu;
	int								n_t_moshi;
	int								n_t_beishu;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	int								n_t_winzhushu;
	DOUBLE							f_t_yingkui;
	int								n_t_zhuihao;
};
#define SUB_GP_CANCEL_TOUZHU			510								//撤单
#define SUB_GR_CANCEL_TOUZHU_RET		511								//撤单返回
struct CMD_GP_CancelTouzhu
{
	int								n_t_userid;							//用户ID
	int								n_t_id;								//投注ID
	int								n_t_kindid;
	TCHART							s_t_qihao[30];
};

struct CMD_GR_CancelTouzhuRet
{
	int								nResult;
};

#define SUB_GP_CHONG_ZHI_TYPE					592					//获取充值信息
#define SUB_GP_CHONG_ZHI_TYPE_RET				593					//获取充值信息返回

struct CMD_GP_GetChongzhiType
{
	int								nUserID;					//玩家ID
	BYTE							cbType;						//类型
};

struct CMD_GP_GetChongzhiTypeRet
{
	TCHART							s_t_yinhang[60];				//银行
	TCHART							s_t_kaihuren[60];				//开户人
	TCHART							s_t_zhanghao[60];				//账号
	TCHART							s_t_web[60];					//网页
};

#define SUB_GP_SET_QUKUAN_ZHANGHAO			540								//设置取款保护
#define SUB_GR_SET_QUKUAN_ZHANGHAO_RET		541								//返回

struct	CMD_GP_SetQukuanZhanghao
{
	DWORD							dwUserID;						
	TCHART							szKaihuYinghang[33];					
	TCHART							szKaihuRen[33];					
	TCHART							szYinhangZhanghu[33];					
	TCHART							szQukuanPass[33];					
};

struct CMD_GR_SetQukuanZhanghao_RET
{
	int							lResult;
};

#define SUB_GP_QUERY_MY_YINHANG			415								//查询自己的提款账号设置
#define SUB_GR_QUERY_MY_YINHANG_RET		416								//返回

struct	CMD_GP_QueryMyYinHang
{
	DWORD							dwUserID;							//用户ID
};
struct CMD_GR_QueryMyYinHang_RET
{
	int							lResult;
	TCHART						szKaihuYinghang[33];
	TCHART						szKaihuRen[33];
	TCHART						szYinhangZhanghu[33];				
};

//获得所有银行
struct CMD_GR_QueryYinhang_RET
{
	LONG							lResultCode;						//操作代码
	TCHART							szYinHangName[128];					//成功消息
};

//获取取款信息 

#define SUB_GP_QU_KUAN_INFO					594					//获取取款信息
#define SUB_GP_QU_KUAN_INFO_RET				595					//获取取款信息返回

struct CMD_GP_GetQukuanInfo
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_GetQukuanInfoRet
{
	DOUBLE							f_t_yue;						//余额
	TCHART							s_t_yinhang[60];				//银行
	TCHART							s_t_kaihuren[60];				//开户人
	TCHART							s_t_zhanghao[60];				//账号
	BYTE							n_t_isvip;						//是否超级VIP
};


//取款申请
#define SUB_GP_DO_QU_KUAN					600					//取款信息
#define SUB_GP_DO_QU_KUAN_RET				601					//取款信息返回

struct CMD_GP_DoQukuan
{
	DWORD							dwUserID;						//用户ID
	DOUBLE							fJine;							//金额
	TCHART							szQukuanPass[33];				//取款密码
};

struct CMD_GP_DoQukuanRet
{
	int								nResult;						//返回值
};


//返回结果  0:成功提交  1:取款密码不对  2:申请额度超过账户余额    3:申请金额至少100元    4:当日申请次数超过限定次数
//5:申请失败，请稍后重试     6:取款时间为上午10:00 - 2:00

//////////////////////////////////////////////////////////////////////////////////
#define SUB_GP_XG_LOGIN_PASS			534								//修改登录密码
#define SUB_GR_XG_LOGIN_PASS_RET		535								//返回

struct	CMD_GP_XG_Loginpass
{
	DWORD							dwUserID;							//用户ID
	TCHART							szOldPass[33];						//旧密码
	TCHART							szNewPass[33];						//新密码
};
struct CMD_GR_XG_Loginpass_RET
{
	int							lResult;
};

#define SUB_GP_XG_QUKUAN_PASS			536								//修改登录密码
#define SUB_GR_XG_QUKUAN_PASS_RET		537								//返回

struct	CMD_GP_XG_Qukuanpass
{
	DWORD							dwUserID;							//用户ID
	TCHART							szOldPass[33];						//旧密码
	TCHART							szNewPass[33];						//新密码
};
struct CMD_GR_XG_Qukuanpass_RET
{
	int							lResult;
};



/******************************
*******************************
*************提现******************
*******************************
*********************************/
//个人账户
#define SUB_GP_GET_TIXIAN_LOG_COUNT			520								//查询提现日志数
#define SUB_GR_GET_TIXIAN_LOG_COUNT_RET		521							//查询提现日志数返回

//查询投注日志数
struct CMD_GP_GetTixianLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetTixianLogCountRet
{
	DWORD							dwCount;							//返回
};



#define SUB_GP_GET_TIXIAN_LOG			518								//查询投注日志
#define SUB_GP_GET_TIXIAN_LOG_RET		519								//返回

struct	CMD_GP_GetTixianLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetTixianLogRet
{
	int								n_t_id;
	SYSTEMTIME						n_t_time;
	TCHART							n_t_yinhang[30];
	DOUBLE							f_t_jine;
	TCHART							s_t_kaihuren[30];
	int								n_t_state;
	TCHART							s_t_note[30];
};

//个人账户
#define SUB_GP_GET_CHONGZHI_LOG_COUNT			522								//查询提现日志数
#define SUB_GR_GET_CHONGZHI_LOG_COUNT_RET		523							//查询提现日志数返回

//查询投注日志数
struct CMD_GP_GetChongzhiLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetChongzhiLogCountRet
{
	DWORD							dwCount;							//返回
};



#define SUB_GP_GET_CHONGZHI_LOG			524								//查询投注日志
#define SUB_GP_GET_CHONGZHI_LOG_RET		525								//返回

struct	CMD_GP_GetChongzhiLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetChongzhiLogRet
{
	int								n_t_id;
	DOUBLE							f_t_jine;
	SYSTEMTIME						n_t_time;
	int								n_t_state;
	TCHART							s_t_pingtai[30];
};

/*************************************/

/*************************************/
/******************************
*******************************
*************游戏记录******************
*******************************
*********************************/
///////////////////////棋牌盈亏///////////////////////////////////////////////////////
//个人账户
#define SUB_GP_GET_QP_YINGKUI_COUNT			411								//查询盈亏日志数
#define SUB_GR_GET_QP_YINGKUI_COUNT_RET		412							//查询盈亏日志数返回

//查询盈亏明细
struct CMD_GP_GetQiPaiYingkuiCount
{
	DWORD							dwUserID;							//用户ID
	int								nTypeID;					
	int								nByTime;					
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetQiPaiYingkuiCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_QP_YINGKUI			413								//查询盈亏明细
#define SUB_GP_GET_QP_YINGKUI_RET		414								//返回

struct	CMD_GP_GetQiPaiYingkui
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetQiPaiYingkuiRet
{
	int								n_t_userid;
	int								n_t_drawid;
	DOUBLE							f_t_score;
	int								n_t_playcount;
	DOUBLE							f_t_revenue;
	SYSTEMTIME							n_t_playtime;
	TCHART							s_t_servername[33];
	TCHART							s_t_accounts[33];
};


/*************************************/

/*************************************/
/******************************
*******************************
*************盈亏记录******************
*******************************
*********************************/

//个人账户
#define SUB_GP_GET_YINGKUI_LOG_COUNT			528								//查询盈亏日志数
#define SUB_GR_GET_YINGKUI_LOG_COUNT_RET		529							//查询盈亏日志数返回

//查询投注日志数
struct CMD_GP_GetYingkuiLogCount
{
	DWORD							dwUserID;							//用户ID
	bool							bTime;								//通过时间查询
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetYingkuiLogCountRet
{
	DWORD							dwCount;							//返回
};



#define SUB_GP_GET_YINGKUI_LOG			526								//查询盈亏日志
#define SUB_GP_GET_YINGKUI_LOG_RET		527								//返回

struct	CMD_GP_GetYingkuiLog
{
	DWORD							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	bool							bByTime;							//通过时间
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetYingkuiLogRet
{
	DOUBLE							f_t_chongzhi;
	DOUBLE							f_t_qukuan;
	DOUBLE							f_t_touzhu;
	DOUBLE							f_t_fandian;
	DOUBLE							f_t_zhongjiang;
	DOUBLE							f_t_huodong;
	DOUBLE							f_t_chedan;
};

/*************************************/

/*************************************/
/******************************
*******************************
*************盈亏明细******************
*******************************
*********************************/
//个人账户
#define SUB_GP_GET_YINGKUI_MX_COUNT			626								//查询盈亏日志数
#define SUB_GR_GET_YINGKUI_MX_COUNT_RET		627							//查询盈亏日志数返回

//查询盈亏明细
struct CMD_GP_GetYingkuiMxCount
{
	DWORD							dwUserID;							//用户ID
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetYingkuiMxCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_YINGKUI_MX			628								//查询盈亏明细
#define SUB_GP_GET_YINGKUI_MX_RET		629								//返回

struct	CMD_GP_GetYingkuiMx
{
	int							dwUserID;							//用户ID
	int								nPage;								//页
	int								nSize;								//页的大小
	int							bByTime;							//通过时间
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetYingkuiMxRet
{
	int								n_t_dingdan;
	DOUBLE							f_t_yingkui;
	DOUBLE							f_t_yue;
	SYSTEMTIME						t_t_yingkuitime;
	TCHART							s_t_type[30];
	TCHART							s_t_zhaiyao[126];
};


/***************添加下级**************/
#define SUB_GP_GET_REG_URL			550								//获取注册链接
#define SUB_GR_GET_REG_URL_RET		551								//返回

struct CMD_GetUrlFandian
{
    int								n_t_userid;						//ID
};
struct CMD_GetRegUrl
{
    TCHART							s_reg_url[126];					//注册链接
    DOUBLE							f_t_fandian;
};

#define SUB_GP_ADD_HY				552								//添加下级
#define SUB_GR_ADD_HY_RET			553								//返回

struct CMD_GP_AddHuiYuan
{
    int								n_t_daili_id;
    int								n_type;
    DOUBLE							f_t_fandian;
    TCHART							s_t_account[33];
    TCHART							s_t_login_pass[33];
    TCHART							s_t_qukuan_pass[33];
    TCHART							s_t_qq[33];
};
struct CMD_GR_AddHuiYuanRet
{
    int								n_t_res;
    TCHART							s_t_Desc[126];
};

#define SUB_GP_KILL_SOCKET			644							//强制下线

struct CMD_GP_KillSocket
{
	BYTE							cbResult;
	TCHART							szMessage[256];
};

#define SUB_GP_GET_Peie			915								//获取配额
#define SUB_GR_GET_Peie_RET		916								//返回

struct CMD_GetPeie
{
	int								n_t_userid;						//ID
};
struct CMD_GetPeieRet
{
	int								n_t_peie_1;					//3.0总配额
	int								n_t_peie_2;					//2.8总配额
	int								n_t_peie_3;					//2.7总配额
	int								n_t_peie_s_1;				//3.0已用配额
	int								n_t_peie_s_2;				//2.8已用配额
	int								n_t_peie_s_3;				//2.7已用配额
};

#define SUB_GP_SET_WEB_FANDIAN				630								//设置网页注册返点
#define SUB_GR_SET_WEB_FANDIAN_RET			631								//返回

struct CMD_GP_SetWebFandian
{
    int								n_t_userid;
    DOUBLE							f_t_fandian;
};
struct CMD_GR_SetWebFandianRet
{
    int								n_t_res;
	DOUBLE							f_t_fandian;

};

/***************会员数据***************/
//个人账户
#define SUB_GP_GET_HYSHJ			566								//查询会员数据
#define SUB_GR_GET_HYSHJ_RET		567							//查询会员数据返回

//查询投注日志数
struct CMD_GP_GetHyShj
{
    DWORD							dwUserID;							//用户ID
	BYTE						cbOnlyToday;						//只看今天   0 不是    1 是
	TCHART						szTimeStart[30];					//起始时间
	TCHART						szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetHyShjRet
{
	int								n_t_xjrs;						//下级人数
	int								n_t_xjzx;						//下级在线
	DOUBLE							f_t_hyye;						//会员余额
	DOUBLE							f_t_jrfd;						//今日返点
	DOUBLE							f_t_jrchz;						//今日充值
	DOUBLE							f_t_jrqk;						//今日取款
	DOUBLE							f_t_jrtzh;						//今日投注
	DOUBLE							f_t_jrhd;						//今日活动
	int								n_t_jinrizhuce;					//今日注册
	DOUBLE							f_chedan_zonge;					//撤单总额
	DOUBLE							f_zhongjiang_zonge;				//中奖总额
	DOUBLE							f_wodefandian;					//我的返点-彩票
	DOUBLE							f_caipiao_yingkui;				//彩票盈亏
	DOUBLE							f_t_xianma;						//冼码
	DOUBLE							f_qp_fandian;					//棋牌返点
	DOUBLE							f_qp_yingkui;					//棋牌盈亏
	DOUBLE							f_my_qp_fandian;				//我的棋牌返点
	DOUBLE							f_touzhuyongjin;				//投注佣金
	DOUBLE							f_kuisunyongjin;				//亏损佣金
};

/*******************下级投注统计*******************/
//个人账户
#define SUB_GP_GET_XJTZH_LOG_COUNT			554								//查询下级投注日志数
#define SUB_GR_GET_XJTZH_LOG_COUNT_RET		555							//查询下级投注日志数返回

//查询投注日志数
struct CMD_GP_GetXJTZHLogCount
{
    int							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    bool							bTime;								//通过时间查询
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetXJTZHLogCountRet
{
    DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJTZH_LOG			556								//查询下级投注
#define SUB_GP_GET_XJTZH_LOG_RET		557								//返回

struct	CMD_GP_GetXJTZHLog
{
    int							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    int								nPage;								//页
    int								nSize;								//页的大小
    bool							bByTime;							//通过时间
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJTZHLogRet
{
    int								n_t_id;
    TCHART							s_t_account[32];
    int								n_t_type;
    DOUBLE							f_t_touzhu_ze;
    DOUBLE							f_t_yingkui_ze;
};
/*****************查看下级投注*****************/
//个人账户
#define SUB_GP_CHK_XJTZH_LOG_COUNT			560								//查询下级投注日志数
#define SUB_GR_CHK_XJTZH_LOG_COUNT_RET		561							//查询下级投注日志数返回

//查询投注日志数
struct CMD_GP_CHKXJTZHLogCount
{
    DWORD							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    int								n_xj_id;							//下级ID
    bool							bTime;								//通过时间查询
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_CHKXJTZHLogCountRet
{
    DWORD							dwCount;							//返回
};

#define SUB_GP_CHK_XJTZH_LOG			562								//查询下级投注
#define SUB_GP_CHK_XJTZH_LOG_RET		563								//返回

struct	CMD_GP_CHKXJTZHLog
{
    DWORD							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    int								nPage;								//页
    int								nSize;								//页的大小
    bool							bByTime;							//通过时间
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_CHKXJTZHLogRet
{
    int								n_t_id;
    TCHART							s_t_account[33];
    int								n_t_type;
    int								n_t_kind;
    TCHART							s_t_qishu[30];
    int								n_t_zhushu;
    int								n_t_moshi;
    int								n_t_beishu;
    SYSTEMTIME						n_t_time;
    int								n_t_state;
    int								n_t_winzhushu;
    DOUBLE							f_t_yingkui;
    int								n_t_zhuihao;
};
/*******************下级游戏盈亏统计******************/
//////////////////////////下级游戏盈亏统计////////////////
//个人账户
#define SUB_GP_GET_XJYX_TJ_COUNT			419								//查询下级游戏盈亏信息日志数
#define SUB_GR_GET_XJYX_TJ_COUNT_RET		420							//查询下级游戏盈亏信息日志数返回

//查询下级盈亏日志数
struct CMD_GP_GetXJYXTjCount
{
    int							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    bool							bTime;								//通过时间查询
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetXJYXTjCountRet
{
    DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJYX_TJ			421							//查询下级盈亏
#define SUB_GP_GET_XJYX_TJ_RET		422								//返回

struct	CMD_GP_GetXJYXTj
{
    int							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    int								nPage;								//页
    int								nSize;								//页的大小
    bool							bByTime;							//通过时间
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJYXTjRet
{
    int								n_t_id;
    TCHART							s_t_account[30];
    int								n_t_type;
    int								n_t_daili_id;
    
    DOUBLE							f_t_fandian_ze;
    DOUBLE							f_t_qipai_ze;
    DOUBLE							f_t_tuandui_ye;
};
/******************下级盈亏明细******************/
#define SUB_GP_GET_XJYK_LOG_COUNT			568								//查询下级盈亏信息日志数
#define SUB_GR_GET_XJYK_LOG_COUNT_RET		569							//查询下级盈亏信息日志数返回

//查询下级盈亏日志数
struct CMD_GP_GetXJYKLogCount
{
    DWORD							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    bool							bTime;								//通过时间查询
    int								nXiaJiID;							//下级ID
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
	int								nType;								//0:投注  1 返点  2下级返点  3 中奖 4 充值 5取款 6棋牌盈亏
};
//返回
struct CMD_GR_GetXJYKLogCountRet
{
    DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJYK_LOG			570								//查询下级盈亏
#define SUB_GP_GET_XJYK_LOG_RET		571								//返回

struct	CMD_GP_GetXJYKLog
{
    DWORD							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
	int								n_sort_type;						//排序    0:团队充值(降)  1 团队充值(升)  2团队取款(降)  3 团队取款(升) 4 团队投注(降) 5团队投注(升) 6团队中奖(降) 7团队中奖(升)  8团队返点(降)  9团队返点(升)
	int								nPage;								//页
    int								nSize;								//页的大小
    bool							bByTime;							//通过时间
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJYKLogRet
{
    int								n_t_id;
    int								n_t_userid;
    TCHART							s_t_account[30];
    DOUBLE							f_t_yingkui;
    DOUBLE							f_t_at_yue;
    
    SYSTEMTIME						n_t_time;
    int								n_t_type;
    TCHART							s_t_comment[126];
    
};
/******************/
//////////////////////////下级盈亏统计////////////////
//个人账户
#define SUB_GP_GET_XJYK_TJ_COUNT			574								//查询下级盈亏信息日志数
#define SUB_GR_GET_XJYK_TJ_COUNT_RET		575							//查询下级盈亏信息日志数返回

//查询下级盈亏日志数
struct CMD_GP_GetXJYKTjCount
{
    int							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    bool							bTime;								//通过时间查询
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetXJYKTjCountRet
{
    DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJYK_TJ			576							//查询下级盈亏
#define SUB_GP_GET_XJYK_TJ_RET		577								//返回

struct	CMD_GP_GetXJYKTj
{
    int							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    int								nPage;								//页
    int								nSize;								//页的大小
	int								n_sort_type;						//排序    0:投注  1 充值  2取款  
	bool							bByTime;							//通过时间
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJYKTjRet
{
    int								n_t_id;
    TCHART							s_t_account[30];
    int								n_t_type;
    DOUBLE							f_t_chongzhi_ze;
    DOUBLE							f_t_qukuan_ze;
    DOUBLE							f_t_touzhu_ze;
    DOUBLE							f_t_zhongjiang_ze;
    DOUBLE							f_t_fandian_ze;
    DOUBLE							f_t_xjfandian_ze;
    DOUBLE							f_t_huodong_ze;
    DOUBLE							f_t_tuandui_ye;
    //	DOUBLE							f_t_qipai_ze;
};

/************************************/
//获取更多开奖结果
#define SUB_GP_GET_MORE_RECORD	 632	 //
#define SUB_GP_GET_MORE_RECORD_RET	 633	 //返回

struct CMD_GP_GetMoreRecord
{
    int	 n_t_type;	 //类型  彩票种类
};

struct CMD_GP_GetMoreRecordRet
{
    WORD	 wKindID;		 //操作代码
    TCHART	 wPeriod[30];	 //期号
    TCHART	 szLotNum[45];	 //开奖数据、
    TCHART	 szShijian[30];
};
/*************************************/
//////////////////////////下级充值日志////////////////
//个人账户
#define SUB_GP_GET_XJCHZH_LOG_COUNT			580								//查询下级充值日志数
#define SUB_GR_GET_XJCHZH_LOG_COUNT_RET		581							//查询下级充值日志数返回

//查询下级盈亏日志数
struct CMD_GP_GetXjChzhLogCount
{
    DWORD							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    bool							bTime;								//通过时间查询
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetXjChzhLogCountRet
{
    DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJCHZH_LOG			582							//查询下级充值
#define SUB_GP_GET_XJCHZH_LOG_RET		583								//返回

struct	CMD_GP_GetXJCHZHLog
{
    DWORD							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    int								nPage;								//页
    int								nSize;								//页的大小
    bool							bByTime;							//通过时间
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJCHZHLogRet
{
    int								n_t_id;
    int								n_t_user_id;
    TCHART							s_t_account[30];
    DOUBLE							f_t_jine;
    SYSTEMTIME						n_t_time;
    int								n_t_state;   //0为未结算  1为结算
    TCHART							s_t_pingtai[30];
};
/*************************************/
//////////////////////////下级提现日志////////////////
//个人账户
#define SUB_GP_GET_XJTX_LOG_COUNT			586								//查询下级充值日志数
#define SUB_GR_GET_XJTX_LOG_COUNT_RET		587							//查询下级充值日志数返回

//查询下级提现日志数
struct CMD_GP_GetXjTxLogCount
{
    DWORD							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    bool							bTime;								//通过时间查询
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};
//返回
struct CMD_GR_GetXjTxLogCountRet
{
    DWORD							dwCount;							//返回
};

#define SUB_GP_GET_XJTX_LOG			588							//查询下级充值
#define SUB_GP_GET_XJTX_LOG_RET		589								//返回

struct	CMD_GP_GetXJTxLog
{
    DWORD							dwUserID;							//用户ID
    int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
    int								n_t_user_id;						//查询ID
    TCHART							s_t_account[30];					//查询账户
    int								nPage;								//页
    int								nSize;								//页的大小
    bool							bByTime;							//通过时间
    TCHART							szTimeStart[30];					//起始时间
    TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetXJTxLogRet
{
    int								n_t_id;
    int								n_t_user_id;
    TCHART							s_t_account[30];
    SYSTEMTIME						n_t_time;
    TCHART							s_t_yinhang[30];
    DOUBLE							f_t_jine;
    TCHART							s_t_kaihuiren[30];
    int								n_t_state;
    TCHART							s_t_note[30];
};
/************************************************************/
/////////////////会员信息//////////////////////////////////////////////////////////////////
//个人账户
#define SUB_GP_GET_HYXX_LOG_COUNT			542								//查询会员信息日志数
#define SUB_GR_GET_HYXX_LOG_COUNT_RET		543							//查询会员信息日志数返回

//查询投注日志数
struct CMD_GP_GetHYXXLogCount
{
	int							dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHART							s_t_account[30];					//查询账户
	bool							bTime;								//通过时间查询
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
	int								nOnline;							//在线玩家
	int								nYue;								//有额玩家

};
//返回
struct CMD_GR_GetHYXXLogCountRet
{
	DWORD							dwCount;							//返回
};

#define SUB_GP_GET_HYXX_LOG			544								//查询盈亏日志
#define SUB_GP_GET_HYXX_LOG_RET		545								//返回


#define SUB_GP_GET_HYXX_LIST			431								//查询会员信息
#define SUB_GP_GET_HYXX_LIST_RET		432								//返回

struct	CMD_GP_GetHYXXLog
{
	int								dwUserID;							//用户ID
	int								n_t_type;							//查询状态 0 :查所有人   1 :根据ID 查询   2 根据账户查询0
	int								n_t_user_id;						//查询ID 
	TCHART							s_t_account[30];					//查询账户
	int								n_t_sorttype;						//排序
	int								nPage;								//页
	int								nSize;								//页的大小
	int								bByTime;							//通过时间
	int								nOnline;							//在线玩家
	int								nYue;								//有额玩家
	TCHART							szTimeStart[30];					//起始时间
	TCHART							szTimeEnd[30];						//结束时间
};

struct CMD_GR_GetHYXXLogRet
{
	int								n_t_id;
	TCHART							s_t_account[32];
	SYSTEMTIME						n_t_time;
	int								n_t_type;
	DOUBLE							f_t_fandian;
	DOUBLE							f_t_yue;
	int								n_t_online;
	TCHART							s_t_qq[11];

};
/*********************棋牌彩票互转**********************/
#define SUB_GP_ZHUANHUAN				624					//转换
#define SUB_GP_ZHUANHUAN_RET			625					//转换返回

struct CMD_GP_ZhuanHuan
{
	int							n_t_userid;					//用户ID
	int							n_t_type;					//类型   1 转金币   2 转彩金
	DOUBLE						f_t_jine;					//金额
};

struct CMD_GP_ZhuanHuanRet
{
	int							nResult;
	DOUBLE						f_t_yue;
	DOUBLE						f_t_score;
};
/***************************************/
#define SUB_GP_GET_USER_INFO			530								//获取个人基本信息
#define SUB_GR_GET_USER_INFO_RET		531								//返回

struct	CMD_GP_GetUserInfo
{
	DWORD							dwUserID;							//用户ID
};

struct CMD_GR_GetUserInfoRet
{
	TCHART							s_t_qq[12];						//个人QQ
	SYSTEMTIME						t_reg_time;						// 注册时间
	TCHART							s_t_sjqq[12];					//上级QQ
	DOUBLE							f_t_yongjin;					//佣金
	DOUBLE							f_t_yongjinze;					//佣金总额
	DOUBLE							f_t_fenhong;					//分红
	DOUBLE							f_t_fenhongze;					//分红总额
	DOUBLE							f_t_qipaiye;					//棋牌余额
	DOUBLE							f_t_kuisunyongjin;					//亏损佣金
	DOUBLE							f_t_kuisunyongjinze;					//亏损佣金总额

	SYSTEMTIME						t_ksyj_time;						// 亏损佣金时间

	SYSTEMTIME						t_fh_time;						//	分红时间 
	SYSTEMTIME						t_yj_time;						// 佣金时间

};

#define SUB_GP_QUIT_GAME					409					//退出
#define SUB_GP_QUIT_GAME_RET				410					//获取退出

struct CMD_GP_QuitGame
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_QuitGameRet
{
	int								nResult;
};



#define SUB_GP_XG_QQ			532								//修改QQ
#define SUB_GR_XG_QQ_RET		533								//返回

struct	CMD_GP_XGqq
{
	DWORD							dwUserID;							//用户ID
	TCHART							szQQ[11];							//新QQ 
};
struct CMD_GR_XG_QQ_RET
{
	LONG							lResult;
};
/***************************************/
#define SUB_GP_GET_LAST_YUE					596					//获取最新余额信息
#define SUB_GP_GET_LAST_YUE_RET				597					//获取最新余额信息返回

struct CMD_GP_GetLastYue
{
	DWORD							dwUserID;						//用户ID
};

struct CMD_GP_GetLastYueRet
{
	DOUBLE							f_t_yue;						//余额
	DOUBLE							f_t_dongjie;					//冻结
	DOUBLE							f_t_score;						//游戏币
};

#define SUB_GP_KEFU_URL				925								//设置客服
#define SUB_GR_KEFU_URL_RET			926								//客服地址返回

struct CMD_GetKefuUrlRet
{
	int								n_t_result;					//返回
	TCHART							s_t_desc[256];				//描述
};


#define SUB_GP_GET_TOU_ZHU_XX				620					//获取投注信息
#define SUB_GP_GET_TOU_ZHU_XX_RET			621					//获取投注信息返回

struct CMD_GP_GetTouzhu
{
	int							n_t_id;						//ID
};

struct CMD_GP_GetTouzhuRet
{
	TCHART						s_t_wanjiazhanghao[30];				//账户
	int							n_t_dingdanhao;						//订单号
	TCHART						s_t_qishu[20];						//期数
	int							n_t_gametype;						//游戏类别
	int							n_t_gamekind;						//玩法
	char						s_t_touzhuhaoma[13312];				//投注号码
	TCHART						s_t_kaijianghaoma[45];				//开奖号码
	SYSTEMTIME					n_t_time;
	int							n_t_state;
	int							n_t_zhushu;
	int							n_t_moshi;
	int							n_t_beishu;
	int							n_t_winzhushu;
	DOUBLE						f_t_yingkui;
	DOUBLE						f_t_danzhujine;
	DOUBLE						f_t_fandian;
	int							n_t_End;
	int							nHaoMaLen;
	int							n_Index;
};

//–ﬁ∏ƒ√‹¬Î
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//”√ªß I D
	TCHART							szDesPassword[LEN_PASSWORD];		//”√ªß√‹¬Î
	TCHART							szScrPassword[LEN_PASSWORD];		//”√ªß√‹¬Î
};

//////////////////////////////////////////////////////////////////////////////////
//π∫¬ÚŒÔ∆∑Ãıº˛
struct CMD_GP_SearchGoodsInfo
{
	TCHART							szSearchCondition[216];
};
//À—À˜ŒÔ∆∑Ãıº˛
struct CMD_GP_GetMyPocket
{
	DWORD							dwUserID;
};

//ŒÔ∆∑◊ ¡œ∑µªÿ
struct CMD_GR_GoodsInfoRet
{
	DWORD							dwGoodID;							//ŒÔ∆∑ I D
	DWORD							dwGoodsImageID;						//ŒÔ∆∑Õº∆¨		
	DWORD							dwSaleCount;						// ˝¡ø
	LONGLONG						lGoodsPrice;						//µ•º€
	TCHART							szGoodsName[LEN_NICKNAME];			//ŒÔ∆∑√˚≥∆
	TCHART							szSaleUserName[LEN_NICKNAME];		//≥ˆ €»À
	SYSTEMTIME						nTime;								// £”‡ ±º‰
};	
//ºƒ €ŒÔ∆∑–≈œ¢
struct CMD_GP_SaleGoods
{
	TCHART							szSaleGoodsName[LEN_NICKNAME];
	LONGLONG						lSalePrice;
	DWORD							dwSaleCount;
	//TCHART							szSaleUserName[LEN_NICKNAME];
	DWORD							dwUserID;
	DWORD							dwCustomGoodsID;		//Õº∆¨–≈œ¢
};
//π∫¬ÚŒÔ∆∑Ãıº˛
struct CMD_GP_UserBuyGoodsInfo
{
	DWORD							dwUserID;
	DWORD							dwBuyGoodsGNO;
	DWORD							dwBuyCount;
};

//////////////////////////////////////////////////////////////////////////////////
// 大厅保险柜数据结构 add by mojunxin

// 查询保险柜
typedef struct
{
	DWORD										dwUserID;					//用户ID
	TCHART										szPassword[LEN_MD5];		//保险柜密码
} CMD_GP_QueryInsureInfo;

// 保险柜资料
typedef struct 
{
	WORD										wRevenueTake;				//税收
	WORD										wRevenueTransfer;			//税收
	WORD										wServerID;					//房间标识
	SCORE										lUserScore;					//用户豆豆
	SCORE										lUserInsure;				//保险柜豆豆
	SCORE										lTransferPrerequisite;		//转账条件
} CMD_GP_UserInsureInfo;

// 存入豆豆
typedef struct
{
	DWORD										dwUserID;					//用户ID
	SCORE										lSaveScore;					//存入豆豆
	TCHART										szMachineID[LEN_MACHINE_ID];	//机器序列
} CMD_GP_UserSaveScore;

// 提取豆豆
typedef struct 
{
	DWORD										dwUserID;					//用户ID
	SCORE										lTakeScore;					//提取豆豆
	TCHART										szPassword[LEN_MD5];		//保险柜密码
	TCHART										szMachineID[LEN_MACHINE_ID];	//机器序列
} CMD_GP_UserTakeScore;

typedef struct
{
	DWORD										dwUserID;					
	BYTE										cbByNickName;				
	SCORE										lTransferScore;				
	TCHART										szPassword[LEN_MD5];		
	TCHART										szNickName[LEN_NICENAME];	
	TCHART										szMachineID[LEN_MACHINE_ID];
} CMD_GP_UserTransferScore;

// 保险柜成功
typedef struct
{
	DWORD							dwUserID;							// 用户 I D
	SCORE							lUserScore;							// 用户豆豆
	SCORE							lUserInsure;						// 保险柜豆豆
	TCHART							szDescribeString[128];				// 描述消息
} CMD_GP_UserInsureSuccess;

// 保险柜失败
typedef struct
{
	LONG							lResultCode;						// 错误代码
	TCHART							szDescribeString[128];				// 描述消息
} CMD_GP_UserInsureFailure;

// 提取结果
typedef struct
{
	DWORD							dwUserID;							// 用户 I D
	SCORE							lUserScore;							// 用户豆豆
	SCORE							lUserInsure;						// 保险柜豆豆
} CMD_GP_UserTakeResult;



//查询用户
typedef struct
{
	BYTE                            cbByNickName;                       // 昵称赠送
	TCHART							szNickName[LEN_NICKNAME];			// 目标用户
} CMD_GP_QueryUserInfoRequest;

//用户信息
typedef struct
{
	DWORD							dwTargetGameID;						//目标用户
	TCHART							szNickName[LEN_NICKNAME];			//目标用户
} CMD_GP_UserTransferUserInfo;
//////////////////////////////////////////////////////////////////////////////////

//操作失败
typedef struct
{
	LONG							lResultCode;						//错误代码
	TCHART							szDescribeString[128];				//描述消息
} CMD_GP_OperateFailure;

//操作成功
typedef struct
{
	LONG							lResultCode;						//操作代码
	TCHART							szDescribeString[128];				//成功消息
} CMD_GP_OperateSuccess;


//查询用户背包返回
struct CMD_GR_GetUserPacketRet
{
    DWORD							dwUserID;							//用户 I D
    DWORD							dwPropertyID;						//道具ID
    DWORD							dwPropertyCount;					//数量
    LONGLONG							lPrice;							//单价
    TCHART							szPropertyName[LEN_NICKNAME];		//物品名称
};

///////////////////////////////////2015.5.22添加， 猜硬币结构  大转盘结构
struct CMD_GP_GuessCoin	//猜硬币发送
{
    DWORD	 dwUserID;	 //玩家ID
    bool	 bCoinSide;	 //猜测正反
    SCORE	 lScore;	 //下注
};
struct CMD_GP_GuessCoinRet//猜硬币返回
{
    bool	 bCoinSide;	 //判断正反
    SCORE	 lScore;	 //下注结果
};

struct CMD_GP_TurnTable//大转盘发送
{
    DWORD	 dwUserID;	 //玩家ID
};

struct CMD_GP_TurnTableRet//大转盘返回
{
    BYTE	 cbGoodType;	 //中奖结果
    SCORE    lScore;        //中奖金额
};

//////////////////////////////////////////////////////////////////////////////////
struct CMD_GP_GetCpUser
{
	int								n_t_userid;
};
//返回
struct CMD_GR_GetCpUserInfoRet //获取彩票玩家信息
{
	int								n_t_userid;
	TCHART							s_t_account[30];
	int								n_t_type;
	DOUBLE							f_t_fandian;
	TCHART							s_t_qq[11];
	int								n_t_daili_id;
	int								n_t_online;
	DOUBLE							f_t_yue;
	DOUBLE							f_t_dongjie;
	int								n_t_ban;

};

struct CMD_GP_GetUserFandian
{
	int								n_t_userid;
	int								n_t_type_id;
	int								n_t_kind_id;
};
//返回
struct CMD_GR_GetUserFandianRet //获取彩票玩家返点
{
	int								n_t_kindid;
	DOUBLE							f_t_bonus;
	DOUBLE							f_t_bonusPercent;
	DOUBLE							f_t_fandian;
};
struct CMD_GR_GetMapBonusRet
{
	int								n_t_type_id;
	int								n_t_kind_id;
	DOUBLE							f_t_bonus;
};

struct CMD_GP_SetUserBonus
{
	int								n_t_userid;
	int								n_t_type_id;
	int								n_t_kind_id;
	DOUBLE							f_t_bonus;
	DOUBLE							f_bonuspercent;
};
//返回
struct CMD_GR_SetUserBonusRet //获取彩票玩家返点
{
	int								n_t_result;
};

//查询游戏开奖结果
struct CMD_GP_QueryGameResult
{
	int							wKindID;							//需要查询的游戏ID
};
//返回查询系统时间
struct CMD_GR_GetSystemTime
{
	SYSTEMTIME						ServerTime;							//系统时间
};
//查询充值提示
struct CMD_GP_GetChonzhiTishi
{
	DWORD							dwUserID;							//用户ID 
};
//充值提示返回
struct CMD_GR_ChongzhiTishiRet
{
	DOUBLE							fJine;
	int								nState;
};
//查询提现提示
struct CMD_GP_GetTixianTishi
{
	DWORD							dwUserID;							//用户ID 
};
//提现提示返回
struct CMD_GR_TixianTishiRet
{
	DOUBLE							fJine;
	int								nState;
};

//投注提示
struct CMD_GP_GetTouzhuTishi
{
	DWORD							dwUserID;							//用户ID 
};
//提现提示返回
struct CMD_GR_TouzhuTishiRet
{
	int								nWinZhushu;
	int								nState;
	TCHART							szQishu[30];
	int								nType;
};

/////////////////////////////////////////////////////////////////////////////////////
//重庆时时彩

//开奖查询结果
struct DBO_GP_QueryLotResult
{
	WORD							wKindID;							//操作代码
	TCHART							wPeriod[30];						//期
	TCHART							szLotNum[45];						//开奖数据
	TCHART							szShijian[30];						//时间
};

//下注
struct CMD_GP_TouZhuCQSSC
{
	DWORD							dwUserID;							//用户ID
	char							strQishu[20];						//期数
	int								nGameType;							//游戏类别
	int								nGameKind;							//玩法种类
	char							strHaoma[15360];					//下注号码
	int								nHaoMaLen;							//号码长度
	int								nZhushu;							//注数
	int								nBeitou;							//倍数
	int								nMoshi;								//模式  元角分
	int								nSign;								//投注标志
	int								bZhuihao;							//是否追号
	WORD							nIndex;								//index
	WORD							cbPlatform;							//平台
};
struct CMD_GP_TouZhuCQSSC_Dan
{
	DWORD							dwUserID;							//用户ID
	char							strQishu[20];						//期数
	int								nGameType;							//游戏类别
	int								nGameKind;							//玩法种类
	char							strHaoma[15360];					//下注号码
	int								nHaoMaLen;							//号码长度
	int								nZhushu;							//注数
	int								nBeitou;							//倍数
	int								nMoshi;								//模式  元角分
	int								nSign;								//投注标志
	int								nEnd;								//是否发送结束
	int								bZhuihao;							//是否追号
	int								nIndex;							//index
};
struct CMD_GP_TouZhuCQSSC_Zhuihao
{
	DWORD							dwUserID;							//用户ID
	char							strQishu[50][20];						//期数
	int								nGameType;							//游戏类别
	int								nGameKind;							//玩法种类
	char							strHaoma[14336];					//下注号码
	int								nHaoMaLen;							//号码长度
	int								nZhushu;							//注数
	int								nBeitou[50];							//倍数
	int								nMoshi;								//模式  元角分
	int								nSign;								//投注标志		最好每次投注加1，避免出现粘包
	int								nEnd;								//是否发送结束	0为发送结束，1为发送没有结束
	int								bZhuihao;							//是否追号		2为普通追号，追号不会自动停止>=3中奖停止
	int								nIndex;							//index 默认为0数据一次没发完，每次加1
};
#define SUB_MB_GET_TOU_ZHU_XX				634					//获取投注信息
#define SUB_MB_GET_TOU_ZHU_XX_RET			635					//获取投注信息返回

struct CMD_MB_GetTouzhu
{
	int							n_t_id;						//ID
};

struct CMD_MB_GetTouzhuRet
{
	TCHART						s_t_wanjiazhanghao[30];				//账户
	int							n_t_dingdanhao;						//订单号
	TCHART						s_t_qishu[20];						//期数
	int							n_t_gametype;						//游戏类别
	int							n_t_gamekind;						//玩法
	TCHART						s_t_touzhuhaoma[13312];				//投注号码
	TCHART						s_t_kaijianghaoma[45];				//开奖号码
	SYSTEMTIME					n_t_time;
	int							n_t_state;
	int							n_t_zhushu;
	int							n_t_moshi;
	int							n_t_beishu;
	int							n_t_winzhushu;
	DOUBLE						f_t_yingkui;
	DOUBLE						f_t_danzhujine;
	DOUBLE						f_t_fandian;
	int							n_t_End;
	int							nHaoMaLen;
	int							n_Index;
};


/*************************************/
/*************************************/
#define MDM_GF_USER						2	
#define MDM_GP_USER						4

#define SUB_GP_USER_UPLOAD_FACE			100
#define SUB_GP_USER_DOWNLOAD_FACE		101
#define SUB_GP_UPLOAD_FACE_RESULT		102
#define SUB_GP_DELETE_FACE_RESULT		103
#define SUB_GP_CUSTOM_FACE_DELETE		104
//#define SUB_GP_MODIFY_INDIVIDUAL		105
#define SUB_GP_MODIFY_INDIVIDUAL_RESULT	106
#define SUB_GP_BANK_RESULT				107
#define SUB_GP_GET_CHAMPION_LIST_REQ	108
#define SUB_GP_GET_CHAMPION_LIST		109
#define SUB_GP_RESET_PASSWD				110								// 修改密码
#define SUB_GP_RESET_PASSWD_RESULT		110								// 修改密码结果
#define SUB_GP_MODIFY_NICKNAME			111								// 修改昵称
#define SUB_GP_NICKNAME_RESULT			111								// 修改昵称结果
#define SUB_GP_MODIFY_HEAD				112								// 修改头像
#define SUB_GP_HEAD_RESULT				112								// 修改头像结果
#define SUB_GP_GETLOGONPRIZE			113								// 获取登陆奖励
#define SUB_GP_GETLOGONPRIZE_RESULT		113								// 获取登陆奖励结果
#define SUB_GP_TASKDAILYDATA			114								// 每日任务
#define SUB_GP_DOWNLOADGAMEGOLD			115								// 下载游戏奖励金币
#define SUB_GP_GET_SCORE				116								// 查询金币
#define SUB_GP_GET_RELIEVE				117								// 领取救济金

#define SUB_GP_MATCH_USER_LIST			200
#define SUB_GP_USER_SYSTEM_MSG_REQ		201
#define SUB_GP_USER_SYSTEM_MSG			202

#define SUB_GP_LOAD_SYSTEM_MSG_REQ		203
#define SUB_GP_LOAD_SYSTEM_MSG			204
#define SUB_GP_WRITE_SYSTEM_MSG			205
#define SUB_GP_DELETE_SYS_MSG			206
#define SUB_GP_W_USER_SYS_MSG			207
#define SUB_GP_MODIFY_MOBILE_EMAIL		208
#define SUB_GP_COST_ISLAND_CARD			209

#define SUB_GF_ISLAND					410								//充值卡消息
#define SUB_GF_SZFCARD					454								//神州付消息
#define SUB_GF_CZ_ORDER					457								//支付订单消息

#define MDM_GP_SYSTEM					3

#define SUB_GP_VERSION					100
#define SUB_SP_SYSTEM_MSG				101


struct CMD_GP_Version
{
	BYTE								bNewVersion;
	BYTE								bAllowConnect;
};

struct CMD_GP_ModifyMobileEmail
{
	DWORD							dwUserID;
	char							szMobileCode[NAME_LEN];
	char							szEmailCode[EMAIL_LEN];
};

//紫金岛充值卡
struct CMD_GP_ISLANDCARD
{
	DWORD							dwGameKindID;						// 游戏ID
	DWORD							dwUserID;							// 玩家ID
	char							szCardNo[NAME_LEN];					//卡号	
	char							szCardPwd[NAME_LEN];				//密码

	CMD_GP_ISLANDCARD()
	{
		memset(this, 0, sizeof(this));
	}
};

struct CMD_GP_SZFCARD
{
	DWORD							dwUserID;
	DWORD							dwCardType;
	DWORD							dwCardFee;
	char							szCardNo[NAME_LEN];	
	char							szCardPwd[NAME_LEN];
};

struct CM_GF_SCOREGIFT
{
	DWORD								dwUserID;
	DWORD								lValue;
	char								szToUserName[NAME_LEN];
        char                                                            szPassword[PASS_LEN];//用户密码
};

struct CMD_GP_TASK
{
	DWORD							dwUserID;
	bool							bType;	
	DWORD							dwTaskID;
};	

struct CMD_GP_ModifyIndividual
{
	DWORD							dwUserID;
	char							szNickname[NAME_LEN];
	int							    nGender;
	int							    nAge;
	char							szAddress[64];
	char							szUnderWrite[32];
	char							szPassword[33];
};

struct CMD_GP_UploadCustomFace
{
	DWORD								dwUserID;					
	WORD								wCurrentSize;					
	bool								bLastPacket;					
	bool								bFirstPacket;					
	BYTE								bFaceData[2048];				
};

struct CMD_GP_DownloadFaceSuccess
{
	DWORD							dwToltalSize;						
	DWORD							dwCurrentSize;						
	DWORD							dwUserID;						
	BYTE							bFaceData[2048];					
};

struct CMD_GP_DownloadFace
{
	DWORD							dwUserID;
};

struct CMD_GP_UploadFaceResult
{
	char							szDescribeMsg[128];
	DWORD							dwFaceVer;
	bool							bOperateSuccess;
};

struct CMD_GP_DeleteFaceResult
{
	char							szDescribeMsg[128];
	DWORD							dwFaceVer;
	bool							bOperateSuccess;
};

struct CMD_GP_CustomFaceDelete
{
	DWORD							dwUserID;
	DWORD							dwFaceVer;
};

struct CMD_GP_ModifyIndividualResult
{
	char							szDescribeMsg[128];
	bool							bOperateSuccess;
};

struct CMD_GP_BankResult
{
	WORD							wCode;
	LONG							lValue;
};

struct CMD_GP_GetPassword
{
	char								szAccounts[NAME_LEN];
	char								szMobileCode[32];
	CMD_GP_GetPassword()
	{
		memset(this, 0, sizeof(*this));
	}
};

//获取密码,获得用户列表结果
struct CMD_GP_GetAccountListResult
{
	LONG								lErrCode;
	char								szUserNameList[270];				//用户名列表，最多20个
};

struct CMD_GP_GetPasswordResult
{
	LONG								lErrCode;							
	char								szPassword[PASS_LEN];
	char								szErrorDescribe[128];
};

//用户获取配置信息
struct CMD_GP_GetConfig
{
	DWORD								dwGameKindID;					//0 只获得通用信息 1获得安卓手机配置信息 2获得苹果手机配置信息
	DWORD								dwVersion;

	CMD_GP_GetConfig()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct CMD_GP_FindPassword
{
	char								szAccounts[NAME_LEN];
	char								szEmail[32];
};

struct CMD_GP_FindPasswordResult
{
	LONG								lErrCode;							
	char								szErrorDescribe[128];
};

//得到配置信息结果
struct CMD_GP_LoginConfigResult
{
	union
	{
		DWORD dwVersion;
		struct
		{
			WORD	wRevisionVer;
			BYTE	nMinorVer;
			BYTE	nMajorVer;
		};
	};															// 版本号
	DWORD								dwVerSize;				// 版本大小
	BYTE								nUpdateType;			// 更新类型1.官网强制更新 2.官网非强制更新 3.友盟更新
	char								szDownUrl[100];			// 下载链接

	CMD_GP_LoginConfigResult()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct CMD_GP_IOSConfigResult
{
	LONG								lGameKindid;						//游戏ID
	char								szGameGg[128];						//游戏公告
	LONG                                lHighVerID;							//高版本
	LONG                                lLowVerID;						    //低版本
	LONG                                isForcedUpdate;                     //是否强制更新
};

struct tagUserLevelParameter
{
	LONG lUserLevel;
};

struct CMD_GP_UserLevelResult
{
	LONG						lInfoType;						
	tagUserLevelParameter				tagUserLeve[8];
};

enum enPlazaLogonType
{
	enPlazaLogonType_PC = 2,
	enPlazaLogonType_MOBILE,
};

//帐号登录
struct CMD_GP_LogonAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;					//广场版本
	TCHART							szMachineID[LEN_MACHINE_ID];		        //机器序列

	//登录信息
	TCHART							szPassword[LEN_MD5];				//登录密码
	TCHART							szAccounts[LEN_ACCOUNTS];			//登录帐号
};


struct CMD_GP_LogonByUserID
{
	DWORD								dwPlazaVersion;
	DWORD								dwUserID;
	unsigned char						szPassWord[PASS_LEN];
};

//注册账号
struct CMD_GP_RegisterAccounts_App
{
	DWORD								dwPlazaVersion;					// 广场版本
	DWORD								dwGameKindID;					// 游戏ID
	WORD								wFaceID;						// 头像标识
	BYTE								cbGender;						// 用户性别
	char								szAccounts[NAME_LEN];			// 登录账号
	char								szPassWord[PASS_LEN];			// 登录密码
	char								szAddress[NAME_LEN];			// 设备地址
	BYTE								cbVistor;						// 是否游客
	char								szSpreader[NAME_LEN];			// 推广人名

	CMD_GP_RegisterAccounts_App()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct CMD_GP_UserGameMatch
{
	WORD							wKindID;							
	WORD							wServerID;
	DWORD							dwTime;
};

struct CMD_GP_ScoreGiftResult
{
	LONG								lErrCode;								
	LONG								lScore;								
	char								szErrorDescribe[128];
};

struct CMD_GP_UserChargeResult
{
	LONG								lErrCode;							
	LONG								lScore;
	LONG								lGoldeggs;
	char								szErrorDescribe[128];
};

struct CMD_GP_UserLevelNameResult
{
	char								szTitleName[10];					
	char								szLevelName1[10];
	char								szLevelName2[10];					
	char								szLevelName3[10];					
	char								szLevelName4[10];					
	char								szLevelName5[10];
	char								szLevelName6[10];
	char								szLevelName7[10];
};

struct CMD_GP_TaskResult
{
	LONG								lErrCode;							
	bool								btype;								
	DWORD								dwTaskID;							
	char								szTaskName[50];
	char								szGiftName[50];
	char								szGiftValue[50];
	char								szGiftNum[50];
	LONG								lScore;								
	LONG								lGoldeggs;							
	char								szErrorDescribe[128];
};

//列表配置
struct CMD_GP_ListConfig
{
	int									nBankruptcy;					// 破产标准

	int									nTaskData[6];					// 任务配置
	int									nRoomEnter[6];					// 房间进入配置

	char								szTitle[64];					// 公告标题
	char								szContent[512];					// 公告内容

	CMD_GP_ListConfig()
	{
		memset(this, 0, sizeof(*this));
	}
};

//领取每日任务返回客户端
struct CMD_GP_GetDailyPrize_Return
{
	DWORD							dwPrizeID;							// 任务 ID
	BYTE							nSubCmd;							// 命令 ID
	LONG							lReturn;							// 返回
	long							lScore;								// 当前分数
	CMD_GP_GetDailyPrize_Return()
	{
		memset(this, 0, sizeof(*this));
	}
};


//第三方充值订单信息
struct CMD_GP_CZORDER
{
	DWORD							dwUserID;							//玩家 ID
	char							szPayName[3];						//充值方名称 如 天下支付就是TXF，爱游戏就是AYX
	DWORD							dwMoney;							//充值金额
	CMD_GP_CZORDER()
	{
		memset(this, 0, sizeof(*this));
	}
};

//修改头像消息
struct CMD_GP_ModifyHead
{
	DWORD							dwUserID;							// 玩家 ID
	int								nHead;								// 玩家昵称
	BYTE							nSex;								// 性别

	CMD_GP_ModifyHead()
	{
		memset(this, 0, sizeof(*this));
	}
};


//修改昵称消息
struct CMD_GP_MODIFYNICKNAME
{
	DWORD							dwUserID;							// 玩家 ID
	char							szNickName[32];						// 玩家昵称
	CMD_GP_MODIFYNICKNAME()
	{
		memset(this, 0, sizeof(*this));
	}
};

//修改密码
struct CMD_GP_ResetPasswd
{
	DWORD							dwUserID;							// 玩家 ID
	char							szPasswordOLD[33];					// 玩家密码(老)
	char							szPasswordNew[33];					// 玩家密码(新)

	CMD_GP_ResetPasswd()
	{
		memset(this, 0, sizeof(*this));
	}
};

//领取每日任务
struct CMD_GP_GetDailyPrize
{
	DWORD							dwGameKindID;						// 游戏 ID
	DWORD							dwUserID;							// 玩家 ID
	DWORD							dwPrizeID;							// 任务 ID
	BYTE							nSubCmd;							// 命令 ID

	CMD_GP_GetDailyPrize()
	{
		memset(this, 0, sizeof(*this));
	}
};


// 下载领奖情况
struct SDownloadGamePrize 
{
	DWORD dwUserID;
	DWORD dwGameKindID;
	BYTE nIndex;
	BYTE nSubCmd;
	SDownloadGamePrize()
	{
		memset(this, 0, sizeof(*this));
	}
};

//游客完善资料
struct CMD_GP_Visitor2Player
{
	DWORD							dwUserID;							// 用户ID
	char							szPassWord[NAME_LEN];				// 密码
	char							szAccounts[NAME_LEN];				// 新账号名
	char							szPassWordNew[NAME_LEN];			// 新密码

	CMD_GP_Visitor2Player()
	{
		memset(this, 0, sizeof(*this));
	}
};
/*********************************/

#pragma pack()

