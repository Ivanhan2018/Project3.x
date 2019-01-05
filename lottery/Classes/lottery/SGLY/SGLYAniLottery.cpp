#include "SGLYAniLottery.h"
#include "SGLYGameScene.h"
#include "SGLYManageAudio.h"


/***************************SGLYAniLottery	begin*********************************************/


SGLYAniLottery::SGLYAniLottery()
{
}

SGLYAniLottery::~SGLYAniLottery()
{
}

SGLYAniLottery* SGLYAniLottery::create(float fDuration, int nIdx)
{
	SGLYAniLottery* pAction = new SGLYAniLottery();
	if (pAction && pAction->initWithDuration(fDuration))
	{
		pAction->autorelease();
		pAction->initData(fDuration, nIdx);
	}
	else
	{
		delete pAction;
		pAction = nullptr;
	}

	return pAction;
}

void SGLYAniLottery::initData(float fDuration, int nIdx)
{
	m_TargetIdx = nIdx;
	m_nFinishSHift = 0;											//	已走路程
	m_nCurrIdx = 0;												//	当前汽车logo索引
	m_bSegregate = false;

	m_nS0 = SGLY_LOTTERY_COUNT;									//	加速时间段总位移
	m_nS1 = 2 * SGLY_LOTTERY_COUNT + nIdx;						//	匀速时间段总位移
	m_nS2 = SGLY_LOTTERY_COUNT;									//	减速时间段总位移

	m_nSSum = m_nS0 + m_nS1 + m_nS2;							//	总路程
	m_nEndS0 = m_nS0;											//	加速段终点
	m_nEndS1 = m_nS0 + m_nS1;									//	匀速段终点
	m_nEndS2 = m_nSSum;											//	减速段终点

	m_nSNor0 = 0;												//	加速段单位路程
	m_nSNor1 = 0;												//	匀速段单位路程
	m_nSNor2 = 0;												//	减速段单位路程

	m_fAveV1 = (2 * m_nS0 + m_nS1 + 2 * m_nS2) / fDuration;		//	匀速平均速度
	m_fT1 = (float)m_nS1 / m_fAveV1;							//	匀速持续时间
	m_fAveT1 = m_fT1 / m_nS1;									//	匀速平均时间

	m_fT0 = (fDuration - m_fT1) / 2;							//	加速持续时间
	m_fT2 = m_fT0;												//	减速持续时间

	m_fStartV0 = 0;												//	加速初速度
	m_fEndV0 = m_fAveV1;										//	加速末速度
	m_fA0 = (m_fEndV0 - m_fStartV0) / m_fT0;					//	加速度

	m_fStartV2 = m_fAveV1;										//	减速初速度
	m_fEndV2 = 0;												//	减速末速度
	m_fA2 = (m_fEndV2 - m_fStartV2) / m_fT2;					//	加速度

	m_fVT = 0;													//	末速度
	m_fPayT = 0;												//	所需时间
}

void SGLYAniLottery::showHighLight(int nIdx)
{
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return;
	}
	pScene->m_pGameLayer->setOnlyLotteryHighLight(nIdx, true);
	SGLYManageAudio::getInstance()->playSoundEffect(E_SGLY_Audio_DispatchLottery);
}

void SGLYAniLottery::update(float time)
{
	if (!_target)
	{
		return;
	}
	bool bIsRun = false;
	float dt = time * _duration;

	if (m_nFinishSHift > m_nSSum && m_nCurrIdx > m_TargetIdx)
	{
		if (time >= 1)
		{
			E_SGLY_Audio eAudioID;
			switch (m_TargetIdx)
			{
			case 2:case 3:
				eAudioID = E_SGLY_Audio_Lottery_Bar;
				break;
			case 14:case 15:
				eAudioID = E_SGLY_Audio_Lottery_Seven;
				break;
			case 19:case 20:
				eAudioID = E_SGLY_Audio_Lottery_Star;
				break;
			case 7:case 8:
				eAudioID = E_SGLY_Audio_Lottery_Watermelon;
				break;
			case 1:case 13:case 23:
				eAudioID = E_SGLY_Audio_Lottery_Bell;
				break;
			case 6:case 17:case 18:
				eAudioID = E_SGLY_Audio_Lottery_Pear;
				break;
			case 0:case 11:case 12:
				eAudioID = E_SGLY_Audio_Lottery_Orange;
				break;
			case 4:case 5:case 10:case 16:case 22:
				eAudioID = E_SGLY_Audio_Lottery_Apple;
				break;
			case 9:
			case 21:
			default:
				return;
			}
			SGLYManageAudio::getInstance()->playSoundEffect(eAudioID);
		}
		return;
	}
	if (m_nFinishSHift == 0)
	{
		bIsRun = true;
	}
	else if (m_nFinishSHift > 0 && m_nFinishSHift <= m_nEndS0)
	{

		if (!m_bSegregate)
		{
			m_fVT = sqrtf((2 * m_fA0 * m_nSNor0) + powf(m_fStartV0, 2));	
			m_fPayT = (m_fVT - m_fStartV0) / m_fA0;

			m_bSegregate = true;
		}
		if (dt >= m_fPayT)
		{
			bIsRun = true;
			m_nSNor0++;
		}
	}
	else if (m_nFinishSHift > m_nEndS0 && m_nFinishSHift <= m_nEndS1)
	{
		float fNodeT = m_fT0 + m_nSNor1 * m_fAveT1;
		if (dt >= fNodeT)
		{
			bIsRun = true;
			m_nSNor1++;
		}
	}
	else if (m_nFinishSHift > m_nEndS1 && m_nFinishSHift <= m_nEndS2)
	{
		if (!m_bSegregate)
		{
			m_fVT = sqrtf((2 * m_fA2 * m_nSNor2) + powf(m_fStartV2, 2));	
			float fTime = (m_fVT - m_fStartV2) / m_fA2;

			m_fPayT = m_fT0 + m_fT1+ fTime;
			m_bSegregate = true;
		}
		if (dt >= m_fPayT)
		{
			bIsRun = true;
			m_nSNor2++;
		}
	}

	if (bIsRun)
	{
		showHighLight(m_nCurrIdx);
		m_nCurrIdx++;
		m_nFinishSHift++;
		m_bSegregate = false;
	}

	if (m_nCurrIdx == SGLY_LOTTERY_COUNT)
	{
		m_nCurrIdx = 0;
	}
}


