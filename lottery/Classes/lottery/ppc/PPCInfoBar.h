#ifndef __PPC_INFO_BAR_H__
#define __PPC_INFO_BAR_H__

#include "cocos2d.h"
#include "PPCProtocol.h"
// #include "GlobalDef.h"

USING_NS_CC;
using namespace std;

//	信息栏
class PPCInfoBar: public Layer
{
public:
	PPCInfoBar();
	~PPCInfoBar();
	CREATE_FUNC(PPCInfoBar);

	//	重置
	void resetPPCInfoBar();
	
	/*更新庄家信息
	wBankerUser			当前庄家
	szBankerNickName	庄家昵称
	lBankerScore		庄家分数
	lBankerWinScore		庄家成绩
	cbBankerTime		庄家局数
	bSystemBanker		系统做庄
	*/
	void updateBankerInfo(WORD wBankerUser, TCHART szBankerNickName[], SCORE lBankerScore,SCORE lBankerWinScore, WORD cbBankerTime, bool bSystemBanker = true);

	//	更新玩家金币
	void updatePlayerGold(SCORE sValue);

	//	更新玩家成绩
	void updatePlayerTotal(SCORE sValue);

	//	更新玩家投注数额
	void updatePlayerBetSum(SCORE sValue);

	//	更新庄家金币
	void updateBankerGold(SCORE sValue);

	//	更新庄家成绩
	void updateBankerTotal(SCORE sValue);

	//	更新庄家连庄数
	void updateBankerTime(WORD wValue);


	//	玩家头像
	Sprite* m_pFace;

	//	玩家昵称
	Label* m_pPlayerName;

	//	玩家财富
	Label* m_pPlayerGold;

	//	玩家成绩
	Label* m_pPlayerTotal;

	//	玩家投注
	Label* m_pPlayerBetSum;

	//	庄家昵称
	Label* m_pBankerName;

	//	庄家财富
	Label* m_pBankerGold;

	//	庄家成绩
	Label* m_pBankerTotal;

	//	庄家连庄
	Label* m_pBankerTime;

private:
	bool init();
	void initUI();
	void initData();

	//	玩家金币数值
	CC_SYNTHESIZE(SCORE, m_dPlayerGold, PlayerGold);

	//	玩家成绩数值
	CC_SYNTHESIZE(SCORE, m_dPlayerTotal, PlayerTotal);

	//	玩家投注总额数值
	CC_SYNTHESIZE(SCORE, m_dPlayerBetSum, PlayerBetSum);

};


#endif  // __PPC_INFO_BAR_H__