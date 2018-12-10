#include "CGPIndividualMission.h"
#include "IServerItem.h"



//////////////////////////////////////////////////////////////////////////

CGPIndividualMission::CGPIndividualMission(const char* url, int port)
: CCallMission("CGPIndividualMission",url, port)
, mMissionType(0)
{
	mIGPIndividualMissionSink = 0;
	addNetCall(CC_CALLBACK_2(CGPIndividualMission::onSubUserAccountInfo,this),SUB_GP_QUERY_ACCOUNTINFO);
	addNetCall(CC_CALLBACK_2(CGPIndividualMission::onSubUserIndividual,this),SUB_GP_USER_INDIVIDUAL);
	addNetCall(CC_CALLBACK_2(CGPIndividualMission::onSubSpreaderResoult,this),SUB_GP_SPREADER_RESOULT);
	addNetCall(CC_CALLBACK_2(CGPIndividualMission::onSubOperateSuccess,this),SUB_GP_OPERATE_SUCCESS);
	addNetCall(CC_CALLBACK_2(CGPIndividualMission::onSubOperateFailure,this),SUB_GP_OPERATE_FAILURE);
}

// 设置回调接口
void CGPIndividualMission::setMissionSink(IGPIndividualMissionSink* pIGPIndividualMissionSink)
{
	mIGPIndividualMissionSink = pIGPIndividualMissionSink;
}

// 查询个人资料
void CGPIndividualMission::query(int iAccountID)
{
	addLinkCallFun(CC_CALLBACK_0(CGPIndividualMission::CB_query,this,iAccountID));

	start();
}
void CGPIndividualMission::CB_query(int iAccountID)
{
	mMissionType = MISSION_INDIVIDUAL_QUERY;
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CMD_GP_QueryIndividual QueryIndividual;
	zeromemory(&QueryIndividual,sizeof(QueryIndividual));
	QueryIndividual.dwUserID = iAccountID;

	send(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INDIVIDUAL,&QueryIndividual,sizeof(QueryIndividual));
}
void CGPIndividualMission::queryAccountInfo(int iAccountID)
{
	if (iAccountID < 0)
	{
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		iAccountID =  pGlobalUserData->dwUserID;
	}

	addLinkCallFun(CC_CALLBACK_0(CGPIndividualMission::CB_queryAccountInfo,this,iAccountID));

	start();
}
void CGPIndividualMission::CB_queryAccountInfo(int iAccountID)
{
	mMissionType = MISSION_INDIVIDUAL_Account;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CMD_GP_QueryAccountInfo QueryIndividual;
	zeromemory(&QueryIndividual,sizeof(QueryIndividual));

	QueryIndividual.dwUserID=iAccountID;
	send(MDM_GP_USER_SERVICE,SUB_GP_QUERY_ACCOUNTINFO,&QueryIndividual,sizeof(QueryIndividual));
	
}
void CGPIndividualMission::sendModifyInfo()
{

	//变量定义
	byte cbBuffer[SIZE_PACK_DATA];
	zeromemory(cbBuffer,sizeof(cbBuffer));

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_ModifyIndividual),sizeof(cbBuffer)-sizeof(CMD_GP_ModifyIndividual));

	//设置变量
	pModifyIndividual->cbGender=mModifyIndividual.cbGender;
	pModifyIndividual->dwUserID=pGlobalUserData->dwUserID;
	strncpy(pModifyIndividual->szPassword,pGlobalUserData->szPassword,countarray(pModifyIndividual->szPassword));

	//用户昵称
	if (mModifyIndividual.szNickName[0]!=0 && countarray(mModifyIndividual.szNickName) >= 6)
	{
		PLAZZ_PRINTF(("szNickName:%s \n"), mModifyIndividual.szNickName);
		SendPacket.AddPacket(mModifyIndividual.szNickName,DTP_GP_UI_NICKNAME);
	}

	//个性签名
	if (mModifyIndividual.szUnderWrite[0]!=0)
	{
		PLAZZ_PRINTF(("szUnderWrite:%s \n"), mModifyIndividual.szUnderWrite);
		SendPacket.AddPacket(mModifyIndividual.szUnderWrite,DTP_GP_UI_UNDER_WRITE);
	}

	//用户备注
	if (mModifyIndividual.szUserNote[0]!=0)
	{
		SendPacket.AddPacket(mModifyIndividual.szUserNote,DTP_GP_UI_USER_NOTE);
	}

	//真实名字
	if (mModifyIndividual.szCompellation[0]!=0) 
	{
		SendPacket.AddPacket(mModifyIndividual.szCompellation,DTP_GP_UI_COMPELLATION);
	}

	//固定号码
	if (mModifyIndividual.szSeatPhone[0]!=0) 
	{
		SendPacket.AddPacket(mModifyIndividual.szSeatPhone,DTP_GP_UI_SEAT_PHONE);
	}

	//手机号码
	if (mModifyIndividual.szMobilePhone[0]!=0)
	{
		SendPacket.AddPacket(mModifyIndividual.szMobilePhone,DTP_GP_UI_MOBILE_PHONE);
	}

	//Q Q 号码
	if (mModifyIndividual.szQQ[0]!=0) 
	{
		SendPacket.AddPacket(mModifyIndividual.szQQ,DTP_GP_UI_QQ);
	}

	//电子邮件
	if (mModifyIndividual.szEMail[0]!=0) 
	{
		SendPacket.AddPacket(mModifyIndividual.szEMail,DTP_GP_UI_EMAIL);
	}

	//详细地址
	if (mModifyIndividual.szDwellingPlace[0]!=0) 
	{
		SendPacket.AddPacket(mModifyIndividual.szDwellingPlace,DTP_GP_UI_DWELLING_PLACE);
	}

	if (mModifyIndividual.szHeadHttp[0]!=0) 
	{
		SendPacket.AddPacket(mModifyIndividual.szHeadHttp,DTP_GP_UI_HEAD_HTTP);
	}

	if (mModifyIndividual.szUserChannel[0]!=0) 
	{
		SendPacket.AddPacket(mModifyIndividual.szUserChannel,DTP_GP_UI_CHANNEL);
	}

	if (mModifyIndividual.szUserGps[0]!=0) 
	{
		SendPacket.AddPacket(mModifyIndividual.szUserGps,DTP_GP_UI_GPS);
	}
	//发送数据
	word wSendSize=sizeof(CMD_GP_ModifyIndividual)+SendPacket.GetDataSize();
	send(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INDIVIDUAL,cbBuffer,wSendSize);
}
void CGPIndividualMission::modifyName(std::string kName)
{
	addLinkCallFun(CC_CALLBACK_0(CGPIndividualMission::CB_modifyName,this,kName));
	start();
}
void CGPIndividualMission::CB_modifyName(std::string kName)
{
	mMissionType = MISSION_INDIVIDUAL_MODIFY;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	mModifyIndividual.cbGender = pGlobalUserData->cbGender;
	strncpy(mModifyIndividual.szNickName,kName.c_str(),kName.size());
	sendModifyInfo();
}

