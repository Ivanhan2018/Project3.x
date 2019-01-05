#ifndef __Game__RegistLayer__
#define __Game__RegistLayer__

#include "CocosUnits.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace extension;
class RegistLayer: public Layer
{
private:
	EditBox * m_pUserNameKuang;
	EditBox * m_pPassWordKuang;
	EditBox * m_pPassWordKuangOK;
public:
	RegistLayer();
	~RegistLayer();
	virtual bool init();

	CREATE_FUNC(RegistLayer);

	void setRegistLayerFrom(int num);

public:
	void dismiss(Object *obj);
	void showPlayerXieYi(Object *obj);
	void showAgreePlayerXieYi(Object *obj);
	bool isLegal(string str);

	void selectMan(Object* obj);
	void selectWoman(Object* obj);

	void onRegisterFaild(Object* obj);
	void onRegistSucc(Object* obj);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮

private:
	int m_nRegistFrom;
	Sprite* spriteSlectSex;

	int n
		;
	int nGenter;
	cocos2d::Size winSize;
};
#endif  //__Game__RegistLayer__