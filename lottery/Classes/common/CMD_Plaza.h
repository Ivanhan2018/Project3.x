#ifndef CMD_PLAZA_HEAD_FILE
#define CMD_PLAZA_HEAD_FILE
#pragma once
#include "Define.h"
#include "cocos2d.h"

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////

//π„≥°∞Ê±æ
#define VER_PLAZA_LOW					5								//π„≥°∞Ê±æ
#define VER_PLAZA_HIGH					2								//π„≥°∞Ê±æ
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)

//////////////////////////////////////////////////////////////////////////
//µ«¬º¥ÌŒÛ±Í ∂

#define ERC_GP_LOGON_SUCCESS			0								//µ«¬Ω≥…π¶
#define ERC_GP_ACCOUNTS_NOT_EXIST		1								//’À∫≈≤ª¥Ê‘⁄
#define ERC_GP_LONG_NULLITY				2								//Ω˚÷πµ«¬º
#define ERC_GP_PASSWORD_ERCOR			3								//√‹¬Î¥ÌŒÛ

//////////////////////////////////////////////////////////////////////////
//µ«¬º√¸¡Ó¬Î

#define MDM_GP_LOGON					1								//π„≥°µ«¬º

#define SUB_GP_LOGON_ACCOUNTS			1								//’À∫≈µ«¬º
#define SUB_GP_LOGON_USERID				2								//I D µ«¬º
#define SUB_GP_REGISTER_ACCOUNTS		3								//◊¢≤·’À∫≈
#define SUB_GP_SYSTEM_VERSION			4								//¥ÛÃ¸∞Ê±æ≈–∂œ 20101011 by ’≈ª™   
#define SUB_GP_GET_PASSWORD				5								//ªÒ»°√‹¬Î
#define SUB_GP_FIND_PASSWORD			6								//’“ªÿ√‹¬Î
#define SUB_GP_GET_PZINFO				7                               //配置信息
#define SUB_GP_LOGON_VERSION			11								//游戏版本
#define SUB_GP_REGISTER_ACCOUNTS_APP	12								// APP注册账号
#define SUB_GP_VISITOR_TO_PALYER		13								// 完善正常账号
#define SUB_GP_QUICK_GAMESERVER_ADR		14								// 获取游戏服地址
#define SUB_GP_LOGON_SUCCESS			100								//µ«¬Ω≥…π¶
#define SUB_GP_LOGON_ERROR				101								//µ«¬Ω ß∞‹
#define SUB_GP_LOGON_FINISH				102								//µ«¬ΩÕÍ≥…

#define SUB_GP_GET_PASSWORD_RESULT		105								//ªÒ»°√‹¬Î
#define SUB_GP_FIND_PASSWORD_RESULT		110								//’“ªÿ√‹¬ÎΩ·π˚
#define SUB_GP_GET_PASSWORD_GETALIST_RESULT		111						//获取密码,得到帐户列表

enum enPlazaLogonType
{
	enPlazaLogonType_PC = 2,
	enPlazaLogonType_MOBILE,
};
//帐号登录
struct CMD_GP_LogonAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHART							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	TCHART							szPassword[LEN_MD5];				//登录密码
	TCHART							szAccounts[LEN_ACCOUNTS];			//登录帐号
};

//I D µ«¬º
struct CMD_GP_LogonByUserID
{
	DWORD								dwPlazaVersion;					//π„≥°∞Ê±æ
	DWORD								dwUserID;						//”√ªß I D
	unsigned char						szPassWord[PASS_LEN];			//µ«¬º√‹¬Î
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
	//TCHART							szNickName[LEN_ACCOUNTS];		//用户昵称
	BYTE							cIsYanZheng;						//是否需要验证码
	BYTE							cbReserved;							//保留字段
	TCHART							szPhoneNum[LEN_ACCOUNTS-1];			//手机号码
	TCHART                           szAddrDescribe[128];				//描述信息

