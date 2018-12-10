#include "CServerItem.h"
//游戏消息


bool CServerItem::OnSocketSubUserTalk(void* data, int dataSize)
{
	if (mIClientKernelSink)
	{
		return mIClientKernelSink->RevTalk(data,dataSize);
	}
	return true;
}

//游戏状态
bool CServerItem::OnSocketSubGameStatus(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubGameStatus\n");

	//效验参数
	ASSERT(dataSize==sizeof(CMD_GF_GameStatus));
	if (dataSize!=sizeof(CMD_GF_GameStatus)) return false;

	//消息处理
	CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)data;

	SetGameStatus(pGameStatus->cbGameStatus);
	return true;
}

//游戏场景
bool CServerItem::OnSocketSubGameScene(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubGameScene1");

	//效验状态
	ASSERT(m_pMeUserItem!=0);
	if (m_pMeUserItem==0) return true;

	if (mIClientKernelSink==0)
		return true;
	//场景处理
	bool bLookonUser=(m_pMeUserItem->GetUserStatus()==US_LOOKON);
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubGameScene3");
	return mIClientKernelSink->OnEventSceneMessage(mGameStatus,bLookonUser,data,dataSize);
}

//旁观状态
bool CServerItem::OnSocketSubLookonStatus(void* data, int dataSize)
{
	PLAZZ_PRINTF("flow->CClientKernel::OnSocketSubLookonStatus\n");

	return true;

}

//发送准备
bool CServerItem::SendUserReady(void * data, word dataSize)
{
	if (m_pMeUserItem == 0) return false;
	return SendSocketData(MDM_GF_FRAME, SUB_GF_USER_READY, 0, 0);
}
//获取状态
byte CServerItem::GetGameStatus()
{
	return mGameStatus;
}

//设置状态
void CServerItem::SetGameStatus(byte cbGameStatus)
{
	mGameStatus = cbGameStatus;
}
void CServerItem::SetClientKernelSink(IClientKernelSink*	pIClientKernelSink)
{
	PLAZZ_PRINTF("CServerItem::SetClientKernelSink");
	if (mIClientKernelSink)
	{
		mIClientKernelSink->clearInfo();
	}
	mIClientKernelSink = pIClientKernelSink;
}
bool CServerItem::OnSocketMainGameFrame(int main,int sub, void* data, int dataSize)
{
	//效验数据
	ASSERT(dataSize<=SOCKET_TCP_PACKET);
	if (dataSize>SOCKET_TCP_PACKET) return false;

	//游戏消息
	if (main==MDM_GF_GAME)
	{
		//效验状态
		ASSERT(mIClientKernelSink!=0);
		if (mIClientKernelSink==0) 
			return false;
		return mIClientKernelSink->OnEventGameMessage(sub,data,dataSize);
	}

	//内核处理
	if (main==MDM_GF_FRAME)
	{
		switch (sub)
		{
		case SUB_GF_USER_CHAT:			//用户聊天
			{
				return OnSocketSubUserChat(data,dataSize);
			}
		case SUB_GR_TABLE_TALK:			//用户聊天
			{
				return OnSocketSubUserTalk(data,dataSize);
			}
		case SUB_GF_USER_EXPRESSION:	//用户表情
			{
				return OnSocketSubExpression(data,dataSize);
			}
		case SUB_GF_GAME_STATUS:		//游戏状态
			{
				return OnSocketSubGameStatus(data,dataSize);
			}
		case SUB_GF_GAME_SCENE:			//游戏场景
			{
				return OnSocketSubGameScene(data,dataSize);
			}
		case SUB_GF_LOOKON_STATUS:		//旁观状态
			{
				return OnSocketSubLookonStatus(data,dataSize);
			}
		case SUB_GF_SYSTEM_MESSAGE:		//系统消息
			{
				return OnSocketSubSystemMessage(data,dataSize);
			}
		case SUB_GF_ACTION_MESSAGE:		//动作消息
			{
				return OnSocketSubActionMessage(data,dataSize);
			}
		case SUB_GF_USER_READY:			//用户准备
			{
				if(m_pMeUserItem ==0 || m_pMeUserItem->GetUserStatus()>=US_READY)
					return true;
				SendUserReady(0,0);
				if (mIClientKernelSink)
					mIClientKernelSink->OnGFMatchWaitTips(0);
				return true;
			}
		case SUB_GR_MATCH_INFO:				//比赛信息
			{
				if (!mIClientKernelSink)
					return true;

				return true;
			}
		case SUB_GR_MATCH_WAIT_TIP:			//等待提示
			{
				if (!mIClientKernelSink)
					return true;

				//设置参数
				if(dataSize==0)
				{
					mIClientKernelSink->OnGFMatchWaitTips(0);
				}

				return true;
			}
		case SUB_GR_MATCH_RESULT:			//比赛结果
			{
				//设置参数
				if (!mIClientKernelSink)
					return true;


				return true;
			}
		}

		return true;
	}

	return false;
}
