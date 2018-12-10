#ifndef SERVER_RULE_HEAD_FILE
#define SERVER_RULE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//聊天规则
#define SR_FORFEND_GAME_CHAT		0x00000001							//禁止公聊
#define SR_FORFEND_ROOM_CHAT		0x00000002							//禁止公聊
#define SR_FORFEND_WISPER_CHAT		0x00000004							//禁止私聊
#define SR_FORFEND_WISPER_ON_GAME	0x00000008							//禁止私聊

//高级规则
#define SR_ALLOW_DYNAMIC_JOIN		0x00000010							//动态加入
#define SR_ALLOW_OFFLINE_TRUSTEE	0x00000020							//断线代打
#define SR_ALLOW_AVERT_CHEAT_MODE	0x00000040							//隐藏信息
#define SR_ALLOW_QUICK_MODE			0x00000080							//快速模式

//游戏规则
#define SR_RECORD_GAME_SCORE		0x00000100							//记录积分
#define SR_RECORD_GAME_TRACK		0x00000200							//记录过程
#define SR_DYNAMIC_CELL_SCORE		0x00000400							//动态底分
#define SR_IMMEDIATE_WRITE_SCORE	0x00000800							//即时写分

//房间规则
#define SR_FORFEND_ROOM_ENTER		0x00001000							//禁止进入
#define SR_FORFEND_GAME_ENTER		0x00002000							//禁止进入
#define SR_FORFEND_GAME_LOOKON		0x00004000							//禁止旁观

//银行规则
#define SR_FORFEND_TAKE_IN_ROOM		0x00010000							//禁止取款
#define SR_FORFEND_TAKE_IN_GAME		0x00020000							//禁止取款
#define SR_FORFEND_SAVE_IN_ROOM		0x00040000							//禁止存钱
#define SR_FORFEND_SAVE_IN_GAME		0x00080000							//禁止存款

//其他规则
#define SR_FORFEND_GAME_RULE		0x00100000							//禁止配置
#define SR_FORFEND_LOCK_TABLE		0x00200000							//禁止锁桌
#define SR_ALLOW_ANDROID_ATTEND		0x00400000							//允许陪玩
#define SR_ALLOW_ANDROID_SIMULATE	0x00800000							//允许占位

//////////////////////////////////////////////////////////////////////////////////

//房间规则
class CServerRule
{
	//聊天规则
public:
	//禁止公聊
	static bool IsForfendGameChat(dword dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_CHAT)!=0; }
	//禁止公聊
	static bool IsForfendRoomChat(dword dwServerRule) { return (dwServerRule&SR_FORFEND_ROOM_CHAT)!=0; }
	//禁止私聊
	static bool IsForfendWisperChat(dword dwServerRule) { return (dwServerRule&SR_FORFEND_WISPER_CHAT)!=0; }
	//禁止私聊
	static bool IsForfendWisperOnGame(dword dwServerRule) { return (dwServerRule&SR_FORFEND_WISPER_ON_GAME)!=0; }

	//模式规则
public:
	//动态加入
	static bool IsAllowDynamicJoin(dword dwServerRule) { return (dwServerRule&SR_ALLOW_DYNAMIC_JOIN)!=0; }
	//断线代打
	static bool IsAllowOffLineTrustee(dword dwServerRule) { return (dwServerRule&SR_ALLOW_OFFLINE_TRUSTEE)!=0; }
	//隐藏信息
	static bool IsAllowAvertCheatMode(dword dwServerRule) { return (dwServerRule&SR_ALLOW_AVERT_CHEAT_MODE)!=0; }
	//快速模式
	static bool IsAllowQuickMode(dword dwServerRule) { return (dwServerRule&SR_ALLOW_QUICK_MODE)!=0; }

	//游戏规则
public:
	//记录积分
	static bool IsRecordGameScore(dword dwServerRule) { return (dwServerRule&SR_RECORD_GAME_SCORE)!=0; }
	//记录过程
	static bool IsRecordGameTrack(dword dwServerRule) { return (dwServerRule&SR_RECORD_GAME_TRACK)!=0; }
	//动态底分
	static bool IsDynamicCellScore(dword dwServerRule) { return (dwServerRule&SR_DYNAMIC_CELL_SCORE)!=0; }
	//即时写分
	static bool IsImmediateWriteScore(dword dwServerRule) { return (dwServerRule&SR_IMMEDIATE_WRITE_SCORE)!=0; }

	//房间规则
public:
	//禁止进入
	static bool IsForfendRoomEnter(dword dwServerRule) { return (dwServerRule&SR_FORFEND_ROOM_ENTER)!=0; }
	//禁止进入
	static bool IsForfendGameEnter(dword dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_ENTER)!=0; }
	//禁止旁观
	static bool IsForfendGameLookon(dword dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_LOOKON)!=0; }

	//银行规则
