#include "CGPPersonalInfoMission.h"

CGPPersonalInfoMission::CGPPersonalInfoMission(const char* url, int port)
: CSocketMission(url, port)
{
	mMissionType = MISSION_LEVEL;
	mIGPPersonalInfoMissionSink = 0;
	need_request_lvInfo = true;
}

// 设置回调接口
void CGPPersonalInfoMission::setMissionSink(IGPPersonalInfoMissionSink* pIGPPersonalInfoMissionSink)
{
	mIGPPersonalInfoMissionSink = pIGPPersonalInfoMissionSink;
}

void CGPPersonalInfoMission::requestLevelInfo()
{
	if (need_request_lvInfo){
		mMissionType = MISSION_LEVEL;
		start();
	}
	else{
		if (mIGPPersonalInfoMissionSink){

			CGlobalUserInfo *pCGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGrowLevelParameter * param_ = pCGlobalUserInfo->GetUserGrowLevelParameter();

			mIGPPersonalInfoMissionSink->onQueryLevelUpTipsSuccess(param_->wCurrLevelID, param_->dwExperience, param_->dwUpgradeExperience, param_->lUpgradeRewardGold, param_->lUpgradeRewardIngot);
		}
	}	
}


void CGPPersonalInfoMission::requesttPersionInfo()
{
	mMissionType = MISSION_PERSON_INFO;
	start();
}


void CGPPersonalInfoMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPPasswordMission::onEventTCPSocketLink\n");


	switch (mMissionType)
	{
		// 登陆密码
	case MISSION_LEVEL:
	{
									//变量定义
 									CGlobalUserInfo *pGlobalUserInfo = CGlobalUserInfo::GetInstance();
 									tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
 
								   break;
	}
		// 银行密码
	case MISSION_NOTICE:
	{
									////变量定义
									//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
									//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

									////变量定义
									//CMD_GP_ModifyInsurePass ModifyInsurePass;
									//zeromemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

									////加密密码
									//tchar szSrcPassword[LEN_PASSWORD]=T_T("");
									//tchar szDesPassword[LEN_PASSWORD]=T_T("");
									//DF::MD5Encrypt(mSrcInsure,szSrcPassword);
									//DF::MD5Encrypt(mDstInsure,szDesPassword);

									////构造数据
									//ModifyInsurePass.dwUserID=pGlobalUserData->dwUserID;
									//tstrcpyn(ModifyInsurePass.szScrPassword,szSrcPassword,countarray(ModifyInsurePass.szScrPassword));
									//tstrcpyn(ModifyInsurePass.szDesPassword,szDesPassword,countarray(ModifyInsurePass.szDesPassword));

									////发送数据
									//send(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INSURE_PASS,&ModifyInsurePass,sizeof(ModifyInsurePass));
									break;
	}
	case MISSION_PERSON_INFO:
		//变量定义
		CGlobalUserInfo *pGlobalUserInfo = CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

		break;
	}

}

void CGPPersonalInfoMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPPersonalInfoMission::onEventTCPSocketShut\n");
}

void CGPPersonalInfoMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPPersonalInfoMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPPersonalInfoMission::onEventTCPSocketRead(int main, int sub, void* data, int size)
{
	PLAZZ_PRINTF("CGPPasswordMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}

	//switch (sub)
	//{
		//操作成功
	//case SUB_GP_GROWLEVEL_QUERY:	return onQueryLevelInfoSuccess(data, size);
		//操作失败
	//case SUB_GP_GROWLEVEL_PARAMETER:	return onQueryLevelUpTipsSuccess(data, size);
	//}

	return false;
}

bool CGPPersonalInfoMission::onQueryLevelInfoSuccess(void *data, int size_)
{
// 	if (mIGPPersonalInfoMissionSink){
// 		mIGPPersonalInfoMissionSink->onGPUpdateSuccess(data, size_);
// 	}
		return true;
}

bool CGPPersonalInfoMission::onQueryLevelUpTipsSuccess(void *data, int size_)
{
	return true;
}
