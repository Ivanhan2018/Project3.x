/*
** implement utility inline functions for c string or cpp string
** author: zhouwei
** date: 2012.09.18
*/

#include <string.h>		// memset(...)

namespace ans{

// ---------------------------------------------------------------------------------
// AnsString
// ---------------------------------------------------------------------------------
inline string AnsString::float2String(float value)
{
	return double2String(value);
}

inline int AnsString::string2Int(const string& str)
{
	return AnsCString::string2Int(str.c_str());
}

inline float AnsString::string2Float(const string& str)
{
	return (float)string2Double(str);
}

inline double AnsString::string2Double(const string& str)
{
	return AnsCString::string2Double(str.c_str());
}

inline bool AnsString::isWhiteSpace(const string& str)
{
	return AnsCString::isWhiteSpace(str.c_str());
}

// -----------------------------------------------------------------------
inline bool AnsString::equal(const string& src, const string& dst, bool ignorCase)
{
	return ignorCase ? toLower(src).compare(dst) == 0 : src.compare(dst) == 0;
}

inline bool AnsString::equal(const string& src, const char* cdst, bool ignorCase)
{
	return ignorCase ? toLower(src).compare(cdst) == 0 : src.compare(cdst) == 0;
}

// ---------------------------------------------------------------------------------
// AnsCString£¨C ×Ö·û´®£©
// ---------------------------------------------------------------------------------
inline float AnsCString::string2Float(const char* cstr)
{
	return (float)string2Double(cstr);
}

inline void AnsCString::makeCharLine(char buff[], char chr, unsigned len)
{
	memset(buff, chr, (size_t)len);
	buff[len-1] = 0;
}

}