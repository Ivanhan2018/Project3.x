#ifndef DEFINE_DEFINE_FILE
#define DEFINE_DEFINE_FILE
#pragma once

#include "cocos2d.h"
#include <string>
using namespace std;
using namespace cocos2d;

#define WINSIZE Director::getInstance()->getWinSize()
#define VISIBLESIZE Director::getInstance()->getVisibleSize()

//定义一个标清屏
#define     SCREEN_WIDTH    720
#define     SCREEN_HEIGHT   1280

#define     WINDOW_WIDTH    540
#define     WINDOW_HEIGHT   960

#define     FRAME_WIDTH    480
#define     FRAME_HEIGHT   800

#define VER_369

const int GameId_DDZ		= 1;
const int GameId_PHZ		= 2;
const int GameId_CSMJ		= 3;
const int GameId_ZJH		= 4;
const int GameId_SDK		= 5;
const int GameId_PDK		= 6;
const int KindId_BJL	    = 122;
const int GameId_dzpk		= 308;

const int SeverID_CSMJ      = 0;
const int SeverID_PDK       = 1;
const int SeverID_PHZ       = 2;
const int SeverID_SDH       = 3;

//游戏编号
typedef enum
{
    kind_default = -1,               //默认类型
    kind_niuniu = 101,               //牛牛类型
    kind_baijiale = 122,            //百家乐类型
    kind_lkpy = 510,                //李逵劈鱼
    kind_brnn = 104,                //百人牛牛
    kind_zjh = 106,                   //诈金花
    kind_tbnn = 136,                 //通比牛牛
    kind_lhdb=219
}LIST_Kind;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
typedef wchar_t		TCHART;
//typedef unsigned char		BYTE;
//typedef unsigned short		WORD;
//typedef unsigned int		DWORD;
//typedef long long			LONGLONG;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
typedef unsigned short				TCHART;
typedef unsigned short	TCHART;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
typedef unsigned short		TCHART;
#endif

#ifdef WIN32
#else
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned int		DWORD;
typedef	long long			LONGLONG;
typedef int                LONG;
//typedef unsigned short		TCHART;
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
//typedef wchar_t		TCHART;
#endif

typedef double			SCORE;
typedef double          DOUBLE;

#ifdef WIN32
#define STRCMP(x,y)	_stricmp(x,y)
#define ATOI64(x) _atoi64(x)
#define SNPRINTF(charArray,format,...) _snprintf(charArray,sizeof(charArray)-1,format,__VA_ARGS__)
#else
#define STRCMP(x,y)	strcasecmp(x,y)
#define ATOI64(x) atoll(x)
#define SNPRINTF(charArray,format,...) snprintf(charArray,sizeof(charArray),format,##__VA_ARGS__)
#endif

//兼容PC函数
#define ASSERT(e)                       assert(e)
#define CopyMemory(des,src,size_t)      memcpy(des,src,size_t)
#define ZeroMemory(des,size)            memset(des,0,size)
#define VERIFY(e)                       assert(e)

/////////////////////////////////////////////////////////////////////
//端口定义
#define MAX_CONTENT			512							//并发容量
#define PORT_AUTO_SELECT	((WORD)(0XFFFF))			//自动端口

//测试
#define IP_LOGON			"192.168.0.88"
#define IP_LOGON2			"192.168.0.88"
#define IP_LOGON3			"192.168.0.88"
#define IP_LOGON4			"192.168.0.88"

#define PORT_LOGON			8500							//登陆端口
#define PORT_CENTER			8310						//协调端口
#define PORT_8311			8311						//测试端口
#define PORT_MANAGER		8320						//管理端口
#define DEFAULT_ACCOUNT		"test009"
#define DEFAULT_PASSWORD	"123456"
/////////////////////////////////////////////////////////////////////

//#pragma mark 数值定义
// 头像大小
#define	FACE_CX				48						// 头像宽度
#define FACE_CY				48						// 头像高度
#define IPADSIZE            1024.0f

// 长度定义
#define	LEN_LESS_ACCOUNTS	3						// 最短帐号
#define LEN_LESS_NICKNAME	3						// 最短昵称
#define LED_LESSPASSWORD	6						// 最短密码

