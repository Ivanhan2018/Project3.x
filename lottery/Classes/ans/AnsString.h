/*
** implement utility functions for c string or cpp string
** author: zhouwei
** date: 2012.09.18
*/


#ifndef __ANS_STRING_H__
#define __ANS_STRING_H__

#include <string>

using namespace std;

namespace ans{

// ---------------------------------------------------------------------------------
// AnsString
// ---------------------------------------------------------------------------------
namespace AnsString
{
	// -------------------------------------------------------------------
	// 判断一个字符串是否全部字符为数字
	// -------------------------------------------------------------------
	bool isDigit(const string& str);

	// -------------------------------------------------------------------
	// 将一个整型数值转换为字符串
	// -------------------------------------------------------------------
	string int2String(int value);

	// -------------------------------------------------------------------
	// 将一个浮点型数值转换为字符串
	// -------------------------------------------------------------------
	inline string float2String(float value);

	// -------------------------------------------------------------------
	// 将一个双精度型数值转换为字符串
	// -------------------------------------------------------------------
	string double2String(double value);

	// -------------------------------------------------------------------
	// 将字符串转换为整形
	// -------------------------------------------------------------------
	inline int string2Int(const string& str);

	// -------------------------------------------------------------------
	// 将字符串转换为双精度型
	// -------------------------------------------------------------------
	inline float string2Float(const string& str);

	// -------------------------------------------------------------------
	// 将字符串转换为浮点型
	// -------------------------------------------------------------------
	inline double string2Double(const string& str);


	// -------------------------------------------------------------------
	// 将字符串所有字符转换为大写
	// -------------------------------------------------------------------
	string toUpper(const string& str);
	string& toUpper(string& str);

	// -------------------------------------------------------------------
	// 将字符串所有字符转换为小写
	// -------------------------------------------------------------------
	string toLower(const string& str);
	string& toLower(string& str); 

	// -------------------------------------------------------------------
	// 判断一个字符串是否为空白串（即整个字符串皆为空格、\t、\n、\r）
	// -------------------------------------------------------------------
	inline bool isWhiteSpace(const string& str);

	// -------------------------------------------------------------------
	// 将一个字符串列表中的所有字符串与指定的字符串进行分隔组合
	// 如：
	//    char cs[][3] = {"abc", "def", "ghi"};
	//    vector<string> v(cs, cs+3);
	//    string resutl = join(", ", v.begin(), v.end());
	//    result 的值将为："abc, def, ghi"
	// 
	// 注意：len 包含结束字符“\0”的个数，也就是输出字符为 len - 1 个
	// -------------------------------------------------------------------
	template<typename Iter>
	std::string join(const std::string& sp, Iter begin, Iter end);

	// -------------------------------------------------------------------
	// 按指定分隔字符串拆分一个字符串
	// 参数：
	//    out    : 输出容器，可以是 vector<string>, list<string>, deque<string> 等
	//    str    : 要拆分的字符串
	//    sp     : 分隔符
	//    strip  : 是否削去每个子串两边的空格
	// -------------------------------------------------------------------
	template<template<typename, typename Alloc=std::allocator<string> > class Container>
	void split(Container<string>& out, const char* cstr, const char* sp, bool strip=false);

	template<template<typename, typename Alloc=std::allocator<string> > class Container>
	void split(Container<string>& out, const string& str, const char* sp, bool strip=false);

	template<template<typename, typename Alloc=std::allocator<string> > class Container>
	inline void split(Container<string>& out, const string& str, const string& sp, bool strip=false);

	template<template<typename, typename Alloc=std::allocator<string> > class Container>
	inline void split(Container<string>& out, const char* cstr, char sp, bool strip=false);

	template<template<typename, typename Alloc=std::allocator<string> > class Container>
	inline void split(Container<string>& out, const string& str, char sp, bool strip=false);

	// -------------------------------------------------------------------
	// 去掉字符串左边的空格
	// -------------------------------------------------------------------
	string ltrim(const string&);
	string& ltrim(string&);

	// -------------------------------------------------------------------
	// 去掉字符串右边的空格
	// -------------------------------------------------------------------
	string rtrim(const string&);
	string& rtrim(string&);

