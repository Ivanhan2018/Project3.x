#include "RoomLayer.h"
#include "GuangDong11Xuan5BetArea.h"
#include "VersionControl.h"

using namespace cocos2d;
GuangDong11Xuan5BetArea::GuangDong11Xuan5BetArea()
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

GuangDong11Xuan5BetArea::~GuangDong11Xuan5BetArea()
{

}

String *GuangDong11Xuan5BetArea::getBetNumber()
{
	return String::createWithFormat("%s","GuangDong11Xuan5BetArea");
}

void GuangDong11Xuan5BetArea::clearNumbers()
{
	CCLOG("GuangDong11Xuan5BetArea::clearNumbers");
}

void GuangDong11Xuan5BetArea::createRandomNumbers()
{
	CCLOG("GuangDong11Xuan5BetArea::createRandomNumbers");
}

void GuangDong11Xuan5BetArea::addRandSeed()
{
	timeval psv;	
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec;
	srand(rand_seed);
}

void GuangDong11Xuan5BetArea::setBetKind(tagEnumIIXuan5 kind)
{
	CCLOG("GuangDong11Xuan5BetArea::setBetKind");
}
int GuangDong11Xuan5BetArea::combo(int base, int up)
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

int GuangDong11Xuan5BetArea::factorial(int number)
{
	int total = 1;
	for(int i = 1 ; i <= number; i++){
		total *= i;
	}
	return total;
}

int GuangDong11Xuan5BetArea::heZhiZhuShu(int heZhi)
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