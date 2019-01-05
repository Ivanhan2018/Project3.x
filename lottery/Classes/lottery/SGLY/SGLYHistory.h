#ifndef __SGLY_HISTORY_H__
#define __SGLY_HISTORY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
using namespace std;

/*
**	@brief	水果乐园——历史记录
*/
class SGLYHistory: public Node
{
public:
	SGLYHistory();
	~SGLYHistory();
	CREATE_FUNC(SGLYHistory);

	//	添加历史记录
	void addHistoryItem(int nLotteryIdx);

	//	获取路单列表项数量
	int getHistoryItemCount();
private:
	bool init();
	void initUI();

	//	路单列表视图
	ListView* m_pListView;
};


#endif  // __SGLY_HISTORY_H__