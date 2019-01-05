#include "SGLYMessageCenter.h"
#include "EntityMgr.h"
#include "MyNSString.h"
#include "SGLYGameScene.h"
#include "SGLYProtocol.h"
#include "SGLYAniLottery.h"
#include "SGLYTool.h"
#include "SGLYRes.h"
#include "SGLYManageAudio.h"
#include "ToolKit.h"

SGLYMessageCenter::SGLYMessageCenter()
{
}

SGLYMessageCenter::~SGLYMessageCenter()
{
}

bool SGLYMessageCenter::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!BaseGameLayer::init());

		bRet = true;
	} while (0);

	return bRet;
}

void SGLYMessageCenter::dealGameState(tagUserData eData)
{
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return;
	}

	tagUserData* pPlayerData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	if (pPlayerData->dwUserID != eData.dwUserID)
	{
		//	不是玩家自身，不做状态更新
		return;
	}

	switch (eData.cbUserStatus)
	{
	case US_NULL:	//	离开
	case US_FREE:
		{
// 			pScene->m_pPopupApplyList->removeListItem(pPlayerData->wChairID);
		}
		break;
	case US_SIT:
	case US_PLAYING:
		{
// 			pScene->m_pInfoBar->m_pPlayerName->setString(eData.szNickName);
// 			pScene->m_pInfoBar->updatePlayerGold(eData.lScore);
// 			if (eData.cbGender == 1)	//	男
// 			{
// 				pScene->m_pInfoBar->m_pPlayerFace->initWithSpriteFrameName(BRNNTextureName::s_brnn_face_man);
// 			}
// 			else
// 			{
// 				pScene->m_pInfoBar->m_pPlayerFace->initWithSpriteFrameName(BRNNTextureName::s_brnn_face_woman);
// 			}
// 			if (pScene->m_pGameLayer->getApplyState() != E_BRNN_EPPC_Apply_State_Banker)
// 			{
// 				pScene->m_pGameLayer->checkoutEnabledBtnChip(pPlayerData->lScore);
// 			}

		}
		break;
	case US_READY:
		{
		}
		break;
	case US_LOOKON:
		break;
	case US_OFFLINE:
		{
		}
		break;
	case US_AGAIN_GAME:
		break;
	case US_LEFT:
		{
		}
		break;
	default:
		break;
	}
}

void SGLYMessageCenter::dealEnterGame(Ref* pSender)
{

}

void SGLYMessageCenter::dealPlayerInfo(Ref* pSender)
{
	tagUserBuffer* tag = (tagUserBuffer*)pSender;
	DWORD dwUserID = tag->dwUserID;

	CUserItem* pUserItem = (CUserItem*)EntityMgr::instance()->roomFrame()->getUserItem(dwUserID);
	if (!pUserItem)
	{
		return;
	}

	tagUserData* pUserData = pUserItem->getUserData();
	if (!pUserData)
	{
		return;
	}
	dealGameState(*pUserData);
}

void SGLYMessageCenter::dealMessageGame(Ref* pSender)
{
	tagGameBuffer* tag = (tagGameBuffer*)pSender;
	int Applicead = tag->nDataSize;
	OnEventGameMessage(tag->wSubCmdID,tag->dateBuffer,tag->nDataSize);
}

bool SGLYMessageCenter::OnEventGameMessage(WORD wSubCmdID, void* pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SGLY_SUB_S_GAME_FREE:
		return OnSubGameFree(pData, wDataSize);
	case SGLY_SUB_S_GAME_START:
		return OnSubGameStart(pData, wDataSize);
	case SGLY_SUB_S_PLACE_JETTON:
		return OnSubPlaceJetton(pData, wDataSize);
	case SGLY_SUB_S_GAME_END:
		return OnSubGameEnd(pData, wDataSize);
	case SGLY_SUB_S_APPLY_BANKER:
		return OnSubUserApplyBanker(pData, wDataSize);
	case SGLY_SUB_S_CHANGE_BANKER:
		return OnSubChangeBanker(pData, wDataSize);
	case SGLY_SUB_S_CHANGE_USER_SCORE:
		break;
	case SGLY_SUB_S_SEND_RECORD:
		return OnSubGameRecord(pData, wDataSize);
	case SGLY_SUB_S_PLACE_JETTON_FAIL:
		onBetFailed(pData, wDataSize);
		break;
	case SGLY_SUB_S_CANCEL_BANKER:
		return OnSubUserCancelBanker(pData, wDataSize);
	case SGLY_SUB_S_BIG_SMALL:
		return OnSubGuessBigSmall(pData, wDataSize);
	case SGLY_SUB_S_CHIP_UPDATE:
		return OnSubUpdateChip(pData, wDataSize);
	case SGLY_SUB_S_ADMIN_COMMDN:
		break;
	case SGLY_SUB_C_GET_PLAYLIST:
		return OnApplyBankerList(pData, wDataSize);
	default:
		break;
	}
	return true;
}

