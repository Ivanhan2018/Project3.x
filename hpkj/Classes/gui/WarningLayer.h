#ifndef __Game__WarningLayler__
#define __Game__WarningLayler__

#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include "PromptBox.h"
#include <iostream>
#include "cocos2d.h"

class WarningLayer : public Layer
{
private:
	cocos2d::Size winSize;
	int signInDays;
	Sprite *signInImage;
	Array *awardImage;
	Menu *signInMenu;
	PromptBox* promptBox;
	bool cancelOrNot;
private:
	/**保存是否点击到勾中**/

public:
	WarningLayer();
	~WarningLayer();
	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(WarningLayer);
	void setSignInDays(int days);
	void requestSignInDays();					//网页查询签到天数
	void signIn();								//签到
	void cancelHttpRequest(Object *obj);
	void onHttpRequestCompleted(Node *sender, void *data);//查询签到天数回到
	void onSignInCompleted(Node *sender, void *data);//签到回调
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮
public:
	void dismiss(Object *obj);
	void onGetTaskAward(Object* obj);
};
#endif  //__Game__WarningLayler__