#include "OptionParameter.h"
#include "CCUserDefault.h"

//构造函数
CParameterGame::CParameterGame()
{
	//默认参数
	DefaultParameter();

	//属性变量
	memset(m_szRegKeyName, 0, sizeof(m_szRegKeyName));

	return;
}

//析构函数
CParameterGame::~CParameterGame()
{
}

//加载参数
void CParameterGame::LoadParameter()
{
	return;
	//配置表项
	
	//胜率限制
	m_wMinWinRate=(word)MyUserDefault::getInstance()->getIntegerForKey(m_szRegKeyName, "MinWinRate", m_wMinWinRate);
	m_bLimitWinRate=MyUserDefault::getInstance()->getBoolForKey(m_szRegKeyName, "LimitWinRate",m_bLimitWinRate);

	//逃率限制
	m_wMaxFleeRate=(word)MyUserDefault::getInstance()->getIntegerForKey(m_szRegKeyName, "MaxFleeRate",m_wMaxFleeRate);
	m_bLimitFleeRate=MyUserDefault::getInstance()->getBoolForKey(m_szRegKeyName, "LimitFleeRate",m_bLimitFleeRate);
		
	//积分限制
	m_lMaxGameScore=MyUserDefault::getInstance()->getIntegerForKey(m_szRegKeyName, "MaxGameScore",m_lMaxGameScore);
	m_lMinGameScore=MyUserDefault::getInstance()->getIntegerForKey(m_szRegKeyName, "MinGameScore",m_lMinGameScore);
	m_bLimitGameScore=MyUserDefault::getInstance()->getBoolForKey(m_szRegKeyName, "LimitGameScore",m_bLimitGameScore);
	
	return;
}

//保存参数
void CParameterGame::SaveParameter()
{
	return;
	//配置表项

	//胜率限制
	MyUserDefault::getInstance()->setIntegerForKey(m_szRegKeyName, "MinWinRate", m_wMinWinRate);
	MyUserDefault::getInstance()->setBoolForKey(m_szRegKeyName, "LimitWinRate",m_bLimitWinRate);

	//逃率限制
	MyUserDefault::getInstance()->setIntegerForKey(m_szRegKeyName, "MaxFleeRate",m_wMaxFleeRate);
	MyUserDefault::getInstance()->setBoolForKey(m_szRegKeyName, "LimitFleeRate",m_bLimitFleeRate);

	//积分限制
	MyUserDefault::getInstance()->setIntegerForKey(m_szRegKeyName, "MaxGameScore",m_lMaxGameScore);
	MyUserDefault::getInstance()->setIntegerForKey(m_szRegKeyName, "MinGameScore",m_lMinGameScore);
	MyUserDefault::getInstance()->setBoolForKey(m_szRegKeyName, "LimitGameScore",m_bLimitGameScore);

	return;
}

//默认参数
void CParameterGame::DefaultParameter()
{
	//胜率限制
	m_wMinWinRate=0;
	m_bLimitWinRate=false;

	//逃率限制
	m_wMaxFleeRate=5000;
	m_bLimitFleeRate=false;

	//积分限制
	m_bLimitGameScore=false;
	m_lMaxGameScore=2147483647L;
	m_lMinGameScore=-2147483646L;

	return;
}

