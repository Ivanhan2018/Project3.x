#ifndef __Game__RechargeLayer__
#define __Game__RechargeLayer__

#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
class RechargeLayer : public Layer
{


public:
	RechargeLayer();
	~RechargeLayer();
	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(RechargeLayer);

public:
	void dismiss(Object *obj);
	void makeSure(Object *obj);
};
#endif  //__Game__RechargeLayer__