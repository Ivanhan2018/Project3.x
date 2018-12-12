#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE
#pragma once
#include "Define.h"
#pragma pack(1)

/////////////////////////////////////////////////////////////////////
// 游戏列表
/*********************************************************************************
* 没有wJoinID挂接索引字段
* 没有wNodeID节点索引字段
* 没有dwGameID游戏ID字段
**********************************************************************************/

// 游戏类型	tagGameType
typedef struct
{   
    WORD				wSortID;						//排序索引
    WORD				wTypeID;						//类型索引
	TCHART				szTypeName[LEN_TYPE];			//种类名字
}tagGameType;

// 游戏种类 tagGameKind
typedef struct
{
	WORD				wTypeID;						//类型索引
	WORD			    wSortID;						//排序索引
	WORD				wKindID;						//类型索引
	WORD				wGameID;						//模块索引
	DWORD				dwOnLineCount;					//在线人数
	TCHART				szKindName[LEN_KIND];			//游戏名字
	TCHART				szProcessName[LEN_PROCESS];		//进程名字
}tagGameKind;

// 游戏节点	tagGameNode
typedef struct  
{
	WORD				wKindID;						//名字索引
	WORD				wJoinID;						//挂接索引
	WORD				wSortID;						//排序索引
	WORD				wNodeID;						//节点索引
	TCHART				szNodeName;						//节点名字
} tagGameNode;

//定制类型 tagGamePage
typedef struct  
{
	WORD				wPageID;						//页面索引
	WORD				wKindID;						//名称索引
	WORD				wNodeID;						//节点索引
	WORD				wSortID;						//排序索引
	WORD				wOperateType;					//控制类型
	TCHART				szDisPlayeName[LEN_PAGE];		//显示名称
} tagGamePage;

// tagGameServer 房间信息里应该增加限制条件
struct tagGameServer
{
	//房间信息
	WORD							wKindID;							//名称索引
	WORD							wSortID;							//排序索引
	WORD							wServerID;							//房间索引
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	//积分信息
	SCORE							lServerScore;						//单元积分
	SCORE							lMinServerScore;					//需要积分

	//房间信息
	TCHART							szServerAddr[32];					//房间地址
	TCHART							szServerName[LEN_SERVER];			//房间名称

	//房间密码
	WORD							wServerKind;							//是否启用密码
	TCHART							szServerPassWork[LEN_SERVER];			//房间密码
};

// 在线信息 tagOnLineInfoKind
typedef struct
{
	WORD				wKindID;						//类型标识
	DWORD				dwOnLineCount;					//在线人数
} tagOnLineInfoKind;

// 在线信息 tagOnLineInfoServer
typedef struct
{
	WORD				wServerID;						//房间标识
	DWORD				dwOnLineCount;					//在线人数
} tagOnLineInfoServer;

/////////////////////////////////////////////////////////////////////////////
//#pragma mark -
//#pragma mark 用户信息

// 桌子状态 tagTableStatus
typedef struct
{
	BYTE				cbTableLock;					//锁定标志
	BYTE				cbPlayStatus;					//游戏标志
} tagTableStatus;

//桌子状态
struct CMD_GR_TableStatus
{
	WORD							wTableID;		//桌子号码
	tagTableStatus                  TableStatus;	//桌子状态
};

// 用户状态 tagUserStatus
typedef struct 
{
	WORD				wTableID;						//桌子索引
	WORD				wChairID;						//椅子位置
	BYTE				cbUserStatus;					//用户状态
} tagUserStatus;

// 用户积分 tagUserScore
typedef struct 
{
	SCORE							lScore;								//用户分数
	SCORE							lInsure;							//用户银行
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwExperience;						//用户经验
}tagUserScore;

struct tagUserGold
{
	LONG								lExperience;
	LONG								lScore;								
	LONG								lGoldEggs;										
	LONG								lStorageGold;
};

//#pragma mark -
//#pragma mark 手机用户积分
typedef struct
{
	// 积分信息
	SCORE				lScore;							// 用户分数

	// 输赢信息
	DWORD				dwWinCount;						//胜利盘数
	DWORD				dwLostCount;					//失败盘数
	DWORD				dwDrawCount;					//和局盘数
	DWORD				dwFleeCount;					//逃跑盘数

	//全局信息
	DWORD				dwExperience;					//用户经验
} tagMobileUserScore;


