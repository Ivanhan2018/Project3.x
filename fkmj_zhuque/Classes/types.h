#ifndef _types_H_
#define _types_H_

//////////////////////////////////////////////////////////////
#include <assert.h>
#include <stdio.h>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <deque>
#include <iomanip>
#include <list>
#include <sstream>

#include <stdlib.h>

//#include <basetyps.h>
#include <assert.h>
//#include <tchar.h>

#ifndef ASSERT
#define ASSERT(f) assert(f)
#endif

///////////////////////////////////////////////////////////////////////////////////////

typedef signed char			int8;
typedef signed short		int16;
typedef signed int			int32;
typedef long long			int64;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;

typedef unsigned char		byte;
typedef unsigned int		uint;
typedef unsigned short		word;
typedef unsigned int		dword;
typedef long long			longlong;
#define SCORE				longlong							
#define TRUE				1
#define FALSE				0

typedef wchar_t				wchar;

#ifdef  _UNICODE
typedef std::wstring	tstring;
typedef wchar			tchar;
#else
typedef std::string		tstring;
typedef char			tchar;
#endif

#ifdef _WIN32
#define LLSTRING "%I64d"
#else
#define LLSTRING "%lld"
#endif

//////////////////////////////////////////////////////////////////////////
// 功能函数
inline const char* toString(int value)
{
	static std::string str;
	str.resize(128, '\0');
	sprintf(&str[0], "%d", value);
	return str.c_str();
}

inline const char* toString(int64 value)
{
	static std::string str;
	str.resize(128, '\0');
	sprintf(&str[0], LLSTRING, value);
	return str.c_str();
}

template<typename T>
inline T tmin(const T& a, const T& b)
{
	return a < b ? a : b;
}

template<typename T>
inline T tmax(const T& a, const T& b)
{
	return a < b ? b : a;
}

template<class T>
inline bool removeVectorByValue(std::vector<T>& a, const T& b)
{
	typename std::vector<T>::iterator itor = a.begin();
	for (;itor != a.end();itor++)
	{
		if((*itor) == b)
		{
			a.erase(itor);
			return true;
		}
	}
	return false;
}

template<class T>
inline bool haveInVectorByValue(std::vector<T>& a, const T& b)
{
	typename std::vector<T>::iterator itor = a.begin();
	for (;itor != a.end();itor++)
	{
		if((*itor) == b)
		{
			return true;
		}
	}
	return false;
}


// inline const std::wstring& toStringW(int value)
// {
// 	static std::wstring str;
// #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	
// 	str.resize(128, L'\0');
// 	_snwprintf(&str[0], 128, L"%d", value);
// 	return str;
// #endif
// 	return str;
// }
// 
// static std::string w2s(const std::wstring& ws)
// {
// #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	std::string curLocale = setlocale(LC_ALL, 0);        // curLocale = "C";
// 	setlocale(LC_ALL, "chs");
// 	size_t len = ws.size() * 2 + 1;
// 	std::string result;
// 	result.resize(len, '\0');
// 
// 	wcstombs_s(&len, &result[0], len, ws.c_str(), len);
// 	setlocale(LC_ALL, curLocale.c_str());
// 	return result;
// #endif
// 	std::string str;
// 	return str;
// }

// static std::wstring s2w(const std::string& s)
// {
// #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 	std::string curLocale = setlocale(LC_ALL, "chs"); 
// 	size_t len = s.size() + 1;
// 	std::wstring result;
// 	result.resize(len, L'\0');
// 	mbstowcs_s(&len, &result[0], len, s.c_str(),len);
// 	setlocale(LC_ALL, curLocale.c_str());
// 	return result;
// #endif
// 	std::wstring ret;
// 	return ret;
// }

//////////////////////////////////////////////////////////////////////////
// 宏定义
#define countarray(ary)		(sizeof(ary)/sizeof(ary[0]))
#define zeromemory(x, size)	memset(x, 0, size)

// #ifdef  _UNICODE
// #define t2s(text)		w2s(text)
// #define t2w(text)		tstring(text)
// #define s2t(text)		s2w(text)
// #define w2t(text)		tstring(text)
// #define T_T(str)		L##str
// //#define toString		toStringW
// 
// #define tstrcpyn(dst, src, len)	wcscpy_s(dst, len, src) 
// #define tstrcpy(dst, src)		wcscpy(dst, src) 
// #define tstrlen(str)			wcslen(str)
// #define tstrcmp(str1, str2)		wcscmp(str1, str2)
// 
// #else
// #define t2s(text)		tstring(text)
// #define t2w(text)		s2w(text)
// #define s2t(text)		tstring(text)
// #define w2t(text)		s2w(text)
// #define T_T(str)		str
// #define toString		toStringA
// 
// #define tstrcpyn(dst, src, len)	strcpy_s(dst, len, src) 
// #define tstrcpy(dst, src)		strcpy(dst, src) 
// #define tstrlen(str)			strlen(str)
// #define tstrcmp(str1, str2)		strcmp(str1, str2)
// #endif



///////////////////////////////////////////////////////////////////////////////////////

#endif // _types_H_