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
	static int mbs2wc(const char *mbs,unsigned int mbs_size,TCHART *wc);

private:

};