void CGPIndividualMission::modifyGender(byte gender)
{
	addLinkCallFun(CC_CALLBACK_0(CGPIndividualMission::CB_modifyGender,this,gender));
	start();
}
void CGPIndividualMission::CB_modifyGender(byte gender)
{
	mMissionType = MISSION_INDIVIDUAL_MODIFY;

	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	mModifyIndividual.cbGender = gender;
	sendModifyInfo();
}
void CGPIndividualMission::modifySpreader(std::string kSpreaderID)
{
	addLinkCallFun(CC_CALLBACK_0(CGPIndividualMission::CB_modifySpreader,this,kSpreaderID));
	start();
}
void CGPIndividualMission::CB_modifySpreader(std::string kSpreaderID)
{
	mMissionType = MISSION_INDIVIDUAL_SPREADER;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CMD_GP_ModifySpreader kNetInfo;
	zeromemory(&kNetInfo,sizeof(kNetInfo));

	//设置变量
	kNetInfo.dwUserID=pGlobalUserData->dwUserID;
	strncpy(kNetInfo.szPassword,pGlobalUserData->szPassword,countarray(kNetInfo.szPassword));
	strncpy(kNetInfo.szSpreader,kSpreaderID.c_str(),countarray(kNetInfo.szSpreader));

	//发送数据
	send(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_SPREADER,&kNetInfo,sizeof(kNetInfo));

}
void CGPIndividualMission::modifyHeadHttp(std::string kHttp)
{
	addLinkCallFun(CC_CALLBACK_0(CGPIndividualMission::CB_modifyHeadHttp,this,kHttp));
	start();
}

