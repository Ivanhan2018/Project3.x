#ifndef __BJK8__Game__LotteryBetArea__
#define __BJK8__Game__LotteryBetArea__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "comm.h"

class BJK8BetArea : public Layer
{

public:
	BJK8BetArea();
	virtual ~BJK8BetArea();
	virtual String *getBetNumber();
	virtual void clearNumbers();
	virtual void createRandomNumbers();
	virtual void setBetKind(BeiJingKuai8Kind kind);
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
	BeiJingKuai8Kind m_betKind;
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
#endif  //__BJK8__