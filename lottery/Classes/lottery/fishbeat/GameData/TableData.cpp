#include "TableData.h"
#include "GlobalDef.h"
#include "CMD_Fish.h"

TableData::TableData(void)
{
	m_pIClientItems = NULL;
	m_bLockerFlag=false;						// 锁标志
	m_bPlayStatus=false;						// 游戏标志
}

TableData::~TableData(void)
{
	if (m_pIClientItems != NULL)
	{
		delete []m_pIClientItems;
		m_pIClientItems = NULL;
	}
}
//空椅子数
WORD TableData::GetNullChairCount(WORD & wNullChairID)
{
	//设置变量
	wNullChairID=INVALID_CHAIR;
	WORD wChairCount = 1;//GameRoom::GetInstance()->GetTableFrame()->GetChairCount();

	//寻找位置
	for (WORD i=0;i<wChairCount;i++)
	{
		if (m_pIClientItems[i]==NULL)
		{
			wNullChairID=i;
			return wNullChairID;
		}
	}
	return INVALID_TABLE;
}

//配置函数
void TableData::InitTableView(WORD wTableID, WORD wChairCount)
{
	if (m_pIClientItems != NULL)
	{
		delete []m_pIClientItems;
		m_pIClientItems = NULL;
	}


	// 获取房间的椅子数量
	WORD wTableChairCount = NS_lrbyFish::BY_GAME_PLAYER;//GameRoom::GetInstance()->GetTableFrame()->GetChairCount();
	m_pIClientItems = new CUserItem *[wTableChairCount];
	for (WORD i = 0; i < wTableChairCount; i++)
	{
		m_pIClientItems[i]=NULL;
	}
}
//获取用户
CUserItem * TableData::GetClientUserItem(WORD wChairID)
{
	WORD wChairCount = NS_lrbyFish::BY_GAME_PLAYER;//GameRoom::GetInstance()->GetTableFrame()->GetChairCount();
	if (wChairID < wChairCount)
		return m_pIClientItems[wChairID];
	return NULL;
}
//设置信息
bool TableData::SetClientUserItem(WORD wChairID, CUserItem * pIClientUserItem)
{
	WORD wChairCount = NS_lrbyFish::BY_GAME_PLAYER;//GameRoom::GetInstance()->GetTableFrame()->GetChairCount();
	if (wChairID < wChairCount)
		m_pIClientItems[wChairID] = pIClientUserItem;
	return true;
}
// 桌子状态
void TableData::setTableStatus(bool bPlaying, bool bLocker)
{
	m_bPlayStatus = bPlaying;
	m_bLockerFlag = bLocker;
}
//游戏标志
bool TableData::GetPlayFlag()
{
	return m_bPlayStatus;
}
//密码标志
bool TableData::GetLockerFlag()
{
	return m_bLockerFlag;
}
