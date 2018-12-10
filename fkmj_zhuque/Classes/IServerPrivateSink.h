#pragma once

#include "IClientUserItem.h"


class IServerPrivateSink
{
public:
	virtual ~IServerPrivateSink(){};

	virtual void OnSocketSubPrivateInfo(CMD_GR_Private_Info* pNetInfo) = 0;
	virtual void OnSocketSubPrivateCreateSuceess(CMD_GR_Create_Private_Sucess* pNetInfo) = 0;
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo) = 0;
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo) = 0;
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo) = 0;
};
