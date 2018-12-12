#ifndef		__JNICONTROL_WUCAN__
#define		__JNICONTROL_WUCAN__
#include "cocos2d.h"
USING_NS_CC;

class JniControl:public Object
{
public:
	JniControl(void);
	~JniControl(void);
	static JniControl* sharedJniControl();

	void JniC_MsgToAndroid(Object* obj);
};

#endif