//配置参数
bool CParameterGame::InitParameter(const char* pszProcessName)
{
	//构造键名
	word wNameIndex=0;
	while (wNameIndex<(countarray(m_szRegKeyName)-1))
	{
		//终止判断
		if (pszProcessName[wNameIndex]==0) break;
		if (pszProcessName[wNameIndex]=='.') break;

		//设置字符
		word wCurrentIndex=wNameIndex++;
		m_szRegKeyName[wCurrentIndex]=pszProcessName[wCurrentIndex];
	}

	//设置变量
	m_szRegKeyName[wNameIndex]=0;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CParameterServer::CParameterServer()
{
	//默认参数
	DefaultParameter();

	return;
}

//析构函数
CParameterServer::~CParameterServer()
{
}

//默认参数
void CParameterServer::DefaultParameter()
{
	//配置变量
	m_bTakePassword=false;
	zeromemory(m_szPassword,sizeof(m_szPassword));

	return;
}

//////////////////////////////////////////////////////////////////////////
//获取对象
//静态变量
CParameterGlobal * __gParameterGlobal=0;			//全局配置

CParameterGlobal* CParameterGlobal::shared()
{
	if (__gParameterGlobal == 0)
		__gParameterGlobal = new CParameterGlobal();
	return __gParameterGlobal;
}

void CParameterGlobal::purge()
{
	if (__gParameterGlobal)
		delete __gParameterGlobal;
	__gParameterGlobal = 0;
}

//////////////////////////////////////////////////////////////////////////
//构造函数
CParameterGlobal::CParameterGlobal()
{
	//默认参数
	DefaultParameter();
	return;
}

//析构函数
CParameterGlobal::~CParameterGlobal()
{
	CParameterGameMap::iterator GameIter = m_ParameterGameMap.begin();
	for (; GameIter != m_ParameterGameMap.end(); ++GameIter)
	{
		delete GameIter->second;
	}

	m_ParameterGameMap.clear();

	CParameterServerMap::iterator ServerIter = m_ParameterServerMap.begin();
	for (; ServerIter != m_ParameterServerMap.end(); ++ServerIter)
	{
		delete ServerIter->second;
	}

	m_ParameterServerMap.clear();
	return;
}

//加载参数
void CParameterGlobal::LoadParameter()
{
	return;
	//自动登录
	m_bLogonAuto=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"LogonAuto",m_bLogonAuto);
	m_bWriteCookie=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"WriteCookie",m_bWriteCookie);

	//邀请模式
	m_cbInviteMode=(byte)MyUserDefault::getInstance()->getIntegerForKey(REG_GOBAL_OPTION,"InviteMode",m_cbInviteMode);
	switch (m_cbInviteMode)
	{
	case INVITE_MODE_NONE:
	case INVITE_MODE_FRIEND: { break; }
	default: { m_cbInviteMode=INVITE_MODE_ALL; }
	}

	//消息模式
	m_cbMessageMode=(byte)MyUserDefault::getInstance()->getIntegerForKey(REG_GOBAL_OPTION,"MessageMode",m_cbMessageMode);
	switch (m_cbMessageMode)
	{
	case MESSAGE_MODE_ALL:
	case MESSAGE_MODE_FRIEND: { break; }
	default: { m_cbMessageMode=MESSAGE_MODE_DETEST; }
	}

	//自动加入
	m_cbActionHitAutoJoin=(byte)MyUserDefault::getInstance()->getIntegerForKey(REG_GOBAL_OPTION,"ActionHitAutoJoin",m_cbActionHitAutoJoin);
	switch (m_cbActionHitAutoJoin)
	{
	case ACTION_SHOW_SEARCH_DLG: { break; }
	default: { m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE; }
	}

	//双击列表
	m_cbActionLeftDoubleList=(byte)MyUserDefault::getInstance()->getIntegerForKey(REG_GOBAL_OPTION,"ActionLeftDoubleList",m_cbActionLeftDoubleList);
	switch (m_cbActionLeftDoubleList)
	{
	case ACTION_ORIENTATION:
	case ACTION_SHOW_USER_INFO: { break; }
	default: { m_cbActionLeftDoubleList=ACTION_SEND_WHISPER; }
	}

	//房间配置
	m_bLimitDetest=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"LimitDetest",m_bLimitDetest);
	m_bLimitSameIP=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"LimitSameIP",m_bLimitSameIP);
	m_bNotifyUserInOut=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"NotifyUserInOut",m_bNotifyUserInOut);
	m_bNotifyFriendCome=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"NotifyFriendCome",m_bNotifyFriendCome);

	//系统配置
	m_bAllowSound=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"AllowSound",m_bAllowSound);
	m_bAllowBackMusic=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"AllowBackMusic",m_bAllowBackMusic);
	m_bAutoSitDown=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"AutoSitDown",m_bAutoSitDown);
	m_bAutoShowWhisper=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"AutoShowWhisper",m_bAutoShowWhisper);
	m_bSaveWhisperChat=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"SaveWhisperChat",m_bSaveWhisperChat);
	m_bSendWhisperByEnter=MyUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION,"SendWhisperByEnter",m_bSendWhisperByEnter);

	return;
}

