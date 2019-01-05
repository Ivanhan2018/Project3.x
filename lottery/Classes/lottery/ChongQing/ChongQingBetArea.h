#ifndef __Game__ChongQingBetArea__
#define __Game__ChongQingBetArea__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "comm.h"

class ChongQingBetArea : public Layer
{

public:
	ChongQingBetArea();
	virtual ~ChongQingBetArea();
	virtual String *getBetNumber();
	virtual void clearNumbers();
	virtual void createRandomNumbers();
	virtual void setBetKind(SSCWanFaKind kind);
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
	SSCWanFaKind m_betKind;
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