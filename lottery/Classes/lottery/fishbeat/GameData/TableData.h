#ifndef _TABLE_VIEW_H_
#define _TABLE_VIEW_H_
#pragma once

#include "cocos2d.h"
#include "AppDelegate.h"
#include "UserItem.h"
#include "Define.h"

class TableData
{
	// / 存储信息
	CUserItem**			m_pIClientItems;			        // 桌子用户信息
	bool						m_bLockerFlag;						// 锁标志
	bool						m_bPlayStatus;						// 游戏标志

public:
	TableData(void);
	~TableData(void);
	//释放对象
	virtual void Release(){ delete this;}
	//接口查询
	//virtual void * QueryInterface(REFGUID Guid, DWORD dwQueryVer){return NULL;};

	//空椅子数
	virtual WORD GetNullChairCount(WORD & wNullChairID);
	//配置函数
	virtual void InitTableView(WORD wTableID, WORD wChairCount);
	//获取用户
	virtual CUserItem * GetClientUserItem(WORD wChairID);
	//设置信息
	virtual bool SetClientUserItem(WORD wChairID, CUserItem * pIClientUserItem);
	// 桌子状态
	virtual void setTableStatus(bool bPlaying, bool bLocker);
	//游戏标志
	virtual bool GetPlayFlag();
	//密码标志
	virtual bool GetLockerFlag();
};

#endif //_TABLE_VIEW_H_
