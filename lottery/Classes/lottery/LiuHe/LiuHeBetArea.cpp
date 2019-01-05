#include "RoomLayer.h"
#include "LiuHeBetArea.h"
#include "VersionControl.h"

using namespace cocos2d;
LiuHeBetArea::LiuHeBetArea()
{
	winSize = Director::getInstance()->getWinSize();
	betZhuShu = 1;
	betBeiShu = 1;
	areaBkHeight = 40;
	ballRadius = 78;//BALL_RADDIUS;

#ifdef VER_369
	ballRadius = 80;
#endif

	ballGapX = (SCREEN_WIDTH - 34 * 2 - ballRadius*7.0) / 6.0;
	ballStartX = 34 + ballRadius / 2;//ballGapX+ballRadius/2;
	areaTitlePosX = 67;//42;
	areaGapY = 15;
	areaHeight = ballRadius*2+areaBkHeight+4*areaGapY;
	areaMenuPosX = 330;
	fontSize = 27;
	this->addRandSeed();
}

LiuHeBetArea::~LiuHeBetArea()
{

}

void LiuHeBetArea::getBetNumber()
{
	return;
}

String *LiuHeBetArea::getBetNumberAt(const int index)
{
	return String::create("");
}

int LiuHeBetArea::getBetBeiShuAt(const int index)
{
	return 0;
}

void LiuHeBetArea::clearNumbers()
{
	CCLOG("LiuHeBetArea::clearNumbers");
}

void LiuHeBetArea::createRandomNumbers()
{
	CCLOG("LiuHeBetArea::createRandomNumbers");
}

void LiuHeBetArea::addRandSeed()
{
	timeval psv;	
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec;
	srand(rand_seed);
}

void LiuHeBetArea::setBetKind(LiuHeCaiKind kind)
{
	CCLOG("LiuHeBetArea::setBetKind");
}

int LiuHeBetArea::combo(int base, int up)
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

int LiuHeBetArea::factorial(int number)
{
	int total = 1;
	for(int i = 1 ; i <= number; i++){
		total *= i;
	}
	return total;
}

int LiuHeBetArea::heZhiZhuShu(int heZhi)
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