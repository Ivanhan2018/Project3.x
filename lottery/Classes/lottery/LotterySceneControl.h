//  [3/19/2014 wucan]

#ifndef __LotterySceneControl__
#define __LotterySceneControl__

#include "cocos2d.h"
USING_NS_CC;

class LotterySceneControl
{
public:
	static LotterySceneControl*		    sharedSceneControl();
	Scene*	getScene(Scene *s);
};



#endif /* defined(__RunBeard__SceneControl__) */
