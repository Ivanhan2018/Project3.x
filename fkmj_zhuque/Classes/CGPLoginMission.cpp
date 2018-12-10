#include "cocos2d.h"
#include "CGPLoginMission.h"
#include "Convert.h"
#include "JniFun.h"

#define MISSION_LOGIN_ACCOUNT	1
#define MISSION_LOGIN_GAMEID	2
#define MISSION_REGISTER		3
#define MISSION_UPDATE_INFO		4
#define MISSION_SERVER_INFO		5
#define MISSION_LOGIN_VISITOR	6	


CGPLoginMission::CGPLoginMission(const char* url, int port)
	: CSocketMission(url, port)
{
	mMissionType = 0;
	mIGPLoginMissionSink = 0;

}


// 设置回调接口
void CGPLoginMission::setMissionSink(IGPLoginMissionSink* pIGPLoginMissionSink)
{
	mIGPLoginMissionSink = pIGPLoginMissionSink;
}

// 账号登陆
void CGPLoginMission::loginAccount(const CMD_GP_LogonAccounts& LoginAccount)
{
	memcpy(&mLoginAccount, &LoginAccount, sizeof(mLoginAccount));
	mMissionType = MISSION_LOGIN_ACCOUNT;
	start();
}
// 游客登录
void CGPLoginMission::loginVisitor(const CMD_GP_VisitorLogon& VisitorAccount)
{
	memcpy(&mVisitorAccount, &VisitorAccount, sizeof(VisitorAccount));
	mMissionType = MISSION_LOGIN_VISITOR;
	start();
}

// I D登陆
void CGPLoginMission::loginGameID(const CMD_GP_LogonByUserID& LoginGameID)
{
	memcpy(&mLoginGameID, &LoginGameID, sizeof(mLoginGameID));
	mMissionType = MISSION_LOGIN_GAMEID;
	start();
}

void CGPLoginMission::registerServer(const CMD_GP_RegisterAccounts& RegisterAccount)
{
	memcpy(&mRegisterAccount, &RegisterAccount, sizeof(mRegisterAccount));
	mMissionType = MISSION_REGISTER;
	start();
}

void CGPLoginMission::updateOnlineInfo()
{
	mMissionType = MISSION_UPDATE_INFO;
	start();
}

//更新类型
bool CGPLoginMission::updateServerInfo(uint16 kind)
{
	KIND_ITER it = std::find(mKindList.begin(), mKindList.end(), kind);

	if (it != mKindList.end())
	{
		return false;
	}

	it = std::find(mKindWaitList.begin(), mKindWaitList.end(), kind);

	if (it != mKindWaitList.end())
	{
		return false;
	}

	mKindList.push_back(kind);
	mMissionType = MISSION_SERVER_INFO;
	start();

	return true;
}

// 登陆
bool CGPLoginMission::sendLoginVisitor(const CMD_GP_VisitorLogon& VisitorAccount)
{
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	strncpy(pGlobalUserData->szPassword,mVisitorAccount.szPassWord,countarray(pGlobalUserData->szPassword));

	//发送数据
	send(MDM_GP_LOGON, SUB_GP_LOGON_VISITOR, (void*)&mVisitorAccount, sizeof(mVisitorAccount));
	return true;
}
//发送登陆信息
bool CGPLoginMission::sendLoginAccount(const CMD_GP_LogonAccounts& LoginAccount)
{
	PLAZZ_PRINTF(("CGPLoginMission::sendLoginAccount\n"));
	//保存密码
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	//strncpy(pGlobalUserData->szPassword,LoginAccount.szPassword, countarray(pGlobalUserData->szPassword));

	//发送数据
	send(MDM_GP_LOGON, SUB_GP_LOGON_ACCOUNTS, (void*)&LoginAccount, sizeof(LoginAccount));
	return true;
}

//发送登陆信息
bool CGPLoginMission::sendLoginGameID(const CMD_GP_LogonByUserID& LoginGameID)
{
	return true;
}

