#ifndef __Game__DLog__
#define __Game__DLog__

//#include "Inc.h"

static bool isShowLog = true;

class DLog
{
    
public:
    
    static void showLog(const char * pszFormat, ...)
    {
        if (!isShowLog) {
            return;
        }
        
        //printf("peniel Log: ");
        /*char szBuf[cocos2d::kMaxLogLen+1] = {0};
        va_list ap;
        va_start(ap, pszFormat);
        vsnprintf(szBuf, cocos2d::kMaxLogLen, pszFormat, ap);
        va_end(ap);*/
        //printf("%s", szBuf);
        //printf("\n");
    }
    
    static void showLog(const char pszFormat, ...)
    {
        showLog(&pszFormat);
    }
    
    static void print(int* array,int size)
    {
        if (!isShowLog) {
            return;
        }
        if (!array) {
            return;
        }
        //printf("peniel Log: ");
        for (int i=0; i<size; ++i) {
            //printf("array[%d]=%d ",i,array[i]);
        }
    }
    
};

#endif