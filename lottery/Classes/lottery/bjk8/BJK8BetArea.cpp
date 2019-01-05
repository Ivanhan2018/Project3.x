#include "RoomLayer.h"
#include "BJK8BetArea.h"
#include "VersionControl.h"

using namespace cocos2d;
BJK8BetArea::BJK8BetArea()
{
	winSize = Director::getInstance()->getWinSize();
	betZhuShu = 1;
	areaBkHeight = 40;
	ballRadius = BALL_RADDIUS;
	ballGapX = (SCREEN_WIDTH - 34 * 2 - ballRadius*7.0) / 6.0;
	ballStartX = 34 + ballRadius / 2;//ballGapX+ballRadius/2;
	areaTitlePosX = 67;//42;
	areaGapY = 15;
	areaHeight = ballRadius*2+areaBkHeight+4*areaGapY;
	areaMenuPosX = 330;
	fontSize = 27;
	this->addRandSeed();
}

BJK8BetArea::~BJK8BetArea()
{

}

String *BJK8BetArea::getBetNumber()
{
	return String::createWithFormat("%s","BJK8BetArea");
}

void BJK8BetArea::clearNumbers()
{
	CCLOG("BJK8BetArea::clearNumbers");
}

void BJK8BetArea::createRandomNumbers()
{
	CCLOG("BJK8BetArea::createRandomNumbers");
}

void BJK8BetArea::addRandSeed()
{
	timeval psv;	
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec;
	srand(rand_seed);
}

void BJK8BetArea::setBetKind(BeiJingKuai8Kind kind)
{
	CCLOG("BJK8BetArea::setBetKind");
}

int BJK8BetArea::combo(int base, int up)
{
	//此方案容易溢出
	//return factorial(base)/(factorial(up)*factorial(base-up));
	int tempUp = factorial(up);
	int tempInt = 1;
	for(int i = 0;i < up;i ++)
	{
		tempInt *= base - i;
	}
	return tempInt / tempUp;
}

int BJK8BetArea::factorial(int number)
{
	int total = 1;
	for(int i = 1 ; i <= number; i++){
		total *= i;
	}
	return total;
}

int BJK8BetArea::heZhiZhuShu(int heZhi)
{
	int totalNum = 0;
	for(int i = 0 ; i <= 9 ; i++){
		for(int j = 0 ; j <= 9 ; j++){
			for(int k = 0 ; k <= 9 ; k++){
				if(i+j+k == heZhi){
					totalNum++;
				}
			}
		}
	}
	return totalNum;
}