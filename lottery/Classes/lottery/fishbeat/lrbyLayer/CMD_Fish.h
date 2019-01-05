#ifndef CMD_FISH_H_
#define CMD_FISH_H_

#include "Define.h"
#include "cocos2d.h"
//////////////////////////////////////////////////////////////////////////
#pragma pack(push,1)
// 服务定义
namespace NS_lrbyFish
{
const WORD BY_KIND_ID					= 715;
const WORD BY_GAME_PLAYER				= 8;				//游戏总人数
const DWORD  BY_VERSION_SERVER			= 101056515;//100728832;       // PROCESS_VERSION(1,0,0)
const DWORD BY_VERSION_CLIENT			= 101056515;//100728832;      // PROCESS_VERSION(1,0,0)

// 简单的版本检测
// 如果客户端有更新应该更改这个值.如果只修改EXE的版本而网络数据包没修改的话，更新客户端是没什么意义的，因为EXE的版本是可以直接被修改的
// 只要保存以前的EXE版本,更新完之后再换回去，如果有BUG的话等于还是没修复。
const WORD  GAME_VERSION			=30;

const int kResolutionWidth			= 1280;
const int kResolutionHeight			= 720;

typedef LONGLONG FISHSCORE;

// add by hxh 20161227
//#define USE_TRACETIME

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#define M_PI_2  1.57079632679489661923f
#define M_PI_4  0.785398163397448309616f
#define M_1_PI  0.318309886183790671538f
#define M_2_PI  0.636619772367581343076f
#endif
const float kChairDefaultAngle[BY_GAME_PLAYER] = { M_PI, M_PI,M_PI, -M_PI_2, 0, 0, 0, M_PI_2};
const float kChairDefaultAngleOther[BY_GAME_PLAYER] = {180.0f,180.0f,180.0f,-90.0f,0.0f,0.0f,0.0f,90.0f};


//const float kChairDefaultAngle[BY_GAME_PLAYER] = {M_PI ,0};

enum TraceType {
  TRACE_LINEAR = 0,
  TRACE_BEZIER
};

struct FPoint {
  float x;
  float y;
};

struct FPointAngle {
  float x;
  float y;
  float angle;
};

//////////////////////////////////////////////////////////////////////////
// 游戏定义

/*
// 座位号
-------------
    0   1   2 
7               3                
    6   5   4
-------------
*/

enum SceneKind {
  SCENE_KIND_1 = 0,
  SCENE_KIND_2,
  SCENE_KIND_3,
  SCENE_KIND_4,
  SCENE_KIND_5,

  SCENE_KIND_COUNT
};

enum FishKind {
  FISH_KIND_1 = 0,
  FISH_KIND_2,
  FISH_KIND_3,
  FISH_KIND_4,
  FISH_KIND_5,
  FISH_KIND_6,
  FISH_KIND_7,
  FISH_KIND_8,
  FISH_KIND_9,
  FISH_KIND_10,
  FISH_KIND_11,
  FISH_KIND_12,
  FISH_KIND_13,
  FISH_KIND_14,
  FISH_KIND_15,
  FISH_KIND_16,
  FISH_KIND_17,
  FISH_KIND_18,
  FISH_KIND_19,
  FISH_KIND_20, // 企鹅
  FISH_KIND_21, // 李逵
  FISH_KIND_23, // 局部炸弹
  FISH_KIND_22, // 定屏炸弹
  FISH_KIND_24, // 超级炸弹 
  FISH_KIND_25, // 大三元1
  FISH_KIND_26, // 大三元2
  FISH_KIND_27, // 大三元3
  FISH_KIND_28, // 大四喜1
  FISH_KIND_29, // 大四喜2
  FISH_KIND_30, // 大四喜3
  FISH_KIND_31, // 鱼王1
  FISH_KIND_32, // 鱼王2
  FISH_KIND_33, // 鱼王3
  FISH_KIND_34, // 鱼王4
  FISH_KIND_35, // 鱼王5
  FISH_KIND_36, // 鱼王6
  FISH_KIND_37, // 鱼王7
  FISH_KIND_38, // 鱼王8
  FISH_KIND_39, // 鱼王9
  FISH_KIND_40, // 鱼王10
 // FISH_KIND_41, // 奖券鱼

  FISH_KIND_COUNT
};

enum BulletKind {
  BULLET_KIND_1_NORMAL = 0,
  BULLET_KIND_2_NORMAL,
  BULLET_KIND_3_NORMAL,
  BULLET_KIND_4_NORMAL,
  BULLET_KIND_1_ION,
  BULLET_KIND_2_ION,
  BULLET_KIND_3_ION,
  BULLET_KIND_4_ION,

