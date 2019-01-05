#include "RoomLayer.h"
#include "XingYunBetArea.h"
#include "VersionControl.h"

using namespace cocos2d;
XingYunBetArea::XingYunBetArea()
{
	winSize = Director::getInstance()->getWinSize();
	betZhuShu = 1;
	areaBkHeight = 40;
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

XingYunBetArea::~XingYunBetArea()
{

}

String *XingYunBetArea::getBetNumber()
{
	return String::createWithFormat("%s","XingYunBetArea");
}

void XingYunBetArea::clearNumbers()
{
	CCLOG("XingYunBetArea::clearNumbers");
}

void XingYunBetArea::createRandomNumbers()
{
	CCLOG("XingYunBetArea::createRandomNumbers");
}

void XingYunBetArea::addRandSeed()
{
	timeval psv;	
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec;
	srand(rand_seed);
}

void XingYunBetArea::setBetKind(XingYun28Kind kind)
{
	CCLOG("XingYunBetArea::setBetKind");
}

int XingYunBetArea::combo(int base, int up)
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

int XingYunBetArea::factorial(int number)
{
	int total = 1;
	for(int i = 1 ; i <= number; i++){
		total *= i;
	}
	return total;
}

int XingYunBetArea::heZhiZhuShu(int heZhi)
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