//发送注册信息
bool CGPLoginMission::sendRegisterPacket(const CMD_GP_RegisterAccounts& RegisterAccount)
{
	PLAZZ_PRINTF(("CGPLoginMission::sendRegisterPacket\n"));

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	strncpy(pGlobalUserData->szPassword,RegisterAccount.szLogonPass, countarray(pGlobalUserData->szPassword));

	//发送数据
	send(MDM_GP_LOGON, SUB_GP_REGISTER_ACCOUNTS, (void*)&RegisterAccount, sizeof(RegisterAccount));
	return true;
}


// 更新人数
bool CGPLoginMission::sendUpdateOnlineInfoPacket()
{
	return true;

}

// 更新类型房间列表
void CGPLoginMission::sendUpdateServerInfo()
{
}

void CGPLoginMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF(("CGPLoginMission::onEventTCPSocketLink\n"));

	switch (mMissionType)
	{
		// 登陆
	case MISSION_LOGIN_ACCOUNT:
		sendLoginAccount(mLoginAccount);
		break;
	case MISSION_LOGIN_VISITOR:
		sendLoginVisitor(mVisitorAccount);
		break;
		// 登陆
	case MISSION_LOGIN_GAMEID:
		sendLoginGameID(mLoginGameID);
		break;
		// 注册
	case MISSION_REGISTER:
		sendRegisterPacket(mRegisterAccount);
		break;
		// 更新人数
	case MISSION_UPDATE_INFO:
		sendUpdateOnlineInfoPacket();
		break;
		// 更新类型房间列表
	case MISSION_SERVER_INFO:
		sendUpdateServerInfo();
		break;
	}
}

void CGPLoginMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF(("CGPLoginMission::onEventTCPSocketShut\n"));
}

void CGPLoginMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF(("CGPLoginMission::onEventTCPSocketError code[%d]\n"), errorCode);
	if (mIGPLoginMissionSink)
		mIGPLoginMissionSink->onGPError(errorCode);
}

bool CGPLoginMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF(("onEventTCPSocketRead main:%d sub:%d size:%d\n"), main, sub, size);
	switch (main)
	{
	//case SUB_MB_LOGON_GAMEID:	return onSocketMainLogon(sub, data, size);
	case MDM_GP_LOGON:			return onSocketMainLogon(sub, data, size);
	case MDM_GP_SERVER_LIST:	return onSocketMainServerList(sub, data, size);
	default:
		break;
	}
	CCAssert(false,"");
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 登陆信息
bool CGPLoginMission::onSocketMainLogon(int sub, void* data, int size)
{
	switch (sub)
	{
		//登录成功
	case SUB_GP_LOGON_SUCCESS:		return onSocketSubLogonSuccess(data, size);//303
		//登录失败
	case SUB_GP_LOGON_FAILURE:		return onSocketSubLogonFailure(data, size);
 		//登录失败
 	case SUB_GP_VALIDATE_MBCARD:	return onSocketSubLogonValidateMBCard(data, size);
 		//升级提示
 	case SUB_GP_UPDATE_NOTIFY:		return onSocketSubUpdateNotify(data, size);
 		//登录完成
	case SUB_GP_LOGON_FINISH:		return onSocketSubLogonFinish(data, size);
	case SUB_GP_GROWLEVEL_CONFIG:	return true;
	case SUB_GP_MATCH_SIGNUPINFO:	return onSocketSubMacthSignupInfo(data, size);
	case SUB_GP_LOGON_USER_GRADE_INFO:return true;//1344
	}

	CCAssert(false,"");
	return false;
}