/***************************SGLYAniLottery	end*********************************************/


/***************************SGLYAniUniform	begin*********************************************/


SGLYAniUniform::SGLYAniUniform()
{
}

SGLYAniUniform::~SGLYAniUniform()
{
}

SGLYAniUniform* SGLYAniUniform::create(float fDuration, int nBeginIdx, int nEndIdx, Node* pMoveTarget, bool bIsForward, int nCount)
{
	SGLYAniUniform* pAction = new SGLYAniUniform();
	if (pAction && pAction->initWithDuration(fDuration))
	{
		pAction->autorelease();
		pAction->initData(fDuration, nBeginIdx, nEndIdx, pMoveTarget, bIsForward, nCount);
	}
	else
	{
		delete pAction;
		pAction = nullptr;
	}

	return pAction;
}

void SGLYAniUniform::initData(float fDuration, int nBeginIdx, int nEndIdx, Node* pMoveTarget, bool bIsForward, int nCount)
{
	m_bOnce = false;
	m_nBeginIdx = nBeginIdx;
	m_nEndIdx = nEndIdx;
	m_bIsForward = bIsForward;
	m_nRunS = 0;
	m_nCurrIdx = m_nBeginIdx;
	m_pMoveTarget = pMoveTarget;
	if (m_bIsForward)
	{
		m_nTotalS = m_nEndIdx - m_nBeginIdx;
	}
	else
	{
		if (m_nEndIdx > m_nBeginIdx)
		{
			m_nTotalS = m_nBeginIdx + (SGLY_LOTTERY_COUNT - m_nEndIdx);
		}
		else
		{
			m_nTotalS = m_nBeginIdx - m_nEndIdx;
		}
		m_nTotalS += SGLY_LOTTERY_COUNT * nCount;
	}
	m_fAveV = _duration / (float)m_nTotalS;
}

void SGLYAniUniform::showHighLight(int nIdx)
{
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return;
	}
	Sprite* pCurLottery = pScene->m_pGameLayer->getLottery(nIdx);
	m_pMoveTarget->setPosition(pCurLottery->getPosition());
	m_pMoveTarget->setVisible(true);
}

void SGLYAniUniform::update(float time)
{
	if (m_nRunS >= m_nTotalS && m_nCurrIdx == m_nEndIdx)
	{
		m_bOnce = false;
		SGLYManageAudio::getInstance()->stopAllSoundEffect();
		SGLYManageAudio::getInstance()->playSoundEffect(E_SGLY_Audio_GoodLuck_Hit);
		return;
	}

	if (!m_bOnce)
	{
		m_bOnce = true;
		SGLYManageAudio::getInstance()->playSoundEffect(E_SGLY_Audio_GoodLuck_Flash, true);
	}
	float fT = time * _duration;
	float fRunTime = m_nRunS * m_fAveV;
	if (fT >= fRunTime)
	{
		m_nRunS++;
		if (m_bIsForward)
		{
			m_nCurrIdx++;
		}
		else
		{
			m_nCurrIdx--;
		}
	}
	if (m_nCurrIdx >= SGLY_LOTTERY_COUNT)
	{
		m_nCurrIdx = 0;
	}
	if (m_nCurrIdx < 0)
	{
		m_nCurrIdx = SGLY_LOTTERY_COUNT - 1;
	}
	showHighLight(m_nCurrIdx);
}

/***************************SGLYAniUniform	end*********************************************/
