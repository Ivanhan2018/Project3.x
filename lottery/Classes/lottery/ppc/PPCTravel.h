#ifndef __PPC_TRAVEL_H__
#define __PPC_TRAVEL_H__

#include "cocos2d.h"
#include "PPCEnum.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
using namespace std;

//	碰碰车——路单
class PPCTravel: public Node
{
public:
	PPCTravel();
	~PPCTravel();
	CREATE_FUNC(PPCTravel);

	//	添加路单
	void addTravelItem(EPPCCarLogoType eType);

	//	获取路单列表项数量
	int getTraveItemCount();
private:
	bool init();
	void initUI();

	//	路单列表视图
	ListView* m_pListView;
};


#endif  // __PPC_TRAVEL_H__