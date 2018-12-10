#include "CGUserInGameServerInfo.h"


CGUserInGameServerInfo::CGUserInGameServerInfo(const char* url, int port)
: CCallMission("CGUserInGameServerInfo",url, port)
,m_pInGameServerInfoSink(NULL)
{
	addNetCall(CC_CALLBACK_2(CGUserInGameServerInfo::Net_InGameServerID,this),SUB_GP_QUERY_INGAME_SEVERID);
}

CGUserInGameServerInfo::~CGUserInGameServerInfo()
{

}
// 设置回调接口
void CGUserInGameServerInfo::setMissionSink(ICGUserInGameServerInfoSink* pSink)
{
	m_pInGameServerInfoSink = pSink;
}

void CGUserInGameServerInfo::PerformInGameServerID(int iUserID)
{
	addLinkCallFun(CC_CALLBACK_0(CGUserInGameServerInfo::CB_InGameServerID,this,iUserID));
	start();
}
void CGUserInGameServerInfo::CB_InGameServerID(int iUserID)
{
	CMD_GP_UserInGameServerID kNetInfo;
	kNetInfo.dwUserID = iUserID;
	send(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INGAME_SEVERID,&kNetInfo,sizeof(kNetInfo));
}
void CGUserInGameServerInfo::Net_InGameServerID(void* data, int dataSize)
{
	ASSERT(dataSize==sizeof(CMD_GP_InGameSeverID));
	if (dataSize!=sizeof(CMD_GP_InGameSeverID)) return ;
	
	CMD_GP_InGameSeverID* pNetInfo = (CMD_GP_InGameSeverID*)data;

	if (m_pInGameServerInfoSink)
	{
		m_pInGameServerInfoSink->onUserInGameServerID(pNetInfo);
	}
}