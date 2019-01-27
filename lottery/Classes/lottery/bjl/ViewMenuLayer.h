//  [3/26/2014 xieyi]
#ifndef		__VIEW_MENU_LAYER__
#define		__VIEW_MENU_LAYER__
#include "cocos2d.h"
#include "BJLRes.h"
#include "SceneData.h"
#include "MyTimer.h"
USING_NS_CC;

class ViewMenuLayer : public Layer
{
public:
	CREATE_FUNC(ViewMenuLayer);
	virtual bool init();
	ViewMenuLayer();
	~ViewMenuLayer();
	bool m_isVisibleStart;	//是否明牌开始
	void setTimer(int userId,Game_State gameState,int t,bool visible,int key);

	void closeTimer();
	void SetTimeSprite();

	//virtual void keyBackClicked(void);				//返回
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void setMenuNameTipsView(bool isVisible);
private:
								//添加按钮项
	void addMenuBarItem(const char * normal,const char * selected,const char * disable,
		Object * context,SEL_MenuHandler selector,Vec2 point,
		Object * pUserObject, int tag,bool visible);
	//Sprite* addMenuBarTips(const char * name,float delTime,Vec2 pt);
	void removeSelfCall(Node* _node);
private:
	CC_SYNTHESIZE(Menu *,m_menu,Menu);								//按钮
	Menu * m_menuBar;			//菜单栏
	Node * m_menuBarNode;		//菜单栏节点
	Node*  m_menuBarTips;
	bool m_menuBarIsShow;		//菜单栏是否显示
	CC_SYNTHESIZE(MyTimer*,m_timer,MyTimer);			//计时器
	float m_menuPositionY;
	bool m_isAutoHints;		//是否自动隐藏
};

#endif