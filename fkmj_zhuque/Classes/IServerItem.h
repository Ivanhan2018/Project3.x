#ifndef _IServerItem_H_
#define _IServerItem_H_

#include "IChatSink.h"
#include "IStringMessageSink.h"
#include "IServerItemSink.h"
#include "IServerPrivateSink.h"
#include "IClientKernelSink.h"

class IServerMatchSink;

//查找桌子
struct tagFindTable
{
	bool								bOneNull;							//一个空位
	bool								bTwoNull;							//两个空位
	bool								bAllNull;							//全空位置
	bool								bNotFull;							//不全满位
	bool								bFilterPass;						//过滤密码
	word								wBeginTableID;						//开始索引
	word								wResultTableID;						//结果桌子
	word								wResultChairID;						//结果椅子
};

class CGameServerItem;

//房间
class IServerItem
{
public:
	virtual ~IServerItem(){};

	//配置接口
public:
	//设置接口
	virtual bool SetServerItemSink(IServerItemSink* pIServerItemSink)=0;
	//设置接口
	virtual bool SetStringMessageSink(IStringMessageSink* pIStringMessageSink)=0;
	//设置接口
	virtual bool SetServerMatchSink(IServerMatchSink* pIServerMatchSink) = 0;
	//设置接口
	virtual bool SetServerPrivateSink(IServerPrivateSink* pIServerMatchSink) = 0;

	//连接接口
public:
	//连接服务器
	virtual bool ConnectServer(CGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr)=0;
	//连接断开
	virtual bool IntermitConnect(bool force)=0;

	//属性接口
public:
	//用户属性
	virtual const tagUserAttribute& GetUserAttribute() const = 0;
	//房间属性
	virtual const tagServerAttribute&  GetServerAttribute() const = 0;
	//服务状态
	virtual enServiceStatus GetServiceStatus()=0;
	//是否服务状态
	virtual bool IsService()=0;
	//设置状态
	virtual void SetServiceStatus(enServiceStatus ServiceStatus)=0;
	//自己状态
	virtual bool IsPlayingMySelf()=0;
	//用户接口
public:
	//自己位置
	virtual IClientUserItem * GetMeUserItem()=0;
	//游戏用户
	virtual IClientUserItem * GetTableUserItem(word wChariID)=0;
	//查找用户
	virtual IClientUserItem * SearchUserByUserID(dword dwUserID)=0;
	//查找用户
	virtual IClientUserItem * SearchUserByGameID(dword dwGameID)=0;
	//查找用户
	virtual IClientUserItem * SearchUserByNickName(const char* szNickName)=0;
	
	//网络接口
public:
	//发送函数
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID)=0;
	//发送函数
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID, void * pData, word wDataSize)=0;

	//动作处理
public:
	//执行快速加入
	virtual bool PerformQuickSitDown()=0;
	//执行旁观
	virtual bool PerformLookonAction(word wTableID, word wChairID)=0;
	//执行起立
	virtual bool PerformStandUpAction()=0;
	//执行坐下
	virtual bool PerformSitDownAction(word wTableID, word wChairID, bool bEfficacyPass, const char * psw = nullptr) = 0;
	//执行购买
	virtual bool PerformBuyProperty(byte cbRequestArea,const char* pszNickName, word wItemCount, word wPropertyIndex)=0;

	//桌子消息
public:
	//获得空桌子
	virtual int GetSpaceTableId()=0;
	//获得对应桌子的空椅子
	virtual int GetSpaceChairId(int tableId)=0;
	//获得总桌子数
	virtual int GetTotalTableCount()=0;
	//获得游戏服务
	//////////////////////////////////////////////////////////////////////////
	// 框架消息
public:
	//游戏已准备好
	virtual void OnGFGameReady()=0;
	//游戏关闭
	virtual void OnGFGameClose(int iExitCode)=0;

	//设置内核接口
	virtual void SetClientKernelSink(IClientKernelSink*	pIClientKernelSink) = 0;

	virtual bool SendUserReady(void * data, word dataSize) = 0;
	//////////////////////////////////////////////////////////////////////////
	// Socket消息
public:
	// 发送数据
	virtual bool GFSendData(int main, int sub, void* data, int size)=0;

public:
	static IServerItem* create();
	static void destory();
	static IServerItem* get();
};

#endif // _IServerItem_H_