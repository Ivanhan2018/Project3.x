#include "MyNSString.h"
#include "Define.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <Foundation/Foundation.h>
#include "private.h"
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include<Windows.h>  
#else  
#include<unistd.h>   
#include<sys/types.h>  
#include<strings.h>  
#include <netinet/in.h>
#endif  

#include <limits.h>  
#include <locale.h>  

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "F:/cocos2d-x-2.2.5\cocos2dx\platform\third_party\win32\iconv\iconv.h"
#include <malloc.h> 
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "F:/cocos2d-x-2.2.5/iconv/include/iconv.h"
#else
#include "iconv.h"
#include <mm_malloc.h>
#endif

#include <stdio.h>  
#include <string.h>   
#include <memory.h>  

#ifdef WIN32  
//#if 0
#define uint8_t  unsigned __int8  
#define uint16_t unsigned __int16  
#define uint32_t unsigned __int32  
#define uint64_t unsigned __int64  

#define int8_t  __int8  
#define int16_t __int16  
#define int32_t __int32  
#else
#define uint8_t  unsigned char  
#define uint16_t unsigned short  
#define uint32_t unsigned __int32  
#define uint64_t unsigned __int64  

#define int8_t  char  
#define int16_t short  
#define int32_t __int32  
#define CP_UTF8                   65001       // UTF-8 translation

#endif 
#define OUTLEN 255

#ifdef WIN32  
int code_convert(char *from_charset,char *to_charset,const char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	iconv_t cd;
	int rc;
	const char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,outlen);
	if (iconv(cd,pin,&inlen,pout,&outlen)==-1)
		return -1;
	iconv_close(cd);
	return 0;
}
#else
int code_convert(char *from_charset,char *to_charset,const char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	iconv_t cd;
	int rc;
	char *in = new char[2048];
	memcpy(in, inbuf, inlen);
	char **pin = &in;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,outlen);
	if (iconv(cd,pin,&inlen,pout,&outlen)==-1)
		return -1;
	iconv_close(cd);
	return 0;
}
#endif 

//UTF-8转GB2312
int u2g(const char *inbuf,int inlen,char *outbuf,int outlen)
{
	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}

//GB2312转UTF-8
int g2u(const char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}

int enc_get_utf8_size(const unsigned char pInput)
{
	unsigned char c = pInput;
	// 0xxxxxxx 返回0
	// 10xxxxxx 不存在
	// 110xxxxx 返回2
	// 1110xxxx 返回3
	// 11110xxx 返回4
	// 111110xx 返回5
	// 1111110x 返回6
	if(c< 0x80) return 0;
	if(c>=0x80 && c<0xC0) return -1;
	if(c>=0xC0 && c<0xE0) return 2;
	if(c>=0xE0 && c<0xF0) return 3;
	if(c>=0xF0 && c<0xF8) return 4;
	if(c>=0xF8 && c<0xFC) return 5;
	if(c>=0xFC) return 6;
}

/***************************************************************************** 
 * 将一个字符的Unicode(UCS-2和UCS-4)编码转换成UTF-8编码.
 * 
 * 参数:
 *    unic     字符的Unicode编码值
 *    pOutput  指向输出的用于存储UTF8编码值的缓冲区的指针
 *    outsize  pOutput缓冲的大小
 * 
 * 返回值:
 *    返回转换后的字符的UTF8编码所占的字节数, 如果出错则返回 0 .
 * 
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 *     2. 请保证 pOutput 缓冲区有最少有 6 字节的空间大小!  这里不再判断大小
 ****************************************************************************/  
int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput)  
{  
    assert(pOutput != NULL);     
  
    if ( unic <= 0x0000007F )  
    {  
        // * U-00000000 - U-0000007F:  0xxxxxxx  
        *pOutput     = (unic & 0x7F);  
        return 1;  
    }  
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )  
    {  
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
        *(pOutput+1) = (unic & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;  
        return 2;  
    }  
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )  
    {  
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
        *(pOutput+2) = (unic & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;  
        return 3;  
    }  
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )  
    {  
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+3) = (unic & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;  
        return 4;  
    }  
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )  
    {  
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+4) = (unic & 0x3F) | 0x80;  
        *(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;  
        return 5;  
    }  
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )  
    {  
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+5) = (unic & 0x3F) | 0x80;  
        *(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;  
        return 6;  
    }  
  
    return 0;  
}  