// 人数定义
#define MAX_CHAIR			100						// 最大椅子
#define	MAX_TABLE			512						// 最大桌子
#define	MAX_COLUMN			32						// 最大列表
#define	MAX_ANDROID			256						// 最大机器
#define	MAX_PROPORTY		128						// 最大道具
#define	MAX_WHISPER_USER	16						// 最大私聊
#define MAX_CHAIR_NORMAL	8						// 最大人数
#define MAX_CHAT_LEN		128						// 聊天长度
#define LIMIT_CHAT_TIMES	1200					// 限时聊天
#define MAX_TABLES          200

// 裂表定义
#define MAX_KIND			128						// 最大类型
#define MAX_SERVER			1024					// 最大房间

// 参数定义
//#define	INVALID_CHAIR		0xFFFF					// 无效椅子
//#define	INVALID_TABLE		0xFFFF					// 无效桌子
//#define INVALID_USERID		0						// 无效用户
//无效数值
#define INVALID_BYTE				((BYTE)(0xFF))						//无效数值
#define INVALID_WORD				((WORD)(0xFFFF))					//无效数值
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//无效数值

//无效数值
#define INVALID_TEAM				INVALID_WORD						//无效组号
#define INVALID_TABLE				INVALID_WORD						//无效桌子
#define INVALID_CHAIR				INVALID_WORD
#define INVALID_ID                  INVALID_DWORD

// 税收起点
#define	REVENUE_BENCHMARK	1000L					// 税收起点
#define	REVENUE_DENOMINATOR	1000L					// 税收分母

//#pragma mark 系统参数
// 游戏状态
#define	GAME_STATUS_FREE	0						// 空闲状态
#define	GAME_STATUS_PLAY	100						// 游戏状态
#define GAME_STATUS_WAIT	200						// 等待状态

// 系统参数
#define LEN_USER_CHAT		128						// 聊天长度
#define	TIME_USER_CHAT		1L						// 聊天间隔
#define TRUMPET_MAX_CHAR    128                     // 喇叭长度
	

//#pragma mark 索引质数
// 列表质数
#define PRIME_TYPE			11L						// 种类数目
#define PRIME_KIND			53L						// 类型数目
#define PRIME_NODE			101L					// 节点数目
#define	PRIME_PAGE			53L						// 自定树木
#define PRIME_SERVER		1009L					// 房间树木

// 人物质数
#define PRIME_SERVER_USER	503L					// 房间人数
#define PRIME_ANDROID_USER	503L					// 机器人数
#define PRIME_PLATFORM_USER	100003L					// 平台人数


//#pragma mark 数据长度
// 资料数据
#define LEN_MD5				33						// 加密密码
#define LEN_ACCOUNTS		32						// 备注长度
#define LEN_NICENAME		32						// 帐号长度
#define LEN_NICKNAME		32						// 昵称长度
#define LEN_PASSWORD		33						// 密码长度
#define LEN_GROUP_NAME		32						// 社团名字
#define LEN_UNDER_WRITE		32						// 个性签名
#define LEN_PASS_PORT_ID	18

// 数据长度
#define LEN_QQ				16						// q q号码
#define LEN_EMAIL			33						// 电子邮件
#define LEN_USER_NOTE		256						// 用户备注
#define LEN_SEAT_PHONE		33						// 固定电话
#define LEN_MOBILE_PHONE	12						// 移动电话
#define Len_PASS_PORT_ID	19						// 证件号码
#define LEN_COMPELLATION	16						// 真实名字
#define LEN_DWELLING_PLACE	128						// 联系地址
/////////////////////////////////////////////////
#define NAME_LEN						32
#define PASS_LEN						33
#define EMAIL_LEN						32
#define GROUP_LEN						32
#define COMPUTER_ID_LEN					33
#define UNDER_WRITE_LEN					32
#define ADDRESS_LEN                     32
#define ZIP_CODE_LEN					7
#define CITY_LEN						32
#define DESCRIBE_LEN					128

#define TABLE_NUMBER					100