  BULLET_KIND_COUNT
};

const DWORD kBulletIonTime						= 20;
const DWORD kLockTime							= 10;

const int kMaxCatchFishCount					= 2;

// 限制客户端的鱼种类数量
//const int kFishKindCount[FISH_KIND_COUNT] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

//////////////////////////////////////////////////////////////////////////
// 服务端命令

const WORD SUB_S_GAME_CONFIG					= 100;
const WORD SUB_S_FISH_TRACE						= 101;
const WORD SUB_S_EXCHANGE_FISHSCORE				= 102;
const WORD SUB_S_USER_FIRE						= 103;
const WORD SUB_S_CATCH_FISH						= 104;
const WORD SUB_S_BULLET_ION_TIMEOUT				= 105;
const WORD SUB_S_LOCK_TIMEOUT					= 106;
const WORD SUB_S_CATCH_SWEEP_FISH				= 107;
const WORD SUB_S_CATCH_SWEEP_FISH_RESULT		= 108;
const WORD SUB_S_HIT_FISH_LK					= 109;
const WORD SUB_S_SWITCH_SCENE					= 110;
const WORD SUB_S_STOCK_OPERATE_RESULT			= 111;
const WORD SUB_S_SCENE_END						= 112;
const WORD SUB_S_FIRE_SCORE						= 113;
const WORD SUB_S_NOW_HARVEST					= 114;		//捕鱼战绩
const WORD SUB_S_ONLINE_AWARD					= 115;		//在线奖励
const WORD SUB_S_AWARD_RESULT					= 116;      //在线奖励返回结果

//20150613
struct FishTraceInfo 
{
	float init_x_pos[5];
	float init_y_pos[5];
	int init_count;
};

struct CMD_S_GameStatus
{
  DWORD game_version;
  //bool   isscene;	//verysharp
  FISHSCORE fish_score[BY_GAME_PLAYER];
  FISHSCORE exchange_fish_score[BY_GAME_PLAYER];
};

struct CMD_S_FishScore
{
	LONGLONG fish_score[BY_GAME_PLAYER];
};

struct CMD_S_AwardScore
{
	WORD chair_id;
	int  curStep;
	FISHSCORE award_score;
};

struct CMD_S_GameConfig {
	int exchange_ratio_userscore;
	int exchange_ratio_fishscore;
	int exchange_count;

	int min_bullet_multiple;
	int max_bullet_multiple;

	int bomb_range_width;
	int bomb_range_height;

	int fish_multiple[FISH_KIND_COUNT];
	int fish_speed[FISH_KIND_COUNT];
	int fish_bounding_box_width[FISH_KIND_COUNT];
	int fish_bounding_box_height[FISH_KIND_COUNT];
	int fish_hit_radius[FISH_KIND_COUNT];

	int bullet_speed[BULLET_KIND_COUNT];
	int net_radius[BULLET_KIND_COUNT];
};

//struct CMD_S_GameConfig
//{
//  int exchange_ratio_userscore;
//  int exchange_ratio_fishscore;
//  int exchange_count;
//
//  int min_bullet_multiple;
//  int max_bullet_multiple;
//  //int cannonNumber;//verysharp
//
//  int bomb_range_width;
//  int bomb_range_height;
//
//  int fish_multiple[FISH_KIND_COUNT];
//  int fish_speed[FISH_KIND_COUNT];
//  int fish_bounding_box_width[FISH_KIND_COUNT];
//  int fish_bounding_box_height[FISH_KIND_COUNT];
//  int fish_hit_radius[FISH_KIND_COUNT];
//
//  int bullet_speed[BULLET_KIND_COUNT];
//  int net_radius[BULLET_KIND_COUNT];
//
//  int lock_percent;
//  int lostlock_percent;
//  int firetime_;
//  int fish_time;
//  int max_bullet_num;
//
//  int fishL_max_multiple;
//  int fishLK_max_multiple;
//};

struct CMD_S_FishTrace
{
  FPoint init_pos[5];
  int init_count;
  FishKind fish_kind;
  int fish_id;
  TraceType trace_type;
#ifdef USE_TRACETIME
  DWORD FishMoveSecs;
  DWORD BuildTraceTime;
#endif
};

struct CMD_S_ExchangeFishScore
{
  WORD chair_id;
  FISHSCORE swap_fish_score;
  FISHSCORE exchange_fish_score;
  //FISHSCORE user_score; //verysharp
};

struct CMD_S_UserFire
{
  BulletKind bullet_kind;
  int bullet_id;
  WORD chair_id;
  WORD android_chairid;
  float angle;
  int bullet_mulriple;
  int lock_fishid;
  FISHSCORE fish_score;
};

struct CMD_S_CatchFish
{
  WORD chair_id;
  int fish_id;
  FishKind fish_kind;
  bool bullet_ion;
  FISHSCORE fish_score;
  //FISHSCORE user_fish_score; //verysharp
};

struct CMD_S_BulletIonTimeout
{
  WORD chair_id;
};

struct CMD_S_CatchSweepFish
{
  WORD chair_id;
  int fish_id;
  //int bulletmul; //verysharp
};

struct CMD_S_CatchSweepFishResult
{
  WORD chair_id;
  int fish_id;
  FISHSCORE fish_score;
  FISHSCORE user_fish_score;
  int catch_fish_count;
  int catch_fish_id[300];
  //int bulletmul; //verysharp
};

struct CMD_S_HitFishLK
{
  WORD chair_id;
  int fish_id;
  int fish_mulriple;
};

struct CMD_S_SwitchScene
{
  SceneKind scene_kind;
  int fish_count;
  FishKind fish_kind[300];
  int fish_id[300];
};

struct CMD_S_StockOperateResult
{
  unsigned char operate_code;
  FISHSCORE stock_score;
};

struct CMD_S_FireHarvest
{
	int nFishHarvest[FISH_KIND_COUNT];
	LONGLONG iNowScore;			//当前身上的金币
	LONGLONG iNowFishScore;		//当前身上的鱼币
	LONGLONG iCanExchangeGold;	//身上的鱼币可转换成的金币
};

struct CMD_S_OnLineAward {
	WORD chair_id;		//桌子ID
	int nAllStep; 
	int nCurStep;
	int nTotalTime;		//用户总时间
};

//////////////////////////////////////////////////////////////////////////
// 客户端命令

const WORD SUB_C_EXCHANGE_FISHSCORE				=1;
const WORD SUB_C_USER_FIRE						=2;
const WORD SUB_C_CATCH_FISH						=3;
const WORD SUB_C_CATCH_SWEEP_FISH				=4;
const WORD SUB_C_HIT_FISH_I						=5;
const WORD SUB_C_STOCK_OPERATE					=6;
const WORD SUB_C_USER_FILTER					=7;
const WORD SUB_C_ANDROID_STAND_UP				=8;
const WORD SUB_C_FISH20_CONFIG					=9;
const WORD SUB_C_ANDROID_BULLET_MUL				=10;
const WORD SUB_C_SWITCH_BULLENT					=11;
const WORD SUB_MOBILE_GET_Harvest				=12;		//手机端获取捕鱼战果
const WORD SUB_C_ONLINE_AWARD_GET				=13;		//获取游戏在线奖励显示
const WORD SUB_C_ONLINE_AWARD_COM				=14;		//确认领取游戏在线奖励
const WORD SUB_C_TIME_STAMP					    =15;		//时间戳<by hxh>

/* 
时间戳
单向传输时间T=(C2-C1+S1-S2)/2
时间差dT=C1-S1
例如，服务器端轨迹建立时间BuildTraceTime换算成客户端的时间戳是BuildTraceTime+dT
*/
struct CMD_C_TimeStamp
{
	CMD_C_TimeStamp()
	{
		memset(this, 0, sizeof(*this));
	}
     
