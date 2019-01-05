#ifndef __Game__GuangDong11Xuan5BetArea__
#define __Game__GuangDong11Xuan5BetArea__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "comm.h"

class GuangDong11Xuan5BetArea : public Layer
{

public:
	GuangDong11Xuan5BetArea();
	virtual ~GuangDong11Xuan5BetArea();
	virtual String *getBetNumber();
	virtual void clearNumbers();
	virtual void createRandomNumbers();
	virtual void setBetKind(tagEnumIIXuan5 kind);
	virtual void cancelAllMenuSelect(){};
	void addRandSeed();
	int getBetZhuShu(){return betZhuShu;};
	int combo(int base, int up);
	int factorial(int number);
	int heZhiZhuShu(int heZhi);

	Vec2 m_endPos;
protected:
	tagEnumIIXuan5 betKind;
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