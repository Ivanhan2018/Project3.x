//  [2/28/2014 wucan]

#ifndef		DDZ___JNICONTROL_WUCAN__
#define		DDZ___JNICONTROL_WUCAN__
#include "cocos2d.h"
USING_NS_CC;

class DDZJniControl:public Object
{
public:
	DDZJniControl(void);
	~DDZJniControl(void);
	static DDZJniControl* sharedDDZJniControl();

	void JniC_MsgToAndroid(Object* obj);
};

#endif