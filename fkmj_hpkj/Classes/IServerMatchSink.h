#pragma once

#include "IClientUserItem.h"


class IServerMatchSink
{
public:
	virtual ~IServerMatchSink(){};

	virtual void OnSocketSubMatchFee(CMD_GR_Match_Fee* pNetInfo) = 0;
	virtual void OnSocketSubMatchNum(CMD_GR_Match_Num* pNetInfo) = 0;
	virtual void OnSocketSubMatchInfo(CMD_GR_Match_Info* pNetInfo) = 0;
	virtual void OnSocketSubMatchWaitTip(bool bShow,CMD_GR_Match_Wait_Tip* pNetInfo) = 0;
	virtual void OnSocketSubMatchResult(CMD_GR_MatchResult* pNetInfo) = 0;
	virtual void OnSocketSubMatchStatus(byte pNetInfo) = 0;
	virtual void OnSocketSubMatchGoldUpdate(CMD_GR_MatchGoldUpdate* pNetInfo) = 0;
	virtual void OnSocketSubMatchEliminate() = 0;
	virtual void OnSocketSubMatchJoinResolt(bool bSucess) = 0;
};
