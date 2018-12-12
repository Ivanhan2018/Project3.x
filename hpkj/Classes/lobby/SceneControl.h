#ifndef __SCENE_CONTROL__
#define __SCENE_CONTROL__

#include "cocos2d.h"
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
	static SceneControl*		sharedSceneControl();
	Scene*						getScene(int index,bool bEffect,float t=0.4f);
	Scene*	                    getScene(Scene *s);
};

#endif