void SGLYMessageCenter::onMessageScene(unsigned char cbStation,const void* pBuffer,unsigned short wDataSize)
{
	CCLOG("SGLYMessageCenter::onMessageScene cbStation=%d,wDataSize=%d", cbStation, wDataSize);
	switch(cbStation)
	{
	case GAME_STATUS_FREE://0
		{
			CCLOG("GAME_STATUS_FREE");
			OnGameSceneFree(pBuffer, wDataSize);
		}
		break;
	case GS_PLACE_JETTON:
		{
			CCLOG("GS_PLACE_JETTON");
			OnGameScenePlay(pBuffer, wDataSize);
		}
		break;
	case GS_GAME_END://100
		{
			CCLOG("GS_GAME_END");
			OnGameScenePlayEnd(pBuffer, wDataSize);
		}
		break;
	default:
		return;
	}
	EntityMgr::instance()->roomFrame()->setGameIsStart(true);
}

//	中途入桌——空闲状态
void SGLYMessageCenter::OnGameSceneFree(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_StatusFree) != wDataSize)
	{
		return;
	}
	SGLY_CMD_S_StatusFree* pMessageData = (SGLY_CMD_S_StatusFree*)pBuffer;
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return;
	}

	//	同步兑率
	SGLYTool::m_dwExchangeRate = pMessageData->dwChipRate;
	SGLYTool::m_llApplyBankerCondition = pMessageData->lApplyBankerCondition;

	WORD wBankerChairID = pMessageData->wBankerUser;
	LONGLONG llBankerChip = 0;
	//	时钟
	pScene->m_pClock->setTimerType(E_SGLY_Process_Idle);
	pScene->m_pClock->playCountdownTimer(pMessageData->cbTimeLeave);

	//	设置当前庄家椅子ID
	pScene->m_pGameLayer->setBankerChairID(wBankerChairID);

	if (wBankerChairID != INVALID_CHAIR)
	{
		llBankerChip = pMessageData->lCurrentJetton[wBankerChairID];
	}
	log(" OnGameSceneFree \n wBankerChairID = %d \n szBankerNickName = %s \n llBankerChip = %lld \n lBankerWinScore = %lld \n wBankerTime = %d",
		wBankerChairID,
		pMessageData->szBankerNickName,
		llBankerChip,
		pMessageData->lBankerWinScore,
		pMessageData->wBankerTime);

	//	庄家
	pScene->m_pInfoCenter->updateBankerInfo(
		wBankerChairID,
		pMessageData->szBankerNickName,
		llBankerChip,
		pMessageData->lBankerWinScore,
		pMessageData->wBankerTime);
}

