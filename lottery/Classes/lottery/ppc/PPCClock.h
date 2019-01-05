#ifndef __PPC_CLOCK_H__
#define __PPC_CLOCK_H__

#include "cocos2d.h"
#include "PPCEnum.h"

USING_NS_CC;

//	时钟
class PPCClock: public Node
{
public:
	PPCClock();
	~PPCClock();
	CREATE_FUNC(PPCClock);

	//	倒计时器
	void countdownTimer(int nValue);

	//	设定计时器类型
	void setTimerType(EPPCProcessType eType);
private:
	bool init();
	void initUI();
	void initData();

	//	运作倒计时器
	void onCountdown(float dt);

	//	处理倒计时事件
	void onTimeEvent();

	//	处理开奖时间事件
	void onLotteryTime();

	//	时钟背景
	Sprite* m_pBG;

	//	提示语
	Sprite* m_pTips;

	//	时钟数字
	Label* m_pBMFNum;

	//	时间数值
	int m_nTime;

	//	流程类型
	EPPCProcessType m_eType;
};

#endif  // __PPC_CLOCK_H__