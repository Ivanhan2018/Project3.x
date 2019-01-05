//
//  DzConfig.h
//  TexasPoker
//
//  Created by peniel on 1/9/14.
//
//

#ifndef __TexasPoker__DzConfig__
#define __TexasPoker__DzConfig__

#include "Inc.h"

class BigHornConfig
{
public:
    BigHornConfig(){ip="";port=0;money=0;}
    ~BigHornConfig(){}  
public:
    std::string ip;
    unsigned int port;
    unsigned int money;
};

class TiggerConfig
{
public:
    TiggerConfig(){open=0;}
    ~TiggerConfig(){}
public:
    unsigned int open;
};

////瓜瓜乐
//class HappyConfig
//{   
//public:
//    HappyConfig(){open=0;money=0;}
//    ~HappyConfig(){}
//public:
//    unsigned int    open;
//    unsigned int    money;
//};

//赌圣
class DuShenConfig
{
    
public:
    DuShenConfig(){totalWinCount=0;isReward=false;}
    ~DuShenConfig(){}
    
public:
    unsigned int    totalWinCount;
    //是否已经领取奖励
    bool       isReward;
};

class DzConfig
{ 
private:
    DzConfig();
public:
    static DzConfig* getInstance();
    ~DzConfig();
    
public:
	unsigned int        online_count;
    int             login_award_info[5];
    unsigned int         brokeMoney;
    unsigned int         brokeGiveMoney;
    std::string     shareUrl;
    BigHornConfig*  pBigHornConfig;
    TiggerConfig*   pTiggerConfig;
    //HappyConfig*    pHappyConfig;
    DuShenConfig*   pDuShenConfig;
};

#endif /* defined(__TexasPoker__Config__) */
