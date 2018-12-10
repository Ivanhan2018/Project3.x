#include "NetLog.h"
#include "cocos2d.h"
namespace NetLog
{
	void Log(std::string kStr)
	{
		cocos2d::log("%s",kStr.c_str());
	}
}