	int								n_type;
	DOUBLE							f_fandian;
	DOUBLE							f_dongjie;
	DOUBLE							f_yue;

	//梦成网络
	BYTE							cbShowServerStatus;					//是否显示在线人数

};

//µ«¬Ω ß∞‹
struct CMD_GP_LogonError
{
	LONG								lErrorCode;						//¥ÌŒÛ¥˙¬Î
	char								szErrorDescribe[128];			//¥ÌŒÛœ˚œ¢
};

//”√ªß±®√˚±»»¸¡–±Ì
struct CMD_GP_UserGameMatch
{
	WORD							wKindID;							//”Œœ∑¿‡–Õ
	WORD							wServerID;							//∑øº‰¿‡–Õ
	DWORD							dwTime;								//±»»¸ ±º‰
};

//”√ªßªÒ»°√‹¬Î
struct CMD_GP_GetPassword
{
	char								szAccounts[NAME_LEN];			//µ«¬º’À∫≈
	char								szMobileCode[32];				// ÷ª˙∫≈¬Î
	CMD_GP_GetPassword()
	{
		memset(this, 0, sizeof(*this));
	}
};

//ªÒ»°√‹¬ÎΩ·π˚
struct CMD_GP_GetPasswordResult
{
	LONG								lErrCode;							//œ˚œ¢±Í÷æ
	char								szPassword[PASS_LEN];				// ÷ª˙∫≈¬Î
	char								szErrorDescribe[128];					//√Ë ˆ–≈œ¢
};

