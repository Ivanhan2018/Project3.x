#include "DZPKTools.h"
// #include "iconv.h"


/*
int GBKToUTF8(std::string & gbkStr, const char* toCode, const char* fromCode)
{
	iconv_t iconvH;
	iconvH = iconv_open(fromCode, toCode);


	if (iconvH == 0)


	{
		return -1;
	}


	const char* strChar = gbkStr.c_str();
	const char** pin = &strChar;
	size_t strLength = gbkStr.length();
	char* outbuf = (char*) malloc(strLength*4);
	char* pBuff = outbuf;


	memset( outbuf, 0, strLength*4);
	size_t outLength = strLength*4;


	if (-1 == iconv(iconvH, pin, &strLength, &outbuf, &outLength))
	{
		iconv_close(iconvH);
		return -1;
	}
	gbkStr = pBuff;
	iconv_close(iconvH);
	return 0;
}
*/

const char* DZPK_loadText(const char* key)
{
	string path = "dzpk_text.xml";//dzpk-jhy
	TinyXmlReader m_TinyXmlReader;
	TinyXMLDocument* doc = m_TinyXmlReader.open(path);
	return doc->readString(key);
	return NULL;
}