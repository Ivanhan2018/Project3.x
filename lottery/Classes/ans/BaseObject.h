

#ifndef __MSG_HANDLER__
#define __MSG_HANDLER__

//#include "ans.h"

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#pragma warning(disable:4407)	// temporarily. disable it in VC
//#endif
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_QT)
//#pragma warning(disable:4100)	// temporarily. disable it in VC
//#pragma warning(disable:4407)	// temporarily. disable it in VC
//#endif

#include "cocos2d.h"

//using namespace std;

class BaseObject
{
public:
	BaseObject(){}
	virtual ~BaseObject(){}
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#define  COMPATIBLE_CAST  static_cast
#else
	#define  COMPATIBLE_CAST  reinterpret_cast 
#endif

#endif