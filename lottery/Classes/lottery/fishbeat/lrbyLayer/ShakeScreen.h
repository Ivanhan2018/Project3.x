#ifndef __GAME_SHAKE_SCREEN_H__
#define __GAME_SHAKE_SCREEN_H__

#include <iostream>
#include "cocos2d.h"


using namespace cocos2d;
using namespace std;

class ShakeScreen : public ActionInterval
{
public:
	ShakeScreen();
	virtual ~ShakeScreen();
	//param d 震动持续的时间
	//param strength 震动的幅度
	static ShakeScreen* create(float d, float strength);
	//产生震动效果的初始化函数，两个方向值不同
	static ShakeScreen* create(float d, float strength_x, float strength_y);
	bool initWithDuration(float d, float strength_x, float strength_y);

	virtual ShakeScreen* clone() const override;
	virtual ShakeScreen* reverse() const override;
	virtual void startWithTarget(Node* target) override;
	virtual void update(float time) override;
	virtual void stop(void);

protected:
	//精灵的位置
	float _initial_x, _initial_y;
	//抖动的幅度
	float _strength_x, _strength_y;
	bool _bShake;
};


#endif // __GAME_SHAKE_SCREEN_H__