//	中途入桌——下注阶段
void SGLYMessageCenter::OnGameScenePlay(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_StatusPlay) != wDataSize)
	{
		return;
	}
	SGLY_CMD_S_StatusPlay* pMessageData = (SGLY_CMD_S_StatusPlay*)pBuffer;
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return;
	}
	//	同步兑率
	SGLYTool::m_dwExchangeRate = pMessageData->dwChipRate;
	SGLYTool::m_llApplyBankerCondition = pMessageData->lApplyBankerCondition;

	WORD wBankerChairID = pMessageData->wBankerUser;
	LONGLONG llBankerChip = 0;

	//	设置当前庄家椅子ID
	pScene->m_pGameLayer->setBankerChairID(wBankerChairID);

	//	时钟
	pScene->m_pClock->setTimerType(E_SGLY_Process_Bet);
	pScene->m_pClock->playCountdownTimer(pMessageData->cbTimeLeave);

	for (int i = E_SGLY_Lottery_Bar; i < E_SGLY_Lottery_Null; i++)
	{
		pScene->m_pInfoCenter->updateTotalBet((E_SGLY_Lottery)i, SGLYTool::exchangeForChip(pMessageData->lTotalJettonScore[i]));
	}

	if (wBankerChairID != INVALID_CHAIR)
	{
		llBankerChip = pMessageData->lCurrentJetton[wBankerChairID];
	}
	log(" OnGameSceneFree \n wBankerChairID = %d \n szBankerNickName = %s \n llBankerChip = %lld \n lBankerWinScore = %lld \n cbBankerTime = %d",
		wBankerChairID,
		pMessageData->szBankerNickName,
		llBankerChip,
		pMessageData->lBankerWinScore,
		pMessageData->cbBankerTime);

	//	庄家
	pScene->m_pInfoCenter->updateBankerInfo(
		wBankerChairID,
		pMessageData->szBankerNickName,
		llBankerChip,
		pMessageData->lBankerWinScore,
		pMessageData->cbBankerTime);
}

//	中途入桌——结算阶段
void SGLYMessageCenter::OnGameScenePlayEnd(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_StatusPlay) != wDataSize)
	{
		return;
	}
	SGLY_CMD_S_StatusPlay* pMessageData = (SGLY_CMD_S_StatusPlay*)pBuffer;
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return;
	}
	//	同步兑率
	SGLYTool::m_dwExchangeRate = pMessageData->dwChipRate;
	SGLYTool::m_llApplyBankerCondition = pMessageData->lApplyBankerCondition;

	WORD wBankerChairID = pMessageData->wBankerUser;
	LONGLONG llBankerChip = 0;

	//	设置当前庄家椅子ID
	pScene->m_pGameLayer->setBankerChairID(wBankerChairID);

	//	时钟
	pScene->m_pClock->setTimerType(E_SGLY_Process_Lottery);
	pScene->m_pClock->playCountdownTimer(pMessageData->cbTimeLeave);

	pScene->m_pGameLayer->dealGameEnd();

	//	更新下注情况
	for (int i = E_SGLY_Lottery_Bar; i < E_SGLY_Lottery_Null; i++)
	{
		pScene->m_pInfoCenter->updateTotalBet((E_SGLY_Lottery)i, SGLYTool::exchangeForChip(pMessageData->lTotalJettonScore[i]));
	}

	//	显示高亮结果
	pScene->m_pGameLayer->setLotteryHighLight(pMessageData->cbWinArea, true);
	if (pMessageData->cbGoodLuckType != 0)	//0：吃分、1：大三元、2：大四喜、3：小三元、4：跑火车、5：随机打枪
	{
		
		for (int i = 0; i < pMessageData->cbPaoHuoCheCount; i++)
		{
			//	GoodLuck 高亮
			Sprite* pLottery = pScene->m_pGameLayer->getLottery(pMessageData->cbPaoHuoCheArea[i]);
			Sprite* pMask = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_HighLightLotteryLuck);
			pLottery->getParent()->addChild(pMask);
			pScene->m_pGameLayer->m_vSpecialMask.push_back(pMask);
			pMask->setPosition(pLottery->getPosition());
		}
	}
	if (wBankerChairID != INVALID_CHAIR)
	{
		llBankerChip = pMessageData->lCurrentJetton[wBankerChairID];
	}
	log(" OnGameSceneFree \n wBankerChairID = %d \n szBankerNickName = %s \n llBankerChip = %lld \n lBankerWinScore = %lld \n cbBankerTime = %d",
		wBankerChairID,
		pMessageData->szBankerNickName,
		llBankerChip,
		pMessageData->lBankerWinScore,
		pMessageData->cbBankerTime);

	//	庄家
	pScene->m_pInfoCenter->updateBankerInfo(
		wBankerChairID,
		pMessageData->szBankerNickName,
		llBankerChip,
		pMessageData->lBankerWinScore,
		pMessageData->cbBankerTime);
}

//	游戏空闲
bool SGLYMessageCenter::OnSubGameFree(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_GameFree) != wDataSize)
	{
		return false;
	}

	SGLY_CMD_S_GameFree* pMessageData = (SGLY_CMD_S_GameFree*)pBuffer;
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}

	//	重置
	pScene->resetGameScene();

	//	时钟
	pScene->m_pClock->setTimerType(E_SGLY_Process_Idle);
	pScene->m_pClock->playCountdownTimer(pMessageData->cbTimeLeave);


	return true;
}

//	游戏开始
bool SGLYMessageCenter::OnSubGameStart(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_GameStart) != wDataSize)
	{
		return false;
	}

	SGLY_CMD_S_GameStart* pMessageData = (SGLY_CMD_S_GameStart*)pBuffer;
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}

	//	时钟
	pScene->m_pClock->setTimerType(E_SGLY_Process_Bet);
	pScene->m_pClock->playCountdownTimer(pMessageData->cbTimeLeave);

	SGLYManageAudio::getInstance()->playSoundEffect(E_SGLY_Audio_GameStart);
	return true;
}

//	用户下注
bool SGLYMessageCenter::OnSubPlaceJetton(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_PlaceJetton) != wDataSize)
	{
		return false;
	}
	SGLY_CMD_S_PlaceJetton* pMessageData = (SGLY_CMD_S_PlaceJetton*)pBuffer;
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}
	E_SGLY_Lottery eType = (E_SGLY_Lottery)pMessageData->cbJettonArea;
	LONGLONG llSelfBet = pMessageData->lJettonScore;
	LONGLONG llTotalBet = SGLYTool::exchangeForChip(pMessageData->lTotalJettonScore[eType]);
	pScene->m_pInfoCenter->updateTotalBet(eType, llTotalBet);
	tagUserData* pPlayerData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	if (pMessageData->wChairID == pPlayerData->wChairID)
	{
		pScene->m_pInfoCenter->setOverwrite(true);
		llSelfBet = SGLYTool::exchangeForChip(llSelfBet);
		llSelfBet += pScene->m_pInfoCenter->getValueSelfBet(eType);
		pScene->m_pInfoCenter->updateSelfBet(eType, llSelfBet);
		LONGLONG llCurrPlayerChip = pScene->m_pInfoCenter->getValueInfoBar(E_SGLY_InfoBar_PlayerChip) - SGLYTool::exchangeForChip(pMessageData->lJettonScore);
		pScene->m_pInfoCenter->updateInfoBar(E_SGLY_InfoBar_PlayerChip, llCurrPlayerChip);
	}

	return true;
}

//	申请做庄
bool SGLYMessageCenter::OnSubUserApplyBanker(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_ApplyBanker) != wDataSize)
	{
		return false;
	}
	SGLY_CMD_S_ApplyBanker* pMessageData = (SGLY_CMD_S_ApplyBanker*)pBuffer;
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}
	int nSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	if (pMessageData->wApplyUser == nSelfChairID)
	{
		//	请求申请坐庄成功，改变显示的按钮
		pScene->m_pGameLayer->setApplyState(E_SGLY_ApplyState_Apply);
	}

	if (pScene->m_pPopupApplyList->isVisible())
	{
		EntityMgr::instance()->getDispatch()->sendSocketDate(SGLY_SUB_C_GET_PLAYLIST);
	}

	return true;
}

//	取消做庄
bool SGLYMessageCenter::OnSubUserCancelBanker(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_CancelBanker) != wDataSize)
	{
		return false;
	}
	SGLY_CMD_S_CancelBanker* pMessageData = (SGLY_CMD_S_CancelBanker*)pBuffer;
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}
	int nSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	if (pMessageData->wChairID == nSelfChairID)
	{
		//	请求申请坐庄成功，改变显示的按钮
		pScene->m_pGameLayer->setApplyState(E_SGLY_ApplyState_Null);
	}

	if (pScene->m_pPopupApplyList->isVisible())
	{
		EntityMgr::instance()->getDispatch()->sendSocketDate(SGLY_SUB_C_GET_PLAYLIST);
	}

	return true;
}

//	切换庄家
bool SGLYMessageCenter::OnSubChangeBanker(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_ChangeBanker) != wDataSize)
	{
		return false;
	}
	SGLY_CMD_S_ChangeBanker* pMessageData = (SGLY_CMD_S_ChangeBanker*)pBuffer;
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}

	tagUserData* pUserData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	int nSelfChairID = pUserData->wChairID;

	//	设置当前庄家椅子ID
	pScene->m_pGameLayer->setBankerChairID(pMessageData->wBankerUser);

	//	切换庄家动画
	pScene->m_pGameLayer->showAniChangeBanker(pMessageData->wBankerUser);

	//	玩家自身处理
	if (pMessageData->wBankerUser == nSelfChairID)
	{
		//	显示【我要下庄】
		pScene->m_pGameLayer->setApplyState(E_SGLY_ApplyState_Banker);
	}
	else
	{
		E_SGLY_ApplyState eApplyState = pScene->m_pGameLayer->getApplyState();
		if (eApplyState == E_SGLY_ApplyState_Banker)
		{
			pScene->m_pGameLayer->setApplyState(E_SGLY_ApplyState_Null);
		}
	}
	if (pScene->m_pPopupApplyList->isVisible())
	{
		EntityMgr::instance()->getDispatch()->sendSocketDate(SGLY_SUB_C_GET_PLAYLIST);
	}

	//	刷新庄家信息
	pScene->m_pInfoCenter->updateBankerInfo(
		pMessageData->wBankerUser,
		pMessageData->szBankerNickName,
		pMessageData->lBankerChip,
		0, 0
		);

	return true;
}

//	游戏结束
bool SGLYMessageCenter::OnSubGameEnd(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_GameEnd) != wDataSize)
	{
		return false;
	}

	SGLY_CMD_S_GameEnd* pMessageData = (SGLY_CMD_S_GameEnd*)pBuffer;
/*
	log("OnSubGameEnd: \n cbWinArea = %d \n cbGoodLuckType = %d \n cbPaoHuoCheCount = %d \n",
		pMessageData->cbWinArea, pMessageData->cbGoodLuckType, pMessageData->cbPaoHuoCheCount);
	log("OnSubGameEnd: \n cbPaoHuoCheArea[0] = %d \n cbPaoHuoCheArea[1] = %d \n cbPaoHuoCheArea[2] = %d \n cbPaoHuoCheArea[3] = %d \n cbPaoHuoCheArea[4] = %d \n cbPaoHuoCheArea[5] = %d \n cbPaoHuoCheArea[6] = %d \n",
		pMessageData->cbPaoHuoCheArea[0],
		pMessageData->cbPaoHuoCheArea[1],
		pMessageData->cbPaoHuoCheArea[2],
		pMessageData->cbPaoHuoCheArea[3],
		pMessageData->cbPaoHuoCheArea[4],
		pMessageData->cbPaoHuoCheArea[5],
		pMessageData->cbPaoHuoCheArea[6]);
*/

	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}
	auto pSelfItem = EntityMgr::instance()->roomFrame()->getMeUserItem();
	if (!pSelfItem)
	{
		return false;
	}

	//	刷新时钟
	pScene->m_pClock->setTimerType(E_SGLY_Process_Lottery);
	pScene->m_pClock->playCountdownTimer(pMessageData->cbTimeLeave);

	SGLYManageAudio::getInstance()->playSoundEffect(E_SGLY_Audio_TurnStart);

	pScene->m_pGameLayer->dealGameEnd();

	//	重写续押数值
	if (pScene->m_pInfoCenter->getOverwrite())
	{
		for (int i = 0; i < E_SGLY_Lottery_Null; i++)
		{
			LONGLONG llCurrBet = pScene->m_pInfoCenter->getValueSelfBet((E_SGLY_Lottery)i);
			pScene->m_pInfoCenter->setValueContinueBet((E_SGLY_Lottery)i, llCurrBet);
		}
		pScene->m_pInfoCenter->setOverwrite(false);
	}

	int nWinIdx = pMessageData->cbWinArea;
	LONGLONG llPlayerScore = SGLYTool::exchangeForChip(pMessageData->lUserScore);
	LONGLONG llBankerWinScore = SGLYTool::exchangeForChip(pMessageData->lBankerTotallScore);
	int nBankerTime = pMessageData->iBankerTime;

	float fGuessTime = 5.0f;	//	猜大小时间
	float fAniTime = 15.0f;	//	默认动画时间
	float fPauseTime = 0.5f;	//	停顿时间
	E_SGLY_GoodLuckType nLuckType = (E_SGLY_GoodLuckType)pMessageData->cbGoodLuckType;
	Sequence* pAniSeq = nullptr;	//	动画序列

	//	猜大小
	auto pCallGuess = CallFunc::create([=](){
		for (int i = E_SGLY_Lottery_Bar; i < E_SGLY_Lottery_Null; i++)
		{
			//	玩家有下注时的表现
			if (pScene->m_pInfoCenter->getValueSelfBet((E_SGLY_Lottery)i) > 0)
			{
				pScene->m_pClock->showGuessAni();
				return;
			}
		}
		//	玩家没有下注时的表现
		pScene->m_pClock->setTimerType(E_SGLY_Process_Guess);
		pScene->m_pClock->setLabNum(0);
		pScene->setVisible(true);
	});

	//	更新数据
	auto pCallUpdate = CallFunc::create([=](){
		pScene->m_pInfoCenter->updateInfoBar(E_SGLY_InfoBar_PlayerScore, llPlayerScore);
		pScene->m_pInfoCenter->updateInfoBar(E_SGLY_InfoBar_BankerWinScore, llBankerWinScore);
		pScene->m_pInfoCenter->updateInfoBar(E_SGLY_InfoBar_BankerTime, nBankerTime);
	});

	if (nWinIdx != 9 && nWinIdx != 21)	//	没开中Good Luck
	{
		float fAniTime = pMessageData->cbTimeLeave - fGuessTime;
		SGLYAniLottery* pAniLottery = SGLYAniLottery::create(fAniTime, nWinIdx);
		auto pCallShowLottery = CallFunc::create([=](){
			pScene->m_pGameLayer->showLotteryType(nWinIdx);
		});
		pAniSeq = Sequence::create(pAniLottery, pCallShowLottery, nullptr);
	}
	else
	{
		Vector<FiniteTimeAction*> vAction;
		SGLYAniLottery* pAniLottery = SGLYAniLottery::create(fAniTime, nWinIdx);

		//	显示GoodLuck类型
		auto pCallShowLuckType = CallFunc::create([=](){
			if (nLuckType != E_SGLY_GoodLuck_SuiJiDaQiang)
			{
				pScene->m_pGameLayer->showGoodLuckType(nLuckType);
			}
		});
		switch (nLuckType)
		{
		case E_SGLY_GoodLuck_Swallow:
			{
				auto pCallAudio = CallFunc::create([=](){
					SGLYManageAudio::getInstance()->playSoundEffect(E_SGLY_Audio_Guess_Lose);
				});
				pAniSeq = Sequence::create(pAniLottery, pCallShowLuckType, pCallAudio, nullptr);
			}
			break;
		case E_SGLY_GoodLuck_DaSanYuan:
		case E_SGLY_GoodLuck_DaSiXi:
		case E_SGLY_GoodLuck_XiaoSanYuan:
		case E_SGLY_GoodLuck_SuiJiDaQiang:
			{
				int nBeginIdx;
				int nEndIdx;
				int nCount = 7;
				float fAveAniTime = (pMessageData->cbTimeLeave - fAniTime - fGuessTime - fPauseTime * nCount) / nCount;

				vAction.pushBack(pAniLottery);
				vAction.pushBack(pCallShowLuckType);
				for (int i = 0; i < pMessageData->cbPaoHuoCheCount; i++)
				{

					if (i == 0)
					{
						nBeginIdx = pMessageData->cbWinArea;
					}
					else
					{
						nBeginIdx = pMessageData->cbPaoHuoCheArea[i - 1];
					}
					nEndIdx = pMessageData->cbPaoHuoCheArea[i];
					Sprite* pLottery = pScene->m_pGameLayer->getLottery(nBeginIdx);
					Sprite* pMask = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_HighLightLotteryLuck);
					pLottery->getParent()->addChild(pMask);
					pScene->m_pGameLayer->m_vSpecialMask.push_back(pMask);
					pMask->setPosition(pLottery->getPosition());
					pMask->setVisible(false);

					DelayTime* pDelayTime = DelayTime::create(fPauseTime);
					SGLYAniUniform* pAni = SGLYAniUniform::create(fAveAniTime , nBeginIdx, nEndIdx, pMask, false);
					Sequence* pSeq = Sequence::create(pDelayTime, pAni, nullptr);
					vAction.pushBack(pSeq);
				}
				pAniSeq = Sequence::create(vAction);
			}
			break;
		case E_SGLY_GoodLuck_PaoHuoChe:
			{
				int nCount = 3;
				float fAveAniTime = (pMessageData->cbTimeLeave - fAniTime - fGuessTime - fPauseTime * nCount) / nCount;
				for (int i = 0; i < pMessageData->cbPaoHuoCheCount; i++)
				{
					int nBeginIdx = i;
					int nEndIdx = pMessageData->cbPaoHuoCheArea[i];
					Sprite* pLottery = pScene->m_pGameLayer->getLottery(nBeginIdx);
					if (!pLottery)
					{
						continue;
					}
					Sprite* pMask = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_HighLightLotteryLuck);
					pLottery->getParent()->addChild(pMask);
					pScene->m_pGameLayer->m_vSpecialMask.push_back(pMask);
					pMask->setPosition(pLottery->getPosition());
					pMask->setVisible(false);

					DelayTime* pDelayTime = DelayTime::create(fPauseTime);
					SGLYAniUniform* pAni = SGLYAniUniform::create(fAveAniTime , nBeginIdx, nEndIdx, pMask, false, 2);
					Sequence* pSeq = Sequence::create(pDelayTime, pAni, nullptr);
					vAction.pushBack(pSeq);
				}
				pAniSeq = Sequence::create(pAniLottery, pCallShowLuckType, Spawn::create(vAction), nullptr);
			}
			break;
		case E_SGLY_GoodLuck_Null:
			break;
		default:
			break;
		}
	}

	auto pAllSeq = Sequence::create(pAniSeq, pCallGuess, pCallUpdate, nullptr);
	pScene->m_pGameLayer->runAction(pAllSeq);

	return true;
}

//	游戏记录
bool SGLYMessageCenter::OnSubGameRecord(const void *pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize%sizeof(SGLY_tagServerGameRecord)!=0)
	{
		return false;
	}

	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}

	WORD wRecordCount = wDataSize / sizeof(SGLY_tagServerGameRecord);

	int nItemCount = pScene->m_pHistory->getHistoryItemCount();
	if (nItemCount < 0)
	{
		return false;
	}

	int nBeginIdx;
	int nEndIdx = wRecordCount;
	if (nItemCount < SGLY_HISTORY_COUNT_MAX && wRecordCount > SGLY_HISTORY_COUNT_MAX)
	{
		nBeginIdx = wRecordCount - SGLY_HISTORY_COUNT_MAX;
	}
	else if (nItemCount >= SGLY_HISTORY_COUNT_MAX && wRecordCount > SGLY_HISTORY_COUNT_MAX)
	{
		nBeginIdx = wRecordCount - 8;
	}
	else
	{
		nBeginIdx = nItemCount;
	}
	for (int i = nBeginIdx; i < nEndIdx; i++) 
	{
		SGLY_tagServerGameRecord* pRecord = (((SGLY_tagServerGameRecord *)pBuffer) + i);
		pScene->m_pHistory->addHistoryItem(pRecord->cbRecord);
	}

	return true;
}

//	上庄列表
bool SGLYMessageCenter::OnApplyBankerList(const void *pBuffer, WORD wDataSize)
{
	//效验数据
	SGLY_CMD_S_ApplyBankerList* pApplyList = (SGLY_CMD_S_ApplyBankerList*)pBuffer;
	if (wDataSize!=pApplyList->getLength()) return false;

	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}

	pScene->m_pPopupApplyList->removeAllListItem();
	pScene->m_pPopupApplyList->setPopupVisible(true);

	for (int i = 0; i < pApplyList->cbCount; i++)
	{
		pScene->m_pPopupApplyList->addListItem((*pApplyList)[i]);
/*
		char cNikeName[300];
		MyNSString::wc2mbs((*pApplyList)[i].szNickName, 32, cNikeName);
		WORD wChairID = (*pApplyList)[i].wChairID;
		DWORD dwUserID = (*pApplyList)[i].dwUserID;
		DOUBLE lScore=(*pApplyList)[i].lScore;
*/
	}

	return true;
}

bool SGLYMessageCenter::onBetFailed(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(CMD_S_PlaceJettonFail) != wDataSize)
	{
		return false;
	}
	CMD_S_PlaceJettonFail* pMessageData = (CMD_S_PlaceJettonFail*)pBuffer;

	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}

	Dictionary * pDictionary = Dictionary::createWithContentsOfFile(SGLY_XML_FILE_NAME);
	string sTips = ((CCString*)pDictionary->objectForKey(SGLY_XML_KEY_BET_FAILED))->getCString();
	MyNSString::GBKToUTF8(sTips);	
	ToolKit::showTips(sTips);

	return true;
}

//	更新筹码
bool SGLYMessageCenter::OnSubUpdateChip(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_ChipUpdate) != wDataSize)
	{
		return false;
	}
	SGLY_CMD_S_ChipUpdate* pMessageData = (SGLY_CMD_S_ChipUpdate*)pBuffer;

	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}

	WORD wBankerChairID = pScene->m_pGameLayer->getBankerChairID();
	WORD wSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	LONGLONG llChipValue = SGLYTool::exchangeForChip(pMessageData->lTotalScore);
	if (wBankerChairID == pMessageData->wChairID)
	{
		pScene->m_pInfoCenter->updateInfoBar(E_SGLY_InfoBar_BankerChip, llChipValue);
	}
	if (wSelfChairID == pMessageData->wChairID)
	{
		if (pScene->m_pClock->getProcess() == E_SGLY_Process_Bet)
		{
			for (int i = 0; i < E_SGLY_Lottery_Null; i++)
			{
				llChipValue -= pScene->m_pInfoCenter->getValueSelfBet((E_SGLY_Lottery)i);
			}
		}
		pScene->m_pInfoCenter->updateInfoBar(E_SGLY_InfoBar_PlayerChip, llChipValue);
	}

	return true;
}

//	猜大小
bool SGLYMessageCenter::OnSubGuessBigSmall(const void *pBuffer, WORD wDataSize)
{
	if (sizeof(SGLY_CMD_S_BigSmall) != wDataSize)
	{
		return false;
	}
	SGLY_CMD_S_BigSmall* pMessageData = (SGLY_CMD_S_BigSmall*)pBuffer;

	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return false;
	}
	if (pScene->m_pClock->getProcess() != E_SGLY_Process_Guess)
	{
		return false;
	}
	if (pMessageData->bWin)
	{
		pScene->m_pGameLayer->showGuessReslt(pMessageData->bWin);
		pScene->m_pClock->unscheduleAllCallbacks();
		pScene->m_pClock->setLabNum(pMessageData->cbBigSmall);

		auto pDelay = DelayTime::create(0.25f);
		auto pCallContinue = CallFunc::create([=](){
			pScene->m_pClock->showGuessAni();
		});
		auto pSeq = Sequence::create(pDelay, pCallContinue, nullptr);
		pScene->runAction(pSeq);
	}
	else
	{
	pScene->m_pGameLayer->showGuessReslt(pMessageData->bWin);
	pScene->m_pClock->unscheduleAllCallbacks();
	pScene->m_pClock->setLabNum(pMessageData->cbBigSmall);
	}

	return true;
}
