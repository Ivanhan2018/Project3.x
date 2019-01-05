//
//  DzConfig.cpp
//  TexasPoker
//
//  Created by peniel on 1/9/14.
//
//

#include "DzConfig.h"

static DzConfig* instance = NULL;

DzConfig::DzConfig()
{
	online_count=0;
	brokeMoney=0;
	brokeGiveMoney=0;
	shareUrl="";
	pBigHornConfig=NULL;
	pTiggerConfig=NULL;
	//pHappyConfig=NULL;
	for (int i=0; i<5; ++i)
	{
		login_award_info[i] = 0;
	}
}

DzConfig::~DzConfig()
{
    CC_SAFE_DELETE(pBigHornConfig);
    CC_SAFE_DELETE(pTiggerConfig);
    //CC_SAFE_DELETE(pHappyConfig);
    CC_SAFE_DELETE(pDuShenConfig);
}

DzConfig* DzConfig::getInstance()
{
    if (instance == NULL)
    {
        instance = new DzConfig();
    }
    return instance;
}