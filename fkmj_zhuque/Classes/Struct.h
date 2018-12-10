#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE

#include "types.h"
#include "Define.h"
//////////////////////////////////////////////////////////////////////////////////
//游戏列表
#pragma pack(8)//zhuque


struct systemtime {
	word wYear;
	word wMonth;
	word wDayOfWeek;
	word wDay;
	word wHour;
	word wMinute;
	word wSecond;
	word wMilliseconds;

	template<typename T1,  typename T2>
	inline std::string toString (T1 p1, T2 p2)
	{
		std::ostringstream stream;
		stream << p1 << p2;
		return stream.str();
	}
	template<typename T1,  typename T2,  typename T3>
	inline std::string toString (T1 p1, T2 p2, T3 p3)
	{
		std::ostringstream stream;
		stream << p1 << p2 << p3;
		return stream.str();
	}

	std::string paseInt(int iValue,int iCout)
	{
		char kTxt[12];
		std::string kTemp = toString("%0",iCout,"d");
		sprintf(kTxt,kTemp.c_str(),iValue);
		return kTxt;
	}

	std::string toString1()
	{
		std::string kStr;
		kStr = toString(kStr,wYear);
		kStr = toString(kStr,"-");
		kStr = toString(kStr,paseInt(wMonth,2));
		kStr = toString(kStr,"-");
		kStr = toString(kStr,paseInt(wDay,2));
		kStr = toString(kStr,'\n');
		kStr = toString(kStr,paseInt(wHour,2));
		kStr = toString(kStr,":");
		kStr = toString(kStr,paseInt(wMinute,2));
		kStr = toString(kStr,":");
		kStr = toString(kStr,paseInt(wSecond,2));
		return kStr;
	}
	std::string toString2()
	{
		std::string kStr;
		kStr = toString(kStr,wYear);
		kStr = toString(kStr,"-");
		kStr = toString(kStr,paseInt(wMonth,2));
		kStr = toString(kStr,"-");
		kStr = toString(kStr,paseInt(wDay,2));
		kStr = toString(kStr,' ');
		kStr = toString(kStr,paseInt(wHour,2));
		kStr = toString(kStr,":");
		kStr = toString(kStr,paseInt(wMinute,2));
		kStr = toString(kStr,":");
		kStr = toString(kStr,paseInt(wSecond,2));
		return kStr;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//道具索引
#define PROP_DOUBLE						0									//双倍积分卡
#define PROP_FOURDOLD					1									//四倍积分卡
#define PROP_NEGAGIVE					2									//负分清零
#define PROP_FLEE						3									//清逃跑率
#define PROP_BUGLE						4									//小喇叭
#define PROP_KICK						5									//防踢卡
#define PROP_SHIELD						6									//护身符
#define PROP_MEMBER_1					7									//会员道具
#define PROP_MEMBER_2					8									//会员道具
#define PROP_MEMBER_3					9									//会员道具
#define PROP_MEMBER_4					10									//会员道具
#define PROP_MEMBER_5					11									//会员道具
#define PROP_MEMBER_6					12									//会员道具
#define PROP_MEMBER_7					13									//会员道具
#define PROP_MEMBER_8					14									//会员道具

byte const g_PropTypeList[]=
{
	PROP_DOUBLE	,
	PROP_FOURDOLD,
	PROP_NEGAGIVE,
	PROP_FLEE	,
	PROP_BUGLE	,
	PROP_KICK	,
	PROP_SHIELD	,
	PROP_MEMBER_1,
	PROP_MEMBER_2,
	PROP_MEMBER_3,
	PROP_MEMBER_4,
	PROP_MEMBER_5,
	PROP_MEMBER_6,
	PROP_MEMBER_7,
	PROP_MEMBER_8,
};
#define PROPERTY_COUNT			(sizeof(g_PropTypeList)/sizeof(g_PropTypeList[0]))//道具数目


//游戏类型结构
struct tagGameType
{
	word								wJoinID;							//挂接索引
	word								wSortID;							//排序索引
	word								wTypeID;							//类型索引
	char								szTypeName[LEN_TYPE];				//种类名字
};

//游戏名称结构
struct tagGameKind
{
#if 1
	word								wSortID;							//排序号码
	word								wTypeID;							//类型号码
	word								wKindID;							//名称号码
	dword								dwMaxVersion;						//最新版本
	dword								dwOnLineCount;						//在线数目
#else
	word								wTypeID;							//类型号码
	word								wJoinID;							//挂接索引
	word								wSortID;							//排序号码
	word								wKindID;							//名称号码
	word								wGameID;							//模块索引
	dword								dwOnLineCount;						//在线人数
	dword								dwFullCount;						//满员人数
#endif
	char								szKindName[LEN_KIND];				//游戏名字
	char								szProcessName[LEN_MODULE];			//进程名字
};


//游戏房间列表结构
struct tagGameServer
{
#if 1
	word								wSortID;							//排序号码
	word								wKindID;							//名称号码
	word								wServerID;							//房间号码
	word								wNodeID;							//站点号码
	word								wServerPort;						//房间端口
	//dword								dwServerAddr;						//房间地址 彭文删除
	dword							    dwServerAddr;				        //电信房间地址 彭文添加
	dword                               dwServerAddrCUN;                    //联通房间地址 彭文添加
	dword								dwOnLineCount;						//在线人数
	dword                               dwFullCount;                        //最大在线人数 彭文添加
	char								szServerName[LEN_SERVER];			//房间名称
	int                             lRestrictScore;                     //彭文添加 限制积分
	int								lRestrictGrade;                     //限制等级
	int							    lEnterScore;                        //进入游戏最低金币
	int                             lCellScore;                         //彭文添加 单位积分
#else
	word							wKindID;							//名称索引
	word							wNodeID;							//节点索引
	word							wSortID;							//排序索引
	word							wServerID;							//房间索引
	//word                            wServerKind;                        //房间类型
	word							wServerType;						//房间类型
	word							wServerPort;						//房间端口
	SCORE							lCellScore;							//单元积分
	SCORE							lEnterScore;						//进入积分
	dword							dwServerRule;						//房间规则
	dword							dwOnLineCount;						//在线人数
	dword							dwAndroidCount;						//机器人数
	dword							dwFullCount;						//满员人数
	char							szServerAddr[32];					//房间名称
	char							szServerName[LEN_SERVER];			//房间名称
#endif
};

//比赛报名
struct tagSignupMatchInfo
{
	word							wServerID;							//房间标识
	dword							dwMatchID;							//比赛标识
	dword							dwMatchNO;							//比赛场次
};

//比赛信息
struct tagGameMatch
{
	//基本信息
	word							wServerID;							//房间标识
	dword							dwMatchID;							//比赛标识
	dword							dwMatchNO;							//比赛场次	
	byte							cbMatchType;						//比赛类型
	char							szMatchName[32];					//比赛名称

	//比赛信息
	byte							cbMemberOrder;						//会员等级
	byte							cbMatchFeeType;						//扣费类型
	SCORE							lMatchFee;							//比赛费用	
	SCORE							lMatchEnterScore;					//准入金币

	//比赛信息
	word							wStartUserCount;					//开赛人数
	word							wMatchPlayCount;					//比赛局数

	//比赛奖励
	word							wRewardCount;						//奖励人数

	//比赛时间
	systemtime						MatchStartTime;						//开始时间
	systemtime						MatchEndTime;						//结束时间	
};

//在线信息
struct tagOnLineInfoKind
{
	word							wKindID;							//类型标识
	dword							dwOnLineCount;						//在线人数
};

//在线信息
struct tagOnLineInfoServer
{
	word							wServerID;							//房间标识
	dword							dwOnLineCount;						//在线人数
};

//////////////////////////////////////////////////////////////////////////////////
//用户信息

//桌子状态
struct tagTableStatus
{
	byte							cbTableLock;						//锁定标志
	byte							cbPlayStatus;						//游戏标志
};

//用户状态
struct tagUserStatus
{
	word							wTableID;							//桌子索引
	word							wChairID;							//椅子位置
	byte							cbUserStatus;						//用户状态
};

//用户属性
struct tagUserAttrib
{
	byte							cbCompanion;						//用户关系
};

//用户积分
struct tagUserScore
{
#if 1
	long long							lScore;						//存储金币
	dword							dwWinCount;							//胜利盘数
	dword							dwLostCount;						//失败盘数
	dword							dwDrawCount;						//和局盘数
	dword							dwFleeCount;						//逃跑盘数
	dword							dwExperience;						//用户经验

	//彭文添加
	long long                            lGold;   //用户的金币 2011-9-30前名字为lGems
	long long                            lInsure;  // //用户的宝石 2011-9-30前为荣誉 lHonor
	//彭文添加
	dword                               dwGrade;  //用户的等级（不超过256） 2011-9-30前为dwClass     

	//osc add
	//char								chGradeName[32];		//等级称谓
	//添加结束
	//=====Author<cxf>;DateTime<2012-03-02>
	//dword								lKindIDWinCount;				//某游戏胜利盘数
	//dword								lKindIDLostCount;				//某游戏失败盘数
	//dword								lKindIDDrawCount;				//某游戏和局盘数
	//dword								lKindIDFleeCount;				//某游戏断线数目
	//=====END
#else
	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行

	//输赢信息
	dword							dwWinCount;							//胜利盘数
	dword							dwLostCount;						//失败盘数
	dword							dwDrawCount;						//和局盘数
	dword							dwFleeCount;						//逃跑盘数

	//全局信息
	dword							dwUserMedal;						//用户奖牌
	dword							dwExperience;						//用户经验
	unsigned int					lLoveLiness;						//用户魅力
#endif
};

//用户积分
struct tagMobileUserScore
{
	//积分信息
	SCORE							lScore;								//用户分数

	//输赢信息
	dword							dwWinCount;							//胜利盘数
	dword							dwLostCount;						//失败盘数
	dword							dwDrawCount;						//和局盘数
	dword							dwFleeCount;						//逃跑盘数

	//全局信息
	dword							dwExperience;						//用户经验
};


//道具使用
struct tagUsePropertyInfo
{
	word                            wPropertyCount;                     //道具数目
	word                            dwValidNum;						    //有效数字
	dword                           dwEffectTime;                       //生效时间
};


//用户道具
struct tagUserProperty
{
	word                            wPropertyUseMark;                   //道具标示
	tagUsePropertyInfo              PropertyInfo[MAX_PT_MARK];			//使用信息   
};

//道具包裹
struct tagPropertyPackage
{
	word                            wTrumpetCount;                     //小喇叭数
	word                            wTyphonCount;                      //大喇叭数
};

//时间信息
struct tagTimeInfo
{
	dword						dwEnterTableTimer;						//进出桌子时间
	dword						dwLeaveTableTimer;						//离开桌子时间
	dword						dwStartGameTimer;						//开始游戏时间
	dword						dwEndGameTimer;							//离开游戏时间
};

//用户信息
struct tagUserInfo
{
	//基本属性
	dword							dwUserID;							//用户 I D
	dword							dwGameID;							//游戏 I D
	dword							dwGroupID;							//社团 I D
	char							szNickName[LEN_NICKNAME];			//用户昵称
	char							szGroupName[LEN_GROUP_NAME];		//社团名字
	char							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
	char							szLogonIP[LEN_ACCOUNTS];			//登录IP
	char							szHeadHttp[LEN_USER_NOTE];			//头像HTTP
	char							szGPS[LEN_NICKNAME];			//GPS

	//头像信息
	word							wFaceID;							//头像索引
	dword							dwCustomID;							//自定标识

	//用户资料
	byte							cbGender;							//用户性别
	byte							cbMemberOrder;						//会员等级
	byte							cbMasterOrder;						//管理等级

	//用户状态
	word							wTableID;							//桌子索引
	word							wLastTableID;						//游戏桌子
	word							wChairID;							//椅子索引
	byte							cbUserStatus;						//用户状态

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsureScore;							//用户银行
	SCORE							lGameGold;								//用户元宝

	//游戏信息
	dword							lWinCount;							//胜利盘数
	dword							lLostCount;						//失败盘数
	dword							lDrawCount;						//和局盘数
	dword							lFleeCount;						//逃跑盘数
	dword							lExperience;						//用户经验
	dword							lLoveLiness;						//用户魅力

	dword							iStarValue;						//评分
	dword							iStartCout;						//评分
	//时间信息
	tagTimeInfo						TimerInfo;		
};

//用户基本信息结构
struct tagUserInfoHead
{
#if 1
	//用户属性
	word								wFaceID;							//头像索引
	dword								dwUserID;							//用户 I D
	dword								dwGameID;							//游戏 I D
	dword								dwGroupID;							//社团索引
	dword								dwUserRight;						//用户等级
	int								    lLoveLiness;						//用户魅力
	dword								dwMasterRight;						//管理权限

	//用户属性
	byte								cbGender;							//用户性别
	byte								cbMemberOrder;						//会员等级
	byte								cbMasterOrder;						//管理等级

	//用户状态
	word								wTableID;							//桌子号码
	word								wChairID;							//椅子位置
	byte								cbUserStatus;						//用户状态

	//用户积分
	tagUserScore						UserScoreInfo;						//积分信息

	//扩展信息
	dword								dwCustomID;					//上传头像
	dword								dwPropResidualTime[30];	            //道具时间
	//用户帐号
	char								szAccounts[NAME_LEN];				//帐号
#else
	//用户属性
	dword							dwGameID;							//游戏 I D
	dword							dwUserID;							//用户 I D
	dword							dwGroupID;							//社团 I D

	//头像信息
	word							wFaceID;							//头像索引
	dword							dwCustomID;							//自定标识

	//用户属性
	byte							cbGender;							//用户性别
	byte							cbMemberOrder;						//会员等级
	byte							cbMasterOrder;						//管理等级

	//用户状态
	word							wTableID;							//桌子索引
	word							wChairID;							//椅子索引
	byte							cbUserStatus;						//用户状态

	//积分信息
	longlong						lScore;								//用户分数
	longlong						lGrade;								//用户成绩
	longlong						lInsure;							//用户银行

	//游戏信息
	dword							dwWinCount;							//胜利盘数
	dword							dwLostCount;						//失败盘数
	dword							dwDrawCount;						//和局盘数
	dword							dwFleeCount;						//逃跑盘数
	dword							dwUserMedal;						//用户奖牌
	dword							dwExperience;						//用户经验
	unsigned int					lLoveLiness;						//用户魅力
#endif
};

//头像信息
struct tagCustomFaceInfo
{
	dword							dwDataSize;							//数据大小
	dword							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//用户信息
struct tagUserRemoteInfo
{
	//用户信息
	dword							dwUserID;							//用户标识
	dword							dwGameID;							//游戏标识
	char							szNickName[LEN_NICKNAME];			//用户昵称

	//等级信息
	byte							cbGender;							//用户性别
	byte							cbMemberOrder;						//会员等级
	byte							cbMasterOrder;						//管理等级

	//位置信息
	word							wKindID;							//类型标识
	word							wServerID;							//房间标识
	char							szGameServer[LEN_SERVER];			//房间位置
};


//等级配置
struct tagGrowLevelConfig
{
	word              wLevelID;             //等级 I D
	dword             dwExperience;           //相应经验
};

//等级参数
struct tagGrowLevelParameter
{
	word              wCurrLevelID;           //当前等级
	dword             dwExperience;           //当前经验
	dword             dwUpgradeExperience;        //下级经验
	SCORE             lUpgradeRewardGold;         //升级奖励
	SCORE             lUpgradeRewardIngot;        //升级奖励
};


//////////////////////////////////////////////////////////////////////////////////

//广场子项
struct tagGamePlaza
{
	word							wPlazaID;							//广场标识
	char							szServerAddr[32];					//服务地址
	char							szServerName[32];					//服务器名
};

//级别子项
struct tagLevelItem
{
	dword							lLevelScore;						//级别积分
	char							szLevelName[16];					//级别描述
};

//会员子项
struct tagMemberItem
{
	byte							cbMemberOrder;						//等级标识
	char							szMemberName[16];					//等级名字
};

//管理子项
struct tagMasterItem
{
	byte							cbMasterOrder;						//等级标识
	char							szMasterName[16];					//等级名字
};

//列表子项
struct tagColumnItem
{
	byte							cbColumnWidth;						//列表宽度
	byte							cbDataDescribe;						//字段类型
	char							szColumnName[16];					//列表名字
};

//地址信息
struct tagAddressInfo
{
	char							szAddress[32];						//服务地址
};

//数据信息
struct tagDataBaseParameter
{
	word							wDataBasePort;						//数据库端口
	char							szDataBaseAddr[32];					//数据库地址
	char							szDataBaseUser[32];					//数据库用户
	char							szDataBasePass[32];					//数据库密码
	char							szDataBaseName[32];					//数据库名字
};

//房间配置
struct tagServerOptionInfo
{
	//挂接属性
	word							wKindID;							//挂接类型
	word							wNodeID;							//挂接节点
	word							wSortID;							//排列标识

	//税收配置
	word							wRevenueRatio;						//税收比例
	SCORE							lServiceScore;						//服务费用

	//房间配置
	SCORE							lRestrictScore;						//限制积分
	SCORE							lMinTableScore;						//最低积分
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分

	//会员限制
	byte							cbMinEnterMember;					//最低会员
	byte							cbMaxEnterMember;					//最高会员

	//房间属性
	dword							dwServerRule;						//房间规则
	char							szServerName[LEN_SERVER];			//房间名称
};


//////////////////////////////////////////////////////////////////////////////////
//比赛信息

//赛事信息
struct tagMatchInfo
{
	char							szTitle[4][64];						//信息标题
	word							wGameCount;							//游戏局数
};

//提示信息
struct tagMatchWaitTip
{
	SCORE							lScore;								//当前积分
	word							wRank;								//当前名次
	word							wCurTableRank;						//本桌名次
	word							wUserCount;							//当前人数
	word							wPlayingTable;						//游戏桌数
	char							szMatchName[LEN_SERVER];			//比赛名称
};

//比赛结果
struct tagMatchResult
{
	char							szDescribe[256];					//得奖描述
	dword							dwGold;								//金币奖励
	dword							dwMedal;							//奖牌奖励
	dword							dwExperience;						//经验奖励
};

//比赛描述
struct tagMatchDesc
{
	char							szTitle[4][16];						//信息标题
	char							szDescribe[4][64];					//描述内容
	dword							crTitleColor;						//标题颜色
	dword							crDescribeColor;					//描述颜色
};

//////////////////////////////////////////////////////////////////////////////////
//排行榜
//排行榜项
struct tagRankItem
{
	dword							dwUserID;							//用户ID
	SCORE							lScore;								//用户积分
	unsigned short					szNickname[LEN_NICKNAME];			//用户昵称
};

#pragma pack()

#endif