//登录成功
bool CGPLoginMission::onSocketSubLogonSuccess(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketSubLogonSuccess\n"));

	int isize = sizeof(CMD_GP_LogonSuccess);

	//登陆成功
	CMD_GP_LogonSuccess* pData = (CMD_GP_LogonSuccess*)data;
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	pGlobalUserData->lUserScore = pData->lUserScore;
	pGlobalUserData->lUserInsure = pData->lUserInsure;
	//保存信息
	pGlobalUserData->wFaceID = pData->wFaceID;
	pGlobalUserData->cbGender = pData->cbGender;
	pGlobalUserData->dwUserID =pData->dwUserID;
	pGlobalUserData->dwGameID = pData->dwGameID;
	pGlobalUserData->dwSpreaderID = pData->dwLockServerID;
	pGlobalUserData->dwExperience =pData->dwExperience;
	pGlobalUserData->cbInsureEnabled = pData->cbMoorMachine;
	strcpy(pGlobalUserData->szNickName, utility::a_u8((char*)pData->szNickName).c_str());
	///strncpy(pGlobalUserData->szAccounts, ((char*)pData->szAccounts), countarray(pGlobalUserData->szAccounts));
	//金币信息
	pGlobalUserInfo->upPlayerInfo();

	if (mIGPLoginMissionSink)
		mIGPLoginMissionSink->onGPLoginSuccess();

	return true;
}

//登录失败
bool CGPLoginMission::onSocketSubLogonFailure(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketSubLogonFailure\n"));

	DBR_GP_LogonError* pNetInfo =(DBR_GP_LogonError*)data;
	//显示消息
	if (mIGPLoginMissionSink)
		mIGPLoginMissionSink->onGPLoginFailure(pNetInfo->lErrorCode,utility::a_u8(pNetInfo->szErrorDescribe).c_str());
	return true;

}

//升级提示
bool CGPLoginMission::onSocketSubUpdateNotify(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketSubUpdateNotify\n"));
	//效验参数
	CMD_GP_UpdateNotify* pNetInfo =(CMD_GP_UpdateNotify*)data;

	JniFun::showWebView(pNetInfo->szDownLoadHttp);
	
	return true;
}

//登录失败(密保卡)
bool CGPLoginMission::onSocketSubLogonValidateMBCard(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketSubLogonValidateMBCard\n"));
	return true;
}

