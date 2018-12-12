#ifndef __BJL__SceneControl__
#define __BJL__SceneControl__

#include "cocos2d.h"
#include "CMD_BJL.h"
#include "SceneControl.h"
USING_NS_CC;

class BJLSceneControl
{
public:
	static BJLSceneControl*		    sharedSceneControl();
	Scene*						getScene(int index,bool bEffect,float t=0.4f);
};



#endif