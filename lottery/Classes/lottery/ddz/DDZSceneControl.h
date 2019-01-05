//  [3/19/2014 wucan]

#ifndef __DDZ__DDZSceneControl__
#define __DDZ__DDZSceneControl__

#include "cocos2d.h"
#include "DDZSceneData.h"
#include "SceneControl.h"//by hxh
USING_NS_CC;

class DDZSceneControl
{
public:
	static DDZSceneControl*		    sharedDDZSceneControl();
	Scene*						getScene(int index,bool bEffect,float t=0.4f);
};



#endif /* defined(__RunBeard__DDZSceneControl__) */
