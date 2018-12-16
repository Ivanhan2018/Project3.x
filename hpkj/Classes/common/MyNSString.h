#pragma once
#include "Define.h"
#include <string>
using namespace std;
//#pragma mark -
//#pragma mark NSString(NSStringMD5)

class MyNSString
{
public:
	static int GBKToUTF8(string &gbkStr);
	static int UTF8ToGBK(string &gbkStr);
	static string GBK2UTF8(const char* src);
	static void CreateTCHARWithString(const char *src, int size, TCHART *dest);
	static int wc2mbs(TCHART *wc,unsigned int wc_size, char *mbs);
	static int mbs2wc(const char *mbs,unsigned int mbs_size,TCHART *wc);

    // ×ª»»UTF8×Ö·û´®µ½UTF16×Ö·û´®
    static void UTF8Str_To_UTF16Str( const std::string &str, WORD* pwszUTF16Str );  
    static int UTF8Str_To_UTF16Str_BYTE( const BYTE* pbszUTF8Str, WORD* pwszUTF16Str );
	// ×ª»»UTF16×Ö·û´®µ½UTF8×Ö·û´®
	static std::string WHConverUnicodeToUtf8WithArray(WORD srcUnicode[]);

private:

};

#define G2U(a) MyNSString::GBK2UTF8(a).c_str()