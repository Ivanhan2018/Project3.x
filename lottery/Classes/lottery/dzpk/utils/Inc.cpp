//
//  Inc.cpp
//  gamePoker
//
//  Created by xavier on 14-1-5.
//
//

#include "Inc.h"
#include <time.h>

extern "C"
{
    double deltaT=0.0;
    
    double getCurrentTime()
    {
        double ts = time(NULL);
        return ts;
    }
    
    double getServerTime()
    {
        return getCurrentTime()-deltaT;
    }
    
    double getDeltaT(){
        return deltaT;
    }
    
    void setDeltaT(double t){
        deltaT = t;
    }
    
    void getOtherHeader(int uid,int seatid)
    {
        //头像
        if (seatid<0 || seatid>8)
            return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (isWifiActive()=="yes"){
            CCHttpAgent::getInstance()->doGetImageByUri(seatid, uid, geAvatarShortPath(uid),true);
        }
#endif
    }
    
}


