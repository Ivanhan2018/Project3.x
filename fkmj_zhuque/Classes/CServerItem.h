#ifndef _CServerItem_H_
#define _CServerItem_H_

#include "ISocketEngine.h"
#include "ClientUserManager.h"
#include "IRoomFrameSink.h"
#include "IGameFrameSink.h"
#include "IServerItemSink.h"
#include "IChatSink.h"
#include "IServerItemSink.h"
#include "IServerMatchSink.h"
#include "IStringMessageSink.h"
#include "IServerItem.h"
#include "IServerPrivateSink.h"
#include "TableViewFrame.h"
#include "IClientKernelSink.h"
class CTableViewFrame;
class CUserManager;

class CServerItem 
	: public IServerItem
	, public IUserManagerSink
	, public ISocketEngineSink
	//, public IRoomFrameSink
{

public:
	CServerItem();
	~CServerItem();
	//////////////////////////////////////////////////////////////////////////
	// IServerItem
	//////////////////////////////////////////////////////////////////////////


	//配置接口
public:
	//设置接口
	virtual bool SetServerItemSink(IServerItemSink* pIServerItemSink);
	//设置接口
	virtual bool SetStringMessageSink(IStringMessageSink* pIStringMessageSink);
	//设置接口
	virtual bool SetServerMatchSink(IServerMatchSink* pIServerMatchSink);
	//设置接口
	virtual bool SetServerPrivateSink(IServerPrivateSink* pIServerMatchSink);
	//配置房间
	virtual bool SetServerAttribute(CGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr);

	//连接接口
public:
	//初始化房间
	virtual bool ConnectServer(CGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr);
	//中断连接
	virtual bool IntermitConnect(bool force);
	

	//属性接口
public:
	//用户属性
	virtual const tagUserAttribute& GetUserAttribute() const;
	//房间属性
	virtual const tagServerAttribute&  GetServerAttribute() const;
	//服务状态
	virtual enServiceStatus GetServiceStatus();
	//是否服务状态
	virtual bool IsService();
	//设置状态
	virtual void SetServiceStatus(enServiceStatus ServiceStatus);
	//自己状态
	virtual bool IsPlayingMySelf();
	
	//用户接口
public:
	//自己位置
	virtual word GetMeChairID();
	//自己位置
	virtual IClientUserItem * GetMeUserItem();
	//游戏用户
	virtual IClientUserItem * GetTableUserItem(word wChariID);
	//查找用户
	virtual IClientUserItem * SearchUserByUserID(dword dwUserID);
	//查找用户
	virtual IClientUserItem * SearchUserByGameID(dword dwGameID);
	//查找用户
	virtual IClientUserItem * SearchUserByNickName(const char* szNickName);
	//用户数
	virtual dword GetActiveUserCount();
	
	//桌子接口
public:
	//获取对应桌子是否锁的状态
	virtual bool GetTableLockState(int tableId);
	//桌子是否游戏
	virtual bool GetTableGameState(int tableId);

	//网络接口
public:
	//发送函数
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID);
	//发送函数
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID, void * data, word dataSize);

	//网络命令
public:
	//发送登录
	virtual bool SendLogonPacket();
	//发送规则
	virtual bool SendUserRulePacket();
	//发送旁观
	virtual bool SendLookonPacket(word wTableID, word wChairID);
	//发送坐下
	virtual bool SendSitDownPacket(word wTableID, word wChairID, const char* lpszPassword=0);
	//发送起立
	virtual bool SendStandUpPacket(word wTableID, word wChairID, byte cbForceLeave);
	//发送表情
	virtual bool SendExpressionPacket(dword dwTargetUserID, word wItemIndex);
	//发送聊天
	virtual bool SendUserChatPacket(dword dwTargetUserID, const char* pszChatString, dword dwColor);

	//动作处理
public:
	//查找桌子
	virtual bool FindGameTable(tagFindTable & FindInfo);
	//执行快速加入
	virtual bool PerformQuickSitDown();
	//执行旁观
	virtual bool PerformLookonAction(word wTableID, word wChairID);
	//执行起立
	virtual bool PerformStandUpAction();
	//执行坐下
	virtual bool PerformSitDownAction(word wTableID, word wChairID, bool bEfficacyPass, const char * psw = nullptr);
	//执行购买
	virtual bool PerformBuyProperty(byte cbRequestArea,const char* pszNickName, word wItemCount, word wPropertyIndex);

	//内部函数
