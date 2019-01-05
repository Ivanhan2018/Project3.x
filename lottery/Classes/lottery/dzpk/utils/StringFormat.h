#ifndef __Game__StringFormat__
#define __Game__StringFormat__

#include "Inc.h"

class StringFormat
{
    
public:
    static char* replace(const char *psrc,const char rc);
    static UNS_INT strToInt(const char *str);
    static void Split(const char* content, const char* token, cocos2d::CCArray* vect);
    static void Split(const char* content, const char* token,int tokenCount, cocos2d::CCArray* vect);
    //去掉空格
    static void trim(std::string& str);
	//将整数转换成9,999,999这种格式
	static const char* intsert(const UNS_LONG num,UNS_INT step,const char* intsertChar);
    
    static const char* GetTime(long sec);
    
    static int float2int(float num);
    static char * float2intstr(float fnum);
    static int str2int(char *str);
    static int getRealInt(float temp,int x);

};

#endif