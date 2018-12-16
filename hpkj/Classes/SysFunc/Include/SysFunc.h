
#ifndef _SYS_FUNC_H_
#define _SYS_FUNC_H_
#include "Define.h"

class SysFunc {
    
public:
    ////////////////////////////////////////////////////////////////////////////
	// 功能描述:  切换到竖屏
    static void ToPortrait();

    ////////////////////////////////////////////////////////////////////////////
	// 功能描述:  切换到横屏
    static void ToLandscape();

	// 获取手机设备码
    static std::string WHGetMacAddress();

	// 获取当前时间，单位为毫秒
    static LONGLONG getsystemtomillisecond();

	// 获取当前时间戳(精确到秒)
	static time_t getTime();
	static std::string getTimeStr(time_t timesec,int flag);
	static std::string getDateNow();
};


#endif
