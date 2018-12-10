#include "CGPRoomListOnLineMission.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGPRoomListOnLineMission::CGPRoomListOnLineMission()
{
	//设置变量
	m_bUpdateOnLine=false;

	return;
}

//析构函数
CGPRoomListOnLineMission::~CGPRoomListOnLineMission()
{
	return;
}

//连接事件
void CGPRoomListOnLineMission::onEventTCPSocketLink()
{
	//设置变量
	m_bUpdateOnLine=false;

	//变量定义
	CMD_GP_GetOnline GetOnline;
	zeromemory(&GetOnline,sizeof(GetOnline));

	//变量定义
	CServerListData * pServerListData=CServerListData::shared();

	CGameServerItemMap::iterator itor = pServerListData->GetServerItemMapBegin();

	CGameServerItem * pGameServerItem=pServerListData->EmunGameServerItem(itor);
	while(pGameServerItem)
	{
		//获取房间
		//溢出判断
		if (GetOnline.wServerCount>=CountArray(GetOnline.wOnLineServerID))
		{
			ASSERT(FALSE);
			break;
		}

		tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
		GetOnline.wOnLineServerID[GetOnline.wServerCount++]=pGameServer->wServerID;

		pGameServerItem=pServerListData->EmunGameServerItem(itor);
	}

	//发送数据
	word wHeadSize=sizeof(GetOnline)-sizeof(GetOnline.wOnLineServerID);
	word wSendSize=wHeadSize+sizeof(GetOnline.wOnLineServerID[0])*GetOnline.wServerCount;
	send(MDM_GP_SERVER_LIST,SUB_GP_GET_ONLINE,&GetOnline,wSendSize);
}
void CGPRoomListOnLineMission::onEventTCPSocketShut()
{

}
void CGPRoomListOnLineMission::onEventTCPSocketError(int errorCode)
{

}
bool CGPRoomListOnLineMission::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	switch (sub)
	{
	//case SUB_GP_LIST_TYPE:		//种类信息
	//	{
	//		//效验参数
	//		ASSERT(dataSize%sizeof(tagGameType)==0);
	//		if (dataSize%sizeof(tagGameType)!=0) return false;

	//		//变量定义
	//		WORD wItemCount=dataSize/sizeof(tagGameType);
	//		tagGameType * pGameType=(tagGameType *)(data);

	//		//获取对象
	//		CServerListData * pServerListData=CServerListData::shared();

	//		//更新数据
	//		for (WORD i=0;i<wItemCount;i++)
	//		{
	//			pServerListData->InsertGameType(pGameType++);
	//		}

	//		return true;
	//	}
	//case SUB_GP_LIST_KIND:		//类型信息
	//	{
	//		//效验参数
	//		ASSERT(dataSize%sizeof(tagGameKind)==0);
	//		if (dataSize%sizeof(tagGameKind)!=0) return false;

	//		//变量定义
	//		WORD wItemCount=dataSize/sizeof(tagGameKind);
	//		tagGameKind * pGameKind=(tagGameKind *)(data);

	//		//获取对象
	//		CServerListData * pServerListData=CServerListData::shared();

	//		//更新数据
	//		for (WORD i=0;i<wItemCount;i++)
	//		{
	//			pServerListData->InsertGameKind(pGameKind++);
	//		}

	//		return true;
	//	}
	//case SUB_GP_LIST_SERVER:	//房间信息
	//	{
	//		//效验参数
	//		ASSERT(dataSize%sizeof(tagGameServer)==0);
	//		if (dataSize%sizeof(tagGameServer)!=0) return false;

	//		//变量定义
	//		WORD wItemCount=dataSize/sizeof(tagGameServer);
	//		tagGameServer * pGameServer=(tagGameServer *)(data);

	//		//获取对象
	//		CServerListData * pServerListData=CServerListData::shared();

	//		//更新数据
	//		for (WORD i=0;i<wItemCount;i++)
	//		{
	//			pServerListData->InsertGameServer(pGameServer++);
	//		}

	//		return true;
	//	}
	//case SUB_GP_LIST_FINISH:	//列表信息
	//	{
	//		//列表完成
	//		ASSERT(CServerListData::shared()!=NULL);
	//		if (CServerListData::shared()!=NULL) CServerListData::shared()->OnEventListFinish();

	//		return true;
	//	}
	//case SUB_GP_SERVER_FINISH:	//房间完成
	//	{
	//		//效验数据
	//		ASSERT(dataSize%sizeof(WORD)==0);
	//		if (dataSize%sizeof(WORD)!=0) return false;

	//		//获取对象
	//		ASSERT(CServerListData::shared()!=NULL);
	//		CServerListData * pServerListData=CServerListData::shared();

	//		//任务处理
	//		for (UINT i=0;i<(dataSize/sizeof(WORD));i++)
	//		{
	//			m_KindIDPending.RemoveKey(((WORD *)data)[i]);
	//			pServerListData->OnEventKindFinish(((WORD *)data)[i]);
	//		}

	//		stop();

	//		return true;
	//	}
	case SUB_GR_KINE_ONLINE:	//类型在线
		{
			//变量定义
			CMD_GP_KindOnline * pKindOnline=(CMD_GP_KindOnline *)data;
			word wHeadSize=(sizeof(CMD_GP_KindOnline)-sizeof(pKindOnline->OnLineInfoKind));

			//效验数据
			ASSERT((dataSize>=wHeadSize)&&(dataSize==(wHeadSize+pKindOnline->wKindCount*sizeof(tagOnLineInfoKind))));
			if ((dataSize<wHeadSize)||(dataSize!=(wHeadSize+pKindOnline->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

			//获取对象
			CServerListData * pServerListData=CServerListData::shared();

			//设置人数
			for (word i=0;i<pKindOnline->wKindCount;i++)
			{
				tagOnLineInfoKind * pOnLineInfoKind=&pKindOnline->OnLineInfoKind[i];
				pServerListData->SetKindOnLineCount(pOnLineInfoKind->wKindID,pOnLineInfoKind->dwOnLineCount);
			}

			return true;
		}
	case SUB_GR_SERVER_ONLINE:	//房间在线
		{
			//变量定义
			CMD_GP_ServerOnline * pServerOnline=(CMD_GP_ServerOnline *)data;
			word wHeadSize=(sizeof(CMD_GP_ServerOnline)-sizeof(pServerOnline->OnLineInfoServer));

			//效验数据
			ASSERT((dataSize>=wHeadSize)&&(dataSize==(wHeadSize+pServerOnline->wServerCount*sizeof(tagOnLineInfoServer))));
			if ((dataSize<wHeadSize)||(dataSize!=(wHeadSize+pServerOnline->wServerCount*sizeof(tagOnLineInfoServer)))) return false;

			//获取对象
			CServerListData * pServerListData=CServerListData::shared();

			//设置人数
			for (word i=0;i<pServerOnline->wServerCount;i++)
			{
				tagOnLineInfoServer * pOnLineInfoServer=&pServerOnline->OnLineInfoServer[i];
				pServerListData->SetServerOnLineCount(pOnLineInfoServer->wServerID,pOnLineInfoServer->dwOnLineCount);
			}

			//完成通知
			pServerListData->SetServerOnLineFinish();

			stop();
			return true;
		}
	}

	return true;
}

//更新人数
void CGPRoomListOnLineMission::UpdateOnLineInfo()
{
	start();
	return;
}