#define US_QUEEN_NO						0x00
#define US_QUEEN_WAIT					0x01

#define TYPE_LEN						32
#define KIND_LEN						32
#define STATION_LEN						32
#define SERVER_LEN						32
#define MODULE_LEN						32
#define PASS_LEN						33

#define GENDER_NULL						0
#define GENDER_BOY						1
#define GENDER_GIRL						2

#define GAME_GENRE_SCORE				0x0001
#define GAME_GENRE_GOLD					0x0002
#define GAME_GENRE_MATCH				0x0004
#define GAME_GENRE_EDUCATE				0x0008    
/////////////////////////////////////////////////

// 机器标识
#define LEN_MACHINE_ID		33						// 序列长度
#define LEN_NETWORK_ID		13						// 网卡长度

// 列表数据
#define LEN_TYPE			32						// 种类长度
#define LEN_KIND			32						// 类型长度
#define LEN_NODE			32						// 节点长度
#define LEN_PAGE			32						// 定制长度
#define LEN_SERVER			32						// 房间长度
#define LEN_PROCESS			32						// 进程长度


//#pragma mark 好友关系
#define CP_NORMAL			0						// 未知关系
#define CP_ERIEND			1						// 好友关系
#define CP_DETEST			2						// 厌恶关系

//#pragma mark 性别定义
#define GENDER_FRMALE		0						// 女性性别
#define	GENDER_MANKIND		1						// 男性性别

//#pragma mark 游戏模式
//#define GAME_GENRE_GOLD		0x0001					// 豆豆类型
//#define GAME_GENRE_SCORE	0x0002					// 点值类型
//#define GAME_GENRE_EDUCATE	0x0003					// 比赛类型
//#define GAME_GENREMATCH		0x0004					// 训练类型

//#pragma mark 分数模式
#define	SCORE_GENRE_NORMAL		0x0100				// 普通模式
#define SCORE_GENRE_POSITIVE	0x0200				// 非负模式

//#pragma mark 用户状态
#define	US_NULL				0x00					// 没有状态
#define US_FREE				0x01					// 站立状态
#define US_SIT				0x02					// 坐下状态
#define US_READY			0x03					// 同意状态
#define US_LOOKON			0x04					// 旁观状态
#define US_PLAYING			0x05					// 游戏状态
#define US_OFFLINE			0x06					// 断线状态
#define US_PLAY							0x05
#define US_AGAIN_GAME                   0x07
#define US_LEFT                         0x08
//#pragma mark 房间规则
#define	SRL_LOOKON			0x00000001				// 旁观标志
#define SRL_OFFLINE			0x00000002				// 断线标志
#define	SRL_SAME_IP			0x00000004				// 同网标志

#define	SRL_ROOM_CHAT		0x00000100				// 聊天标志
#define SRL_GAME_CHAT		0x00000200				// 聊天标志
#define SRL_WISPER_CHAT		0x00000400				// 私聊标志
#define SRL_HIDE_USER_INFO	0x00000800				// 隐藏标志

//#pragma mark 列表数据
//无效属性
#define	UD_NULL				0						// 无效子项
#define UD_IMAGE			100						// 图形子项

//基本属性
#define UD_GAME_ID			1						// 游戏标识
#define UD_USER_ID			2						// 用户标识
#define UD_NICKNAME			3						// 用户昵称

//扩展属性
#define	UD_GENDER			10						// 用户性别
#define UD_GROUP_NAME		11						// 社团名字
#define	US_UNDER_WRITE		12						// 个性签名

//状态信息
#define	UD_TABLE			20						// 游戏桌号
#define UD_CHAIR			21						// 椅子号码

//积分信息
#define	UD_SCORE			30						// 用户分数
#define UD_BANKER			31						// 用户保险柜
#define UD_EXPERIENCE		32						// 用户经验
#define UD_LOVELINESS		33						// 用户魅力
#define	UD_WIN_COUNT		34						// 胜利盘数
#define UD_LOSE_COUNT		35						// 输局盘数
#define	UD_DRAW_COUNT		36						// 和局盘数
#define UD_FLEE_COUNT		37						// 逃局盘数
#define UD_PLAY_COUNT		38						// 总局盘数

