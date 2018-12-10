#include "CGPPasswordMission.h"

CGPPasswordMission::CGPPasswordMission(const char* url, int port)
: CSocketMission(url, port)
{
	mMissionType = MISSION_PASSWORD_NULL;
	mIGPPasswordMissionSink = 0;
}

// 设置回调接口
void CGPPasswordMission::setMissionSink(IGPPasswordMissionSink* pIGPPasswordMissionSink)
{
	mIGPPasswordMissionSink = pIGPPasswordMissionSink;
}

// 修改登陆密码
void CGPPasswordMission::modifyLogin(const char* szSrc, const char* szDst)
{
	mMissionType = MISSION_PASSWORD_LOGIN;
	memcpy(mSrcLogin, szSrc, sizeof(mSrcLogin));
	memcpy(mDstLogin, szDst, sizeof(mDstLogin));
	start();
}

// 修改银行柜密码
void CGPPasswordMission::modifyInsure(const char* szSrc, const char* szDst)
{
	mMissionType = MISSION_PASSWORD_INSURE;
	memcpy(mSrcInsure, szSrc, sizeof(mSrcInsure));
	memcpy(mDstInsure, szDst, sizeof(mDstInsure));
	start();
}

void CGPPasswordMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPPasswordMission::onEventTCPSocketLink\n");

	
	switch (mMissionType)
	{
		// 登陆密码
	case MISSION_PASSWORD_LOGIN:
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//变量定义
			CMD_GP_ModifyLogonPass ModifyLogonPass;
			zeromemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

			//构造数据
			ModifyLogonPass.dwUserID=pGlobalUserData->dwUserID;
			strcpy(ModifyLogonPass.szScrPassword,mSrcLogin);
			strcpy(ModifyLogonPass.szDesPassword,mDstLogin);

			//发送数据
			send(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_LOGON_PASS,&ModifyLogonPass,sizeof(ModifyLogonPass));
			break;
		}
		// 银行密码
	case MISSION_PASSWORD_INSURE:
		{
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//变量定义
			CMD_GP_ModifyInsurePass ModifyInsurePass;
			zeromemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//构造数据
			ModifyInsurePass.dwUserID=pGlobalUserData->dwUserID;
			strcpy(ModifyInsurePass.szScrPassword,mSrcInsure);
			strcpy(ModifyInsurePass.szDesPassword,mDstInsure);

			//发送数据
			send(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INSURE_PASS,&ModifyInsurePass,sizeof(ModifyInsurePass));
			break;
		}
	}
}

void CGPPasswordMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPPasswordMission::onEventTCPSocketShut\n");
}

void CGPPasswordMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPPasswordMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPPasswordMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPPasswordMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}
	switch (sub)
	{
		//操作成功
	case SUB_GP_OPERATE_SUCCESS:	return onSubOperateSuccess(data, size);
		//操作失败
	case SUB_GP_OPERATE_FAILURE:	return onSubOperateFailure(data, size);
	}

	return false;
}

// 操作成功
bool CGPPasswordMission::onSubOperateSuccess(void* data, int size)
{
	//变量定义
	CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)data;

	//效验数据
	ASSERT(size>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
	if (size<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) return false;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	switch (mMissionType)
	{
		// 登陆密码
	case MISSION_PASSWORD_LOGIN:
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//密码资料
			strcpy(pGlobalUserData->szPassword,mDstLogin);
			break;
		}
		// 银行密码
	case MISSION_PASSWORD_INSURE:
		{
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			pGlobalUserData->cbInsureEnabled = 1;
			break;
		}
	}

	stop();

	//显示消息
	if (mIGPPasswordMissionSink)
		mIGPPasswordMissionSink->onGPPasswordSuccess(mMissionType, (pOperateSuccess->szDescribeString));

	return true;
}

// 操作失败
bool CGPPasswordMission::onSubOperateFailure(void* data, int size)
{
	//效验参数
	CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)data;
	ASSERT(size>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
	if (size<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) return false;

	stop();

	//显示消息
	if (mIGPPasswordMissionSink)
		mIGPPasswordMissionSink->onGPPasswordFailure(mMissionType, (pOperateFailure->szDescribeString));

	return true;
}