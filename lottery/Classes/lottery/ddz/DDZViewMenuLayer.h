//  [3/26/2014 xieyi]
#ifndef	DDZ___VIEW_MENU_LAYER__
#define	DDZ___VIEW_MENU_LAYER__
#include "cocos2d.h"
#include "DDZDDZRes.h"
#include "DDZSceneData.h"
#include "DDZTimer.h"
#include "ui/CocosGUI.h"
#include "DDZDialogLayer.h"

USING_NS_CC;
using namespace ui;


class DDZViewMenuLayer : public Layer
{
public:
	DDZViewMenuLayer();
	~DDZViewMenuLayer();
	CREATE_FUNC(DDZViewMenuLayer);
	virtual bool init();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);

	void enterGameView();		//进入对局界面
	void startGame();		//开始游戏
	void visibleCard();		//明牌
	void jiaoDiZhu();		//叫地主
	void qiangDiZhu();		//抢地主
	void outCard(int state);			//出牌
	void jiaBei(bool isShow);			//加倍
	void firstOutCard();	//第一轮出牌
	void diZhuFirstOutCard();	//地主第一轮出牌
	void gameDDZSettlement();	//游戏结算
	void finishClick();		//完成按钮点击事件(隐藏按钮)
	void setVisibleStartNumber(int num);	//设置明牌开始倍率
	void setTimer(int userId,DDZGame_State gameState,int t,bool visible,int key);
	void closeTimer();
	void hintVisibleCardItem();		//隐藏明牌x2按钮
	void setRobotEnable(bool enable);//设置机器人是否可用
	void setTalkEnable(bool enable);//设置聊天是否可用

	bool isShowBtns();//是否展示打牌相关按钮

	//	设置按钮菜单项是否显示
	void setMenuItemVisible(DDZMenuItemType type, bool isVisible);

	bool m_isVisibleStart;	//是否明牌开始
private:
	//菜单按钮回调
	void menuCallback(Object * pSender);			

	//添加按钮项
	MenuItemSprite* addMenuItem(const char * normal,const char * selected,const char * disable,
		Object * context,SEL_MenuHandler selector,Vec2 point,
		Object * pUserObject, int tag,bool visible);

	MenuItemSprite* addMenuBarItem(const char * normal,const char * selected,const char * disable,
		Object * context,SEL_MenuHandler selector,Vec2 point,
		Object * pUserObject, int tag,bool visible);

	//	点击菜单栏拉环
	void onClickPullTab(Ref* sender);

	//	点击返回
	void onClickBack(Ref* sender);

	//	点击设置
	void onClickSetting(Ref* sender);

private:
	CC_SYNTHESIZE(Menu *,m_menu,Menu);					//	按钮
	CC_SYNTHESIZE(DDZTimer*,m_timer,DDZTimer);			//	计时器
	Menu* m_menuBar;									//	菜单栏
	float m_menuPositionY;								//	按钮位置Y坐标
	Sprite* m_pMenuBG;									//	菜单栏背景
	Button* m_pPullTab;									//	菜单栏拉环
	bool m_bIsShowMenu;									//	是否显示菜单栏
	MenuItemSprite* m_pMenuItem[DDZmenu_Count];			//	菜单项数组
};

#endif