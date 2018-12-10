#include "CGPGameRecord.h"


CGPGameRecordMission::CGPGameRecordMission(const char* url, int port)
: CCallMission("CGPGameRecordMission",url, port)
{
	mIGPGameRecordSink = 0;

	addNetCall(CC_CALLBACK_2(CGPGameRecordMission::Net_GetGameRecordList,this),SUB_GP_GAME_RECORD_LIST);
	addNetCall(CC_CALLBACK_2(CGPGameRecordMission::Net_PrivateRandTotalRecord,this),SUB_GP_GAME_RECORD_TOTAL);
	addNetCall(CC_CALLBACK_2(CGPGameRecordMission::Net_PrivateRandChildRecord,this),SUB_GP_GAME_RECORD_CHILD);
}

// 设置回调接口
void CGPGameRecordMission::setMissionSink(CGGameRecordSink* pIGPKefuMissionSink)
{
	mIGPGameRecordSink = pIGPKefuMissionSink;
}

void CGPGameRecordMission::GetGameRecordList(int iUserID)
{
	PLAZZ_PRINTF("CGPGameRecordMission::GetGameRecordList\n");

	addLinkCallFun(CC_CALLBACK_0(CGPGameRecordMission::CB_GetGameRecordList,this,iUserID));

	start();
}
void CGPGameRecordMission::CB_GetGameRecordList(int iUserID)
{
	PLAZZ_PRINTF("CGPGameRecordMission::CB_GetGameRecordList\n");
	CMD_GP_GetGameRecordList kNetInfo;
	kNetInfo.dwUserID = iUserID;

	send(MDM_GP_USER_SERVICE,SUB_GP_GAME_RECORD_LIST,&kNetInfo,sizeof(kNetInfo));
}
void CGPGameRecordMission::Net_GetGameRecordList(void* data, int dataSize)
{
	PLAZZ_PRINTF("CGPGameRecordMission::Net_GetGameRecordList\n");

	datastream kDataStream(data,dataSize);
	tagPrivateRandTotalRecordList kNetInfo;
	kNetInfo.StreamValue(kDataStream,false);

	if (mIGPGameRecordSink)
	{
		mIGPGameRecordSink->onGPBackGameRecordList(&kNetInfo);
	}

	stop();
}
void CGPGameRecordMission::GetPrivateRandTotalRecord(int iRecordID)
{
	PLAZZ_PRINTF("CGPGameRecordMission::GetGameRecordVideo\n");

	addLinkCallFun(CC_CALLBACK_0(CGPGameRecordMission::CB_PrivateRandTotalRecord,this,iRecordID));

	start();
}
void CGPGameRecordMission::CB_PrivateRandTotalRecord(int iRecordID)
{
	CMD_GP_GetGameTotalRecord kNetInfo;
	kNetInfo.dwUserID = 0;
	kNetInfo.dwRecordID = iRecordID;
	send(MDM_GP_USER_SERVICE, SUB_GP_GAME_RECORD_TOTAL,&kNetInfo,sizeof(CMD_GP_GetGameTotalRecord));

}
void CGPGameRecordMission::Net_PrivateRandTotalRecord(void* data, int dataSize)
{

	datastream kStream(data,dataSize);
	tagPrivateRandTotalRecord kNetInfo;
	kNetInfo.StreamValue(kStream,false);

	if (mIGPGameRecordSink)
	{
		mIGPGameRecordSink->onGPBackGameTotalRecord(&kNetInfo);
	}

	stop();

}

void CGPGameRecordMission::GetPrivateRandChildRecord(int iChildRecordID)
{
	PLAZZ_PRINTF("CGPGameRecordMission::GetPrivateRandChildRecord\n");

	addLinkCallFun(CC_CALLBACK_0(CGPGameRecordMission::CB_PrivateRandChildRecord,this,iChildRecordID));

	start();

}
void CGPGameRecordMission::CB_PrivateRandChildRecord(int iChildRecordID)
{
	CMD_GP_GetGameChildRecord kNetInfo;
	kNetInfo.dwUserID = 0;
	kNetInfo.dwRecordID = iChildRecordID;
	send(MDM_GP_USER_SERVICE, SUB_GP_GAME_RECORD_CHILD,&kNetInfo,sizeof(CMD_GP_GetGameChildRecord));
}
void CGPGameRecordMission::Net_PrivateRandChildRecord(void* data, int dataSize)
{
	datastream kStream(data,dataSize);
	tagPrivateRandRecordChild kNetInfo;
	kNetInfo.StreamValue(kStream,false);

	if (mIGPGameRecordSink)
	{
		mIGPGameRecordSink->onGPBackGameChildRecord(&kNetInfo);
	}

	stop();
}