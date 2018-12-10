#include "CServerItem.h"
//状态信息
bool CServerItem::OnSocketMainStatus(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//桌子信息
	case SUB_GR_TABLE_INFO:		return OnSocketSubStatusTableInfo(data, dataSize);
		//桌子状态
	case SUB_GR_TABLE_STATUS:	return OnSocketSubStatusTableStatus(data, dataSize);
	}

	return true;
}

//桌子信息
bool CServerItem::OnSocketSubStatusTableInfo(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubStatusTableInfo(void* data, int dataSize)\n"));
	
	//变量定义
	return true;

	//CMD_GR_TableInfo * pTableInfo=(CMD_GR_TableInfo *)data;
	//word wHeadSize=sizeof(CMD_GR_TableInfo)-sizeof(pTableInfo->TableStatusArray);

	////效验数据
	//ASSERT((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))==dataSize);
	//if ((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))!=dataSize) return false;

	//_tprintf(T_T("设置桌子信息:%d\n"), pTableInfo->wTableCount);
	////消息处理
	//for (word i=0;i<pTableInfo->wTableCount;i++)
	//{
	//	byte cbTableLock=pTableInfo->TableStatusArray[i].cbTableLock;
	//	byte cbPlayStatus=pTableInfo->TableStatusArray[i].cbPlayStatus;
	//	mTableViewFrame.SetTableStatus(i,(cbPlayStatus==TRUE),(cbTableLock==TRUE));
	//}

	//return true;
}

//桌子状态
bool CServerItem::OnSocketSubStatusTableStatus(void* data, int dataSize)
{

	////效验参数
	//ASSERT(dataSize==sizeof(CMD_GR_TableStatus));
	//if (dataSize!=sizeof(CMD_GR_TableStatus)) return false;

	////变量定义
	//CMD_GR_TableStatus * pTableStatus=(CMD_GR_TableStatus *)data;

	////设置桌子
	//byte cbTableLock=pTableStatus->TableStatus.cbTableLock;
	//byte cbPlayStatus=pTableStatus->TableStatus.cbPlayStatus;
	//mTableViewFrame.SetTableStatus(pTableStatus->wTableID,(cbPlayStatus==TRUE),(cbTableLock==TRUE));

	////设置桌子
	//if(cbPlayStatus==TRUE && mMeUserItem->GetTableID()==pTableStatus->wTableID && CServerRule::IsAllowAvertCheatMode(mServerAttribute.dwServerRule))
	//{
	//	mTableViewFrame.SetTableStatus(false);
	//}

	return true;
}