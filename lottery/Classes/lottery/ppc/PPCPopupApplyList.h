#ifndef __PPC_POPUP_APPLY_LIST_H__
#define __PPC_POPUP_APPLY_LIST_H__

#include "cocos2d.h"
#include "BaseDialog.h"
#include "PPCEnum.h"
#include "PPCRes.h"
#include "PPCProtocol.h"
#include "GlobalDef.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;
using namespace ui;

//	碰碰车——上装列表
class PPCPopupApplyList: public Layer
{
public:
	PPCPopupApplyList();
	~PPCPopupApplyList();
	CREATE_FUNC(PPCPopupApplyList);

	//	刷新列表
	void updateListView();

	//	添加列表项
	void addListItem(int nChairID);

	//	添加列表项
	void addListItem(ppcTagBankerListPlayer sBankerData);

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

	//	点击关闭按钮
	void onClickBtnClosed(Ref* pSender);

	//	背景
	Sprite* m_pBG;

	//	关闭按钮
	ui::Button* m_pClosed;

	//	列表视图
	ui::ListView* m_pListView;

	//	列表子项数组
	map<int, Widget*> m_mItemArray;

};

#endif  // __PPC_POPUP_APPLY_LIST_H__