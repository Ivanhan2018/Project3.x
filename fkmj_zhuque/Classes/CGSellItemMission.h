#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CCallMission.h"

//出售
struct CMD_GP_SellItem
{
	dword						dwSrcUserID;					
	std::string					strSrcPassword;					
	dword						dwDestUserID;					
	dword						dwItemType;						
	dword						dwItemValue;					

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwSrcUserID);
		Stream_VALUE(strSrcPassword);
		Stream_VALUE(dwDestUserID);
		Stream_VALUE(dwItemType);
		Stream_VALUE(dwItemValue);
	}
};
//出售
struct CMD_GP_GetSellRecordList
{
	word						dwUserID;
};


//商城列表
struct CMD_GP_BackSellOderItem
{
	int							DestUserID;					 
	int							kItemType;				 
	int							kItemValue;				
	std::string					DestUserName;		
	systemtime					kPlayTime;		 

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(DestUserID);
		Stream_VALUE(kItemType);
		Stream_VALUE(kItemValue);
		Stream_VALUE(DestUserName);
		Stream_VALUE_SYSTEMTIME(kPlayTime);
	}
};
struct CMD_GP_BackSellOderList
{			
	std::vector<CMD_GP_BackSellOderItem>	kItems;		 

	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(CMD_GP_BackSellOderItem,kItems);
	}
};
//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class CGSellItemMissionSink
{
public:
	virtual ~CGSellItemMissionSink(){}
	virtual void onGPSellTimeResoult(int iError,std::string strError){}
	virtual void onGPBackSellOderList(CMD_GP_BackSellOderList* pNetInfo){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGSellItemMission
	: public CCallMission
{
public:
	CGSellItemMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(CGSellItemMissionSink* pIGPKefuMissionSink);

	void sellItem(CMD_GP_SellItem kItemInfo);
	void CB_sellItem(CMD_GP_SellItem kItemInfo);

	void GetSellRecordList(int iUserID);
	void CB_GetSellRecordList(int iUserID);
	void Net_GetSellRecordList(void* data, int dataSize);

	// 操作成功
	void onSubOperateSuccess(void* data, int size);
	// 操作失败
	void onSubOperateFailure(void* data, int size);
private:
	// 回调
	CGSellItemMissionSink* mIGPSellItemMissionSink;
}; 
