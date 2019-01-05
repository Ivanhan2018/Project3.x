#ifndef __BRNN_SETTLEMENT_H__
#define __BRNN_SETTLEMENT_H__

#include "cocos2d.h"
#include "Define.h"

USING_NS_CC;

/*
**	@brief	百人牛牛——结算界面
*/
class BRNNSettlement: public Layer
{
public:
	BRNNSettlement();
	~BRNNSettlement();

	CREATE_FUNC(BRNNSettlement);

	//	更新玩家得分
	void updatePlayerScore(SCORE sValue);

	//	更新玩家返回本金
	void updatePlayerCapital(SCORE sValue);

	//	更新庄家得分
	void updateBankerScore(SCORE sValue);

private:
	bool init();
	void initUI();

	//	背景
	Sprite* m_pBG;

	//	玩家得分
	Label* m_pLabPlayerScore;

	//	玩家返回本金
	Label* m_pLabPlayerCapital;

	//	庄家得分
	Label* m_pLabBankerScore;

};

#endif  // #define __BRNN_SETTLEMENT_H__
