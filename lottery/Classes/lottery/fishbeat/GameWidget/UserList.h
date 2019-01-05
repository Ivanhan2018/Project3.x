#ifndef _USERLIST_H_
#define _USERLIST_H_

#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

enum emUserListBtn
{
	em_OP_UserList_exit=70,
};

class UserList : public cocos2d::Layer
{
	ListView* m_listView ;
	Button*	m_close;
public:
	UserList(void);
	~UserList(void);
	virtual bool init();
	CREATE_FUNC(UserList);
public:
	void onCallback(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
	void addItem(std::int32_t userID);
	void removeAllItem();
	void setUserScore(unsigned short nChairID, long long score);
};

#endif //_USERLIST_H_
