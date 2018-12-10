#include "CServerItem.h"
//用户处理
bool CServerItem::OnSocketMainUser(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//请求坐下失败
	case SUB_GR_SIT_FAILED:			return OnSocketSubRequestFailure(data,dataSize);
		//用户进入
	case SUB_GR_USER_ENTER:			return OnSocketSubUserEnter(data,dataSize);//267
		//用户积分
	case SUB_GR_USER_SCORE:			return OnSocketSubUserScore(data,dataSize);
		//用户状态
	case SUB_GR_USER_STATUS:		return OnSocketSubUserStatus(data,dataSize);//12
		//排队人数
	case SUB_GR_QUEUE_COUNT:		return OnSocketSubQueueCount(data,dataSize);//4
		//用户聊天
	case SUB_GR_USER_CHAT:			return OnSocketSubUserChat(data,dataSize);
		//用户表情
	case SUB_GR_USER_EXPRESSION:	return OnSocketSubExpression(data,dataSize);
		//用户私聊
	case SUB_GR_WISPER_CHAT:		return OnSocketSubWisperUserChat(data,dataSize);
		//私聊表情
	case SUB_GR_WISPER_EXPRESSION:	return OnSocketSubWisperExpression(data,dataSize);
	
		//道具成功
	case SUB_GR_PROPERTY_SUCCESS:	return OnSocketSubPropertySuccess(data,dataSize);
		//道具失败
	case SUB_GR_PROPERTY_FAILURE:	return OnSocketSubPropertyFailure(data,dataSize);
		//道具效应
	case SUB_GR_PROPERTY_EFFECT:	return OnSocketSubPropertyEffect(data,dataSize);
		//礼物消息
	case SUB_GR_PROPERTY_MESSAGE:   return OnSocketSubPropertyMessage(data,dataSize);
		//喇叭消息
	case SUB_GR_PROPERTY_TRUMPET:	return OnSocketSubPropertyTrumpet(data,dataSize);
		//喜报消息
	case SUB_GR_GLAD_MESSAGE:		return OnSocketSubGladMessage(data,dataSize);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 桌子状态
//请求失败
bool CServerItem::OnSocketSubRequestFailure(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubRequestFailure\n"));
	//变量定义
	CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)data;

	//效验参数
	ASSERT(dataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	if (dataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	if (mIStringMessageSink)
	{
		mIStringMessageSink->InsertSystemString(pRequestFailure->szDescribeString);
	}
	return true;

}
//////////////////////////////////////////////////////////////////////////

//用户进入
bool CServerItem::OnSocketSubUserEnter(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubUserEnter\n"));

	int n = sizeof(tagUserInfoHead);
	//变量定义
	tagUserInfo kUserInfo;
	tagCustomFaceInfo CustomFaceInfo;
	zeromemory(&kUserInfo,sizeof(kUserInfo));
	zeromemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo =CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	ASSERT(CParameterGlobal::shared()!=0);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	tagUserInfoHead* pUserInfoHead = (tagUserInfoHead*)data;
	
	kUserInfo.dwUserID=pUserInfoHead->dwUserID;
	kUserInfo.wTableID=pUserInfoHead->wTableID;
	kUserInfo.wChairID=pUserInfoHead->wChairID;
	kUserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;
	kUserInfo.wFaceID=pUserInfoHead->wFaceID;
	kUserInfo.dwCustomID=pUserInfoHead->dwCustomID;
	kUserInfo.cbGender=pUserInfoHead->cbGender;
	kUserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
	kUserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	kUserInfo.dwGameID=pUserInfoHead->dwGameID;
	kUserInfo.dwGroupID=pUserInfoHead->dwGroupID;
	kUserInfo.lLoveLiness=pUserInfoHead->lLoveLiness;
	kUserInfo.lScore=pUserInfoHead->UserScoreInfo.lScore;
	kUserInfo.lGameGold=pUserInfoHead->UserScoreInfo.lGold;
	kUserInfo.lInsureScore=pUserInfoHead->UserScoreInfo.lInsure;
	kUserInfo.lWinCount=pUserInfoHead->UserScoreInfo.dwWinCount;
	kUserInfo.lLostCount=pUserInfoHead->UserScoreInfo.dwLostCount;
	kUserInfo.lDrawCount=pUserInfoHead->UserScoreInfo.dwDrawCount;
	kUserInfo.lFleeCount=pUserInfoHead->UserScoreInfo.dwFleeCount;
	kUserInfo.lExperience=pUserInfoHead->UserScoreInfo.dwExperience;

	//获取对象
	CServerListData * pServerListData=CServerListData::shared();

	void * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pUserInfoHead+1,dataSize-sizeof(tagUserInfoHead));

	//扩展信息
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GR_NICK_NAME:		//用户昵称
			{
				ASSERT(pDataBuffer!=NULL);
				ASSERT(DataDescribe.wDataSize<=sizeof(kUserInfo.szNickName));
				if (DataDescribe.wDataSize<=sizeof(kUserInfo.szNickName))
				{
					memcpy(&kUserInfo.szNickName,pDataBuffer,DataDescribe.wDataSize);
					kUserInfo.szNickName[CountArray(kUserInfo.szNickName)-1]=0;
				}
				break;
			}
		case DTP_GR_GROUP_NAME:		//用户社团
			{
				ASSERT(pDataBuffer!=NULL);
				ASSERT(DataDescribe.wDataSize<=sizeof(kUserInfo.szGroupName));
				if (DataDescribe.wDataSize<=sizeof(kUserInfo.szGroupName))
				{
					memcpy(&kUserInfo.szGroupName,pDataBuffer,DataDescribe.wDataSize);
					kUserInfo.szGroupName[CountArray(kUserInfo.szGroupName)-1]=0;
				}
				break;
			}
		case DTP_GR_UNDER_WRITE:	//个性签名
			{
				ASSERT(pDataBuffer!=NULL);
				ASSERT(DataDescribe.wDataSize<=sizeof(kUserInfo.szUnderWrite));
				if (DataDescribe.wDataSize<=sizeof(kUserInfo.szUnderWrite))
				{
					memcpy(kUserInfo.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
					kUserInfo.szUnderWrite[CountArray(kUserInfo.szUnderWrite)-1]=0;
				}
				break;
			}
		}
	}

	//激活用户
	IClientUserItem * pIClientUserItem = mUserManager->SearchUserByUserID(kUserInfo.dwUserID);
	pIClientUserItem = mUserManager->ActiveUserItem(kUserInfo, CustomFaceInfo);


	//人数更新
	if (pServerListData)
		pServerListData->SetServerOnLineCount(mServerAttribute.wServerID,mUserManager->GetActiveUserCount());

	return true;;
	
}

