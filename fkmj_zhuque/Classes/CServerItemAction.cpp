#include "CServerItem.h"
#include "Convert.h"

//查找桌子
bool CServerItem::FindGameTable(tagFindTable & FindInfo)
{

	//变量定义
	word wNullCount=0;
	ITableView * pTableView=NULL;
	word wChairUser=m_TableFrame.GetChairCount();
	word wMeTableID=m_pMeUserItem->GetTableID();

	//搜索桌子
	for (word i=0;i<m_TableFrame.GetTableCount();i++)
	{
		FindInfo.wResultTableID=(FindInfo.wBeginTableID+i)%m_TableFrame.GetTableCount();
		if (wMeTableID!=FindInfo.wResultTableID)
		{
			//获取桌子
			pTableView=m_TableFrame.GetTableViewItem(FindInfo.wResultTableID);
			ASSERT(pTableView!=NULL);

			//判断是否开始游戏
			if (pTableView->GetPlayFlag()==true) continue;

			//过滤密码
			bool bTablePass=pTableView->GetLockerFlag();
			if ((FindInfo.bFilterPass==true)&&(bTablePass==true)) continue;

			//寻找空位置
			wNullCount=pTableView->GetNullChairCount(FindInfo.wResultChairID,m_pMeUserItem->GetUserID());

			//判断数目
			if (wNullCount>0)
			{
				//效验规则
				char strDescribe[256];
				if (EfficacyTableRule(FindInfo.wResultTableID,FindInfo.wResultChairID,false,strDescribe)==false)
				{
					continue;
				}

				if ((FindInfo.bNotFull==true)&&(wNullCount<wChairUser)) return true;
				if ((FindInfo.bOneNull==true)&&(wNullCount==1)) return true;
				if ((FindInfo.bTwoNull==true)&&(wNullCount==2)) return true;
				if ((FindInfo.bAllNull==true)&&(wNullCount==wChairUser)) return true;
			}
		}
	}

	//设置数据
	FindInfo.wResultTableID=INVALID_TABLE;
	FindInfo.wResultChairID=INVALID_CHAIR;

	return false;
}

//快速加入
bool CServerItem::PerformQuickSitDown()
{
	if (!IsService())
		return false;

	//自己状态
	if (m_pMeUserItem->GetUserStatus()>=US_PLAYING)
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(("InGame"), DLG_MB_OK);
	}

	//先搜索桌子不全空的
	tagFindTable FindInfo;
	FindInfo.bAllNull=false;
	FindInfo.bFilterPass=true;
	FindInfo.bNotFull=true;
	FindInfo.bOneNull=true;
	FindInfo.bTwoNull=(m_TableFrame.GetChairCount()!=2);
	FindInfo.wBeginTableID=mFindTableID+1;
	FindInfo.wResultTableID=INVALID_TABLE;
	FindInfo.wResultChairID=INVALID_CHAIR;
	bool bSuccess=FindGameTable(FindInfo);
	mFindTableID=FindInfo.wResultTableID;

	//搜索全部游戏桌
	if (bSuccess==false)
	{
		FindInfo.bAllNull=true;
		FindInfo.bFilterPass=true;
		FindInfo.bNotFull=true;
		FindInfo.bOneNull=true;
		FindInfo.bTwoNull=true;
		FindInfo.wBeginTableID=mFindTableID+1;
		FindInfo.wResultTableID=INVALID_TABLE;
		FindInfo.wResultChairID=INVALID_CHAIR;
		bSuccess=FindGameTable(FindInfo);
		mFindTableID=FindInfo.wResultTableID;
	}

	//结果判断
	if (bSuccess==true)
	{
		//效验数据
		ASSERT(FindInfo.wResultTableID!=INVALID_TABLE);
		ASSERT(FindInfo.wResultChairID!=INVALID_CHAIR);


		//自动坐下
		PerformSitDownAction(mFindTableID,FindInfo.wResultChairID,NULL);
	}
	else
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString("No Table", DLG_MB_OK);
	}

	return true;
} 


//执行旁观
bool CServerItem::PerformLookonAction(word wTableID, word wChairID)
{
	PLAZZ_PRINTF("CServerItem::PerformLookonAction...\n");

	if (!IsService())
		return false;

	//效验数据
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//状态过滤
	if (mServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//自己状态
	if (m_pMeUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString((">=US_PLAYING"), DLG_MB_OK);
		return false;
	}

	//权限判断
	if (CUserRight::CanLookon(mUserAttribute.dwUserRight)==false)
	{
		//提示消息
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(("CanLookon == false"), DLG_MB_OK);
		return false;
	}

	//清理界面
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID!=MAX_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_TableFrame.GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pMeUserItem) m_TableFrame.SetClientUserItem(m_wReqTableID,m_wReqChairID,0);
	}

	//设置变量
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	mFindTableID=INVALID_TABLE;

	//设置界面
	m_TableFrame.VisibleTable(wTableID);

	PLAZZ_PRINTF("CServerItem::PerformLookonAction send\n");
	//发送命令
	SendLookonPacket(wTableID,wChairID);

	return true;
}

//执行起立
bool CServerItem::PerformStandUpAction()
{
	PLAZZ_PRINTF("CServerItem::PerformStandUpAction...\n");

	if (!IsService())
		return false;

	//状态过滤
	if (mServiceStatus!=ServiceStatus_ServiceIng) return false;


	//设置界面
	PLAZZ_PRINTF("CServerItem::PerformStandUpAction send\n");
	//发送命令
	SendStandUpPacket(m_pMeUserItem->GetTableID(),m_pMeUserItem->GetChairID(),FALSE);

	return true;
}

//执行坐下
bool CServerItem::PerformSitDownAction(word wTableID, word wChairID, bool bEfficacyPass, const char * psw)
{
	PLAZZ_PRINTF("CServerItem::PerformSitDownAction...\n");

	if (!IsService())
		return false;

	//状态过滤
	if (mServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqTableID==wTableID)) return false;
	if ((m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID==wChairID)) return false;

	//密码判断
	char szPassword[LEN_PASSWORD] = {0};

	//设置变量
	m_wReqTableID=0;
	m_wReqChairID=0;
	mFindTableID=INVALID_TABLE;


	PLAZZ_PRINTF("CServerItem::PerformSitDownAction send... wTableID =%d  wCharID =%d \n,", wTableID, wChairID);
	//发送命令
	SendSitDownPacket(INVALID_TABLE,INVALID_CHAIR,szPassword);

	return true;
}

//执行购买
bool CServerItem::PerformBuyProperty(byte cbRequestArea,const char* pszNickName, word wItemCount, word wPropertyIndex)
{
	PLAZZ_PRINTF("CServerItem::PerformBuyProperty...\n");

	if (!IsService())
		return false;

	return true;

}
