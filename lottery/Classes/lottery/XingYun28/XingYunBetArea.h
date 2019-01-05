#ifndef __XingYun__Game__XingYunBetArea__
#define __XingYun__Game__XingYunBetArea__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "comm.h"

class XingYunBetArea : public Layer
{

public:
	XingYunBetArea();
	virtual ~XingYunBetArea();
	virtual String *getBetNumber();
	virtual void clearNumbers();
	virtual void createRandomNumbers();
	virtual void setBetKind(XingYun28Kind kind);
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
#endif  //