#include "StringFormat.h"
#include <cstring>

char* StringFormat::replace(const char *psrc,const char rc)
{
    if (!psrc)
    {
        return NULL;
    }
    char *pdst = (char *)malloc(strlen(psrc)*2+1);
    strcpy(pdst,psrc);
    for (char *pflag = pdst; *pflag != '\0'; pflag++)
    {
        if (*pflag == rc)
        {
            memcpy(pflag,pflag+1,(strlen(pdst)-(pflag-pdst)));
            pflag--;
            continue;
        }
    }
    return pdst;
}

UNS_INT StringFormat::strToInt(const char *str)
{
    int sum=0;
    while(*str)
    {
        sum=10*sum+((*str++)-'0');
    }
    return sum;
}

void StringFormat::Split(const char* content, const char* token, CCArray* vect)
{
    if(content == NULL)
    {
        return;
    }
    std::string strContent(content);
    std::string strTemp("");
    std::string::size_type startPos = 0;
    std::string::size_type endPos = strContent.find(token,startPos);
    
    do {
        strTemp = strContent.substr(startPos,endPos-startPos);
        vect->addObject(CCString::create(strTemp));
        startPos = endPos+1;
        endPos = strContent.find(token,startPos);
        //DLog::showLog("startPos:%d--endPos:%d",startPos,endPos);
    } while (startPos != 0);
}

void StringFormat::Split(const char* content, const char* token,int tokenCount, cocos2d::CCArray* vect)
{
    if(content == NULL)
    {
        return;
    }
    std::string strContent(content);
    std::string strTemp("");
    std::string::size_type startPos = 0;
    std::string::size_type endPos = strContent.find(token,startPos);
    for (int i=0;i<tokenCount;++i)
    {
        strTemp = strContent.substr(startPos,endPos-startPos);
        vect->addObject(CCString::create(strTemp));
        startPos = endPos+1;
        endPos = strContent.find(token,startPos);
        //DLog::showLog("i:%d---startPos:%d--endPos:%d",i,startPos,endPos);
    }
}

void StringFormat::trim(std::string &str)
{
    //空格的ASCII码为32
    //去掉后面的空格
    const int k = 32;
    for (int i=str.length()-1; i>=0; --i) {
        if (str[i] == k || str[i] == '.')
        {
            str[i] = '\0';
        }
        else
        {
            break;
        }
    }
    //去掉前面的空格
    std::string::size_type endPos = str.find_last_of(k);
    str = str.substr(endPos+1);
}

const char* StringFormat::intsert(const UNS_LONG num,UNS_INT step,const char* intsertChar)
{
	CCString* cstr = CCString::create("");

	std::string numChar = CCString::createWithFormat("%ld",num)->getCString();
	CCArray* allChar = CCArray::create();
	
	UNS_INT length = numChar.length();
	UNS_INT firstInsertPoint = length%step==0 ? step : length%step;
	for (UNS_INT i=0;i<length;++i)
	{
		std::string str = "";
		str += numChar.at(i);
		allChar->addObject(CCString::createWithFormat("%s",str.c_str()));
		if (firstInsertPoint == step)
		{
			if ((i+1)%step==0 && i!=length-1)
			{
				allChar->addObject(CCString::createWithFormat("%s",intsertChar));
			}
		}
		else
		{
			if ((i == firstInsertPoint-1 || (i+1)%step==firstInsertPoint) && (i!=length-1))
			{
				allChar->addObject(CCString::createWithFormat("%s",intsertChar));
			}
		}	
	}

	for (UNS_INT i=0; i<allChar->count(); ++i)
	{
		const char* prevChar = cstr->getCString();
		const char* nextChar = ((CCString*)allChar->objectAtIndex(i))->getCString();
		cstr->initWithFormat("%s",CCString::createWithFormat("%s%s",prevChar,nextChar)->getCString());
	}
	
	return cstr->getCString();
}

const char* StringFormat::GetTime(long sec)
{
    struct tm *tm;
    time_t timep = (time_t)sec;
    tm = localtime(&timep);
    
    return CCString::createWithFormat("%d-%d-%d",(tm->tm_year+1900),(tm->tm_mon+1),tm->tm_mday)->getCString();
}

int StringFormat::float2int(float num)
{
    char *str = NULL;
    int  ret_int;
    str = float2intstr(num);
    
    ret_int = str2int(str);
    // ret_int = atol(str);
    
    return ret_int;
}

char * StringFormat::float2intstr(float fnum)
{
    char num_str[20];
    char *ret_str = NULL;
    int i = 0;
    int ret_len = 0;
    
    memset(num_str,0,20);
    sprintf(num_str,"%f",fnum);
    
    while(num_str[i]!='\0'&&num_str[i] != '.')
    {
        i++;
    }
    
    if (num_str[i] == '\0')
    {
    }else if(num_str[i] == '.')
    {
        num_str[i] = '\0';
    }
    
    ret_len = strlen(num_str);
    ret_str = (char *)malloc(ret_len+1);
    
    if (ret_str == NULL)
    {
        return NULL;
    }
    
    memset(ret_str,0,ret_len+1);
    memcpy(ret_str,num_str,ret_len);
    
    return ret_str;
}

int StringFormat::str2int(char *str)
{
    int i    = 0;
    int len  = 0;
    char *pChr = NULL;
    int temp  = 0;
    int retVal  = 0;
    int bNegative = false;
    
    pChr = str;
    if (pChr == NULL)
    {
        return 0;//error
    }
    
    while (*pChr == ' ') pChr++;
    
    
    if (*pChr == '-')
    {
        bNegative = true;
        pChr++;
    }else if(*pChr == '+')
    {
        bNegative = false;
        pChr++;
    }
    
    if (pChr == NULL)
    {
        return 0;
    }
    
    len = strlen(pChr);
    
    while(*pChr != '\0')
    {
        i=0;
        if (*pChr >= '0' && *pChr <= '9')
        {
            temp = (*pChr - '0');
            
            while(len - ++i)
            {
                temp *= 10;
            }
            
            retVal += temp;
            pChr++;
            len--;
        }else
        {
            return 0;//error
        }
        
    }
    
    if (str != NULL)
    {
        free(str);
        str = NULL;
    }
    
    if(bNegative == true)
    {
        retVal *= -1;
    }
    
    
    return retVal;
    // return atol(str);
}

int StringFormat::getRealInt(float temp,int x)
{
    if ((temp - x) > 0.5)
    {
        x = x + 1;
    }
    else if ((temp -x) < -0.5)
    {
        x = x - 1;
    }
    
    return x;
}






