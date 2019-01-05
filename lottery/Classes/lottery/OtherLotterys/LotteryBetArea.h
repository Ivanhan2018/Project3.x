#ifndef __LOTTERY__Game__LotteryBetArea__
#define __LOTTERY__Game__LotteryBetArea__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "comm.h"

class LotteryBetArea : public Layer
{

public:
	LotteryBetArea();
	virtual ~LotteryBetArea();
	virtual String *getBetNumber();
	virtual void clearNumbers();
	virtual void createRandomNumbers();
	virtual void setBetKind(QiXingCaiKind kind);
	void addRandSeed();
	int getBetZhuShu(){
		return betZhuShu;
	};
	int combo(int base, int up);
	int factorial(int number);
	int heZhiZhuShu(int heZhi);
	virtual void cancelAllMenuSelect(){};

	Vec2 m_endPos;
protected:
	QiXingCaiKind m_betKind;
	int betZhuShu;
	cocos2d::Size winSize;
	float areaBkHeight;
	float ballRadius;
	float ballStartX;
	float ballGapX;
	float areaTitlePosX;
	float areaHeight;
	float areaGapY;
	float fontSize;
	float areaMenuPosX;
};
#endif  //__Game__HelpScrollView__