void CGPIndividualMission::CB_modifyHeadHttp(std::string kHttp)
{
	mMissionType = MISSION_INDIVIDUAL_HEAD_HTTP;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	mModifyIndividual.cbGender = pGlobalUserData->cbGender;
	strncpy(mModifyIndividual.szHeadHttp,kHttp.c_str(),kHttp.size());
	sendModifyInfo();
}
void CGPIndividualMission::modifyUserChannel(std::string szUserChannel)
{
	addLinkCallFun(CC_CALLBACK_0(CGPIndividualMission::CB_modifyUserChannel,this,szUserChannel));
	start();
}
void CGPIndividualMission::CB_modifyUserChannel(std::string szUserChannel)
{
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	mMissionType = MISSION_INDIVIDUAL_USER_CHANNEL;

	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	mModifyIndividual.cbGender = pGlobalUserData->cbGender;
	strncpy(mModifyIndividual.szUserChannel,szUserChannel.c_str(),szUserChannel.size());
	sendModifyInfo();
}

void CGPIndividualMission::modifyPhoneNumber( std::string kPhoneNumber )
{
	addLinkCallFun(CC_CALLBACK_0(CGPIndividualMission::CB_modifyPhoneNumber,this,kPhoneNumber));
	start();
}
void CGPIndividualMission::CB_modifyPhoneNumber(std::string kPhoneNumber)
{
	mMissionType = MISSION_INDIVIDUAL_MODIFY;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	mModifyIndividual.cbGender = pGlobalUserData->cbGender;
	strncpy(mModifyIndividual.szMobilePhone,kPhoneNumber.c_str(),kPhoneNumber.size());
	sendModifyInfo();
}

