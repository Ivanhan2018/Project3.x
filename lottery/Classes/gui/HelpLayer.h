#ifndef __Game__HelpLayer__
#define __Game__HelpLayer__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
class HelpLayer : public Layer
{

public:
	HelpLayer();
	~HelpLayer();
	virtual bool init();
	static Scene* scene();

	
	CREATE_FUNC(HelpLayer);
public:
	
	void dismiss(Object *obj);
	void changeTipsContent(float dt);
	void toFreeLayer(Object *obj);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮

private:
	Node *pNode;
	float m_start;
	float m_end;

	LabelTTF* m_pXiaoTiShi;

};
#endif  //__Game__HelpLayer__