#ifndef __ROOM_FRAMESINK_H__
#define __ROOM_FRAMESINK_H__

#pragma once
#include "cocos2d.h"
#include "TableData.h"
#include "Define.h"

#define UPDATA_ROOM_TABLE 123

class CTableFrame 
{
private:
	// 信息存储
	WORD					m_nTableCount;
	bool                    m_bWaitDistribute;                  //等待分配
	WORD					m_nChairCount;
	TableData*				m_TableView;
	DWORD					m_lServerRule;
public:
	WORD					m_nEnterTableID;
	CTableFrame(void);
	~CTableFrame(void);
public:
	// 释放对象
	virtual void Release(){ delete this;}
	// 接口查询
	//virtual void * QueryInterface(REFGUID Guid, DWORD dwQueryVer){return NULL;};
	// 配置桌子
	virtual bool ConfigTableFrame(WORD nTableCount, WORD nChairCount,DWORD lServerRule, WORD nServerType, WORD nServerID);
	// 桌子数目
	virtual WORD GetTableCount();
	// 椅子数目
	virtual WORD GetChairCount();
	// 获取用户
	virtual CUserItem* GetClientUserItem(WORD nTableID, WORD nChairID);
	// 设置信息
	virtual bool SetClientUserItem(WORD nTableID, WORD nChairID,TableData* clientUserItem);
	// 游戏标志
	virtual bool GetPlayFlag(WORD nTableID);
	// 密码标志
	virtual bool GetLockerFlag(WORD nTableID);
	// 焦点框架
	virtual void SetFocusFrame(WORD nTableID, bool bFocusFrame);
	// 桌子状态
	virtual void SetTableStatus(WORD nTableID, bool bPlaying, bool bLocker);
	//桌子状态 (是否等待分桌)
	virtual void SetTableStatus(bool bWaitDistribute);
	// 桌子可视
	virtual bool VisibleTable(WORD nTableID);
	// 更新游戏桌视图
	virtual bool UpdateTableView(WORD nTableID);
	// 获取桌子
	virtual TableData* GetTableViewItem(WORD nTableID);
	// 空椅子数0位置标示总数，1位置标示空位
	virtual WORD GetNullChairCount(WORD nTableID, WORD & wNullChairID);
	// 桌子状态
	bool GetWaitDistribute() { return m_bWaitDistribute;};
	// 房间规则
	DWORD GetServerRule() { return m_lServerRule; };
};

#endif //__ROOM_FRAMESINK_H__