void CGPIndividualMission::modifyUserGps( std::string kUserGps )
{
	addLinkCallFun(CC_CALLBACK_0(CGPIndividualMission::CB_modifyUserGps,this,kUserGps));
	start();
}
void CGPIndividualMission::CB_modifyUserGps(std::string kUserGps)
{
	mMissionType = MISSION_INDIVIDUAL_MODIFY;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	memset(&mModifyIndividual, 0, sizeof(mModifyIndividual));
	mModifyIndividual.cbGender = pGlobalUserData->cbGender;
	strncpy(mModifyIndividual.szUserGps,kUserGps.c_str(),kUserGps.size());
	sendModifyInfo();
}
void CGPIndividualMission::modifyStarValue( int dwUserID,int dwStarValue)
{
	addLinkCallFun(CC_CALLBACK_0(CGPIndividualMission::CB_modifyStarValue,this,dwUserID,dwStarValue));
	start();
}
void CGPIndividualMission::CB_modifyStarValue( int dwUserID,int dwStarValue)
{
	CMD_GP_ModifyStarValue kNetInfo;
	kNetInfo.dwStarValue = dwStarValue;
	kNetInfo.dwUserID = dwUserID;
	send(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_STARVALUE,&kNetInfo,sizeof(kNetInfo));
}
// 个人信息
void CGPIndividualMission::onSubUserAccountInfo(void* data, int size)
{
	PLAZZ_PRINTF("CGPIndividualMission::onSubUserAccountInfo\n");

	//变量定义
	CMD_GP_UserAccountInfo * pAccountInfo=(CMD_GP_UserAccountInfo *)data;
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if (pAccountInfo->dwUserID == pGlobalUserInfo->getUserID())
	{
		pGlobalUserData->lUserScore = pAccountInfo->lUserScore;
		pGlobalUserData->lUserInsure = pAccountInfo->lUserInsure;
		//保存信息
		pGlobalUserData->wFaceID = pAccountInfo->wFaceID;
		pGlobalUserData->cbGender = pAccountInfo->cbGender;
		pGlobalUserData->dwUserID =pAccountInfo->dwUserID;
		pGlobalUserData->dwGameID = pAccountInfo->dwGameID;
		pGlobalUserData->dwSpreaderID = pAccountInfo->dwSpreaderID;
		pGlobalUserData->dwExperience =pAccountInfo->dwExperience;
		strcpy(pGlobalUserData->szNickName, utility::a_u8((char*)pAccountInfo->szNickName).c_str());
		strncpy(pGlobalUserData->szAccounts, ((char*)pAccountInfo->szAccounts), countarray(pGlobalUserData->szAccounts));
		strncpy(pGlobalUserData->szLogonIP, ((char*)pAccountInfo->szLogonIp), countarray(pGlobalUserData->szLogonIP));
		//金币信息
		pGlobalUserInfo->upPlayerInfo();
	}
	if (mIGPIndividualMissionSink)
	{
		mIGPIndividualMissionSink->onGPAccountInfo(pAccountInfo);
	}

	stop();

}
// 个人信息
void CGPIndividualMission::onSubUserIndividual(void* data, int size)
{
	PLAZZ_PRINTF("CGPIndividualMission::onSubUserIndividual\n");

	CMD_GP_UserIndividual* pModifyIndividual = (CMD_GP_UserIndividual*)data;


	void * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pModifyIndividual+1,size-sizeof(CMD_GP_UserIndividual));

	TAG_UserIndividual kTagInfo;
	kTagInfo.dwUserID = pModifyIndividual->dwUserID;
	kTagInfo.dwStarValue = pModifyIndividual->dwUserStarValue;
	kTagInfo.dwStarCout = pModifyIndividual->dwUserStarCout;
	bool bUpdate = false;
	//扩展信息
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_UI_HEAD_HTTP:	//联系地址
			{
				if (DataDescribe.wDataSize<=LEN_USER_NOTE)
				{
					bUpdate = true;
					kTagInfo.kHttp.assign((char*)pDataBuffer,DataDescribe.wDataSize);
					
				}
				break;
			}
		case DTP_GP_UI_IP:	//联系地址
			{
				if (DataDescribe.wDataSize<=LEN_NICKNAME)
				{
					bUpdate = true;
					kTagInfo.kIP.assign((char*)pDataBuffer,DataDescribe.wDataSize);

				}
				break;
			}
		case DTP_GP_UI_CHANNEL:	//联系地址
			{
				if (DataDescribe.wDataSize<=LEN_NICKNAME)
				{
					bUpdate = true;
					kTagInfo.kChannel.assign((char*)pDataBuffer,DataDescribe.wDataSize);

				}
				break;
			}
		case DTP_GP_UI_GPS:	//联系地址
			{
				if (DataDescribe.wDataSize<=LEN_NICKNAME)
				{
					bUpdate = true;
					kTagInfo.kGPS.assign((char*)pDataBuffer,DataDescribe.wDataSize);

				}
				break;
			}
		}
	}
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if (pModifyIndividual->dwUserID == pGlobalUserInfo->getUserID())
	{
		if (kTagInfo.kIP != "")
		{
			strncpy(pGlobalUserData->szLogonIP, (kTagInfo.kIP.c_str()), kTagInfo.kIP.size());
		}
		if (kTagInfo.kHttp != "")
		{
			strncpy(pGlobalUserData->szHeadHttp, (kTagInfo.kHttp.c_str()), kTagInfo.kHttp.size());
		}
		if (kTagInfo.kChannel != "")
		{
			strncpy(pGlobalUserData->szUserChannel, (kTagInfo.kChannel.c_str()), kTagInfo.kChannel.size());
		}
		//金币信息
		pGlobalUserInfo->upPlayerInfo();
	}

	if (bUpdate && mIGPIndividualMissionSink)
		mIGPIndividualMissionSink->onGPAccountInfoHttpIP(&kTagInfo);


	//通知
	if (mIGPIndividualMissionSink)
		mIGPIndividualMissionSink->onGPIndividualInfo(mMissionType);

	stop();

}
void CGPIndividualMission::onSubSpreaderResoult(void* data, int size)
{
	PLAZZ_PRINTF("CGPIndividualMission::onSubSpreaderResoult\n");
	//变量定义
	CMD_GP_SpreaderResoult * pOperateSuccess=(CMD_GP_SpreaderResoult *)data;

	//效验数据
	ASSERT(size>=(sizeof(CMD_GP_SpreaderResoult)-sizeof(pOperateSuccess->szDescribeString)));
	if (size<(sizeof(CMD_GP_SpreaderResoult)-sizeof(pOperateSuccess->szDescribeString))) return;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if (pOperateSuccess->lResultCode == 0)
	{
		pGlobalUserData->dwSpreaderID = pOperateSuccess->lScore;

		pGlobalUserInfo->upPlayerInfo();
	}
	//关闭连接

	//显示消息
	if (mIGPIndividualMissionSink)
		mIGPIndividualMissionSink->onGPIndividualSuccess(mMissionType, pOperateSuccess->szDescribeString);

	stop();
}

