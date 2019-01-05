#ifndef __BRNN_CLOCK_H__
#define __BRNN_CLOCK_H__

#include "cocos2d.h"
#include "BRNNDefine.h"

USING_NS_CC;

/*
**	@brief	百人牛牛——时钟
*/
class BRNNClock: public Node
{
public:
	BRNNClock();
	~BRNNClock();
	CREATE_FUNC(BRNNClock);

	//	倒计时器
	void playCountdownTimer(int nValue);

	//	设定计时器类型
	void setTimerType(E_BRNNProcessType eType);
private:
	bool init();
	void initUI();
	void initData();

	//	运作倒计时器
	void onCountdown(float dt);

	//	时钟背景
	Sprite* m_pBG;

	//	提示语
	Sprite* m_pTips;

	//	时钟数字
	Label* m_pBMFNum;

	//	时间数值
	int m_nTime;

	//	流程类型
	E_BRNNProcessType m_eType;
};

#endif  // __BRNN_CLOCK_H__