//积分比率
#define	UD_WIN_RATE			40						// 用户胜率
#define	UD_LOSE_RATE		41						// 用户输率
#define UD_DRAW_RATE		42						// 用户和率
#define	UD_FLEE_RATE		43						// 用户逃率
#define UD_GAME_LEVEL		44						// 游戏等级

//扩展信息
#define	UD_NOTE_INFO		50						// 用户备注
#define	UD_LOOKON_USER		51						// 旁观用户

//图像列表
#define	UD_FLAG				(UD_IMAGE+1)			// 用户标志
#define UD_STATUS			(UD_IMAGE+2)			// 用户状态

//#pragma mark 数据库定义
#define	DB_ERROR			-1						// 处理失败
#define	DB_SUCCESS			0						// 处理成功

//#定义默认颜色
#define	COLOR_BK_TITLE			ccc3(76,76,76)
#define	COLOR_BK_RED			ccc3(161,47,47)
#define	COLOR_BK_GRAY			ccc3(150,150,150)


//Macro.h
///////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
//#define	INVALID_BYTE		((BYTE)(0xff))			// 无效数值
//#define INVALID_WORD		((WORD)(0xffff))		// 无效数值
//#define INVALID_DWORD		((DWORD)(0xffffffff))	// 无效数值

//数组维数
#define CountArray(Array)	(sizeof(Array)/sizeof(Array[0]))



//无效地址
#define INVALID_IP_ADDRESS(IPAddress)	(((IPAddress == 0L) || (IPAddress == INADDR_NONE)))


// 用户注册配置字符
#define strAccounts         "m_pAccounts"          // 帐号
#define strPassword         "m_pPassword"          // 密码
#define strNickname         "m_pNickname"          // 昵称
#define strBankword         "m_pBankWord"          // 保险柜密码

#define strQuickRegCount    "m_pQuickRegCount"     // 快速注册的次数



#define IMAGE_BG_480        "game_bg-hd.png"
#define IMAGE_BG_568        "game_bg-hd568.png"

#define ROOM_BG_480         "common_bg.png"
#define ROOM_BG_568         "common_bg.png"

#define WEB_BG_480          "common_bg_2.png"
#define WEB_BG_568          "common_bg_2.png"

// 游戏房间豆豆定义
#define ROOM_MAX_PK               (500000)  // 50万以上为pk房
#define ROOM_MAX_NORMAL           (100000)  // 两者中间为普通房
#define ROOM_MAX_PRIMARY          (10000)   // 1万以下为初级房

#define ROOM_MENU_POSX          (15)
#define ROOM_MENU_POSY          (85.0-13)      // 92
#define ROOM_MENU_W             (450.0)      // 92
#define ROOM_MENU_H             (86.0/2)      // 92


#define SPACE_LEFT_W            ( 112.0/2)
#define SPACE_RIGHT_W           ( 113.0/2)


// 分享和评分赠送游戏币类型
typedef enum
{
	type_appStoreLove = 1,      // appstore评分
	type_GoodFriends,           // 微信好友
	type_FriendsCircle,         // 微信朋友圏
	type_txWeb,                 // 腾讯微博
	type_sineWeb,               // 新浪微博
	
} ShareType;

// 支付商品名称
#define PAY_NAME_6            "6万哈皮豆"
#define PAY_NAME_12           "13.2万哈皮豆"
#define PAY_NAME_50           "60万哈皮豆"
#define PAY_NAME_98           "117.6万哈皮豆"
#define PAY_NAME_328          "475.6万哈皮豆"
#define PAY_NAME_648          "1069万哈皮豆"


#define PAY_NAME_EN6          "6wan hapidou"
#define PAY_NAME_EN12         "13.2wan hapidou"
#define PAY_NAME_EN50         "60wan hapidou"
#define PAY_NAME_EN98         "117.6wan hapidou"
#define PAY_NAME_EN328        "475.6wan hapidou"
#define PAY_NAME_EN648        "1069wan hapidou"

#define ERROR_NEED_BUY			(0x203)						//515 低于游戏房间，需要购买
#endif