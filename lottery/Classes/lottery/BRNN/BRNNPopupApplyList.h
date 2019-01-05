#ifndef __BRNN_POPUP_APPLY_LIST_H__
#define __BRNN_POPUP_APPLY_LIST_H__

#include "cocos2d.h"
#include "GlobalDef.h"
#include "ui/CocosGUI.h"
#include "BRNNProtocol.h"

USING_NS_CC;
using namespace std;
using namespace ui;

/*
**	@brief	百人牛牛——上庄列表
*/
class BRNNPopupApplyList: public Layer
{
public:
	BRNNPopupApplyList();
	~BRNNPopupApplyList();
	CREATE_FUNC(BRNNPopupApplyList);

	//	刷新列表
	void updateListView();

	//	设置弹窗是否显示
	void setPopupVisible(bool bIsVisible);

	//	添加列表项
	void addListItem(int nChairID);

	//	添加列表项
	void addListItem(BRNN_tagBankerListPlayer sBankerData);

	//	获得列表子项
	Widget* getListItem(int nChairID);

	//	删除列表项
	void removeListItem(int nChairID);

	//	删除所有列表项
	void removeAllListItem();

	//	判断列表项是否存在
	bool isItemExist(int nChairID);
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

	//	标题——头像
	Sprite* m_pTitleFace;

	//	标题——昵称
	Sprite* m_pTitleNickname;

	//	标题——金额
	Sprite* m_pTitleGold;

	//	列表视图
	ListView* m_pListView;

	//	列表子项数组
	map<int, Widget*> m_mItemArray;

};

#endif  // __BRNN_POPUP_APPLY_LIST_H__