/***************************************************************************** 
 * 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 * 
 * 参数:
 *    pInput      指向输入缓冲区, 以UTF-8编码
 *    Unic        指向输出缓冲区, 其保存的数据即是Unicode编码值,
 *                类型为unsigned long .
 * 
 * 返回值:
 *    成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
 * 
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/  
int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned long *Unic)  
{  
    assert(pInput != NULL && Unic != NULL);  
  
    // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
    char b1, b2, b3, b4, b5, b6;  
  
    *Unic = 0x0; // 把 *Unic 初始化为全零
    int utfbytes = enc_get_utf8_size(*pInput);  
    unsigned char *pOutput = (unsigned char *) Unic;  
  
    switch ( utfbytes )  
    {  
        case 0:  
            *pOutput     = *pInput;  
            utfbytes    += 1;  
            break;  
        case 2:  
            b1 = *pInput;  
            b2 = *(pInput + 1);  
            if ( (b2 & 0xE0) != 0x80 )  
                return 0;  
            *pOutput     = (b1 << 6) + (b2 & 0x3F);  
            *(pOutput+1) = (b1 >> 2) & 0x07;  
            break;  
        case 3:  
            b1 = *pInput;  
            b2 = *(pInput + 1);  
            b3 = *(pInput + 2);  
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )  
                return 0;  
            *pOutput     = (b2 << 6) + (b3 & 0x3F);  
            *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);  
            break;  
        case 4:  
            b1 = *pInput;  
            b2 = *(pInput + 1);  
            b3 = *(pInput + 2);  
            b4 = *(pInput + 3);  
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)  
                    || ((b4 & 0xC0) != 0x80) )  
                return 0;  
            *pOutput     = (b3 << 6) + (b4 & 0x3F);  
            *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);  
            *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);  
            break;  
        case 5:  
            b1 = *pInput;  
            b2 = *(pInput + 1);  
            b3 = *(pInput + 2);  
            b4 = *(pInput + 3);  
            b5 = *(pInput + 4);  
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)  
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )  
                return 0;  
            *pOutput     = (b4 << 6) + (b5 & 0x3F);  
            *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);  
            *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);  
            *(pOutput+3) = (b1 << 6);  
            break;  
        case 6:  
            b1 = *pInput;  
            b2 = *(pInput + 1);  
            b3 = *(pInput + 2);  
            b4 = *(pInput + 3);  
            b5 = *(pInput + 4);  
            b6 = *(pInput + 5);  
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)  
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)  
                    || ((b6 & 0xC0) != 0x80) )  
                return 0;  
            *pOutput     = (b5 << 6) + (b6 & 0x3F);  
            *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);  
            *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);  
            *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);  
            break;  
        default:  
            return 0;  
            break;  
    }  
  
    return utfbytes;  
}  

int unicode_to_utf8(uint16_t *in, int insize, uint8_t **out)  
{  
	int count = insize / sizeof(uint16_t);	
	uint8_t *result = NULL;
	result = (uint8_t *)malloc(count * 3 + 1);  
	memset(result, 0, count * 3 + 1);  
	int count_one = 6;
	unsigned char *result_one;
	int result_count = 0;
	//保证 pOutput 缓冲区有最少有 6 字节的空间大小
	result_one = (unsigned char *)malloc(count_one * sizeof(unsigned char));

	int j = 0;
	for(int i = 0;i < count;i++)
	{
		int temp = enc_unicode_to_utf8_one(in[i],result_one);			
		if (temp == 0) continue;
		//必须去掉字符串结束符
		for(int k = 0;k < temp;k ++)
		{
			result[j+k] = result_one[k];
		}		
		j += temp;		
	}
	result[j + 1] = '\0';
	*out = result;

	return j + 1;	
}  

int utf8_to_unicode(uint8_t *in, uint16_t **out, int *outsize)  
{  
	int count = strlen((int8_t *)in);
	
	uint16_t *result = NULL;
	result = (uint16_t *)malloc(strlen((int8_t *)in) * 2 + 2);  
	memset(result, 0, strlen((int8_t *)in) * 2 + 2);  
	int count_one = 0;
	int count_result = 0;
	unsigned long *result_one;
	result_one = (unsigned long*)malloc(sizeof(uint16_t)); //最大4个字节
	
	for(int i = 0,j=0;i < count;i ++)
	{
		count_one = enc_utf8_to_unicode_one(&in[i],result_one);
		if (count_one == 0) continue;		
		//刚刚好把字符串结束符\0去掉
		result[j++] = *result_one;
		count_result += count_one;
	}
	result[count] = '\0';
	*out = result;
	*outsize = count * 2 + 2;

	return 0;
} 

void MyNSString::CreateTCHARWithString(const char *src, int size, TCHART *dest)
{
	for(int i = 0 ; i < size ; i++)
	{
		dest[i] = src[i];
	}
}

int MyNSString::GBKToUTF8(string &gbkStr)
{
	iconv_t iconvH;
	iconvH = iconv_open("utf-8", "gb2312"); //iconv_open(to,from)
	if (iconvH == 0)
	{
		return -1;
	}
	size_t strLength = gbkStr.length();
	size_t outLength = strLength*4;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	const char* strChar = (gbkStr.c_str());
	const char** pin = &strChar;
#else if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	char* strChar = const_cast<char*>(gbkStr.c_str());
	char** pin = &strChar;
#endif

	//const char* strChar = (gbkStr.c_str());
	//const char** pin = &strChar;

	char *outbuf = (char *)malloc(outLength);
	char *pBuff = outbuf;
	memset(outbuf, 0, outLength);	

	if(-1 == iconv(iconvH, pin, &strLength, &outbuf, &outLength))
	{
		iconv_close(iconvH);
		return -1;
	}
	gbkStr = pBuff;
	iconv_close(iconvH);
	return 0;
}

int MyNSString::UTF8ToGBK(string &gbkStr)
{
	iconv_t iconvH;
	iconvH = iconv_open("gb2312", "utf-8");//iconv_open(to,from)
	if (iconvH == 0)
	{
		return -1;
	}
	size_t strLength = gbkStr.length();
	size_t outLength = strLength*4;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	const char* strChar = (gbkStr.c_str());
	const char** pin = &strChar;
#else if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	char* strChar = const_cast<char*>(gbkStr.c_str());
	char** pin = &strChar;
#endif

	//const char* strChar = (gbkStr.c_str());
	//const char** pin = &strChar;

	char *outbuf = (char *)malloc(outLength);
	char *pBuff = outbuf;
	memset(outbuf, 0, outLength);	

	if(-1 == iconv(iconvH, pin, &strLength, &outbuf, &outLength))
	{
		iconv_close(iconvH);
		return -1;
	}
	gbkStr = pBuff;
	iconv_close(iconvH);
	return 0;
}

string MyNSString::GBK2UTF8(const char* src)
{
	string str=src;
	GBKToUTF8(str);
	return str;
}

int MyNSString::wc2mbs(TCHART *wc,unsigned int wc_size,char *mbs)  
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		NSString *result = [NSString CreateStringWithTHCAR:wc size:wc_size];
		memcpy(mbs, [result UTF8String], [result length]*3+2);
		string str = mbs;
		CCLOG("%s", mbs);
		CCLOG("%d", [result length]);
		CCLOG("%d", strlen(mbs));
		return [result length];
	}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)//CC_PLATFORM_ANDROID
	int mbs_size;
#if (defined WIN32) ||  (defined _WIN32)  
	setlocale(LC_ALL,"chs");  
#else  
	setlocale(LC_ALL,"zh_CN.gbk");  
#endif  
	//#if 0
	if(wc_size==0)  
		wc_size=UINT_MAX;  

	mbs_size=wcstombs(0,wc,wc_size);  

	if(mbs!=0)  
		mbs_size=wcstombs(mbs,wc,wc_size); 

	string str = mbs;
	if(str != ""){
		MyNSString::GBKToUTF8(str);
		memcpy(mbs, str.c_str(), str.length());
	}

	return mbs_size; 
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	uint8_t *myMbs = (uint8_t *)malloc(255);
	memset(myMbs, 0, 255);
	uint16_t *myWc = (uint16_t *)malloc(wc_size);
	memset(myWc, 0, wc_size);
	memcpy(myWc,wc,wc_size); 
	//转成utf8即可，不用再转gbk
	int tempInt = unicode_to_utf8(myWc, wc_size, &myMbs);

	memcpy(mbs,myMbs,tempInt);

	CCLOG("%s",mbs);
	
	free(myMbs);
	free(myWc);

	return wc_size; 
#endif  		 
}

int MyNSString::mbs2wc(const char *mbs,unsigned int mbs_size,TCHART *wc)  
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		NSString *mbsString = [NSString stringWithUTF8String:mbs];
		[NSString CreateTCHARWithString:mbsString size:[mbsString length] TCHART:wc];
		return mbs_size;
	}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)//CC_PLATFORM_WIN32)
	char mbsOut[1024] = {0};
	//gb2312转utf8 调用iconv
	g2u(mbs,strlen(mbs),mbsOut,mbs_size);
	int wc_size; 

#if (defined WIN32) ||  (defined _WIN32)  
	setlocale(LC_ALL,"chs");  
#else  
	setlocale(LC_ALL,"zh_CN.gbk");  
#endif  

	if(mbs_size==0)  
		mbs_size=UINT_MAX;  

	wc_size=mbstowcs(0,mbsOut,mbs_size);  
	if(wc!=0)  
		wc_size=mbstowcs(wc,mbsOut,mbs_size);  

	return wc_size;  
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)		
	int wc_size;
	//cocos2dx内部处理string用的是utf8编码
	uint8_t *myMbs = (uint8_t *)malloc(1024);
	memset(myMbs, 0, 1024);
	memcpy(myMbs, mbs, mbs_size);//mbsOut->mbs 
	uint16_t *myWc = (uint16_t *)malloc(1024);
	memset(myWc, 0, 1024);
	//转成宽字节 才能存到数据库
	utf8_to_unicode(myMbs, &myWc, &wc_size);
	memcpy(wc, myWc, wc_size);	
	free(myMbs);		
	free(myWc);

	CCLOG("%s",wc);

	return wc_size; 

	
#endif
	
} 
