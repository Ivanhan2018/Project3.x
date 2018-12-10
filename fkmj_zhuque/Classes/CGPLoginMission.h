#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

#ifndef LOGIN_MISSION_SINK
#define LOGIN_MISSION_SINK
//////////////////////////////////////////////////////////////////////////
// 登陆提示
//////////////////////////////////////////////////////////////////////////
class IGPLoginMissionSink
{
public:
	virtual ~IGPLoginMissionSink(){}
	virtual void onGPLoginSuccess(){}
	virtual void onGPLoginComplete(){}
	virtual bool onGPUpdateNotify(byte cbMustUpdate, const char* szDescription){ return false; }
	virtual void onGPLoginFailure(unsigned int iErrorCode,const char* szDescription){}
	virtual void onGPError(int errorCode){}
};

#endif // LOGIN_MISSION_SINK

class CGPLoginMission
	: public CSocketMission
{
public:
	CGPLoginMission(const char* url, int port);

	//////////////////////////////////////////////////////////////////////////
	// 登陆
public:
	// 设置回调接口
	void setMissionSink(IGPLoginMissionSink* pIGPLoginMissionSink);
	// 账号登陆
	void loginAccount(const CMD_GP_LogonAccounts& LoginAccount);
	// 游客登陆
	void loginVisitor(const CMD_GP_VisitorLogon& VisitorAccount);
	// I D登陆
	void loginGameID(const CMD_GP_LogonByUserID& LoginGameID);
	// 注册
	void registerServer(const CMD_GP_RegisterAccounts& RegisterAccount);
	//更新人数
	void updateOnlineInfo();
	//更新类型
	bool updateServerInfo(uint16 kind);
private:
	//////////////////////////////////////////////////////////////////////////
	// 登陆
	bool sendLoginAccount(const CMD_GP_LogonAccounts& LoginAccount);
	// 登陆
	bool sendLoginVisitor(const CMD_GP_VisitorLogon& VisitorAccount); 
	// 登陆
	bool sendLoginGameID(const CMD_GP_LogonByUserID& LoginGameID);
	// 注册
	bool sendRegisterPacket(const CMD_GP_RegisterAccounts& RegisterAccount);
	// 更新人数
	bool sendUpdateOnlineInfoPacket();
	// 更新类型房间列表
	void sendUpdateServerInfo();

	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	//////////////////////////////////////////////////////////////////////////
	// 登陆信息
	bool onSocketMainLogon(int sub, void* data, int size);
	//登录成功
	bool onSocketSubLogonSuccess(void* data, int size);
	//登录失败
	bool onSocketSubLogonFailure(void* data, int size);
	//登录完成
	bool onSocketSubLogonFinish(void* data, int size);
	//登录完成
	bool onSocketSubMacthSignupInfo(void* data, int size);
	//升级提示
	bool onSocketSubUpdateNotify(void* data, int size);
	//登录失败(密保卡)
	bool onSocketSubLogonValidateMBCard(void* data, int size);
		
	//////////////////////////////////////////////////////////////////////////
	// 列表信息
	bool onSocketMainServerList(int sub, void* data, int size);
	//种类信息
	bool onSocketListType(void* data, int size);
	//类型信息
	bool onSocketListKind(void* data, int size);
	//房间信息
	bool onSocketListServer(void* data, int size);
	//比赛列表
	bool onSocketListMatch(void* data, int size);
	//列表完成
	bool onSocketListFinish(void* data, int size);
	//列表配置
	bool onSocketListConfig(void* data, int size);
	//视频配置
	bool onSocketVideoOption(void* data, int size);

	//类型在线
	bool onSocketKindOnline(void* data, int size);
	//房间在线
	bool onSocketServerOnline(void* data, int size);
	//在线完成更新完成
	bool onSocketOnlineFinish(void* data, int size);

private:
	// 回调
	IGPLoginMissionSink* mIGPLoginMissionSink;

	// 任务类型
	uint8 mMissionType;
	// 登陆数据
	CMD_GP_LogonAccounts	mLoginAccount;
	// 登陆数据
	CMD_GP_LogonByUserID	mLoginGameID;
	// 注册数据
	CMD_GP_RegisterAccounts mRegisterAccount;
	// 注册数据
	CMD_GP_VisitorLogon mVisitorAccount;
	
	// 更新类型
	typedef std::list<uint16>	KIND_LIST;
	typedef KIND_LIST::iterator KIND_ITER;

	KIND_LIST mKindList;
	KIND_LIST mKindWaitList;
	///< 允许的类型
};