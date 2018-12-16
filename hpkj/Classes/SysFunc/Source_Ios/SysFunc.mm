#include "SysFunc.h"
#include "Encrypt.h"

void SysFunc::ToPortrait()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_IOS)

#endif
}

void SysFunc::ToLandscape()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_IOS)

#endif
}

//mac address
std::string SysFunc::WHGetMacAddress()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *macaddress = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    std::string test = [macaddress UTF8String];
    test = CMD5Encrypt::MD5Encrypt(test);
    return test;
#endif    
}