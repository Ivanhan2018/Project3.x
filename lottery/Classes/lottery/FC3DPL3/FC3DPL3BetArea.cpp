#include "RoomLayer.h"
#include "FC3DPL3BetArea.h"
#include "VersionControl.h"

using namespace cocos2d;
FC3DPL3BetArea::FC3DPL3BetArea()
{
	winSize = Director::getInstance()->getWinSize();
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

FC3DPL3BetArea::~FC3DPL3BetArea()
{

}

String *FC3DPL3BetArea::getBetNumber()
{
	return String::createWithFormat("%s","FC3DPL3BetArea");
}

void FC3DPL3BetArea::clearNumbers()
{
	CCLOG("FC3DPL3BetArea::clearNumbers");
}

void FC3DPL3BetArea::createRandomNumbers()
{
	CCLOG("FC3DPL3BetArea::createRandomNumbers");
}

void FC3DPL3BetArea::addRandSeed()
{
	timeval psv;	
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec;
	srand(rand_seed);
}

void FC3DPL3BetArea::setBetKind(ThreeNumbersKind kind)
{
	CCLOG("FC3DPL3BetArea::setBetKind");
}
int FC3DPL3BetArea::combo(int base, int up)
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

int FC3DPL3BetArea::factorial(int number)
{
	int total = 1;
	for(int i = 1 ; i <= number; i++){
		total *= i;
	}
	return total;
}

int FC3DPL3BetArea::heZhiZhuShu(int heZhi)
{
	int totalNum = 0;
	for(int i = 0 ; i <= 11 ; i++){
		for(int j = 0 ; j <= 11 ; j++){
			for(int k = 0 ; k <= 11 ; k++){
				if(i+j+k == heZhi){
					totalNum++;
				}
			}
		}
	}
	return totalNum;
}