#ifndef __GAME_FC3D_PL3_BET_AREA_H_
#define __GAME_FC3D_PL3_BET_AREA_H_
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "comm.h"

class FC3DPL3BetArea : public Layer
{

public:
	FC3DPL3BetArea();
	virtual ~FC3DPL3BetArea();
	virtual String *getBetNumber();
	virtual void clearNumbers();
	virtual void createRandomNumbers();
	virtual void setBetKind(ThreeNumbersKind kind);
	virtual void cancelAllMenuSelect(){};
	void addRandSeed();
	int getBetZhuShu(){return betZhuShu;};
	int combo(int base, int up);
	int factorial(int number);
	int heZhiZhuShu(int heZhi);

	Vec2 m_endPos;
protected:
	ThreeNumbersKind m_betKind;
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