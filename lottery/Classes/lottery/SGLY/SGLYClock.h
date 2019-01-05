#ifndef __SGLY_CLOCK_H__
#define __SGLY_CLOCK_H__

#include "cocos2d.h"
#include "SGLYDefine.h"

USING_NS_CC;

/*
**	@brief	水果乐园——时钟
*/
class SGLYClock: public Node
{
public:
	SGLYClock();
	~SGLYClock();
	CREATE_FUNC(SGLYClock);

	//	重置
	void reset();

	//	倒计时器
	void playCountdownTimer(int nValue);

	//	设定计时器类型
	void setTimerType(E_SGLY_Process eType);

	//	显示猜大小动画
	void showGuessAni();

	//	设置显示的数字
	void setLabNum(int nValue);

	//	获得当前流程状态
	E_SGLY_Process getProcess() const;
private:
	bool init();
	void initUI();
	void initData();

	//	运作倒计时器
	void onCountdown(float dt);

	//	随机显示数字
	void showRandomNum(float dt);

	//	时钟背景
	Sprite* m_pBG;

	//	提示背景框
	Sprite* m_pTipsBG;

	//	提示语
	Sprite* m_pTips;

	//	时钟数字
	Label* m_pLabNum;

	//	时间数值
	int m_nTime;

	//	随机数字
	int m_nRandomNum;

	//	流程类型
	E_SGLY_Process m_eType;
};

#endif  // __SGLY_CLOCK_H__