#ifndef  _TOOLS_H_
#define  _TOOLS_H_


//中文转码


#include "cocos2d.h"
#include "AnsSingleton.h"
#include "TinyXmlReader.h"

#pragma comment(lib, "libiconv.lib")


//中文文本资源
const char* loadText(const char* key);

#endif