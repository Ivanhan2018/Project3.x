#include "CServerItem.h"
//登录消息
bool CServerItem::OnSocketMainLogon(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//登录成功
	case SUB_GR_LOGON_SUCCESS:	return OnSocketSubLogonSuccess(data,dataSize);
		//登录失败
	case SUB_GR_LOGON_FAILURE:	return OnSocketSubLogonFailure(data,dataSize);
		//登录完成
	case SUB_GR_LOGON_FINISH:	return OnSocketSubLogonFinish(data,dataSize);
		//更新提示
	case SUB_GR_UPDATE_NOTIFY:	return OnSocketSubUpdateNotify(data,dataSize);
	}

	return true;
}


//登录成功
bool CServerItem::OnSocketSubLogonSuccess(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubLogonSuccess\n"));

	mIsQuickSitDown = false;

	//设置状态
	SetServiceStatus(ServiceStatus_RecvInfo);

	if (mIServerItemSink)
		mIServerItemSink->OnGRLogonSuccess(data,dataSize);

	return true;
}

//登录失败
bool CServerItem::OnSocketSubLogonFailure(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubLogonFailure\n"));
	CMD_GR_LogonError* pGameServer = (CMD_GR_LogonError*)data;
	if (mIStringMessageSink)
	{
		mIStringMessageSink->InsertSystemString(pGameServer->szErrorDescribe);
	}
	
	//关闭连接
	IntermitConnect(true);
	return true;
}

//登录完成
bool CServerItem::OnSocketSubLogonFinish(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubLogonFinish\n"));

	//设置状态
	SetServiceStatus(ServiceStatus_ServiceIng);
		
	mUserAttribute.dwUserID=m_pMeUserItem->GetUserID();
	mUserAttribute.wChairID=INVALID_CHAIR;
	mUserAttribute.wTableID=INVALID_TABLE;


	if (mIServerItemSink)
		mIServerItemSink->OnGRLogonFinish();

	return true;
}


//更新提示
bool CServerItem::OnSocketSubUpdateNotify(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubUpdateNotify\n"));
	CMD_GR_UpdateNotify* pNetInfo = (CMD_GR_UpdateNotify*)data;
	
	IntermitConnect(true);

	if (mIServerItemSink)
		mIServerItemSink->OnGRUpdateNotify(0, pNetInfo->szDownLoadHttp);

	return true;

}

