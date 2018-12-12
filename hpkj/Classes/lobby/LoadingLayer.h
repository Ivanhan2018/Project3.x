#ifndef __Game__LoadingLayer__
#define __Game__LoadingLayer__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
class LoadingLayer : public Layer
{

public:
	LoadingLayer();
	~LoadingLayer();
	virtual bool init();
	static Scene* scene();
	void update(float dt);
	CREATE_FUNC(LoadingLayer);
	void onEnter();
private:
	ProgressTimer *pJinDuTiao;
};
#endif  //__Game__LoadingLayer__