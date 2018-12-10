#ifndef _Convert_H_
#define _Convert_H_
#include <vector>
#include <string>

#include "cocos2d.h"


namespace utility
{

	int utf8_len(std::string utf8);
	std::string a_u8(std::string gbk);
	std::string u8_a(std::string utf8);
} 

#endif // _Convert_H_