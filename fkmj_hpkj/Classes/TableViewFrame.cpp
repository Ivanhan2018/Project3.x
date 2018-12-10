#include "TableViewFrame.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableView::CTableView()
{
	//组件接口
	mITableViewFrame=0;

	//桌子标志
	mIsLocker		= false;
	mIsPlaying		= false;
	mIsFocusFrame	= false;

	//桌子状态
	mWatchCount		= 0;
	mTableOwnerID	= 0;

	//属性变量
	mTableID		= 0;
	mChairCount		= 0;
	
	zeromemory(mIClientUserItem, sizeof(mIClientUserItem));
	return;
}

//析构函数
CTableView::~CTableView()
{
}

//空椅子数
word CTableView::GetNullChairCount(word & wNullChairID,dword wUserID)
{
	//设置变量
	wNullChairID=INVALID_CHAIR;

	//寻找位置
	word wNullCount=0;
	for (word i=0;i<mChairCount;i++)
	{
		if (mIClientUserItem[i]==0 )
		{
			//设置数目
			wNullCount++;

			//设置结果
			if (wNullChairID==INVALID_CHAIR) wNullChairID=i;
		}
	}
	for (word i=0;i<mChairCount;i++)
	{
		if (mIClientUserItem[i] && mIClientUserItem[i]->GetUserID() == wUserID)
		{
			//设置结果
			if (wNullChairID==INVALID_CHAIR) wNullChairID=i;
		}
	}

	return wNullCount;
}

//配置函数
void CTableView::InitTableView(word wTableID, word wChairCount, ITableViewFrame * pITableViewFrame)
{
	//设置属性
	mTableID=wTableID;
	mChairCount=wChairCount;

	//设置接口
	mITableViewFrame=pITableViewFrame;
	return;
}

//获取用户
IClientUserItem * CTableView::GetClientUserItem(word wChairID)
{
	//效验参数
	ASSERT(wChairID<mChairCount);
	if (wChairID>=mChairCount) return 0;

	//获取用户
	return mIClientUserItem[wChairID];
}

//设置信息
bool CTableView::SetClientUserItem(word wChairID, IClientUserItem * pIClientUserItem)
{
	//效验参数
	ASSERT(wChairID<mChairCount);
	if (wChairID>=mChairCount) return false;

	//设置用户
	mIClientUserItem[wChairID]=pIClientUserItem;

	//更新界面
	mITableViewFrame->UpdateTableView(mTableID);

	return true;
}

//桌子状态 
void CTableView::SetTableStatus(bool bPlaying, bool bLocker)
{
	//设置标志
	if ((mIsLocker!=bLocker)||(mIsPlaying!=bPlaying))
	{
		//设置变量
		mIsLocker=bLocker; 
		mIsPlaying=bPlaying;

		//更新界面
		mITableViewFrame->UpdateTableView(mTableID);
	}

	return;
}

//焦点框架
void CTableView::SetFocusFrame(bool bFocusFrame)
{
	//设置标志
	if (mIsFocusFrame!=bFocusFrame)
	{
		//设置变量
		mIsFocusFrame=bFocusFrame;

		//更新界面
		mITableViewFrame->UpdateTableView(mTableID);
	}

	return;
}
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableViewFrame::CTableViewFrame()
{
	//属性变量
	mTableCount=0;
	mChairCount=0;

	//比赛变量
	mMatchTotalUser=0;
	mMatchWaittingCount=0;
	mMatchStatus=MS_NULL;
	return;
}

//析构函数
CTableViewFrame::~CTableViewFrame()
{
	//删除桌子
	for (int i=0, l = (int)mTableViewArray.size(); i<l ;i++)
	{
		CTableView * pTableView=mTableViewArray[i];
		if (pTableView!=0) 
			SafeDelete(pTableView);
	}

	//删除数组
	mTableViewArray.clear();

	return;
}


