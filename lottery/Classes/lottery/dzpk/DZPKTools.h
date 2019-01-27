#ifndef _DZPKTOOLS_H_
#define  _DZPKTOOLS_H_


//中文转码


#include "cocos2d.h"
#include "AnsSingleton.h"
#include "TinyXmlReader.h"

#pragma comment(lib, "libiconv.lib")


/*
int GBKToUTF8(std::string & gbkStr, const char* toCode, const char* fromCode);
*/

//中文文本资源
const char* DZPK_loadText(const char* key);

#endif