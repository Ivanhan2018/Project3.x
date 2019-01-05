#ifndef __LotteryUpdate__
#define __LotteryUpdate__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryUpdate
{

public:
	LotteryUpdate();
	~LotteryUpdate();	

	static void showModal(Node* nowLayer,std::string updateUrl);

	void showPopWndUpdate(Node* nowLayer,int idx);

private:
	std::string updateUrl;
};

#endif