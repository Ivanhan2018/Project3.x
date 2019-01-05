#ifndef __BRNN_POPUP_TRAVEL_H__
#define __BRNN_POPUP_TRAVEL_H__

#include "cocos2d.h"
#include "GlobalDef.h"
#include "ui/CocosGUI.h"
#include "BRNNProtocol.h"

USING_NS_CC;
using namespace ui;

/*
**	@brief	百人牛牛——路单
*/
class BRNNPopupTravel: public Layer
{
public:
	BRNNPopupTravel();
	~BRNNPopupTravel();
	CREATE_FUNC(BRNNPopupTravel);

	//	添加子项
	void addListItem(BRNN_tagServerGameRecord sData);

	//	获取路单列表项数量
	int getItemCount();

	//	设置弹窗是否显示
	void setPopupVisible(bool bIsVisible);

private:
	bool init();
	void initUI();

	//	单点触摸事件——开始
	bool onTouchBegan(Touch *touch, Event *unused_event);

	//	单点触摸事件——移动
	void onTouchMoved(Touch *touch, Event *unused_event);

	//	单点触摸事件——取消
	void onTouchCancelled(Touch *touch, Event *unused_event);

	//	单点触摸事件——结束
	void onTouchEnded(Touch *touch, Event *unused_event);

	//	背景
	Sprite* m_pBG;

	//	列表视图
	ListView* m_pListView;

	//	列表子项数组
	map<int, Widget*> m_mItemArray;
};

#endif  // __BRNN_POPUP_TRAVEL_H__