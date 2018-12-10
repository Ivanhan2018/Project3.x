#ifndef _IClientKernelSink_H_
#define _IClientKernelSink_H_

#include "IClientUserItem.h"

class IClientKernelSink
{
public:
	virtual ~IClientKernelSink(){};

	//控制接口
public:
	//启动游戏
	virtual void clearInfo()=0;
	//启动游戏
	virtual bool SetupGameClient()=0;
	//重置游戏
	virtual void ResetGameClient()=0;
	//关闭游戏
	virtual void CloseGameClient()=0;
	//是否在游戏中
	virtual bool IsInGame() = 0;

	//框架事件
public:
	//系统滚动消息
	virtual bool OnGFTableMessage(const char* szMessage)=0;
	//比赛信息
	virtual bool OnGFMatchInfo(tagMatchInfo* pMatchInfo)=0;
	//比赛等待提示
	virtual bool OnGFMatchWaitTips(tagMatchWaitTip* pMatchWaitTip)=0;
	//比赛结果
	virtual bool OnGFMatchResult(tagMatchResult* pMatchResult)=0;

	//游戏事件
public:
	//旁观消息
	virtual bool OnEventLookonMode(void* data, int dataSize)=0;
	//场景消息
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)=0;
	//场景消息
	virtual bool OnEventGameMessage(int sub, void* data, int dataSize)=0;

	//用户事件
public:
	//用户进入
	virtual void OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//用户离开
	virtual void OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//用户积分
	virtual void OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//用户状态
	virtual void OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//用户属性
	virtual void OnEventUserAttrib(IClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//用户头像
	virtual void OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser)=0;

	//私人房
public:
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo) = 0;
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo) = 0;
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo) = 0;

public:
	virtual bool RevTalk(void* data,int dataSize) = 0;

	virtual bool BackKey() = 0;
	virtual void OnGameServerItem(CGameServerItem* pGameServerItem)=0;
};

#endif // _IClientKernelSink_H_