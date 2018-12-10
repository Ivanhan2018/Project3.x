#ifndef OPTION_PARAMETER_HEAD_FILE
#define OPTION_PARAMETER_HEAD_FILE

#include <map>
#include "types.h"
#include "Struct.h"
#include "Define.h"
//配置信息
#define REG_GOBAL_OPTION			"GolbalOption"				//全局信息


//////////////////////////////////////////////////////////////////////////////////
//枚举定义

//邀请模式
#define INVITE_MODE_ALL				0									//全部显示
#define INVITE_MODE_NONE			1									//全不显示
#define INVITE_MODE_FRIEND			2									//好友显示

//消息模式
#define MESSAGE_MODE_ALL			0									//全部显示
#define MESSAGE_MODE_DETEST			1									//屏蔽厌恶
#define MESSAGE_MODE_FRIEND			2									//社团好友
#define MESSAGE_MODE_NONE			3									//全不显示

//界面动作
#define ACTION_ORIENTATION			0									//定位用户
#define ACTION_SEND_WHISPER			1									//发送私聊
#define ACTION_SHOW_USER_INFO		2									//用户信息
#define ACTION_SEARCH_TABLE			3									//寻找位置
#define ACTION_SHOW_SEARCH_DLG		4									//查找界面

//////////////////////////////////////////////////////////////////////////////////

//游戏参数
class CParameterGame
{
	//友元定义
	friend class CParameterGlobal;

	//胜率限制
public:
	word							m_wMinWinRate;						//最低胜率
	bool							m_bLimitWinRate;					//限制胜率

	//逃率限制
public:
	word							m_wMaxFleeRate;						//最高逃跑
	bool							m_bLimitFleeRate;					//限制断线

	//积分限制
public:
	long							m_lMaxGameScore;					//最高分数 
	long							m_lMinGameScore;					//最低分数
	bool							m_bLimitGameScore;					//限制分数

	//属性变量
protected:
	char							m_szRegKeyName[16];					//注册项名

	//函数定义
public:
	//构造函数
	CParameterGame();
	//析构函数
	virtual ~CParameterGame();

	//功能函数
public:
	//加载参数
	void LoadParameter();
	//保存参数
	void SaveParameter();
	//默认参数
	void DefaultParameter();

	//配置函数
protected:
	//配置参数
	bool InitParameter(const char* pszProcessName);
};

//////////////////////////////////////////////////////////////////////////////////

//房间参数
class CParameterServer
{
	//配置变量
public:
	bool							m_bTakePassword;					//密码标志
	char							m_szPassword[LEN_PASSWORD];			//桌子密码

	//函数定义
public:
	//构造函数
	CParameterServer();
	//析构函数
	virtual ~CParameterServer();

	//配置函数
public:
	//默认参数
	void DefaultParameter();
};

//////////////////////////////////////////////////////////////////////////////////
typedef std::map<word,CParameterGame *> CParameterGameMap;
typedef std::map<word,CParameterServer *> CParameterServerMap;

//全局参数
class CParameterGlobal
{
	//静态函数
public:
	//获取对象
	static CParameterGlobal* shared();
	static void purge();

	//登录配置
public:
	bool							m_bLogonAuto;						//自动登录
	bool							m_bWriteCookie;						//写入甜饼
	bool							m_bRemberPassword;					//记住密码

	//模式配置
public:
	byte							m_cbInviteMode;						//邀请模式
	byte							m_cbMessageMode;					//消息模式
	byte							m_cbActionHitAutoJoin;				//自动加入
	byte							m_cbActionLeftDoubleList;			//双击列表

	//时间配置
public:
	word							m_wIntermitTime;					//中断时间
	word							m_wOnLineCountTime;					//人数时间

	//房间配置
public:
	bool							m_bLimitDetest;						//限制厌恶
	bool							m_bLimitSameIP;						//限制地址
	bool							m_bNotifyUserInOut;					//进出消息
	bool							m_bNotifyFriendCome;				//好友提示

	//系统配置
public:
	bool							m_bAllowSound;						//允许声音
	bool							m_bAllowBackMusic;					//允许背景音乐
	bool							m_bAutoSitDown;						//自动坐下
	bool							m_bAutoShowWhisper;					//显示私聊
	bool							m_bSaveWhisperChat;					//保存私聊
	bool							m_bSendWhisperByEnter;				//回车发送

	//配置组件
protected:
	CParameterGameMap				m_ParameterGameMap;					//游戏配置
	CParameterServerMap				m_ParameterServerMap;				//房间配置

	//显示配置
public:
	bool                            m_bShowServerStatus;                //显示服务器状态

	//函数定义
public:
	//构造函数
	CParameterGlobal();
	//析构函数
	virtual ~CParameterGlobal();

	//配置函数
public:
	//加载参数
	void LoadParameter();
	//保存参数
	void SaveParameter();
	//默认参数
	void DefaultParameter();

	//游戏配置
public:
	//游戏配置
	CParameterGame * GetParameterGame(tagGameKind * pGameKind);
	//房间配置
	CParameterServer * GetParameterServer(tagGameServer * pGameServer);
};

//////////////////////////////////////////////////////////////////////////////////

#endif