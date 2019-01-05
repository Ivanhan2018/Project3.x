#ifndef GLOBAL_DEF_HEAD_FILE
#define GLOBAL_DEF_HEAD_FILE
#pragma once

#include "GBEvent.h"
#include "struct.h"
#include "cocos2d.h"
//////////////////////////////////////////////////////////////////////////
//公共定义

#define MAX_CHAIR						100								//最大椅子
#define MAX_CHAIR_NORMAL				8								//最大人数

#define MAX_ANDROID						256								//最大机器
#define MAX_CHAT_LEN					128								//聊天长度  2010.12.21 by gaoshan
#define LIMIT_CHAT_TIMES				1200							//限时聊天
#define MAX_TABLES                      200                             //◊Ó¥Û◊¿◊” ˝ 2010.9.14 by gaoshan add
/////////////////////////////////////////////////////////////////////////////////////////

//常用常量

//无效数值
#define INVALID_BYTE				((BYTE)(0xFF))						//无效数值
#define INVALID_WORD				((WORD)(0xFFFF))					//无效数值
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//无效数值

//////////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define INVALID_TEAM				INVALID_WORD						//无效组号
#define INVALID_TABLE				INVALID_WORD						//无效桌子
#define INVALID_CHAIR				INVALID_WORD
#define INVALID_ID                  INVALID_DWORD


//∫Í∂®“Â

//端口定义
#define PORT_VIDEO_SERVER				7600								// ”∆µ∑˛ŒÒ∆˜
#define PORT_LOGON_SERVER				9001								//登陆端口
#define PORT_CENTER_SERVER				9010								//协调端口
#define PORT_INFO_SERVER				9019								//–≈œ¢∑˛ŒÒ∆˜

//Õ¯¬Á ˝æ›∂®“Â
//#define SOCKET_VER						0x66								//Õ¯¬Á∞Ê±æ
#define SOCKET_VER						0x01
#define SOCKET_BUFFER					16384								//网络缓冲
#define SOCKET_PACKET					(SOCKET_BUFFER-sizeof(CMD_Head)-2*sizeof(DWORD))

/////////////////////////////////////////////////////////////////////////////////////////

//内核命令
#define MDM_KN_COMMAND				0									//内核命令
#define SUB_KN_DETECT_SOCKET		1									//检测命令
#define SUB_KN_SHUT_DOWN_SOCKET			2									//÷–∂œÕ¯¬Á

//登陆状态
enum enLoginState
{
    enLogin_Server,                     //登陆服务器
    enGame_Server,                      //游戏服务器
};

//网络内核
struct CMD_Info
{
	BYTE								cbVersion;							//∞Ê±æ±Í ∂
	BYTE								cbCheckCode;						//效验字段
	WORD								wPacketSize;						//数据大小
};

//网络命令
struct CMD_Command
{
	WORD								wMainCmdID;							//主命令码
	WORD								wSubCmdID;							//子命令码
};

//网络包头
struct CMD_Head
{
	CMD_Info							CmdInfo;							//基础结构
	CMD_Command							CommandInfo;						//命令信息
};

//网络缓冲
struct CMD_Buffer
{
	CMD_Head							Head;								//数据包头
	BYTE								cbBuffer[SOCKET_PACKET];			//数据缓冲
};

//ºÏ≤‚Ω·ππ–≈œ¢
struct CMD_KN_DetectSocket
{
	DWORD								dwSendTickCount;					//∑¢ÀÕ ±º‰
	DWORD								dwRecvTickCount;					//Ω” ’ ±º‰
};

/////////////////////////////////////////////////////////////////////////////////////////

//IPC  ˝æ›∂®“Â
#define IPC_VER							0x0001								//IPC ∞Ê±æ
#define IPC_IDENTIFIER					0x0001								//±Í ∂∫≈¬Î
#define IPC_PACKAGE						4096								//◊Ó¥Û IPC ∞¸
#define IPC_BUFFER						(sizeof(IPC_Head)+IPC_PACKAGE)		//ª∫≥Â≥§∂»

