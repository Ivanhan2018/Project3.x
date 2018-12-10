#include "CGPInsureMission.h"

CGPInsureMission::CGPInsureMission(const char* url, int port)
: CSocketMission(url, port)
{
	mMissionType = 0;
	mIGPInsureMissionSink = 0;
}

void CGPInsureMission::setMissionSink(IGPInsureMissionSink* pIGPInsureMissionSink)
{
	mIGPInsureMissionSink = pIGPInsureMissionSink;
}

void CGPInsureMission::query()
{
	mMissionType = INSURE_MISSION_QUERY;
	start();
}

void CGPInsureMission::save(int64 score)
{
	mScore = score;
	mMissionType = INSURE_MISSION_SAVE;
	start();
}

void CGPInsureMission::take(int64 score, const char* szInsurePass)
{
	mScore = score;
	memcpy(mInsurePass, szInsurePass, sizeof(mInsurePass));
	mMissionType = INSURE_MISSION_TAKE;
	start();
}

void CGPInsureMission::transfer(int64 score, const char* szInsurePass, const char* szNickName, uint8 cbByNickName)
{
	mScore = score;
	mByNickName = cbByNickName;
	memcpy(mInsurePass, szInsurePass, sizeof(mInsurePass));
	memcpy(mNickName, szNickName, sizeof(mNickName));
	mMissionType = INSURE_MISSION_TRANSFER;
	start();
}

void CGPInsureMission::enable(const char* szLogonPass, const char* szInsurePass)
{
	memcpy(mLogonPass, szLogonPass, sizeof(mLogonPass));
	memcpy(mInsurePass, szInsurePass, sizeof(mInsurePass));
	mMissionType = INSURE_MISSION_ENABLE;
	start();
}

void CGPInsureMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPInsureMission::onEventTCPSocketLink\n");


	switch (mMissionType)
	{
		// 查询
	case INSURE_MISSION_QUERY:
	{
		CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

		CMD_GP_QueryInsureInfo QueryInsureInfo;
		zeromemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

		QueryInsureInfo.dwUserID = pGlobalUserInfo->getUserID();

		//发送数据
		send(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
		break;
	}
		// 存入
	case INSURE_MISSION_SAVE:
	{
		CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

		//变量定义
		CMD_GP_UserSaveScore UserSaveScore;
		zeromemory(&UserSaveScore,sizeof(UserSaveScore));

		//设置变量
		UserSaveScore.lSaveScore=mScore;
		UserSaveScore.dwUserID=pGlobalUserData->dwUserID;
		strcpy(UserSaveScore.szMachineID,"QYMachineID");

		//发送数据
		send(MDM_GP_USER_SERVICE,SUB_GP_USER_SAVE_SCORE,&UserSaveScore,sizeof(UserSaveScore));

		break;
	}
		// 取款
	case INSURE_MISSION_TAKE:
	{
		CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

		//变量定义
		CMD_GP_UserTakeScore UserTakeScore;
		zeromemory(&UserTakeScore,sizeof(UserTakeScore));

		//设置变量
		UserTakeScore.lTakeScore=mScore;
		UserTakeScore.dwUserID=pGlobalUserData->dwUserID;
		strcpy(UserTakeScore.szMachineID,"QYMachineID");
		strcpy(UserTakeScore.szPassword,mInsurePass);

		//发送数据
		send(MDM_GP_USER_SERVICE,SUB_GP_USER_TAKE_SCORE,&UserTakeScore,sizeof(UserTakeScore));

		break;
	}
		// 转账
	case INSURE_MISSION_TRANSFER:
	{
		//变量定义
		CMD_GP_QueryUserInfoRequest QueryUserInfoRequest;
		zeromemory(&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));

		//设置变量
		QueryUserInfoRequest.cbByNickName=mByNickName;
		strcpy(QueryUserInfoRequest.szNickName,mNickName);

		//发送数据
		send(MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_REQUEST,&QueryUserInfoRequest,sizeof(QueryUserInfoRequest));
		break;
	}
	case INSURE_MISSION_ENABLE:
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

		break;
	}
	}
}

void CGPInsureMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPInsureMission::onEventTCPSocketShut\n");
}

void CGPInsureMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPInsureMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPInsureMission::onEventTCPSocketRead(int main, int sub, void* data, int size)
{
	PLAZZ_PRINTF("CGPInsureMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}

	switch (sub)
	{
		//转账查询
	case SUB_GP_QUERY_USER_INFO_RESULT:		return onSubQueryInfoResult(data, size);
		//银行资料
	case SUB_GP_USER_INSURE_INFO:			return onSubInsureInfo(data, size);
		//银行成功
	case SUB_GP_USER_INSURE_SUCCESS:		return onSubInsureSuccess(data, size);
		//银行失败
	case SUB_GP_USER_INSURE_FAILURE:		return onSubInsureFailure(data, size);
		//操作成功
	case SUB_GP_OPERATE_SUCCESS:			return onSubOperateSuccess(data, size);
		//操作失败
	case SUB_GP_OPERATE_FAILURE:			return onSubOperateFailure(data, size);
	}

	return false;
}

//转账查询
bool CGPInsureMission::onSubQueryInfoResult(void* data, int wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GP_UserTransferUserInfo));
	if (wDataSize<sizeof(CMD_GP_UserTransferUserInfo)) return false;

	//变量定义
	CMD_GP_UserTransferUserInfo * pTransferUserInfo=(CMD_GP_UserTransferUserInfo *)data;
	CMD_GP_UserTransferScore UserTransferScore;
	zeromemory(&UserTransferScore,sizeof(UserTransferScore));

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置变量
	UserTransferScore.cbByNickName=0;
	UserTransferScore.lTransferScore=mScore;
	UserTransferScore.dwUserID=pGlobalUserData->dwUserID;
	strcpy(UserTransferScore.szMachineID,"QYMachineID");
	strcpy(UserTransferScore.szNickName,mNickName);
	strcpy(UserTransferScore.szPassword,mInsurePass);

	//发送数据
	send(MDM_GP_USER_SERVICE,SUB_GP_USER_TRANSFER_SCORE,&UserTransferScore,sizeof(UserTransferScore));
	return true;

}

//银行资料
bool CGPInsureMission::onSubInsureInfo(void* data, int size)
{	
	//效验参数
	ASSERT(size==sizeof(CMD_GP_UserInsureInfo));
	if (size<sizeof(CMD_GP_UserInsureInfo)) return false;

	//变量定义
	CMD_GP_UserInsureInfo * pUserInsureInfo=(CMD_GP_UserInsureInfo *)data;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagUserInsureInfo * pInsureInfo=pGlobalUserInfo->GetUserInsureInfo();

	pInsureInfo->wRevenueTake			= pUserInsureInfo->wRevenueTake;
	pInsureInfo->wRevenueTransfer		= pUserInsureInfo->wRevenueTransfer;
	pInsureInfo->wServerID				= pUserInsureInfo->wServerID;
	pInsureInfo->lUserScore				= pUserInsureInfo->lUserScore;
	pInsureInfo->lUserInsure			= pUserInsureInfo->lUserInsure;
	pInsureInfo->lTransferPrerequisite	= pUserInsureInfo->lTransferPrerequisite;

	//关闭连接
	stop();

	if (mIGPInsureMissionSink)
		mIGPInsureMissionSink->onInsureInfo();
	return true;
}

//银行成功
bool CGPInsureMission::onSubInsureSuccess(void* data, int size)
{
	//效验参数
	CMD_GP_UserInsureSuccess * pUserInsureSuccess=(CMD_GP_UserInsureSuccess *)data;
	ASSERT(size>=(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
	if (size<(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo   = CGlobalUserInfo::GetInstance();
	tagUserInsureInfo * pInsureInfo=pGlobalUserInfo->GetUserInsureInfo();
	tagGlobalUserData * pUserData=pGlobalUserInfo->GetGlobalUserData();

	//更新
	pInsureInfo->lUserScore			= pUserInsureSuccess->lUserScore;
	pInsureInfo->lUserInsure		= pUserInsureSuccess->lUserInsure;
	pUserData->lUserScore			= pUserInsureSuccess->lUserScore;
	pUserData->lUserInsure			= pUserInsureSuccess->lUserInsure;

	pGlobalUserInfo->upPlayerInfo();

	//关闭连接
	stop();

	//显示消息
	if (mIGPInsureMissionSink)
		mIGPInsureMissionSink->onInsureSuccess(mMissionType, pUserInsureSuccess->szDescribeString);
	return true;
}

//银行失败
bool CGPInsureMission::onSubInsureFailure(void* data, int size)
{
	//效验参数
	CMD_GP_UserInsureFailure * pUserInsureFailure=(CMD_GP_UserInsureFailure *)data;
	ASSERT(size>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
	if (size<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) 
		return false;

	//关闭连接
	stop();

	//显示消息
	if (mIGPInsureMissionSink)
		mIGPInsureMissionSink->onInsureFailure(mMissionType, pUserInsureFailure->szDescribeString);
	return true;
}

//操作成功
bool CGPInsureMission::onSubOperateSuccess(void* data, int size)
{
	////变量定义
	CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)data;

	//效验数据
	ASSERT(size>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
	if (size<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) return false;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//关闭连接
	stop();

	//显示消息
	if (mIGPInsureMissionSink)
		mIGPInsureMissionSink->onInsureSuccess(mMissionType, pOperateSuccess->szDescribeString);

	return true;
}

// 操作失败
bool CGPInsureMission::onSubOperateFailure(void* data, int size)
{
	//效验参数
	CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)data;
	ASSERT(size>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
	if (size<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) return false;

	//关闭连接
	stop();

	//显示消息
	if (mIGPInsureMissionSink)
		mIGPInsureMissionSink->onInsureFailure(mMissionType, pOperateFailure->szDescribeString);
	return true;
}
