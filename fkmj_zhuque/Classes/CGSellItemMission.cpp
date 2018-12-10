#include "CGSellItemMission.h"


CGSellItemMission::CGSellItemMission(const char* url, int port)
: CCallMission("CGPAddRankMission",url, port)
{
	mIGPSellItemMissionSink = 0;

	addNetCall(CC_CALLBACK_2(CGSellItemMission::Net_GetSellRecordList,this),SUB_GP_SELLLIST_BACK);
	addNetCall(CC_CALLBACK_2(CGSellItemMission::onSubOperateSuccess,this),SUB_GP_OPERATE_SUCCESS);
	addNetCall(CC_CALLBACK_2(CGSellItemMission::onSubOperateFailure,this),SUB_GP_OPERATE_FAILURE);
}

// 设置回调接口
void CGSellItemMission::setMissionSink(CGSellItemMissionSink* pIGPKefuMissionSink)
{
	mIGPSellItemMissionSink = pIGPKefuMissionSink;
}

void CGSellItemMission::sellItem(CMD_GP_SellItem kItemInfo)
{
	addLinkCallFun(CC_CALLBACK_0(CGSellItemMission::CB_sellItem,this,kItemInfo));

	start();
}
void CGSellItemMission::CB_sellItem(CMD_GP_SellItem kItemInfo)
{
	datastream kDataStream;
	kItemInfo.StreamValue(kDataStream,true);
	send(MDM_GP_USER_SERVICE,SUB_GP_SELLITEM,&kDataStream[0],kDataStream.size());
}

void CGSellItemMission::GetSellRecordList(int iUserID)
{
	addLinkCallFun(CC_CALLBACK_0(CGSellItemMission::CB_GetSellRecordList,this,iUserID));

	start();
}
void CGSellItemMission::CB_GetSellRecordList(int iUserID)
{
	CMD_GP_GetSellRecordList kNetInfo;
	send(MDM_GP_USER_SERVICE,SUB_GP_SELLLIST_BACK,&kNetInfo,sizeof(kNetInfo));
}
void CGSellItemMission::Net_GetSellRecordList(void* data, int dataSize)
{
	stop();
}

// 操作成功
void CGSellItemMission::onSubOperateSuccess(void* data, int size)
{
	CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)data;

	if (mIGPSellItemMissionSink)
	{
		mIGPSellItemMissionSink->onGPSellTimeResoult(pOperateSuccess->lResultCode,utility::a_u8(pOperateSuccess->szDescribeString));
	}
	stop();
}
// 操作失败
void CGSellItemMission::onSubOperateFailure(void* data, int size)
{

	CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)data;

	if (mIGPSellItemMissionSink)
	{
		mIGPSellItemMissionSink->onGPSellTimeResoult(pOperateFailure->lResultCode,utility::a_u8(pOperateFailure->szDescribeString));
	}
	stop();
}
