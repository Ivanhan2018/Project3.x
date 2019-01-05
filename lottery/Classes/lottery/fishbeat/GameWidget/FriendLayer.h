#ifndef _FriendLayer_H_
#define _FriendLayer_H_
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
using namespace ui;
USING_NS_CC_EXT;

class FriendLayer :  public LayerColor
{
private:
	enum Friend_Menu
	{
		Menu_MF,
		Menu_AF,
		Menu_AG,
		Menu_CG,

	};

public:
	FriendLayer(void);
	~FriendLayer(void);

	virtual bool init(float fScale);
	static FriendLayer *create(float fScale);
	void SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
	void setMenuMode(Friend_Menu mode);



	void onMenuSelect(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);







private:
	Widget*	      m_widgetUI;
	Button*		  server_close;
	Layout*		m_panel;

	Button*		m_btnMyFriend;
	Button*		m_btnAddFriend;
	Button*		m_btnAddGroup;
	Button*		m_btnCreateGroup;


};

#endif // !_NoticeLayer_H_
