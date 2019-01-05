#ifndef __Game__OverRegistLayer__
#define __Game__OverRegistLayer__

#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace extension;
class OverRegistLayer: public Layer
{
private:
	EditBox * m_pUserNameKuang;
	EditBox * m_pPassWordKuang;
	EditBox * m_pPassWordKuangOK;
public:
	OverRegistLayer();
	~OverRegistLayer();
	virtual bool init();

	CREATE_FUNC(OverRegistLayer);

	void setRegistLayerFrom(int num);

public:
	void dismiss(Object *obj);
	void showPlayerXieYi(Object *obj);

	bool isLegal(string str);



	void onOverRes(Object* obj);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮

private:
	Sprite* spriteSlectSex;
	int m_nRegistFrom;
	cocos2d::Size winSize;
};
#endif  //__Game__RegistLayer__