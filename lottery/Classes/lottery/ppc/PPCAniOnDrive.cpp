#include "PPCAniOnDrive.h"
#include "PPCGameScene.h"

PPCAniOnDrive::PPCAniOnDrive()
{
}

PPCAniOnDrive::~PPCAniOnDrive()
{
}

PPCAniOnDrive* PPCAniOnDrive::create(float fDuration, int nIdx)
{
	PPCAniOnDrive* pAction = new PPCAniOnDrive();
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

void PPCAniOnDrive::initData(float fDuration, int nIdx)
{
	m_TargetIdx = nIdx;
	m_nFinishSHift = 0;											//	已走路程
	m_nCurrIdx = 0;												//	当前汽车logo索引
	m_bSegregate = false;

	m_nS0 = PPC_CARLGOG_COUNT;									//	加速时间段总位移
	m_nS1 = 2 * PPC_CARLGOG_COUNT + nIdx;						//	匀速时间段总位移
	m_nS2 = PPC_CARLGOG_COUNT;									//	减速时间段总位移

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

void PPCAniOnDrive::showHighLight(int nIdx)
{
	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return;
	}
	pGameScene->m_pPPCGameLayer->showSingleCarLogoHighLight(nIdx, true);
}

void PPCAniOnDrive::update(float time)
{
	if (!_target)
	{
		return;
	}
	bool bIsRun = false;
	float dt = time * _duration;

	if (m_nFinishSHift > m_nSSum && m_nCurrIdx > m_TargetIdx)
	{
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

	if (m_nCurrIdx == PPC_CARLGOG_COUNT)
	{
		m_nCurrIdx = 0;
	}
}