	unsigned long long tClientSendTime;				//客户端发送时刻C1,客户端发送请求包时填写
	DWORD tServerRecvTime;							//服务器接收时刻S1
	DWORD tServerSendTime;							//服务器发送时刻S2
    unsigned long long tClientRecvTime;				//客户端接收时刻C2,客户端收到返回包时填写
	// 计算出来的常量
    long long tTransTime;//常量T 
    long long tClientServerTimeInterval;//常量dT
	inline void AutoCalc();
};

inline void CMD_C_TimeStamp::AutoCalc()
{
	tTransTime=(tClientRecvTime-tClientSendTime+tServerRecvTime-tServerSendTime)/2;
	tClientServerTimeInterval=tClientSendTime-tServerRecvTime;
}

struct CMD_C_ExchangeFishScore
{
	int increase;		//0表示下分，1表示按配置文件设定数额上分，2表示一次性上完所有的分，3表示手机端上分
	//__int64 iExchangeCount;	//手机端上分数量 //verysharp
};

struct CMD_C_UserFire
{
  BulletKind bullet_kind;
  float angle;
  int bullet_mulriple;
  int lock_fishid;
};

struct CMD_C_CatchFish
{
  WORD chair_id;
  int fish_id;
  BulletKind bullet_kind;
  int bullet_id;
  int bullet_mulriple;
  //DWORD dwCatchFishTick; //verysharp
};

struct CMD_C_CatchSweepFish
{
  WORD chair_id;
  int fish_id;
  int catch_fish_count;
  int catch_fish_id[300];
  //int bulletmul;  //verysharp
  //DWORD dwCatchFishTick;  //verysharp
};

struct CMD_C_HitFishLK
{
  int fish_id;
  //int bulletmul; //verysharp
};

struct CMD_C_StockOperate
{
  unsigned char operate_code; // 0查询 1 清除 2 增加 3 查询抽水
};

struct CMD_C_UserFilter
{
  DWORD game_id;
  unsigned char operate_code; // 0 黑名单 1 白名单 2 清除
};

struct CMD_C_Fish20Config 
{
  DWORD game_id;
  int catch_count;
  double catch_probability;
};

struct CMD_C_AndroidBulletMul
{
  WORD chair_id;
  int bullet_id;
  int bullet_mulriple;
  BulletKind bullet_kind;
};

enum SendDataBaseRequst
{	
	DBR_GR_FISH_ROOMDATA = 2000,				//查询房间
	DBR_GR_FISH_SAVE_SUERDATA,					//写人个人	
	DBR_GR_FISH_GET_SUERDATA,					//获取个人	
	DBR_GR_FISH_UPATA_SUERDATA					//更新个人

};

}
#pragma pack(pop)
#endif // CMD_FISH_H_