// 操作成功
void CGPIndividualMission::onSubOperateSuccess(void* data, int size)
{
	PLAZZ_PRINTF("CGPIndividualMission::onSubOperateSuccess\n");
	//变量定义
	CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)data;

	//效验数据
	ASSERT(size>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
	if (size<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) return;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	switch (mMissionType)
	{
		// 查询个人资料
	case MISSION_INDIVIDUAL_SPREADER:
		{
			pGlobalUserData->dwSpreaderID = 1;
			pGlobalUserInfo->upPlayerInfo();
			break;
		}
		// 查询个人资料
	case MISSION_INDIVIDUAL_QUERY:
		{
			break;
		}
		// 修改个人资料
	case MISSION_INDIVIDUAL_MODIFY:
	case MISSION_INDIVIDUAL_USER_CHANNEL:
		{
			tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

			//帐号资料
			pGlobalUserData->cbGender=mModifyIndividual.cbGender;

			//用户昵称
			if (mModifyIndividual.szNickName[0]!=0)
				strcpy(pGlobalUserData->szNickName,(mModifyIndividual.szNickName));

			//个性签名
			if (mModifyIndividual.szUnderWrite[0]!=0)
				strncpy(pGlobalUserData->szUnderWrite,mModifyIndividual.szUnderWrite,countarray(pGlobalUserData->szUnderWrite));

			//详细资料

			//用户备注
			if (mModifyIndividual.szUserNote[0]!=0)
				strncpy(pIndividualUserData->szUserNote,mModifyIndividual.szUserNote,countarray(pIndividualUserData->szUserNote));

			//真实名字
			if (mModifyIndividual.szCompellation[0]!=0) 
				strncpy(pIndividualUserData->szCompellation,mModifyIndividual.szCompellation,countarray(pIndividualUserData->szCompellation));

			//固定号码
			if (mModifyIndividual.szSeatPhone[0]!=0) 
				strncpy(pIndividualUserData->szSeatPhone,mModifyIndividual.szSeatPhone,countarray(pIndividualUserData->szSeatPhone));

			//手机号码
			if (mModifyIndividual.szMobilePhone[0]!=0)
				strncpy(pIndividualUserData->szMobilePhone,mModifyIndividual.szMobilePhone,countarray(pIndividualUserData->szMobilePhone));

			//Q Q 号码
			if (mModifyIndividual.szQQ[0]!=0) 
				strncpy(pIndividualUserData->szQQ,mModifyIndividual.szQQ,countarray(pIndividualUserData->szQQ));

			//电子邮件
			if (mModifyIndividual.szEMail[0]!=0) 
				strncpy(pIndividualUserData->szEMail,mModifyIndividual.szEMail,countarray(pIndividualUserData->szEMail));

			//详细地址
			if (mModifyIndividual.szHeadHttp[0]!=0) 
				strncpy(pGlobalUserData->szHeadHttp,mModifyIndividual.szHeadHttp,countarray(pGlobalUserData->szHeadHttp));

			//详细地址
			if (mModifyIndividual.szUserChannel[0]!=0) 
				strncpy(pGlobalUserData->szUserChannel,mModifyIndividual.szUserChannel,countarray(pGlobalUserData->szUserChannel));

			pGlobalUserInfo->upPlayerInfo();
			break;
		}
	}

	//显示消息
	if (mIGPIndividualMissionSink)
		mIGPIndividualMissionSink->onGPIndividualSuccess(mMissionType, pOperateSuccess->szDescribeString);

	stop();

}

// 操作失败
void CGPIndividualMission::onSubOperateFailure(void* data, int size)
{
	PLAZZ_PRINTF("CGPIndividualMission::onSubOperateFailure\n");
	//效验参数
	CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)data;
	ASSERT(size>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
	if (size<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) return;

	//显示消息
	if (mIGPIndividualMissionSink)
		mIGPIndividualMissionSink->onGPIndividualFailure(mMissionType, pOperateFailure->szDescribeString);

	stop();
}