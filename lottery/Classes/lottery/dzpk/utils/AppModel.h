#ifndef __TexasPoker__AppModel__
#define __TexasPoker__AppModel__

#include "Inc.h"

class AppModel
{
    
public:
    AppModel();
    ~AppModel();
    
public:
    /*
     app_status: 0,
     app_desc: "应用服务器修理中，期间为10:00 ~ 13:00，请耐心等候，为你带来不变，非常抱歉。",
     app: "DZ.apk",
     url: "http://zjh.boxwan.cn/download/ZJH.apk",
     verName: "1.1.1",
     verCode: 1,
     features: "1、增加托管； 2、增加自动比牌设置。 ",
     apkSize: 10,
     isForce: 1
     */
    UNS_INT             app_status;
    std::string         app_desc;
    std::string         app;
    std::string         url;
    std::string         verName;
    std::string         features;
    UNS_INT             verCode;
    UNS_INT             apkSize;
    UNS_INT             isForce;
    
};

#endif
