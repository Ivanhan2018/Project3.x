#include "TableFrame.h"
#include "UserItem.h"
#include "GlobalDef.h"
#include "UINotificationCenter.h"


CTableFrame::CTableFrame(void)
{
	m_nTableCount = 0;
	m_bWaitDistribute=false;
	m_TableView = NULL;
	m_nEnterTableID = INVALID_TABLE;
	m_lServerRule = 0;
}


CTableFrame::~CTableFrame(void)
{
	if(m_TableView!=NULL)
	{
		delete []m_TableView;
		m_TableView = NULL;
	}
}
bool CTableFrame::ConfigTableFrame(WORD nTableCount, WORD nChairCount,DWORD lServerRule, WORD nServerType, WORD nServerID)
{	
		m_nTableCount = nTableCount;
		m_nChairCount = nChairCount;
		m_lServerRule = lServerRule;
		if(m_TableView!=NULL)
		{
			delete []m_TableView;
			m_TableView = NULL;
		}
		m_TableView = new TableData[m_nTableCount];
		for (WORD i = 0; i < nTableCount; i++)
		{
			m_TableView[i].InitTableView(i+1, nChairCount);
		}
		//		
	return true;
}
// 桌子数目
WORD CTableFrame::GetTableCount()
{
	return m_nTableCount;
}

// 椅子数目
WORD CTableFrame::GetChairCount()
{
	return m_nChairCount;
}

// 获取用户
CUserItem* CTableFrame::GetClientUserItem(WORD nTableID, WORD nChairID)
{
	if (nTableID < m_nTableCount)
		return m_TableView[nTableID].GetClientUserItem(nChairID);
	return NULL;
}

// 设置信息
bool CTableFrame::SetClientUserItem(WORD nTableID, WORD nChairID,TableData* clientUserItem)
{
	if (nTableID < m_nTableCount)
	{
		bool bRet= m_TableView[nTableID].SetClientUserItem(nChairID,(CUserItem*)clientUserItem);
		CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GR_UpdateTable,nTableID);
		NotificationCenter::getInstance()->postNotification("ongameroom", pNotifRef);
		return true;
	}
	return false;
}

// 游戏标志
bool CTableFrame::GetPlayFlag(WORD nTableID)
{
	if (nTableID < m_nTableCount)
		return m_TableView[nTableID].GetPlayFlag();
	return false;
}

// 密码标志
bool CTableFrame::GetLockerFlag(WORD nTableID)
{
	if (nTableID < m_nTableCount)
		return m_TableView[nTableID].GetLockerFlag();
	return false;
}

// 焦点框架
void CTableFrame::SetFocusFrame(WORD nTableID, bool bFocusFrame)
{

}

// 桌子状态
void CTableFrame::SetTableStatus(WORD nTableID, bool bPlaying, bool bLocker)
{
	if (nTableID < m_nTableCount)
	{
		m_TableView[nTableID].setTableStatus(bPlaying, bLocker);
		CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GR_UpdateTable,nTableID);
		NotificationCenter::getInstance()->postNotification("ongameroom", pNotifRef);
	}
}

//桌子状态 (是否等待分桌)
void CTableFrame::SetTableStatus(bool bWaitDistribute)
{
	m_bWaitDistribute=bWaitDistribute;
	//CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GR_UpdateTable,nTableID);
	//NotificationCenter::getInstance()->postNotification("ongameroom", pNotifRef);
	return;
}
// 桌子可视
bool CTableFrame::VisibleTable(WORD nTableID)
{
	return true;
}

// 更新游戏桌视图
bool CTableFrame::UpdateTableView(WORD nTableID)
{
	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GR_UpdateTable,nTableID);
	NotificationCenter::getInstance()->postNotification("ongameroom", pNotifRef);
	return true;
}

// 获取桌子
TableData* CTableFrame::GetTableViewItem(WORD nTableID)
{
	if (nTableID < m_nTableCount)
		return &m_TableView[nTableID];
	return NULL;
}

// 空椅子数0位置标示总数，1位置标示空位
WORD CTableFrame::GetNullChairCount(WORD nTableID, WORD & wNullChairID)
{
	if (nTableID < m_nTableCount)
	{
		m_TableView[nTableID].GetNullChairCount(wNullChairID);
		if(wNullChairID!=INVALID_CHAIR)
			return nTableID;//m_TableView[nTableID].GetNullChairCount(wNullChairID);
	}
	return INVALID_TABLE;
}
