#ifndef __LiuHe__Game__LiuHeBetArea__
#define __LiuHe__Game__LiuHeBetArea__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "comm.h"

class LiuHeBetArea : public Layer
{

public:
	LiuHeBetArea();
	virtual ~LiuHeBetArea();
	virtual void getBetNumber(); //¼ÆËã×¢Êý
	virtual void clearNumbers();
	virtual String* getBetNumberAt(const int index);
	virtual int getBetBeiShuAt(const int index);
	virtual void createRandomNumbers();
	virtual void setBetKind(LiuHeCaiKind kind);
	void addRandSeed();
	int getBetZhuShu(){
		return betZhuShu;
	};
	int getBetBeiShu(){return betBeiShu;}
	int combo(int base, int up);
	int factorial(int number);
	int heZhiZhuShu(int heZhi);
	virtual void cancelAllMenuSelect(){};

	Vec2 m_endPos;
protected:
	LiuHeCaiKind m_betKind;
	int betZhuShu;
	int betBeiShu;
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