//获取密码,获得用户列表结果
struct CMD_GP_GetAccountListResult
{
	LONG								lErrCode;
	char								szUserNameList[270];				//用户名列表，最多２０个
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
//”√ªß’“ªÿ√‹¬Î
struct CMD_GP_FindPassword
{
	char								szAccounts[NAME_LEN];			//µ«¬º’À∫≈
	char								szEmail[32];				//∞≤»´” œ‰
};

//’“ªÿ√‹¬ÎΩ·π˚
struct CMD_GP_FindPasswordResult
{
	LONG								lErrCode;							//œ˚œ¢±Í÷æ0≥…π¶  1 ß∞‹	
	char								szErrorDescribe[128];					//√Ë ˆ–≈œ¢
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

////紫金岛卡结果
//struct CMD_GP_ScoreGiftResult
//{
//	LONG								lErrCode;							//消息标志0成功  1失败	
//	LONG								lScore;								//用户分数
//	unsigned char								szErrorDescribe[128];				//描述信息
//};
//
////电话卡结果
//struct CMD_GP_UserChargeResult
//{
//	LONG								lErrCode;							//消息标志 1成功  0失败	
//	LONG								lScore;								//用户分数
//	LONG								lGoldeggs;								//用户话费
//	unsigned char								szErrorDescribe[128];					//描述信息
//};

//如果lGameKindid==0 表示所有小游戏发送结束

//Ω·π˚
struct CMD_GP_ScoreGiftResult
{
	LONG								lErrCode;							//œ˚œ¢±Í÷æ0≥…π¶  1 ß∞‹	
	LONG								lScore;								//”√ªß∑÷ ˝
	char								szErrorDescribe[128];				//√Ë ˆ–≈œ¢
};
//Ω·π˚
struct CMD_GP_UserChargeResult
{
	LONG								lErrCode;							//œ˚œ¢±Í÷æ 1≥…π¶  0 ß∞‹	
	LONG								lScore;								//”√ªß∑÷ ˝
	LONG								lGoldeggs;								//”√ªßª∞∑—
	char								szErrorDescribe[128];					//√Ë ˆ–≈œ¢
};

//º∂±Ω·π˚
struct CMD_GP_UserLevelNameResult
{
	char								szTitleName[10];					//√Ë ˆº∂±–≈œ¢
	char								szLevelName1[10];					//º∂±“ª√˚≥∆
	char								szLevelName2[10];					//º∂±∂˛√˚≥∆
	char								szLevelName3[10];					//º∂±»˝√˚≥∆
	char								szLevelName4[10];					//º∂±Àƒ√˚≥∆
	char								szLevelName5[10];					//º∂±ŒÂ√˚≥∆
	char								szLevelName6[10];					//º∂±¡˘√˚≥∆
	char								szLevelName7[10];					//º∂±∆ﬂ√˚≥∆
};

struct tagUserLevelParameter
{
	LONG lUserLevel;														//º∂±∂‘”¶≤Œ ˝
};
//º∂±Ω·π˚
struct CMD_GP_UserLevelResult
{
	LONG								lInfoType;						//√Ë ˆº∂±–≈œ¢
	tagUserLevelParameter				tagUserLeve[8];					//º∂±∂‘”¶≤Œ ˝
};

//»ŒŒÒœ˚œ¢Ω·π˚
struct CMD_GP_TaskResult
{
	LONG								lErrCode;							//œ˚œ¢±Í÷æ 1≥…π¶  0 ß∞‹	
	bool								btype;								//œ˚œ¢¿‡±£¨tŒ™ªÒ»°»ŒŒÒ–≈œ¢£¨fŒ™»∑»œ¡Ï»°
	DWORD								dwTaskID;							//»ŒŒÒID
	char								szTaskName[50];						//»ŒŒÒ√˚≥∆
	char								szGiftName[50];						//Ω±¿¯√˚≥∆
	char								szGiftValue[50];					//Ω±¿¯º€÷µ
	char								szGiftNum[50];						//Ω±¿¯ ˝◊÷
	LONG								lScore;								//”√ªß∑÷ ˝
	LONG								lGoldeggs;							//”√ªßª∞∑—
	char								szErrorDescribe[128];				//√Ë ˆ–≈œ¢
};
//////////////////////////////////////////////////////////////////////////
//”Œœ∑¡–±Ì√¸¡Ó¬Î

#define MDM_GP_SERVER_LIST				2								//¡–±Ì–≈œ¢

#define SUB_GP_LIST_TYPE				100								//¿‡–Õ¡–±Ì
#define SUB_GP_LIST_KIND				101								//÷÷¿‡¡–±Ì
#define SUB_GP_LIST_STATION				102								//’æµ„¡–±Ì
#define SUB_GP_LIST_SERVER				103								//∑øº‰¡–±Ì
#define SUB_GP_LIST_FINISH				104								//∑¢ÀÕÕÍ≥…
#define SUB_GP_LIST_CONFIG				105								//¡–±Ì≈‰÷√

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

//////////////////////////////////////////////////////////////////////////
//œµÕ≥√¸¡Ó¬Î

#define MDM_GP_SYSTEM					3								//œµÕ≥√¸¡Ó

#define SUB_GP_VERSION					100								//∞Ê±æÕ®÷™
#define SUB_SP_SYSTEM_MSG				101								//œµÕ≥œ˚œ¢

//∞Ê±æÕ®÷™
struct CMD_GP_Version
{
	BYTE								bNewVersion;					//∏¸–¬∞Ê±æ
	BYTE								bAllowConnect;					//‘ –Ì¡¨Ω”
};

//////////////////////////////////////////////////////////////////////////
#define MDM_GF_USER						2	
#define MDM_GP_USER						4								//”√ªß–≈œ¢

#define SUB_GP_USER_UPLOAD_FACE			100								//…œ¥´Õ∑œÒ
#define SUB_GP_USER_DOWNLOAD_FACE		101								//œ¬‘ÿÕ∑œÒ
#define SUB_GP_UPLOAD_FACE_RESULT		102								//…œ¥´Ω·π˚
#define SUB_GP_DELETE_FACE_RESULT		103								//…æ≥˝Ω·π˚
#define SUB_GP_CUSTOM_FACE_DELETE		104								//…æ≥˝Õ∑œÒ
//#define SUB_GP_MODIFY_INDIVIDUAL		105								//∏ˆ»À◊ ¡œ
#define SUB_GP_MODIFY_INDIVIDUAL_RESULT	106								//–ﬁ∏ƒΩ·π˚
#define SUB_GP_BANK_RESULT				107								//“¯––Ω·π˚
#define SUB_GP_GET_CHAMPION_LIST_REQ	108								//π⁄æ¸¡–±Ì
#define SUB_GP_GET_CHAMPION_LIST		109								//π⁄æ¸¡–±Ì
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



#define SUB_GP_MATCH_USER_LIST			200								//±®√˚¡–±Ì
#define SUB_GP_USER_SYSTEM_MSG_REQ		201								//”√ªßœµÕ≥œ˚œ¢
#define SUB_GP_USER_SYSTEM_MSG			202								//”√ªßœµÕ≥œ˚œ¢

#define SUB_GP_LOAD_SYSTEM_MSG_REQ		203								//º”‘ÿœµÕ≥œ˚œ¢
#define SUB_GP_LOAD_SYSTEM_MSG			204								//º”‘ÿœµÕ≥œ˚œ¢
#define SUB_GP_WRITE_SYSTEM_MSG			205								//–¥œµÕ≥œ˚œ¢
#define SUB_GP_DELETE_SYS_MSG			206								//…æ≥˝œµÕ≥œ˚œ¢
#define SUB_GP_W_USER_SYS_MSG			207								//–¥”√ªßœµÕ≥œ˚œ¢
#define SUB_GP_MODIFY_MOBILE_EMAIL		208								//ÃÌº” ÷ª˙∫ÕEmail
#define SUB_GP_COST_ISLAND_CARD			209								//◊œΩµ∫ø®≥‰÷µ

#define SUB_GF_ISLAND					410								//充值卡消息
#define SUB_GF_SZFCARD					454								//神州付消息
#define SUB_GF_CZ_ORDER					457								//支付订单消息

////充值卡消息
////紫金岛充值卡
//struct CMD_GP_ISLANDCARD
//{
//	DWORD							dwUserID;							//玩家 ID
//	char							szCardNo[NAME_LEN];					//卡号	
//	char							szCardPwd[NAME_LEN];				//密码
//};
//
////电话卡消息
//struct CMD_GP_SZFCARD
//{
//	DWORD							dwUserID;							//玩家 ID
//	DWORD							dwCardType;							//卡类型
//	DWORD							dwCardFee;							//卡金额
//	char							szCardNo[NAME_LEN];					//卡号	
//	char							szCardPwd[NAME_LEN];				//密码
//};


struct SDownloadGamePrize_Return
{
	LONG lReturn;
	DWORD dwDownloadGameInfo;
	DWORD dwCurScore;
	BYTE nIndex;
	BYTE nSubCmd;
	SDownloadGamePrize_Return()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct SDownloadGamePrize_Returnn : public Object
{
	LONG lReturn;
	DWORD dwDownloadGameInfo;
	DWORD dwCurScore;
	BYTE nIndex;
	BYTE nSubCmd;
	SDownloadGamePrize_Returnn()
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





//∏ˆ»À ÷ª˙∫≈¬Î∫Õ” º˛
struct CMD_GP_ModifyMobileEmail
{
	DWORD							dwUserID;							//ÕÊº“ ID
	char							szMobileCode[NAME_LEN];				// ÷ª˙∫≈¬Î	
	char							szEmailCode[EMAIL_LEN];				//” º˛µÿ÷∑
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

//…Ò÷›∏∂≥‰÷µø®
struct CMD_GP_SZFCARD
{
	DWORD							dwUserID;							//ÕÊº“ ID
	DWORD							dwCardType;							//ø®¿‡–Õ
	DWORD							dwCardFee;							//ø®Ω∂Ó
	char							szCardNo[NAME_LEN];					//ø®∫≈	
	char							szCardPwd[NAME_LEN];				//√‹¬Î
};
//Ω±“‘˘ÀÕœ˚œ¢
struct CM_GF_SCOREGIFT
{
	DWORD								dwUserID;						//”√ªßID
	DWORD								lValue;							//¥Ê¥¢/Ã·»°Ω±“
	char								szToUserName[NAME_LEN];			//∂‘∑Ω’ ªß
    char                               szPassword[PASS_LEN];           //用户密码
};
//¥ÛÃ¸»ŒŒÒ
struct CMD_GP_TASK
{
	DWORD							dwUserID;							//ÕÊº“ ID
	bool							bType;								//œ˚œ¢¿‡±£¨tŒ™ªÒ»°»ŒŒÒ–≈œ¢£¨fŒ™»∑»œ¡Ï»°	
	DWORD							dwTaskID;							//»ŒŒÒID£¨bTypeŒ™tµƒ ±∫ÚŒ™0
};	
//∏ˆ»À◊ ¡œ
struct CMD_GP_ModifyIndividual
{
	DWORD							dwUserID;							//ÕÊº“ ID
	char							szNickname[NAME_LEN];				//ÕÊº“Í«≥∆
	int								nGender;							//ÕÊº“–‘±
	int								nAge;								//ÕÊº“ƒÍ¡‰
	char							szAddress[64];						//ÕÊº“µÿ÷∑
	char							szUnderWrite[32];					//∏ˆ–‘«©√˚
	char							szPassword[33];						//ÕÊº“√‹¬Î
};

//∂®“ÂÕ∑œÒ
struct CMD_GP_UploadCustomFace
{
	DWORD								dwUserID;						//ÕÊº“ ID
	WORD								wCurrentSize;					//µ±«∞¥Û–°
	bool								bLastPacket;					//◊Ó∫Û±Í ∂
	bool								bFirstPacket;					//µ⁄“ª∏ˆ±Í ∂
	BYTE								bFaceData[2048];				//Õ∑œÒ ˝æ›
};

//œ¬‘ÿ≥…π¶
struct CMD_GP_DownloadFaceSuccess
{
	DWORD							dwToltalSize;						//◊‹π≤¥Û–°
	DWORD							dwCurrentSize;						//µ±«∞¥Û–°
	DWORD							dwUserID;							//ÕÊº“ ID
	BYTE							bFaceData[2048];					//Õ∑œÒ ˝æ›
};

//œ¬‘ÿÕ∑œÒ
struct CMD_GP_DownloadFace
{
	DWORD							dwUserID;							//ÕÊº“ ID
};

//…œ¥´Ω·π˚
struct CMD_GP_UploadFaceResult
{
	char							szDescribeMsg[128];					//√Ë ˆ–≈œ¢
	DWORD							dwFaceVer;							//Õ∑œÒ∞Ê±æ
	bool							bOperateSuccess;					//≥…π¶±Í ∂
};

//…æ≥˝Ω·π˚
struct CMD_GP_DeleteFaceResult
{
	char							szDescribeMsg[128];					//√Ë ˆ–≈œ¢
	DWORD							dwFaceVer;							//Õ∑œÒ∞Ê±æ
	bool							bOperateSuccess;					//≥…π¶±Í ∂
};

//…æ≥˝œ˚œ¢
struct CMD_GP_CustomFaceDelete
{
	DWORD							dwUserID;							//ÕÊº“ ID
	DWORD							dwFaceVer;							//Õ∑œÒ∞Ê±æ
};

//–ﬁ∏ƒΩ·π˚
struct CMD_GP_ModifyIndividualResult
{
	char							szDescribeMsg[128];					//√Ë ˆ–≈œ¢
	bool							bOperateSuccess;					//≥…π¶±Í ∂
};

//“¯––Ω·π˚
struct CMD_GP_BankResult
{
	WORD							wCode;								//≤Ÿ◊˜¿‡–Õ
	LONG							lValue;								//Ω±“
};
//////////////////////////////////////////////////////////////////////////

#pragma pack()
#endif