	// -------------------------------------------------------------------
	// 去掉字符串两边的空格
	// -------------------------------------------------------------------
	string trim(const string&str);
	string& trim(string&);


	// -------------------------------------------------------------------
	// 用新的字符串替换掉指定字符串中的所有子串
	// -------------------------------------------------------------------
	string& replace(string &str, const string& strOld, const string& strNew);
	string& replace(string &str, const char* cstrOld, const char* cstrNew);
	string replace(const string &str, const string& strOld, const string& strNew);
	string replace(const string &str, const char* cstrOld, const char* cstrNew);

	// -------------------------------------------------------------------
	// 判断两个字符串是否相等
	// 参数：
	//     src      : 源字符串
	//     dst      : 目标字符串
	//     ignorCase: 是否忽略大小写
	// -------------------------------------------------------------------
	bool equal(const string& src, const string& dst, bool ignorCase=false);
	bool equal(const string& src, const char* cdst, bool ignorCase=false);
}


// ---------------------------------------------------------------------------------
// AnsCString（C 字符串）
// ---------------------------------------------------------------------------------
namespace AnsCString
{
	// -------------------------------------------------------------------
	// 判断一个字符串是否全部字符为数字
	// -------------------------------------------------------------------
	bool isDigit(const char* cstr);

	// -------------------------------------------------------------------
	// 将字符串转换为整形
	// -------------------------------------------------------------------
	int string2Int(const char* cstr);

	// -------------------------------------------------------------------
	// 将字符串转换为浮点型
	// -------------------------------------------------------------------
	inline float string2Float(const char* cstr);

	// -------------------------------------------------------------------
	// 将字符串转换为双精度型
	// -------------------------------------------------------------------
	double string2Double(const char* cstr);


	// -------------------------------------------------------------------
	// 将字符串所有字符转换为大写
	// -------------------------------------------------------------------
	void toUpper(char* buff, size_t size, const char* cstr);

	// -------------------------------------------------------------------
	// 将字符串所有字符转换为小写
	// -------------------------------------------------------------------
	void toLower(char* buff, size_t size, const char* cstr);

	// -------------------------------------------------------------------
	// 判断某字符串是否为空白字符串（即整个字符串皆为空格、\t、\n、\r）
	// -------------------------------------------------------------------
	bool isWhiteSpace(const char* cstr);

	// -------------------------------------------------------------------
	// 判断某字符串是否以指定的字符串开头
	// -------------------------------------------------------------------
	bool isStartsWidth(const char* cstr, const char* sub);

	// -------------------------------------------------------------------
	// 判断某字符串是否以指定的字符串结尾
	// -------------------------------------------------------------------
	bool isEndsWidth(const char* cstr, const char* sub);

	// -------------------------------------------------------------------
	// 用指定字符画一根 len - 1 个字符长度的线，保存到 buff 中
	// 注意：len 包含结束字符“\0”的个数，也就是输出字符为 len - 1 个
	// -------------------------------------------------------------------
	inline void makeCharLine(char buff[], char chr, unsigned len);

	// -------------------------------------------------------------------
	// 去掉字符串左边的空格
	// -------------------------------------------------------------------
	void ltrim(char* buff, size_t size, const char* cstr);

	// -------------------------------------------------------------------
	// 去掉字符串右边的空格
	// -------------------------------------------------------------------
	void rtrim(char* buff, size_t size, const char* cstr);

	// -------------------------------------------------------------------
	// 去掉字符串两边的空格
	// -------------------------------------------------------------------
	void trim(char* buff, size_t size, const char* cstr);


	// -------------------------------------------------------------------
	// 替换某字符串的指定子串
	// 参数：
	//    buff   : 替换后的输出新串
	//    size   : 替换后输出新串的缓冲长度
	//    cstr   : 需要被替换的源字符串
	//    cstrOld: 要被替换的子串
	//    cstrNew: 要替换的新子串
	// -------------------------------------------------------------------
	void replace(char* buff, size_t size, const char* cstr, const char* cstrOld, const char* cstrNew);
}

}

#include "AnsString.ipp"
#include "AnsString.tpp"

#endif