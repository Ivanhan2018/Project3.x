#ifndef __Game__FreeLayer__
#define __Game__FreeLayer__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"

#include "GlobalDef.h"
#include "CMD_Plaza.h"

class FreeLayer : public Layer
{

public:
	FreeLayer();
	~FreeLayer();
	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(FreeLayer);


	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮

	void onGetDownData(Object* obj);
	void getDownAwardOver(Object* obj);
public:

	void dismiss(Object *obj);
	void downLoad(Object *obj);

	SDownloadGamePrize_Returnn returnData;

};
#endif  //__Game__FreeLayer__