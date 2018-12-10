#ifndef _IServerItemSink_H_
#define _IServerItemSink_H_

#include "IClientUserItem.h"

//服务状态
enum enServiceStatus
{
	ServiceStatus_Unknow,			//未知状态
	ServiceStatus_Entering,			//进入状态
	ServiceStatus_Validate,			//验证状态
	ServiceStatus_RecvInfo,			//读取状态
	ServiceStatus_ServiceIng,		//服务状态
	ServiceStatus_NetworkDown,		//中断状态
};

//游戏退出代码
enum enGameExitCode
{
	GameExitCode_Normal,			//正常退出
	GameExitCode_CreateFailed,		//创建失败
	GameExitCode_Timeout,			//定时到时
	GameExitCode_Shutdown,			//断开连接
};

//房间退出代码
enum enServerExitCode
{
	ServerExitCode_Normal,			//正常退出
	ServerExitCode_Shutdown,		//断开连接
};

class IServerItemSink
{
public:
	virtual ~IServerItemSink(){};

	//登陆信息
public:
	//请求失败
	virtual void onGRRequestFailure(const std::string& sDescribeString) = 0;
	//登陆成功
	virtual void OnGRLogonSuccess(void* data, int dataSize) = 0;
	//登陆失败
	virtual void OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString) = 0;
	//登陆完成
	virtual void OnGRLogonFinish() = 0;
	//更新通知
	virtual void OnGRUpdateNotify(byte cbMustUpdate, const std::string& szDownHttp)=0;

	//配置信息
public:
	//列表配置
	virtual void OnGRConfigColumn() = 0;
	//房间配置
	virtual void OnGRConfigServer() = 0;
	//道具配置
	virtual void OnGRConfigProperty() = 0;
	//玩家权限配置
	virtual void OnGRConfigUserRight() = 0;
	//配置完成
	virtual void OnGRConfigFinish() = 0;

	//用户信息
public:
	//用户进入
	virtual void OnGRUserEnter(IClientUserItem* pIClientUserItem)=0;
	//用户更新
	virtual void OnGRUserUpdate(IClientUserItem* pIClientUserItem) = 0;
	//用户删除
	virtual void OnGRUserDelete(IClientUserItem* pIClientUserItem)=0;

	//框架消息
public:
	//用户邀请
	virtual void OnGFUserInvite(const std::string& szMessage)=0;
	//用户邀请失败
	virtual void OnGFUserInviteFailure(const std::string& szMessage)=0;
	//房间退出
	virtual void OnGFServerClose(const std::string& szMessage)=0;
	//创建游戏内核
	virtual bool CreateKernel()=0;
	//启动游戏
	virtual bool StartGame()=0;
	//心跳包
	virtual void HeartTick()=0;
	//网络错误
	virtual void onEventTCPSocketError(int errorCode) = 0;
};

#endif // _IServerItemSink_H_