#ifndef _LOTTERY_BET_DATA_H_
#define _LOTTERY_BET_DATA_H_

#include "cocos2d.h"

#define LIST_MAX_SIZE 9

USING_NS_CC;


class MyBetData
{
private:

	static MyBetData* _instance;
	//用户已经选择的玩法 序列 id是和相对的xml里面的是一样的
	std::vector<int> m_wanfaList;
	//用户彩种参数 第一个参数是当前玩法，第二个参数是当前元角分
	std::vector<int> m_paramList;
	//当前彩种的id
	int m_curCaizhong;
	//玩法key
	std::string m_wanfaKey;
	//参数key
	std::string m_paramKey;
	
	MyBetData();
	~MyBetData();	

public:
	static MyBetData* getInstance();	

	//id
	bool addWanfaToList(const int wanfaId);
	bool delWanfaFromList(const int wanfaId);
	//设置当前彩种
	void setCurCaiZhongId(const int caizhongId);
	int getCurCaiZhongId();
	//上一次玩法
	void setLastWanfaId(const int wanfaId);
	int getLastWanfaId();
	//上一次元角分
	void setLastMoShi(const int moshi);
	int getLastMoshi();
	//保存数据
	void saveData(bool isSaveWanfa = true,bool isSaveParam = true);
	//读取数据
	void reloadData();

	//玩法列表大小
	int getWanfaSize();
	//玩法列表的值
	int getWanfaAt(const int index);

private:

	//分割字符串
	void splitString(std::vector<int>& vec,std::string str);

};


#endif //_LOTTERY_BET_DATA_H_