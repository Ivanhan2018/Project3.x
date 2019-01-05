/*
 
 ˽��
 �����������˽�з���
 
 */
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
	static void CreateTCHARWithString(const char *src, int size, TCHART *dest);
	static int wc2mbs(TCHART *wc,unsigned int wc_size, char *mbs);
	// PC下有bug
	static int mbs2wc(const char *mbs,unsigned int mbs_size,TCHART *wc);

	// Android下有崩溃性bug
	static string ws2s(const wstring& ws);
	// Android下有bug
	static wstring s2ws(const string& s);

private:

};