//保存参数
void CParameterGlobal::SaveParameter()
{
	return;
	//控制变量
	{
		//自动登录
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"LogonAuto",m_bLogonAuto);
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"WriteCookie",m_bWriteCookie);

		//邀请模式
		MyUserDefault::getInstance()->setIntegerForKey(REG_GOBAL_OPTION,"InviteMode",m_cbInviteMode);
		
		//消息模式
		MyUserDefault::getInstance()->setIntegerForKey(REG_GOBAL_OPTION,"MessageMode",m_cbMessageMode);
		
		//自动加入
		MyUserDefault::getInstance()->setIntegerForKey(REG_GOBAL_OPTION,"ActionHitAutoJoin",m_cbActionHitAutoJoin);
		
		//双击列表
		MyUserDefault::getInstance()->setIntegerForKey(REG_GOBAL_OPTION,"ActionLeftDoubleList",m_cbActionLeftDoubleList);
		
		//房间配置
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"LimitDetest",m_bLimitDetest);
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"LimitSameIP",m_bLimitSameIP);
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"NotifyUserInOut",m_bNotifyUserInOut);
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"NotifyFriendCome",m_bNotifyFriendCome);

		//系统配置
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"AllowSound",m_bAllowSound);
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"AllowBackMusic",m_bAllowBackMusic);
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"AutoSitDown",m_bAutoSitDown);
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"AutoShowWhisper",m_bAutoShowWhisper);
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"SaveWhisperChat",m_bSaveWhisperChat);
		MyUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION,"SendWhisperByEnter",m_bSendWhisperByEnter);

	}

	
	CParameterGameMap::iterator GameIter = m_ParameterGameMap.begin();
	for (; GameIter != m_ParameterGameMap.end(); ++GameIter)
	{
		GameIter->second->SaveParameter();
	}
	return;
}

//默认参数
void CParameterGlobal::DefaultParameter()
{
	//登录配置
	m_bLogonAuto=false;
	m_bWriteCookie=true;
	m_bRemberPassword=false;

	//时间配置
	m_wIntermitTime=0L;
	m_wOnLineCountTime=0L;

	//房间配置
	m_cbInviteMode=INVITE_MODE_ALL;
	m_cbMessageMode=MESSAGE_MODE_ALL;
	m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE;
	m_cbActionLeftDoubleList=ACTION_SEND_WHISPER;

	//房间配置
	m_bLimitDetest=false;
	m_bLimitSameIP=false;
	m_bNotifyUserInOut=false;
	m_bNotifyFriendCome=true;

	//系统配置
	m_bAllowSound=true;
	m_bAllowBackMusic=true;
	m_bAutoSitDown=true;
	m_bSaveWhisperChat=true;
	m_bAutoShowWhisper=false;
	m_bSendWhisperByEnter=true;

	return;
}

//游戏配置
CParameterGame * CParameterGlobal::GetParameterGame(tagGameKind * pGameKind)
{
	//寻找现存
	CParameterGameMap::iterator it= m_ParameterGameMap.find(pGameKind->wKindID);
	if (it != m_ParameterGameMap.end())
		return it->second;

	//创建对象
	try
	{
		//创建对象
		CParameterGame* pParameterGame=new CParameterGame;
		if (pParameterGame==0) return 0;

		//配置对象
//		pParameterGame->InitParameter(pGameKind->szProcessName);

		//加载参数
		pParameterGame->LoadParameter();

		//设置数组
		m_ParameterGameMap[pGameKind->wKindID]=pParameterGame;

		return pParameterGame;
	}
	catch (...) {}

	return 0;
}

//房间配置
CParameterServer * CParameterGlobal::GetParameterServer(tagGameServer * pGameServer)
{
	//寻找现存
	CParameterServerMap::iterator it= m_ParameterServerMap.find(pGameServer->wServerID);
	if (it != m_ParameterServerMap.end())
		return it->second;

	//创建对象
	try
	{
		//创建对象
		CParameterServer* pParameterServer=new CParameterServer;
		if (pParameterServer==0) return 0;

		//设置数组
		m_ParameterServerMap[pGameServer->wServerID]=pParameterServer;

		return pParameterServer;
	}
	catch (...) {}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