//IPC  ˝æ›∞¸Õ∑
struct IPC_Head
{
	WORD								wVersion;							//IPC ∞Ê±æ
	WORD								wDataSize;							// ˝æ›¥Û–°
	WORD								wMainCmdID;							//÷˜√¸¡Ó¬Î
	WORD								wSubCmdID;							//◊”√¸¡Ó¬Î
};

//IPC ª∫≥ÂΩ·ππ
struct IPC_Buffer
{
	IPC_Head							Head;								// ˝æ›∞¸Õ∑
	BYTE								cbBuffer[IPC_PACKAGE];				// ˝æ›ª∫≥Â
};

//////////////////////////////////////////////////////////////////////////

//≥§∂»∫Í∂®“Â
#define TYPE_LEN						32									//÷÷¿‡≥§∂»
#define KIND_LEN						32									//¿‡–Õ≥§∂»
#define STATION_LEN						32									//’æµ„≥§∂»
#define SERVER_LEN						32									//∑øº‰≥§∂»
#define MODULE_LEN						32									//Ω¯≥Ã≥§∂»
#define PASS_LEN						33									//√‹¬Î≥§∂»

//–‘±∂®“Â
#define GENDER_NULL						0									//Œ¥÷™–‘±
#define GENDER_BOY						1									//ƒ––‘–‘±
#define GENDER_GIRL						2									//≈Æ–‘–‘±

//”Œœ∑¿‡–Õ
#define GAME_GENRE_SCORE				0x0001								//µ„÷µ¿‡–Õ
#define GAME_GENRE_GOLD					0x0002								//Ω±“¿‡–Õ
#define GAME_GENRE_MATCH				0x0004								//±»»¸¿‡–Õ
#define GAME_GENRE_EDUCATE				0x0008								//—µ¡∑¿‡–Õ

//”Œœ∑¿‡–ÕΩ·ππ
/*struct tagGameType
{
	WORD								wSortID;							//≈≈–Ú∫≈¬Î
	WORD								wTypeID;							//÷÷¿‡∫≈¬Î
	char								szTypeName[TYPE_LEN];				//÷÷¿‡√˚◊÷
};

//”Œœ∑√˚≥∆Ω·ππ
struct tagGameKind
{
	WORD								wSortID;							//≈≈–Ú∫≈¬Î
	WORD								wTypeID;							//¿‡–Õ∫≈¬Î
	WORD								wKindID;							//√˚≥∆∫≈¬Î
	DWORD								dwMaxVersion;						//◊Ó–¬∞Ê±æ
	DWORD								dwOnLineCount;						//‘⁄œﬂ ˝ƒø
	char								szKindName[KIND_LEN];				//”Œœ∑√˚◊÷
	char								szProcessName[MODULE_LEN];			//Ω¯≥Ã√˚◊÷
};*/

//”Œœ∑’æµ„Ω·ππ
struct tagGameStation
{
	WORD								wSortID;							//≈≈–Ú∫≈¬Î
	WORD								wKindID;							//√˚≥∆∫≈¬Î
	WORD								wJoinID;							//π“Ω”∫≈¬Î
	WORD								wStationID;							//’æµ„∫≈¬Î
	char								szStationName[STATION_LEN];			//’æµ„√˚≥∆
};

//”Œœ∑∑øº‰¡–±ÌΩ·ππ
/*struct tagGameServer
{
	WORD								wSortID;							//排序号码
	WORD								wKindID;							//名称号码
	WORD								wServerID;							//房间号码
	WORD								wStationID;							//站点号码
	WORD								wServerPort;						//房间端口
	DWORD								dwServerAddr;						//房间地址
	DWORD								dwOnLineCount;						//在线人数
	DWORD								dwMaxUserCount;						//房间人数 2010.9.30 by 张华
	LONG								llimitScore;						// 进入积分 2010.9.30 by 张华
	DWORD								lMaxScore;							// 积分上限
	WORD								wPrice;								// 座位费
	WORD								wServerType;
	char								szServerName[SERVER_LEN];			// 房间名称
	LONG								lCellScore;							// 单元积分 赔率  2010.9.17 by 张华
};

//”Œœ∑º∂±Ω·ππ
struct tagLevelItem
{
	LONG								lLevelScore;						//º∂±ª˝∑÷
	TCHART								szLevelName[16];					//º∂±√Ë ˆ
};*/
//////////////////////////////////////////////////////////////////////////
//±»»¸◊¥Ã¨
enum EN_MatchStatus
{
	EN_NULL								= 0,								//√ª”–
	EN_LOOKON							= 1,								//π€ø¥
	EN_OUT								= 2,								//≥ˆæ÷
	EN_UNDETERMINED						= 3,								//¥˝∂®
	EN_RATED							= 4,								//∆’º∂	
	EN_MATCH							= 5,								//±»»¸
};
////用户状态定义<已经在Define.h中定义了>
//#define US_NULL						0x00								//没有状态
//#define US_FREE						0x01								//站立状态
//#define US_SIT						0x02								//坐下状态
//#define US_READY						0x03								//同意状态
//#define US_LOOKON						0x04								//旁观状态
//#define US_PLAY						0x05								//游戏状态
//#define US_OFFLINE					0x06								//断线状态
//#define US_AGAIN_GAME                 0x07                                //ºÃ–¯”Œœ∑ 2010.9.15 by gaoshan add
//#define US_LEFT                       0x08                                //«øÕÀ◊¥Ã¨ 2010.9.26 by gaoshan add

//≥§∂»∫Í∂®“Â
#define NAME_LEN						32									//√˚◊÷≥§∂»
#define PASS_LEN						33									//√‹¬Î≥§∂»
#define EMAIL_LEN						32									//” œ‰≥§∂»
#define GROUP_LEN						32									//…ÁÕ≈≥§∂»
#define COMPUTER_ID_LEN					33									//ª˙∆˜–Ú¡–
#define UNDER_WRITE_LEN					32									//∏ˆ–‘«©√˚
#define ADDRESS_LEN                     32                                  //ÕÊº“µÿ÷∑
#define ZIP_CODE_LEN					7									//” ’˛±‡¬Î 2010925 by’≈ª™
#define CITY_LEN						32									//≥« ––≈œ¢ 2010925 by’≈ª™
#define DESCRIBE_LEN					128									//≥« ––≈œ¢ 2010925 by’≈ª™
/////////////////////////////////////////////////Œ¨◊”£¨9.21
#define TABLE_NUMBER					100									//◊¿◊” ˝¡ø

#define US_QUEEN_NO						0x00								//∑«∂”¡–◊¥Ã¨
#define US_QUEEN_WAIT					0x01								//∂”¡–µ»¥˝÷–


//”√ªßª˝∑÷–≈œ¢
/*struct tagUserScore
{
	WORD								wSortID;							//≈≈√˚
	LONG								lScore;								//”√ªß∑÷ ˝
	LONG								lGoldEggs;							//”Œœ∑Ωµ∞
	LONG								lInsureScore;						//¥Ê¥¢Ω±“
	LONG								lWinCount;							// §¿˚≈Ã ˝
	LONG								lLostCount;							// ß∞‹≈Ã ˝
	LONG								lDrawCount;							//∫Õæ÷≈Ã ˝
	LONG								lFleeCount;							//∂œœﬂ ˝ƒø
	LONG								lExperience;						//”√ªßæ≠—È
	LONG                                lMatchScore;                        //±»»¸Ω±“
};*/
//”√ªßΩ±“–≈œ¢
struct tagUserGold
{
	LONG								lExperience;						//”√ªßæ≠—È
	LONG								lScore;								//”Œœ∑±“
	LONG								lGoldEggs;							//”√ªßΩµ∞			
	//LONG								lLottery;							//Ω±»Ø
	//LONG								lCredit;							//»Ÿ”˛µ„
	LONG								lStorageGold;						//¥Ê¥¢ª˝∑÷
};
//”√ªß◊¥Ã¨–≈œ¢
/*struct tagUserStatus
{
	WORD								wTeamID;							//–°◊È∫≈¬Î
	WORD								wTableID;							//◊¿◊”∫≈¬Î
	WORD								wChairID;							//“Œ◊”Œª÷√
	BYTE								cbUserStatus;						//”√ªß◊¥Ã¨
};*/

//用户基本信息结构
struct tagUserInfoHead
{
	//用户属性
	WORD								wFaceID;							//头像索引
	DWORD								dwUserID;							//用户 I D
	DWORD								dwGameID;							//游戏 I D
	DWORD								dwGroupID;							//社团索引
	DWORD								dwUserRight;						//用户等级
	LONG								lLoveliness;						//用户魅力
	DWORD								dwMasterRight;						//管理权限

	//用户属性
	BYTE								cbGender;							//用户性别
	BYTE								cbMemberOrder;						//会员等级
	BYTE								cbMasterOrder;						//管理等级

	//用户状态
	//WORD								wTeamID;							//–°◊È∫≈¬Î 2010.9.17 by gaoshan
	WORD								wTableID;							//桌子号码
	WORD								wChairID;							//椅子位置
	BYTE								cbUserStatus;						//用户状态

	//用户积分
	tagUserScore						UserScoreInfo;						//积分信息
	LONG								lWeekMingci;						//”√ªß±æ÷‹≈≈√˚ 20101022 by’≈ª™
	LONG								lWeekWinCount;						//”√ªß±æ÷‹æ‡…œ“ª√˚æ÷ ˝ 20101022 by’≈ª™						//”Œœ∑Ωµ∞//   2010-9-28 by ’≈ª™
	DWORD								dwCustomFaceVer;					//上传头像
	DWORD								dwPropResidualTime[15];             //道具时间

};

//本地用户信息结构，协议使用的是tagGameInUserInfo结构体<by hxh>
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
	char								szNickName[NAME_LEN];				//”√ªßÍ«≥∆
	char								szGroupName[GROUP_LEN];				//社团名字
	char								szUnderWrite[UNDER_WRITE_LEN];		//个性签名
	char                               szZipCode[ZIP_CODE_LEN];            //” ’˛±‡¬Î  2010-10-22 by ’≈ª™
	char                               szDwellingPlace[CITY_LEN];			//≥« ––≈œ¢  2010-10-22 by ’≈ª™
	DWORD								sdServerID;						//±ªÀ¯∂®∑˛ŒÒID 2010925 by’≈ª™
	DWORD								sdKindID;						//±ªÀ¯∂®”Œœ∑ID 2010925 by’≈ª™
	LONG								lWeekMingci;					//”√ªß±æ÷‹≈≈√˚ 20100926 by’≈ª™
	LONG								lWeekWinCount;					//”√ªß±æ÷‹æ‡…œ“ª√˚æ÷ ˝ 20100926 by’≈ª™

	//用户属性
	TGender								cbGender;							//用户性别
	BYTE								cbMemberOrder;						//会员等级
	BYTE								cbMasterOrder;						//管理等级

	//用户积分
	SCORE								lInsureScore;						//用户银行<by hxh 20160812>
	SCORE								lScore;								//用户分数<by hxh 20160812>
	LONG								lWinCount;							//胜利盘数
	LONG								lLostCount;							//失败盘数
	LONG								lDrawCount;							//和局盘数
	LONG								lFleeCount;							//断线数目
	LONG								lExperience;						//用户经验
	LONG								lGoldEggs;							//Ωµ∞
	LONG                                lMatchScore;                        //±»»¸”√ªßª˝∑÷2010.9.29 by gaoshan 
	
	//用户状态
	WORD								wSortID;							//≈≈√˚
	//WORD								wTeamID;							//–°◊È∫≈¬Î
	WORD								wTableID;							//桌子号码
	WORD								wChairID;							//椅子位置
	TUserState							cbUserStatus;						//用户状态

	//其他信息
	BYTE								cbCompanion;						//用户关系
	//DWORD								dwPropResidualTime[PROPERTY_COUNT];	//道具时间
};

// œµÕ≥œ˚œ¢
struct tagSystemMessage
{
	DWORD						dwMessageID;
	char						szAddresser[16];
	char						szTitle[32];
	char						szContent[128];
	char						szTime[24];
	WORD						wDistribute;
};
//”√ªßœµÕ≥œ˚œ¢
struct tagUserSystemMessage
{
	DWORD							dwUserID;
	DWORD							dwMessageID;
	WORD							wDes;
};