//登录完成
bool CGPLoginMission::onSocketSubLogonFinish(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketSubLogonFinish\n"));

	stop();

	if (mIGPLoginMissionSink)
		mIGPLoginMissionSink->onGPLoginComplete();

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	pGlobalUserInfo->LoginSucess();

	return true;
}
bool CGPLoginMission::onSocketSubMacthSignupInfo(void* data, int size)
{
	//校验数据
	if(size%sizeof(word)!=0) return false;

	//提取数据			
	tagSignupMatchInfo * pSignupMatchInfo = (tagSignupMatchInfo *)data;
	word wSignupCount = size/sizeof(pSignupMatchInfo);

	//获取对象
	ASSERT(CServerListData::shared()!=NULL);
	CServerListData * pServerListData=CServerListData::shared();

	//查找房间
	CGameServerItem * pGameServerItem = NULL;

	//设置报名
	for(word i=0;i<wSignupCount;i++)
	{
		pGameServerItem = pServerListData->SearchGameServer(pSignupMatchInfo[i].wServerID);
		if(pGameServerItem!=NULL && pGameServerItem->m_GameMatch.dwMatchID==pSignupMatchInfo[i].dwMatchID)
		{
			pGameServerItem->m_bSignuped=true;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 列表信息
bool CGPLoginMission::onSocketMainServerList(int sub, void* data, int size)
{
 	switch (sub)
	{
		case SUB_GP_LIST_TYPE:		return onSocketListType(data, size);
		case SUB_GP_LIST_KIND:		return onSocketListKind(data, size);
		case SUB_GP_LIST_SERVER:	return onSocketListServer(data, size);
		case SUB_GP_LIST_CONFIG:	return onSocketServerOnline(data, size);
		case SUB_GP_LIST_FINISH:	return onSocketListFinish(data, size);
		case SUB_GR_KINE_ONLINE:	return onSocketKindOnline(data, size);
		case SUB_GR_SERVER_ONLINE:	return onSocketServerOnline(data, size);
 		case SUB_GR_ONLINE_FINISH:	return onSocketOnlineFinish(data, size);
		case SUB_GP_LIST_NODE:	    return true;//2400
 		//case SUB_GP_LIST_PAGE:	    return true;
	}

	return false;
}

//种类信息
bool CGPLoginMission::onSocketListType(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketListType\n"));
	//QY
	return true;
}

//类型信息
bool CGPLoginMission::onSocketListKind(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketListKind\n"));
	////更新数据
	int itemSize = sizeof(tagGameKind);

	ASSERT(size%itemSize==0);
	if (size%itemSize!=0) return false;
	char* pNetInfo = (char*)data;
	int iItemCount=size/itemSize;
	CServerListData * pServerListData=CServerListData::shared();
	for (int i=0;i<iItemCount;i++)
	{
		tagGameKind* pGameKind = (tagGameKind*)&(pNetInfo[i*itemSize]);
		pServerListData->InsertGameKind(pGameKind);

	}
	return true;
}

// 房间列表
bool CGPLoginMission::onSocketListServer(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketListServer\n"));
	////更新数据
	int itemSize = sizeof(tagGameServer);

	ASSERT(size%itemSize==0);
	if (size%itemSize!=0) return false;
	char* pNetInfo = (char*)data;
	int iItemCount=size/itemSize;
	CServerListData * pServerListData=CServerListData::shared();
	for (int i=0;i<iItemCount;i++)
	{
		tagGameServer* pGameServer = (tagGameServer*)&(pNetInfo[i*itemSize]);
		pServerListData->InsertGameServer(pGameServer);
		
	}
	return true;
}
//比赛列表
bool CGPLoginMission::onSocketListMatch(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketListMatch\n"));
	int itemSize = sizeof(tagGameMatch);

	ASSERT(size%itemSize==0);
	if (size%itemSize!=0) return false;

	//变量定义
	word wItemCount=size/sizeof(tagGameMatch);
	tagGameMatch * pGameMatch=(tagGameMatch *)(data);

	//获取对象
	CGameServerItem * pGameServerItem=NULL;
	CServerListData * pServerListData=CServerListData::shared();

	//更新数据
	for (word i=0;i<wItemCount;i++)
	{
		pGameServerItem = pServerListData->SearchGameServer(pGameMatch->wServerID);
		if(pGameServerItem!=NULL)
		{
			memcpy(&pGameServerItem->m_GameMatch,pGameMatch++,sizeof(pGameServerItem->m_GameMatch));
		}
	}

	return true;

}

// 列表完成
bool CGPLoginMission::onSocketListFinish(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketListFinish\n"));

	ASSERT(CServerListData::shared()!=0);
	if (CServerListData::shared()!=0) 
	{
		CServerListData::shared()->OnEventListFinish();
	}
	return true;
}
//列表配置
bool CGPLoginMission::onSocketListConfig(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketListConfig\n"));

	CMD_GP_ListConfig* pListConfig = (CMD_GP_ListConfig*)data;
	return true;
}


//////////////////////////////////////////////////////////////////////////
// 在线更新
//////////////////////////////////////////////////////////////////////////
//类型在线
bool CGPLoginMission::onSocketKindOnline(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketKindOnline\n"));

	//变量定义
	
	return true;

}

//房间在线
bool CGPLoginMission::onSocketServerOnline(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketServerOnline\n"));

	//变量定义
	BYTE bShowOnLineCount=*(BYTE*)data;				//显示人数

	return true;

}

// 在线更新完成
bool CGPLoginMission::onSocketOnlineFinish(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketOnlineFinish\n"));

	return true;
}

//////////////////////////////////////////////////////////////////////////

//视频配置
bool CGPLoginMission::onSocketVideoOption(void* data, int size)
{
	PLAZZ_PRINTF(("CGPLoginMission::onSocketVideoOption\n"));

	PLAZZ_PRINTF("onSocketVideoOption");
	return true;
}
