#pragma once

#include "cocos2d.h"
#include "FvSingleton.h"
#include "GameLib.h"

#include "IServerItemSink.h"
#include "ServerListData.h"
#include "CServerItem.h"
#include "IClientKernelSink.h"

class GameManagerBase 
	:public IServerItemSink
	,public IServerListDataSink
	,public IStringMessageSink
	,public cocos2d::Ref
	,public TimeNode
	,public FvSingletonBase<GameManagerBase>
{
public:
	GameManagerBase();
	~GameManagerBase();
public:
	bool IsPrivateGame();
	int getActServerID();
	CGameServerItem* SearchGameServer(int iServerID);
	bool connectGameServerByKindID(word wKindID,word wServerType = INVALID_WORD);
	bool connectGameServerByServerID(int iServerID);
	bool connectGameServer(CGameServerItem* pGameServerItem);

	SCORE getGameDiFen();
public:
	virtual void onEnterTransitionDidFinish();

	//登陆信息
public:
	//坐下失败
	virtual void onGRRequestFailure(const std::string& sDescribeString);
	//登陆成功
	virtual void OnGRLogonSuccess(void* data, int dataSize);
	//登陆失败
	virtual void OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString);
	//登陆完成
	virtual void OnGRLogonFinish();
	//更新通知
	virtual void OnGRUpdateNotify(byte cbMustUpdate, const std::string& sDescribeString);

	virtual void CB_GameLogonFinsh();

	//配置信息
public:
	//列表配置
	virtual void OnGRConfigColumn();
	//房间配置
	virtual void OnGRConfigServer();
	//道具配置
	virtual void OnGRConfigProperty();
	//玩家权限配置
	virtual void OnGRConfigUserRight();
	//配置完成
	virtual void OnGRConfigFinish();

	//用户信息
public:
	//用户进入
	virtual void OnGRUserEnter(IClientUserItem* pIClientUserItem);
	//用户更新
	virtual void OnGRUserUpdate(IClientUserItem* pIClientUserItem);
	//用户删除
	virtual void OnGRUserDelete(IClientUserItem* pIClientUserItem);

	//框架消息
public:
	//用户邀请
	virtual void OnGFUserInvite(const std::string& szMessage);
	//用户邀请失败
	virtual void OnGFUserInviteFailure(const std::string& szMessage);
	//房间退出
	virtual void OnGFServerClose(const std::string& szMessage);
	//创建游戏内核
	virtual bool CreateKernel();
	//启动游戏
	virtual bool StartGame();
public:
	//完成通知
	virtual void OnGameItemFinish();
	//完成通知
	virtual void OnGameKindFinish(uint16 wKindID);
	//更新通知
	virtual void OnGameItemUpdateFinish();

	//更新通知
public:
	//插入通知
	virtual void OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	virtual void OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	virtual void OnGameItemDelete(CGameListItem * pGameListItem);
public:
	virtual IClientKernelSink* CreateGame(word wKindID) = 0;
	virtual void loadGameBaseData(word wKindID) = 0;
public:
	virtual void OnGameEnd(cocos2d::Node* pNode,float fWaiteTime){}
public:
	///< 链接服务器
	void connectServer();
	///< 断开连接
	void disconnectServer();	//事件消息
public:
	//进入事件
	virtual bool InsertUserEnter(const char* pszUserName);
	//离开事件
	virtual bool InsertUserLeave(const char* pszUserName);
	//断线事件
	virtual bool InsertUserOffLine(const char* pszUserName);
public:
	//普通消息(窗口输出)
	virtual bool InsertNormalString(const char* pszString);
	//系统消息(窗口输出)
	virtual bool InsertSystemString(const char* pszString);
	//系统消息(窗口输出)
	virtual bool InsertSystemStringScript(const char* pszString);
	//提示消息(对话框方式??)0:确认 1:确认,取消
	virtual int InsertPromptString(const char* pszString, int iButtonType);

public:
	virtual bool InsertGladString(const char* pszContent, const char* pszNickName, const char* pszNum, dword colText, dword colName, dword colNum);

	bool BackKey();
	//心跳包
public:
	virtual void HeartTick();
	virtual void onEventTCPSocketError(int errorCode);
	void closeClinet();
	void RestTick();
	void StartGameReconnect();
	void upDataTime(float fTime);

	//服务状态
	enum enReconnectStatus
	{
		ReconnectStatus_NULL,			
		ReconnectStatus_DisConnect,		
		ReconnectStatus_Connecting,		
	};
public:
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
protected:
	enReconnectStatus	m_eInReconnect;
	float				m_fHeatTickTime;

	CGameServerItem*	mGameServerItem;
	CServerItem*		mCServerItem;
};