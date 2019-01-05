#ifndef __bjpk10KindScrollView_H__
#define __bjpk10KindScrollView_H__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"

#define ITEM_TAG				1
#define MOVE_GAP				1
#define MENU_WIDTH	SCREEN_WIDTH - 100
#define MENU_HEIGT				350

class bjpk10KindScrollView : public Layer
{

public:
	bjpk10KindScrollView();
	~bjpk10KindScrollView();
	virtual bool init();
	static Scene* scene();
	void pressKindButton(int tag);
	CREATE_FUNC(bjpk10KindScrollView);

	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);

	void refreshContent();
	void refreshMenu();

private:
	cocos2d::Size winSize;
	Node *m_node;
	float m_start;
	float m_end;
	Vec2 m_endPos;
	Vec2 startPoint;
	Vec2 endPoint;
	int selectedSpriteTag;

	//std::vector<int> m_list;

	int totalItemSpriteCount;
	Sprite *selectedSprite;
	void initView();
	void initData();
	LayerColor* lyWanFa;
	bool m_isNowDel;

	//สýื้
	std::vector<Vec2>  m_vecPot;
	bool m_bDrag;
};
#endif  //__Game__HelpScrollView__