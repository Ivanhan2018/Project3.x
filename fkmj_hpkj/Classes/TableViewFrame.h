#ifndef _TableViewFrame_H_
#define _TableViewFrame_H_

#include <vector>
#include "IClientUserItem.h"
#include "PlatformHeader.h"
#include "ITableViewFrame.h"
//////////////////////////////////////////////////////////////////////////////////
//桌子视图
class CTableView : public ITableView
{
	//桌子属性
protected:
	//桌子标志
	bool							mIsLocker;							//密码标志
	bool							mIsPlaying;							//游戏标志
	bool							mIsFocusFrame;						//框架标志

	//桌子状态
	word							mWatchCount;						//旁观数目
	dword							mTableOwnerID;						//桌主索引

	//属性变量
	word							mTableID;							//桌子号码
	word							mChairCount;						//椅子数目
	IClientUserItem *				mIClientUserItem[MAX_CHAIR];		//用户信息

	//组件接口
protected:
	ITableViewFrame *				mITableViewFrame;					//桌子接口

	//函数定义
public:
	//构造函数
	CTableView();
	//析构函数
	virtual ~CTableView();

	//功能接口
public:
	//空椅子数
	virtual word GetNullChairCount(word & wNullChairID,dword wUserID);
	//配置函数
	virtual void InitTableView(word wTableID, word wChairCount, ITableViewFrame * pITableViewFrame);

	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(word wChairID);
	//设置信息
	virtual bool SetClientUserItem(word wChairID, IClientUserItem * pIClientUserItem);

	//查询接口
public:
	//游戏标志
	virtual bool GetPlayFlag() { return mIsPlaying; }
	//密码标志
	virtual bool GetLockerFlag() { return mIsLocker; }

	//状态接口
public:
	//焦点框架
	virtual void SetFocusFrame(bool bFocusFrame);
	//桌子状态 
	virtual void SetTableStatus(bool bPlaying, bool bLocker);
};

//////////////////////////////////////////////////////////////////////////////////

//数组定义
typedef std::vector<CTableView *>		CTableViewArray;					//桌子数组

//桌子框架
class CTableViewFrame 
	: public ITableViewFrame
{
	//友元定义
	friend class CTableView;

	//属性变量
protected:
	word							mTableCount;						//游戏桌数
	word							mChairCount;						//椅子数目

	//控件变量
public:
	CTableViewArray					mTableViewArray;					//桌子数组

	//比赛变量
public:
	dword							mMatchWaittingCount;				//等待人数
	dword							mMatchTotalUser;					//参赛人数
	dword							mMatchBestRank;						//最好成绩
	dword							mMatchJoinCount;					//参数次数
	byte							mMatchStatus;						//比赛状态
	tagMatchDesc					mMatchDesc;							//信息描述

	//函数定义
public:
	//构造函数
	CTableViewFrame();
	//析构函数
	virtual ~CTableViewFrame();

	//配置接口
public:
	//配置桌子
	virtual bool ConfigTableFrame(word wTableCount, word wChairCount, word wServerID);

	//信息接口
public:
	//桌子数目
	virtual word GetTableCount() { return mTableCount; }
	//椅子数目
	virtual word GetChairCount() { return mChairCount; }
	
	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(word wTableID, word wChairID);
	//设置信息
	virtual bool SetClientUserItem(word wTableID, word wChairID, IClientUserItem * pIClientUserItem);

	//状态接口
public:
	//游戏标志
	virtual bool GetPlayFlag(word wTableID);
	//密码标志
	virtual bool GetLockerFlag(word wTableID);
	//焦点框架
	virtual void SetFocusFrame(word wTableID, bool bFocusFrame);
	//桌子状态 
	virtual void SetTableStatus(word wTableID, bool bPlaying, bool bLocker);
	//桌子状态 
	virtual void SetTableStatus(bool bWaitDistribute);

	//视图控制
public:
	//桌子可视
	virtual bool VisibleTable(word wTableID);
	//闪动桌子
	virtual bool FlashGameTable(word wTableID);
	//闪动椅子
	virtual bool FlashGameChair(word wTableID, word wChairID);

	//功能接口
public:
	//更新桌子
	virtual bool UpdateTableView(word wTableID);
	//获取桌子
	virtual ITableView * GetTableViewItem(word wTableID);
	//空椅子数
	virtual word GetNullChairCount(word wTableID, word & wNullChairID,dword wUserID);

	//比赛函数
public:
	//比赛状态
	void SetMatchStatus(byte cbMatchStatus);
};

//////////////////////////////////////////////////////////////////////////////////

#endif // _TableViewFrame_H_