//********ÕÊº“◊‘º∫µƒ–≈œ¢ 2010.9.8 by gaoshan 
struct tagMyInfo
{
	char                               szUserName[NAME_LEN] ;            //ÕÊº“µƒ√˚◊÷
	LONG                                lUserScore;                       //ÕÊº“µƒª˝∑÷
	DWORD                                wUserGold;                        //ÕÊº“Ωµ∞
    char                               szUserAddr[ADDRESS_LEN];          //ÕÊº“µÿ«¯
    BYTE                                cbUserLev;                       //ÕÊº“µ»º∂
	BYTE                                cbMemberOrder;                   //ª·‘±µ»º∂
	char                               szUserLev[NAME_LEN] ;            //ÕÊº“µ»º∂
};

//********ÕÊº“»ÀŒÔ Ù–‘–≈œ¢ 2010.9.8 by gaoshan
//Ω·ππÃÂ∂®“Â
struct tagUserAttribute
{
	WORD                           wChairID;                             //ÕÊº““Œ◊”∫≈
	char                          pszUserName[64];                      //ÕÊº“Í«≥∆
	char                          pszUserAddr[64];                      //ÕÊº“«¯”Ú
	LONG                           lUserScore;                           //ÕÊº“ª˝∑÷
	BYTE                           cbUserLev;                            //”√ªßµ»º∂
    BYTE                           cbMemberOrder;                           //ª·‘±µ»º∂
};

///////////±»»¸ÕÊº“–≈œ¢2010.10.4 by gaoshan add
struct tagMatchUserInfo
{
	DWORD             dwUserID;                         //”√ªßID
	LONG              lMatchScore;                      //”√ªßª˝∑÷
	char             szUserName[64];                   //”√ªßÍ«≥∆
};

///////////±»»¸ÕÊº“–≈œ¢2010.10.4 by gaoshan add

//////////////////////////////////////////////////////////////////////////

//ª˙∆˜–Ú¡–∫≈Ω·ππ
struct tagClientSerial
{
	//DWORD								dwSystemVer;						//œµÕ≥∞Ê±æ
	char								szComputerID[32];					//ª˙∆˜–Ú¡–
};

//≈‰÷√ª∫≥ÂΩ·ππ
struct tagOptionBuffer
{
	BYTE								cbBufferLen;						// ˝æ›≥§∂»
	BYTE								cbOptionBuf[32];					//…Ë÷√ª∫≥Â
};

//π⁄æ¸ ˝æ›
struct CMD_CS_Champion
{
	WORD								wMatchType;						//±»»¸¿‡–Õ
	WORD								wKindID;						//√˚≥∆∫≈¬Î
	WORD								wServerID;						//∑øº‰∫≈¬Î
	WORD								wMatchMen;						//±»»¸»À ˝
	char								szMatchTime[24];				//±»»¸ ±º‰
	char								szAccounts[NAME_LEN];			//”√ªß√˚≥∆
};

///////////////////////////////////////ªÊª≠»ÀŒÔÕ∑œÒÀ˘–Ë∂®“Â2010.10.9 by gaohsan //////////////////////////
//Õº∆¨–ßπ˚¿‡–Õ
enum enFaceImgType
{
	enFaceImg_UnPlane     =0,           //Õ∏ ”¿‡–Õ
	enFaceImg_Plane       =1            //∆Ω√Ê¿‡–Õ
};
//»ÀŒÔÕ∑œÒ±Ì«È¿‡–Õ
enum enUserExpressType
{
	enUserExpress_Wait   =0,          //µ»¥˝ £®”Œœ∑ø™ º«∞ªÊª≠£©
	enUserExpress_Smile  =1,          //Œ¢–¶  £®”Œœ∑÷–ªÊª≠£©
	enUserExpress_Angry  =2,           //…˙∆¯   £®”Œœ∑Ω· ¯∫ÛªÊª≠£¨ ß∞‹ ±ªÊª≠…˙∆¯±Ì«È£¨≥…π¶ ±ªÊª≠Œ¢–¶±Ì«È£©
	enUserExpress_Gray   =3,           //ª“…´±Ì«È£®”√ªß∂œœﬂ÷–£¨Œﬁ∂Øª≠–ßπ˚£©
    enUserExpress_Null   =4            //Œﬁ±Ì«È
	
};

