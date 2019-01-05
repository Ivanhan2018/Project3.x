#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE
#pragma once
#include "Define.h"
#pragma pack(1)  // #pragma pack(1)让编译器将结构体数据强制连续排列，这样的话，sizeof（struct s）输出的结果就是5了。

/////////////////////////////////////////////////////////////////////
// 游戏列表


// 游戏类型	tagGameType
typedef struct
{
#if 0
	WORD				wJoinID;						//挂接索引
#endif     
    WORD				wSortID;						//排序索引
    WORD				wTypeID;						//类型索引
	TCHART				szTypeName[LEN_TYPE];			//种类名字

}tagGameType;

// 游戏种类 tagGameKind
typedef struct
{
	WORD				wTypeID;						//类型索引
#if 0
	WORD				wJoinID;						//挂接索引
#endif
	WORD			        wSortID;						//排序索引
	WORD				wKindID;						//类型索引
	WORD				wGameID;						//模块索引
	DWORD				dwOnLineCount;					//在线人数
	TCHART				szKindName[LEN_KIND];			//游戏名字
#if 0
	TCHART				szProcessName;					//进程名字
#else
	TCHART							szProcessName[LEN_PROCESS];			//进程名字
#endif
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
#if 0
	WORD				            wNodeID;						    //节点索引
#endif
	WORD							wSortID;							//排序索引
	WORD							wServerID;							//房间索引
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
#if 0
	DWORD				dwFullCount;					//满员人数
	TCHART				szServerAddr[LEN_SERVER];		//房间地址
	TCHART				szServerName[LEN_SERVER-8];		//房间名称

	// 积分信息
	SCORE				lServerScore;					//单元积分 8BYTES
	SCORE				lMinServerScore;				//需要积分 8BYTES
#else
	//积分信息
	SCORE							lServerScore;						//单元积分
	SCORE							lMinServerScore;					//需要积分

	//房间信息
	TCHART							szServerAddr[32];					//房间地址
	TCHART							szServerName[LEN_SERVER];			//房间名称

	//梦成网络房间密码
	WORD							wServerKind;							//是否启用密码
	TCHART							szServerPassWork[LEN_SERVER];			//房间密码
#endif
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

//桌子状态<add by hxh 20160624>
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
#if 0
	// 全局信息
	DWORD				dwUserMedal;					//用户奖牌
	DWORD				dwExperience;					//用户经验
	DWORD				dwLoveLiness;					//用户魅力
#else
	DWORD							dwExperience;						//用户经验
#endif
}tagUserScore;

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
#if 0
	DWORD				dwGameID;						// 游戏ID
#endif
	DWORD				dwUserID;						// 用户ID
	TCHART							szNickName[LEN_NICKNAME];			//用户昵称

	//头像信息
	WORD				wFaceID;						// 头像索引
#if 0
	DWORD				dwCustomID;						// 自定标识
#endif
	// 用户属性
	BYTE				cbGender;						// 用户性别
	BYTE				cbMemberOrder;					// 会员等级 1,2,3,4,5
#if 1
	BYTE				cbMasterOrder;						//管理等级
#endif

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

//用户信息
struct tagDDZUserInfoHead
{
	//用户属性
	DWORD							dwUserID;							//用户 I D

	//用户资料
	WORD							wFaceID;							//头像索引
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//用户状态
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lInsure;							//用户银行
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwExperience;						//用户经验
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

//// 会员子项	tagMemberItem
//typedef struct
//{
//	BYTE				cbMemberOrder;					// 等级标识
//	TCHART				szMemberName[16];				// 等级名字
//} tagMemberItem;

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