#ifndef _LOTTERY_GLOBAL_H_
#define _LOTTERY_GLOBAL_H_

#include "cocos2d.h"

#define MAX_QIHAO 100

USING_NS_CC;

struct Bet_Info 
{
	Integer* m_gameKind;	//游戏id
	Integer* m_wanFaKind;	//玩法id
	String* m_betNumber;		//下注号码
	Integer* m_betCount;	//注数
	Integer* m_mode;			//元角分 0,1,2 对应着 1.0f,0.1f,0.01f
	Integer* m_betPlus;		//倍数

public: double getBetMoney()
		{
			float nMoshiNumber = 1;
			if(m_mode->getValue() == 0)
			{
				nMoshiNumber =1;		
			}else if(m_mode->getValue() == 1)
			{
				nMoshiNumber =0.1;			
			}else if(m_mode->getValue() == 2)
			{
				nMoshiNumber = 0.01;		
			}else if(m_mode->getValue() == 3)
			{
				nMoshiNumber = 0.001;
			}

			return m_betCount->getValue() * nMoshiNumber * m_betPlus->getValue() * 2;
		}
};

static float ChongQingcartAbs(float a)
{
	return a>0?a:-a;
}

class MyBetNumber
{
private:

	static MyBetNumber* _instance;
	//投注数组
	std::vector<Bet_Info> betNumbers;	
	//总共注
	int betCount;
	//弹窗类别
	int retType;

	Array* m_arr_caizhong;

	MyBetNumber(){betCount = 0;retType = 0;m_arr_caizhong = NULL;}
	~MyBetNumber();	

public:
	static MyBetNumber* getInstance();	

	void addItem(Bet_Info item);
	void removeById(int id);
	void clear();
	int size();
	Bet_Info getItem(int id);

	void betReady();
	bool isEndBet();
	void betOneEnd();
	void setRetType(int retType);
	int getRetType(){return this->retType;}	
	static void addRandSeed();	

	void saveCaiZhong(Array* arr_caizhong);
	//返回彩种id
	int getCaiZhong(const int index);
	//彩种个数
	int getCaiZhongCount();
};


#endif //_LOTTERY_GLOBAL_H_