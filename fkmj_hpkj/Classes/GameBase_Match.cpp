#include "GameBase.h"

#include "GameLib.h"


//框架事件
//系统滚动消息
bool GameBase::OnGFTableMessage(const char* szMessage)
{

	return true;
}
//比赛信息
bool GameBase::OnGFMatchInfo(tagMatchInfo* pMatchInfo)
{

	return true;
}
//比赛等待提示
bool GameBase::OnGFMatchWaitTips(tagMatchWaitTip* pMatchWaitTip)
{
	return true;
}
//比赛结果
bool GameBase::OnGFMatchResult(tagMatchResult* pMatchResult)
{

	return true;
}

