#ifndef __GAME_BJCAR_PK10_BET_AREA_H_
#define __GAME_BJCAR_PK10_BET_AREA_H_
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "comm.h"

class bjpk10BetArea : public Layer
{

public:
	bjpk10BetArea();
	virtual ~bjpk10BetArea();
	virtual String *getBetNumber();
	virtual void clearNumbers();
	virtual void createRandomNumbers();
	virtual void setBetKind(PK10GameKind kind);
	virtual void cancelAllMenuSelect(){};
	void addRandSeed();
	int getBetZhuShu(){return betZhuShu;};
	int combo(int base, int up);
	int factorial(int number);
	int heZhiZhuShu(int heZhi);

	Vec2 m_endPos;
protected:
	PK10GameKind m_betKind;
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