//»ÀŒÔÕ∑œÒŒª÷√¿‡–Õ
enum enUserFacePosType
{   
	enUserFacePos_Right   =0,         //”“±ﬂÕÊº“
	enUserFacePos_Left    =1,         //◊Û±ﬂÕÊº“
	enUserFacePos_Front   =2,         //«∞∑ΩÕÊº“
	enUserFacePos_MySelf  =3,         //◊‘º∫∑Ω 2010.11.22 by gaoshan
	enUserFacePos_Null    =4,         //ø’Œª÷√ÕÊº“
};

//ÕºœÒ◊Ó¥Û÷÷¿‡
#define MAX_FACE_NUM                    16                             //ÕºœÒ÷÷¿‡  2011.2.28  by gaoshan 

///////////////////////////////////////ªÊª≠»ÀŒÔÕ∑œÒÀ˘–Ë∂®“Â2010.10.9 by gaohsan //////////////////////////
//////////////////////////////////////////////////////////////////////////

//加密密钥
const DWORD g_dwPacketKey=0xA55AA55A;

//发送映射
const TCHART g_SendByteMap[256]=
{
	0x70,0x2F,0x40,0x5F,0x44,0x8E,0x6E,0x45,0x7E,0xAB,0x2C,0x1F,0xB4,0xAC,0x9D,0x91,
	0x0D,0x36,0x9B,0x0B,0xD4,0xC4,0x39,0x74,0xBF,0x23,0x16,0x14,0x06,0xEB,0x04,0x3E,
	0x12,0x5C,0x8B,0xBC,0x61,0x63,0xF6,0xA5,0xE1,0x65,0xD8,0xF5,0x5A,0x07,0xF0,0x13,
	0xF2,0x20,0x6B,0x4A,0x24,0x59,0x89,0x64,0xD7,0x42,0x6A,0x5E,0x3D,0x0A,0x77,0xE0,
	0x80,0x27,0xB8,0xC5,0x8C,0x0E,0xFA,0x8A,0xD5,0x29,0x56,0x57,0x6C,0x53,0x67,0x41,
	0xE8,0x00,0x1A,0xCE,0x86,0x83,0xB0,0x22,0x28,0x4D,0x3F,0x26,0x46,0x4F,0x6F,0x2B,
	0x72,0x3A,0xF1,0x8D,0x97,0x95,0x49,0x84,0xE5,0xE3,0x79,0x8F,0x51,0x10,0xA8,0x82,
	0xC6,0xDD,0xFF,0xFC,0xE4,0xCF,0xB3,0x09,0x5D,0xEA,0x9C,0x34,0xF9,0x17,0x9F,0xDA,
	0x87,0xF8,0x15,0x05,0x3C,0xD3,0xA4,0x85,0x2E,0xFB,0xEE,0x47,0x3B,0xEF,0x37,0x7F,
	0x93,0xAF,0x69,0x0C,0x71,0x31,0xDE,0x21,0x75,0xA0,0xAA,0xBA,0x7C,0x38,0x02,0xB7,
	0x81,0x01,0xFD,0xE7,0x1D,0xCC,0xCD,0xBD,0x1B,0x7A,0x2A,0xAD,0x66,0xBE,0x55,0x33,
	0x03,0xDB,0x88,0xB2,0x1E,0x4E,0xB9,0xE6,0xC2,0xF7,0xCB,0x7D,0xC9,0x62,0xC3,0xA6,
	0xDC,0xA7,0x50,0xB5,0x4B,0x94,0xC0,0x92,0x4C,0x11,0x5B,0x78,0xD9,0xB1,0xED,0x19,
	0xE9,0xA1,0x1C,0xB6,0x32,0x99,0xA3,0x76,0x9E,0x7B,0x6D,0x9A,0x30,0xD6,0xA9,0x25,
	0xC7,0xAE,0x96,0x35,0xD0,0xBB,0xD2,0xC8,0xA2,0x08,0xF3,0xD1,0x73,0xF4,0x48,0x2D,
	0x90,0xCA,0xE2,0x58,0xC1,0x18,0x52,0xFE,0xDF,0x68,0x98,0x54,0xEC,0x60,0x43,0x0F
};

