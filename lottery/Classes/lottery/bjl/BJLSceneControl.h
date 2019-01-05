//  [3/19/2014 wucan]

#ifndef __BJL__SceneControl__
#define __BJL__SceneControl__

#include "cocos2d.h"
#include "SceneData.h"
#include "SceneControl.h"//by hxh
USING_NS_CC;

class BJLSceneControl
{
public:
	static BJLSceneControl*		    sharedSceneControl();
	Scene*						getScene(int index,bool bEffect,float t=0.4f);
};



#endif /* defined(__RunBeard__SceneControl__) */
