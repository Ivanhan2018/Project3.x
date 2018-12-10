#include "CGPFaceMission.h"

CGPFaceMission::CGPFaceMission(const char* url, int port)
: CSocketMission(url, port)
{
	mMissionType = MISSION_FACE_NULL;
	mIGPFaceMissionSink = 0;
}

// 设置回调接口
void CGPFaceMission::setMissionSink(IGPFaceMissionSink* pIGPFaceMissionSink)
{
	mIGPFaceMissionSink = pIGPFaceMissionSink;
}

// 设置系统头像
void CGPFaceMission::setToSystemFace(uint16 face)
{
	mMissionType = MISSION_FACE_SYSTEM;
	mFace = face;
	start();
}

// 设置自定义头像
void CGPFaceMission::setToCustomFace(uint32 custom[FACE_CX*FACE_CY])
{
	mMissionType = MISSION_FACE_CUSTOM;
	memcpy(mCustom, custom, sizeof(mCustom));
	start();
}

void CGPFaceMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPFaceMission::onEventTCPSocketLink\n");

	
	switch (mMissionType)
	{
		// 系统头像
	case MISSION_FACE_SYSTEM:
		{
			//TEST_NULL
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			////变量定义
			//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			////变量定义
			//CMD_GP_SystemFaceInfo SystemFaceInfo;
			//zeromemory(&SystemFaceInfo,sizeof(SystemFaceInfo));

			////设置变量
			//SystemFaceInfo.wFaceID=mFace;
			//SystemFaceInfo.dwUserID=pGlobalUserData->dwUserID;
			//DF::shared()->GetMachineID(SystemFaceInfo.szMachineID);
			//memcpy(SystemFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(SystemFaceInfo.szPassword));

			////发送数据
			//send(MDM_GP_USER_SERVICE,SUB_GP_SYSTEM_FACE_INFO,&SystemFaceInfo,sizeof(SystemFaceInfo));

			break;
		}
		// 自定义头像
	case MISSION_FACE_CUSTOM:
		{
			
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//变量定义
			CMD_GP_CustomFaceInfo CustomFaceInfo;
			zeromemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

			//设置变量
			CustomFaceInfo.dwUserID=pGlobalUserData->dwUserID;
			memcpy(CustomFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(CustomFaceInfo.szPassword));
			memcpy(CustomFaceInfo.dwCustomFace,mCustom,sizeof(CustomFaceInfo.dwCustomFace));

			//发送数据
			send(MDM_GP_USER_SERVICE,SUB_GP_CUSTOM_FACE_INFO,&CustomFaceInfo,sizeof(CustomFaceInfo));
			break;
		}
	}

}

void CGPFaceMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPFaceMission::onEventTCPSocketShut\n");
}

void CGPFaceMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPFaceMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPFaceMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPFaceMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}

	switch (sub)
	{
		// 用户头像
	case SUB_GP_USER_FACE_INFO:		return onSubUserFaceInfo(data, size);
		//操作成功
	case SUB_GP_OPERATE_SUCCESS:	return onSubOperateSuccess(data, size);
		//操作失败
	case SUB_GP_OPERATE_FAILURE:	return onSubOperateFailure(data, size);
	}

	return false;
}

// 用户头像
bool CGPFaceMission::onSubUserFaceInfo(void* data, int size)
{
	//TEST_NULL
	//效验参数
	ASSERT(size==6);
	if (size!=6) return false;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	

	return true;

	////效验参数
	//ASSERT(size==sizeof(CMD_GP_UserFaceInfo));
	//if (size!=sizeof(CMD_GP_UserFaceInfo)) return false;

	////消息处理
	//CMD_GP_UserFaceInfo * pUserFaceInfo=(CMD_GP_UserFaceInfo *)data;

	////变量定义
	//CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	//tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//pGlobalUserData->dwCustomID = pUserFaceInfo->dwCustomID;

	////关闭连接
	//stop();

	////保存数据
	//if (pUserFaceInfo->dwCustomID==0)
	//{
	//	memset(&pGlobalUserData->CustomFaceInfo, 0, sizeof(pGlobalUserData->CustomFaceInfo));
	//	if (mIGPFaceMissionSink)
	//		mIGPFaceMissionSink->onGPFaceInfo();
	//}
	//else
	//{
	//	//memcpy(&pGlobalUserData->CustomFaceInfo, pUserFaceInfo->, sizeof(pGlobalUserData->CustomFaceInfo))
	//	//保存数据
	//	//CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
	//	//pCustomFaceManager->SaveUserCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo.dwCustomFace);

	//	if (mIGPFaceMissionSink)
	//		mIGPFaceMissionSink->onGPFaceInfo();
	//}

	//return true;
}

// 操作成功
bool CGPFaceMission::onSubOperateSuccess(void* data, int size)
{
	//效验数据
	ASSERT(size>=4);
	if (size<4) return false;

	//关闭连接
	stop();

	
	return true;

	////变量定义
	//CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)data;

	////效验数据
	//ASSERT(size>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
	//if (size<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) return false;

	////关闭连接
	//stop();

	////显示消息
	//if (mIGPFaceMissionSink)
	//	mIGPFaceMissionSink->onGPFaceSuccess(u2_8(pOperateSuccess->szDescribeString));

	//return true;
}

// 操作失败
bool CGPFaceMission::onSubOperateFailure(void* data, int size)
{
	//TEST_NULL
	//效验数据
	ASSERT(size>=4);
	if (size<4) return false;

	//关闭连接
	stop();


	return true;
	
	////效验参数
	//CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)data;
	//ASSERT(size>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
	//if (size<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) return false;

	////关闭连接
	//stop();

	////显示消息
	//if (mIGPFaceMissionSink)
	//	mIGPFaceMissionSink->onGPFaceFailure(u2_8(pOperateFailure->szDescribeString));

	//return true;
}