//接收映射
const TCHART g_RecvByteMap[256]=
{
	0x51,0xA1,0x9E,0xB0,0x1E,0x83,0x1C,0x2D,0xE9,0x77,0x3D,0x13,0x93,0x10,0x45,0xFF,
	0x6D,0xC9,0x20,0x2F,0x1B,0x82,0x1A,0x7D,0xF5,0xCF,0x52,0xA8,0xD2,0xA4,0xB4,0x0B,
	0x31,0x97,0x57,0x19,0x34,0xDF,0x5B,0x41,0x58,0x49,0xAA,0x5F,0x0A,0xEF,0x88,0x01,
	0xDC,0x95,0xD4,0xAF,0x7B,0xE3,0x11,0x8E,0x9D,0x16,0x61,0x8C,0x84,0x3C,0x1F,0x5A,
	0x02,0x4F,0x39,0xFE,0x04,0x07,0x5C,0x8B,0xEE,0x66,0x33,0xC4,0xC8,0x59,0xB5,0x5D,
	0xC2,0x6C,0xF6,0x4D,0xFB,0xAE,0x4A,0x4B,0xF3,0x35,0x2C,0xCA,0x21,0x78,0x3B,0x03,
	0xFD,0x24,0xBD,0x25,0x37,0x29,0xAC,0x4E,0xF9,0x92,0x3A,0x32,0x4C,0xDA,0x06,0x5E,
	0x00,0x94,0x60,0xEC,0x17,0x98,0xD7,0x3E,0xCB,0x6A,0xA9,0xD9,0x9C,0xBB,0x08,0x8F,
	0x40,0xA0,0x6F,0x55,0x67,0x87,0x54,0x80,0xB2,0x36,0x47,0x22,0x44,0x63,0x05,0x6B,
	0xF0,0x0F,0xC7,0x90,0xC5,0x65,0xE2,0x64,0xFA,0xD5,0xDB,0x12,0x7A,0x0E,0xD8,0x7E,
	0x99,0xD1,0xE8,0xD6,0x86,0x27,0xBF,0xC1,0x6E,0xDE,0x9A,0x09,0x0D,0xAB,0xE1,0x91,
	0x56,0xCD,0xB3,0x76,0x0C,0xC3,0xD3,0x9F,0x42,0xB6,0x9B,0xE5,0x23,0xA7,0xAD,0x18,
	0xC6,0xF4,0xB8,0xBE,0x15,0x43,0x70,0xE0,0xE7,0xBC,0xF1,0xBA,0xA5,0xA6,0x53,0x75,
	0xE4,0xEB,0xE6,0x85,0x14,0x48,0xDD,0x38,0x2A,0xCC,0x7F,0xB1,0xC0,0x71,0x96,0xF8,
	0x3F,0x28,0xF2,0x69,0x74,0x68,0xB7,0xA3,0x50,0xD0,0x79,0x1D,0xFC,0xCE,0x8A,0x8D,
	0x2E,0x62,0x30,0xEA,0xED,0x2B,0x26,0xB9,0x81,0x7C,0x46,0x89,0x73,0xA2,0xF7,0x72
};

//////////////////////////////////////////////////////////////////////////
struct tagBuffer : public GBEventArg
{
    BYTE dateBuffer[81920];
    unsigned int nDataSize;
    tagBuffer()
    {
        memset(dateBuffer, '\0', 0);
        nDataSize = 0;
    }
};

struct tagCmdBuffer : public GBEventArg
{
    CMD_Command command;
    BYTE dateBuffer[40960];
    unsigned int nDataSize;
};

struct tagUserBuffer : public cocos2d::Object
{
    unsigned int dwUserID;
};

struct tagGameBuffer : public cocos2d::Object
{
    WORD wSubCmdID;
    BYTE dateBuffer[4096];
    unsigned int nDataSize;
};

struct tagSceneBuffer : public cocos2d::Object
{
    unsigned char cbState;
    unsigned int nDataSize;
    BYTE dateBuffer[4096];
};


struct tagBuffer1 : public GBEventArg
{
    BYTE dateBuffer[4096];
    unsigned int nDataSize;
};

