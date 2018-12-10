#include "CServerItem.h"
//配置信息
bool CServerItem::OnSocketMainConfig(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//列表配置
	case SUB_GR_CONFIG_COLUMN:		return OnSocketSubConfigColumn(data, dataSize);
		//房间配置
	case SUB_GR_CONFIG_SERVER:		return OnSocketSubConfigServer(data, dataSize);
		//道具配置
	case SUB_GR_CONFIG_PROPERTY:	return OnSocketSubConfigOrder(data, dataSize);
		//配置玩家权限	
	///case SUB_GR_CONFIG_USER_RIGHT:	return OnSocketSubConfigMmber(data, dataSize);
		//配置完成
	case SUB_GR_CONFIG_FINISH:		return OnSocketSubConfigFinish(data, dataSize);	
	}

	//错误断言
	ASSERT(FALSE);
	return true;
}

//列表配置
bool CServerItem::OnSocketSubConfigColumn(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubConfigColumn\n"));

	//变量定义
	//PACKET_AIDE_DATA(data);
	//byte cbColumnCount = packet.readByte();

	//u2string str;

	//for (int i = 0; i < cbColumnCount; ++i)
	//{
	//	tagColumnItem ColumnItem;
	//	ColumnItem.cbColumnWidth = packet.readByte();
	//	ColumnItem.cbDataDescribe = packet.readByte();
	//	str.resize(16+1,'\0');
	//	packet.readUTF16(&str[0], 16);
	//}

	if (mIServerItemSink)
		mIServerItemSink->OnGRConfigColumn();

	return true;
}

//房间配置
bool CServerItem::OnSocketSubConfigServer(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubConfigServer\n"));

	//效验数据
	ASSERT(dataSize==sizeof(CMD_GR_ConfigServer));
	if (dataSize<sizeof(CMD_GR_ConfigServer)) return false;

	//消息处理
	CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)data;

	mServerAttribute.wTableCount=pConfigServer->wTableCount;
	mServerAttribute.wChairCount=pConfigServer->wChairCount;

	if (!m_TableFrame.ConfigTableFrame(
		mServerAttribute.wTableCount,
		mServerAttribute.wChairCount,
		mServerAttribute.wServerID))
	{
		IntermitConnect(false);
		return false;
	}

	if (mIServerItemSink)
		mIServerItemSink->OnGRConfigServer();

	return true;

}

//道具配置
bool CServerItem::OnSocketSubConfigOrder(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubConfigOrder\n"));

	////变量定义
	//PACKET_AIDE_DATA(data);
	//byte cbPropertyCount=packet.readByte();

	//for (int i = 0; i < cbPropertyCount; ++i)
	//{
	//	tagPropertyInfo PropertyInfo;
	//	PropertyInfo.wIndex = packet.read2Byte();
	//	PropertyInfo.wDiscount = packet.read2Byte();
	//	PropertyInfo.wIssueArea = packet.read2Byte();
	//	PropertyInfo.lPropertyGold = packet.read8Byte();
	//	PropertyInfo.dPropertyCash = packet.read8Byte();
	//	PropertyInfo.lSendLoveLiness = packet.read8Byte();
	//	PropertyInfo.lRecvLoveLiness = packet.read8Byte();
	//}

	if (mIServerItemSink)
		mIServerItemSink->OnGRConfigProperty();

	return true;

}

//配置玩家权限	
bool CServerItem::OnSocketSubConfigMmber(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubConfigMmber\n"));
	
	if (mIServerItemSink)
		mIServerItemSink->OnGRConfigUserRight();

	return true;
	////效验数据
}

//配置完成
bool CServerItem::OnSocketSubConfigFinish(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CServerItem::OnSocketSubConfigFinish\n"));

	if (mIServerItemSink)
		mIServerItemSink->OnGRConfigFinish();
	return true;
}