//配置函数
bool CTableViewFrame::ConfigTableFrame(word wTableCount, word wChairCount, word wServerID)
{
	//效验参数
	ASSERT(wChairCount<=MAX_CHAIR);
	
	//设置变量
	mTableCount=wTableCount;
	mChairCount=wChairCount;
	mTableViewArray.resize(mTableCount);
	zeromemory(&mTableViewArray[0],mTableCount*sizeof(CTableView *));

	//创建桌子
	for (word i=0;i<mTableCount;i++)
	{
		mTableViewArray[i]=new CTableView;
		mTableViewArray[i]->InitTableView(i,wChairCount,this);
	}


	return true;
}

//获取用户
IClientUserItem * CTableViewFrame::GetClientUserItem(word wTableID, word wChairID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=0);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取用户
	if (pITableView!=0)
	{
		return pITableView->GetClientUserItem(wChairID);
	}

	return 0;
}

//设置信息
bool CTableViewFrame::SetClientUserItem(word wTableID, word wChairID, IClientUserItem * pIClientUserItem)
{
	ITableView * pITableView=GetTableViewItem(wTableID);
	if (pITableView!=0) pITableView->SetClientUserItem(wChairID,pIClientUserItem);
	return true;
}

//游戏标志
bool CTableViewFrame::GetPlayFlag(word wTableID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=0);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取标志
	if (pITableView!=0)
	{
		return pITableView->GetPlayFlag();
	}

	return false;
}

//密码标志
bool CTableViewFrame::GetLockerFlag(word wTableID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=0);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取标志
	if (pITableView!=0)
	{
		return pITableView->GetLockerFlag();
	}

	return false;
}

//焦点框架
void CTableViewFrame::SetFocusFrame(word wTableID, bool bFocusFrame)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=0);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//设置标志
	if (pITableView!=0) pITableView->SetFocusFrame(bFocusFrame);

	return;
}


//桌子状态 
void CTableViewFrame::SetTableStatus(word wTableID, bool bPlaying, bool bLocker)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=0);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//设置标志
	if (pITableView!=0) pITableView->SetTableStatus(bPlaying,bLocker);

	return;
}

//桌子状态 
void CTableViewFrame::SetTableStatus(bool bWaitDistribute)
{
	//m_bWaitDistribute=bWaitDistribute;
	return;
}

//桌子可视
bool CTableViewFrame::VisibleTable(word wTableID)
{
	//效验参数
	ASSERT(wTableID<mTableCount);
	if (wTableID>=mTableCount) return false;

	return true;
}

//闪动桌子
bool CTableViewFrame::FlashGameTable(word wTableID)
{
	//获取桌子
	ITableView * pITableView=GetTableViewItem(wTableID);

	//错误判断
	if (pITableView==0)
	{
		ASSERT(FALSE);
		return false;
	}


	return true;
}

//闪动椅子
bool CTableViewFrame::FlashGameChair(word wTableID, word wChairID)
{
	//获取桌子
	ITableView * pITableView=GetTableViewItem(wTableID);

	//错误判断
	if (pITableView==0)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//更新桌子
bool CTableViewFrame::UpdateTableView(word wTableID)
{
	//获取桌子
	ITableView * pITableView=GetTableViewItem(wTableID);
	if (pITableView==0) return false;

	
	return true;
}

//获取桌子
ITableView * CTableViewFrame::GetTableViewItem(word wTableID)
{
	//获取桌子
	if (wTableID!=INVALID_TABLE)
	{
		//效验参数
		ASSERT(wTableID<(int)mTableViewArray.size());
		if (wTableID>=(int)mTableViewArray.size()) return 0;

		//获取桌子
		ITableView * pITableView=mTableViewArray[wTableID];

		return pITableView;
	}

	return 0;
}

//空椅子数
word CTableViewFrame::GetNullChairCount(word wTableID, word & wNullChairID,dword wUserID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=0);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取状态
	if (pITableView!=0)
	{
		return pITableView->GetNullChairCount(wNullChairID,wUserID);
	}

	return 0;
}

//比赛状态
void CTableViewFrame::SetMatchStatus(byte cbMatchStatus)
{
	mMatchStatus=cbMatchStatus;
}

//////////////////////////////////////////////////////////////////////////////////
