#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"
#include "CCallMission.h"

//////////////////////////////////////////////////////////////////////////

class ICGMatchMissionSink
{
public:
	virtual ~ICGMatchMissionSink(){}
	virtual void onSignUpResoult(tagGameMatch&	pGameMatchInfo,bool bSucess,bool bSignUp,const char* pStr ){}
	virtual void onMatchAwardList(CMD_GR_MatchAwardList* pList){}
};

//////////////////////////////////////////////////////////////////////////

class CGMatchMission
	: public CCallMission
{
	//任务标识
protected:
	bool									m_bSignup;							//报名任务	

	//任务变量
protected:
	tagGameMatch							m_pGameMatchInfo;					//比赛信息

	//函数定义
public:
	//构造函数
	CGMatchMission(const char* url, int port);
	//析构函数
	virtual ~CGMatchMission();

	void setMissionSink(ICGMatchMissionSink* pSink);

	//网络事件
public:
	//报名比赛
	void PerformSignupMatch(int iGamerServerID);
	//取消报名
	void PerformUnSignupMatch(int iGamerServerID);

	void CB_SignupMatch();

	//取消报名
	void Net_SignupResult(void* data, int dataSize);

	//功能函数
public:
	//取消报名
	void PerformMatchAward(int iGamerServerID);
	void CB_MatchAward();
	void Net_MatchAward(void* data, int dataSize);
private:
	// 回调
	ICGMatchMissionSink* m_pMatchSink;
}; 
