#ifndef XDATE_H 
#define XDATE_H  
#include "stdio.h"
#include "stdlib.h"

#include "GlobalDef.h"
#include "GlobalUnits.h"
#include "cocos2d.h"
#include "time.h"
using namespace cocos2d;
using namespace std;

#ifndef MAKELONG
typedef DWORD DWORD_PTR;

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

#endif // !MAKELONG



extern const int START_YEAR; 
extern const int END_YEAR ; 

class xDate 
{ 
private: 
   int    m_iYear, m_iMonth, m_iDay; 
   void    l_InitData(); 
   //计算从1901年1月1日过iSpanDays天后的阴历日期    
   static void   l_CalcLunarDate(int &iYear, int &iMonth ,int &iDay,int iSpanDays); 
   //计算公历iYear年iMonth月iDay日对应的节气 0-24，0表不是节气 
   static int   l_GetLunarHolDay(int iYear, int iMonth, int iDay); 
public:  
   //=====================================================================================// 
   xDate(int iYear, int iMonth, int iDay);  
   xDate(); 
   //=====================================================================================// 

   int GetYear(){return m_iYear;}  
   int GetMonth(){return m_iMonth;} 
   int GetDay(){return m_iDay;}  
    
   //=====================================================================================// 
   void GetDate(int &iYear, int &iMonth, int &iDay);  
   bool SetDate(int iYear , int iMonth , int iDay); 
   //=====================================================================================// 
   //判断iYear是不是闰年  
    static bool IsLeapYear(int iYear)  {return !(iYear%4)&&(iYear%100) || !(iYear%400);}  
    
   //计算iYear,iMonth,iDay对应是星期几 1年1月1日 --- 65535年12月31日  
   static int WeekDay(int iYear, int iMonth, int iDay); 
    
   //返回iYear年iMonth月的天数 1年1月 --- 65535年12月  
   static int MonthDays(int iYear, int iMonth);  
    
   //返回阴历iLunarYer年阴历iLunarMonth月的天数，如果iLunarMonth为闰月， 
   //高字为第二个iLunarMonth月的天数，否则高字为0  
   // 1901年1月---2050年12月  
   static int LunarMonthDays(int iLunarYear, int iLunarMonth);  
    
   //返回阴历iLunarYear年的总天数 
   // 1901年1月---2050年12月  
   static int LunarYearDays(int iLunarYear);  
    
   //返回阴历iLunarYear年的闰月月份，如没有返回0  
   // 1901年1月---2050年12月 
   static int GetLeapMonth(int iLunarYear);  
    
   //把iYear年格式化成天干记年法表示的字符串  
   static void FormatLunarYear(int  iYear, char *pBuffer);  

   //把iMonth格式化成中文字符串  
   static void FormatMonth(int iMonth, char *pBuffer, bool bLunar = true);  
    
   //把iDay格式化成中文字符串  
   static void FormatLunarDay(int  iDay, char *pBuffer); 
    
   //计算公历两个日期间相差的天数  1年1月1日 --- 65535年12月31日  
   static int CalcDateDiff(int iEndYear, int iEndMonth, int iEndDay,int iStartYear = START_YEAR,int iStartMonth =1, int iStartDay =1);  
    
   //计算公历iYear年iMonth月iDay日对应的阴历日期,返回对应的阴历节气 0-24 
   //1901年1月1日---2050年12月31日  
   static int GetLunarDate(int iYear, int iMonth, int iDay,int &iLunarYear, int &iLunarMonth, int &iLunarDay); 
};  

#endif //XDATE_H 