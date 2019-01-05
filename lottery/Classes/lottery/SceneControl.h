//  [3/19/2014 wucan]

#ifndef __DDZ__SceneControl__
#define __DDZ__SceneControl__

#include "cocos2d.h"
//#include "SceneData.h"
USING_NS_CC;

enum
{
	SCENE_start=1,			//´óÌü
	SCENE_game,
	SCENE_endAgain
};

class SceneControl
{
public:
	static SceneControl*		    sharedSceneControl();
	Scene*						getScene(int index,bool bEffect,float t=0.4f);
};



#endif /* defined(__RunBeard__SceneControl__) */