//注册buffer
struct tagRegBuffer : public cocos2d::Object
{
	LONG            lErrorCode;
	unsigned char	szErrorDescribe[128];	
};

//系统buffer
struct tagSystemBuffer : public GBEventArg
{
	WORD			wMessageType;					//消息类型
	WORD			wMessageLength;					//消息长度
	char			szContent[1024];				//消息内容
};

//找回密码
struct tagGetPassObj : public cocos2d::Object
{
    long 			lErrCode;                       //消息标志 0成功
	char			szPassword[33];                 //密码
	unsigned char	szErrorDescribe[128];           //描述信息
};

//赠送金币
struct tagScoreGifObj : public cocos2d::Object
{
    long 			lErrCode;                       //消息标志 0成功
	long			lScore;                         //赠送金币
	unsigned char	szErrorDescribe[128];           //描述信息
};

//领取金币 话费点
struct tagChargeObj  : public cocos2d::Object
{
	long			lErrCode;                       //
	long			lScore;                         //
	long			lGoldeggs;						//
    unsigned char	szErrorDescribe[128];			//
};

//房间登录失败<by hxh>
struct tagRoomLogonFailObj  : public cocos2d::Object
{
	int							lErrorCode;					// 错误代码
	TCHART						szDescribeString[128];		// 错误描述
};

//请求失败<by hxh>
struct tagRequestFailureObj  : public cocos2d::Object
{
	BYTE							cbFailureCode;						//错误代码
	BYTE                            cbCommandCode;                      //命令代码
	TCHART							szDescribeString[256];				// 描述信息
};

//破产 降级
struct tagBankruptObj  : public cocos2d::Object
{
    bool bBankrupt;    //true 破产,flase 领取金币
};

//获取用户列表
struct tagUserListObj  : public cocos2d::Object
{
    long				lErrCode;
    unsigned char		szUserNameList[270];				//用户名列表，最多２０个
};

//获取用户
struct tagUserObj  : public cocos2d::Object
{
    unsigned char       szUserName[32];                  //用户名
};

//配置信息
struct tagConfigObj  : public cocos2d::Object
{
    char		szGame[128];                                //游戏公告
	unsigned int        nHighVerID;							//高版本
	unsigned int        nLowVerID;						    //低版本
	bool                bForcedUpdate;                     //是否强制更新
};

//表情消息
struct tagExpressObj  : public cocos2d::Object
{
    unsigned int        type;       //1表情 2语音
    unsigned int        nId;        //id
};

//更新消息
struct tagUpdateObj  : public cocos2d::Object
{
    bool bUpdate;     //更新消息
};

struct tagInviteInfo : public cocos2d::Object
{
    WORD wTableID;
    unsigned char       szUserName[32];//用户名
};

//struct CMD_GP_IOSConfigResult
//{
//    
//	LONG								lGameKindid;						//游戏ID
//	char								szGameGg[128];						//游戏公告
//	LONG                                lHighVerID;							//高版本
//	LONG                                lLowVerID;						    //低版本
//	LONG                                isForcedUpdate;                     //是否强制更新
//};

////获取密码,获得用户列表结果
//struct CMD_GP_GetAccountListResult
//{
//	LONG								lErrCode;
//	char								szUserNameList[270];				//用户名列表，最多２０个
//};

//struct CMD_GP_ScoreGiftResult
//{
//	LONG								lErrCode;							//œ˚œ¢±Í÷æ0≥…π¶  1 ß∞‹
//	LONG								lScore;								//”√ªß∑÷ ˝
//	char								szErrorDescribe[128];				//√Ë ˆ–≈œ¢
//};

//获取密码结果
//            struct CMD_GP_GetPasswordResult
//            {
//                LONG			lErrCode;	//消息标志 0成功
//                char			szPassword[33];	//密码
//                char			szErrorDescribe[128];	//描述信息
//            };

//struct CMD_GP_UserChargeResult
//{
//	LONG								lErrCode;							//œ˚œ¢±Í÷æ 1≥…π¶  0 ß∞‹
//	LONG								lScore;								//”√ªß∑÷ ˝
//	LONG								lGoldeggs;								//”√ªßª∞∑—
//	char								szErrorDescribe[128];					//√Ë ˆ–≈œ¢
//};

#endif