//用户积分
bool CServerItem::OnSocketSubUserScore(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubUserScore\n"));
	//效验参数
	ASSERT(dataSize==sizeof(CMD_GR_UserScore));
	if (dataSize<sizeof(CMD_GR_UserScore)) return false;

	//寻找用户
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)data;
	IClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(pUserScore->dwUserID);

	//用户判断
	if (pIClientUserItem==0) return true;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	mUserManager->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);


	return true;
}

//用户状态
bool CServerItem::OnSocketSubUserStatus(void* data, int dataSize)
{
	PLAZZ_PRINTF("CServerItem::OnSocketSubUserStatus\n");

	ASSERT(dataSize>=sizeof(CMD_GR_UserStatus));
	if (dataSize<sizeof(CMD_GR_UserStatus)) return false;

	//处理数据
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)data;

	IClientUserItem * pIClientUserItem=mUserManager->SearchUserByUserID(pUserStatus->dwUserID);
	if (pIClientUserItem==NULL) return true;

	tagUserStatus UserStatus;
	UserStatus.wTableID = pUserStatus->UserStatus.wTableID;
	UserStatus.wChairID = pUserStatus->UserStatus.wChairID;
	UserStatus.cbUserStatus = pUserStatus->UserStatus.cbUserStatus;

	//设置状态
	if (UserStatus.cbUserStatus==US_NULL) 
	{
		//删除用户
		mUserManager->DeleteUserItem(pIClientUserItem);

		//获取对象
		CServerListData * pServerListData=CServerListData::shared();

		//人数更新
		pServerListData->SetServerOnLineCount(mServerAttribute.wServerID,mUserManager->GetActiveUserCount());
	}
	else
	{
		//更新用户
		mUserManager->UpdateUserItemStatus(pIClientUserItem,&UserStatus);
	}

	return true;
}

//排队人数
bool CServerItem::OnSocketSubQueueCount(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubQueueCount\n"));

	CMD_GR_AutoQueue * pUserInvite=(CMD_GR_AutoQueue *)data;
	int m_QueueNum = pUserInvite->iAutoQueueNum;

	return true;
}

//////////////////////////////////////////////////////////////////////////
//聊天信息
//用户聊天
bool CServerItem::OnSocketSubUserChat(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubUserChat\n"));

	return true;

}

//用户表情
bool CServerItem::OnSocketSubExpression(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubExpression\n"));

	return true;

}

//用户私聊
bool CServerItem::OnSocketSubWisperUserChat(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubWisperUserChat\n"));

	return true;

}

//私聊表情
bool CServerItem::OnSocketSubWisperExpression(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubWisperExpression\n"));

	return true;

}

//////////////////////////////////////////////////////////////////////////
//道具信息
//道具成功
bool CServerItem::OnSocketSubPropertySuccess(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubPropertySuccess\n"));
	
	return true;

}

//道具失败
bool CServerItem::OnSocketSubPropertyFailure(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubPropertyFailure\n"));
	
	return true;
	
}

//道具效应
bool CServerItem::OnSocketSubPropertyEffect(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubPropertyEffect\n"));
	
	return true;
	
}

//道具消息
bool CServerItem::OnSocketSubPropertyMessage(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubPropertyMessage\n"));
	

	return true;
}

//道具喇叭
bool CServerItem::OnSocketSubPropertyTrumpet(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubPropertyTrumpet\n"));
	

	return true;
}

//喜报消息
bool CServerItem::OnSocketSubGladMessage(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubGladMessage\n"));

	return true;
}