public:
	//禁止取款
	static bool IsForfendTakeInRoom(dword dwServerRule) { return (dwServerRule&SR_FORFEND_TAKE_IN_ROOM)!=0; }
	//禁止取款
	static bool IsForfendTakeInGame(dword dwServerRule) { return (dwServerRule&SR_FORFEND_TAKE_IN_GAME)!=0; }
	//禁止存钱
	static bool IsForfendSaveInRoom(dword dwServerRule) { return (dwServerRule&SR_FORFEND_SAVE_IN_ROOM)!=0; }
	//禁止存钱
	static bool IsForfendSaveInGame(dword dwServerRule) { return (dwServerRule&SR_FORFEND_SAVE_IN_GAME)!=0; }

	//其他规则
public:
	//禁止配置
	static bool IsForfendGameRule(dword dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_RULE)!=0; }
	//禁止锁桌
	static bool IsForfendLockTable(dword dwServerRule) { return (dwServerRule&SR_FORFEND_LOCK_TABLE)!=0; }
	//允许陪玩
	static bool IsAllowAndroidAttend(dword dwServerRule) { return (dwServerRule&SR_ALLOW_ANDROID_ATTEND)!=0; }
	//允许占位
	static bool IsAllowAndroidSimulate(dword dwServerRule) { return (dwServerRule&SR_ALLOW_ANDROID_SIMULATE)!=0; }

	//聊天规则
public:
	//禁止公聊
	static void SetForfendGameChat(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_CHAT:dwServerRule&=~SR_FORFEND_GAME_CHAT; }
	//禁止公聊
	static void SetForfendRoomChat(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_ROOM_CHAT:dwServerRule&=~SR_FORFEND_ROOM_CHAT; }
	//禁止私聊
	static void SetForfendWisperChat(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_WISPER_CHAT:dwServerRule&=~SR_FORFEND_WISPER_CHAT; }
	//禁止私聊
	static void SetForfendWisperOnGame(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_WISPER_ON_GAME:dwServerRule&=~SR_FORFEND_WISPER_ON_GAME; }

	//模式规则
public:
	//动态加入
	static void SetAllowDynamicJoin(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_DYNAMIC_JOIN:dwServerRule&=~SR_ALLOW_DYNAMIC_JOIN; }
	//断线代打
	static void SetAllowOffLineTrustee(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_OFFLINE_TRUSTEE:dwServerRule&=~SR_ALLOW_OFFLINE_TRUSTEE; }
	//隐藏信息
	static void SetAllowAvertCheatMode(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_AVERT_CHEAT_MODE:dwServerRule&=~SR_ALLOW_AVERT_CHEAT_MODE; }
	//快速模式
	static void SetAllowQuickMode(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_QUICK_MODE:dwServerRule&=~SR_ALLOW_QUICK_MODE; }

	//游戏规则
public:
	//记录积分
	static void SetRecordGameScore(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_RECORD_GAME_SCORE:dwServerRule&=~SR_RECORD_GAME_SCORE; }
	//记录过程
	static void SetRecordGameTrack(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_RECORD_GAME_TRACK:dwServerRule&=~SR_RECORD_GAME_TRACK; }
	//动态底分
	static void SetDynamicCellScore(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_DYNAMIC_CELL_SCORE:dwServerRule&=~SR_DYNAMIC_CELL_SCORE; }
	//即时写分
	static void SetImmediateWriteScore(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_IMMEDIATE_WRITE_SCORE:dwServerRule&=~SR_IMMEDIATE_WRITE_SCORE; }

	//房间规则
public:
	//禁止进入
	static void SetForfendRoomEnter(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_ROOM_ENTER:dwServerRule&=~SR_FORFEND_ROOM_ENTER; }
	//禁止进入
	static void SetForfendGameEnter(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_ENTER:dwServerRule&=~SR_FORFEND_GAME_ENTER; }
	//禁止旁观
	static void SetForfendGameLookon(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_LOOKON:dwServerRule&=~SR_FORFEND_GAME_LOOKON; }

	//银行规则
public:
	//禁止取款
	static void SetForfendTakeInRoom(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_TAKE_IN_ROOM:dwServerRule&=~SR_FORFEND_TAKE_IN_ROOM; }
	//禁止取款
	static void SetForfendTakeInGame(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_TAKE_IN_GAME:dwServerRule&=~SR_FORFEND_TAKE_IN_GAME; }
	//禁止存钱
	static void SetForfendSaveInRoom(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_SAVE_IN_ROOM:dwServerRule&=~SR_FORFEND_SAVE_IN_ROOM; }
	//禁止存钱
	static void SetForfendSaveInGame(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_SAVE_IN_GAME:dwServerRule&=~SR_FORFEND_SAVE_IN_GAME; }

	//其他规则
public:
	//禁止配置
	static void SetForfendGameRule(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_RULE:dwServerRule&=~SR_FORFEND_GAME_RULE; }
	//禁止锁桌
	static void SetForfendLockTable(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_LOCK_TABLE:dwServerRule&=~SR_FORFEND_LOCK_TABLE; }
	//允许陪玩
	static void SetAllowAndroidAttend(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_ANDROID_ATTEND:dwServerRule&=~SR_ALLOW_ANDROID_ATTEND; }
	//允许占位
	static void SetAllowAndroidSimulate(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_ANDROID_SIMULATE:dwServerRule&=~SR_ALLOW_ANDROID_SIMULATE; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif