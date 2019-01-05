#include "PPCGameMessage.h"
#include "GBEventIDS.h"
#include "GlobalDef.h"
#include "EntityMgr.h"
#include "PPCProtocol.h"
#include "PPCGameScene.h"
#include "PPCTool.h"
#include "PPCSound.h"
#include "PPCAniOnDrive.h"
#include "MyNSString.h"

PPCGameMessage::PPCGameMessage()
{
}

PPCGameMessage::~PPCGameMessage()
{
	PPCSound::getInstance()->StopMusic();
	PPCSound::getInstance()->StopEffect();
}

bool PPCGameMessage::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!BaseGameLayer::init());

// 		tagUserData* pSelfData = EntityMgr::instance()->roomFrame()->getMeUserDate();
// 		EntityMgr::instance()->getDispatch()->SendPacketWithRequestChairUserInfo(pSelfData->wTableID, pSelfData->wChairID);

		bRet = true;
	} while (0);

	return bRet;
}

void PPCGameMessage::dealGameState(tagUserData eData)
{
	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return;
	}

	tagUserData* pPlayerData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	if (pPlayerData->dwUserID != eData.dwUserID)
	{
		//	���������������״̬����?
		return;
	}

	switch (eData.cbUserStatus)
	{
		case US_NULL:	//	�뿪
		case US_FREE:
			{
				PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
				if (!pGameScene)
				{
					break;
				}
				pGameScene->m_pApplyListView->removeListItem(pPlayerData->wChairID);
			}
			break;
		case US_SIT:
		case US_PLAYING:
			{
				pGameScene->m_pPPCInfoBar->m_pPlayerName->setString(eData.szNickName);
				pGameScene->m_pPPCInfoBar->updatePlayerGold(eData.lScore);
				if (eData.cbGender == 1)	//	��
				{
					pGameScene->m_pPPCInfoBar->m_pFace->initWithSpriteFrameName(PPCTextureName::s_ppc_face_man);
				}
				else
				{
					pGameScene->m_pPPCInfoBar->m_pFace->initWithSpriteFrameName(PPCTextureName::s_ppc_face_woman);
				}
				if (pGameScene->m_pPPCGameLayer->getApplyState() != EPPC_EPPC_Apply_State_Banker)
				{
					pGameScene->m_pPPCControl->checkoutEnabledBtnChip(pPlayerData->lScore);
				}

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

void PPCGameMessage::dealEnterGame(Ref* pSender)
{

}

void PPCGameMessage::dealPlayerInfo(Ref* pSender)
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
	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (pGameScene)
	{
	}
	dealGameState(*pUserData);
}

void PPCGameMessage::dealMessageGame(Ref* pSender)
{
	tagGameBuffer* tag = (tagGameBuffer*)pSender;
	int Applicead = tag->nDataSize;
	OnEventGameMessage(tag->wSubCmdID,tag->dateBuffer,tag->nDataSize);
}

void PPCGameMessage::onMessageScene(unsigned char cbStation,const void* pBuffer,unsigned short wDataSize)
{
	CCLOG("BaseGameLayer::onMessageScene cbStation=%d,wDataSize=%d", cbStation, wDataSize);
	switch(cbStation)
	{
	case GAME_STATUS_FREE://0
		{
			CCLOG("GAME_STATUS_FREE 0");
			OnGameSceneFree(pBuffer, wDataSize);
		}
		break;
	case GS_PLACE_JETTON:
		{
			OnGameScenePlay(pBuffer, wDataSize);
		}
		break;
	case GS_GAME_END://100
		{
			CCLOG("GAME_STATUS_PLAY 100");
			OnGameScenePlayEnd(pBuffer, wDataSize);
		}
		break;
	default:
		break;
	}
}

//	����״̬
void PPCGameMessage::OnGameSceneFree(const void *pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(ppcCMD_S_StatusFree)) return;
	ppcCMD_S_StatusFree* pStatusFree = (ppcCMD_S_StatusFree*)pBuffer;

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return;
	}
	pGameScene->m_GameStatue = EPPC_Idle;
	pGameScene->m_lApplyBankerCondition = pStatusFree->lApplyBankerCondition;
	//	ʱ��

	int nTimeLeave = pStatusFree->cbTimeLeave;
	pGameScene->m_pPPCClock->countdownTimer(nTimeLeave);
	pGameScene->m_pPPCClock->setTimerType(EPPC_Idle);

	//	���?

	tagUserData* pPlayerData = EntityMgr::instance()->roomFrame()->getMeUserDate();

	pGameScene->m_pPPCInfoBar->m_pPlayerName->setString(pPlayerData->szNickName);
	pGameScene->m_pPPCInfoBar->updatePlayerGold(pPlayerData->lScore);
	pGameScene->m_pPPCInfoBar->updatePlayerTotal(pGameScene->m_pPPCInfoBar->getPlayerTotal());
	pGameScene->m_pPPCInfoBar->updatePlayerBetSum(pGameScene->m_pPPCInfoBar->getPlayerBetSum());

	//����ׯ����Ϣ
	pGameScene->m_pPPCInfoBar->updateBankerInfo(
		pStatusFree->wBankerUser,
		pStatusFree->szBankerNickName,
		pStatusFree->lBankerScore,
		pStatusFree->lBankerWinScore,
		pStatusFree->cbBankerTime,
		pStatusFree->bEnableSysBanker);

	//	������ׯ�б���Ϣ
	if (pGameScene->m_pApplyListView->isVisible())
	{
		EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_GET_PLAYLIST);
	}
}

//	��ע״̬
void  PPCGameMessage::OnGameScenePlay(const void *pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(ppcCMD_S_StatusPlay)) return;
	ppcCMD_S_StatusPlay* pStatusPlay = (ppcCMD_S_StatusPlay*)pBuffer;

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return;
	}

	pGameScene->m_GameStatue = EPPC_Bet;
	pGameScene->m_lApplyBankerCondition = pStatusPlay->lApplyBankerCondition;

	int nTime = pStatusPlay->cbTimeLeave;
	pGameScene->m_pPPCClock->countdownTimer(nTime);
	pGameScene->m_pPPCClock->setTimerType(EPPC_Bet);
	//�Ѿ���ע��ֵ �� ���� ��1��ʼ
	for (int i = 1; i <= AREA_COUNT; i++)
	{
		//��ǰ�����Ѿ���ע����չʾ
		pGameScene->m_pPPCControl->betAll(PPCTool::convertToClientArea(i), pStatusPlay->lAllJettonScore[i]);
	}
	//����ׯ����Ϣ
	pGameScene->m_pPPCInfoBar->updateBankerInfo(
		pStatusPlay->wBankerUser,
		pStatusPlay->szBankerNickName,
		pStatusPlay->lBankerScore,
		pStatusPlay->lBankerWinScore,
		pStatusPlay->cbBankerTime,
		pStatusPlay->bEnableSysBanker);
}

//	����״̬
void PPCGameMessage::OnGameScenePlayEnd(const void *pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(ppcCMD_S_StatusPlay)) return;
	ppcCMD_S_StatusPlay* pStatusPlay = (ppcCMD_S_StatusPlay*)pBuffer;

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return;
	}

	pGameScene->m_GameStatue = EPPC_Lottery;
	pGameScene->m_lApplyBankerCondition = pStatusPlay->lApplyBankerCondition;

	int nTime = pStatusPlay->cbTimeLeave;
	pGameScene->m_pPPCClock->countdownTimer(nTime);
	pGameScene->m_pPPCClock->setTimerType(EPPC_Lottery);

	//�Ѿ���ע��ֵ ��  ���� ��1��ʼ
	for (int i = 1; i <= AREA_COUNT; i++)
	{
		pGameScene->m_pPPCControl->betAll(PPCTool::convertToClientArea(i), pStatusPlay->lAllJettonScore[i]);
	}

	//	����չʾ

	int nLotteryID = int(pStatusPlay->cbTableCardArray[0][0]) - 1;	//	�����ֻ�������ID

	float fDuration = pStatusPlay->cbTimeLeave - PPC_SHOW_ENDING_DURATION;
	fDuration = fDuration >= PPC_SHOW_ENDING_DURATION ? fDuration : 0;
	auto pDelay = DelayTime::create(fDuration);
	auto pCall = CallFunc::create([=]()
	{
		PPCCarLogo* pCarLogo = pGameScene->m_pPPCGameLayer->m_mCarLogoList[nLotteryID];
		pCarLogo->setHighLight(true);
		pGameScene->m_pPPCControl->showAreaHighLight(pCarLogo->getCarLogoType());
	});
	auto pSeq = Sequence::create(pDelay, pCall, nullptr);
	runAction(pSeq);

	//need add  չʾ������

	char cBufPlayerScore[128];
	char cBufPlayerCapital[128];
	char cBufBankerScore[128];

	sprintf(cBufPlayerScore, "%.2f", pStatusPlay->lEndUserScore);
	sprintf(cBufPlayerCapital, "%.2f", pStatusPlay->lEndUserReturnScore);
	sprintf(cBufBankerScore, "%.2f", pStatusPlay->lEndBankerScore);

	pGameScene->m_pPPCEnding->m_pLabPlayerScore->setString(cBufPlayerScore);
	pGameScene->m_pPPCEnding->m_pLabPlayerCapital->setString(cBufPlayerCapital);
	pGameScene->m_pPPCEnding->m_pLabBankerScore->setString(cBufBankerScore);

	//����ׯ����Ϣ
	pGameScene->m_pPPCInfoBar->updateBankerInfo(
		pStatusPlay->wBankerUser,
		pStatusPlay->szBankerNickName,
		pStatusPlay->lBankerScore,
		pStatusPlay->lBankerWinScore,
		pStatusPlay->cbBankerTime,
		pStatusPlay->bEnableSysBanker);
}

bool PPCGameMessage::OnEventGameMessage(WORD wSubCmdID, void* pBuffer, WORD wDataSize)
{
	//log("xin:: PPCGameMessage::OnEventGameMessage  wSubCmdID = %d, wDataSize = %d", wSubCmdID, wDataSize);
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pBuffer,wDataSize, true);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_S_CHECK_IMAGE:
		{
			return OnSubCheckImageIndex(pBuffer,wDataSize);
		}
	case SUB_S_ADMIN_COMMDN:		//����
		{
			return OnSubAdminControl(pBuffer,wDataSize);
		}	
	case SUB_S_SCORE_RESULT:
		{
			return OnSubScoreResult(pBuffer,wDataSize);
		}
	case SUB_S_ACCOUNT_RESULT:
		{
			return OnSubAccountResult(pBuffer,wDataSize);
		}
	case SUB_C_GET_PLAYLIST:
		{
			return OnApplyBankerList(pBuffer,wDataSize);
		}
	}

	return true;
}

//��Ϸ����
bool PPCGameMessage::OnSubGameFree( void * pBuffer, WORD wDataSize)
{
	//Ч������
	//ASSERT(wDataSize==sizeof(ppcCMD_S_GameFree));
	if (wDataSize!=sizeof(ppcCMD_S_GameFree)) return false;

	PPCSound::getInstance()->playMusiceOnGameFree();

	//��Ϣ����
	ppcCMD_S_GameFree * pGameFree=(ppcCMD_S_GameFree *)pBuffer;

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return false;
	}
	pGameScene->m_GameStatue = EPPC_Idle;
	int nTime = pGameFree->cbTimeLeave;
	pGameScene->m_pPPCClock->countdownTimer(nTime);
	pGameScene->m_pPPCClock->setTimerType(EPPC_Idle);

	pGameScene->resetGameScene();

	
	return true;
}

//��Ϸ��ʼ
bool PPCGameMessage::OnSubGameStart( void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(ppcCMD_S_GameStart))
		return false;


	//��Ϣ����
	ppcCMD_S_GameStart * pGameStart=(ppcCMD_S_GameStart *)pBuffer;
	
	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return false;
	}
	pGameScene->m_GameStatue = EPPC_Bet;
	//	��ʾʱ��

	int nTime = pGameStart->cbTimeLeave;
	pGameScene->m_pPPCClock->countdownTimer(nTime);
	pGameScene->m_pPPCClock->setTimerType(EPPC_Bet);

	PPCSound::getInstance()->playGameStartEffects();

	//	ˢ����Ϣ��

	if (pGameStart->wBankerUser == INVALID_CHAIR)
	{
		return true;
	}

	auto pSelfItem = EntityMgr::instance()->roomFrame()->getMeUserItem();
	if (!pSelfItem)
	{
		return false;
	}

	pGameScene->m_pPPCInfoBar->updatePlayerGold(pSelfItem->getUserGold());
// 	pGameScene->m_pPPCInfoBar->updatePlayerGold(pGameStart->lUserMaxScore);

	return true;
}

//�û���ע
bool PPCGameMessage::OnSubPlaceJetton( void * pBuffer, WORD wDataSize,bool bGameMes)
{
	//Ч������
	if (wDataSize!=sizeof(ppcCMD_S_PlaceJetton)) return false;

	//��Ϣ����
	ppcCMD_S_PlaceJetton * pPlaceJetton=(ppcCMD_S_PlaceJetton *)pBuffer;

	PPCSound::getInstance()->playEffect(EFFECT_TYPE_ADD_GOLD);

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return false;
	}

	tagUserData* pPlayerData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	EPPCCarLogoType eAreaType = PPCTool::convertToClientArea((int)pPlaceJetton->cbJettonArea);
	pGameScene->m_pPPCControl->bet(eAreaType, pPlaceJetton->lJettonScore);

	if (pPlaceJetton->wChairID == pPlayerData->wChairID)
	{
		//	������Ҹ��?
		PPCTableArea* pArea = pGameScene->m_pPPCControl->getTableArea(eAreaType);
		LONGLONG llSelf = pArea->getChipValueSelf() + (LONGLONG)pPlaceJetton->lJettonScore;
		pArea->setChipValueSelf(llSelf);
		pArea->setLabChipValueSelf(llSelf);

		//	�������Ͷ�?
		LONGLONG llPlayerBetSum = pGameScene->m_pPPCInfoBar->getPlayerBetSum() + pPlaceJetton->lJettonScore;
		pGameScene->m_pPPCInfoBar->updatePlayerBetSum(llPlayerBetSum);

		//	������ҿ��ý����
		SCORE sPlayerGold = pGameScene->m_pPPCInfoBar->getPlayerGold() - pPlaceJetton->lJettonScore;
		pGameScene->m_pPPCInfoBar->updatePlayerGold(sPlayerGold);

		//	ˢ��
		pGameScene->m_pPPCControl->checkoutEnabledBtnChip(sPlayerGold);
	}

	return true;
}

//������ׯ
bool PPCGameMessage::OnSubUserApplyBanker( void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(ppcCMD_S_ApplyBanker)) return false;

	//��Ϣ����
	ppcCMD_S_ApplyBanker * pApplyBanker=(ppcCMD_S_ApplyBanker *)pBuffer;

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return false;
	}

	int nSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	if (pApplyBanker->wApplyUser == nSelfChairID)
	{
		//	����������ׯ�ɹ����ı���ʾ�İ�ť
		pGameScene->m_pPPCGameLayer->setApplyState(EPPC_EPPC_Apply_State_Apply);
	}

	if (pGameScene->m_pApplyListView->isVisible())
	{
		EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_GET_PLAYLIST);
	}
	
	return true;
}

//ȡ����ׯ
bool PPCGameMessage::OnSubUserCancelBanker( void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize != sizeof(ppcCMD_S_CancelBanker))
	{
		return false;
	}

	//��Ϣ����
	ppcCMD_S_CancelBanker * pCancelBanker=(ppcCMD_S_CancelBanker *)pBuffer;

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return false;
	}

	int nSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	//getUserName
	if (pCancelBanker->wChairID == nSelfChairID)
	{
		//	����������ׯ�ɹ����ı���ʾ�İ�ť
		pGameScene->m_pPPCGameLayer->setApplyState(EPPC_Apply_State_Null);
	}
	if (pGameScene->m_pApplyListView->isVisible())
	{
		EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_GET_PLAYLIST);
	}
	return true;
}

//�л�ׯ��
bool PPCGameMessage::OnSubChangeBanker( void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(ppcCMD_S_ChangeBanker)) return false;

	//��Ϣ����
	ppcCMD_S_ChangeBanker * pChangeBanker=(ppcCMD_S_ChangeBanker *)pBuffer;

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return false;
	}

	tagUserData* pUserData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	int nSelfChairID = pUserData->wChairID;

	//	�л�ׯ�Ҷ���
	pGameScene->m_pPPCGameLayer->showAniChangeBanker(pChangeBanker->wBankerUser);

	//	���������?
	if (pChangeBanker->wBankerUser == nSelfChairID)
	{
		//	���ó��밴ť
		pGameScene->m_pPPCControl->setAllBtnChipEnable(false);
		//	��ʾ����Ҫ��ׯ��
		pGameScene->m_pPPCGameLayer->setApplyState(EPPC_EPPC_Apply_State_Banker);
	}
	else
	{
		EPPCApplyState eApplyState = pGameScene->m_pPPCGameLayer->getApplyState();
		if (eApplyState == EPPC_EPPC_Apply_State_Banker)
		{
			pGameScene->m_pPPCGameLayer->setApplyState(EPPC_Apply_State_Null);
			pGameScene->m_pPPCControl->checkoutEnabledBtnChip(pUserData->lScore);
		}
	}

	if (pGameScene->m_pApplyListView->isVisible())
	{
		EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_GET_PLAYLIST);
	}

	//	ˢ��ׯ����Ϣ
	pGameScene->m_pPPCInfoBar->updateBankerInfo(
		pChangeBanker->wBankerUser,
		pChangeBanker->szBankerNickName,
		pChangeBanker->lBankerScore, 0, 0);
	return true;
}

//��Ϸ����
bool PPCGameMessage::OnSubGameEnd( void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(ppcCMD_S_GameEnd)) return false;

	PPCSound::getInstance()->StopMusic();
	PPCSound::getInstance()->playGameEndEffects();

	//��Ϣ����
	ppcCMD_S_GameEnd * pGameEnd=(ppcCMD_S_GameEnd *)pBuffer;

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return false;
	}

	SCORE sUserScore = pGameEnd->lUserScore;
	pGameScene->m_GameStatue = EPPC_Lottery;

	pGameScene->m_pPPCGameLayer->resetCarLogo();
	//	��ʾʱ��

	int nTime = pGameEnd->cbTimeLeave;
	pGameScene->m_pPPCClock->countdownTimer(nTime);
	pGameScene->m_pPPCClock->setTimerType(EPPC_Lottery);


	//	�������?

	char cBufPlayerScore[128];
	char cBufPlayerCapital[128];
	char cBufBankerScore[128];

	sprintf(cBufPlayerScore, "%.2f", pGameEnd->lUserScore);
	sprintf(cBufPlayerCapital, "%.2f", pGameEnd->lUserReturnScore);
	sprintf(cBufBankerScore, "%.2f", pGameEnd->lBankerScore);

	pGameScene->m_pPPCEnding->m_pLabPlayerScore->setString(cBufPlayerScore);
	pGameScene->m_pPPCEnding->m_pLabPlayerCapital->setString(cBufPlayerCapital);
	pGameScene->m_pPPCEnding->m_pLabBankerScore->setString(cBufBankerScore);

	//	����չʾ

	int nLotteryID = int(pGameEnd->cbTableCardArray[0][0]) - 1;	//	�����ֻ�������ID

	//2018.8
	//DOUBLE MBankerScore = pGameEnd->lBankerScore;
	//DOUBLE MBankerTotallScore = pGameEnd->lBankerTotallScore;
	//int MBankerTime = pGameEnd->nBankerTime;
	float fDuration = nTime - PPC_SHOW_ENDING_DURATION;
	auto pAni = PPCAniOnDrive::create(fDuration, nLotteryID);
	auto pCall = CallFunc::create([=]()
	{
		//	�������?
		PPCCarLogo* pCarLogo = pGameScene->m_pPPCGameLayer->m_mCarLogoList[nLotteryID];
		pGameScene->m_pPPCControl->showAreaHighLight(pCarLogo->getCarLogoType());

		//	��ҳɼ�?
		SCORE sPlayerTotal = pGameScene->m_pPPCInfoBar->getPlayerTotal() + sUserScore;
		pGameScene->m_pPPCInfoBar->updatePlayerTotal(sPlayerTotal);

		//testcode , need add the win or lost info
		bool bIsWin = pGameScene->m_pPPCControl->isWinTheLotterys(pCarLogo->getCarLogoType());
		PPCSound::getInstance()->playEffectShowResult(bIsWin);

		//pGameScene->m_pPPCInfoBar->updateBankerGold(MBankerScore);
		//pGameScene->m_pPPCInfoBar->updateBankerTotal(MBankerTotallScore);
		//pGameScene->m_pPPCInfoBar->updateBankerTime(MBankerTime);
	});

	auto pSeq = Sequence::create(pAni, pCall, nullptr);
	pGameScene->m_pPPCGameLayer->runAction(pSeq);

	return true;
}

//��Ϸ��¼
bool PPCGameMessage::OnSubGameRecord( void * pBuffer, WORD wDataSize)
{
	//Ч�����?
	if (wDataSize%sizeof(ppcTagServerGameRecord)!=0) return false;

	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(ppcTagServerGameRecord);
	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return false;
	}

	//	��������¼��
	int nServerRecordTable[AREA_COUNT] =
	{
		EPPCCarLogo_Porsche_Big,
		EPPCCarLogo_BMW_Big,
		EPPCCarLogo_Benz_Big,
		EPPCCarLogo_VW_Big,
		EPPCCarLogo_Porsche_Small,
		EPPCCarLogo_BMW_Small,
		EPPCCarLogo_Benz_Small,
		EPPCCarLogo_VW_Small
	};

	int nItemCount = pGameScene->m_pPPCGameLayer->m_pTravel->getTraveItemCount();
	if (nItemCount < 0)
	{
		return false;
	}
	//	ʵ�������ȶ������1���������䴦��
	if (nItemCount >= (PPC_MAX_SCORE_HISTORY - 1) && wRecordCount == (PPC_MAX_SCORE_HISTORY - 1))
	{
		nItemCount = PPC_MAX_SCORE_HISTORY - 2;
	}
	for (int i = nItemCount; i < wRecordCount; i++) 
	{
		ppcTagServerGameRecord* pServerGameRecord = (((ppcTagServerGameRecord *)pBuffer) + i);
		for (int j = 0; j < AREA_COUNT; j++)
		{
			if (pServerGameRecord->bWinMen[j] > 0)
			{
				pGameScene->m_pPPCGameLayer->addGameRecord((EPPCCarLogoType)nServerRecordTable[j]);
			}
		}
	}

	return true;
}

//	��ׯ�б�
bool PPCGameMessage::OnApplyBankerList(void *pBuffer, WORD wDataSize)
{
	//Ч������
	ppcCMD_S_ApplyBankerList* pApplyList = (ppcCMD_S_ApplyBankerList*)pBuffer;
	if (wDataSize!=pApplyList->getLength()) return false;

	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return false;
	}

	pGameScene->m_pApplyListView->removeAllListItem();
	pGameScene->m_pApplyListView->setVisible(true);

	for (int i = 0; i < pApplyList->cbCount; i++)
	{
		pGameScene->m_pApplyListView->addListItem((*pApplyList)[i]);
/*
		char cNikeName[300];
		MyNSString::wc2mbs((*pApplyList)[i].szNickName, 32, cNikeName);
		WORD wChairID = (*pApplyList)[i].wChairID;
		DWORD dwUserID = (*pApplyList)[i].dwUserID;
		DOUBLE lScore=(*pApplyList)[i].lScore;
*/
	}
}

//��עʧ��
bool PPCGameMessage::OnSubPlaceJettonFail( void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(ppcCMD_S_PlaceJettonFail)) return false;

	//��Ϣ����
	ppcCMD_S_PlaceJettonFail * pPlaceJettonFail=(ppcCMD_S_PlaceJettonFail *)pBuffer;

	Dictionary * pDictionary = Dictionary::createWithContentsOfFile(PPC_XML_FILE_NAME);
	string sTips = ((CCString*)pDictionary->objectForKey(PPC_XML_KEY_BET_FAILED))->getCString();
	//ADDED BY 2018.8
	MyNSString::GBKToUTF8(sTips);	
	ToolKit::showTips(sTips);

	return true;
}

bool PPCGameMessage::OnSubCheckImageIndex( void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(ppcCMD_C_CheckImage)) return false;

	ppcCMD_C_CheckImage * pCheckImage=(ppcCMD_C_CheckImage *)pBuffer;

	return true;
}

//����
bool PPCGameMessage::OnSubAdminControl(  void * pBuffer, WORD wDataSize )
{
	if(wDataSize!=sizeof(ppcCMD_S_ControlReturns)) return false;

	ppcCMD_S_ControlReturns* pResult = (ppcCMD_S_ControlReturns*)pBuffer;

	//if( NULL != m_GameClientView.m_pClientControlDlg && NULL != m_GameClientView.m_pClientControlDlg->GetSafeHwnd() )
	//{
	//	CMD_S_ControlReturns* pResult = (CMD_S_ControlReturns*)pBuffer;
	//	m_GameClientView.m_pClientControlDlg->UpdateControl(pResult);
	//}
	return true;
}

bool PPCGameMessage::OnSubScoreResult(void * pBuffer, WORD wDataSize)
{
	if(wDataSize!=sizeof(ppcCMD_S_ScoreResult)) return false;

	ppcCMD_S_ScoreResult * pResult=(ppcCMD_S_ScoreResult*)pBuffer;

	return true;
}

bool PPCGameMessage::OnSubAccountResult(void * pBuffer, WORD wDataSize)
{
	if(wDataSize!=sizeof(ppcCMD_S_AccountResult)) return false;

	ppcCMD_S_AccountResult * pResult=(ppcCMD_S_AccountResult*)pBuffer;

	return true;
}
