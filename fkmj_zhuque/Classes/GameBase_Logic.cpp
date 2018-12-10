#include "GameBase.h"

#include "GameLib.h"

#include "IServerItem.h"

//旁观消息
bool GameBase::OnEventLookonMode(void* data, int dataSize)
{
	return true;
}
//场景消息
bool GameBase::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	return true;
}
//场景消息
bool GameBase::OnEventGameMessage(int sub, void* data, int dataSize)
{
	std::vector<NET_CBInfo*> kCallFun;

	NET_CBInfoList::iterator itor1 = m_kCBInfoList.begin();
	while (itor1 != m_kCBInfoList.end())
	{
		if (itor1->iIdex == sub)
		{
			kCallFun.push_back(&(*itor1));
		}
		itor1++;
	}

	std::vector<NET_CBInfo*>::iterator itor2 = kCallFun.begin();
	while (itor2 != kCallFun.end())
	{
		utility::log((*itor2)->kCallFunName.c_str());
		((*itor2)->pCallBack)(data,dataSize);
		itor2++;
	}
	return true;
}

void GameBase::sendReady()
{
	IServerItem::get()->SendUserReady(0,0);
}
//发送函数
bool GameBase::SendSocketData( word wSubCmdID)
{
	utility::log("GameBase::SendSocketData ",(int)wSubCmdID);
	return IServerItem::get()->SendSocketData(MDM_GF_GAME,wSubCmdID);
}
//发送函数
bool GameBase::SendSocketData( word wSubCmdID, void * data, word dataSize)
{
	return IServerItem::get()->SendSocketData(MDM_GF_GAME,wSubCmdID,data,dataSize);
}

void GameBase::addNetCB(int iIdex,cocos2d::Ref* pScence,std::function<void(void*,int)> pCallBack,std::string kCallName)
{
	NET_CBInfo kInfo;
	kInfo.iIdex = iIdex;
	kInfo.pCallBack = pCallBack;
	kInfo.pSrc1 = pScence;
	kInfo.kCallFunName = kCallName;
	m_kCBInfoList.push_back(kInfo);
}