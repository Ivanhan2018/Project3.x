#ifndef __PPC_ENDING_H__
#define __PPC_ENDING_H__

#include "cocos2d.h"
#include "BaseDialog.h"
#include "PPCEnum.h"
#include "PPCRes.h"

USING_NS_CC;
using namespace std;



//	结算界面
class PPCEnding: public Layer
{
public:
	PPCEnding();
	~PPCEnding();

	CREATE_FUNC(PPCEnding);

	//	玩家得分
	Label* m_pLabPlayerScore;

	//	玩家返回本金
	Label* m_pLabPlayerCapital;

	//	庄家得分
	Label* m_pLabBankerScore;

	//	庄家返回本金
	Label* m_pLabBankerCapital;

private:
	bool init();
	void initUI();

	//	背景
	Sprite* m_pBG;


};

#endif  // __PPC_ENDING_H__