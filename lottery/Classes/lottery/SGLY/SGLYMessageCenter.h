#ifndef __SGLY_MESSAGE_CENTER_H__
#define __SGLY_MESSAGE_CENTER_H__

#include "cocos2d.h"
#include "BaseGameLayer.h"
#include "GlobalDef.h"

USING_NS_CC;

/*
**	@brief	水果乐园——消息处理中心
*/
class SGLYMessageCenter: public BaseGameLayer
{
public:
	SGLYMessageCenter();
	~SGLYMessageCenter();
	CREATE_FUNC(SGLYMessageCenter);

public://继承的子类方法

	//	处理进入游戏
	void dealEnterGame(Ref* pSender);

	//	处理玩家信息
	void dealPlayerInfo(Ref* pSender);

	//	处理游戏消息
	void dealMessageGame(Ref* pSender);

private:
	bool init();
	
	//	更新状态
	void dealGameState(tagUserData eData);

private://通讯接口

	//	实现游戏消息
	bool OnEventGameMessage(WORD wSubCmdID, void* pData, WORD wDataSize);

	//	实现游戏场景
	void onMessageScene(unsigned char cbStation,const void* pBuffer,unsigned short wDataSize);

	//	中途入桌——空闲状态
	void OnGameSceneFree(const void *pBuffer, WORD wDataSize);

	//	中途入桌——下注阶段
	void OnGameScenePlay(const void *pBuffer, WORD wDataSize);

	//	中途入桌——结算阶段
	void OnGameScenePlayEnd(const void *pBuffer, WORD wDataSize);

	//	游戏空闲
	bool OnSubGameFree(const void *pBuffer, WORD wDataSize);

	//	游戏开始
	bool OnSubGameStart(const void *pBuffer, WORD wDataSize);

	//	用户下注
	bool OnSubPlaceJetton(const void *pBuffer, WORD wDataSize);

	//	申请做庄
	bool OnSubUserApplyBanker(const void *pBuffer, WORD wDataSize);

	//	取消做庄
	bool OnSubUserCancelBanker(const void *pBuffer, WORD wDataSize);

	//	切换庄家
	bool OnSubChangeBanker(const void *pBuffer, WORD wDataSize);

	//	游戏结束
	bool OnSubGameEnd(const void *pBuffer, WORD wDataSize);

	//	游戏记录
	bool OnSubGameRecord(const void *pBuffer, WORD wDataSize);

	//	上庄列表
	bool OnApplyBankerList(const void *pBuffer, WORD wDataSize);

	//	下注失败
	bool onBetFailed(const void *pBuffer, WORD wDataSize);

	//	更新筹码
	bool OnSubUpdateChip(const void *pBuffer, WORD wDataSize);

	//	猜大小
	bool OnSubGuessBigSmall(const void *pBuffer, WORD wDataSize);

};

#endif  // __SGLY_MESSAGE_CENTER_H__