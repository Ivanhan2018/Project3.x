#ifndef _LKPY_GAME_CLIENTENGINE_H_ 
#define _LKPY_GAME_CLIENTENGINE_H_

#pragma once

#include "cocos2d.h"
#include "CMD_Fish.h"
#include "GameScene.h"
#include "LrbyClientView.h"
#include "GlobalDef.h"
#include "TableData.h"

#include "EntityMgr.h"
#include "MyNSString.h"
#include "Define.h"

class LrbyClientEngine : public Node
{
public:
	static int	switchViewChairID(unsigned int nChairID); 
//=========================================================================================
	//控件变量
protected:
	//IClientKernel*					m_pClientKernel;
	//用户变量
public:
	LrbyClientView*					m_pGameClientView;					//游戏视图
	TCHART							m_szAccounts[BY_GAME_PLAYER][LEN_NICKNAME];
	bool							m_bBackGroundSound;					//背景声音
	tagDDZUserInfoHead*                    m_userInfo[BY_GAME_PLAYER];			//用户信息
	bool                            m_isGameStart;
	int								m_MyChair;
	bool							m_isMyself;
	//游戏变量
public:
	char							m_userChat[LEN_USER_CHAT * 2];		//聊天内容
	BYTE							m_userChatID;						//聊天内容
public:
	LrbyClientEngine(void);
	virtual~LrbyClientEngine(void);

	CREATE_FUNC(LrbyClientEngine);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
public:
	//获取内核
	//IClientKernel* getClientKernel()const{return m_pClientKernel;};
	//释放对象
	virtual void Release(){ 
		delete this;
	}
	//接口查询
	//virtual void * QueryInterface(REFGUID Guid, DWORD dwQueryVer){return NULL;}
public:
//	void AllowFire(bool allow) { allow_fire_ = allow; }

	//消息处理

	//消息函数  add by zengxin
	void onMessageGame(Object* obj);			
	void onMessageScene(Object* obj);		
	void onUpdateTable(Object* obj);		
	void onUpdataMatch(Object* obj);		//this Messager is userd to change gear
	void onViewMessage(Object* obj);
	//bool onSceneMessage(unsigned char cbStation,const void* pBuffer,WORD wDataSize);
	//bool onGameMessage(WORD wSubCmdID, const void* pBuffer, WORD wDataSize);

	void onUpdateNotification();

protected:
	bool OnSubExchangeFishScore(void* data, WORD data_size);	//换打鱼用的金币
	bool OnSubSwitchScene(void* data, WORD data_size);			//场景切换
	bool OnSubHitFishLK(void* data, WORD data_size);			//		
	bool OnSubCatSweepFishResult(void* data, WORD data_size);	//
	bool OnSubCatSweepFish(void* data, WORD data_size);			//
	bool OnSubLockTimeout(void* data, WORD data_size);			//锁屏时间结束
	bool OnSubBulletIonTimeout(void* data, WORD data_size);		//Ion子弹时间结束
	bool OnSubCatchFish(void* data, WORD data_size);			//捕获鱼
	bool OnSubUserFire(void* data, WORD data_size);				//玩具开火
	bool OnSubFishTrace(void* data, WORD data_size);			//新的鱼群生成
	bool OnSubSceneEnd(void* data, WORD data_size);				//切换场景结束
	bool OnSubStockOperateResult(void* data, WORD data_size);	//
	bool OnSubGameConfig(void* data, WORD data_size);			//游戏配置信息
	bool OnOnLineAward(void* data, WORD data_size);				//游戏在线奖励
	bool OnSubTimeStamp(void* data, WORD data_size);			//时间戳<by hxh>
	// 开始消息;
	void Start();
public :
	// 强退
	void CloseGameView();
	// 初始化游戏视图
	virtual void* InitGameView();
	// 获取游戏视图
	virtual void* GetGameView();
	//
	virtual DWORD GetGameVersion();
	// 游戏初始化
	virtual bool OnInitClientKernel(TableData* clientKernel);
	// 
	void ReleaseGameView();
	// 救济金结果
	virtual bool OnGetAlmsResult(BYTE cbRetCode);
	// 旁观状态
	virtual void OnEventLookonMode(bool bLookonUser, void* pData, WORD wDataSize);
	// 网络消息
	virtual bool OnEventFrameMessage(WORD wSubCmdID, void* pData, WORD wDataSize);
	// 游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, void* pData, WORD wDataSize);
	// 场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, void * pData, WORD wDataSize);
	// 桌面滚动消息
	virtual bool AddGameTableMessage(TCHART* strMessage, WORD wType);
	// 比赛结果消息
	virtual bool OnEventMatchMessage(WORD wSubCmdID, void* pData, WORD wDataSize);
	// 用户进入
	virtual void OnEventUserEnter(tagDDZUserInfoHead* userInfo, WORD wChairID, bool bLookonMode);
	// 用户离开
	virtual void OnEventUserLeave(tagDDZUserInfoHead* userInfo, WORD wChairID, bool bLookonMode);
	// 用户积分
	virtual void OnEventUserScore(tagDDZUserInfoHead* userInfo, WORD wChairID, bool bLookonMode);
	// 用户状态
	virtual void OnEventUserStatus(tagDDZUserInfoHead* userInfo, WORD wChairID, bool bLookonMode);
	// 用户会员
	virtual void OnEventUserMemberOrder(tagDDZUserInfoHead* userInfo, WORD wChairID,bool bLookonMode);
	// 桌子状态
	virtual void OnEventTableStatus(BYTE cbTableStatus);
	// 重置框架
	virtual void ResetGameFrame();
	// 消息显示与处理
	virtual bool OnMessage(WORD wType, const char * szText);
	// 获取旁观
	virtual bool GetAllowLookon();
	// 定时器
	virtual bool OnTimer(unsigned int uTimerID, unsigned int uBindParameter);
	// 申请订单结果
	virtual bool OnApplyOrderResult(signed int nResultCode, uint16_t wCardTotal, uint32_t dwOrderAmount,const char* lpszOrderID,TCHART* lpszDesc);
	// 支付结果
	virtual bool OnPayResult(signed int nResultCode, uint8_t cbScoreType, LONGLONG lScore,TCHART* lpszDesc);
	// 系统消息
	virtual bool OnSystemMessage(WORD wType, TCHART* strMessage);
};

#endif //_LKPY_GAME_CLIENTENGINE_H_
