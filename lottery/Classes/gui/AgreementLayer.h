#ifndef __Game__AgreementLayer__
#define __Game__AgreementLayer__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
class AgreementLayer : public Layer
{

public:
	AgreementLayer();
	~AgreementLayer();
	virtual bool init();
	static Scene* scene();

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮

	CREATE_FUNC(AgreementLayer);
public:

	void dismiss(Object *obj);

};
#endif  //__Game__AgreementLayer__