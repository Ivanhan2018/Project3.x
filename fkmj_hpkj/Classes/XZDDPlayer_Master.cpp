#include "XZDDPlayer.h"
#include "XZDDGameLogic.h"
#include "XZDDSoundFun.h"

void XZDDPlayer::setHandCard_EX(std::vector<BYTE> kCardList)
{
	int nCardCount = kCardList.size();
	ASSERT(nCardCount <= MAX_COUNT);
	for (int i=0;i<nCardCount;i++)
	{
		m_kHandCardData[i] = kCardList[i];
	}
	m_kHandCardCout = nCardCount;
}
void XZDDPlayer::setMaskHandCard(std::vector<XZDD_MasterHandCardInfo> kMasterHandCardInfo)
{
	for (int i=0;i<(int)kMasterHandCardInfo.size();i++)
	{
		if (GetChairID() == kMasterHandCardInfo[i].nChairId)
		{
			setHandCard_EX(kMasterHandCardInfo[i].kMasterHandCard);
		}
	}
}
void XZDDPlayer::setMingPai(bool bValue)
{
	m_bMingPai = bValue;
}