// 用户信息 tagUserInfoHead 玩家信息
typedef struct  
{
	// 用户属性
	DWORD				dwUserID;						// 用户ID

	//头像信息
	WORD				wFaceID;						// 头像索引
	// 用户属性
	BYTE				cbGender;						// 用户性别
	BYTE				cbMemberOrder;					// 会员等级 1,2,3,4,5
	BYTE				cbMasterOrder;					//管理等级

	// 用户状态
	WORD				wTableID;						// 桌子索引
	WORD				wChairID;						// 椅子索引
	BYTE				cbUserStatus;					// 用户状态

	// 积分信息
	SCORE                           lScore;             // 用户分数
	SCORE                           lInsure;            // 用户银行

	// 游戏信息
	DWORD				dwWinCount;						// 胜利盘数
	DWORD				dwLostCount;					// 失败盘数
	DWORD				dwDrawCount;					// 和局盘数
	DWORD				dwFleeCount;					// 逃跑盘数
	DWORD				dwExperience;					// 用户经验
} tagGameInUserInfo;	// 游戏中自己的信息

//用户信息结构
struct tagUserData
{
	//用户属性
	WORD								wFaceID;							//头像索引
	DWORD								dwUserID;							//用户 I D
	DWORD								dwGroupID;							//社团索引
	DWORD								dwGameID;							//游戏I D
	DWORD								dwUserRight;						//用户等级
	DWORD								dwMasterRight;						//管理权限
	char								szName[NAME_LEN];					//用户名字
	char								szNickName[NAME_LEN];
	char								szGroupName[GROUP_LEN];				//社团名字
	char								szUnderWrite[UNDER_WRITE_LEN];		//个性签名
	char                                szZipCode[ZIP_CODE_LEN];            
	char                                szDwellingPlace[CITY_LEN];			
	DWORD								sdServerID;
	DWORD								sdKindID;
	LONG								lWeekMingci;
	LONG								lWeekWinCount;
	//用户属性
	BYTE								cbGender;							//用户性别
	BYTE								cbMemberOrder;						//会员等级
	BYTE								cbMasterOrder;						//管理等级

	//用户积分
	LONG								lInsureScore;
	LONG								lScore;								//用户分数
	LONG								lWinCount;							//胜利盘数
	LONG								lLostCount;							//失败盘数
	LONG								lDrawCount;							//和局盘数
	LONG								lFleeCount;							//断线数目
	LONG								lExperience;						//用户经验
	LONG								lGoldEggs;							
	LONG                                lMatchScore;
	//用户状态
	WORD								wSortID;							
	WORD								wTableID;							//桌子号码
	WORD								wChairID;							
    //椅子位置
	BYTE								cbUserStatus;						//用户状态

	//其他信息
	BYTE								cbCompanion;						//用户关系
};

// 扩展昵称
typedef struct
{
	WORD				wDataSize;						// 数据大小
	WORD				wDataDecribe;					// 数据描述
} tagDataDescribe;

// 头像信息 tagCustomFaceInfo
typedef struct
{
	DWORD				dwDataSize;						// 数据大小
	DWORD				dwCutstomFace[FACE_CX*FACE_CY];	// 图片信息
} tagCustomFaceInfo;

struct tagGameStation
{
	WORD								wSortID;
	WORD								wKindID;
	WORD								wJoinID;
	WORD								wStationID;
	char								szStationName[STATION_LEN];
};

enum EN_MatchStatus
{
	EN_NULL								= 0,
	EN_LOOKON							= 1,
	EN_OUT								= 2,
	EN_UNDETERMINED						= 3,
	EN_RATED							= 4,
	EN_MATCH							= 5,
};

// 广场子项 tagGamePlaza
typedef struct
{
	WORD				wPlazaID;						// 广场标识
	TCHART				szServerAddr[32];				// 服务地址
	TCHART				szServerName[32];				// 服务器名
} tagGamePlaza;

// 级别子项 tagLevelItem
typedef struct
{
	long				lLevelScore;					// 级别积分
	TCHART				szLevelName[16];				// 级别描述
} tagLevelItem;

// 管理子项	tagMasterItem
typedef struct
{
	BYTE				cbMasterOrder;					// 等级标识
	TCHART				szMasterName[16];				// 等级名字
} tagMasterItem;

// 列表子项 tagColumnItem
typedef struct
{
	BYTE				cbColumnWidth;					// 列表宽度
	BYTE				cbDataDescribe;					// 字段类型
	TCHART				szColumnName[16];				// 列表名字
} tagColumnItem;

// 地址信息 tagAddressInfo
typedef struct
{
	TCHART				szAddress[32];					// 服务器地址
} tagAddressInfo;

// 数据库信息 tagDataBaseParameter
typedef struct
{
	WORD				wDataBasePort;					// 数据库端口
	TCHART				szDataBaseAddr[32];				// 数据库地址
	TCHART				szDataBaseUser[32];				// 数据库用户
	TCHART				szDataBasePass[32];				// 数据库密码
	TCHART				szDataBaseName[32];				// 数据库名字
} tagDataBaseParameter;

#pragma pack()

#endif