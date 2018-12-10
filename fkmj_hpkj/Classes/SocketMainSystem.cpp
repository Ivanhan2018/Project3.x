#include "CServerItem.h"
//系统消息
bool CServerItem::OnSocketMainSystem(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//系统消息
	case SUB_CM_SYSTEM_MESSAGE:	return OnSocketSubSystemMessage(data,dataSize);
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}


//系统消息
bool CServerItem::OnSocketSubSystemMessage(void* data, int wDataSize)
{
	PLAZZ_PRINTF(("CGPLoginMission::OnSocketSubSystemMessage\n"));

	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)data;
	word wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(char))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(char)))) return false;

	word wType = pSystemMessage->wType;

	//关闭处理
	if ((wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		if (mIStringMessageSink)
		{
			mIStringMessageSink->InsertSystemString(pSystemMessage->szString);
		}
		OnGFGameClose(0);
	}

	//显示消息
	if (wType&SMT_CHAT) 
	{
		if (mIStringMessageSink)
		{
			mIStringMessageSink->InsertSystemString(pSystemMessage->szString);
		}
	}

	//关闭游戏
	if (wType&SMT_CLOSE_GAME)
	{
		if (mIStringMessageSink)
		{
			mIStringMessageSink->InsertSystemString(pSystemMessage->szString);
		}
		OnGFGameClose(0);
	}

	//弹出消息
	if (wType&SMT_EJECT)
	{
		if (mIStringMessageSink)
		{
			mIStringMessageSink->InsertPromptString(pSystemMessage->szString,0);
		}
	}

	//关闭房间
	if (wType&SMT_CLOSE_ROOM)
	{
		if (mIStringMessageSink)
		{
			mIStringMessageSink->InsertSystemString(pSystemMessage->szString);
		}
		OnGFGameClose(0);
	}

	return true;

}

//动作消息
bool CServerItem::OnSocketSubActionMessage(void* data, int dataSize)
{
	PLAZZ_PRINTF(("CGPLoginMission::OnSocketSubActionMessage\n"));
	return true;
}