protected:
	//聊天效验
	bool EfficacyUserChat(const char* pszChatString, word wExpressionIndex);
	//桌子效验
	bool EfficacyTableRule(word wTableID, word wChairID, bool bReqLookon, char strDescribe[256]);
	//获得空的桌子id
	virtual int GetSpaceTableId();
	//获得空的椅子
	virtual int GetSpaceChairId(int tableId);
	//获得桌子总数量
	virtual int GetTotalTableCount();
	//获得游戏服务
	//////////////////////////////////////////////////////////////////////////
	// IUserManagerSink
	//////////////////////////////////////////////////////////////////////////
public:
	virtual void OnUserItemAcitve(IClientUserItem* pIClientUserItem);
	virtual void OnUserItemDelete(IClientUserItem* pIClientUserItem);
	virtual void OnUserFaceUpdate(IClientUserItem* pIClientUserItem);
	virtual void OnUserItemUpdate(IClientUserItem* pIClientUserItem, const tagUserScore& LastScore);
	virtual void OnUserItemUpdate(IClientUserItem* pIClientUserItem, const tagUserStatus& LastStatus);
	virtual void OnUserItemUpdate(IClientUserItem* pIClientUserItem, const tagUserAttrib & UserAttrib);

	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
	//////////////////////////////////////////////////////////////////////////
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
	virtual bool onEventTCPHeartTick();

	//////////////////////////////////////////////////////////////////////////
	//登录消息
	bool OnSocketMainLogon(int sub, void* data, int dataSize);
	//登录成功
	bool OnSocketSubLogonSuccess(void* data, int dataSize);
	//登录失败
	bool OnSocketSubLogonFailure(void* data, int dataSize);
	//登录完成
	bool OnSocketSubLogonFinish(void* data, int dataSize);
	//更新提示
	bool OnSocketSubUpdateNotify(void* data, int dataSize);
	
	//////////////////////////////////////////////////////////////////////////
	//配置信息
	bool OnSocketMainConfig(int sub, void* data, int dataSize);
	//列表配置
	bool OnSocketSubConfigColumn(void* data, int dataSize);
	//房间配置
	bool OnSocketSubConfigServer(void* data, int dataSize);
	//道具配置
	bool OnSocketSubConfigOrder(void* data, int dataSize);
	//配置玩家权限	
	bool OnSocketSubConfigMmber(void* data, int dataSize);
	//配置完成
	bool OnSocketSubConfigFinish(void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//用户信息
	bool OnSocketMainUser(int sub, void* data, int dataSize);
	//请求失败
	bool OnSocketSubRequestFailure(void* data, int dataSize);
	//用户进入
	bool OnSocketSubUserEnter(void* data, int dataSize);
	//用户积分
	bool OnSocketSubUserScore(void* data, int dataSize);
	//用户状态
	bool OnSocketSubUserStatus(void* data, int dataSize);
	//排队人数
	bool OnSocketSubQueueCount(void* data, int dataSize);
	//用户聊天
	bool OnSocketSubUserChat(void* data, int dataSize);
	//用户表情
	bool OnSocketSubExpression(void* data, int dataSize);
	//用户私聊
	bool OnSocketSubWisperUserChat(void* data, int dataSize);
	//私聊表情
	bool OnSocketSubWisperExpression(void* data, int dataSize);
	//道具成功
	bool OnSocketSubPropertySuccess(void* data, int dataSize);
	//道具失败
	bool OnSocketSubPropertyFailure(void* data, int dataSize);
	//道具效应
	bool OnSocketSubPropertyEffect(void* data, int dataSize);
	//道具消息
	bool OnSocketSubPropertyMessage(void* data, int dataSize);
	//道具喇叭
	bool OnSocketSubPropertyTrumpet(void* data, int dataSize);
	//喜报消息
	bool OnSocketSubGladMessage(void* data, int dataSize);
	
	//////////////////////////////////////////////////////////////////////////
	//状态信息
	bool OnSocketMainStatus(int sub, void* data, int dataSize);
	//桌子信息
	bool OnSocketSubStatusTableInfo(void* data, int dataSize);
	//桌子状态
	bool OnSocketSubStatusTableStatus(void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//银行消息
	bool OnSocketMainInsure(int sub, void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//管理消息
	bool OnSocketMainManager(int sub, void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//系统消息
	bool OnSocketMainSystem(int sub, void* data, int dataSize);
	//系统消息
	bool OnSocketSubSystemMessage(void* data, int dataSize);
	//动作消息
	bool OnSocketSubActionMessage(void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//设置内核接口
	virtual void SetClientKernelSink(IClientKernelSink*	pIClientKernelSink);

	virtual bool SendUserReady(void * data, word dataSize);
	//游戏消息,框架消息
	bool OnSocketMainGameFrame(int main,int sub, void* data, int dataSize);
	//用户聊天
	bool OnSocketSubUserTalk(void* data, int dataSize);
	//游戏状态
	bool OnSocketSubGameStatus(void* data, int dataSize);
	//游戏场景
	bool OnSocketSubGameScene(void* data, int dataSize);
	//旁观状态
	bool OnSocketSubLookonStatus(void* data, int dataSize);
	
	
	//获取状态
	byte GetGameStatus();
	//设置状态
	void SetGameStatus(byte cbGameStatus);
	//////////////////////////////////////////////////////////////////////////
	//比赛消息
	bool OnSocketMainMatch(int sub, void* data, int dataSize);

	void sendMacthFree();
	void sendExitMacth();
	//比赛费用
	bool OnSocketSubMatchFee(void* data, int dataSize);
	//参赛人数
	bool OnSocketSubMatchNum(void* data, int dataSize);
	bool OnSocketSubMatchInfo(void* data, int dataSize);
	bool OnSocketSubWaitTip(void* data, int dataSize);
	bool OnSocketSubMatchResult(void* data, int dataSize);
	bool OnSocketSubMatchStatus(void* data, int dataSize);
	bool OnSocketSubMatchGoldUpdate(void* data, int dataSize);
	bool OnSocketSubMatchEliminate(void* data, int dataSize);
	bool OnSocketSubMatchJoinResoult(void* data, int dataSize);


	//////////////////////////////////////////////////////////////////////////
	//私人场消息
	bool OnSocketMainPrivate(int sub, void* data, int dataSize);

	bool OnSocketSubPrivateInfo(void* data, int dataSize);
	bool OnSocketSubPrivateCreateSuceess(void* data, int dataSize);
	bool OnSocketSubPrivateRoomInfo(void* data, int dataSize);
	bool OnSocketSubPrivateEnd(void* data, int dataSize);
	bool OnSocketSubPrivateDismissInfo(void* data, int dataSize);
	
	//////////////////////////////////////////////////////////////////////////
	// 框架消息
public:
	//游戏已准备好
	virtual void OnGFGameReady();
	//游戏关闭
	virtual void OnGFGameClose(int iExitCode);

	void restSeverState();

	bool IsInGame();
	//////////////////////////////////////////////////////////////////////////
	// Socket消息
public:
	// 发送数据
	virtual bool GFSendData(int main, int sub, void* data, int size);

public:

	bool BackKey();

	//////////////////////////////////////////////////////////////////////////
	// 数据
	//////////////////////////////////////////////////////////////////////////
private:
	//辅助变量
	word	m_wReqTableID;						//请求桌子
	word	m_wReqChairID;						//请求位置
	word	mFindTableID;						//查找桌子
	bool	mIsGameReady;						//游戏是否准备好
	bool	mIsQuickSitDown;						//是否快速加入操作

	CMD_GR_Match_Fee	m_kMacthCost;
	//用户
	IClientUserItem*	m_pMeUserItem;
	CPlazaUserManager*	mUserManager;

	//房间属性
	enServiceStatus		mServiceStatus;
	tagGameKind			mGameKind;						//类型信息
	tagGameServer		mGameServer;					//房间信息
	tagUserAttribute	mUserAttribute;					//用户属性
	tagServerAttribute	mServerAttribute;				//房间属性

	//配置参数
	CParameterGame*		mParameterGame;					//游戏配置
	CParameterServer*	mParameterServer;				//房间配置
	
	//桌子
	CTableViewFrame		m_TableFrame;					//桌子框架

	//接口
	IServerItemSink*	mIServerItemSink;				//房间接口
	IServerMatchSink*	mIServerMatchSink;				//比赛接口
	IServerPrivateSink*	mIServerPrivateSink;			//私人场接口
	IStringMessageSink*	mIStringMessageSink;			//信息接口

	IClientKernelSink*	mIClientKernelSink;				//内核接口
	byte				mGameStatus;									//游戏状态
	// 网络连接
	ISocketEngine*		mSocketEngine;
};
#endif // _CServerItem_H_